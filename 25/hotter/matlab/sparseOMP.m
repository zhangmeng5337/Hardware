function [s, err_mse, iter_time]=sparseOMP(x,A,m,varargin)
[n1 n2]=size(x);
if n2 == 1
    n=n1;
elseif n1 == 1
    x=x';
    n=n2;
else
   error('x must be a vector.');
end
    
sigsize     = x'*x/n;
initial_given=0;  
err_mse     = [];
iter_time   = [];
STOPCRIT    = 'M';
STOPTOL     = ceil(n/4);
MAXITER     = n;
verbose     = false;
s_initial   = zeros(m,1);
GradSteps   = 'auto';
alpha       = 1;
weakness    = 1;

if verbose
   display('Initialising...') 
end

switch nargout 
    case 3
        comp_err=true;
        comp_time=true;
    case 2 
        comp_err=true;
        comp_time=false;
    case 1
        comp_err=false;
        comp_time=false;
    case 0
        error('Please assign output variable.')        
    otherwise
        error('Too many output arguments specified')
end


% Put option into nice format
Options={};
OS=nargin-3;
c=1;
for i=1:OS
    if isa(varargin{i},'cell')
        CellSize=length(varargin{i});
        ThisCell=varargin{i};
        for j=1:CellSize
            Options{c}=ThisCell{j};
            c=c+1;
        end
    else
        Options{c}=varargin{i};
        c=c+1;
    end
end
OS=length(Options);
if rem(OS,2)
   error('Something is wrong with argument name and argument value pairs.') 
end
for i=1:2:OS
   switch Options{i}
        case {'stopCrit'}
            if (strmatch(Options{i+1},{'M'; 'corr'; 'mse'; 'mse_change'},'exact'));
                STOPCRIT    = Options{i+1};  
            else error('stopCrit must be char string [M, corr, mse, mse_change]. Exiting.'); end 
        case {'stopTol'}
            if isa(Options{i+1},'numeric') ; STOPTOL     = Options{i+1};   
            else error('stopTol must be number. Exiting.'); end
        case {'P_trans'} 
            if isa(Options{i+1},'function_handle'); Pt = Options{i+1};   
            else error('P_trans must be function _handle. Exiting.'); end
        case {'maxIter'}
            if isa(Options{i+1},'numeric'); MAXITER     = Options{i+1};             
            else error('maxIter must be a number. Exiting.'); end
        case {'wf'}
            if isa(Options{i+1},'numeric'); alpha       = Options{i+1}; 
                if alpha <1 weakness =0; else alpha =1; weakness = 1; end          
            else error('wf must be a number. Exiting.'); end
        case {'verbose'}
            if isa(Options{i+1},'logical'); verbose     = Options{i+1};   
            else error('verbose must be a logical. Exiting.'); end 
        case {'start_val'}
            if isa(Options{i+1},'numeric') && length(Options{i+1}) == m ;
                s_initial     = Options{i+1};   
                initial_given=1;
            else error('start_val must be a vector of length m. Exiting.'); end
        case {'GradSteps'}
            if isa(Options{i+1},'numeric') || strcmp(Options{i+1},'auto') ;
                GradSteps     = Options{i+1};   
            else error('start_val must be a vector of length m. Exiting.'); end
        otherwise
            error('Unrecognised option. Exiting.') 
   end
end




if strcmp(STOPCRIT,'M') 
    maxM=STOPTOL;
else
    maxM=MAXITER;
end

if nargout >=2
    err_mse = zeros(maxM,1);
end
if nargout ==3
    iter_time = zeros(maxM,1);
end


if          isa(A,'float')      P =@(z) A*z;  Pt =@(z) A'*z;
elseif      isobject(A)         P =@(z) A*z;  Pt =@(z) A'*z;
elseif      isa(A,'function_handle') 
    try
        if          isa(Pt,'function_handle'); P=A;
        else        error('If P is a function handle, Pt also needs to be a function handle. Exiting.'); end
    catch error('If P is a function handle, Pt needs to be specified. Exiting.'); end
else        error('P is of unsupported type. Use matrix, function_handle or object. Exiting.'); end

if initial_given ==1;
    IN          = find(s_initial);
    Residual    = x-P(s_initial);
    s           = s_initial;
    oldERR      = Residual'*Residual/n;
else
    IN          = [];
    Residual    = x;
    s           = s_initial;
    sigsize     = x'*x/n;
    oldERR      = sigsize;
end

        mask=zeros(m,1);
        mask(ceil(rand*m))=1;
        nP=norm(P(mask));
        if abs(1-nP)>1e-3;
            display('Dictionary appears not to have unit norm columns.')
        end

if verbose
   display('Main iterations...') 
end
tic
t=0;
normA=(sum(A.^2,1)).^0.5;
NI = 1:size(A,2);
p=zeros(m,1);
DR=Pt(Residual);
[v I]=max(abs(DR(NI))./normA(NI)');
INI = NI(I);
IN=[IN INI];
NI(I) = [];
if weakness ~= 1
    [vals inds] = sort(abs(DR),'descend');
    I=inds( find( vals >= alpha * v ) );
end
    
IN = union(IN,I);
if strcmp(STOPCRIT,'M') & length(IN) >= STOPTOL
    IN=IN(1:STOPTOL);
end
MASK=zeros(size(DR));
pDDp=1;
done = 0;
iter=1;

while ~done

    
    % Select new element
    if isa(GradSteps,'char')
        if strcmp(GradSteps,'auto')
             
%             finished=0;    
%             while ~finished
            % Update direction    
                 if iter==1
                     p(IN)=DR(IN);
                     Dp=P(p);
                 else
                     MASK(IN)=1;
                     PDR=P(DR.*MASK);
                     b=-Dp'*PDR/pDDp;
                     p(IN)=DR(IN) +b*p(IN);
                     Dp=PDR +b* Dp;
                 end
             % Step size
%                  Dp=P(p); % =P(DR(IN)) +b P(p(IN));
                 pDDp=Dp'*Dp;
                 a=Residual'*Dp/(pDDp);
             % Update coefficients   
                 s=s+a*p;
             % New Residual and inner products
                 Residual=Residual-a*Dp;
                 DR=Pt(Residual);
                 % select new element
                     [v I]=max(abs(DR(NI))./normA(NI)');
                     INI = NI(I);
                     if weakness ~= 1
                         [vals inds] = sort(abs(DR),'descend');
                         I=inds( find( vals >= alpha * v ) );
                     end
                     IN = union(IN,INI);
                     NI(I) = [];
                     if strcmp(STOPCRIT,'M') & length(IN) >= STOPTOL
                        IN=IN(1:STOPTOL);
                     end


        else
            
            
            error('Undefined option for GradSteps, use ''auto'' or an integer.')
        end
    elseif isa(GradSteps,'numeric') 

                
        % Do GradSteps gradient steps
        count=1;
        while count<=GradSteps
            % Update direction    
                 if iter==1
                     p(IN)=DR(IN);
                     Dp=P(p);
                 else
                     MASK(IN)=1;
                     PDR=P(DR.*MASK);
                     b=-Dp'*PDR/pDDp;
                     p(IN)=DR(IN) +b*p(IN);
                     Dp=PDR +b* Dp;
                 end
             % Step size
%                  Dp=P(p);   
                 pDDp=Dp'*Dp;
                 a=Residual'*Dp/(pDDp);
             % Update coefficients   
                 s=s+a*p;
             % New Residual and inner products
                 Residual=Residual-a*Dp;
                 DR=Pt(Residual);
                  count=count+1;
        end
             % select new element
                 [v I]=max(abs(DR(NI))./normA(NI)');
                 INI = NI(I);
                 if weakness ~= 1
                     [vals inds] = sort(abs(DR),'descend');
                     I=inds( find( vals >= alpha * v ) );
                 end
                 IN = union(IN,INI);
                  NI(I) = [];
                 if strcmp(STOPCRIT,'M') & length(IN) >= STOPTOL
                    IN=IN(1:STOPTOL);
                 end
                
     else
          error('Undefined option for GradSteps, use ''auto'' or an integer.')
     end


     ERR=Residual'*Residual/n;
     if comp_err
         err_mse(iter)=ERR;
     end
     
     if comp_time
         iter_time(iter)=toc;
     end


     if strcmp(STOPCRIT,'M')
         if length(IN) >= STOPTOL
             done =1;
         elseif verbose && toc-t>10
            display(sprintf('Iteration %i. --- %i selected elements',iter ,length(IN))) 
            t=toc;
         end
    elseif strcmp(STOPCRIT,'mse')
         if comp_err
            if err_mse(iter)<STOPTOL;
                done = 1; 
            elseif verbose && toc-t>10
                display(sprintf('Iteration %i. --- %i mse',iter ,err_mse(iter))) 
                t=toc;
            end
         else
             if ERR<STOPTOL;
                done = 1; 
             elseif verbose && toc-t>10
                display(sprintf('Iteration %i. --- %i mse',iter ,ERR)) 
                t=toc;
             end
         end
     elseif strcmp(STOPCRIT,'mse_change') && iter >=2
         if comp_err && iter >=2
              if ((err_mse(iter-1)-err_mse(iter))/sigsize <STOPTOL);
                done = 1; 
             elseif verbose && toc-t>10
                display(sprintf('Iteration %i. --- %i mse change',iter ,(err_mse(iter-1)-err_mse(iter))/sigsize )) 
                t=toc;
             end
         else
             if ((oldERR - ERR)/sigsize < STOPTOL);
                done = 1; 
             elseif verbose && toc-t>10
                display(sprintf('Iteration %i. --- %i mse change',iter ,(oldERR - ERR)/sigsize)) 
                t=toc;
             end
         end
     elseif strcmp(STOPCRIT,'corr') 
          if max(abs(DR)) < STOPTOL;
             done = 1; 
          elseif verbose && toc-t>10
                display(sprintf('Iteration %i. --- %i corr',iter ,max(abs(DR)))) 
                t=toc;
          end
     end
     
    % Also stop if residual gets too small or maxIter reached
     if comp_err
         if err_mse(iter)<1e-16
             display('Stopping. Exact signal representation found!')
             done=1;
         end
     else


         if iter>1
             if ERR<1e-16
                 display('Stopping. Exact signal representation found!')
                 done=1;
             end
         end
     end

     if iter >= MAXITER
         display('Stopping. Maximum number of iterations reached!')
         done = 1; 
     end
     
   
     if ~done
        iter=iter+1;
        oldERR=ERR;
     end
end



if nargout >=2
    err_mse = err_mse(1:iter);
end
if nargout ==3
    iter_time = iter_time(1:iter);
end
if verbose
   display('Done') 
end
-----------------------------------------------------------------------
-- ʹ��˵����                                                        --
-- V1.0                                                              --
-- �û���Ҫ�������һ��2��������ʵ��                                 --
-- 1����ʱ�·����������ʼ��������device_timer_init(dev)����ѡ��     --
-- 2�����豸�ϴ����ݽ��н��������������ȣ���device_data_analyze(dev) --
-----------------------------------------------------------------------


-------------------------------------------------------
-- ע��C����                                         --
-- u2f ��32λ�����ڴ�����ת��Ϊ����������ͬ��ֵת����--
-- ����C���� *(float*)(&u)                           --
-- function utf(u)                                   --
-- @param   u   number   ����ֵ                      --
-- @return  �ɹ����ظ�����ֵ�����򷵻�nil            --
-- @example local u = 123                            --
--          local f = u2f( 123 )                     --
-------------------------------------------------------

--------------------------------------------------------
-- ��bytes stringת��hex string                       --
-- @param   s   string   bytes string                 --
-- @return  ����hex string������"0A0B0C0D..."         --
-- @example local hex = to_hex("\2\2\0\150\0\37\206") --
--------------------------------------------------------
function to_hex(s)
	local i
	local t

	t={s:byte(1,s:len())}
	for i=1,#t do
		t[i]=string.format('%02X',t[i])
	end

	return table.concat(t)
end

-----------------------------------------------
-- ��object���л����ַ���                    --
-- @param   o   boolean|number|string|table  --
-- @return  �������л�string                 --
-- @example local str = to_str({x=100})      --
-----------------------------------------------
function to_str(o)
	local i=1
	local t={}
	local f

	f=function(x)
		local y=type(x)
		if y=="number" then
			t[i]=x
			i=i+1
		elseif y=="boolean" then
			t[i]=tostring(x)
			i=i+1
		elseif y=="string" then
			t[i]="\""
			t[i+1]=x
			t[i+2]="\""
			i=i+3
		elseif y=="table" then
			t[i]="{"
			i=i+1

			local z=true
			for k,v in pairs(x) do
				if z then
					z=false
					t[i]="\""
					t[i+1]=k
					t[i+2]="\""
					t[i+3]=":"
					i=i+4
					f(v)
				else
					t[i]=","
					t[i+1]="\""
					t[i+2]=k
					t[i+3]="\""
					t[i+4]=":"
					i=i+5
					f(v)
				end
			end

			t[i]="}"
			i=i+1
		else
			t[i]="nil"
			i=i+1
		end
	end
	f(o)

	return table.concat(t)
end

----------------------------------------------------------------------------------------------------------
-- ���ֵ���ݵ㵽table��                                                                                --
-- @param   t   table                                                                                   --
--          i   string                      ��������������ģ������                                      --
--          a   number                      ���뼶ʱ��������루00:00:00 UTC, January 1, 1970���ĺ��룻 --
--                                          ���ֵΪ0����ʾʹ�õ�ǰʱ��                                 --
--          v   boolean|number|string|table ����ֵ����ֵ���ַ�����json                                  --
-- @return  �ɹ�����true�����򷵻�false                                                                 --
-- @example local ok = add_val(t,"dsname",0,100)                                                        --
----------------------------------------------------------------------------------------------------------
function add_val(t, i, a, v)
	if type(t)~="table" then
		return false
	elseif type(i)~="string" then
		return false
	elseif type(a)~="number" then
		return false
	else
		local o = type(v)
		if o~="boolean" and o~="number" and o~="string" and o~="table" then
			return false
		end

		local n = {i=i,v=v}
		if a~=0 then
			n["a"]=a
		end

		-- list push_back --
		if t.h==nil then
			t.h={nil,n}
			t.t=t.h
		else
			t.t[1]={nil,n}
			t.t=t.t[1]
		end
	end

	return true
end

----------------------------------------------------------------------------------------------------------
-- ��Ӷ��������ݵ㵽table��                                                                            --
-- @param   t   table                                                                                   --
--          i   string                      ��������������ģ������                                      --
--          a   number                      ���뼶ʱ��������루00:00:00 UTC, January 1, 1970���ĺ��룻 --
--                                          ���ֵΪ0����ʾʹ�õ�ǰʱ��                                 --
--          b   string                      ���������ݣ�hex string��������"0A0B0C0D..."                 --
--          d   boolean|number|string|table ��������b����ѡ������ֵ���ַ�����json                       --
-- @return  �ɹ�����true�����򷵻�false                                                                 --
-- @example local ok = add_val(t,"dsname",0,"0A0B0C0D...",{...})                                        --
----------------------------------------------------------------------------------------------------------
function add_bin(t, i, a, b, d)
	if type(t)~="table" then
		return false
	elseif type(i)~="string" then
		return false
	elseif type(a)~="number" then
		return false
	elseif type(b)~="string" then
		return false
	else
		local o=type(d)
		if o~="nil" and o~="string" and o~="table" then
			return false
		end

		local n={i=i,b=to_hex(b)}
		if a~=0 then
			n["a"]=a
		end
		if d~=nil then
			n["d"]=d
		end

		-- list push_back --
		if t.h==nil then
			t.h={nil,n}
			t.t=t.h
		else
			t.t[1]={nil,n}
			t.t=t.t[1]
		end
	end

	return true
end

--------------------------------------------------------------
-- ��table���л���json�ַ���                                --
-- @param   t   table   ͨ��add_val��add_bin����������table --
-- @return  �������л�json�ַ���                            --
-- @example local json = to_json(t)                         --
--------------------------------------------------------------
function to_json(t)
	local i=1
	local o={}
	local n

	o[i]="["
	i=i+1
	n=t.h
	while n~=nil do
		if n[2]~=nil then
			o[i]=to_str(n[2])
			i=i+1
		end

		n=n[1]
		if n~=nil then
			o[i]=","
			i=i+1
		end
	end
	o[i]="]"

	return table.concat(o)
end

------------------------------------
-- begin-����û��Զ���ֵ������ --

-- end-����û��Զ���ֵ������   --
------------------------------------

------------------------------------------------------------------------------------------
-- ���ö�ʱ�·��豸�����ݣ���ѡ��                                             --
-- @param  dev    user_data   �豸������                                                --
-- @return ��                                                                           --
-- @notice �˺���Ϊ�ص������������ڽű��ڵ���                                           --
-- @readme dev�ṩһ�¼���������                                                        --
--         dev:add(interval,name,data)��Ӷ�ʱ�·�����                                  --
--           @param   interval   number   �����·���ʱ�������룩                      --
--                    name       string   ���ƣ��뱣֤Ψһ�ԣ�                          --
--                    data       string   ���ݣ����������ݣ���ʹ��luaת���ַ���         --
--           @return  �ɹ�����true�����򷵻�false                                       --
--           @notice  ��ʱ�����·���ƽ��Ƶ�ʲ�����1����1/interval_1+...+1/interval_n<=1 --
--           @example local ok = dev:add(10,��test��,��\1\1\0\150\0\37\253\29��)        --
--         dev:timeout(sec)�����·����ݵ��豸��Ӧ��ʱʱ�䣨�룩                         --
--           @param   sec        int      ��Ӧ��ʱʱ�䣨�룩                            --
--           @return  ��                                                                --
--           @example dev:timeout(3)                                                    --
--         dev:response()�豸��Ӧ�ɹ�                                                   --
--           @param   ��                                                                --
--           @return  ��                                                                --
--           @example dev:response()                                                    --
--         dev:send(data)�·����ݵ��豸                                                 --
--           @param   data   string   ���ݣ����������ݣ���ʹ��luaת���ַ���             --
--           @return  ��                                                                --
--           @example dev:send("\2\2\0\150\0\37\206\89")                                --
------------------------------------------------------------------------------------------
function device_timer_init(dev)
	-- ����û��Զ������ --
	-- ���磺 --
	dev:timeout(3)
	--dev:add(10,"heart beat","heart beat\r\n")--
end

-----------------------------------------------------------------------------------------------------------
-- �����豸�ϴ�����                                                                                      --
-- @param  dev    user_data   �豸������                                                                 --
-- @return size��ʾ�ѽ����豸�ϴ����ݵ��ֽ�����json��ʾ����������ݵ㼯�ϣ���ʽ���£�                    --
--         [                                                                                             --
--           {                                                                                           --
--             ��i�� : ��dsname1��,         // ��������������ģ������1                                   --
--             ��a�� : 1234567890,          // ���뼶ʱ��������루00:00:00 UTC, January 1, 1970���ĺ��� --
--                                          // ���ֵΪ0����ʾʹ�õ�ǰʱ��                               --
--             ��v�� : 123 | ��123�� | {��} // ����ֵ����ֵ���ַ�����json                                --
--             ��b�� : ��0A0B0C0D����       // ���������ݣ�16�����ַ���������v���⣬��ͬʱ����           --
--             ��d�� : xxx | ��xxx�� | {��} // ��������b����ѡ��������ֵ����ֵ���ַ�����json             --
--           }                                                                                           --
--           ��                                                                                          --
--           {                                                                                           --
--             ��i�� : ��dsnamen��,         // ��������������ģ������n                                   --
--             ��a�� : 1234567890,          // ���뼶ʱ��������루00:00:00 UTC, January 1, 1970���ĺ��� --
--                                          // ���ֵΪ0����ʾʹ�õ�ǰʱ��                               --
--             ��v�� : 123 | ��123�� | {��} // ����ֵ����ֵ���ַ�����json                                --
--             ��b�� : ��0A0B0C0D����       // ���������ݣ�16�����ַ���������v���⣬��ͬʱ����           --
--             ��d�� : xxx | ��xxx�� | {��} // ��������b����ѡ��������ֵ����ֵ���ַ�����json             --
--           }                                                                                           --
--         ]                                                                                             --
-- @notice �˺���Ϊ�ص������������ڽű��ڵ���                                                            --
-- @readme dev�ṩһ�¼���������                                                                         --
--         dev:add(interval,name,data)��Ӷ�ʱ�·�����                                                   --
--           @param   interval number   �����·���ʱ�������룩                                       --
--                    name     string   ���ƣ��뱣֤Ψһ�ԣ�                                           --
--                    data     string   ���ݣ����������ݣ���ʹ��luaת���ַ���                          --
--           @return  �ɹ�����true�����򷵻�false                                                        --
--           @notice  ��ʱ�����·���ƽ��Ƶ�ʲ�����1����1/interval_1+...+1/interval_n<=1                  --
--           @example local ok = dev:add(10,��test��,��\1\1\0\150\0\37\253\29��)                         --
--         dev:timeout(sec)�����·����ݵ��豸��Ӧ��ʱʱ�䣨�룩                                          --
--           @param   sec      int      ��Ӧ��ʱʱ�䣨�룩                                             --
--           @return  ��                                                                                 --
--           @example dev:timeout(3)                                                                     --
--         dev:response()�豸��Ӧ�ɹ�                                                                    --
--           @param   ��                                                                                 --
--           @return  ��                                                                                 --
--           @example dev:response()                                                                     --
--         dev:send(data)�·����ݵ��豸                                                                  --
--           @param   data   string   ���ݣ����������ݣ���ʹ��luaת���ַ���                              --
--           @return  ��                                                                                 --
--           @example dev:send("\2\2\0\150\0\37\206\89")                                                 --
--         dev:size()��ȡ�豸���ݴ�С���ֽ�����                                                          --
--           @param   ��                                                                                 --
--           @return  �����豸���ݴ�С���ֽ�����                                                         --
--           @example local sz = dev:size()                                                              --
--         dev:byte(pos)��ȡpos��Ӧλ�õ��豸���ݣ��ֽڣ�                                                --
--           @param   pos   number   ָ���Ļ�ȡλ�ã�ȡֵ��Χ[1,dev:size()+1)                            --
--           @return  �ɹ������豸���ݣ�int�������򷵻�nil                                               --
--           @example local data = dev:byte(1)                                                           --
--         dev:bytes(pos,count)��ȡ��pos��ʼ��count���豸����                                            --
--           @param   pos   number   ָ���Ļ�ȡ��ʼλ�ã�ȡֵ��Χ[1,dev:size()+1)                        --
--                    count number   ָ���Ļ�ȡ����������ȡֵ��Χ[0,dev:size()+1-pos]                    --
--           @return  �ɹ������豸���ݣ�string�������򷵻�nil                                            --
--           @example local datas = dev:bytes(1,dev:size())                                              --
-----------------------------------------------------------------------------------------------------------
function device_data_analyze(dev)
	local t={}
	local a=0

	-- ����û��Զ������ --
	-- ���磺 --
	local s = dev:size()
	add_bin(t,"ds_test",a,dev:bytes(1,s) )
	dev:response()
	dev:send("received")

	-- return $1,$2 --
	-- ���磺 --
	return s,to_json(t)
end
function s = filterWaveletTh(data,wname,SORH,lev,tptr,options)
% ʹ��С����ֵ��ʵ���˲������ļ���Ϊ����ʽͳһ������õ�һ���װ������ʵ����kwden.m��kwdencmp.m��kwthresh.m��kthselect.m�ļ���
% ��4���ļ���Ϊ�����˵�������ĺ����ļ���ԭ�����ֱ�Ϊwden,wdencmp,wthresh,thselect�����Թ����⼸������������ܿ��Բ鿴��Ӧ�Ĺٷ������ĵ�
% ���룺
% data�����˲�����
% wname��С�����ƣ���ѡ��Χ�ο����https://ww2.mathworks.cn/help/wavelet/ref/wfilters.html?searchHighlight=wname&s_tid=srchtitle_wname_2#d123e130597
% SORH����ֵ��������
%       sorh ='a1'ʱ�����øĽ�����1���Ľ��������ֵ��
%                     �ο����ģ�������, ����. ���ڸĽ�������ֵС��������ĵ����ź�����[J]. �������̴�ѧѧ��, 2019(4).
%       sorh = 'a2'ʱ�����øĽ�����2��
%                     �ο����ģ�����, ������, �˽��,��. ����VMD��Ľ�С����ֵ�ľַ��ź�ȥ��[J]. �ִ����Ӽ���, 2021, 44(21):6.
%       sorh = 'a3'ʱ�����øĽ�����4��
%                     �ο����ģ����ڸĽ�С����ֵ-CEEMDAN�㷨��ECG�ź�ȥ���о�
%       sorh = 'a4'ʱ�����øĽ�����3��
%                     �ο����ģ����ڸĽ�С����ֵ������������ǿ�㷨�о�
%                     ���ø÷�����Ҫ���������������ӣ��ֱ���alpha��gamma��ȡalpha>0��0<gamma<1
%       sorh = 'a5'ʱ�����øĽ�����3��
%                     �ο����ģ�����VMD��Ľ�С����ֵ�ĵ����ź�ȥ�뷽���о�
%                     ���ø÷�����Ҫ���������������ӣ��ֱ���alpha��beta% lev��  С���ֽ�ˮƽ
% tptr�� ��ֵѡ������п�ѡ���ͣ�
%       '' �� Adaptive threshold selection using the principle of Stein's Unbiased Risk Estimate (SURE).
%       'sqtwolog' �� Fixed-form threshold is sqrt(2*log(length(X))).
%       'heursure' �� Heuristic variant of 'rigrsure' and 'sqtwolog'.
%       'minimaxi' �� Minimax thresholding.
%       'visushrink' - ͨ����ֵȥ�뷽��
% options��������ֵ������Ҫ����Ĳ������ã���ĳЩ�������ӣ���Ҫ�ýṹ�巽ʽ���÷�ֵ����options.a4_alpha=2���������Ҫ���ã����Ը�ֵΪoptions=[]���������£�
%          -a4_alpha���Ľ�����4��alphaֵ����
%          -a4_gamma���Ľ�����4��gammaֵ����
%          -a5_alpha���Ľ�����5��alphaֵ����
%          -a5_beta���Ľ�����5��betaֵ����
% �����
% s�������˲���õ�������
% ���۽������https://zhuanlan.zhihu.com/p/579187348/
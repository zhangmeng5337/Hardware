clear all;
clc;
Tone=audioread('Z:\\E\prj\code\sound_200-2000.wav')
%close all;
time1 =3; %��һ������Ƭ�ε���ʱ��
vol1 = 120; %��һ������Ƭ�ε�����
fs =44100; %����Ƶ��

T = 1/fs; %��������
% Tone = 0;
%      for freq=150:1:500
%     % freq = 1000
%     tone1 = sin(2*pi*freq*(0:T:time1))*(db2mag(vol1)); 
%     Tone = [Tone,tone1]; %�����������Ƭ��
%     end
sound(Tone,fs) %���Բ��������ĺ��� sound()

%�洢.wav��Ƶ�ļ�

filename = ('test.wav'); %���ļ�ȡ��

% audiowrite(filename,Tone,fs) %�洢.wav��Ƶ�ļ����������ļ���Ϊtest.wav

% fs = 44100; %����Ƶ��
% 
% T = 1/fs; %��������
% 
% time1 = 105; %��һ������Ƭ�ε���ʱ��
% 
% time2 = 0.1;
% 
% time3 = 0.25;
% 
% freq1 = 261; %��һ������Ƭ�ε�Ƶ�ʣ�������
% 
% freq2 = 0;
% 
% freq3 = 370;
% 
% vol1 = 80; %��һ������Ƭ�ε�����
% 
% vol2 = 0;
% 
% vol3 = 80;
% 
% tone1 = sin(2*pi*freq1*(0:T:time1))*(db2mag(vol1)); %���ɵ�һ������Ƭ�Σ�ע����Ҫ��db2mag()������dBת����magnitude�������ù�ʽ���˶�����������Χ�ķֱ���׼10^((vol-85)/20)ת����
% 
% tone2 = sin(2*pi*freq2*(0:T:time2))*(db2mag(vol2));
% 
% tone3 = sin(2*pi*freq3*(0:T:time3))*(db2mag(vol3));
% 
% Tone = [tone1, tone2, tone3]; %�����������Ƭ��

% sound(Tone,fs) %���Բ��������ĺ��� sound()
% 
% %�洢.wav��Ƶ�ļ�
% 
% filename = ('test.wav'); %���ļ�ȡ��
% 
% audiowrite(filename,Tone,fs) %�洢.wav��Ƶ�ļ����������ļ���Ϊtest.wav
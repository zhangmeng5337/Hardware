clear all;
clc;
Tone=audioread('Z:\\E\prj\code\sound_200-2000.wav')
%close all;
time1 =3; %第一个声音片段的总时长
vol1 = 120; %第一个声音片段的音量
fs =44100; %采样频率

T = 1/fs; %采样周期
% Tone = 0;
%      for freq=150:1:500
%     % freq = 1000
%     tone1 = sin(2*pi*freq*(0:T:time1))*(db2mag(vol1)); 
%     Tone = [Tone,tone1]; %组合所有声音片段
%     end
sound(Tone,fs) %可以播放声音的函数 sound()

%存储.wav音频文件

filename = ('test.wav'); %给文件取名

% audiowrite(filename,Tone,fs) %存储.wav音频文件，在这里文件名为test.wav

% fs = 44100; %采样频率
% 
% T = 1/fs; %采样周期
% 
% time1 = 105; %第一个声音片段的总时长
% 
% time2 = 0.1;
% 
% time3 = 0.25;
% 
% freq1 = 261; %第一个声音片段的频率（音调）
% 
% freq2 = 0;
% 
% freq3 = 370;
% 
% vol1 = 80; %第一个声音片段的音量
% 
% vol2 = 0;
% 
% vol3 = 80;
% 
% tone1 = sin(2*pi*freq1*(0:T:time1))*(db2mag(vol1)); %生成第一个声音片段，注意需要用db2mag()函数把dB转换成magnitude，或者用公式以人耳听力正常范围的分贝标准10^((vol-85)/20)转换。
% 
% tone2 = sin(2*pi*freq2*(0:T:time2))*(db2mag(vol2));
% 
% tone3 = sin(2*pi*freq3*(0:T:time3))*(db2mag(vol3));
% 
% Tone = [tone1, tone2, tone3]; %组合所有声音片段

% sound(Tone,fs) %可以播放声音的函数 sound()
% 
% %存储.wav音频文件
% 
% filename = ('test.wav'); %给文件取名
% 
% audiowrite(filename,Tone,fs) %存储.wav音频文件，在这里文件名为test.wav
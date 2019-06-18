#!/usr/bin/python
# -*-coding: utf-8 -*-

import serial
import threading
import binascii
from datetime import datetime
import struct
import csv

class SerialPort:
    def __init__(self, port, buand):
        self.port = serial.Serial(port, buand)
        self.port.close()
        if not self.port.isOpen():
            self.port.open()

    def port_open(self):
        if not self.port.isOpen():
            self.port.open()

    def port_close(self):
        self.port.close()

    def send_data(self):
        self.port.write('')

    def read_data(self):
        global is_exit
        global data_bytes
        while not is_exit:
            count = self.port.inWaiting()
            if count > 0:
                rec_str = self.port.read(count)
                data_bytes=data_bytes+rec_str
                #print('当前数据接收总字节数：'+str(len(data_bytes))+' 本次接收字节数：'+str(len(rec_str)))
                print(str(datetime.now()),':',binascii.b2a_hex(rec_str))


serialPort = 'COM21'  # 串口
baudRate = 9600  # 波特率
is_exit=False
data_bytes=bytearray()

if __name__ == '__main__':
    #打开串口
    mSerial = SerialPort(serialPort, baudRate)

    #文件写入操作
    filename=input('请输入文件名：比如test.csv:')
    dt=datetime.now()
    nowtime_str=dt.strftime('%y-%m-%d %I-%M-%S')  #时间
    filename=nowtime_str+'_'+filename
    out=open(filename,'a+')
    csv_writer=csv.writer(out)

    #开始数据读取线程
    t1 = threading.Thread(target=mSerial.read_data)
    t1.setDaemon(True)
    t1.start()
    
    while not is_exit:
        #主线程:对读取的串口数据进行处理
        data_len=len(data_bytes)
        i=0
        while(i<data_len-1):
            if(data_bytes[i]==0xFF and data_bytes[i+1]==0x5A):
                frame_code=data_bytes[i+2]
                frame_len=struct.unpack('<H',data_bytes[i+4:i+6])[0]
                frame_time=struct.unpack('<I',data_bytes[i+6:i+10])[0]
                print('帧类型：',frame_code,'帧长度：',frame_len,'时间戳：',frame_time)
                #print(frame_code,frame_len,frame_time)
                if frame_code==0x03:   #判断帧类型
                    #struct 解析数据帧
                    accelerated_x,accelerated_y,accelerated_z,angular_x,angular_y,angular_z,tem,speed_x,speed_y,speed_z,\
                    angular_v_x,angular_v_y,angular_v_z=struct.unpack('<fffffffffffff',data_bytes[i+12:i+12+frame_len-6])
                    dt=datetime.now()
                    nowtime_str=dt.strftime('%y-%m-%d %I:%M:%S')  #时间
                    loc_str=[nowtime_str,frame_time,accelerated_x,accelerated_y,accelerated_z,angular_x,angular_y,angular_z,tem,speed_x,speed_y,speed_z,\
                    angular_v_x,angular_v_y,angular_v_z]

                    #写入csv文件
                    try:
                       csv_writer.writerow(loc_str)
                    except Exception as e:
                       raise e       
                i=i+6+frame_len+3
            else:
                i=i+1
        data_bytes[0:i]=b''

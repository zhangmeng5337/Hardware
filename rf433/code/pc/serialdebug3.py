#encoding = utf-8
import serial
import threading
from tkinter import *
from tkinter import ttk
import serial.tools.list_ports
from tkinter import messagebox
import time
 
root = Tk()
root.geometry("880x700+0+0")
root.title("PVBox校正软件")
mySerial = serial.Serial()
Open = StringVar()
Send = StringVar()
data = ''
'''=================================Frame=================================='''
frame1 = LabelFrame(root, text="串口配置", width=600, height=80)
frame1.grid(row=0, column=0, sticky=W, padx=10)
 
frame = Frame(root, width=600, height=450)
frame.grid(row=1, column=0, sticky=W, padx=10)
 
frame2 = LabelFrame(frame, text="接收区", width=600, height=450)
frame2.grid(row=0, column=0, sticky=W)
 
frame3 = LabelFrame(frame, text="发送区", width=600, height=100)
frame3.grid(row=1, column=0, sticky=W)
 
frame4 = Frame(root, width=600, height=50)
frame4.grid(row=3, column=0, sticky=E)
 
frame5 = Frame(root, width=250, height=600)
frame5.grid(row=1, column=1, sticky=N)
 
frame6 = LabelFrame(frame5, text="设置区", width=250, height=100)
frame6.grid(row=0, column=0, sticky=S)
 
frame7 = LabelFrame(frame5, text="校正区", width=250, height=500)
frame7.grid(row=1, column=0, sticky=S)
 
'''============================================Frame2===================================='''
Text_get = Text(frame2, height=44)
Text_get.grid(row=0, column=0)
'''===================================Frame1================================='''
Label_port = ttk.Label(frame1, text="串口号")
Label_port.grid(row=0, column=0)
ComPort = ttk.Combobox(frame1, width=8, height=10)
ComPort['values'] = ['COM1', 'COM2', 'COM3', 'COM4', 'COM5', 'COM6', 'COM7', 'COM8']
ComPort.current(2)
ComPort.grid(row=0, column=1)
 
Label_baud = ttk.Label(frame1, text="波特率")
Label_baud.grid(row=0, column=2)
ComBaud = ttk.Combobox(frame1, width=8, height=10)
ComBaud['values'] = ['2400', '4800', '9600', '14400', '19200']
ComBaud.current(2)
ComBaud.grid(row=0, column=3)

'''======================================检查串口按钮============================'''
def CheckPort():
    port_list = list(serial.tools.list_ports.comports())
    ComPort['values'] = [i[0] for i in port_list]
BtnPort = ttk.Button(frame1, text="检查串口", width=9, command=CheckPort)
BtnPort.grid(row=0, column=6)
'''=======================================打开串口按钮============================'''
#获取数据
def ReceiveData():
    print("The receive_data threading is start")
    while mySerial.isOpen():
        wait = mySerial.inWaiting()
        if wait and data:
            res_data = mySerial.readline()
            Text_get.insert(END, res_data + bytes('\n', encoding='utf8'))
            mySerial.flushInput()
 
def OpenPort():
    if not mySerial.isOpen():
        try:
            mySerial.port = ComPort.get()
            mySerial.baudrate = int(ComBaud.get())
            mySerial.timeout = 0.1
            mySerial.open()
            Open.set('关闭串口')
            if mySerial.isOpen():
                t = threading.Thread(target=ReceiveData)
                t.setDaemon(True)
                t.start()
        except:
            messagebox.showinfo('错误提示', '出错啦，请检查！')
    else:
        mySerial.close()
        Open.set('打开串口')
Open.set('打开串口')
BtnOpen = ttk.Button(frame1, textvariable=Open, width=9, command=OpenPort)
BtnOpen.grid(row=0, column=7)

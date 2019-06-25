import tkinter as tk
import serial
import threading
import time
import binascii
import struct

from binascii import a2b_hex
from threading import Event
from time import ctime,sleep
from  serial.tools import list_ports
from tkinter import ttk
from tkinter import *
from tkinter import messagebox
win = Tk()
win.geometry("700x500+500+200")
win.title("串口配置工具")
mySerial = serial.Serial()
Open = StringVar()
Send = StringVar()
data = ''
#*****************************UI*********************************************
frameUpDown = LabelFrame(win, text="上下键功能设定", width=300, height=80)
frameUpDown.grid(row=0, column=0, sticky=W,padx=10,pady=7)
ttk.Label(frameUpDown,text="上下关系").grid(column=50,row=0)#增加标签
ttk.Label(frameUpDown,text="上UP").grid(sticky = W,column=0,row=0)#增加up标签
ttk.Label(frameUpDown,text="下DOWN").grid(sticky = W,column=0,row=1)#增加DOWN标签
up= tk.StringVar()#增加up下拉菜单
cmbChosenUp = ttk.Combobox(frameUpDown,width=8, height=10)
cmbChosenUp['value']=('点动','自锁')
cmbChosenUp.grid(column = 1, row = 0)
cmbChosenUp.current(0)
down= tk.StringVar()#增加down下拉菜单
cmbChosenDown = ttk.Combobox(frameUpDown,width=8, height=10)
cmbChosenDown['value']=('点动','自锁')
cmbChosenDown.grid(column = 1, row = 1,padx=10,pady=7)
cmbChosenDown.current(0)
UpDown= tk.StringVar()#增加上下关系下拉菜单
cmbChosenUpDown = ttk.Combobox(frameUpDown,width=8, height=10)
cmbChosenUpDown['value']=('相互拟制','不拟制')
cmbChosenUpDown.grid(column = 50, row = 1,padx=30,pady=7)
cmbChosenUpDown.current(0)


#增加frame******************东西
frameEW = ttk.LabelFrame(win,text = "东西键功能设定", width=300, height=80)
frameEW.grid(row=1, column=0, sticky=W,padx=10,pady=7)
ttk.Label(frameEW,text="东西关系").grid(column=50,row=1)#增加东西关系标签
ttk.Label(frameEW,text="东EAST").grid(sticky = W,column=0,row=1)#增加东标签
ttk.Label(frameEW,text="西WEST").grid(sticky = W,column=0,row=2)#增加西WEST标签
EAST= tk.StringVar()#增加EAST下拉菜单
cmbChosenEAST = ttk.Combobox(frameEW,width=8, height=10)
cmbChosenEAST['value']=('点动','自锁')
cmbChosenEAST.grid(column = 1, row = 1,padx=17)
cmbChosenEAST.current(0)
WEST= tk.StringVar()#增加WEST下拉菜单
cmbChosenWEST = ttk.Combobox(frameEW,width=8, height=10)
cmbChosenWEST['value']=('点动','自锁')
cmbChosenWEST.grid(column = 1, row =2,padx=10,pady=7)
cmbChosenWEST.current(0)
EAST_WEST= tk.StringVar()
cmbChosenEAST_WEST = ttk.Combobox(frameEW,width=8, height=10)#增加东西关系下拉菜单
cmbChosenEAST_WEST['value']=('相互拟制','不拟制')
cmbChosenEAST_WEST.grid(column = 50, row = 2,padx=24,pady=7)
cmbChosenEAST_WEST.current(0)

frameNS = ttk.LabelFrame(win,text = "南北键功能设定", width=300, height=80)
frameNS.grid(row=2, column=0, sticky=W,padx=10,pady=7)
ttk.Label(frameNS,text="南北关系").grid(column=50,row=1)#增加东西关系标签
ttk.Label(frameNS,text="南SOUTH").grid(sticky = W,column=0,row=1)#增加东标签
ttk.Label(frameNS,text="北NORTH").grid(sticky = W,column=0,row=2)#增加西WEST标签
SOUTH= tk.StringVar()#增加EAST下拉菜单
cmbChosenSOUTH = ttk.Combobox(frameNS,width=8, height=10)
cmbChosenSOUTH['value']=('点动','自锁')
cmbChosenSOUTH.grid(column = 1, row = 1,padx=5)
cmbChosenSOUTH.current(0)
NORTH= tk.StringVar()#增加WEST下拉菜单
cmbChosenNORTH = ttk.Combobox(frameNS,width=8, height=10)
cmbChosenNORTH['value']=('点动','自锁')
cmbChosenNORTH.grid(column = 1, row =2,padx=5,pady=7)
cmbChosenNORTH.current(0)
SOUTH_NORTH= tk.StringVar()
cmbChosenSOUTH_NORTH = ttk.Combobox(frameNS,width=8, height=10)#增加东西关系下拉菜单
cmbChosenSOUTH_NORTH['value']=('相互拟制','不拟制')
cmbChosenSOUTH_NORTH.grid(column = 50, row = 2,padx=36,pady=7)
cmbChosenSOUTH_NORTH.current(0)



frameSTART_STOP = ttk.LabelFrame(win,text = "开停机键功能设定", width=300, height=80)
frameSTART_STOP.grid(row=4, column=0, sticky=W,padx=10,pady=7)
ttk.Label(frameSTART_STOP,text="开START").grid(column=0,row=1)#增加东西关系标签
ttk.Label(frameSTART_STOP,text="停STOP").grid(sticky = W,column=0,row=2)#增加东标签

START= tk.StringVar()#增加EAST下拉菜单
cmbChosenSTART = ttk.Combobox(frameSTART_STOP,width=8, height=10)
cmbChosenSTART['value']=('备用点动','备用自锁')
cmbChosenSTART.grid(column = 1, row = 1,padx=10)
cmbChosenSTART.current(0)
STOP= tk.StringVar()#增加WEST下拉菜单
cmbChosenSTOP = ttk.Combobox(frameSTART_STOP,width=8, height=10)
cmbChosenSTOP['value']=('点动','自锁')
cmbChosenSTOP.grid(column = 1, row =2,padx=10,pady=7)
cmbChosenSTOP.current(0)


frameOUT1_OUT2 = ttk.LabelFrame(win,text = "OUT1  OUT2键功能设定", width=300, height=80)
frameOUT1_OUT2.grid(row=5, column=0, sticky=W,padx=10,pady=7)
ttk.Label(frameOUT1_OUT2,text="OUT1 OUT2关系").grid(column=50,row=1)#增加东西关系标签
ttk.Label(frameOUT1_OUT2,text="OUT1").grid(sticky = W,column=0,row=1)#增加东标签
ttk.Label(frameOUT1_OUT2,text="OUT2").grid(sticky = W,column=0,row=2)#增加西WEST标签
OUT1= tk.StringVar()#增加EAST下拉菜单
cmbChosenOUT1 = ttk.Combobox(frameOUT1_OUT2,width=8, height=10)
cmbChosenOUT1['value']=('点动','自锁')
cmbChosenOUT1.grid(column = 1, row = 1,padx=25)
cmbChosenOUT1.current(0)
OUT2= tk.StringVar()#增加WEST下拉菜单
cmbChosenOUT2 = ttk.Combobox(frameOUT1_OUT2,width=8, height=10)
cmbChosenOUT2['value']=('点动','自锁')
cmbChosenOUT2.grid(column = 1, row =2,padx=25,pady=7)
cmbChosenOUT2.current(0)
OUT1_OUT2 = tk.StringVar()
cmbChosenOUT1_OUT2 = ttk.Combobox(frameOUT1_OUT2,width=8, height=10)#增加东西关系下拉菜单
cmbChosenOUT1_OUT2['value']=('相互拟制','不拟制')
cmbChosenOUT1_OUT2.grid(column = 50, row = 2,padx=21,pady=7)
cmbChosenOUT1_OUT2.current(0)


frameOUT3_OUT4 = ttk.LabelFrame(win,text = "OUT3  OUT4键功能设定", width=300, height=80)
frameOUT3_OUT4.grid(row=5, column=81, sticky=W,padx=10,pady=7)
ttk.Label(frameOUT3_OUT4,text="OUT3 OUT4关系").grid(column=50,row=1)#增加东西关系标签
ttk.Label(frameOUT3_OUT4,text="OUT3").grid(sticky = W,column=0,row=1)#增加东标签
ttk.Label(frameOUT3_OUT4,text="OUT4").grid(sticky = W,column=0,row=2)#增加西WEST标签
OUT3= tk.StringVar()#增加EAST下拉菜单
cmbChosenOUT1 = ttk.Combobox(frameOUT3_OUT4,width=8, height=10)
cmbChosenOUT1['value']=('点动','自锁')
cmbChosenOUT1.grid(column = 1, row = 1,padx=25)
cmbChosenOUT1.current(0)
OUT4= tk.StringVar()#增加WEST下拉菜单
cmbChosenOUT2 = ttk.Combobox(frameOUT3_OUT4,width=8, height=10)
cmbChosenOUT2['value']=('点动','自锁')
cmbChosenOUT2.grid(column = 1, row =2,padx=25,pady=7)
cmbChosenOUT2.current(0)
OUT3_OUT4 = tk.StringVar()
cmbChosenOUT3_OUT4 = ttk.Combobox(frameOUT3_OUT4,width=8, height=10)#增加东西关系下拉菜单
cmbChosenOUT3_OUT4['value']=('相互拟制','不拟制')
cmbChosenOUT3_OUT4.grid(column = 50, row = 2,padx=21,pady=7)
cmbChosenOUT3_OUT4.current(0)


###增加frame******************SN_CH功能设定
frameSN_CH = ttk.LabelFrame(win,height = 175,width =300,text = "读写SN、CH码")
frameSN_CH.grid(column = 81, row = 2,sticky = E+W)
frameSN_CH.place(relx=0,rely=0,x=328,y=195)
#增加frame******************格式化数据卡
frameFORMAT = ttk.LabelFrame(win,height = 80 ,width =300,text = "数据卡格式化")
frameFORMAT.grid(column = 81, row = 1)
frameFORMAT.place(relx=0,rely=0,x=328,y=100)

#Entry控件布局
ttk.Label(frameSN_CH,text="SN:").grid(column=1,row=1,padx = 30,pady =20)
ttk.Label(frameSN_CH,text="CH:").grid(column=1,row=5,padx = 30,pady =40)
entrySN=Entry(frameSN_CH,width = 10,state = 'normal')#sn码
entryCH=Entry(frameSN_CH,width = 10,state = 'normal')#ch码
entrySN.place(relx=0,rely=0,x=70,y=20)
entryCH.place(relx=0,rely=0,x=70,y=100)

# 创建button
ReadParams = ttk.Button(frameSN_CH,width = 8,text="读参数")
ReadParams.grid(column = 12, row = 0,sticky=W)
ReadParams.place(relx=0,rely=0,x=200,y=15)
WriteParams = ttk.Button(frameSN_CH,width = 8,text="下载参数")
WriteParams.place(relx=0,rely=0,x=200,y=95)



frameCOMX = tk.LabelFrame(win,height = 80,width = 300,text = "通讯参数")
frameCOMX.grid(column = 81, row = 0)
frameCOMX.place(relx=0,rely=0,x=328,y=5)
COMX= tk.StringVar(frameCOMX,'COM1')#增加COM下拉菜单
cmbChosenCOMX = ttk.Combobox(frameCOMX,state  = 'readonly',width=6,textvariable = COMX)
cmbChosenCOMX.grid(column = 1, row = 0,padx=5,sticky=N+S)
cmbChosenCOMX.place(relx=0,rely=0,x=10,y=12)

COMOFF =  ttk.Button(frameCOMX,width = 8,text="打开串口", textvariable=Open)
COMOFF.grid(column = 12, row = 0,sticky=W)
COMOFF.place(relx=0,rely=0,x=110,y=10)
WAKECOM =  ttk.Button(frameCOMX,width = 8,text="唤醒串口")
WAKECOM.place(relx=0,rely=0,x=215,y=10)



FORMAT = ttk.Button(frameFORMAT,width = 36,text="数据卡格式化")
FORMAT.grid(column = 12, row = 0)
FORMAT.place(relx=0,rely=0,x=11,y=12)

frameUpDown.grid_propagate(0)
frameEW.grid_propagate(0)
frameNS.grid_propagate(0)
frameSTART_STOP.grid_propagate(0)
frameOUT1_OUT2.grid_propagate(0)
frameOUT3_OUT4.grid_propagate(0)
frameSN_CH.grid_propagate(0)
frameFORMAT.grid_propagate(0)
frameCOMX.grid_propagate(0)



#*************************应用*************************************************
'''======================================检查串口按钮============================'''
def CheckPort(self):
    port_list = list(serial.tools.list_ports.comports())
    cmbChosenCOMX['values'] = [i[0] for i in port_list]
cmbChosenCOMX.bind("<Button-1>",CheckPort)

#获取数据
def ReceiveData():
    print("The receive_data threading is start")
    while mySerial.isOpen():
        wait = mySerial.inWaiting()
        if wait :
            res_data = mySerial.readline()
            Text_get.insert(END, res_data + bytes('\n', encoding='utf8'))
            print('ReceiveData',res_data)
            mySerial.flushInput()
def OpenPort(self):
    if not mySerial.isOpen():
        try:
            mySerial.port = cmbChosenCOMX.get()           #设置端口号
            mySerial.baudrate = 9600     #设置波特率
            mySerial.bytesize = 8        #设置数据位
            mySerial.stopbits = 1        #设置停止位
            mySerial.parity = "N"        #设置校验位
            mySerial.timeout = 0.1
            mySerial.open()
            Open.set('关闭串口')
            if mySerial.isOpen():
                t = threading.Thread(target=ReceiveData)
                t.setDaemon(True)
                t.start()
        except:
            info = sys.exc_info()
            print('info',info[0],":",info[1])
            messagebox.showinfo('错误提示', '出错啦，请检查！')
    else:
        mySerial.close()
        Open.set('打开串口')
Open.set('打开串口')
COMOFF.bind("<Button-1>",OpenPort)


win.mainloop()

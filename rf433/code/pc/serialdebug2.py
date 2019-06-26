import tkinter as tk
import serial
import threading
import time
import binascii
import struct
import crcmod.predefined
from binascii import a2b_hex
from threading import Event
from time import ctime,sleep
from  serial.tools import list_ports
from tkinter import ttk
from tkinter import *
from tkinter import messagebox
txBuffer = bytearray(32)
res_data = b''
keystatusH = 0
keystatusL = 0
keyInhibitonNum = 0
PortOpenFlag = 0
win = Tk()
win.geometry("700x500+1200+200")
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
UP= tk.StringVar()#增加up下拉菜单
cmbChosenUp = ttk.Combobox(frameUpDown,width=8, height=10,textvariable =UP)
cmbChosenUp['value']=('点动','自锁')
cmbChosenUp.grid(column = 1, row = 0)
cmbChosenUp.current(0)
DOWN= tk.StringVar()#增加down下拉菜单
cmbChosenDown = ttk.Combobox(frameUpDown,width=8, height=10,textvariable =DOWN)
cmbChosenDown['value']=('点动','自锁')
cmbChosenDown.grid(column = 1, row = 1,padx=10,pady=7)
cmbChosenDown.current(0)
UpDown= tk.StringVar()#增加上下关系下拉菜单
cmbChosenUpDown = ttk.Combobox(frameUpDown,width=8, height=10,textvariable =UpDown)
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
cmbChosenEAST = ttk.Combobox(frameEW,width=8, height=10,textvariable = EAST)
cmbChosenEAST['value']=('点动','自锁')
cmbChosenEAST.grid(column = 1, row = 1,padx=17)
cmbChosenEAST.current(0)
WEST= tk.StringVar()#增加WEST下拉菜单
cmbChosenWEST = ttk.Combobox(frameEW,width=8, height=10,textvariable = WEST)
cmbChosenWEST['value']=('点动','自锁')
cmbChosenWEST.grid(column = 1, row =2,padx=10,pady=7)
cmbChosenWEST.current(0)
EAST_WEST= tk.StringVar()
cmbChosenEAST_WEST = ttk.Combobox(frameEW,width=8, height=10,textvariable =EAST_WEST)#增加东西关系下拉菜单
cmbChosenEAST_WEST['value']=('相互拟制','不拟制')
cmbChosenEAST_WEST.grid(column = 50, row = 2,padx=24,pady=7)
cmbChosenEAST_WEST.current(0)

frameNS = ttk.LabelFrame(win,text = "南北键功能设定", width=300, height=80)
frameNS.grid(row=2, column=0, sticky=W,padx=10,pady=7)
ttk.Label(frameNS,text="南北关系").grid(column=50,row=1)#增加东西关系标签
ttk.Label(frameNS,text="南SOUTH").grid(sticky = W,column=0,row=1)#增加东标签
ttk.Label(frameNS,text="北NORTH").grid(sticky = W,column=0,row=2)#增加西WEST标签
SOUTH= tk.StringVar()#增加EAST下拉菜单
cmbChosenSOUTH = ttk.Combobox(frameNS,width=8, height=10,textvariable =SOUTH )
cmbChosenSOUTH['value']=('点动','自锁')
cmbChosenSOUTH.grid(column = 1, row = 1,padx=5)
cmbChosenSOUTH.current(0)
NORTH= tk.StringVar()#增加WEST下拉菜单
cmbChosenNORTH = ttk.Combobox(frameNS,width=8, height=10,textvariable =NORTH)
cmbChosenNORTH['value']=('点动','自锁')
cmbChosenNORTH.grid(column = 1, row =2,padx=5,pady=7)
cmbChosenNORTH.current(0)
SOUTH_NORTH= tk.StringVar()
cmbChosenSOUTH_NORTH = ttk.Combobox(frameNS,width=8, height=10,textvariable =SOUTH_NORTH)#增加东西关系下拉菜单
cmbChosenSOUTH_NORTH['value']=('相互拟制','不拟制')
cmbChosenSOUTH_NORTH.grid(column = 50, row = 2,padx=36,pady=7)
cmbChosenSOUTH_NORTH.current(0)



frameSTART_STOP = ttk.LabelFrame(win,text = "开停机键功能设定", width=300, height=80)
frameSTART_STOP.grid(row=4, column=0, sticky=W,padx=10,pady=7)
ttk.Label(frameSTART_STOP,text="开START").grid(column=0,row=1)#增加东西关系标签
ttk.Label(frameSTART_STOP,text="停STOP").grid(sticky = W,column=0,row=2)#增加东标签

START= tk.StringVar()#增加EAST下拉菜单
cmbChosenSTART = ttk.Combobox(frameSTART_STOP,width=8, height=10,textvariable =START)
cmbChosenSTART['value']=('备用点动','备用自锁')
cmbChosenSTART.grid(column = 1, row = 1,padx=10)
cmbChosenSTART.current(0)
STOP= tk.StringVar()#增加WEST下拉菜单
cmbChosenSTOP = ttk.Combobox(frameSTART_STOP,width=8, height=10,textvariable =STOP)
cmbChosenSTOP['value']=('关机')
cmbChosenSTOP.grid(column = 1, row =2,padx=10,pady=7)
cmbChosenSTOP.current(0)


frameOUT1_OUT2 = ttk.LabelFrame(win,text = "OUT1  OUT2键功能设定", width=300, height=80)
frameOUT1_OUT2.grid(row=5, column=0, sticky=W,padx=10,pady=7)
ttk.Label(frameOUT1_OUT2,text="OUT1 OUT2关系").grid(column=50,row=1)#增加东西关系标签
ttk.Label(frameOUT1_OUT2,text="OUT1").grid(sticky = W,column=0,row=1)#增加东标签
ttk.Label(frameOUT1_OUT2,text="OUT2").grid(sticky = W,column=0,row=2)#增加西WEST标签
OUT1= tk.StringVar()#增加EAST下拉菜单
cmbChosenOUT1 = ttk.Combobox(frameOUT1_OUT2,width=8, height=10,textvariable =OUT1)
cmbChosenOUT1['value']=('点动','自锁')
cmbChosenOUT1.grid(column = 1, row = 1,padx=25)
cmbChosenOUT1.current(0)
OUT2= tk.StringVar()#增加WEST下拉菜单
cmbChosenOUT2 = ttk.Combobox(frameOUT1_OUT2,width=8, height=10,textvariable =OUT2)
cmbChosenOUT2['value']=('点动','自锁')
cmbChosenOUT2.grid(column = 1, row =2,padx=25,pady=7)
cmbChosenOUT2.current(0)
OUT1_OUT2 = tk.StringVar()
cmbChosenOUT1_OUT2 = ttk.Combobox(frameOUT1_OUT2,width=8, height=10,textvariable =OUT1_OUT2)#增加东西关系下拉菜单
cmbChosenOUT1_OUT2['value']=('相互拟制','不拟制')
cmbChosenOUT1_OUT2.grid(column = 50, row = 2,padx=21,pady=7)
cmbChosenOUT1_OUT2.current(0)


frameOUT3_OUT4 = ttk.LabelFrame(win,text = "OUT3  OUT4键功能设定", width=300, height=80)
frameOUT3_OUT4.grid(row=5, column=81, sticky=W,padx=10,pady=7)
ttk.Label(frameOUT3_OUT4,text="OUT3 OUT4关系").grid(column=50,row=1)#增加东西关系标签
ttk.Label(frameOUT3_OUT4,text="OUT3").grid(sticky = W,column=0,row=1)#增加东标签
ttk.Label(frameOUT3_OUT4,text="OUT4").grid(sticky = W,column=0,row=2)#增加西WEST标签
OUT3= tk.StringVar()#增加EAST下拉菜单
cmbChosenOUT3 = ttk.Combobox(frameOUT3_OUT4,width=8, height=10,textvariable =OUT3)
cmbChosenOUT3['value']=('点动','自锁')
cmbChosenOUT3.grid(column = 1, row = 1,padx=25)
cmbChosenOUT3.current(0)
OUT4= tk.StringVar()#增加WEST下拉菜单
cmbChosenOUT4 = ttk.Combobox(frameOUT3_OUT4,width=8, height=10,textvariable =OUT4)
cmbChosenOUT4['value']=('点动','自锁')
cmbChosenOUT4.grid(column = 1, row =2,padx=25,pady=7)
cmbChosenOUT4.current(0)
OUT3_OUT4 = tk.StringVar()
cmbChosenOUT3_OUT4 = ttk.Combobox(frameOUT3_OUT4,width=8, height=10,textvariable =OUT3_OUT4)#增加东西关系下拉菜单
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
    global res_data
    print("The receive_data threading is start")
    while mySerial.isOpen():
        wait = mySerial.inWaiting()
        if wait :
            res_data = mySerial.readline()
            
            #Text_get.insert(END, res_data + bytes('\n', encoding='utf8'))
            print('ReceiveData',res_data)
            Get_DeviceParams()
            mySerial.flushInput()
def OpenPort(self):
    global PortOpenFlag
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
            WAKECOM.config(state = 'able')
            WAKECOM.bind("<Button-1>")
            FORMAT.config(state = 'able')
            FORMAT.bind("<Button-1>")
            ReadParams.config(state = 'able')
            ReadParams.bind("<Button-1>")
            WriteParams.config(state = 'able')
            WriteParams.bind("<Button-1>")
##WAKECOM =  ttk.Button(frameCOMX,width = 8,text="唤醒串口",command=WakeUpPort)
##
##FORMAT = ttk.Button(frameFORMAT,width = 36,text="数据卡格式化",command=FormatCard)
##ReadParams = ttk.Button(frameSN_CH,width = 8,text="读参数",command=Read_paramsStatus)
##WriteParams = ttk.Button(frameSN_CH,width = 8,text="下载参数",command=SendData)

            if mySerial.isOpen():
                PortOpenFlag = 1
                t = threading.Thread(target=ReceiveData)
                t.setDaemon(True)
                t.start()
        except:
            PortOpenFlag = 0
            info = sys.exc_info()
            print('info',info[0],":",info[1])
            messagebox.showinfo('错误提示', '出错啦，请检查！')
    else:
        mySerial.close()
        PortOpenFlag = 0
##        COMOFF.config(state = 'disabled')
##        COMOFF.unbind("<Button-1>")
        WAKECOM.config(state = 'disabled')
        WAKECOM.unbind("<Button-1>")
        FORMAT.config(state = 'disabled')
        FORMAT.unbind("<Button-1>")
        ReadParams.config(state = 'disabled')
        ReadParams.unbind("<Button-1>")
        WriteParams.config(state = 'disabled')
        WriteParams.unbind("<Button-1>")        


        
        Open.set('打开串口')
Open.set('打开串口')

def com(event1,event2):
    try:
        if event1 != "" and event2 != "":
            if len(event1) != 8 or len(event2) != 2:
                error_flag = 1
                messagebox.showwarning('警告','输入数据长度不合法')
            else :
                error_flag = 0
                params1 = a2b_hex(event1)
                params2 = a2b_hex(event2)
                print('sn len',len(event1))
                print('ch len',len(event2))
                print('try1',params1)
                return params1,params2
        else :
            #print('try2',params)
            messagebox.showwarning('警告','请输入16进制数字')
    except TypeError   as e:
        info = sys.exc_info()
        print('info',info[0],":",info[1])
        if error_flag == 0 :
            error_flag = 0
            messagebox.showwarning('警告','请输入16进制数字')
    except binascii.Error as e:
        info = sys.exc_info()
        print('info',info[0],":",info[1])
        if error_flag == 0 :
          error_flag = 0
          messagebox.showwarning('警告','请输入16进制数字') 
COMOFF.bind("<Button-1>",OpenPort)

def payload_pack(event):
    global txBuffer
    if(event == 0x12):
        txBuffer = bytes.fromhex('ff')
        txBuffer = txBuffer+bytes.fromhex('ff')
        txBuffer = txBuffer+bytes.fromhex('ff')
        txBuffer = txBuffer+bytes.fromhex('ff')
        txBuffer = txBuffer+bytes.fromhex('12')
        txBuffer = txBuffer+bytes.fromhex('0d')
    if(event == 0x13):
        txBuffer = bytes.fromhex('ff')
        txBuffer = txBuffer+bytes.fromhex('ff')
        txBuffer = txBuffer+bytes.fromhex('ff')
        txBuffer = txBuffer+bytes.fromhex('ff')
        txBuffer = txBuffer+bytes.fromhex('13')
        txBuffer = txBuffer+bytes.fromhex('0d')
    if(event == 0x56):
        txBuffer = bytes.fromhex('ff')
        txBuffer = txBuffer+bytes.fromhex('ff')
        txBuffer = txBuffer+bytes.fromhex('ff')
        txBuffer = txBuffer+bytes.fromhex('ff')
        txBuffer = txBuffer+bytes.fromhex('56')
        txBuffer = txBuffer+bytes.fromhex('0d')
    if(event == 0x57):
        txBuffer = bytes.fromhex('ff')
        txBuffer = txBuffer+bytes.fromhex('ff')
        txBuffer = txBuffer+bytes.fromhex('ff')
        txBuffer = txBuffer+bytes.fromhex('ff')
        txBuffer = txBuffer+bytes.fromhex('57')
        txBuffer = txBuffer+bytes.fromhex('0d')
def Get_paramsStatus():
    global txBuffer
    global keystatusH, keystatusL
    global keyInhibitonNum
    if cmbChosenUp.get() == '点动':
        keystatusH =  keystatusH & 0x7f
##        print('cmbChosenUp点动',keystatusH)
    else :
         keystatusH =  keystatusH | 0x80
##         print('cmbChosenUp',keystatusH)
 #cmbChosenDown******************************************        
    if cmbChosenDown.get() == '点动':
        keystatusH =  keystatusH & 0xbf
##        print('cmbChosenDown点动',keystatusH)
    else :
         keystatusH =  keystatusH | 0x40
##         print('cmbChosenDown',keystatusH)
 #cmbChosenEAST******************************************            
    if cmbChosenEAST.get() == '点动':
        keystatusH =  keystatusH & 0xdf
    else :
         keystatusH =  keystatusH | 0x20
 #cmbChosenWEST******************************************        
    if cmbChosenWEST.get() == '点动':
        keystatusH =  keystatusH & 0xef
    else :
         keystatusH =  keystatusH | 0x10
 #cmbChosenSOUTH******************************************              
    if cmbChosenSOUTH.get() == '点动':
        keystatusH =  keystatusH & 0xf7
    else :
         keystatusH =  keystatusH | 0x08
 #cmbChosenNORTH******************************************            
    if cmbChosenNORTH.get() == '点动':
        keystatusH =  keystatusH & 0xfb
    else :
         keystatusH =  keystatusH | 0x04
 #cmbChosenSTART******************************************          
    if cmbChosenSTART.get() == '备用点动':
        keystatusH =  keystatusH & 0xfd
    else :
         keystatusH =  keystatusH | 0x02
 #cmbChosenSTOP******************************************          
    if cmbChosenSTOP.get() == '关机':
        keystatusH =  keystatusH & 0xfe
        #print('keystatusH',keystatusH)
        #print('keystatusH',cmbChosenSTOP.get())
    else :
         keystatusH =  keystatusH | 0x01        
         print('keystatusH',keystatusH)

#cmbChosenOUT1******************************************
    if cmbChosenOUT1.get() == '点动':
        keystatusL =  keystatusL & 0x7f
    else :
         keystatusL =  keystatusL | 0x80
 #cmbChosenOUT2******************************************        
    if cmbChosenOUT2.get() == '点动':
        keystatusL =  keystatusL & 0xbf
    else :
         keystatusL =  keystatusL | 0x40
 #cmbChosenOUT3******************************************            
    if cmbChosenOUT3.get() == '点动':
        keystatusL =  keystatusL & 0xdf
    else :
         keystatusL =  keystatusL | 0x20
 #cmbChosenOUT4******************************************        
    if cmbChosenOUT4.get() == '点动':
        keystatusL =  keystatusL & 0xef
    else :
         keystatusL =  keystatusL | 0x10       
##    print('keystatusL',keystatusL)

 #cmbChosenUpDown******************************************        
    if cmbChosenUpDown.get() == '相互拟制':
        keyInhibitonNum =  keyInhibitonNum & 0x7f
    else :
         keyInhibitonNum =  keyInhibitonNum | 0x80       

 #cmbChosenEAST_WEST******************************************        
    if cmbChosenEAST_WEST.get() == '相互拟制':
        keyInhibitonNum =  keyInhibitonNum & 0xbf
    else :
         keyInhibitonNum =  keyInhibitonNum | 0x40       
    #print('keystatusL',keystatusL)


 #cmbChosenSOUTH_NORTH******************************************        
    if cmbChosenSOUTH_NORTH.get() == '相互拟制':
        keyInhibitonNum =  keyInhibitonNum & 0xdf
    else :
         keyInhibitonNum =  keyInhibitonNum | 0x20       
    #print('keystatusL',keystatusL)
 #cmbChosenOUT1_OUT2******************************************        
    if cmbChosenOUT1_OUT2.get() == '相互拟制':
        keyInhibitonNum =  keyInhibitonNum & 0xef
    else :
         keyInhibitonNum =  keyInhibitonNum | 0x10       
    #print('keystatusL',keystatusL)
 #cmbChosenOUT3_OUT4******************************************        
    if cmbChosenOUT3_OUT4.get() == '相互拟制':
        keyInhibitonNum =  keyInhibitonNum & 0xf7
    else :
         keyInhibitonNum =  keyInhibitonNum | 0x08       
   
    payload_pack(0x12)
   
    keystatusHt = struct.pack('B',keystatusH)
    keystatusLt = struct.pack('B',keystatusL)
    keyInhibitonNumt = struct.pack('B',keyInhibitonNum)
    SNt,CHt = com(entrySN.get(),entryCH.get())
    print('keystatusH',keystatusH)
    print('keystatusL',keystatusL)
    print('keyInhibitonNum',keyInhibitonNum)
    txBuffer= txBuffer+keystatusHt
    txBuffer= txBuffer+keystatusLt
    txBuffer= txBuffer+keyInhibitonNumt
    txBuffer= txBuffer+SNt
    txBuffer= txBuffer+CHt
    txBuffer= txBuffer+crc_cal(txBuffer)
    txBuffer = txBuffer+bytes.fromhex('fe')
    print('Get_paramsStatus  end tag',txBuffer)
def Get_DeviceParams():
    global txBuffer
    global keystatusH, keystatusL
    global keyInhibitonNum
    global res_data
##    if cmbChosenUp.get() == '点动':
##        keystatusH =  keystatusH & 0x7f
####        print('cmbChosenUp点动',keystatusH)
##    else :
##         keystatusH =  keystatusH | 0x80
####         print('cmbChosenUp',keystatusH)
## #cmbChosenDown******************************************        
##    if cmbChosenDown.get() == '点动':
##        keystatusH =  keystatusH & 0xbf
####        print('cmbChosenDown点动',keystatusH)
##    else :
##         keystatusH =  keystatusH | 0x40
####         print('cmbChosenDown',keystatusH)
## #cmbChosenEAST******************************************            
##    if cmbChosenEAST.get() == '点动':
##        keystatusH =  keystatusH & 0xdf
##    else :
##         keystatusH =  keystatusH | 0x20
## #cmbChosenWEST******************************************        
##    if cmbChosenWEST.get() == '点动':
##        keystatusH =  keystatusH & 0xef
##    else :
##         keystatusH =  keystatusH | 0x10
## #cmbChosenSOUTH******************************************              
##    if cmbChosenSOUTH.get() == '点动':
##        keystatusH =  keystatusH & 0xf7
##    else :
##         keystatusH =  keystatusH | 0x08
## #cmbChosenNORTH******************************************            
##    if cmbChosenNORTH.get() == '点动':
##        keystatusH =  keystatusH & 0xfb
##    else :
##         keystatusH =  keystatusH | 0x04
## #cmbChosenSTART******************************************          
##    if cmbChosenSTART.get() == '备用点动':
##        keystatusH =  keystatusH & 0xfd
##    else :
##         keystatusH =  keystatusH | 0x02
## #cmbChosenSTOP******************************************          
##    if cmbChosenSTOP.get() == '关机':
##        keystatusH =  keystatusH & 0xfe
##        #print('keystatusH',keystatusH)
##        #print('keystatusH',cmbChosenSTOP.get())
##    else :
##         keystatusH =  keystatusH | 0x01        
##         print('keystatusH',keystatusH)
##
###cmbChosenOUT1******************************************
##    if cmbChosenOUT1.get() == '点动':
##        keystatusL =  keystatusL & 0x7f
##    else :
##         keystatusL =  keystatusL | 0x80
## #cmbChosenOUT2******************************************        
##    if cmbChosenOUT2.get() == '点动':
##        keystatusL =  keystatusL & 0xbf
##    else :
##         keystatusL =  keystatusL | 0x40
## #cmbChosenOUT3******************************************            
##    if cmbChosenOUT3.get() == '点动':
##        keystatusL =  keystatusL & 0xdf
##    else :
##         keystatusL =  keystatusL | 0x20
## #cmbChosenOUT4******************************************        
##    if cmbChosenOUT4.get() == '点动':
##        keystatusL =  keystatusL & 0xef
##    else :
##         keystatusL =  keystatusL | 0x10       
####    print('keystatusL',keystatusL)
##
## #cmbChosenUpDown******************************************        
##    if cmbChosenUpDown.get() == '相互拟制':
##        keyInhibitonNum =  keyInhibitonNum & 0x7f
##    else :
##         keyInhibitonNum =  keyInhibitonNum | 0x80       
##
## #cmbChosenEAST_WEST******************************************        
##    if cmbChosenEAST_WEST.get() == '相互拟制':
##        keyInhibitonNum =  keyInhibitonNum & 0xbf
##    else :
##         keyInhibitonNum =  keyInhibitonNum | 0x40       
##    #print('keystatusL',keystatusL)
##
##
## #cmbChosenSOUTH_NORTH******************************************        
##    if cmbChosenSOUTH_NORTH.get() == '相互拟制':
##        keyInhibitonNum =  keyInhibitonNum & 0xdf
##    else :
##         keyInhibitonNum =  keyInhibitonNum | 0x20       
##    #print('keystatusL',keystatusL)
## #cmbChosenOUT1_OUT2******************************************        
##    if cmbChosenOUT1_OUT2.get() == '相互拟制':
##        keyInhibitonNum =  keyInhibitonNum & 0xef
##    else :
##         keyInhibitonNum =  keyInhibitonNum | 0x10       
##    #print('keystatusL',keystatusL)
## #cmbChosenOUT3_OUT4******************************************        
##    if cmbChosenOUT3_OUT4.get() == '相互拟制':
##        keyInhibitonNum =  keyInhibitonNum & 0xf7
##    else :
##         keyInhibitonNum =  keyInhibitonNum | 0x08       
##   
##    txBuffer = bytes.fromhex('ff')
##    txBuffer = txBuffer+bytes.fromhex('ff')
##    txBuffer = txBuffer+bytes.fromhex('ff')
##    txBuffer = txBuffer+bytes.fromhex('ff')
##    txBuffer = txBuffer+bytes.fromhex('12')
##    txBuffer = txBuffer+bytes.fromhex('0d')
##   
##    keystatusHt = struct.pack('B',keystatusH)
##    keystatusLt = struct.pack('B',keystatusL)
##    keyInhibitonNumt = struct.pack('B',keyInhibitonNum)
##    SNt,CHt = com(entrySN.get(),entryCH.get())
##    print('keystatusH',keystatusH)
##    print('keystatusL',keystatusL)
##    print('keyInhibitonNum',keyInhibitonNum)
##    txBuffer= txBuffer+keystatusHt
##    txBuffer= txBuffer+keystatusLt
##    txBuffer= txBuffer+keyInhibitonNumt
##    txBuffer= txBuffer+SNt
##    txBuffer= txBuffer+CHt

    #print(txBuffer

    if(len(res_data)>=17):
      if(res_data[0] == 0xff and res_data[1] == 0xff and res_data[2] == 0xff and
         res_data[3] == 0xff and res_data[4] == 0x12 and res_data[len(res_data)-1] == 0xfe):
          crc16_xmodemt = crc_cal(res_data[0:(len(res_data)-3)])
          if(crc16_xmodemt == res_data[(len(res_data)-3):(len(res_data)-1)]):
              print('Get_DeviceParams CRC',crc16_xmodemt)
              print('Get_DeviceParams CRC ok')
def crc_cal(event):
    crc16_xmodem = crcmod.mkCrcFun(0x11021,rev=False,initCrc = 0x0000,xorOut = 0x0000)
    crc16_xmodemt = struct.pack('H',crc16_xmodem(event))
    return crc16_xmodemt
def Read_paramsStatus():
    global txBuffer
    payload_pack(0x13)
    txBuffer= txBuffer+bytes.fromhex('00')
    txBuffer= txBuffer+bytes.fromhex('00')
    txBuffer= txBuffer+bytes.fromhex('00')
    txBuffer= txBuffer+bytes.fromhex('00000000')
    txBuffer= txBuffer+bytes.fromhex('00')  
    txBuffer= txBuffer+crc_cal(txBuffer)
    txBuffer = txBuffer+bytes.fromhex('fe')
    print('Get_paramsStatus  end tag',txBuffer)
    t = threading.Thread(target=GetData)
    t.setDaemon(True)
    t.start()
    t.join()
def GetData():
      global txBuffer
      mySerial.write( txBuffer)
      wait = mySerial.inWaiting()
      if wait:
        res_data = mySerial.readline()
        #print(type(res_data))
##        Text_get.insert(END, res_data + bytes('\n', encoding='utf8'))
##        mySerial.flushInput()
def SendData():
    if mySerial.isOpen():
        Get_paramsStatus()
        t = threading.Thread(target=GetData)
        t.setDaemon(True)
        t.start()
        t.join()
def FormatCard():
    global txBuffer
    payload_pack(0x56)
    txBuffer= txBuffer+bytes.fromhex('00')
    txBuffer= txBuffer+bytes.fromhex('00')
    txBuffer= txBuffer+bytes.fromhex('00')
    txBuffer= txBuffer+bytes.fromhex('00000000')
    txBuffer= txBuffer+bytes.fromhex('00')  
    txBuffer= txBuffer+crc_cal(txBuffer)
    txBuffer = txBuffer+bytes.fromhex('fe')
    print('FormatCard  end tag',txBuffer)
    t = threading.Thread(target=GetData)
    t.setDaemon(True)
    t.start()
    t.join()
def WakeUpPort():
    global txBuffer
    payload_pack(0x57)
    txBuffer= txBuffer+bytes.fromhex('00')
    txBuffer= txBuffer+bytes.fromhex('00')
    txBuffer= txBuffer+bytes.fromhex('00')
    txBuffer= txBuffer+bytes.fromhex('00000000')
    txBuffer= txBuffer+bytes.fromhex('00')  
    txBuffer= txBuffer+crc_cal(txBuffer)
    txBuffer = txBuffer+bytes.fromhex('fe')
    print('FormatCard  end tag',txBuffer)
    t = threading.Thread(target=GetData)
    t.setDaemon(True)
    t.start()
    t.join()
# 创建button
WAKECOM =  ttk.Button(frameCOMX,width = 8,text="唤醒串口",command=WakeUpPort)
WAKECOM.place(relx=0,rely=0,x=215,y=10)
FORMAT = ttk.Button(frameFORMAT,width = 36,text="数据卡格式化",command=FormatCard)
FORMAT.grid(column = 12, row = 0)
FORMAT.place(relx=0,rely=0,x=11,y=12)
ReadParams = ttk.Button(frameSN_CH,width = 8,text="读参数",command=Read_paramsStatus)
ReadParams.grid(column = 12, row = 0,sticky=W)
ReadParams.place(relx=0,rely=0,x=200,y=15)
WriteParams = ttk.Button(frameSN_CH,width = 8,text="下载参数",command=SendData)
WriteParams.place(relx=0,rely=0,x=200,y=95)
WAKECOM.config(state = 'disabled')
WAKECOM.unbind("<Button-1>")
FORMAT.config(state = 'disabled')
FORMAT.unbind("<Button-1>")
ReadParams.config(state = 'disabled')
ReadParams.unbind("<Button-1>")
WriteParams.config(state = 'disabled')
WriteParams.unbind("<Button-1>") 
win.mainloop()

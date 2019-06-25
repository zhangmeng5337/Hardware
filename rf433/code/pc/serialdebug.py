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
FrameHeight     = 80
FrameWidth      = 290
FrameHeightLarge = 170
RowSpace        = 0
OpadxSpace      = 15
OpadxSpaceSmall = 10
OpadySpace      = 10
OpadySpaceStr   = 2
OpadySpaceSmall   = 0
IpadxSpace      = 10
IpadySpace      = 5
BoxWidth        = 6
BoxWidthLong    = 7
serialOpenFlag=0
recvBuffer = bytearray()
txBuffer = bytearray(32)
keystatusH = 0
keystatusL = 0
keyInhibitonNum = 0
recv_count=0
j=0
n = 0
recvCount = 0
#创建主窗口
win=tk.Tk()
win.title("串口配置工具")
win.geometry("700x500+500+200")#窗口大小800x800，显示器位置500，500

#增加frame******************上下
frameUpDown = LabelFrame(win, text="上下键功能设定", width=FrameWidth, height=FrameHeight)
frameUpDown.grid(row=0, column=0, sticky=S)
##frameUpDown = ttk.LabelFrame(win,height = FrameHeight,width = FrameWidth,text = "上下键功能设定")
##frameUpDown.grid(column = 0, row = 0,ipadx = IpadxSpace,ipady =IpadySpace,padx = OpadxSpace,pady =OpadySpace)
frameUpDown.place(relx=0,rely=0,x=15,y=20)
ttk.Label(frameUpDown,text="上下关系").grid(column=50,row=0,padx = OpadxSpace,pady =0)#增加标签
ttk.Label(frameUpDown,text="上UP").grid(sticky = W,column=0,row=0,padx = OpadxSpaceSmall,pady =OpadySpaceStr)#增加up标签
ttk.Label(frameUpDown,text="下DOWN").grid(sticky = W,column=0,row=1,padx =OpadxSpaceSmall,pady =OpadySpaceStr)#增加DOWN标签
#增加下拉菜单******************上下
up= tk.StringVar()#增加up下拉菜单
cmbChosenUp = ttk.Combobox(frameUpDown,width=BoxWidth,textvariable = up)
cmbChosenUp['value']=('点动','自锁')
cmbChosenUp.grid(column = 1, row = 0,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosenUp.current(0)
down= tk.StringVar()#增加down下拉菜单
cmbChosenDown = ttk.Combobox(frameUpDown,width=BoxWidth,textvariable = down)
cmbChosenDown['value']=('点动','自锁')
cmbChosenDown.grid(column = 1, row = 1,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosenDown.current(0)

UpDown= tk.StringVar()#增加上下关系下拉菜单
cmbChosenUpDown = ttk.Combobox(frameUpDown,width=BoxWidthLong,textvariable = UpDown)
cmbChosenUpDown['value']=('相互拟制','不拟制')
cmbChosenUpDown.grid(column = 50, row = 1,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosenUpDown.current(0)



#增加frame******************东西
frameEW = ttk.LabelFrame(win,height = FrameHeight,width = FrameWidth,text = "东西键功能设定")
frameEW.grid(column = 0, row = 4,ipadx = IpadxSpace,ipady =IpadySpace,padx = OpadxSpace,pady =OpadySpace)
frameEW.place(relx=0,rely=0,x=15,y=110)
ttk.Label(frameEW,text="东西关系").grid(column=50,row=0,padx = OpadxSpace,pady =OpadySpaceStr)#增加东西关系标签
ttk.Label(frameEW,text="东EAST").grid(sticky = W,column=0,row=0,padx = OpadxSpace,pady =OpadySpaceStr)#增加东标签
ttk.Label(frameEW,text="西WEST").grid(sticky = W,column=0,row=1,padx =OpadxSpace,pady =OpadySpaceStr)#增加西WEST标签
EAST= tk.StringVar()#增加EAST下拉菜单
cmbChosenEAST = ttk.Combobox(frameEW,width=BoxWidth,textvariable = EAST)
cmbChosenEAST['value']=('点动','自锁')
cmbChosenEAST.grid(column = 1, row = 0,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosenEAST.current(0)
WEST= tk.StringVar()#增加WEST下拉菜单
cmbChosenWEST = ttk.Combobox(frameEW,width=BoxWidth,textvariable = WEST)
cmbChosenWEST['value']=('点动','自锁')
cmbChosenWEST.grid(column = 1, row = 1,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosenWEST.current(0)
EAST_WEST= tk.StringVar()
cmbChosenEAST_WEST = ttk.Combobox(frameEW,width=BoxWidthLong,textvariable = EAST_WEST)#增加东西关系下拉菜单
cmbChosenEAST_WEST['value']=('相互拟制','不拟制')
cmbChosenEAST_WEST.grid(column = 50, row = 1,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosenEAST_WEST.current(0)
###增加frame******************南北
frameSOUTH_NORTH = ttk.LabelFrame(win,height = FrameHeight,width = FrameWidth,text = "南北键功能设定")
frameSOUTH_NORTH.grid(column = 0, row = 8,ipadx = 0,ipady =5,padx = OpadxSpace,pady =OpadySpace)
frameSOUTH_NORTH.place(relx=0,rely=0,x=15,y=200)
ttk.Label(frameSOUTH_NORTH,text="南北关系").grid(column=50,row=0,padx = OpadxSpace,pady =OpadySpaceStr)
ttk.Label(frameSOUTH_NORTH,text="南SOUTH").grid(sticky = W,column=0,row=0,padx = OpadxSpaceSmall,pady =OpadySpaceStr)#增加SOUTH标签
ttk.Label(frameSOUTH_NORTH,text="北NORTH").grid(sticky = W,column=0,row=1,padx =OpadxSpaceSmall,pady =OpadySpaceStr)#增加NORTH标签
SOUTH= tk.StringVar()#增加SOUTH下拉菜单
cmbChosenSOUTH = ttk.Combobox(frameSOUTH_NORTH,width=BoxWidth,textvariable = SOUTH)
cmbChosenSOUTH['value']=('点动','自锁')
cmbChosenSOUTH.grid(column = 1, row = 0,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosenSOUTH.current(0)
NORTH= tk.StringVar()#增加NORTHT下拉菜单
cmbChosenNORTH = ttk.Combobox(frameSOUTH_NORTH,width=BoxWidth,textvariable = NORTH)
cmbChosenNORTH['value']=('点动','自锁')
cmbChosenNORTH.grid(column = 1, row = 1,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosenNORTH.current(0)

SOUTH_NORTH= tk.StringVar()#增加南北关系下拉菜单
cmbChosenSOUTH_NORTH = ttk.Combobox(frameSOUTH_NORTH,width=BoxWidthLong,textvariable = SOUTH_NORTH)
cmbChosenSOUTH_NORTH['value']=('相互拟制','不拟制')
cmbChosenSOUTH_NORTH.grid(column = 50, row = 1,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosenSOUTH_NORTH.current(0)
#增加frame******************开停机键功能设定
frameSTART_STOP = ttk.LabelFrame(win,height = FrameHeight,width = FrameWidth,text = "开停机键功能设定")
frameSTART_STOP.grid(column = 0, row = 12,ipadx = IpadxSpace,ipady =IpadySpace,padx = OpadxSpace,pady =OpadySpace)
frameSTART_STOP.place(relx=0,rely=0,x=15,y=290)
ttk.Label(frameSTART_STOP,text="开START").grid(sticky = W,column=0,row=0,padx = 12,pady =OpadySpaceStr)#增加开START标签
ttk.Label(frameSTART_STOP,text="停STOP").grid(sticky = W,column=0,row=1,padx =12,pady =OpadySpaceStr)#增加停STOP标签
START= tk.StringVar()#增加开START下拉菜单
cmbChosenSTART = ttk.Combobox(frameSTART_STOP,width=BoxWidthLong,textvariable = START)
cmbChosenSTART['value']=('备用点动','备用自锁')
cmbChosenSTART.grid(column = 1, row = 0,padx = 0,pady =OpadySpaceStr)
cmbChosenSTART.current(0)
STOP= tk.StringVar()#增加停STOP下拉菜单
cmbChosenSTOP = ttk.Combobox(frameSTART_STOP,width=BoxWidthLong,textvariable = STOP)
cmbChosenSTOP['value']=('关机')
cmbChosenSTOP.grid(column = 1, row = 1,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosenSTOP.current(0)
###增加frame******************OUT1,OUT2
frameOUT1_OUT2 = tk.LabelFrame(win,height = FrameHeight,width = FrameWidth,text = "OUT1  OUT2键功能设定")
frameOUT1_OUT2.grid(column = 0, row = 16,ipadx = 0,ipady =5,padx = OpadxSpace,pady =OpadySpace)
frameOUT1_OUT2.place(relx=0,rely=0,x=15,y=380)
ttk.Label(frameOUT1_OUT2,text="OUT1 OUT2关系").grid(column=50,row=0,padx = OpadxSpace,pady =OpadySpaceStr)
ttk.Label(frameOUT1_OUT2,text="OUT1").grid(sticky = W,column=0,row=0,padx = 20,pady =OpadySpaceStr)#增加OUT1标签
ttk.Label(frameOUT1_OUT2,text="OUT2").grid(sticky = W,column=0,row=1,padx =20,pady =OpadySpaceStr)#增加OUT2标签
OUT1= tk.StringVar()#增加OUT1下拉菜单
cmbChosenOUT1 = ttk.Combobox(frameOUT1_OUT2,width=BoxWidth,textvariable = OUT1)
cmbChosenOUT1['value']=('点动','自锁')
cmbChosenOUT1.grid(column = 1, row = 0,padx = 13,pady =OpadySpaceStr)
cmbChosenOUT1.current(0)
OUT2= tk.StringVar()#增加OUT2下拉菜单
cmbChosenOUT2 = ttk.Combobox(frameOUT1_OUT2,width=BoxWidth,textvariable = OUT2)
cmbChosenOUT2['value']=('点动','自锁')
cmbChosenOUT2.grid(column = 1, row = 1,padx = 13,pady =OpadySpaceStr)
cmbChosenOUT2.current(0)
OUT1_OUT2= tk.StringVar()#增加OUT1 OUT2下拉菜单
cmbChosenOUT1_OUT2 = ttk.Combobox(frameOUT1_OUT2,width=BoxWidthLong,textvariable = OUT1_OUT2)
cmbChosenOUT1_OUT2['value']=('相互拟制','不拟制')
cmbChosenOUT1_OUT2.grid(column = 50, row = 1,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosenOUT1_OUT2.current(0)
#增加frame******************OUT3,OUT4
frameOUT3_OUT4 = tk.LabelFrame(win,height = FrameHeight,width = FrameWidth,text = "OUT  OUT4键功能设定")
frameOUT3_OUT4.grid(column = 4, row = 16,ipadx = 0,ipady =5,padx = OpadxSpace,pady =OpadySpace)
frameOUT3_OUT4.place(relx=0,rely=0,x=325,y=380)
ttk.Label(frameOUT3_OUT4,text="OUT3 OUT4关系").grid(column=50,row=0,padx = OpadxSpace,pady =OpadySpaceStr)
ttk.Label(frameOUT3_OUT4,text="OUT3").grid(sticky = W,column=0,row=0,padx = OpadxSpace,pady =OpadySpaceStr)#增加OUT3标签
ttk.Label(frameOUT3_OUT4,text="OUT4").grid(sticky = W,column=0,row=1,padx =OpadxSpace,pady =OpadySpaceStr)#增加OUT4标签
OUT3= tk.StringVar()#增加OUT3下拉菜单
cmbChosenOUT3 = ttk.Combobox(frameOUT3_OUT4,width=BoxWidth,textvariable = OUT3)
cmbChosenOUT3['value']=('点动','自锁')
cmbChosenOUT3.grid(column = 1, row = 0,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosenOUT3.current(0)
OUT4= tk.StringVar()#增加OUT4下拉菜单
cmbChosenOUT4 = ttk.Combobox(frameOUT3_OUT4,width=BoxWidth,textvariable = OUT4)
cmbChosenOUT4['value']=('点动','自锁')
cmbChosenOUT4.grid(column = 1, row = 1,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosenOUT4.current(0)
OUT3_OUT4= tk.StringVar()#增加OUT3 OUT4下拉菜单
cmbChosenOUT3_OUT4 = ttk.Combobox(frameOUT3_OUT4,width=BoxWidthLong,textvariable = OUT3_OUT4)
cmbChosenOUT3_OUT4['value']=('相互拟制','不拟制')
cmbChosenOUT3_OUT4.grid(column = 50, row = 1,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosenOUT3_OUT4.current(0)
###增加frame******************SN_CH功能设定
frameSN_CH = ttk.LabelFrame(win,height = FrameHeightLarge,width =FrameWidth,text = "读写SN、CH码")
frameSN_CH.grid(column = 4, row = 8,ipadx = IpadxSpace,ipady =IpadySpace,padx = OpadxSpace,pady =OpadySpace)
frameSN_CH.place(relx=0,rely=0,x=325,y=200)
#增加frame******************格式化数据卡
frameFORMAT = ttk.LabelFrame(win,height = FrameHeight ,width =FrameWidth,text = "数据卡格式化")
frameFORMAT.grid(column = 4, row = 8,ipadx = IpadxSpace,ipady =IpadySpace,padx = OpadxSpace,pady =OpadySpace)
frameFORMAT.place(relx=0,rely=0,x=325,y=110)
# 创建button
ReadParams = tk.Button(frameSN_CH,height =2,width = 8,text="读参数")
ReadParams.grid(column = 12, row = 0,sticky=W)
ReadParams.place(relx=0,rely=0,x=200,y=5)
WriteParams = tk.Button(frameSN_CH,height = 2,width = 8,text="下载参数")
WriteParams.place(relx=0,rely=0,x=200,y=90)
FORMAT = tk.Button(frameFORMAT,height =2,width = 36,text="数据卡格式化")
FORMAT.grid(column = 12, row = 0)
FORMAT.place(relx=0,rely=0,x=11,y=0.6)
###增加frame******************通讯参数
frameCOMX = tk.LabelFrame(win,height = FrameHeight,width = FrameWidth,text = "通讯参数")
frameCOMX.grid(column = 45, row = 0,ipadx = IpadxSpace,ipady =IpadySpace,padx = OpadxSpace,pady =OpadySpace)
frameCOMX.place(relx=0,rely=0,x=325,y=20)
COMX= tk.StringVar(frameCOMX,'COM1')#增加COM下拉菜单
cmbChosenCOMX = ttk.Combobox(frameCOMX,state  = 'readonly',width=6,textvariable = COMX)
cmbChosenCOMX.grid(column = 1, row = 0,padx = OpadxSpace,pady =OpadySpace)



COMOFF = tk.Button(frameCOMX,height =2,width = BoxWidthLong,text="打开串口")
COMOFF.grid(column = 12, row = 0,sticky=W)
COMOFF.place(relx=0,rely=0,x=110,y=1)
WAKECOM = tk.Button(frameCOMX,height = 2,width = BoxWidthLong,text="唤醒串口")
WAKECOM.place(relx=0,rely=0,x=200,y=1)
#Entry控件布局
ttk.Label(frameSN_CH,text="SN:").grid(column=1,row=1,padx = 30,pady =20)
ttk.Label(frameSN_CH,text="CH:").grid(column=1,row=5,padx = 30,pady =40)
entrySN=Entry(frameSN_CH,width = 10,state = 'normal')#sn码
entryCH=Entry(frameSN_CH,width = 10,state = 'normal')#ch码
entrySN.place(relx=0,rely=0,x=70,y=20)
entryCH.place(relx=0,rely=0,x=70,y=100)
frameUpDown.grid_propagate(0)
frameEW.grid_propagate(0)
frameSOUTH_NORTH.grid_propagate(0)
frameSTART_STOP.grid_propagate(0)
frameOUT1_OUT2.grid_propagate(0)
frameOUT3_OUT4.grid_propagate(0)
frameSN_CH.grid_propagate(0)
frameFORMAT.grid_propagate(0)
frameCOMX.grid_propagate(0)


#****************************应用逻辑**********************************************


#打开串口
def port_open():
    global serialOpenFlag
    global recv_count
    serialOpenFlag = 0
    ser.port = cmbChosenCOMX.get()           #设置端口号
    ser.baudrate = 9600     #设置波特率
    ser.bytesize = 8        #设置数据位
    ser.stopbits = 1        #设置停止位
    ser.parity = "N"        #设置校验位
    #port_close()
    ser.open()              #打开串口,要找到对的串口号才会成功
    if(ser.isOpen()):
        serialOpenFlag = 1
        recv_count = 0
        print("打开成功")
        COMOFF['text'] = '关闭串口'
        #print(serialOpenFlag)
        #print(ser.isOpen())  
    else:
        NoSerialDiage()
        COMOFF['text'] = '打开串口'
        print("打开失败")
   #关闭串口     
def port_close():
    global recv_count
    ser.close()
    if (ser.isOpen()):
        print("关闭失败")

    else:
        print("关闭成功")

        recv_count = 0
        serialOpenFlag = 0
#串口发送数据
def send(send_data):
    if (ser.isOpen()):
        #ser.write(send_data.encode("gbk"))  #utf-8 编码发送
        ser.write(send_data.encode("utf-8"))  #Hex发送
        print("发送成功",send_data)
    else:
        print("发送失败")
def uart_tx(pbuffer,length):
      for i in range(0,length):
          send(chr(pbuffer[i]))
#警示
def SerialShuntDiage():
    top = Toplevel()
    top.title('Python')
    w = tk.Label(top, text="串口已拔出，请重插入，再次打开串口", font=("华文行楷", 20), fg="red").grid(sticky = W,column=0,row=1,padx = OpadxSpace,pady =OpadySpaceStr)
    #SerialDiag = Button(top, text='确定',height =2,width = BoxWidthLong).grid(column=0,row=2,padx = OpadxSpace,pady =OpadySpaceStr)
    print("No Serial" )
    #串口接收数据
def recv():
    global serialOpenFlag
    global recvBuffer
    global recv_count
    global n


 
    while True:      
        if (serialOpenFlag == 1 and (ser.isOpen()== True)):
            #print(serialOpenFlag)
            #print(ser.isOpen())
            n = ser.inWaiting()
            if n > 0 :
              j = ser.read(n)

              recvBuffer = recvBuffer+j            
              recv_count=recv_count+1
              

        if  serialOpenFlag ==1 and (ser.isOpen() == False):
                 #SerialShuntDiage()
                 serialOpenFlag = 0
                # print(serialOpenFlag)
                 #print(ser.isOpen())
    time.sleep(0.1) 
def uart_data_analy():
      while 1:
          global recvCount
          global recvBuffer
          global recvThreadFlag
          i = 0
          while i < len(recvBuffer) :
                  for i in range(0,len(recvBuffer)):
                      i = i +1
                    
                      recvCount = recvCount +1
                      print("recv data",hex(recvBuffer[i-1]))
                      print("recv count",recvCount)
                  recvBuffer[0:i] = b''
                  print("recv sum",recvCount)
                  #i = 0
          time.sleep(0.1)
def NoSerialDiage():
    top = Toplevel()
    top.title('Python')
    w = tk.Label(top, text="串口不存在或者被占用，请重新选择", font=("华文行楷", 20), fg="red").grid(sticky = W,column=0,row=1,padx = OpadxSpace,pady =OpadySpaceStr)
    #SerialDiag = Button(top, text='确定',height =2,width = BoxWidthLong).grid(column=0,row=2,padx = OpadxSpace,pady =OpadySpaceStr)
    print("No Serial" )
#打开关闭串口
def SerialOnOFF(event):
  a =8
  global recvThreadFlag
  if COMOFF['text'] == '打开串口':
     COMOFF['text'] == '关闭串口'
     print(COMOFF['text'])
     if SerialListCheck(cmbChosenCOMX.get()) == None: 
       NoSerialDiage()
     else :
        port_open()
        if ( type(a)== int):
            send(chr(a))
            #print(type(a))
        else :
          uart_tx(a,1)
        #
  elif COMOFF['text'] == '关闭串口':
     COMOFF['text'] == '打开串口'     
     port_close()
     print(COMOFF['text'])
def SerialOnSuperv():
  global serialOpenFlag
  global recvThreadFlag
  while 1:
    # ser.open()              #打开串口,要找到对的串口号才会成功
     if   (ser.isOpen() == False and serialOpenFlag == 1 ):
       NoSerialDiage()

       print("SerialOnSuperv serialOpenFlag",serialOpenFlag)
       print("SerialOnSuperv ser.isOpen",ser.isOpen)
       time.sleep(0.5)
##     else :
##        SerialShuntDiage()

#刷新选择串口
def SerialListCheck(event):
             data=[]

             plist = list(serial.tools.list_ports.comports())
             if len(plist) <= 0:
               serialExistFlag = 0
               print("No Serial In SerialListCheck************************")
             else :
              data = [0 for i in range(len(plist))]
              for i in range(0,len(plist)) :
                    port_list_0= list(plist[i])
                    #ser = serial.Serial(port_list_0[0],9600,timeout = 60)
                    data[i]=port_list_0[0]
                    if event == data[i]:
                        serialExistFlag = 1
                        print("Serial No Exist SerialListCheck************************",data[i])
                        return  serialExistFlag                  
              print("Check  No Serial SerialListCheck************************")
              serialExistFlag = 0   
             # cmbChosenCOMX['values'] = data
              return  serialExistFlag 
def SerialList():
             global serialOpenFlag
             data=[]
##             if (serialOpenFlag == 1 and (ser.isOpen()== True)):
##                 event.set()
             plist = list(serial.tools.list_ports.comports())
             if len(plist) <= 0:
               #print ("no serial")
               cmbChosenCOMX['values'] = ''
             else :
              data = [0 for i in range(len(plist))]
              for i in range(0,len(plist)) :
                port_list_0= list(plist[i])
                data[i]=port_list_0[0]
              cmbChosenCOMX['values'] = data
              
              print("Serial List************************",cmbChosenCOMX['values'])
            # time.sleep(0.1)
            # return data 

def SerialSelect(self):
    SerialList()
    #cmbChosenCOMX.current(0)
    
cmbChosenCOMX.bind("<Button-1>",SerialSelect)
ser = serial.Serial()
 
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
def UpLoadParams():
    ff = 0
def DownLoadParams(event):
    global txBuffer
    global keystatusH, keystatusL
    global keyInhibitonNum 
##    #up down状态
##    print("up down************************")  
##    print(cmbChosenUp.get())
##    print(cmbChosenDown.get())
##    print(cmbChosenUpDown.get())
##    #East West状态
##    print("EAST_WEST************************")  
##    print(cmbChosenEAST.get())
##    print(cmbChosenWEST.get())
##    print(cmbChosenEAST_WEST.get())
##    #South North状态
##    print("SOUTH_NORTH************************") 
##    print(cmbChosenSOUTH.get())
##    print(cmbChosenNORTH.get())
##    print(cmbChosenSOUTH_NORTH.get())
##    #START STOP状态
##    print("START_STOP************************") 
##    print(cmbChosenSTART.get())
##    print(cmbChosenSTOP.get())
##
##    #OUT1 OUT2状态
##    print("OUT1_OUT2************************") 
##    print(cmbChosenOUT1.get())
##    print(cmbChosenOUT2.get())
##    print(cmbChosenOUT1_OUT2.get())
##    #OUT3 OUT4状态
##    print("OUT3_OUT4************************") 
##    print(cmbChosenOUT3.get())
##    print(cmbChosenOUT4.get())
##    print(cmbChosenOUT3_OUT4.get())
##    print("SN_CH************************") 
##    print(entrySN.get())
##    print(entryCH.get())
#cmbChosenUp******************************************
    print("p1",type(keystatusH))
    print("p2",type(0x7f)) 
    if cmbChosenUp.get() == '点动':
        keystatusH =  keystatusH & 0x7f
        print('cmbChosenUp点动',keystatusH)
    else :
         keystatusH =  keystatusH | 0x80
         print('cmbChosenUp',keystatusH)
 #cmbChosenDown******************************************        
    if cmbChosenDown.get() == '点动':
        keystatusH =  keystatusH & 0xbf
        print('cmbChosenDown点动',keystatusH)
    else :
         keystatusH =  keystatusH | 0x40
         print('cmbChosenDown',keystatusH)
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
        print('keystatusH',keystatusH)
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
    print('keystatusL',keystatusL)

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
   
    txBuffer = bytes.fromhex('ff')
    txBuffer = txBuffer+bytes.fromhex('ff')
    txBuffer = txBuffer+bytes.fromhex('ff')
    txBuffer = txBuffer+bytes.fromhex('ff')
    txBuffer = txBuffer+bytes.fromhex('12')
    txBuffer = txBuffer+bytes.fromhex('0d')
   
    keystatusHt = struct.pack('B',keystatusH)
    keystatusLt = struct.pack('B',keystatusL)
    if COMOFF['text'] == '关闭串口':
        try:
            SNt,CHt = com(entrySN.get(),entryCH.get())
        except TypeError as e:
            messagebox.showwarning('警告','请输入16进制数字')
        keyInhibitonNumt = struct.pack('B',keyInhibitonNum)
        print('keystatusH is4',keystatusH)      
        txBuffer= txBuffer+keystatusHt
        txBuffer= txBuffer+keystatusLt
        txBuffer= txBuffer+keyInhibitonNumt
        txBuffer= txBuffer+SNt
        txBuffer= txBuffer+CHt 
        print(txBuffer)
        ser.write(txBuffer)  #Hex发送5aa55aa5  254
    else:
         messagebox.showwarning('警告','串口未打开')
WriteParams.bind("<Button-1>",UpLoadParams)    
WriteParams.bind("<Button-1>",DownLoadParams)
COMOFF.bind("<Button-1>",SerialOnOFF)


t1 = threading.Thread(target=recv)
t2 = threading.Thread(target=uart_data_analy)
t3 = threading.Thread(target=SerialList)
t4 = threading.Thread(target=SerialOnSuperv)
t1.start()

#if __name__=='__main__':
 
t1 = threading.Thread(target=recv)

t2 = threading.Thread(target=uart_data_analy)
t2.start()
t3.start()
t4.start()
 #t2.pause()

 #uart_data_analy()
##  
win.mainloop()

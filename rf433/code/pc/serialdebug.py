import tkinter as tk
from tkinter import ttk
from tkinter import *
import serial
from  serial.tools import list_ports
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

#创建主窗口
win=tk.Tk()
win.title("串口配置工具")
win.geometry("700x500+500+200")#窗口大小800x800，显示器位置500，500

#增加frame******************上下
frameUpDown = ttk.LabelFrame(win,height = FrameHeight,width = FrameWidth,text = "上下键功能设定")
frameUpDown.grid(column = 0, row = 0,ipadx = IpadxSpace,ipady =IpadySpace,padx = OpadxSpace,pady =OpadySpace)
frameUpDown.place(relx=0,rely=0,x=15,y=20)
ttk.Label(frameUpDown,text="上下关系").grid(column=50,row=0,padx = OpadxSpace,pady =0)#增加标签
ttk.Label(frameUpDown,text="上UP").grid(sticky = W,column=0,row=0,padx = OpadxSpaceSmall,pady =OpadySpaceStr)#增加up标签
ttk.Label(frameUpDown,text="下DOWN").grid(sticky = W,column=0,row=1,padx =OpadxSpaceSmall,pady =OpadySpaceStr)#增加DOWN标签
#增加下拉菜单******************上下
up= tk.StringVar()#增加up下拉菜单
cmbChosen = ttk.Combobox(frameUpDown,width=BoxWidth,textvariable = up)
cmbChosen['value']=('点动','自锁')
cmbChosen.grid(column = 1, row = 0,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosen.current(0)
down= tk.StringVar()#增加down下拉菜单
cmbChosen = ttk.Combobox(frameUpDown,width=BoxWidth,textvariable = down)
cmbChosen['value']=('点动','自锁')
cmbChosen.grid(column = 1, row = 1,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosen.current(0)

UpDown= tk.StringVar()#增加上下关系下拉菜单
cmbChosen = ttk.Combobox(frameUpDown,width=BoxWidthLong,textvariable = UpDown)
cmbChosen['value']=('相互拟制','不拟制')
cmbChosen.grid(column = 50, row = 1,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosen.current(0)



#增加frame******************东西
frameEW = ttk.LabelFrame(win,height = FrameHeight,width = FrameWidth,text = "东西键功能设定")
frameEW.grid(column = 0, row = 4,ipadx = IpadxSpace,ipady =IpadySpace,padx = OpadxSpace,pady =OpadySpace)
frameEW.place(relx=0,rely=0,x=15,y=110)
ttk.Label(frameEW,text="东西关系").grid(column=50,row=0,padx = OpadxSpace,pady =OpadySpaceStr)#增加东西关系标签
ttk.Label(frameEW,text="东EAST").grid(sticky = W,column=0,row=0,padx = OpadxSpace,pady =OpadySpaceStr)#增加东标签
ttk.Label(frameEW,text="西WEST").grid(sticky = W,column=0,row=1,padx =OpadxSpace,pady =OpadySpaceStr)#增加西WEST标签
EAST= tk.StringVar()#增加EAST下拉菜单
cmbChosen = ttk.Combobox(frameEW,width=BoxWidth,textvariable = EAST)
cmbChosen['value']=('点动','自锁')
cmbChosen.grid(column = 1, row = 0,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosen.current(0)
WEST= tk.StringVar()#增加WEST下拉菜单
cmbChosen = ttk.Combobox(frameEW,width=BoxWidth,textvariable = WEST)
cmbChosen['value']=('点动','自锁')
cmbChosen.grid(column = 1, row = 1,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosen.current(0)
EAST_WEST= tk.StringVar()
cmbChosen = ttk.Combobox(frameEW,width=BoxWidthLong,textvariable = EAST_WEST)#增加东西关系下拉菜单
cmbChosen['value']=('相互拟制','不拟制')
cmbChosen.grid(column = 50, row = 1,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosen.current(0)
###增加frame******************南北
frameSOUTH_NORTH = ttk.LabelFrame(win,height = FrameHeight,width = FrameWidth,text = "南北键功能设定")
frameSOUTH_NORTH.grid(column = 0, row = 8,ipadx = 0,ipady =5,padx = OpadxSpace,pady =OpadySpace)
frameSOUTH_NORTH.place(relx=0,rely=0,x=15,y=200)
ttk.Label(frameSOUTH_NORTH,text="南北关系").grid(column=50,row=0,padx = OpadxSpace,pady =OpadySpaceStr)
ttk.Label(frameSOUTH_NORTH,text="南SOUTH").grid(sticky = W,column=0,row=0,padx = OpadxSpaceSmall,pady =OpadySpaceStr)#增加SOUTH标签
ttk.Label(frameSOUTH_NORTH,text="北NORTH").grid(sticky = W,column=0,row=1,padx =OpadxSpaceSmall,pady =OpadySpaceStr)#增加NORTH标签
SOUTH= tk.StringVar()#增加SOUTH下拉菜单
cmbChosen = ttk.Combobox(frameSOUTH_NORTH,width=BoxWidth,textvariable = SOUTH)
cmbChosen['value']=('点动','自锁')
cmbChosen.grid(column = 1, row = 0,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosen.current(0)
NORTH= tk.StringVar()#增加NORTHT下拉菜单
cmbChosen = ttk.Combobox(frameSOUTH_NORTH,width=BoxWidth,textvariable = NORTH)
cmbChosen['value']=('点动','自锁')
cmbChosen.grid(column = 1, row = 1,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosen.current(0)

SOUTH_NORTH= tk.StringVar()#增加南北关系下拉菜单
cmbChosen = ttk.Combobox(frameSOUTH_NORTH,width=BoxWidthLong,textvariable = SOUTH_NORTH)
cmbChosen['value']=('相互拟制','不拟制')
cmbChosen.grid(column = 50, row = 1,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosen.current(0)
#增加frame******************开停机键功能设定
frameSTART_STOP = ttk.LabelFrame(win,height = FrameHeight,width = FrameWidth,text = "开停机键功能设定")
frameSTART_STOP.grid(column = 0, row = 12,ipadx = IpadxSpace,ipady =IpadySpace,padx = OpadxSpace,pady =OpadySpace)
frameSTART_STOP.place(relx=0,rely=0,x=15,y=290)
ttk.Label(frameSTART_STOP,text="开START").grid(sticky = W,column=0,row=0,padx = 12,pady =OpadySpaceStr)#增加开START标签
ttk.Label(frameSTART_STOP,text="停STOP").grid(sticky = W,column=0,row=1,padx =12,pady =OpadySpaceStr)#增加停STOP标签
START= tk.StringVar()#增加开START下拉菜单
cmbChosen = ttk.Combobox(frameSTART_STOP,width=BoxWidthLong,textvariable = START)
cmbChosen['value']=('备用点动','备用自锁')
cmbChosen.grid(column = 1, row = 0,padx = 0,pady =OpadySpaceStr)
cmbChosen.current(0)
STOP= tk.StringVar()#增加停STOP下拉菜单
cmbChosen = ttk.Combobox(frameSTART_STOP,width=BoxWidthLong,textvariable = STOP)
cmbChosen['value']=('关机')
cmbChosen.grid(column = 1, row = 1,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosen.current(0)
###增加frame******************OUT1,OUT2
frameOUT1_OUT2 = tk.LabelFrame(win,height = FrameHeight,width = FrameWidth,text = "OUT1  OUT2键功能设定")
frameOUT1_OUT2.grid(column = 0, row = 16,ipadx = 0,ipady =5,padx = OpadxSpace,pady =OpadySpace)
frameOUT1_OUT2.place(relx=0,rely=0,x=15,y=380)
ttk.Label(frameOUT1_OUT2,text="OUT1 OUT2关系").grid(column=50,row=0,padx = OpadxSpace,pady =OpadySpaceStr)
ttk.Label(frameOUT1_OUT2,text="OUT1").grid(sticky = W,column=0,row=0,padx = 20,pady =OpadySpaceStr)#增加OUT1标签
ttk.Label(frameOUT1_OUT2,text="OUT2").grid(sticky = W,column=0,row=1,padx =20,pady =OpadySpaceStr)#增加OUT2标签
OUT1= tk.StringVar()#增加OUT1下拉菜单
cmbChosen = ttk.Combobox(frameOUT1_OUT2,width=BoxWidth,textvariable = OUT1)
cmbChosen['value']=('点动','自锁')
cmbChosen.grid(column = 1, row = 0,padx = 13,pady =OpadySpaceStr)
cmbChosen.current(0)
OUT2= tk.StringVar()#增加OUT2下拉菜单
cmbChosen = ttk.Combobox(frameOUT1_OUT2,width=BoxWidth,textvariable = OUT2)
cmbChosen['value']=('点动','自锁')
cmbChosen.grid(column = 1, row = 1,padx = 13,pady =OpadySpaceStr)
cmbChosen.current(0)
OUT1_OUT2= tk.StringVar()#增加OUT1 OUT2下拉菜单
cmbChosen = ttk.Combobox(frameOUT1_OUT2,width=BoxWidthLong,textvariable = OUT1_OUT2)
cmbChosen['value']=('相互拟制','不拟制')
cmbChosen.grid(column = 50, row = 1,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosen.current(0)
#增加frame******************OUT3,OUT4
frameOUT3_OUT4 = tk.LabelFrame(win,height = FrameHeight,width = FrameWidth,text = "OUT  OUT4键功能设定")
frameOUT3_OUT4.grid(column = 4, row = 16,ipadx = 0,ipady =5,padx = OpadxSpace,pady =OpadySpace)
frameOUT3_OUT4.place(relx=0,rely=0,x=325,y=380)
ttk.Label(frameOUT3_OUT4,text="OUT3 OUT4关系").grid(column=50,row=0,padx = OpadxSpace,pady =OpadySpaceStr)
ttk.Label(frameOUT3_OUT4,text="OUT3").grid(sticky = W,column=0,row=0,padx = OpadxSpace,pady =OpadySpaceStr)#增加OUT3标签
ttk.Label(frameOUT3_OUT4,text="OUT4").grid(sticky = W,column=0,row=1,padx =OpadxSpace,pady =OpadySpaceStr)#增加OUT4标签
OUT3= tk.StringVar()#增加OUT3下拉菜单
cmbChosen = ttk.Combobox(frameOUT3_OUT4,width=BoxWidth,textvariable = OUT3)
cmbChosen['value']=('点动','自锁')
cmbChosen.grid(column = 1, row = 0,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosen.current(0)
OUT4= tk.StringVar()#增加OUT4下拉菜单
cmbChosenOUT4 = ttk.Combobox(frameOUT3_OUT4,width=BoxWidth,textvariable = OUT4)
cmbChosenOUT4['value']=('点动','自锁')
cmbChosenOUT4.grid(column = 1, row = 1,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosenOUT4.current(0)
OUT3_OUT4= tk.StringVar()#增加OUT3 OUT4下拉菜单
cmbChosen = ttk.Combobox(frameOUT3_OUT4,width=BoxWidthLong,textvariable = OUT3_OUT4)
cmbChosen['value']=('相互拟制','不拟制')
cmbChosen.grid(column = 50, row = 1,padx = OpadxSpace,pady =OpadySpaceStr)
cmbChosen.current(0)
###增加frame******************SN_CH功能设定
frameSN_CH = ttk.LabelFrame(win,height = FrameHeightLarge,width =FrameWidth,text = "读取SN、CH码")
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
COMOFF = tk.Button(frameCOMX,height =2,width = BoxWidthLong,text="关闭串口")
COMOFF.grid(column = 12, row = 0,sticky=W)
COMOFF.place(relx=0,rely=0,x=110,y=1)
WAKECOM = tk.Button(frameCOMX,height = 2,width = BoxWidthLong,text="唤醒串口")
WAKECOM.place(relx=0,rely=0,x=200,y=1)
#Entry控件布局
ttk.Label(frameSN_CH,text="SN:").grid(column=1,row=1,padx = 30,pady =20)
ttk.Label(frameSN_CH,text="CH:").grid(column=1,row=5,padx = 30,pady =40)
entry1=Entry(frameSN_CH,width = 10,state = 'disabled')
entry2=Entry(frameSN_CH,width = 10,state = 'disabled')
entry1.place(relx=0,rely=0,x=70,y=20)
entry2.place(relx=0,rely=0,x=70,y=100)
frameUpDown.grid_propagate(0)
frameEW.grid_propagate(0)
frameSOUTH_NORTH.grid_propagate(0)
frameSTART_STOP.grid_propagate(0)
frameOUT1_OUT2.grid_propagate(0)
frameOUT3_OUT4.grid_propagate(0)
frameSN_CH.grid_propagate(0)
frameFORMAT.grid_propagate(0)
frameCOMX.grid_propagate(0)
#串口
data=[]
plist = list(serial.tools.list_ports.comports())
if len(plist) <= 0:
  print ("no serial")
else :
  data = [0 for i in range(len(plist))]
  for i in range(0,len(plist)) :
     port_list_0= list(plist[i])
     #ser = serial.Serial(port_serial,9600,timeout = 60)
     data[i]=port_list_0[0]
cmbChosenCOMX["values"] = data
cmbChosenCOMX.current(0)



print(cmbChosenCOMX.get())
win.mainloop()

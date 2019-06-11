import tkinter as tk
from tkinter import ttk
from tkinter import *
#创建主窗口
win=tk.Tk()
win.title("串口配置工具")
win.geometry("1000x650+100+100")#窗口大小800x800，显示器位置500，500

#增加frame******************上下
frameUpDown = ttk.LabelFrame(win,height = 90,width = 300,text = "上下键功能设定")
frameUpDown.grid(column = 0, row = 0,ipadx = 0,ipady =5,padx = 10,pady =10)
#增加标签
ttk.Label(frameUpDown,text="上下关系").grid(column=50,row=0,padx = 10,pady =10)
ttk.Label(frameUpDown,text="上UP").grid(sticky = W,column=0,row=0,padx = 10,pady =0)#增加up标签
ttk.Label(frameUpDown,text="下DOWN").grid(sticky = W,column=0,row=1,padx =10,pady =0)#增加DOWN标签

up= tk.StringVar()#增加up下拉菜单
cmbChosen = ttk.Combobox(frameUpDown,width=6,textvariable = up)
cmbChosen['value']=('点动','自锁')
cmbChosen.grid(column = 1, row = 0,padx = 15,pady =5)
cmbChosen.current(0)

down= tk.StringVar()#增加down下拉菜单
cmbChosen = ttk.Combobox(frameUpDown,width=6,textvariable = down)
cmbChosen['value']=('点动','自锁')
cmbChosen.grid(column = 1, row = 1,padx = 15,pady =5)
cmbChosen.current(0)

UpDown= tk.StringVar()#增加上下关系下拉菜单
cmbChosen = ttk.Combobox(frameUpDown,width=8,textvariable = UpDown)
cmbChosen['value']=('相互拟制','不拟制')
cmbChosen.grid(column = 50, row = 1,padx = 15,pady =5)
cmbChosen.current(0)



#增加frame******************东西
frameEW = ttk.LabelFrame(win,height = 90,width = 300,text = "东西键功能设定")
frameEW.grid(column = 0, row = 4,ipadx = 0,ipady =5,padx = 10,pady =10)

ttk.Label(frameEW,text="东西关系").grid(column=50,row=0,padx = 10,pady =10)#增加东西关系标签
ttk.Label(frameEW,text="东EAST").grid(sticky = W,column=0,row=0,padx = 10,pady =0)#增加东标签
ttk.Label(frameEW,text="西WEST").grid(sticky = W,column=0,row=1,padx =10,pady =0)#增加西WEST标签

EAST= tk.StringVar()#增加EAST下拉菜单
cmbChosen = ttk.Combobox(frameEW,width=6,textvariable = EAST)
cmbChosen['value']=('点动','自锁')
cmbChosen.grid(column = 1, row = 0,padx = 15,pady =5)
cmbChosen.current(0)

WEST= tk.StringVar()#增加WEST下拉菜单
cmbChosen = ttk.Combobox(frameEW,width=6,textvariable = WEST)
cmbChosen['value']=('点动','自锁')
cmbChosen.grid(column = 1, row = 1,padx = 15,pady =5)
cmbChosen.current(0)

EAST_WEST= tk.StringVar()
cmbChosen = ttk.Combobox(frameEW,width=8,textvariable = EAST_WEST)#增加东西关系下拉菜单
cmbChosen['value']=('相互拟制','不拟制')
cmbChosen.grid(column = 50, row = 1,padx = 15,pady =5)
cmbChosen.current(0)



#增加frame******************南北
frameSOUTH_NORTH = ttk.LabelFrame(win,height = 90,width = 300,text = "南北键功能设定")
frameSOUTH_NORTH.grid(column = 0, row = 8,ipadx = 0,ipady =5,padx = 10,pady =10)

ttk.Label(frameSOUTH_NORTH,text="南北关系").grid(column=50,row=0,padx = 10,pady =10)
ttk.Label(frameSOUTH_NORTH,text="南SOUTH").grid(sticky = W,column=0,row=0,padx = 10,pady =0)#增加SOUTH标签
ttk.Label(frameSOUTH_NORTH,text="北NORTH").grid(sticky = W,column=0,row=1,padx =10,pady =0)#增加NORTH标签

SOUTH= tk.StringVar()#增加SOUTH下拉菜单
cmbChosen = ttk.Combobox(frameSOUTH_NORTH,width=6,textvariable = SOUTH)
cmbChosen['value']=('点动','自锁')
cmbChosen.grid(column = 1, row = 0,padx = 15,pady =5)
cmbChosen.current(0)

NORTH= tk.StringVar()#增加NORTHT下拉菜单
cmbChosen = ttk.Combobox(frameSOUTH_NORTH,width=6,textvariable = NORTH)
cmbChosen['value']=('点动','自锁')
cmbChosen.grid(column = 1, row = 1,padx = 15,pady =5)
cmbChosen.current(0)

SOUTH_NORTH= tk.StringVar()#增加南北关系下拉菜单
cmbChosen = ttk.Combobox(frameSOUTH_NORTH,width=8,textvariable = SOUTH_NORTH)
cmbChosen['value']=('相互拟制','不拟制')
cmbChosen.grid(column = 50, row = 1,padx = 15,pady =5)
cmbChosen.current(0)


#增加frame******************开停机键功能设定
frameSTART_STOP = ttk.LabelFrame(win,height = 90,width = 300,text = "开停机键功能设定")
frameSTART_STOP.grid(column = 0, row = 12,ipadx = 0,ipady =5,padx = 10,pady =10)


ttk.Label(frameSTART_STOP,text="开START").grid(sticky = W,column=0,row=0,padx = 10,pady =0)#增加开START标签
ttk.Label(frameSTART_STOP,text="停STOP").grid(sticky = W,column=0,row=1,padx =10,pady =0)#增加停STOP标签

START= tk.StringVar()#增加开START下拉菜单
cmbChosen = ttk.Combobox(frameSTART_STOP,width=8,textvariable = START)
cmbChosen['value']=('备用点动','备用自锁')
cmbChosen.grid(column = 1, row = 0,padx = 15,pady =5)
cmbChosen.current(0)

STOP= tk.StringVar()#增加停STOP下拉菜单
cmbChosen = ttk.Combobox(frameSTART_STOP,width=8,textvariable = STOP)
cmbChosen['value']=('关机')
cmbChosen.grid(column = 1, row = 1,padx = 15,pady =5)
cmbChosen.current(0)



#增加frame******************OUT1,OUT2
frameOUT1_OUT2 = tk.LabelFrame(win,height = 90,width = 300,text = "OUT1  OUT2键功能设定")
frameOUT1_OUT2.grid(column = 0, row = 16,ipadx = 0,ipady =5,padx = 10,pady =10)

ttk.Label(frameOUT1_OUT2,text="OUT1 OUT2关系").grid(column=50,row=0,padx = 10,pady =10)
ttk.Label(frameOUT1_OUT2,text="OUT1").grid(sticky = W,column=0,row=0,padx = 10,pady =0)#增加OUT1标签
ttk.Label(frameOUT1_OUT2,text="OUT2").grid(sticky = W,column=0,row=1,padx =10,pady =0)#增加OUT2标签

OUT1= tk.StringVar()#增加OUT1下拉菜单
cmbChosen = ttk.Combobox(frameOUT1_OUT2,width=6,textvariable = OUT1)
cmbChosen['value']=('点动','自锁')
cmbChosen.grid(column = 1, row = 0,padx = 15,pady =5)
cmbChosen.current(0)

OUT2= tk.StringVar()#增加OUT2下拉菜单
cmbChosen = ttk.Combobox(frameOUT1_OUT2,width=6,textvariable = OUT2)
cmbChosen['value']=('点动','自锁')
cmbChosen.grid(column = 1, row = 1,padx = 15,pady =5)
cmbChosen.current(0)

OUT1_OUT2= tk.StringVar()#增加OUT1 OUT2下拉菜单
cmbChosen = ttk.Combobox(frameOUT1_OUT2,width=8,textvariable = OUT1_OUT2)
cmbChosen['value']=('相互拟制','不拟制')
cmbChosen.grid(column = 50, row = 1,padx = 15,pady =5)
cmbChosen.current(0)

#增加frame******************OUT3,OUT4
frameOUT3_OUT4 = tk.LabelFrame(win,height = 90,width = 300,text = "OUT  OUT4键功能设定")
frameOUT3_OUT4.grid(column = 4, row = 16,ipadx = 0,ipady =5,padx = 10,pady =10)

ttk.Label(frameOUT3_OUT4,text="OUT3 OUT4关系").grid(column=50,row=0,padx = 10,pady =10)
ttk.Label(frameOUT3_OUT4,text="OUT3").grid(sticky = W,column=0,row=0,padx = 10,pady =0)#增加OUT3标签
ttk.Label(frameOUT3_OUT4,text="OUT4").grid(sticky = W,column=0,row=1,padx =10,pady =0)#增加OUT4标签

OUT3= tk.StringVar()#增加OUT3下拉菜单
cmbChosen = ttk.Combobox(frameOUT3_OUT4,width=6,textvariable = OUT3)
cmbChosen['value']=('点动','自锁')
cmbChosen.grid(column = 1, row = 0,padx = 15,pady =5)
cmbChosen.current(0)

OUT4= tk.StringVar()#增加OUT4下拉菜单
cmbChosen = ttk.Combobox(frameOUT3_OUT4,width=6,textvariable = OUT4)
cmbChosen['value']=('点动','自锁')
cmbChosen.grid(column = 1, row = 1,padx = 15,pady =5)
cmbChosen.current(0)

OUT3_OUT4= tk.StringVar()#增加OUT3 OUT4下拉菜单
cmbChosen = ttk.Combobox(frameOUT3_OUT4,width=8,textvariable = OUT3_OUT4)
cmbChosen['value']=('相互拟制','不拟制')
cmbChosen.grid(column = 50, row = 1,padx = 15,pady =5)
cmbChosen.current(0)


#增加frame******************SN_CH功能设定
frameSN_CH = ttk.LabelFrame(win,height = 225,width =300,text = "读取SN、CH码")
frameSN_CH.grid(column = 4, row = 8,ipadx = 0,ipady =5,padx = 5,pady =10)
frameSN_CH.place(relx=0,rely=0,x=330,y=250)



#增加frame******************格式化数据卡
frameFORMAT = ttk.LabelFrame(win,height = 225,width =300,text = "数据卡格式化")
frameFORMAT.grid(column = 4, row = 8,ipadx = 0,ipady =5,padx = 5,pady =10)
frameFORMAT.place(relx=0,rely=0,x=330,y=20)


  # 创建button
ReadParams = tk.Button(frameSN_CH,height =3,width = 8,text="读参数")
ReadParams.grid(column = 12, row = 0,sticky=W)
ReadParams.place(relx=0,rely=0,x=220,y=40)

WriteParams = tk.Button(frameSN_CH,height = 3,width = 8,text="下载参数")
WriteParams.place(relx=0,rely=0,x=220,y=120)

FORMAT = tk.Button(frameFORMAT,height =10,width = 35,text="数据卡格式化")
FORMAT.grid(column = 12, row = 0)
FORMAT.place(relx=0,rely=0,x=20,y=10)

#增加frame******************OUT1,OUT2
frameCOMX = tk.LabelFrame(win,height = 180,width = 200,text = "通讯参数")
frameCOMX.grid(column = 45, row = 0,ipadx = 0,ipady =5,padx = 10,pady =10)

COMX= tk.StringVar(frameCOMX,'COM1')#增加OUT1下拉菜单
cmbChosen = ttk.Combobox(frameCOMX,width=6,textvariable = COMX)
#cmbChosen['value']=('点动','自锁')
cmbChosen.grid(column = 1, row = 0,padx = 15,pady =5)


COMOFF = tk.Button(frameCOMX,height =3,width = 8,text="关闭串口")
COMOFF.grid(column = 12, row = 0,sticky=W)
COMOFF.place(relx=0,rely=0,x=220,y=40)

WAKECOM = tk.Button(frameCOMX,height = 3,width = 8,text="唤醒串口")
WAKECOM.place(relx=0,rely=0,x=220,y=120)


frameSN_CH.grid_propagate(0)
frameUpDown.grid_propagate(0)
frameEW.grid_propagate(0)
frameSOUTH_NORTH.grid_propagate(0)
frameSTART_STOP.grid_propagate(0)
frameOUT1_OUT2.grid_propagate(0)
frameOUT3_OUT4.grid_propagate(0)
frameFORMAT.grid_propagate(0)
frameCOMX.grid_propagate(0) 
#cmbChosen.current(0)
#entryvar=IntVar()
#Entry(win,textvariable=entryvar).grid(row=0,column=1)


 # 创建了两个button，wuya为在名字为wuya的窗口中显示，text为button上显示的文字
##def printInfo():
##    #清理entry2
##    entry2.delete(0, END)
##    #根据输入半径计算面积
##    R=int(entry1.get())
##    S= 3.1415926*R*R
##    entry2.insert(10, S)
##    #清空entry2控件
##    entry1.delete(0, END)
    
###初始化Tk()
##myWindow = Tk()
###设置标题
##myWindow.title('Python GUI Learning')
## 
###标签控件布局
##Label(myWindow, text="input").grid(row=0)
##Label(myWindow, text="output").grid(row=1)
## 
###Entry控件布局
##entry1=Entry(myWindow)
##entry2=Entry(myWindow)
##entry1.grid(row=0, column=1)
##entry2.grid(row=1, column=1)
 




win.mainloop()

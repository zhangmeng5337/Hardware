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
## 
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
'''===============================================Frame3===================================='''
Text_send = ttk.Entry(frame3, width=50, font=("微软雅黑", 10), textvariable=Send)
Text_send.grid(row=0, column=0)
'''===============================================Frame4===================================='''
def ClearData():
    Send.set("")
    Text_get.delete(0.0, END)
BtnClear = ttk.Button(frame3, width=10, text='清空', command=ClearData)
BtnClear.grid(row=0, column=1)
 
def GetData(txt):
    mySerial.write(bytes(txt.get(), encoding='utf8'))
    wait = mySerial.inWaiting()
    if wait:
        res_data = mySerial.readline()
        Text_get.insert(END, res_data + bytes('\n', encoding='utf8'))
        mySerial.flushInput()
 
def SendData():
    if mySerial.isOpen():
        t = threading.Thread(target=GetData, args=(Text_send, ))
        t.setDaemon(True)
        t.start()
        t.join()
 
 
BtnSend = ttk.Button(frame3, width=10, text='发送', command=SendData)
BtnSend.grid(row=0, column=2)
'''=======================================Frame5============================='''
Label_current = Label(frame7, text="参考电流(A)", width=15, font=("微软雅黑", 10))
Label_current.grid(row=0, column=0, padx=5, pady=5)
current = StringVar()
current.set('1.88')
EntryReferenceCurrent = ttk.Entry(frame7, textvariable=current, width=15)
EntryReferenceCurrent.grid(row=0, column=1, padx=5, pady=5)
'''***************************************************************************'''
Label_voltage = Label(frame7, text="参考电压(V)", width=15, font=("微软雅黑", 10))
Label_voltage.grid(row=1, column=0, padx=5, pady=5)
voltage = StringVar()
voltage.set('31.7')
EntryReferenceVoltage = ttk.Entry(frame7, textvariable=voltage, width=15)
EntryReferenceVoltage.grid(row=1, column=1, padx=5, pady=5)
'''****************************************************************************'''
Label_battery = Label(frame7, text="参考电量(V)", width=15, font=("微软雅黑", 10))
Label_battery.grid(row=2, column=0, padx=5, pady=5)
battery = StringVar()
battery.set('4.2')
EntryReferenceBattery = ttk.Entry(frame7, textvariable=battery, width=15)
EntryReferenceBattery.grid(row=2, column=1, padx=5, pady=5)
'''*****************************************************************************'''
Label_temperature = Label(frame7, text="参考温度(℃)", width=15, font=("微软雅黑", 10))
Label_temperature.grid(row=3, column=0, padx=5, pady=5)
temperature = StringVar()
temperature.set('37')
EntryReferenceTemperature = ttk.Entry(frame7, textvariable=temperature, width=15)
EntryReferenceTemperature.grid(row=3, column=1, padx=5, pady=5)
'''*****************************************************************************'''
Label_radiation = Label(frame7, text="参考辐照(W/m²)", width=15, font=("微软雅黑", 10))
Label_radiation.grid(row=4, column=0, padx=5, pady=5)
radiation = StringVar()
radiation.set('1000')
EntryReferenceRadiation = ttk.Entry(frame7, textvariable=radiation, width=15)
EntryReferenceRadiation.grid(row=4, column=1, padx=5, pady=5)
'''*****************************************************************************'''
def WriteData(data):
    mySerial.write(bytes(data, encoding='utf8'))
    wait = mySerial.inWaiting()
    if wait:
        order = mySerial.readline()
        Text_get.insert(END, order + bytes('\n', encoding='utf8'))
 
def AddData(data):
    if len(data) <= 5:
        res = '0' * (5 - len(data)) + data
        return res
    else:
        messagebox.showinfo('提示信息', '数据错误，请检查!')
 
 
def ClickVoltage():
    dataVoltage = AddData(txtVoltage.get())
    if dataVoltage == '00000':
        messagebox.showinfo('提示消息', '数据错误，请连接参考源进行测试!')
    else:
        info = 'ZCV' + dataVoltage + 'S'
        t = threading.Thread(target=WriteData, args=(info, ))
        t.setDaemon(True)
        t.start()
        t.join()
BtnVoltage = ttk.Button(frame7, text="写入电压", width=15, command=ClickVoltage)
BtnVoltage.grid(row=5, column=0, padx=5, pady=5)
txtVoltage = StringVar()
EntryVoltage = ttk.Entry(frame7, textvariable=txtVoltage, width=15)
EntryVoltage.grid(row=5, column=1, padx=5, pady=5)
'''******************************************************************************'''
def ClickCurrent():
    dataCurrent = AddData(txtCurrent.get())
    if dataCurrent == '00000':
        messagebox.showinfo('提示消息', '数据错误，请连接参考源进行测试!')
    else:
        info = 'ZCI' + dataCurrent + 'S'
        t = threading.Thread(target=WriteData, args=(info, ))
        t.setDaemon(True)
        t.start()
        t.join()
BtnCurrent = ttk.Button(frame7, text="写入电流", width=15, command=ClickCurrent)
BtnCurrent.grid(row=6, column=0, padx=5, pady=5)
txtCurrent = StringVar()
EntryCurrent = ttk.Entry(frame7, textvariable=txtCurrent, width=15)
EntryCurrent.grid(row=6, column=1, padx=5, pady=5)
'''*****************************************************************************'''
def ClickRadiation():
    dataRadiation = AddData(txtRadiation.get())
    if dataRadiation == '00000':
        messagebox.showinfo('提示消息', '数据错误，请连接参考源进行测试!')
    else:
        info = 'ZCR' + dataRadiation + 'S'
        t = threading.Thread(target=WriteData, args=(info, ))
        t.setDaemon(True)
        t.start()
        t.join()
BtnRadiation = ttk.Button(frame7, text="写入辐照", width=15, command=ClickRadiation)
BtnRadiation.grid(row=7, column=0, padx=5, pady=5)
txtRadiation = StringVar()
EntryRadiation = ttk.Entry(frame7, textvariable=txtRadiation, width=15)
EntryRadiation.grid(row=7, column=1, padx=5, pady=5)
'''*****************************************************************************'''
def ClickTemperature():
    dataTemperature = AddData(txtTemperature.get())
    if dataTemperature == '00000':
        messagebox.showinfo('提示消息', '数据错误，请连接参考源进行测试!')
    else:
        info = 'ZCT' + dataTemperature + 'S'
        t = threading.Thread(target=WriteData, args=(info,))
        t.setDaemon(True)
        t.start()
        t.join()
BtnTemperature = ttk.Button(frame7, text="写入温度", width=15, command=ClickTemperature)
BtnTemperature.grid(row=8, column=0, padx=5, pady=5)
txtTemperature = StringVar()
EntryTemperature = ttk.Entry(frame7, textvariable=txtTemperature, width=15)
EntryTemperature.grid(row=8, column=1, padx=5, pady=5)
'''*********************************************************************************'''
def ClickBattery():
    dataBattery = AddData(txtBattery.get())
    if dataBattery == '00000':
        messagebox.showinfo('提示消息', '数据错误，请连接参考源进行测试!')
    else:
        info = 'ZCP' + dataBattery + 'S'
        t = threading.Thread(target=WriteData, args=(info, ))
        t.setDaemon(True)
        t.start()
        t.join()
BtnBattery = ttk.Button(frame7, text="写入电量", width=15, command=ClickBattery)
BtnBattery.grid(row=9, column=0, padx=5, pady=5)
txtBattery = StringVar()
EntryBattery = ttk.Entry(frame7, textvariable=txtBattery, width=15)
EntryBattery.grid(row=9, column=1, padx=5, pady=5)
'''*****************************************************************************'''
def WriteDataOnce():
    infoVoltage = ''
    infoCurrent = ''
    infoTemperature = ''
    infoRadiation = ''
    infoBattery = ''
 
    dataVoltage = AddData(txtVoltage.get())
    if dataVoltage == '00000':
        messagebox.showinfo('提示消息', '电压数据异常!')
    else:
        infoVoltage = 'ZCV' + dataVoltage + 'S'
 
    dataCurrent = AddData(txtCurrent.get())
    if dataCurrent == '00000':
        messagebox.showinfo('提示消息', '电流数据异常!')
    else:
        infoCurrent = 'ZCI' + dataCurrent + 'S'
 
    dataRadiation = AddData(txtRadiation.get())
    if dataRadiation == '00000':
        messagebox.showinfo('提示消息', '辐照数据异常!')
    else:
        infoRadiation = 'ZCR' + dataRadiation + 'S'
 
    dataTemperature = AddData(txtTemperature.get())
    if dataTemperature == '00000':
        messagebox.showinfo('提示消息', '温度数据异常!')
    else:
        infoTemperature = 'ZCT' + dataTemperature + 'S'
 
    dataBattery = AddData(txtBattery.get())
    if dataBattery == '00000':
        messagebox.showinfo('提示消息', '电量数据异常!')
    else:
        infoBattery = 'ZCP' + dataBattery + 'S'
 
    # threads = []
    if infoVoltage and infoCurrent and infoRadiation and infoTemperature and infoBattery:
        for i in [infoVoltage, infoCurrent, infoRadiation, infoTemperature, infoBattery]:
            t = threading.Thread(target=WriteData, args=(i, ))
            t.setDaemon(True)
            t.start()
            time.sleep(0.8)#单片机执行指令
            # threads.append(t)
        # for thread in threads:
        #     thread.join(1)
    else:
        messagebox.showinfo('提示消息', '有数据异常，无法进行计算!')
BtnSet = Button(frame7, text="一键写入", command=WriteDataOnce, width=10, bg="#1AAD19", fg="white", font=("微软雅黑", 12, "bold"))
BtnSet.grid(row=10, column=1, ipady=8, padx=0, pady=5)
'''*****************************************************************************'''
def ClickBtn1():
    if mySerial.isOpen():
        mySerial.write(bytes('ZREADMACS', encoding='utf8'))
        wait = mySerial.inWaiting()
        if wait:
            res_data = mySerial.readline()
            Text_get.insert(END, res_data + bytes('\n', encoding='utf8'))
            mySerial.flushInput()
Btn1 = Button(frame7, text="原始参数", width=10, command=ClickBtn1, bg="#1AAD19", fg="white", font=("微软雅黑", 12, "bold"))
Btn1.grid(row=11, column=0, ipady=8, padx=5, pady=5)
'''********************************************************************************'''
def CheckParameters():
    global data
    data = ''
    mySerial.write(bytes('ZIVPOINTS', encoding='utf8'))
    while True:
        res = mySerial.readline()
        if res:
            data = str(res)
            break
    if data:
        referVoltage = voltage.get()
        referCurrent = current.get()
        referTemperature = temperature.get()
        referRadiation = radiation.get()
        referBattery = battery.get()
        checkVoltage = data[18:23]
        checkCurrent = data[23:28]
        checkTemperature = data[28:33]
        checkBattery = data[33:38]
        checkRadiation = data[13: 18]
        dataVoltage = data[43:48]
        dataCurrent = data[48:53]
        dataTemperature = data[53:58]
        dataRadiation = data[38:43]
        dataBattery = data[58:63]
        res_voltage = round(int(checkVoltage) * int(dataVoltage) * 2.5 / (65536 * 1000), 1)
        res_current = round(int(checkCurrent) * int(dataCurrent) * 2.5 / (65536 * 1000), 1)
        res_temperature = round(int(checkTemperature) * int(dataTemperature) / (1000 * 100), 1)
        res_battery = round(int(checkBattery) * int(dataBattery) * 2.5 / (65536 * 1000), 1)
        res_radiation = round(int(checkRadiation) * int(dataRadiation) * 2.5 / (65536 * 0.1 * 0.12 * 1000), 1)
        percent_voltage = round((res_voltage - float(referVoltage)) / float(referVoltage) * 100, 1)
        percent_current = round((res_current - float(referCurrent)) / float(referCurrent) * 100, 1)
        percent_temperature = round((res_temperature - float(referTemperature)) / float(referTemperature) * 100, 1)
        percent_radiation = round((res_radiation - float(referRadiation)) / float(referRadiation) * 100, 1)
        percent_battery = round((res_battery - float(referBattery)) / float(referBattery) * 100, 1)
        info = '电压:' + str(res_voltage) + 'V\n' + '误差:' + str(percent_voltage) + '%\n' + '电流:' + str(res_current) +\
               'A\n' + '误差:' + str(percent_current) + '%\n' + '温度:' + str(res_temperature) + '℃\n' + '误差:' +\
               str(percent_temperature) + '%\n' + '辐照强度:' + str(res_radiation) + 'W\\m²\n' + '误差:' +\
               str(percent_radiation) + '%\n' + '电量:' + str(res_battery) + 'V\n' + '误差:' + str(percent_battery) + '%\n'
        Text_get.insert(END, info)
 
BtnTest = Button(frame7, text="参数检验", width=10, command=CheckParameters, bg="#1AAD19", fg="white", font=("微软雅黑", 12, "bold"))
BtnTest.grid(row=11, column=1, ipady=8, padx=5, pady=5)
'''********************************************************************************'''
def SetData():
    global data
    data = ''
    mySerial.write(bytes('ZIVPOINTS', encoding='utf8'))
    while True:
        res = mySerial.readline()
        if res:
            data = str(res)
            break
    if data:
        dataVoltage = data[43:48]
        dataCurrent = data[48:53]
        dataTemperature = data[53:58]
        dataRadiation = data[38:43]
        dataBattery = data[58:63]
        res_voltage = round(float(voltage.get()) * 65536 * 1000 / (2.5 * int(dataVoltage)))
        res_current = round(float(current.get()) * 65536 * 1000 / (2.5 * int(dataCurrent)))
        res_temperature = round(float(temperature.get()) * 100 * 1000 / (int(dataTemperature)))
        res_radiation = round(float(radiation.get()) * 65536 * 1000 * 0.1 * 0.12 / (2.5 * int(dataRadiation)))
        res_battery = round(float(battery.get()) * 65536 * 1000 / (2.5 * int(dataBattery)))
        txtVoltage.set(str(res_voltage))
        txtCurrent.set(str(res_current))
        txtTemperature.set(str(res_temperature))
        txtRadiation.set(str(res_radiation))
        txtBattery.set(str(res_battery))
    else:
        messagebox.showinfo('提示消息', '未获取初始数据，请检查!')
 
def ClickBtn2():
    if mySerial.isOpen():
        t = threading.Thread(target=SetData)
        t.setDaemon(True)
        t.start()
Btn2 = Button(frame7, text="校正计算", width=10, command=ClickBtn2, bg="#1AAD19", fg="white", font=("微软雅黑", 12, "bold"))
Btn2.grid(row=10, column=0, ipady=8, padx=5, pady=5)
'''**********************************************************************************'''
def ClickBtnGuest():
    if mySerial.isOpen():
        if len(txtGuest.get()) != 5:
            messagebox.showinfo('提示消息', '请输入正确参数!')
        else:
            info = 'ZCL' + txtGuest.get() + 'S'
            t = threading.Thread(target=WriteData, args=(info, ))
            t.setDaemon(True)
            t.start()
            t.join()
BtnGuest = ttk.Button(frame6, text="客户信息", width=15, command=ClickBtnGuest)
BtnGuest.grid(row=0, column=0, padx=5, pady=5)
txtGuest = StringVar()
txtGuest.set("TESTR")
EntryGuest = ttk.Entry(frame6, textvariable=txtGuest, width=15)
EntryGuest.grid(row=0, column=1, padx=5, pady=5)
'''**************************************************************************************'''
def ClickBtnEquipment():
    if mySerial.isOpen():
        if len(txtEquipment.get()) != 5:
            messagebox.showinfo('提示消息', '请输入正确参数!')
        else:
            info = 'ZCH' + txtEquipment.get() + 'S'
            t = threading.Thread(target=WriteData, args=(info, ))
            t.setDaemon(True)
            t.start()
            t.join()
BtnEquipment = ttk.Button(frame6, text="设备编号", width=15, command=ClickBtnEquipment)
BtnEquipment.grid(row=1, column=0, padx=5, pady=5)
txtEquipment = StringVar()
txtEquipment.set("R0006")
EntryEquipment = ttk.Entry(frame6, textvariable=txtEquipment, width=15)
EntryEquipment.grid(row=1, column=1, padx=5, pady=5)
'''********************************************************************************'''
def ClickBtnTime():
    if mySerial.isOpen():
        if len(txtTime.get()) != 5:
            messagebox.showinfo('提示消息', '请输入正确参数!')
        else:
            info = 'ZCD' + txtTime.get() + 'S'
            t = threading.Thread(target=WriteData, args=(info, ))
            t.setDaemon(True)
            t.start()
            t.join()
BtnTime = ttk.Button(frame6, text="设置时间", width=15, command=ClickBtnTime)
BtnTime.grid(row=2, column=0, padx=5, pady=5)
txtTime = StringVar()
txtTime.set("00010")
EntryTime = ttk.Entry(frame6, textvariable=txtTime, width=15)
EntryTime.grid(row=2, column=1, padx=5, pady=5)
 
 
root.mainloop()

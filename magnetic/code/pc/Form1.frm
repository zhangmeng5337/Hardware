VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.1#0"; "mscomm32.ocx"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   7980
   ClientLeft      =   120
   ClientTop       =   465
   ClientWidth     =   10440
   LinkTopic       =   "Form1"
   ScaleHeight     =   7980
   ScaleWidth      =   10440
   StartUpPosition =   3  '窗口缺省
   Begin VB.TextBox RXCount 
      Height          =   270
      Left            =   6600
      TabIndex        =   53
      Text            =   "接收:0"
      Top             =   7560
      Width           =   1450
   End
   Begin VB.TextBox TXCount 
      Height          =   270
      Left            =   4560
      TabIndex        =   52
      Text            =   "发送:0"
      Top             =   7560
      Width           =   1450
   End
   Begin VB.CheckBox Check3 
      Caption         =   "保存到文件"
      Height          =   375
      Left            =   8520
      TabIndex        =   18
      Top             =   7200
      Width           =   1575
   End
   Begin VB.CheckBox ChkHexSend 
      Caption         =   "16进制"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   10.5
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   8520
      TabIndex        =   14
      Top             =   6840
      Width           =   1575
   End
   Begin VB.CheckBox Check1 
      Caption         =   "循环发送"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   10.5
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   8520
      TabIndex        =   13
      Top             =   6480
      Width           =   1575
   End
   Begin VB.TextBox TxPeriod 
      Height          =   375
      Left            =   8520
      TabIndex        =   11
      Text            =   "Text2"
      Top             =   6000
      Width           =   975
   End
   Begin VB.CommandButton Command3 
      Caption         =   "清空"
      Height          =   375
      Left            =   9500
      TabIndex        =   10
      Top             =   5400
      Width           =   740
   End
   Begin VB.CommandButton TXButton 
      Caption         =   "发送"
      Height          =   375
      Left            =   8520
      TabIndex        =   9
      Top             =   5400
      Width           =   740
   End
   Begin VB.TextBox TxData 
      Height          =   1800
      Left            =   3120
      MultiLine       =   -1  'True
      TabIndex        =   8
      Text            =   "Form1.frx":0000
      Top             =   5400
      Width           =   5300
   End
   Begin TabDlg.SSTab AnalysisData 
      Height          =   4695
      Left            =   3105
      TabIndex        =   7
      Top             =   300
      Width           =   7050
      _ExtentX        =   12435
      _ExtentY        =   8281
      _Version        =   393216
      Tab             =   1
      TabHeight       =   520
      TabCaption(0)   =   "解析数据"
      TabPicture(0)   =   "Form1.frx":0006
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "AnalysisiData"
      Tab(0).ControlCount=   1
      TabCaption(1)   =   "原始数据"
      TabPicture(1)   =   "Form1.frx":0022
      Tab(1).ControlEnabled=   -1  'True
      Tab(1).Control(0)=   "OriginalData"
      Tab(1).Control(0).Enabled=   0   'False
      Tab(1).Control(1)=   "StatusBar1"
      Tab(1).Control(1).Enabled=   0   'False
      Tab(1).Control(2)=   "ChkHexReceive"
      Tab(1).Control(2).Enabled=   0   'False
      Tab(1).ControlCount=   3
      TabCaption(2)   =   "监控显示"
      TabPicture(2)   =   "Form1.frx":003E
      Tab(2).ControlEnabled=   0   'False
      Tab(2).Control(0)=   "Supervisior"
      Tab(2).ControlCount=   1
      Begin VB.CheckBox ChkHexReceive 
         Caption         =   "16进制"
         Height          =   375
         Left            =   2760
         TabIndex        =   54
         Top             =   440
         Width           =   1575
      End
      Begin MSComctlLib.StatusBar StatusBar1 
         Height          =   375
         Left            =   2160
         TabIndex        =   51
         Top             =   7200
         Width           =   975
         _ExtentX        =   1720
         _ExtentY        =   661
         _Version        =   393216
         BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
            NumPanels       =   1
            BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            EndProperty
         EndProperty
      End
      Begin VB.TextBox Supervisior 
         Height          =   3495
         Left            =   -74700
         TabIndex        =   17
         Text            =   "Text5"
         Top             =   800
         Width           =   6500
      End
      Begin VB.TextBox OriginalData 
         Height          =   3495
         Left            =   300
         MultiLine       =   -1  'True
         TabIndex        =   16
         Top             =   800
         Width           =   6500
      End
      Begin VB.TextBox AnalysisiData 
         Height          =   3495
         Left            =   -74700
         TabIndex        =   15
         Text            =   "Text3"
         Top             =   800
         Width           =   6500
      End
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   4350
      Left            =   300
      TabIndex        =   6
      Top             =   3120
      Width           =   2415
      _ExtentX        =   4260
      _ExtentY        =   7673
      _Version        =   393216
      Style           =   1
      Tab             =   2
      TabHeight       =   520
      TabCaption(0)   =   "透传"
      TabPicture(0)   =   "Form1.frx":005A
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "Command4(2)"
      Tab(0).Control(1)=   "Command1(0)"
      Tab(0).Control(2)=   "Combo4"
      Tab(0).Control(3)=   "Combo3"
      Tab(0).Control(4)=   "Combo2"
      Tab(0).Control(5)=   "Combo1(0)"
      Tab(0).Control(6)=   "Label9(0)"
      Tab(0).Control(7)=   "Label8(0)"
      Tab(0).Control(8)=   "Label7(0)"
      Tab(0).Control(9)=   "Label6(0)"
      Tab(0).ControlCount=   10
      TabCaption(1)   =   "基站"
      TabPicture(1)   =   "Form1.frx":0076
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "Command4(1)"
      Tab(1).Control(1)=   "Command1(1)"
      Tab(1).Control(2)=   "Combo1(4)"
      Tab(1).Control(3)=   "Combo1(3)"
      Tab(1).Control(4)=   "Combo1(2)"
      Tab(1).Control(5)=   "Combo1(1)"
      Tab(1).Control(6)=   "Label9(1)"
      Tab(1).Control(7)=   "Label8(1)"
      Tab(1).Control(8)=   "Label7(1)"
      Tab(1).Control(9)=   "Label6(1)"
      Tab(1).ControlCount=   10
      TabCaption(2)   =   "地磁"
      TabPicture(2)   =   "Form1.frx":0092
      Tab(2).ControlEnabled=   -1  'True
      Tab(2).Control(0)=   "Label6(2)"
      Tab(2).Control(0).Enabled=   0   'False
      Tab(2).Control(1)=   "Label7(2)"
      Tab(2).Control(1).Enabled=   0   'False
      Tab(2).Control(2)=   "Label8(2)"
      Tab(2).Control(2).Enabled=   0   'False
      Tab(2).Control(3)=   "Label9(2)"
      Tab(2).Control(3).Enabled=   0   'False
      Tab(2).Control(4)=   "Label9(3)"
      Tab(2).Control(4).Enabled=   0   'False
      Tab(2).Control(5)=   "Combo1(5)"
      Tab(2).Control(5).Enabled=   0   'False
      Tab(2).Control(6)=   "Combo1(6)"
      Tab(2).Control(6).Enabled=   0   'False
      Tab(2).Control(7)=   "Combo1(7)"
      Tab(2).Control(7).Enabled=   0   'False
      Tab(2).Control(8)=   "Combo1(8)"
      Tab(2).Control(8).Enabled=   0   'False
      Tab(2).Control(9)=   "Combo1(9)"
      Tab(2).Control(9).Enabled=   0   'False
      Tab(2).Control(10)=   "Combo1(10)"
      Tab(2).Control(10).Enabled=   0   'False
      Tab(2).ControlCount=   11
      Begin VB.ComboBox Combo1 
         Height          =   300
         Index           =   10
         Left            =   1200
         TabIndex        =   45
         Text            =   "Combo1"
         Top             =   3200
         Width           =   1095
      End
      Begin VB.ComboBox Combo1 
         Height          =   300
         Index           =   9
         Left            =   1200
         TabIndex        =   44
         Text            =   "Combo1"
         Top             =   2320
         Width           =   1095
      End
      Begin VB.ComboBox Combo1 
         Height          =   300
         Index           =   8
         Left            =   1200
         TabIndex        =   43
         Text            =   "Combo1"
         Top             =   2760
         Width           =   1095
      End
      Begin VB.ComboBox Combo1 
         Height          =   300
         Index           =   7
         Left            =   1440
         TabIndex        =   42
         Text            =   "Combo1"
         Top             =   4440
         Width           =   1095
      End
      Begin VB.ComboBox Combo1 
         Height          =   300
         Index           =   6
         Left            =   1200
         TabIndex        =   41
         Text            =   "Combo1"
         Top             =   1440
         Width           =   1095
      End
      Begin VB.ComboBox Combo1 
         Height          =   300
         Index           =   5
         Left            =   1200
         TabIndex        =   40
         Text            =   "Combo1"
         Top             =   1880
         Width           =   1095
      End
      Begin VB.CommandButton Command4 
         Caption         =   "更改参数"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   9
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   500
         Index           =   2
         Left            =   -73750
         TabIndex        =   39
         Top             =   3360
         Width           =   1000
      End
      Begin VB.CommandButton Command4 
         Caption         =   "更改参数"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   9
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   500
         Index           =   1
         Left            =   -73750
         TabIndex        =   38
         Top             =   3360
         Width           =   1000
      End
      Begin VB.CommandButton Command1 
         Caption         =   "获取参数"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   9
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   500
         Index           =   1
         Left            =   -74870
         TabIndex        =   37
         Top             =   3360
         Width           =   1000
      End
      Begin VB.ComboBox Combo1 
         Height          =   300
         Index           =   4
         Left            =   -73800
         TabIndex        =   32
         Text            =   "Combo1"
         Top             =   2760
         Width           =   1095
      End
      Begin VB.ComboBox Combo1 
         Height          =   300
         Index           =   3
         Left            =   -73800
         TabIndex        =   31
         Text            =   "Combo1"
         Top             =   1440
         Width           =   1095
      End
      Begin VB.ComboBox Combo1 
         Height          =   300
         Index           =   2
         Left            =   -73800
         TabIndex        =   30
         Text            =   "Combo1"
         Top             =   1880
         Width           =   1095
      End
      Begin VB.ComboBox Combo1 
         Height          =   300
         Index           =   1
         Left            =   -73800
         TabIndex        =   29
         Text            =   "Combo1"
         Top             =   2320
         Width           =   1095
      End
      Begin VB.CommandButton Command4 
         Caption         =   "更改参数"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   9
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   500
         Index           =   0
         Left            =   -73750
         TabIndex        =   28
         Top             =   3360
         Width           =   1000
      End
      Begin VB.CommandButton Command1 
         Caption         =   "获取参数"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   9
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   500
         Index           =   0
         Left            =   -74870
         TabIndex        =   27
         Top             =   3360
         Width           =   1000
      End
      Begin VB.ComboBox Combo4 
         Height          =   300
         Left            =   -73800
         TabIndex        =   23
         Text            =   "Combo4"
         Top             =   2760
         Width           =   1095
      End
      Begin VB.ComboBox Combo3 
         Height          =   300
         Left            =   -73800
         TabIndex        =   22
         Text            =   "Combo3"
         Top             =   2320
         Width           =   1095
      End
      Begin VB.ComboBox Combo2 
         Height          =   300
         ItemData        =   "Form1.frx":00AE
         Left            =   -73800
         List            =   "Form1.frx":00B0
         TabIndex        =   21
         Text            =   "Combo2"
         Top             =   1880
         Width           =   1095
      End
      Begin VB.ComboBox Combo1 
         Height          =   300
         Index           =   0
         Left            =   -73800
         TabIndex        =   19
         Text            =   "Combo1"
         Top             =   1440
         Width           =   1095
      End
      Begin VB.Label Label9 
         Caption         =   "基站ID"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   10.5
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   240
         Index           =   3
         Left            =   255
         TabIndex        =   50
         Top             =   3200
         Width           =   1080
      End
      Begin VB.Label Label9 
         Caption         =   "唤醒时间"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   10.5
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   240
         Index           =   2
         Left            =   255
         TabIndex        =   49
         Top             =   2800
         Width           =   1080
      End
      Begin VB.Label Label8 
         Caption         =   "空中速度"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   10.5
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   240
         Index           =   2
         Left            =   255
         TabIndex        =   48
         Top             =   2350
         Width           =   1080
      End
      Begin VB.Label Label7 
         Caption         =   "波特率"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   10.5
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   240
         Index           =   2
         Left            =   255
         TabIndex        =   47
         Top             =   1920
         Width           =   855
      End
      Begin VB.Label Label6 
         Caption         =   "设备ID"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   10.5
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   240
         Index           =   2
         Left            =   255
         TabIndex        =   46
         Top             =   1450
         Width           =   855
      End
      Begin VB.Label Label9 
         Caption         =   "唤醒时间"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   10.5
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   240
         Index           =   1
         Left            =   -74745
         TabIndex        =   36
         Top             =   2350
         Width           =   1080
      End
      Begin VB.Label Label8 
         Caption         =   "空中速度"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   10.5
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   240
         Index           =   1
         Left            =   -74745
         TabIndex        =   35
         Top             =   2800
         Width           =   1080
      End
      Begin VB.Label Label7 
         Caption         =   "波特率"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   10.5
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   240
         Index           =   1
         Left            =   -74745
         TabIndex        =   34
         Top             =   1920
         Width           =   855
      End
      Begin VB.Label Label6 
         Caption         =   "设备ID"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   10.5
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   240
         Index           =   1
         Left            =   -74745
         TabIndex        =   33
         Top             =   1450
         Width           =   855
      End
      Begin VB.Label Label9 
         Caption         =   "唤醒时间"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   10.5
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   240
         Index           =   0
         Left            =   -74745
         TabIndex        =   26
         Top             =   2800
         Width           =   1080
      End
      Begin VB.Label Label8 
         Caption         =   "空中速度"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   10.5
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   240
         Index           =   0
         Left            =   -74745
         TabIndex        =   25
         Top             =   2350
         Width           =   1080
      End
      Begin VB.Label Label7 
         Caption         =   "波特率"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   10.5
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   240
         Index           =   0
         Left            =   -74745
         TabIndex        =   24
         Top             =   1920
         Width           =   855
      End
      Begin VB.Label Label6 
         Caption         =   "设备ID"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   10.5
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   240
         Index           =   0
         Left            =   -74745
         TabIndex        =   20
         Top             =   1450
         Width           =   855
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "端口设置"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   9
         Charset         =   134
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   2295
      Left            =   300
      TabIndex        =   0
      Top             =   300
      Width           =   2415
      Begin MSCommLib.MSComm MSComm1 
         Left            =   120
         Top             =   1200
         _ExtentX        =   1005
         _ExtentY        =   1005
         _Version        =   393216
         DTREnable       =   -1  'True
         OutBufferSize   =   4096
         RThreshold      =   1
         SThreshold      =   1
      End
      Begin VB.CommandButton OpenComButton 
         Caption         =   "打开串口"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   10.5
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   495
         Left            =   480
         TabIndex        =   5
         Top             =   1500
         Width           =   1455
      End
      Begin VB.ComboBox Boderate 
         Height          =   300
         ItemData        =   "Form1.frx":00B2
         Left            =   1080
         List            =   "Form1.frx":00D4
         Style           =   2  'Dropdown List
         TabIndex        =   4
         Top             =   800
         Width           =   1095
      End
      Begin VB.ComboBox ComNumber 
         Height          =   300
         Left            =   1100
         TabIndex        =   1
         Top             =   360
         Width           =   1095
      End
      Begin VB.Label Label2 
         Caption         =   "波特率"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   9
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   240
         Left            =   255
         TabIndex        =   3
         Top             =   820
         Width           =   855
      End
      Begin VB.Label Label1 
         Caption         =   "端口号"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   9
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   240
         Index           =   0
         Left            =   255
         TabIndex        =   2
         Top             =   390
         Width           =   855
      End
   End
   Begin VB.Label Label3 
      Caption         =   "毫秒"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   10.5
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   9720
      TabIndex        =   12
      Top             =   6050
      Width           =   495
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
Dim com_num_offset(29) As Byte
Dim com_open_flag  As Boolean
Dim ComSwitch As Boolean                   '串口打开标志位
Dim receiveData As String
Dim Counter As Long
Dim tg(14) As Byte
Dim OutputSignal As String                                                          ' 发送数据暂存
Dim InputSignal As String
Dim SendCount As Long                                                               ' 发送数据字节计数器
Dim ReceiveCount As Long                                                            ' 接收数据字节计数器

Private Sub Boderate_Click()
MSComm1.Settings = Boderate.Text & "n,8,1"
End Sub

Private Sub Command2_Click()

End Sub

Private Sub Check2_Click()

End Sub

Private Sub ComNumber_Click()
 Dim menu_init_flag As Boolean
  On Error GoTo BLK
MSComm1.Settings = Boderate.Text & "n,8,1"
  
scanPort:    If MSComm1.PortOpen = True Then
       MSComm1.PortOpen = False
       MSComm1.CommPort = com_num_offset(ComNumber.ListIndex)
       MSComm1.PortOpen = True
       com_open_flag = True
      '  ComSwitch = False
    Else
       MSComm1.CommPort = com_num_offset(ComNumber.ListIndex)
       MSComm1.PortOpen = True
       com_open_flag = True
       ' ComSwitch = True
    End If
    OpenComButton.Caption = "打开串口"
    
    If MSComm1.PortOpen = True Then
     ComSwitch = True
    Else
     ComSwitch = False
    End If
    Exit Sub
    
BLK:
       menu_init_flag = False
       com_open_flag = False
    MsgBox "串口不存在或者被占用!", vbOKOnly, "提示信息"

End Sub

Private Sub COM_dec()
 Dim A As Byte
 Dim i As Byte
'  Combo1.ListIndex = 0
  index_offset = 0
  For i = 0 To 29
   com_num_offset(i) = 0
  Next
  
   On Error GoTo uerror
    For A = 1 To 30
       MSComm1.PortOpen = False
        MSComm1.CommPort = A
        MSComm1.PortOpen = True
        
        If MSComm1.PortOpen = True Then
            If MSComm1.CommPort = A Then
              com_num_offset(index_offset) = A
              index_offset = index_offset + 1
            '  Call sorting_al
              ' MsgBox "可用Com号= ", a
              ComNumber.AddItem "COM" & A
              MSComm1.PortOpen = False
            End If
            

        End If
    Next
    Exit Sub
uerror:
   ' MsgBox (Error(Err.Number))
    Resume Next
End Sub
Private Sub ComNumber_DropDown()
  ComNumber.Clear
  Erase Array(com_num_offset)
 Call COM_dec
End Sub

Private Sub Form_Load()
Boderate.ListIndex = 3
MSComm1.Settings = Boderate & "n,8,1"
End Sub

'====================================================================================
'                 十六进制发送

'====================================================================================

Private Sub hexSend()
    
    On Error Resume Next
    Dim outputLen As Integer                                                    ' 发送数据长度
    Dim outData As String                                                       ' 发送数据暂存
    Dim SendArr() As Byte                                                       ' 发送数组
    Dim TemporarySave As String                                                 ' 数据暂存
    Dim dataCount As Integer                                                    ' 数据个数计数
    Dim i As Integer                                                            ' 局部变量
    
    outData = UCase(Replace(TxData.Text, Space(1), Space(0)))                  ' 先去掉空格，再转换为大写字母
    outData = UCase(outData)                                                    ' 转换成大写
    outputLen = Len(outData)                                                    ' 数据长度
    ' MsgBox outData, vbOKOnly, "提示信息"
    For i = 0 To outputLen
        TemporarySave = Mid(outData, i + 1, 1)                                  ' 取一位数据
        If (Asc(TemporarySave) >= 48 And Asc(TemporarySave) <= 57) Or (Asc(TemporarySave) >= 65 And Asc(TemporarySave) <= 70) Then
            dataCount = dataCount + 1
        Else
            Exit For
            Exit Sub
        End If
    Next
    
    If dataCount Mod 2 <> 0 Then                                                ' 判断十六进制数据是否为双数
        dataCount = dataCount - 1                                               ' 不是双数，则减1
    End If
    
    outData = Left(outData, dataCount)                                          ' 取出有效的十六进制数据
    
    ReDim SendArr(dataCount / 2 - 1)                                            ' 重新定义数组长度
    For i = 0 To dataCount / 2 - 1
        SendArr(i) = Val("&H" + Mid(outData, i * 2 + 1, 2))                     ' 取出数据转换成十六进制并放入数组中
    Next
    
    SendCount = SendCount + (dataCount / 2)                                     ' 计算总发送数
     TXCount.Text = "TX:" & SendCount
    ' MsgBox SendArr, vbOKOnly, "提示信息"
    MSComm1.Output = SendArr                                                     ' 发送数据
    
End Sub
'====================================================================================
'                 文本发送

'====================================================================================

Private Sub textSend()
    
    On Error GoTo Err
  '  If ModeSend = True Then
   '     OutputSignal = FileData                                                 ' 发送文件
   ' Else
        OutputSignal = TxData.Text                                             ' 发送文本
   ' End If
    
    SendCount = SendCount + LenB(StrConv(OutputSignal, vbFromUnicode))          ' 计算总发送数
    TXCount.Text = "TX:" & SendCount                                         ' 发送字节数显示
  
Exit Sub
Err:
    
End Sub
'====================================================================================
'                 文本接收

'====================================================================================
Private Sub textReceive()

     ' Dim ds As Byte
    Dim tmpp1 As String
    Dim tmpp2 As String
    On Error GoTo Err
        
   ' MsgBox MSComm1.Input
    InputSignal = MSComm1.Input
    ReceiveCount = ReceiveCount + LenB(StrConv(InputSignal, vbFromUnicode))     ' 计算总接收数据
    If DisplaySwitch = False Then                                               ' 显示接收文本
        If mod_set_flag = False Then
            OriginalData.Text = OriginalData.Text & InputSignal                         ' 单片机内存的值用TextReceive显示出
            OriginalData.SelStart = Len(OriginalData.Text)                              ' 显示光标位置
        End If
    End If
    If mod_set_flag = False Then
     RXCount.Text = "RX:" & ReceiveCount                                      ' 接收字节数显示
    End If
Exit Sub
Err:

    MsgBox (Error(Err.Number))
End Sub
'====================================================================================
'                 十六进制数据接受

'====================================================================================

Private Sub hexReceive()
    
    Dim tg2() As Byte
    Dim ReceiveArr() As Byte                                                    ' 接收数据数组
    Dim i As Integer
                                                           ' 循环变
    On Error GoTo Err
        If MSComm1.InBufferCount > 0 Then
            ' = Counter + 1                                          ' 读取接收数据个数
            Counter = MSComm1.InBufferCount
            receiveData = ""                                                        ' 清缓冲
            ReceiveArr = MSComm1.Input                                               ' 数据放入数组
            For i = 0 To (Counter - 1) Step 1
                tttt = tttt + 1
                If (ReceiveArr(i) < 16) Then
                    receiveData = receiveData & "0" + Hex(ReceiveArr(i)) & Space(1) ' 小于16，前面加0
                    str1 = str1 & receiveData
                Else
                    receiveData = receiveData & Hex(ReceiveArr(i)) & Space(1)       ' 加空格显示
                End If
            tg(ik) = ReceiveArr(i)
            ik = ik + 1
            str1 = receiveData
            If tttt Mod 15 = 0 And tttt > 0 Then
                tttt = 0
                ik = 0
                sh = Mid(str1, 1, 21) '返回“:”  (从第5位开始截取一个字符)
                MyComp = StrComp(mystr1, sh, 0)
                If MyComp = False Then
                    pa_tmp = tg(7) * 256 + tg(8)
                    uburate = pa_tmp
                    burate.Text = uburate
                    
                    pa_tmp = tg(9) * 256 + tg(10)
                    uburate_air = pa_tmp
                    air_rate.Text = uburate_air
                    
                    pa_tmp = tg(11) * 256 + tg(12)
                    uid = pa_tmp
                    rf_id_num.Text = uid
                    
                    pa_tmp = tg(13)
                    upower = pa_tmp
                    power.Text = upower
                    
                    pa_tmp = tg(14)
                    uch = pa_tmp
                    channel.Text = uch
                    ChkHexReceive.Value = 0
                    air_rate.Enabled = True
                    power.Enabled = True
                    rf_id_num.Enabled = True
                    channel.Enabled = True
                    burate.Enabled = True
                End If
                str1 = ""
            End If
            Next i


            If mod_set_flag = False Then
                OriginalData.Text = OriginalData.Text + receiveData                         ' 显示接收的十六进制数据
                OriginalData.SelStart = Len(OriginalData.Text)                              ' 显示光标位置
    
            End If
          '  MsgBox MSComm1.InBufferCount
       End If
        If mod_set_flag = False Then

            ReceiveCount = ReceiveCount + Counter                                       ' 接收计数
            RXCount.Text = "RX:" & ReceiveCount                                      ' 接收字节数显示
        End If
        ' Exit Sub
Err:
 ' MsgBox (Error(Err.Number))
End Sub

Private Sub MSComm1_OnComm()
On Error GoTo Err
    Select Case MSComm1.CommEvent                                                    ' 每接收1个数就触发一次
        Case comEvReceive
            If ChkHexReceive.Value = 1 Then
                Sleep 30
                Call hexReceive                                                     ' 十六进制接收
            Else
                Call textReceive                                                    ' 文本接收
        End If
      '  If MSComm1.InBufferCount <> 0 Then
       '  GoTo xv
      ' End If
        Case comEvSend                                                              ' 每发送1个数就触发一次
            If ChkHexSend.Value = 1 Then
            Call hexSend
           Else
                If mod_set_flag = False Then
                    Call textSend                                                       ' 文本发送
                End If

            End If
        Case Else
    End Select
Err:     ' MsgBox Str(Len((Error(Err.Number))))
    
     'MsgBox (Err.Number)  '8002
    
    ' MsgBox (Error(Err.Number))
End Sub

Private Sub OpenComButton_Click()
  On Error GoTo Err
    If ComSwitch = True Then
       
    '  If com_open_flag = True Then
       ' ComSwitch = False
        MSComm1.PortOpen = True
        OpenComButton.Caption = "关闭串口"
      
      'End If
      
      ComSwitch = False
    Else

        
        If ComSwitch = False Then
            If MSComm1.PortOpen = True Then
                OpenComButton.Caption = "打开串口"
                 MSComm1.PortOpen = False
                 
           Else
               MSComm1.PortOpen = True
           ' MsgBox "串口不存在或者被占用!", vbOKOnly, "提示信息"
            End If
        Else
           ComSwitch = True
           'MSComm1.PortOpen = False
           OpenComButton.Caption = "关闭串口"
            

          ' Call menu_init
           
           

        End If
    End If
  Exit Sub
Err:
    MsgBox "串口不存在或者被占用!", vbOKOnly, "提示信息"
      
    ComSwitch = False


     
End Sub


Private Sub TXButton_Click()
    On Error GoTo Err
   
    If MSComm1.PortOpen = True Then                                              ' 如果串口打开了，则可以发送数据
    
        If TxData.Text = "" Then                                               ' 判断发送数据是否为空
            MsgBox "发送数据不能为空", 16, "畅连助手"                       ' 发送数据为空则提示
        Else
            If ChkHexSend.Value = 1 Then                                        ' 发送方式判断
                MSComm1.InputMode = comInputModeBinary                           ' 二进制发送
                Call hexSend                                                    ' 发送十六进制数据
            Else                                                                ' 按十六进制接收文本方式发送的数据时，文本也要按二进制发送发送
                'If ChkHexReceive.Value = 1 Then
                  '  MSComm1.InputMode = comInputModeBinary                       ' 二进制发送
               ' Else
                    MSComm1.InputMode = comInputModeText                         ' 文本发送
               ' End If

    '    OutputSignal = TxtSend.Text                                             ' 发送文本
    
  '  SendCount = SendCount + LenB(StrConv(OutputSignal, vbFromUnicode))          ' 计算总发送数
  '  TxtTXCount.Text = "TX:" & SendCount                                         ' 发送字节数显示

                MSComm1.Output = Trim(TxData.Text)                              ' 发送数据
              '  ModeSend = False                                                ' 设置文本发送方式
            End If
        End If
    Else
        MsgBox "串口没有打开，请打开串口", 48, "畅连助手"                   ' 如果串口没有被打开，提示打开串口
    End If
    Exit Sub
Err:
    MsgBox "串口没有打开，请打开串口", 48, "畅连助手"
End Sub


VERSION 5.00
Object = "{BDC217C8-ED16-11CD-956C-0000C04E4C0A}#1.1#0"; "TABCTL32.OCX"
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
   Begin VB.CheckBox Check3 
      Caption         =   "保存到文件"
      Height          =   375
      Left            =   8520
      TabIndex        =   20
      Top             =   7200
      Width           =   1575
   End
   Begin VB.CheckBox Check2 
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
   Begin VB.CommandButton Command2 
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
      TabIndex        =   8
      Text            =   "Text1"
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
      Tab             =   2
      TabHeight       =   520
      TabCaption(0)   =   "解析数据"
      TabPicture(0)   =   "Form1.frx":0000
      Tab(0).ControlEnabled=   0   'False
      Tab(0).Control(0)=   "AnalysisiData"
      Tab(0).ControlCount=   1
      TabCaption(1)   =   "原始数据"
      TabPicture(1)   =   "Form1.frx":001C
      Tab(1).ControlEnabled=   0   'False
      Tab(1).Control(0)=   "OriginalData"
      Tab(1).ControlCount=   1
      TabCaption(2)   =   "监控显示"
      TabPicture(2)   =   "Form1.frx":0038
      Tab(2).ControlEnabled=   -1  'True
      Tab(2).Control(0)=   "Supervisior"
      Tab(2).Control(0).Enabled=   0   'False
      Tab(2).ControlCount=   1
      Begin VB.TextBox Supervisior 
         Height          =   3495
         Left            =   300
         TabIndex        =   19
         Text            =   "Text5"
         Top             =   800
         Width           =   6500
      End
      Begin VB.TextBox OriginalData 
         Height          =   3495
         Left            =   -74700
         TabIndex        =   18
         Text            =   "Text4"
         Top             =   800
         Width           =   6500
      End
      Begin VB.TextBox AnalysisiData 
         Height          =   3495
         Left            =   -74700
         TabIndex        =   17
         Text            =   "Text3"
         Top             =   800
         Width           =   6500
      End
   End
   Begin TabDlg.SSTab SSTab1 
      Height          =   4350
      Left            =   300
      TabIndex        =   6
      Top             =   2805
      Width           =   2415
      _ExtentX        =   4260
      _ExtentY        =   7673
      _Version        =   393216
      Style           =   1
      TabHeight       =   520
      TabCaption(0)   =   "透传"
      TabPicture(0)   =   "Form1.frx":0054
      Tab(0).ControlEnabled=   -1  'True
      Tab(0).Control(0)=   "Label6(0)"
      Tab(0).Control(0).Enabled=   0   'False
      Tab(0).Control(1)=   "Label7(0)"
      Tab(0).Control(1).Enabled=   0   'False
      Tab(0).Control(2)=   "Label8(0)"
      Tab(0).Control(2).Enabled=   0   'False
      Tab(0).Control(3)=   "Label9(0)"
      Tab(0).Control(3).Enabled=   0   'False
      Tab(0).Control(4)=   "Combo1(0)"
      Tab(0).Control(4).Enabled=   0   'False
      Tab(0).Control(5)=   "Combo2"
      Tab(0).Control(5).Enabled=   0   'False
      Tab(0).Control(6)=   "Combo3"
      Tab(0).Control(6).Enabled=   0   'False
      Tab(0).Control(7)=   "Combo4"
      Tab(0).Control(7).Enabled=   0   'False
      Tab(0).Control(8)=   "Command1(0)"
      Tab(0).Control(8).Enabled=   0   'False
      Tab(0).Control(9)=   "Command4(2)"
      Tab(0).Control(9).Enabled=   0   'False
      Tab(0).ControlCount=   10
      TabCaption(1)   =   "基站"
      TabPicture(1)   =   "Form1.frx":0070
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
      TabPicture(2)   =   "Form1.frx":008C
      Tab(2).ControlEnabled=   0   'False
      Tab(2).ControlCount=   0
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
         Left            =   1250
         TabIndex        =   41
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
         TabIndex        =   40
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
         TabIndex        =   39
         Top             =   3360
         Width           =   1000
      End
      Begin VB.ComboBox Combo1 
         Height          =   300
         Index           =   4
         Left            =   -73800
         TabIndex        =   34
         Text            =   "Combo1"
         Top             =   2760
         Width           =   1095
      End
      Begin VB.ComboBox Combo1 
         Height          =   300
         Index           =   3
         Left            =   -73800
         TabIndex        =   33
         Text            =   "Combo1"
         Top             =   1440
         Width           =   1095
      End
      Begin VB.ComboBox Combo1 
         Height          =   300
         Index           =   2
         Left            =   -73800
         TabIndex        =   32
         Text            =   "Combo1"
         Top             =   1880
         Width           =   1095
      End
      Begin VB.ComboBox Combo1 
         Height          =   300
         Index           =   1
         Left            =   -73800
         TabIndex        =   31
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
         TabIndex        =   30
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
         Left            =   130
         TabIndex        =   29
         Top             =   3360
         Width           =   1000
      End
      Begin VB.ComboBox Combo4 
         Height          =   300
         Left            =   1200
         TabIndex        =   25
         Text            =   "Combo4"
         Top             =   2760
         Width           =   1095
      End
      Begin VB.ComboBox Combo3 
         Height          =   300
         Left            =   1200
         TabIndex        =   24
         Text            =   "Combo3"
         Top             =   2320
         Width           =   1095
      End
      Begin VB.ComboBox Combo2 
         Height          =   300
         ItemData        =   "Form1.frx":00A8
         Left            =   1200
         List            =   "Form1.frx":00AA
         TabIndex        =   23
         Text            =   "Combo2"
         Top             =   1880
         Width           =   1095
      End
      Begin VB.ComboBox Combo1 
         Height          =   300
         Index           =   0
         Left            =   1200
         TabIndex        =   21
         Text            =   "Combo1"
         Top             =   1440
         Width           =   1095
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
         TabIndex        =   38
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
         TabIndex        =   37
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
         TabIndex        =   36
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
         TabIndex        =   35
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
         Left            =   255
         TabIndex        =   28
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
         Left            =   255
         TabIndex        =   27
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
         Left            =   255
         TabIndex        =   26
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
         Left            =   255
         TabIndex        =   22
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
         Left            =   1080
         TabIndex        =   4
         Text            =   "Combo2"
         Top             =   800
         Width           =   1095
      End
      Begin VB.ComboBox ComNumber 
         Height          =   300
         Left            =   1100
         TabIndex        =   1
         Text            =   "Combo1"
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
   Begin VB.Label Label5 
      Caption         =   "接收:"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   10.5
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   6600
      TabIndex        =   16
      Top             =   7560
      Width           =   1455
   End
   Begin VB.Label Label4 
      Caption         =   "发送:"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   10.5
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   3720
      TabIndex        =   15
      Top             =   7600
      Width           =   1455
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

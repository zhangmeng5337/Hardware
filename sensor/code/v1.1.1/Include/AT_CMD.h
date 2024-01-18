#ifndef AT_CMD_H_
#define AT_CMD_H_

//const unsigned char Test2[]='\0';
const unsigned char Test[]="AT\r";
const unsigned char Respond_OK[]="OK\r\n";
const unsigned char Respond_Start[]="PB DONE\r\n";
const unsigned char Respond_ERROR[]="ERROR\r\n";
const unsigned char Respond_IP[]="+IPADDR: ";
const unsigned char Respond_Connection_ERROR[]="CLOSED";
const unsigned char Respond_Connection_OK[]="CONNECT 115200\r";
const unsigned char Respond_CPIN[]="+CPIN: READY\r\n";
const unsigned char Respond_Network_Intensity[]="+CSQ: ";//��������ź�ǿ��
const unsigned char Respond_Network_Intensity2[]=",99\r";//��������ź�ǿ��
const unsigned char Respond_Network_Intensity_eff[]="+CSQ: ";//��������ź�ǿ��
const unsigned char Respond_Network_Normal_ModeT[]="+CFUN: 1";//��������ź�ǿ��
const unsigned char platform_received[]="receive";//��������ź�ǿ��
const unsigned char SMS_Ready[]="SMS Ready";//��������ź�ǿ��
const unsigned char Respond_Attached_Ok[]="+CGATT: 1";//

const unsigned char Respond_Attached_Fail[]="+CGATT: 0";//
const unsigned char Respond_Network_Open[]="Network is already opened\r";//
const unsigned char Respond_TCP_Connect[]="CONNECT\r";//
const unsigned char Respond_Network_Open2[]="+NETOPEN: 0";//
const unsigned char Respond_No_Carrier[]="+NO CARRIER";//
const unsigned char Respond_IPR[]="+IPR: 9600";//



const unsigned char TIMEOUT[]="AT+CIPTIMEOUT?\r\n";
const unsigned char TO_DATA_MODE[]="ATO\r";//enter data transparent
const unsigned char Quit_transparent[]="+++";//quit from data transparent
const unsigned char TCP_ERROR[]="+CIPOPEN: 0,4\r\n";//
const unsigned char TCP_Closed[]="CLOSED\r\n";//
const unsigned char TCP_Connect[]="CONNECT\r\n";//
const unsigned char CPU_ID[]="Get CPU ID\r\n";//
const unsigned char Echo_Dis[]="ATE0\r";//���Թر����������
const unsigned char Read_SIM_Num[]="AT+CICCID\r";
const unsigned char Check_SIM[]="AT+CPIN?\r";//�������ƶ��豸(ME)������
const unsigned char Normal_Mode[]="AT+CFUN=1\r";
const unsigned char Normal_ModeT[]="AT+CFUN?\r";
const unsigned char ATE_W[]="ATE0&W\r";

const unsigned char Mnimum_Mode[]="AT+CFUN=0\r";
const unsigned char Network_Intensity[]="AT+CSQ\r";//��������ź�ǿ��
const unsigned char GPRS_Register[]="AT+CGREG?\r";//��� GPRS ע��״̬
const unsigned char Network_Register[]="AT+CREG?\r";//�������ע��״̬
const unsigned char GPRS_Attached_State[]="AT+CGATT?\r";//��� GPRS ����״̬
const unsigned char Sing_Link[]="AT+CIPMUX=0\r";//������ģʽ
const unsigned char Pass_Through[]="AT+CIPMODE=1\r";//����Ϊ͸��ģʽ
const unsigned char Net_Open[]="AT+NETOPEN\r";//����Ϊ͸��ģʽ
const unsigned char Net_Close[]="AT+NETCLOSE\r";//����Ϊ͸��ģʽ

const unsigned char CIP_Close[]="AT+CIPCLOSE=0\r";//����Ϊ͸��ģʽ
const unsigned char LAST_CMD[]="A/\r";
const unsigned char APN[]="AT+CGDCONT=1,\"IP\",\"jsczaiot02.clfu.njm2mapn\"\r";//��ѯ��Ӫ��ѡ��״̬
//const unsigned char Establish_TCP_Connection[]="AT+CIPSTART=\"TCP\",\"15k801n729.iok.la\",25718\r";
 
//��������\"TCP\",\"15k801n729.iok.la\",25718\r";
//\"TCP\",\"139.199.223.136\",12345\r";
const unsigned char Airplane_Mode[]="AT+CFUN=0\r";//����ģʽ���ر�GPRS
const unsigned char China_Mobile[]="AT+COPS?\r";//��ѯ��Ӫ��ѡ��״̬
const unsigned char AT_SHUNT[]="AT+CIPSHUT\r";//��ѯ��Ӫ��ѡ��״̬
const unsigned char FLY_Mode[]="AT+CFUN=0\r";
const unsigned char M_RESET[]="AT+CRESET\r";
const unsigned char IPR_SET[]="AT+IPR=9600\r";
const unsigned char IPR_GET[]="AT+IPR?\r";
//**********************************GPS Infor********************************************

const unsigned char Start_GPS[]="AT+CGPSAUTO=1\r";//start gps
const unsigned char Stop_GPS[]="AT+CGPS=0\r";
const unsigned char AT_GPS_info[]="AT+CGPSINFO\r";

#endif

#include "cJSON.h"
#include "main.h"
#include "json_para.h"
#include "mqtt_analy.h"

//https://blog.csdn.net/weixin_45030703/article/details/106208593
//https://blog.csdn.net/sinat_41785418/article/details/104635446
 /*		cJSON *root,*fmt;
		 root=cJSON_CreateObject();
		 cJSON_AddStringToObject(root,"Address",sta.Address);
		 cJSON_AddStringToObject(root,"EndpointId",sta.Endpointid);
		 cJSON_AddItemToObject(root, "State", fmt=cJSON_CreateObject());
		 cJSON_AddStringToObject(fmt,"State", sta.State);
		 cJSON_AddStringToObject(root,"StateType",sta.StateType);
		 cJSON_AddNumberToObject(root,"DeviceType",1101);
		 
		 char *out=cJSON_PrintUnformatted(root);
		 if(out)
		 {
			strncpy((char*)sta_buff,out,strlen(out));
			sta_flag=1;
			free(out);
		 }
		 
{
��Address��: ��1234567890ABCDEF��,
��EndpointId��: ��1��,
��State��: {
��State��: ��1��
},
��StateType��: ��020D����
��DeviceType��: 1101
}*/

/*dev---->ser
{
    "Dev ID": "866289037465624",
    "Status Data": {
        "Out Tem": "45",
        "In Tem": "26",
        "Front Pressure": "0.5",
        "After Pressure": "0.5",
        "Status": "4"
    },
    "Dev Params": {
        "Version": "V3.0.1",
        "Set Out Tem": "45",
        "Set In Tem": "23",
        "Upload Period(second)": "10"
    }
}

/*服务�?-->设备（周期下发）
{
   "Dev ID":  "866289037465624",
   "Room Temp": {
               "Room 1 Temp": "15",
               "Room 2 Temp": "18",
               "Room 3 Temp": "16",
               "Room 4 Temp": "17",
               "Room 5 Temp": "15",
               "Room 6 Temp": "13",
               "Room 7 Temp": "12",
               "Room 8 Temp": "21",
               "Room 9 Temp": "20",
               "Room 10 Temp": "15"
     }
}*

/*服务�?-->设备
{
   "Dev ID":  "866289037465624",
   "Dev Ctrl": {
               "Updat Frimware": "1",
               "Reboot Dev": "1",
               "Power ctrl": "1",
               "Set Out Temp": "45",
               "Set Room Temp": "23",
               "Set Upload Period(second)": "13"
     }
}*/

        //sprintf(mqtt_send_buf,"{\\0D\\0A\\
  \\22Dev ID\\22: %s,\\0D\\0A\\
  \\22Status Data\\22: {\\0D\\0A\\
    \\22Out Tem\\22: \\22%f\\22,\\0D\\0A\\
    \\22In Tem\\22: \\22%f\\22,\\0D\\0A\\
    \\22Front Pressure\\22: \\22%f\\22,\\0D\\0A\\
    \\22After Pressure\\22: \\22%f\\22,\\0D\\0A\\
    \\22Status\\22: \\22%d\\22\\0D\\0A\\
    },\\0D\\0A\\
  \\22Dev Params\\22: {\\0D\\0A\\
    \\22Version\\22: \\22%s\\22,\\0D\\0A\\
    \\22Set Out Tem\\22: \\22%f\\22,\\0D\\0A\\
    \\22Set In Tem\\22: \\22%f\\22,\\0D\\0A\\
    \\22Upload Period(second)\\22: \\222\\22\\0D\\0A\\
    }\\0D\\0A\\
}",mqtt_payload_u.devid,
//mqtt_payload_u.data[TOUT_INDEX],
//mqtt_payload_u.data[TIN_INDEX],
//mqtt_payload_u.data[PUMP_F_INDEX],
//mqtt_payload_u.data[PUMP_E_INDEX],
//mqtt_payload_u.status[DEV_STATUS_INDEX],
//mqtt_payload_u.version,
//mqtt_payload_u.data[WATER_O_INDEX],
//mqtt_payload_u.data[WATER_IN_INDEX],
//mqtt_payload_u.data[UP_PERIOD_INDEX]);


char *json_pack(mqtt_payload_stru *pb)
{
	static uint8_t number_data;
    cJSON *dev_pub,*pub_status,*pub_params;
	char *data;


	/*{
    "Dev ID": "866289037465624",
    "Status Data": {
        "Out Tem": "45",
        "In Tem": "26",
        "Front Pressure": "0.5",
        "After Pressure": "0.5",
        "Status": "4"
    },
    "Dev Params": {
        "Version": "V3.0.1",
        "Set Out Tem": "45",
        "Set In Tem": "23",
        "Upload Period(second)": "10"
    }
    }*/
	  number_data=number_data+1;	//ģ������
	  if(number_data>100)
	  {
	   	number_data=0;
	  }
        dev_pub=cJSON_CreateObject();   //���������ݶ���
	    cJSON_AddItemToObject(dev_pub, "Dev ID", cJSON_CreateString(pb->devid));  //���ڵ����������

		cJSON_AddItemToObject(dev_pub, "Status Data", pub_status=cJSON_CreateObject());			 //���ڵ�������ַ�
        cJSON_AddItemToObject(pub_status, "Out Tem", cJSON_CreateString("h"));			 		//���ڵ�����Ӻ���
        cJSON_AddItemToObject(pub_status, "In Tem", cJSON_CreateString("˧"));	
        cJSON_AddItemToObject(pub_status, "Front Pressure", cJSON_CreateString("˧"));	
        cJSON_AddItemToObject(pub_status, "After Pressure", cJSON_CreateString("˧"));	
        cJSON_AddItemToObject(pub_status, "Status", cJSON_CreateString("˧"));	
		
        cJSON_AddItemToObject(dev_pub, "Dev Params", pub_params=cJSON_CreateObject());	
        cJSON_AddItemToObject(pub_params, "Version", cJSON_CreateString("˧"));	
        cJSON_AddItemToObject(pub_params, "Set Out Tem", cJSON_CreateString("˧"));	
        cJSON_AddItemToObject(pub_params, "Set In Tem", cJSON_CreateString("˧"));	
        cJSON_AddItemToObject(pub_params, "Upload Period(second)", cJSON_CreateString("˧"));	


		


  return cJSON_Print(dev_pub);
	/*	data = cJSON_Print(dev_pub);   //��json��ʽ��ӡ�������ַ�����ʽ(����\r\n)
//	    data = cJSON_PrintUnformatted(usr);   //��json��ʽ��ӡ�������ַ�����ʽ(û��\r\n)
		 printf("%s",data);			//ͨ�����ڴ�ӡ����

		char *out=cJSON_PrintUnformatted(dev_pub);
		if(out)
		{
		   strncpy((char*)sta_buff,out,strlen(out));
		   sta_flag=1;
		   cJSON_Delete(dev_pub);
		   free(out);
		}*/


}

void json_analysis()
{

    cJSON *json,*json_one,*json_two,*json_three;
    char* out="{\"one\":\"long\",\"two\":\"2\",\"three\":3}";
 
    json = cJSON_Parse(out); //���õ����ַ���������json��ʽ
    /****************************/
    /*	  ���Խ�JSON��ӡ����	*/
    /***************************/
   //char *out_data = cJSON_Print(json);   //��json��ʽ��ӡ�������ַ�����ʽ
   //printf("%s",out_data);

    json_one = cJSON_GetObjectItem( json , "one" );  //��json��ȡ��ֵ����
    json_two = cJSON_GetObjectItem( json , "two" );//��json��ȡ��ֵ����
    json_three = cJSON_GetObjectItem( json , "three" );//��json��ȡ��ֵ����
 
    printf("\r\none:%s   two:%d   three:%d",json_one->valuestring,json_two->valueint,json_three->valueint);
 
    cJSON_Delete(json);  //�ͷ��ڴ� 
}


#include "cJSON.h"
#include "main.h"
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
“Address”: “1234567890ABCDEF”,
“EndpointId”: “1”,
“State”: {
“State”: “1”
},
“StateType”: “020D”，
“DeviceType”: 1101
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

/*鏈嶅姟绔?-->璁惧锛堝懆鏈熶笅鍙戯級
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

/*鏈嶅姟绔?-->璁惧
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



void json_pack(void)
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
	  number_data=number_data+1;	//模拟数字
	  if(number_data>100)
	  {
	   	number_data=0;
	  }
        dev_pub=cJSON_CreateObject();   //创建根数据对象
	      cJSON_AddItemToObject(dev_pub, "Dev ID", cJSON_CreateNumber(number_data));  //根节点下添加数字

		    cJSON_AddItemToObject(dev_pub, "Status Data", pub_status=cJSON_CreateObject());			 //根节点下添加字符
        cJSON_AddItemToObject(pub_status, "Out Tem", cJSON_CreateString("h"));			 		//根节点下添加汉字
        cJSON_AddItemToObject(pub_status, "In Tem", cJSON_CreateString("帅"));	
        cJSON_AddItemToObject(pub_status, "Front Pressure", cJSON_CreateString("帅"));	
        cJSON_AddItemToObject(pub_status, "After Pressure", cJSON_CreateString("帅"));	
        cJSON_AddItemToObject(pub_status, "Status", cJSON_CreateString("帅"));	
		
        cJSON_AddItemToObject(dev_pub, "Dev Params", pub_params=cJSON_CreateObject());	
        cJSON_AddItemToObject(pub_params, "Version", cJSON_CreateString("帅"));	
        cJSON_AddItemToObject(pub_params, "Set Out Tem", cJSON_CreateString("帅"));	
        cJSON_AddItemToObject(pub_params, "Set In Tem", cJSON_CreateString("帅"));	
        cJSON_AddItemToObject(pub_params, "Upload Period(second)", cJSON_CreateString("帅"));	


		



	/*	data = cJSON_Print(dev_pub);   //将json形式打印成正常字符串形式(带有\r\n)
//	    data = cJSON_PrintUnformatted(usr);   //将json形式打印成正常字符串形式(没有\r\n)
		 printf("%s",data);			//通过串口打印出来

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
 
    json = cJSON_Parse(out); //将得到的字符串解析成json形式
    /****************************/
    /*	  测试将JSON打印出来	*/
    /***************************/
   //char *out_data = cJSON_Print(json);   //将json形式打印成正常字符串形式
   //printf("%s",out_data);

    json_one = cJSON_GetObjectItem( json , "one" );  //从json获取键值内容
    json_two = cJSON_GetObjectItem( json , "two" );//从json获取键值内容
    json_three = cJSON_GetObjectItem( json , "three" );//从json获取键值内容
 
    printf("\r\none:%s   two:%d   three:%d",json_one->valuestring,json_two->valueint,json_three->valueint);
 
    cJSON_Delete(json);  //释放内存 
}


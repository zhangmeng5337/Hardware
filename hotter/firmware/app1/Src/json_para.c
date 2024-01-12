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



void json_pack(void)
{
	static uint8_t number_data;
    cJSON * usr;
	char *data;
	  number_data=number_data+1;	//模拟数字
	  if(number_data>100)
	  {
	   	number_data=0;
	  }
        usr=cJSON_CreateObject();   //创建根数据对象
	    cJSON_AddItemToObject(usr, "A", cJSON_CreateNumber(number_data));  //根节点下添加数字
        cJSON_AddItemToObject(usr, "B", cJSON_CreateString("hello"));			 //根节点下添加字符
        cJSON_AddItemToObject(usr, "C", cJSON_CreateString("帅"));			 		//根节点下添加汉字
	    data = cJSON_Print(usr);   //将json形式打印成正常字符串形式(带有\r\n)
//	    data = cJSON_PrintUnformatted(usr);   //将json形式打印成正常字符串形式(没有\r\n)
		 printf("%s",data);			//通过串口打印出来
		/********************/
		/*		释放内存				*/
		/*********************/
		cJSON_Delete(usr);
		free(data);
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


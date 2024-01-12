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
��Address��: ��1234567890ABCDEF��,
��EndpointId��: ��1��,
��State��: {
��State��: ��1��
},
��StateType��: ��020D����
��DeviceType��: 1101
}*/



void json_pack(void)
{
	static uint8_t number_data;
    cJSON * usr;
	char *data;
	  number_data=number_data+1;	//ģ������
	  if(number_data>100)
	  {
	   	number_data=0;
	  }
        usr=cJSON_CreateObject();   //���������ݶ���
	    cJSON_AddItemToObject(usr, "A", cJSON_CreateNumber(number_data));  //���ڵ����������
        cJSON_AddItemToObject(usr, "B", cJSON_CreateString("hello"));			 //���ڵ�������ַ�
        cJSON_AddItemToObject(usr, "C", cJSON_CreateString("˧"));			 		//���ڵ�����Ӻ���
	    data = cJSON_Print(usr);   //��json��ʽ��ӡ�������ַ�����ʽ(����\r\n)
//	    data = cJSON_PrintUnformatted(usr);   //��json��ʽ��ӡ�������ַ�����ʽ(û��\r\n)
		 printf("%s",data);			//ͨ�����ڴ�ӡ����
		/********************/
		/*		�ͷ��ڴ�				*/
		/*********************/
		cJSON_Delete(usr);
		free(data);
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


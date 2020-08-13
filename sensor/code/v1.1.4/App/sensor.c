#include "sensor.h"
#include "cJSON.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
static cJSON *struct_to_json(void* struct_obj) {

  cJSON *root;
  cJSON *Array;
  cJSON *objId;
  
  Sensor_Stru *sensor = (Sensor_Stru *)struct_obj;
  root = cJSON_CreateObject();
  Array = cJSON_CreateArray();
  objId = cJSON_CreateObject();
  
  cJSON_AddNumberToObject(objId, "kqwd", sensor->air_temper);
  cJSON_AddNumberToObject(objId, "kqsd", sensor->air_humid);       
  cJSON_AddNumberToObject(objId, "trwd", sensor->soil_tempre);        
  cJSON_AddNumberToObject(objId, "trsd", sensor->soil_humid);        
  cJSON_AddNumberToObject(objId, "trph", sensor->soil_ph);
  cJSON_AddNumberToObject(objId, "gzd", sensor->sunshine);
  unsigned char i;
  for (i=0;i<COLLECT_DAT_SIZE;i++) 
    cJSON_AddNumberToObject(objId, "collect_time", sensor->Collect_Date[i]);        
  cJSON_AddNumberToObject(objId, "error", sensor->error_staus);    
  cJSON_AddItemToArray(Array, objId);
  cJSON_AddItemToObject(root, "records", Array); 
  cJSON_AddNumberToObject(root, "total", sensor->amount);
  for (i=0;i<HEAD_SIZE;i++) 
    cJSON_AddNumberToObject(root, "header", sensor->header[i]);
  cJSON_AddNumberToObject(root, "version", sensor->version);
  for (i=0;i<ID_SIZE;i++) 
    cJSON_AddNumberToObject(root, "id", sensor->id[i]);    
  for (i=0;i<VER_DEV_SIZE;i++) 
    cJSON_AddNumberToObject(root, "number", sensor->ver_dev[i]);
  for (i=0;i<SESSION_SIZE;i++) 
    cJSON_AddNumberToObject(root, "session", sensor->session[i]);
  for (i=0;i<SUB_DAT_SIZE;i++) 
    cJSON_AddNumberToObject(root, "submit_time", sensor->Sub_Date[i]);
  for (i=0;i<IMEI_SIZE;i++) 
    cJSON_AddNumberToObject(root, "imei", sensor->imei[i]);     
  for (i=0;i<ICCID_SIZE;i++) 
    cJSON_AddNumberToObject(root, "iccid", sensor->iccid[i]);
  for (i=0;i<CSQ_SIZE;i++) 
    cJSON_AddNumberToObject(root, "csq", sensor->csq[i]);       
  for (i=0;i<LONG_SIZE;i++) 
    cJSON_AddNumberToObject(root, "longitude", sensor->longitude[i]);
  for (i=0;i<LAT_SIZE;i++) 
    cJSON_AddNumberToObject(root, "latitude", sensor->latitude[i]);
  for (i=0;i<VBAT_SIZE;i++) 
    cJSON_AddNumberToObject(root, "battery", sensor->battery[i]);
  for (i=0;i<RSI_SIZE;i++) 
    cJSON_AddNumberToObject(root, "xhqd", sensor->rssi[i]);                                                     
  
  char *out = cJSON_Print(root);   //将json形式打印成正常字符串形式
  printf("%s\n",out);
  
  // 释放内存  
  cJSON_Delete(root);  
  free(out); 
  return root;
}

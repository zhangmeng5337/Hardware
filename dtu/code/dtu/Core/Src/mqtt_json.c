#include <jansson.h>
json_t *jsonboot = json_object();//创建根对象	 
json_t *jsonparams = json_object();//创建params对象

char* json_pack(unsigned char pack_num)
{
	char* json_str;

    switch(pack_num)
    {
		case 1:
			json_object_set_new(jsonboot, "id", json_string("a123"));
			json_object_set_new(jsonboot, "id", json_string("a123"));
			
			json_object_set_new(jsonparams,"model",json_string("deepseek"));
			json_object_set_new(jsonparams,"age",json_integer(10));
			json_object_set_new(jsonboot, "params", jsonparams);  //将对象添加到其中
			json_str = json_dumps(jsonboot, JSON_INDENT(0));	//printf无法直接打印json数据
			//printf("%s\r\n", json_str);
			
			break;
	}
	return json_str;
	
}
json_analy()
{
	    const char *json_str = "{\"id\":\"a123\",\"value\":23}";
		char id[10];
		char value[10];
		json_t *jsonbox; 
		json_error_t error;
		jsonbox = json_loads(json_str, 0, &error);	
		json_t *id_obj = json_object_get(jsonbox, "id");
		const char *id_val = json_string_value(id_obj);
		snprintf(id, sizeof(id), "%s", id_val);
		
		json_t *value_obj = json_object_get(jsonbox, "value");
		int value_1 = json_integer_value(value_obj);
		snprintf(value, sizeof(value), "%d", value_1);
		printf("id: %s\r\n", id);
		printf("value: %s\r\n", value);

}

void json_delete()
{
	json_decref(jsonboot);

}




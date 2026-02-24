#ifndef Protocol_C_H_                                    
#define Protocol_C_H_                                    
                                                         
#include "main.h"                                        
#include "loraHW.h"                                      
#define EQUIP_PRO_PERIOD			15000

#define PAYLOAD_MAX_LEN		         0x28                  
#define PAYLOAD_MIN_LEN		         0x01                  
#define AUTO_REPORT_SIZE			40                                                         
                                                         
#define ADDR_SIZE			             0x08            
#define START_ADDR_SIZE		             0x02            
#define PAYLOAD_SIZE		             280             
#define PAYLOAD_LEN_INDEX                0x14            
#define UNSIGNED                         0x00            
#define SIGN_DOT1                        0x81            
#define UNSIGN_DOT1                      0x01            
#define UNSIGN_DOT2                      0x02            
#define UNSIGN_DOT3                      0x03            
                                                         
                                                         
                                                         
                                                         
#define  gateway_Rcmd	 			  0x17    
#define  pc_cmd 					  0x18    
#define  PC_readFrame				  0x30    
#define  auto_report_cmd 			  0x51    
                                                         
                                                         
                                                         
                                                         
#define  soilRH_DEV                      0x01   
#define  soilCON_DEV                     0x02	 
#define  co2_DEV	 						           0x03   
#define  con_DEV	 						           0x04   
#define  sunShine_DEV					    	     0x05   
#define  pressure_DEV					    	     0x08   
#define  flow_DEV						      	     0x09   
#define  gateway_DEV 				      	     0xfd   
                                                         
                                                         
                                                         
#define 	reg_period_set 	 								0xf001 
#define 	reg_period_Hmin	 								0xf002 
#define 	reg_period_Lmin	 								0xf003 
#define 	reg_period_sec									0xf004 
#define 	reg_power												0xf005 
#define 	reg_quantity      							0xf006 
#define 	reg_node_addr_H									0xf007 
#define 	reg_node_addr_M									0xf008 
#define 	reg_node_addr_LM 								0xf009 
#define 	reg_node_addr_L									0xf00a 
#define 	reg_lora_native_addr			      0xf00b 
#define 	reg_modbus_sensor_rate		      0xf00c 
#define 	reg_modbus_pc_rate				      0xf00d 
#define 	reg_longitude_H 					       	      0xe000 
#define 	reg_longitude_M 					       	      0xe001 
#define 	reg_longitude_LM 					       	      0xe002 
#define 	reg_longitude_L 					       	      0xe003 
#define 	reg_latitude_H 						       	      0xe008 
#define 	reg_latitude_M 						       	      0xe009 
#define 	reg_latitude_LM 					       	      0xe00a 
#define 	reg_latitude_L 						       	      0xe00b 
#define 	reg_soil_temp_5 					       	      0x1001 
#define 	reg_soil_rh_5 						       	      0x1002 
#define 	reg_soil_temp_4 					       	      0x1003 
#define 	reg_soil_rh_4  						       	      0x1004 
#define 	reg_soil_temp_3 					       	      0x1005 
#define 	reg_soil_rh_3 						       	      0x1006 
#define 	reg_soil_temp_2 					       	      0x1007 
#define 	reg_soil_rh_2 						       	      0x1008 
#define 	reg_soil_temp_1 					       	      0x1009 
#define 	reg_soil_rh_1 						       	      0x100a 
#define   reg_actutor1 						         	      0x1010 
#define 	reg_actutor2 							       	      0x1011 
#define 	reg_actutor3 							       	      0x1012 
#define 	reg_clock_ym 							       	      0x1100 
#define 	reg_clock_dh							       	      0x1101 
#define 	reg_clock_ms							       	      0x1102 
#define 	reg_dest_gateway_addrH 		       	      0x1200 
#define 	reg_dest_gateway_addrM		       	      0x1201 
#define 	reg_dest_gateway_addrLM		       	      0x1202 
#define 	reg_dest_gateway_addrL		       	      0x1203 
//#define 	reg_dest_lora_addrH			       	      0x1204 
//#define 	reg_dest_lora_center_addr		   	      0x1205 
#define 	reg_lora_air_rate						  0x1204 
#define 	reg_lora_ch										   	  0x1205 
#define 	reg_server_ipH							     	      0x1206 
#define 	reg_server_ipM							     	      0x1207 
#define 	reg_server_ipLM							     	      0x1208 
#define 	reg_server_ipL							     	      0x1209 
#define 	reg_server_port							     	      0x120a 
                                                         
                                                         
                                                         
                                                         
#define		temperature													0x01  
#define		rh 															0x02  
#define		light														0x03  
#define		soilT														0x04  
#define		soilW														0x05  
#define		pressure_air 												0x06  
#define		pressure 													0x07  
#define		co2															0x08  
#define		co1 														0x09  
#define		so2															0x0a  
#define		ph 															0x0b  
#define		ec 															0x0c  
#define		o2 															0x0d  
#define		rain 											    		0x0e  
#define		wind 											    		0x0f  
#define		direction 								    				0x10  
#define		p_pa 											    		0x11  
#define		P_kpa 										    			0x12  
#define		P_Mpa 										    			0x13  
#define		P_bar 										    			0x14  
#define		P_Mbar 										    			0x15  
#define		quantity 									    			0xf2  
#define		year_month								    				0xf3  
#define		day_hour									    			0xf4  
#define		min_sec										    			0xf5  
#define		auto_report_time					    					0xf6  


typedef struct
{
  unsigned char DesAddr[ADDR_SIZE];
  unsigned char SrcAddr[ADDR_SIZE];
  unsigned char devCode;
  unsigned char funCode;
  unsigned char startAddr[START_ADDR_SIZE];
  unsigned char payloadLenH;
  unsigned char payloadLenL; 
  unsigned char payload[PAYLOAD_SIZE];
}Proctocol_C_stru;
typedef struct
{
  unsigned char timoutFLag;
  unsigned char tickFlag;
  uint32_t tickCount;
}loraModuleTimeout_stru;
void equipmentProcess(unsigned char p);
void get_equipmentData(void);
unsigned char protocolCAnaly(unsigned char  *p);
void WrRead_equipment(LORAHW_stru *loraNo);
void wirelessTimoutStart(unsigned char p);
Proctocol_C_stru *protocolCstru(void);
unsigned char *ackmodeStatus(void);
void SendLoraData(LORAHW_stru *loraNo,unsigned char *p );
void SendPayloadPack(LORAHW_stru *loraNo,unsigned char ackMode);
void SnedToNbiot(unsigned char *p,unsigned int len);

void PCDevice_Modbus_Process(void);
void ProctocolInit(void);
void setDataSrc(unsigned char p);
unsigned char *getDataSrc(void);
void dev_params_pack(unsigned int start_addr,unsigned char sensors_prams,unsigned char len,unsigned char dataSrc);
void ProctocolInit(void);

#endif


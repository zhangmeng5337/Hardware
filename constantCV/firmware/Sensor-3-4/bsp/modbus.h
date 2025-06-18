/*******************************************************************************
*
* 浠ｇ爜璁稿彲鍜屽厤璐ｄ俊�?* 姝︽眽鑺簮鍗婂浣撴湁闄愬叕鍙告巿浜堟偍浣跨敤鎵€鏈夌紪绋嬩唬鐮佺ず渚嬬殑闈炰笓灞炵殑鐗堟潈璁稿彲锛屾偍鍙互鐢辨�?* 鐢熸垚鏍规嵁鎮ㄧ殑鐗瑰畾闇€瑕佽€屽畾鍒剁殑鐩镐技鍔熻兘銆傛牴鎹笉鑳借鎺掗櫎鐨勪换浣曟硶瀹氫繚璇侊紝姝︽眽鑺簮�?* 瀵间綋鏈夐檺鍏徃鍙婂叾绋嬪簭寮€鍙戝晢鍜屼緵搴斿晢瀵圭▼搴忔垨鎶€鏈敮鎸侊紙濡傛灉鏈夛級涓嶆彁渚涗换浣曟槑绀烘垨�?* 鍚殑淇濊瘉鎴栨潯浠讹紝鍖呮嫭浣嗕笉闄愪簬鏆楀惈鐨勬湁鍏抽€傞攢鎬с€侀€傜敤浜庢煇绉嶇壒瀹氱敤閫斿拰闈炰镜鏉冪殑淇濊�?* 鎴栨潯浠躲€?* 鏃犺浣曠鎯呭舰锛屾姹夎姱婧愬崐瀵间綋鏈夐檺鍏徃鍙婂叾绋嬪簭寮€鍙戝晢鎴栦緵搴斿晢鍧囦笉瀵逛笅鍒楀悇椤硅礋璐ｏ�?* 鍗充娇琚憡鐭ュ叾鍙戠敓鐨勫彲鑳芥€ф椂锛屼篃鏄姝わ細鏁版嵁鐨勪涪澶辨垨鎹熷潖锛涚洿鎺ョ殑銆佺壒鍒殑銆侀檮甯︾殑
* 鎴栭棿鎺ョ殑鎹熷锛屾垨浠讳綍鍚庢灉鎬х粡娴庢崯瀹筹紱鎴栧埄娑︺€佷笟鍔°€佹敹鍏ャ€佸晢瑾夋垨棰勬湡鍙妭鐪侀噾棰濈殑
* 鎹熷け銆?* 鏌愪簺鍙告硶杈栧尯涓嶅厑璁稿鐩存帴鐨勩€侀檮甯︾殑鎴栧悗鏋滄€х殑鎹熷鏈変换浣曠殑鎺掗櫎鎴栭檺鍒讹紝鍥犳鏌愪簺鎴?* 鍏ㄩ儴涓婅堪鎺掗櫎鎴栭檺鍒跺彲鑳藉苟涓嶉€傜敤浜庢偍�?*
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SENSOR_H
#define __SENSOR_H
#include "main.h"
#include "reg.h"

#define MODBUS_RECV_SIZE		128
#define REG_COUNT 32
#define SUPPORT_CMD_SIZE 4

/*modbus**************************************************/
#define MODBUS_FUNC_READ  				0x03
#define MODBUS_FUNC_ONLYREAD  			0x04
#define MODBUS_FUNC_ONE_WRITE	  		0x06
#define MODBUS_FUNC_MUL_WRITE	  		0x10

#define PC_FUNC_REG_READ	  				0x33 // pc read 
#define PC_FUNC_REG_WRITE	  		        0x56 //pc write
#define PC_FUNC_SET_REGS_WRITE	  		    0x46 //pc set reg all
#define PC_FUNC_READ_REGS	  				0x53 //pc read rea all


#define INS_DEV_REG			0x0000
#define INS_ADC_REG1H    	 	0x0001
#define INS_ADC_REG1L    	 	0x0002
#define INS_ADC_REG2H    	 	0x0003
#define INS_ADC_REG2L    	 	0x0004
#define INS_ADC_REG3H    	 	0x0005
#define INS_ADC_REG3L    	 	0x0006
#define INS_ADC_REG4H    	 	0x0007
#define INS_ADC_REG4L    	 	0x0008
#define INS_ADC_REG5H    	 	0x0009
#define INS_ADC_REG5L    	 	0x000a

#define INS_TYPE_REG        0x000B
#define PGA_REG  			0x000C
#define CUR_CONS_REGH        0x000D
#define CUR_CONS_REGL        0x000D

#define FAC_UNIT_REG        0x000E
#define FAC_COE_H     		0x000F
#define FAC_COE_L     		0x0010
#define FAC_OFFSET_HREG     0x0011
#define FAC_OFFSET_LREG     0x0012
                            
#define COE1_REG			0x0013
#define COE2_REG			0x0015
#define COE3_REG			0x0017
#define COE4_REG			0x0019
#define SAVE_REG			0x001b
#define ZERO_REG			0x001c
#define FACTOR_MODE_REG		0x001d
#define CAL1_REG		   0x001e
#define CAL2_REG		   0x0020
#define CAL3_REG		0x0022
#define CAL4_REG		0x0024
#define CAL5_REG		0x0026

#define CAL1_VAL		0x0028
#define CAL2_VAL		0x002a
#define CAL3_VAL		0x002c
#define CAL4_VAL		0x002e
#define CAL5_VAL		0x0030
#define MASK_ZERO		0x0032
#define COE5_REG		0x0034


typedef struct
{
unsigned int Reg;
unsigned char SupportCmd[SUPPORT_CMD_SIZE];//analy request
unsigned char CmdSize;

}modbus_reg_cmdCheck_stru;
//typedef struct
//{
//modbus_reg_cmd_stru *RegCmd;
//
////unsigned int val_uint; 
//reg_dat_pack *RegDat;
//
//}modbus_reg_stru;

typedef struct
{
modbus_reg_cmdCheck_stru *RegCmdDat;
reg_dat_pack *RegNum;
unsigned int cmd_sup;
}modbus_reg_cmd_stru;

typedef struct
{
unsigned char DevAddr;
unsigned char Addr;
unsigned char Func;
unsigned int RegStart;
unsigned int RegEnd;
unsigned int RegCount;
unsigned int len;
unsigned int crc;
unsigned char payload[MODBUS_RECV_SIZE];//analy request
unsigned char tx_buf[MODBUS_RECV_SIZE];//decto mcu
uint32_t tick;
}modbus_stru;

typedef struct
{
unsigned int reg;
unsigned int val_uint; 
int val_int;
float f;
unsigned char data_type;//0:unint;1:int  2:float
}reg_dat_type_stru;



void modbus_recv_proc(void);
void modbus_init(void);

#endif /*__CW32L010_SPI_H */


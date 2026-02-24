/*******************************************************************************
*
* 浠ｇ爜璁稿彲鍜屽厤璐ｄ俊鎭?* 姝︽眽鑺簮鍗婂浣撴湁闄愬叕鍙告巿浜堟偍浣跨敤鎵€鏈夌紪绋嬩唬鐮佺ず渚嬬殑闈炰笓灞炵殑鐗堟潈璁稿彲锛屾偍鍙互鐢辨
* 鐢熸垚鏍规嵁鎮ㄧ殑鐗瑰畾闇€瑕佽€屽畾鍒剁殑鐩镐技鍔熻兘銆傛牴鎹笉鑳借鎺掗櫎鐨勪换浣曟硶瀹氫繚璇侊紝姝︽眽鑺簮鍗?* 瀵间綋鏈夐檺鍏徃鍙婂叾绋嬪簭寮€鍙戝晢鍜屼緵搴斿晢瀵圭▼搴忔垨鎶€鏈敮鎸侊紙濡傛灉鏈夛級涓嶆彁渚涗换浣曟槑绀烘垨鏆?* 鍚殑淇濊瘉鎴栨潯浠讹紝鍖呮嫭浣嗕笉闄愪簬鏆楀惈鐨勬湁鍏抽€傞攢鎬с€侀€傜敤浜庢煇绉嶇壒瀹氱敤閫斿拰闈炰镜鏉冪殑淇濊瘉
* 鎴栨潯浠躲€?* 鏃犺浣曠鎯呭舰锛屾姹夎姱婧愬崐瀵间綋鏈夐檺鍏徃鍙婂叾绋嬪簭寮€鍙戝晢鎴栦緵搴斿晢鍧囦笉瀵逛笅鍒楀悇椤硅礋璐ｏ紝
* 鍗充娇琚憡鐭ュ叾鍙戠敓鐨勫彲鑳芥€ф椂锛屼篃鏄姝わ細鏁版嵁鐨勪涪澶辨垨鎹熷潖锛涚洿鎺ョ殑銆佺壒鍒殑銆侀檮甯︾殑
* 鎴栭棿鎺ョ殑鎹熷锛屾垨浠讳綍鍚庢灉鎬х粡娴庢崯瀹筹紱鎴栧埄娑︺€佷笟鍔°€佹敹鍏ャ€佸晢瑾夋垨棰勬湡鍙妭鐪侀噾棰濈殑
* 鎹熷け銆?* 鏌愪簺鍙告硶杈栧尯涓嶅厑璁稿鐩存帴鐨勩€侀檮甯︾殑鎴栧悗鏋滄€х殑鎹熷鏈変换浣曠殑鎺掗櫎鎴栭檺鍒讹紝鍥犳鏌愪簺鎴?* 鍏ㄩ儴涓婅堪鎺掗櫎鎴栭檺鍒跺彲鑳藉苟涓嶉€傜敤浜庢偍銆?*
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __74HC595_H
#define __74HC595_H
#include "main.h"
#define HC595_SHCP_Low()   	   GPIO_WritePin(IN_SCK_GPIO_PORT,IN_SCK_GPIO_PIN,GPIO_Pin_RESET)
#define HC595_SHCP_High()      GPIO_WritePin(IN_SCK_GPIO_PORT,IN_SCK_GPIO_PIN,GPIO_Pin_SET)
#define HC595_STCP_Low() 			 GPIO_WritePin(IN_RCKGPIO_PORT,IN_RCK_GPIO_PIN,GPIO_Pin_RESET)
#define HC595_STCP_High() 		 GPIO_WritePin(IN_SCK_GPIO_PORT,IN_RCK_GPIO_PIN,GPIO_Pin_SET)
#define HC595_Data_Low()   	   GPIO_WritePin(IN_SI_GPIO_PORT,IN_SI_GPIO_PIN,GPIO_Pin_RESET)
#define HC595_Data_High()      GPIO_WritePin(IN_SI_GPIO_PORT,IN_SI_GPIO_PIN,GPIO_Pin_SET)

void XL74HC595_MultiWrite(uint8_t *data, uint8_t Length);//混合写数据

#endif /*__CW32L010_SPI_H */


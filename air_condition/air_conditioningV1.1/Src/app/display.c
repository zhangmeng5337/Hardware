#include "74hc595.h"
#include "display.h"
#include "settings.h"
#include "lcd.h"
#include "lcd_init.h"



unsigned char table[]= {0x03,0x9f,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09,0x11,0xc1,0x63,0x85,0x61,0x71};
//unsigned char table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
const unsigned char seg_table[16]= {0};
//static unsigned char buf[4]= {MINUS,MINUS,MINUS,MINUS};
//void display_dat_deal(float dat,unsigned char *header_code,unsigned char dattypes)
//{
//    uint32_t tmp;
//        if(dattypes == 0)
//        {
//            if(dat<10)
//            {
//                tmp = dat*1000;
//                buf[0] = table[tmp/1000]-DOT;
//                buf[1] = table[tmp%1000/100];
//                buf[2] = table[tmp%100/10];
//                buf[3] = table[tmp%10];
//            }
//            else if(dat>=10&&dat<100)
//            {
//                tmp = dat*100;
//                buf[0] =  table[tmp/1000];
//                buf[1] =  table[tmp%1000/100]-DOT;
//                buf[2] =  table[tmp%100/10];
//                buf[3] =  table[tmp%10];
//            }
//            else if(dat>=100&&dat<1000)
//            {
//                tmp = dat*10;
//                buf[0] =  table[tmp/1000];
//                buf[1] =  table[tmp%1000/100];
//                buf[2] =  table[tmp%100/10]-DOT;
//                buf[3] =  table[tmp%10];
//            }
//            else
//            {
//                tmp = dat;
//                buf[0] =  table[tmp/1000];
//                buf[1] =  table[tmp%1000/100];
//                buf[2] =  table[tmp%100/10];
//                buf[3] =  table[tmp%10]-DOT;
//            }

//        }
//        else if(dattypes == 1)
//        {
//            if(dat<10)
//            {
//                tmp = dat*1000;
//                buf[0] = table[tmp/1000];
//                buf[1] = table[tmp%1000/100];
//                buf[2] = table[tmp%100/10];
//                buf[3] = table[tmp%10];
//            }
//            else if(dat>=10&&dat<100)
//            {
//                tmp = dat*100;
//                buf[0] =  table[tmp/1000];
//                buf[1] =  table[tmp%1000/100];
//                buf[2] =  table[tmp%100/10];
//                buf[3] =  table[tmp%10];
//            }
//            else if(dat>=100&&dat<1000)
//            {
//                tmp = dat*10;
//                buf[0] =  table[tmp/1000];
//                buf[1] =  table[tmp%1000/100];
//                buf[2] =  table[tmp%100/10];
//                buf[3] =  table[tmp%10];
//            }
//            else
//            {
//                // tmp = dat*10;
//                buf[0] =  table[tmp/1000];
//                buf[1] =  table[tmp%1000/100];
//                buf[2] =  table[tmp%100/10];
//                buf[3] =  table[tmp%10];
//            }
//        }
//        else if(dattypes == 2)//16进制显示
//        {
//            {
//                tmp = dat;
//                buf[0] = table[tmp>>16];
//                buf[1] = table[(tmp>>8)&0x00f];
//                buf[2] = table[(tmp>>4)&0x00f];
//                buf[3] = table[tmp&0x0f];
//            }

//        }
//        else if(dattypes == 3)//16进制显示
//        {
//            tmp = dat;
//			buf[0] = header_code[0];
//            if(dat<10)
//            {
//                tmp = dat*1000;
//                buf[1] = table[tmp/1000]-DOT;
//                buf[2] = table[tmp%1000/100];
//                buf[3] = table[tmp%100/10];
//               // buf[3] = table[tmp%10];
//            }
//            else if(dat>=10&&dat<100)
//            {
//                tmp = dat*100;
//                buf[1] =  table[tmp/1000];
//                buf[2] =  table[tmp%1000/100]-DOT;
//                buf[3] =  table[tmp%100/10];
//                //buf[3] =  table[tmp%10];
//            }
//            else if(dat>=100&&dat<1000)
//            {
//                tmp = dat*10;
//                buf[1] =  table[tmp/1000];
//                buf[2] =  table[tmp%1000/100];
//                buf[3] =  table[tmp%100/10]-DOT;
//                //buf[3] =  table[tmp%10];
//            }
//            else
//            {
//                tmp = dat;
//                buf[1] =  table[tmp/1000];
//                buf[2] =  table[tmp%1000/100];
//                buf[3] =  table[tmp%100/10];
//                //buf[3] =  table[tmp%10]-DOT;
//            }

//			if(dat<0)
//			{
//				// tmp = dat*10;
//				buf[0] =  MINUS;
//				buf[1] = buf[0];
//				buf[2] =  buf[1];
//				buf[3] =  buf[2];
//			}

//            
//            //buf[1] = table[tmp/100];
//            //buf[2] = table[tmp%100/10];
//            //buf[3] = table[tmp%10];

//        }
//        else if(dattypes == 4)//16进制显示
//        {
//            {
//                tmp = dat;
//                buf[0] = header_code[0];;
//                buf[1] = header_code[1];;
//                buf[2] = header_code[2];;
//                buf[3] = header_code[3];;
//            }

//        }

//        if(dat<0)
//        {
//            // tmp = dat*10;
//            buf[0] =  MINUS;
//            buf[1] = buf[0];
//            buf[2] =  buf[1];
//            buf[3] =  buf[2];
//        }
//#if DEBUG_USER
//        printf("display is:   %d\n",dat);
//#endif

//}

//static unsigned char bitselect=0;

//void display_off(void)
//{
//    //seg_select(5);
//}
//void init_seg()
//{
//    bitselect = 0;

//}
//void diplay_blink_ctrl(unsigned char mode)
//{
//    static uint32_t gettime,gettime2;

//    if(mode == 1)
//    {
//        if((HAL_GetTick()-gettime2)<DISPLAY_BLINK_ON)
//        {
//            if((HAL_GetTick()-gettime)>=DISPLAY_PERIOD)
//            {
//                gettime = HAL_GetTick();
//                //M74HC595_WriteData(buf[bitselect-1]);
//             //   seg_select(bitselect);
//                bitselect = bitselect +1;
//                if(bitselect > 4)
//                    bitselect = 1;
//            }

//        }
//        else if((HAL_GetTick()-gettime2)>=DISPLAY_BLINK_ON&&(HAL_GetTick()-gettime2)<DISPLAY_BLINK_PERIOD)
//        {
//            display_off();
//        }
//        else if((HAL_GetTick()-gettime2)>=DISPLAY_BLINK_PERIOD)
//            gettime2 = HAL_GetTick();

//    }
//    else
//    {
//        if((HAL_GetTick()-gettime)>=DISPLAY_PERIOD)
//        {
//            gettime = HAL_GetTick();
//            //M74HC595_WriteData(buf[bitselect-1]);
//           // seg_select(bitselect);
//            bitselect = bitselect +1;
//            if(bitselect > 4)
//                bitselect = 1;
//        }

//    }


//}
//void display_proc(unsigned char flag)
//{

//    //if((get_params_mode()->status)>=WORK_OFF)
//    {
//        if(flag == 0)
//        {
//            if( get_params_mode()->mode ==NORMAL)
//            {
//                if(get_params_mode()->status !=WORK_ON&&get_params_mode()->modeNo >STAND_MODE)
//                {

//                    display_menu(1);//diplay_blink_ctrl(1);
//                }
//                else
//                {
//                   display_menu(1);;


//                }



//            }
//            else if( get_params_mode()->mode == SETTING_MODE)
//            {
//                 display_menu(2);;
//            }
//        }
//        else  //test mode
//        {
//            if( get_params_mode()->mode==NORMAL)
//            {
//               ;// diplay_blink_ctrl(1);


//            }
//        }

//    }



//}
extern params_stru settings_params;
extern mode_stru mode;
void display_menu(unsigned char number)
{
		unsigned int index;
//	 unsigned char mode_no;
	index = 5;
	uint32_t gettime;
	
	

	
	 if((HAL_GetTick()-gettime>=100))
	 {
			gettime = HAL_GetTick();
   if(number)
	 {
	    switch(mode.modeNo)
    {
			case 0:
				//LCD_ShowChinese(index+10+24*3,183+10,"制冷",LIGHTBLUE,BLACK,24,0);
				LCD_ShowIntNum(index+24*3,62+42,(unsigned int)(settings_params.tar_set_temper[0]),2,YELLOW,BLACK,32);
			break;		
			case COLD:
				//LCD_ShowString(index+10+24*2+10,183+10,"  ",BLACK,BLACK,24,0);
			 //  LCD_Fill(index+10+24*2+10,183+10,index+10+24*6+10,183+10+24,BLACK);
				LCD_ShowChinese(index+10+24*2+10,183+10,"制冷",LIGHTBLUE,BLACK,24,0);
			  LCD_ShowString(index+10+24*4+10,183+10,"    ",BLACK,BLACK,24,0);
				LCD_ShowIntNum(index+24*3,62+42,(unsigned int)(settings_params.tar_set_temper[0]),2,YELLOW,BLACK,32);
			break;
			case HOT:
				LCD_ShowChinese(index+10+24*2+10,183+10,"制热",LIGHTBLUE,BLACK,24,0);
			 LCD_ShowString(index+10+24*4+10,183+10,"    ",BLACK,BLACK,24,0);
			LCD_ShowIntNum(index+24*3,62+42,(unsigned int)(settings_params.tar_set_temper[1]),2,YELLOW,BLACK,32);
			break;
			case COLD_HUM_COLD:
				LCD_ShowChinese(index+10+24*2+10,183+10,"加湿制冷",LIGHTBLUE,BLACK,24,0);
				LCD_ShowIntNum(index+24*3,62+42,(unsigned int)(settings_params.tar_set_temper[2]),2,YELLOW,BLACK,32);
			break;
//			case COLD_HUM_HUM:
//				LCD_ShowChinese(index+10+24*2+10,183+10,"加湿制冷",LIGHTBLUE,BLACK,24,0);
//				LCD_ShowIntNum(index+24*3,62+42,(unsigned int)(settings_params.tar_set_temper[2]),2,YELLOW,BLACK,32);
//			break;
			case FAST_COLD:
				LCD_ShowChinese(index+10+24*2+10,183+10,"快速制冷",LIGHTBLUE,BLACK,24,0);
				LCD_ShowIntNum(index+24*3,62+42,(unsigned int)(settings_params.tar_set_temper[3]),2,YELLOW,BLACK,32);
			break;
//			case FAST_COLD_HUM:
//				LCD_ShowChinese(index+10+24*2+10,183+10,"快速制冷",LIGHTBLUE,BLACK,24,0);
//				LCD_ShowIntNum(index+24*3,62+42,(unsigned int)(settings_params.tar_set_temper[3]),2,YELLOW,BLACK,32);break;
			case STAND_MODE:
				if(mode.last_mode_no)
					mode.modeNo = mode.last_mode_no;
				else
				{
					mode.modeNo = COLD;
				LCD_ShowChinese(index+10+24*2+10,183+10,"制冷",LIGHTBLUE,BLACK,24,0);
				LCD_ShowString(index+10+24*4+10,183+10,"    ",BLACK,BLACK,24,0);}
				LCD_ShowChinese(index+24*3,18,"待机",LIGHTBLUE,BLACK,24,0);
				 
			LCD_ShowIntNum(index+24*3,62+42,(unsigned int)(settings_params.tar_set_temper[0]),2,YELLOW,BLACK,32);

			break;

     //   LCD_ShowChinese(index+10+24*3,183+10,"制冷",LIGHTBLUE,BLACK,24,0);
    }	 
	 }

		 
 if(number == 0)
	 {
	 LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
		// LCD_BLK_Clr();
	 }
	 else if(number == 1)
	 {
			LCD_ShowChinese(index,18,"状态",LIGHTBLUE,BLACK,24,0);
		 if(mode.status == WORK_ON)
			LCD_ShowChinese(index+24*3,18,"运行",LIGHTBLUE,BLACK,24,0);
			LCD_ShowChinese(index+24*7,18,"水温",LIGHTBLUE,BLACK,24,0);
			LCD_ShowIntNum(index+24*10,14,(unsigned int)(settings_params.water_temper),2,LIGHTBLUE,BLACK,32);
			LCD_ShowChinese(index+24*11+10,14,"℃",LIGHTBLUE,BLACK,32,0);

			LCD_ShowChinese(index,62+23,"设定",LIGHTBLUE,BLACK,32,0);
			LCD_ShowChinese(index,62+23+32*1,"温度",LIGHTBLUE,BLACK,32,0);
		//	LCD_ShowIntNum(index+24*3,62+42,(unsigned int)(settings_params.tar_set_temper),2,YELLOW,BLACK,32);
			LCD_ShowChinese(index+24*4+12,62+42,"℃",YELLOW,BLACK,32,0);

			LCD_ShowChinese(index+32*5,62+23,"当前",LIGHTBLUE,BLACK,32,0);
			LCD_ShowChinese(index+32*5,62+23+32*1,"温度",LIGHTBLUE,BLACK,32,0);
			LCD_ShowIntNum(index+24*10,62+42,(unsigned int)(settings_params.tar_env_temper),2,YELLOW,BLACK,32);
			LCD_ShowChinese(index+24*11+10,62+42,"℃",YELLOW,BLACK,32,0);

			LCD_ShowChinese(index+10,183+10,"模式",LIGHTBLUE,BLACK,24,0);
		//	LCD_ShowChinese(index+10+24*3,183+10,"制冷",LIGHTBLUE,BLACK,24,0);
			LCD_ShowChinese(index+24*7,183+10,"湿度",LIGHTBLUE,BLACK,24,0);
			LCD_ShowIntNum(index+24*10,183+10,(unsigned int)(settings_params.humid[0]),2,LIGHTBLUE,BLACK,32);
			LCD_ShowString(index+24*11+10,183+10,"%",LIGHTBLUE,BLACK,32,0);

			LCD_DrawLine(0,60,320,0,YELLOW);//**************************************
			LCD_DrawLine(0,61,320,0,YELLOW);
			LCD_DrawLine(0,62,320,0,YELLOW);

			LCD_DrawLine(0,180,320,0,YELLOW);//***************************************
			LCD_DrawLine(0,181,320,0,YELLOW);
			LCD_DrawLine(0,182,320,0,YELLOW);	
	 }
	 else if(number == 2)
	 {
			LCD_ShowChinese(index,18,"状态",LIGHTBLUE,BLACK,24,0);
		 if(mode.status != WORK_ON)
			LCD_ShowChinese(index+24*3,18,"待机",LIGHTBLUE,BLACK,24,0);
			LCD_ShowChinese(index+24*7,18,"水温",LIGHTBLUE,BLACK,24,0);
			LCD_ShowIntNum(index+24*10,14,(unsigned int)(settings_params.water_temper),2,LIGHTBLUE,BLACK,32);
			LCD_ShowChinese(index+24*11+10,14,"℃",LIGHTBLUE,BLACK,32,0);

			LCD_ShowChinese(index,62+23,"设定",LIGHTBLUE,BLACK,32,0);
			LCD_ShowChinese(index,62+23+32*1,"温度",LIGHTBLUE,BLACK,32,0);
		//	LCD_ShowIntNum(index+24*3,62+42,(unsigned int)(settings_params.tar_set_temper),2,YELLOW,BLACK,32);
			LCD_ShowChinese(index+24*4+12,62+42,"℃",YELLOW,BLACK,32,0);

			LCD_ShowChinese(index+32*5,62+23,"当前",LIGHTBLUE,BLACK,32,0);
			LCD_ShowChinese(index+32*5,62+23+32*1,"温度",LIGHTBLUE,BLACK,32,0);
			LCD_ShowIntNum(index+24*10,62+42,(unsigned int)(settings_params.tar_env_temper),2,YELLOW,BLACK,32);
			LCD_ShowChinese(index+24*11+10,62+42,"℃",YELLOW,BLACK,32,0);

			LCD_ShowChinese(index+10,183+10,"模式",LIGHTBLUE,BLACK,24,0);
		 if(mode.modeNo == 0)
		 {	
			 mode.modeNo = COLD;
			 LCD_ShowChinese(index+10+24*2+10,183+10,"制冷",LIGHTBLUE,BLACK,24,0);
			 LCD_ShowString(index+10+24*4+10,183+10,"    ",BLACK,BLACK,24,0);
		 }
		 LCD_ShowChinese(index+24*7,183+10,"湿度",LIGHTBLUE,BLACK,24,0);
			LCD_ShowIntNum(index+24*10,183+10,(unsigned int)(settings_params.humid[0]),2,LIGHTBLUE,BLACK,32);
			LCD_ShowString(index+24*11+10,183+10,"%",LIGHTBLUE,BLACK,32,0);

			LCD_DrawLine(0,60,320,0,YELLOW);//**************************************
			LCD_DrawLine(0,61,320,0,YELLOW);
			LCD_DrawLine(0,62,320,0,YELLOW);

			LCD_DrawLine(0,180,320,0,YELLOW);//***************************************
			LCD_DrawLine(0,181,320,0,YELLOW);
			LCD_DrawLine(0,182,320,0,YELLOW);	
	 }
	 }
	

	
	
  
}

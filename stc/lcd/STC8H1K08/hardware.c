#include "hardware.h"
unsigned char code seg_code[]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,
                                0x7d,0x07,0x7f,0x6f,0x77,  0x7c,
                                0x39,0x5e, 0x79,0x71}; //0  1 2 3 4 5 6 7 8 9 A B C  D  E  F
//unsigned char code seg1_bit_ctrl[]= {S1_AP,S1_AN,S1_BP,S1_BN,S1_CP,S1_CN,S1_DP,S1_DN,S1_EP,
//                                     S1_EN,S1_FP,S1_FN,S1_GP,S1_GN,S1_DP_P,S1_DP_N
//                                    };//ap-an bp-bn ...dp_p-dp_n

//unsigned char code seg2_bit_ctrl[]= {S1_AP,S1_AN,S1_BP,S1_BN,S1_CP,S1_CN,S1_DP,S1_DN,S1_EP,
//                                     S1_EN,S1_FP,S1_FN,S1_GP,S1_GN,S1_DP_P,S1_DP_N
//                                    };//ap-an bp-bn ...dp_p-dp_n

//unsigned char code seg3_bit_ctrl[]= {S1_AP,S1_AN,S1_BP,S1_BN,S1_CP,S1_CN,S1_DP,S1_DN,S1_EP,
//                                     S1_EN,S1_FP,S1_FN,S1_GP,S1_GN,S1_DP_P,S1_DP_N};//ap-an bp-bn ...dp_p-dp_n
unsigned char seg1_dat[16];
unsigned char seg2_dat[16];
unsigned char seg3_dat[16];
unsigned char seg4_dat[2];
unsigned char seg5_dat[2];
void Delay200ms(void)	//@11.0592MHz
{
	unsigned char data i, j, k;

	i = 9;
	j = 104;
	k = 139;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void Delay5ms(void)	//@11.0592MHz
{
    unsigned char data i, j;

    i = 54;
    j = 199;
    do
    {
        while (--j);
    }
    while (--i);
}

void seg_bit_r(unsigned char digit,unsigned char dat,unsigned char seg_num)
{
    unsigned char i,j,tmp;

    switch(seg_num)
    {
    case 1:
        tmp = dat;
        for(i=0; i<7; i++)
        {
            j = tmp&0x01;
            tmp = tmp>>1;
            if(j==1)
            {
                
                seg1_dat[2*i] = 1;
                seg1_dat[2*i+1] = 0;

            }
            else
            {
                seg1_dat[2*i] = 0;
                seg1_dat[2*i+1] = 0;
            }
            
            if(digit == 1)
            {
                seg1_dat[14] = 1;
                seg1_dat[15] = 0;
            }
            else
            {
                seg1_dat[14] = 0;
                seg1_dat[15] = 0;

            }
           
        }

        break;
    case 2:
        for(i=0; i<7; i++)
        {
            j = tmp&0x01;
            tmp = tmp>>1;
            if(j==1)
            {
                seg2_dat[2*i] = 1;
                seg2_dat[2*i+1] = 0;

            }
            else
            {
                seg2_dat[2*i] = 0;
                seg2_dat[2*i+1] = 0;
            }
          
            if(digit == 1)
            {
                seg2_dat[14] = 1;
                seg2_dat[15] = 0;
            }
            else
            {
                seg2_dat[14] = 0;
                seg2_dat[15] = 0;

            }
           

        }


        break;
    case 3:
        for(i=0; i<7; i++)
        {
            j = tmp&0x01;
            tmp = tmp>>1;
            if(j==1)
            {
                seg3_dat[2*i] = 1;
                seg3_dat[2*i+1] = 0;

            }
            else
            {
                seg3_dat[2*i] = 0;
                seg3_dat[2*i+1] = 0;
            }
//            if(digit == 1)
//            {
//                seg3_dat[14] = 1;
//                seg3_dat[15] = 0;
//            }
//            else
//            {
//                seg3_dat[14] = 0;
//                seg3_dat[15] = 0;
//
//            }
           
        }

        break;

    }
	}
void seg_bit_ctrl(unsigned char seg_num)
{
unsigned char i;
	switch(seg_num)
	   {

		case 1:
            i= 0;
			S1_AP = seg1_dat[i++]==1?1:0;//Delay5ms();
			S1_AN = seg1_dat[i++]==1?1:0;Delay5ms();
			S1_BP = seg1_dat[i++]==1?1:0;//Delay5ms();
			S1_BN = seg1_dat[i++]==1?1:0;Delay5ms();
			S1_CP = seg1_dat[i++]==1?1:0;//Delay5ms();
			S1_CN = seg1_dat[i++]==1?1:0;Delay5ms();
			S1_DP = seg1_dat[i++]==1?1:0;//Delay5ms();
			S1_DN = seg1_dat[i++]==1?1:0;Delay5ms();
			S1_EP = seg1_dat[i++]==1?1:0;//Delay5ms();
			S1_EN = seg1_dat[i++]==1?1:0;Delay5ms();
			S1_FP = seg1_dat[i++]==1?1:0;//Delay5ms();
			S1_FN = seg1_dat[i++]==1?1:0;Delay5ms();
			S1_GP = seg1_dat[i++]==1?1:0;//Delay5ms();
			S1_GN = seg1_dat[i++]==1?1:0;Delay5ms();
			S1_DP_P = seg1_dat[i++]==1?1:0;//Delay5ms();
			S1_DP_N = seg1_dat[i++]==1?1:0;Delay5ms();		
			break;
	case 2:
		i= 0;
		S2_AP = seg2_dat[i++]==1?1:0;//Delay5ms();
		S2_AN = seg2_dat[i++]==1?1:0;Delay5ms();
		S2_BP = seg2_dat[i++]==1?1:0;//Delay5ms();
		S2_BN = seg2_dat[i++]==1?1:0;Delay5ms();
		S2_CP = seg2_dat[i++]==1?1:0;//Delay5ms();
		S2_CN = seg2_dat[i++]==1?1:0;Delay5ms();
		S2_DP = seg2_dat[i++]==1?1:0;//Delay5ms();
		S2_DN = seg2_dat[i++]==1?1:0;Delay5ms();
		S2_EP = seg2_dat[i++]==1?1:0;//Delay5ms();
		S2_EN = seg2_dat[i++]==1?1:0;Delay5ms();
		S2_FP = seg2_dat[i++]==1?1:0;//Delay5ms();
		S2_FN = seg2_dat[i++]==1?1:0;Delay5ms();
		S2_GP = seg2_dat[i++]==1?1:0;//Delay5ms();
		S2_GN = seg2_dat[i++]==1?1:0;Delay5ms();
		S2_DP_P = seg2_dat[i++]==1?1:0;//Delay5ms();
		S2_DP_N = seg2_dat[i++]==1?1:0;Delay5ms();		
		break;
	case 3:
		i= 0;
		S3_AP = seg3_dat[i++]==1?1:0;//Delay5ms();
		S3_AN = seg3_dat[i++]==1?1:0;Delay5ms();
		S3_BP = seg3_dat[i++]==1?1:0;//Delay5ms();
		S3_BN = seg3_dat[i++]==1?1:0;Delay5ms();
		S3_CP = seg3_dat[i++]==1?1:0;//Delay5ms();
		S3_CN = seg3_dat[i++]==1?1:0;Delay5ms();
		S3_DP = seg3_dat[i++]==1?1:0;//Delay5ms();
		S3_DN = seg3_dat[i++]==1?1:0;Delay5ms();
		S3_EP = seg3_dat[i++]==1?1:0;//Delay5ms();
		S3_EN = seg3_dat[i++]==1?1:0;Delay5ms();
		S3_FP = seg3_dat[i++]==1?1:0;//Delay5ms();
		S3_FN = seg3_dat[i++]==1?1:0;Delay5ms();
		S3_GP = seg3_dat[i++]==1?1:0;//Delay5ms();
		S3_GN = seg3_dat[i++]==1?1:0;Delay5ms();
		//S3_DP_P = seg3_dat[i++]==1?1:0;Delay5ms();
		//S3_DP_N = seg3_dat[i++]==1?1:0;Delay5ms();		
		break;
	case 4:
		i= 0;
		S4_AP = seg4_dat[i++]==1?1:0;//Delay5ms();
		S4_AN = seg4_dat[i++]==1?1:0;Delay5ms();
		
		break;
	case 5:
		i= 0;
		S5_VP = seg5_dat[i++]==1?1:0;//Delay5ms();
		S5_VN = seg5_dat[i++]==1?1:0;Delay5ms();
	
		break;

	}

}
void display(unsigned char digit,unsigned char dat,unsigned char seg_num)
{
		seg_bit_r(digit,seg_code[dat],seg_num);
		seg_bit_ctrl(seg_num);

}



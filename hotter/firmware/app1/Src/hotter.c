#include "hotter.h"
hotter_stru hotter_u0;
hotter_stru hotter_u1;
hotter_stru hotter_u2;
hotter_stru hotter_u3;
hotter_stru hotter_u4;
hotter_stru hotter_u5;
hotter_stru hotter_u6;

uint32_t eep;

float ep;
float *get_power()
{
	return &ep;
}
uint32_t *get_power2()
{
	return &eep;
}

void init_hotter()
{
	hotter_u0.status[0] = 1;
	hotter_u1.status[0] = 2;

	hotter_u2.status[0] = 3;
	hotter_u3.status[0] = 4;
	hotter_u4.status[0] = 5;

	hotter_u5.status[0] = 6;	
	hotter_u6.status[0] = 7;
	unsigned int i;
	for(i = 1;i<(STATUS1_SIZE-1);i++)
		{
	hotter_u0.status[i] = 200;
//	hotter_u1.status[i] = 201;
//	hotter_u2.status[i] = 202;
//	hotter_u3.status[i] = 203;

	}
		{
	hotter_u0.status[i] = 201;
//	hotter_u1.status[i] = 213;
//	hotter_u2.status[i] = 214;
//	hotter_u3.status[i] = 215;

	}

	for(i = 1;i<(STATUS2_SIZE-1);i++)
			{
	hotter_u0.status2[i] = 202;
//	hotter_u1.status2[i] = 205;
//	hotter_u2.status2[i] = 206;
//	hotter_u3.status2[i] = 207;

	}	
			{
	hotter_u0.status2[i] = 203;
//	hotter_u1.status2[i] = 217;
//	hotter_u2.status2[i] = 218;
//	hotter_u3.status2[i] = 219;

	}		
	for(i = 1;i<(STATUS3_SIZE-1);i++)
			{
	hotter_u0.status3[i] = 204;
//	hotter_u1.status3[i] = 209;
//	hotter_u2.status3[i] = 210;
//	hotter_u3.status3[i] = 211;

	}
			{
	hotter_u0.status3[i] = 205;
//	hotter_u1.status3[i] = 221;
//	hotter_u2.status3[i] = 222;
//	hotter_u3.status3[i] = 223;

	}

}
hotter_stru *get_hotter(unsigned dev_addr)
{
    hotter_stru *hotter_tmp;
	switch(dev_addr)
	{
		case 0:hotter_tmp = &hotter_u0;break;

		case 1:hotter_tmp = &hotter_u1;break;
		case 2:hotter_tmp = &hotter_u2;break;
		case 3:hotter_tmp = &hotter_u3;break;
		case 4:hotter_tmp = &hotter_u4;break;
		case 5:hotter_tmp = &hotter_u5;break;

		default:hotter_tmp = &hotter_u6;break;

	}
	return hotter_tmp;
}


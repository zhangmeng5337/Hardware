#include "hotter.h"

hotter_stru hotter_u1;
hotter_stru hotter_u2;
hotter_stru hotter_u3;
hotter_stru hotter_u4;



void init_hotter()
{
	

}
hotter_stru *get_hotter(unsigned dev_addr)
{
    hotter_stru *hotter_tmp;
	switch(dev_addr)
	{
		case 1:hotter_tmp = &hotter_u1;break;
		case 2:hotter_tmp = &hotter_u2;break;
		case 3:hotter_tmp = &hotter_u3;break;
		case 4:hotter_tmp = &hotter_u4;break;
		default:hotter_tmp = &hotter_u1;break;

	}
	return hotter_tmp;
}


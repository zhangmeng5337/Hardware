#include <math.h>

//日期序列化
unsigned short dateToInt(unsigned short year, unsigned char month, unsigned char day)
{
	int m;
	int days[] = {0,31,59,90,120,151,181,212,243,273,304,334};
	m = days[month - 1];
 	m = m + day;

	if(year % 400 == 0 || (year % 4 == 0 && year % 100 !=0))
		m++;
	return  m;
}

double GetSunriseAndSunset(signed short longitude_du, signed char longitude_fen, signed short latitude_du, signed char latitude_fen, double tz, unsigned short year, unsigned char month, unsigned char day, signed char sunrise) 
{
	double longitude, latitude;
	longitude =	(double)longitude_fen /60.0 + longitude_du;
	latitude  =	(double)latitude_fen /60.0  + latitude_du;

	return (180 + tz * 15 - longitude + sunrise * acos(tan(0.40906639157298210*cos(6.283185307179586*(dateToInt(year,month,day) + 9)/365))*tan(latitude*0.0174532925199433))*57.29577951308233)/15;
}

//调用方式
void main(void)
{
	double mytime;

	//日出
	mytime = GetSunriseAndSunset(116, 23, 39, 54, 8.0, 2018, 1, 1, -1);
	
	//日落
	mytime = GetSunriseAndSunset(116, 23, 39, 54, 8.0, 2018, 1, 1, 1);
	return;
}

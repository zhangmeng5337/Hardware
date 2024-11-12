#include "math.h"
#define M_PI       3.14159265358979323846

double RAD = 180.0 * 3600 /M_PI;

double dateToDouble(int year, int month, int day)
{
	double jd;
	int leap = 0;
	if (month <= 2)
	{
		month += 12;
		year --;
	}
	if ((long)year*372 + month*31 + day >= 588829)
	{
		leap = year / 100;
		leap = 2 - leap + (int)(leap / 4);
	}
	jd = floor(365.25 * (year + 4716)) + floor(30.60001 * (month + 1)) + day + leap - 1524.5;
	return jd;
}

void doubleToTime(double time, int* hour, int* min, int* sec)
{
	double t;

	t = time + 0.5;
	t = (t - (int) t) * 24;
	*hour = (int) t;
	t = (t - *hour) * 60;
	*min = (int) t;
	t = (t - *min) * 60;
	*sec = (int) t;
}

double sunHJ(double t)
{
	double j;
	t = t + (32.0 * (t + 1.8) * (t + 1.8) - 20) / 86400.0 / 36525.0;
	j = 48950621.66 + 6283319653.318 * t + 53 * t * t - 994 + 334166 *cos(4.669257 + 628.307585 * t) + 3489 * cos(4.6261 + 1256.61517 * t) + 2060.6 * cos(2.67823 + 628.307585 * t) * t;
	return (j / 10000000);
}

double mod(double num1, double num2)
{
	num2 = fabs(num2);
	if (num1 >= 0)
		return (num1 - (int)(num1 / num2) * num2 );
	else
		return ((int)(fabs(num1) / num2) * num2 - fabs(num1));
}

double degree(double ag)
{
	ag = mod(ag, 2 * M_PI);

	if (ag <= -M_PI)
		ag = ag + 2*M_PI;
	else if( ag > M_PI)
		ag = ag - 2*M_PI;
	return ag;
}

double sunRiseTime(double date, double longitude, double latitude, double tz, double* midDayTime, double* dawnTime)
{
	double t, j, sinJ, cosJ, gst, E,a,D,cosH0, cosH1,H0,H1,H;
	date -= tz;
	t = date / 36525;
	j = sunHJ(t);

	sinJ = sin(j);
	cosJ = cos(j);

	gst = 2 * M_PI * (0.779057273264 + 1.00273781191135448 * date) + (0.014506 + 4612.15739966 * t + 1.39667721 * t * t) / RAD;
	E = (84381.406 - 46.836769 * t) / RAD;
	a = atan2(sinJ * cos(E), cosJ);
	D = asin(sin(E) * sinJ);
	cosH0 = (sin(-50 * 60 / RAD) - sin(latitude) * sin(D)) / (cos(latitude) * cos(D));
	cosH1 = (sin(-6 * 3600 / RAD) - sin(latitude) * sin(D)) / (cos(latitude) * cos(D));
	if (cosH0 >= 1 || cosH0 <= -1)
		return -0.5;
	H0 = -acos(cosH0);
	H1 = -acos(cosH1);
	H = gst - longitude - a;

	*midDayTime = date - degree(H) / M_PI / 2 + tz;
	*dawnTime   = date - degree(H - H1) / M_PI / 2 + tz;
	return date - degree(H - H0) / M_PI / 2 + tz;
}

void GetSunriseAndSunset(double longitude, double latitude, double tz, int year, int month, int day, int* sunrise, int* sunset)
{
	int i;
	int hour, min, sec;
	double sun_Rise, midDayTime, dawnTime;
	longitude = -longitude / 180 * M_PI;
	latitude  = latitude  / 180 * M_PI;

	sun_Rise = dateToDouble(year, month, day) - 2451544.5;
	for (i=0; i<10; i++)
		sun_Rise = sunRiseTime(sun_Rise, longitude, latitude, tz/24.0, &midDayTime, &dawnTime);

	doubleToTime(sun_Rise, &hour, &min, &sec);
	sunrise[0]=hour;
	sunrise[1]=min;
	sunrise[2]=sec;

	doubleToTime(midDayTime + midDayTime - sun_Rise, &hour, &min, &sec);
	sunset[0]=hour;
	sunset[1]=min;
	sunset[2]=sec;
}

void main(void)
{
	int sunrise[3], sunset[3];
	GetSunriseAndSunset(106.7166,26.58333,8,2018,7,22, sunrise, sunset);
}

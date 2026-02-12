#include "filter.h"
float average_filter(float bArray)
{
    static float  sum = 0, result;
    static unsigned int count, flag = 0, i;
    static float buf[FILTER_SIZE];
    if (count < FILTER_SIZE)
    {
        buf[count] = bArray;
        count++;
    }
    else
    {

        flag = 1;
        count = 0;

    }
    sum = 0;
    if (flag == 1)
    {
        for (i = 0; i < FILTER_SIZE; i++)
        {
            sum  = sum + buf[i];

        }
        result = sum / FILTER_SIZE;
    }
    else
    {

        for (i = 0; i < count; i++)
        {
            sum  = sum + buf[i];

        }
        result = sum / count;

    }

    return result;
}


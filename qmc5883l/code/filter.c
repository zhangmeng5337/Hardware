#define N		20
#define BUFFERSIZE	60
void smooth(unsigned int  *data,unsigned int *result,unsigned int count)
{
        unsigned int i,k;
	unsigned int sum1;
	if(count<N)
	{
		for(i=0;i<count;i++)
		{
			for(k=0;k<=i;k++)
			{
			   sum1 = sum1 + data[k];
			}
			result[i] = sum1 /k;
		}
	}
	else
	{
		for(i=(count-N+1);i<count;i++)
		{
			   sum1 = sum1 + data[i];
			
		}
		result[count] = sum1 /N;
	}
}

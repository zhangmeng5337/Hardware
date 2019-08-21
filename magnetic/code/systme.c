uint32_t TimingCount=0;
static start_flag;

TimingStop(uint32_t)
{
	start_flag = 0;
}

TimingStart(uint32_t tCount)
{

	
	if(start_flag ==0 )
	{
		TimingCount = gettick();
		start_flag = 1;
	}
	else
	{
		if(gettick() - TimingCount >=tCount )
		{
			start_flag = 0;
			return 1;
		}	
	}
	return 0;
}




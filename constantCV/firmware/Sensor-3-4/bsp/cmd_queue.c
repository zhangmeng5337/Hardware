#include "cmd_queue.h"


typedef struct _QUEUE
{
	qsize _head;
	qsize _tail;
	qdata _data[QUEUE_MAX_SIZE];	
}QUEUE;

static QUEUE que = {0,0,0};
static qsize cmd_pos = 0;
static qdata _length=0;


void queue_reset()
{
	que._head = que._tail = 0;
	cmd_pos =  0;
}

void queue_push(qdata _data)
{
	qsize pos = (que._head+1)%QUEUE_MAX_SIZE;
	if(pos!=que._tail)//����״̬
	{
		que._data[que._head] = _data;
		que._head = pos;
	}
}

static void queue_pop(qdata* _data)
{
	if(que._tail!=que._head)//�ǿ�״̬
	{
		*_data = que._data[que._tail];
		que._tail = (que._tail+1)%QUEUE_MAX_SIZE;
	}
}

static qsize queue_size()
{
	return ((que._head+QUEUE_MAX_SIZE-que._tail)%QUEUE_MAX_SIZE);
}

qsize queue_find_cmd(qdata *buffer,unsigned char head,qsize buf_len)
{
	qsize cmd_size = 0;
	qdata _data = 0;
	
	while(queue_size()>0)
	{
		//ȡһ������
		queue_pop(&_data);

		if(cmd_pos==0&&_data!=head)//֡ͷ��������
		    continue;

		if(cmd_pos<buf_len)//��ֹ���������
		{
			if(cmd_pos ==0)
			{
				_length = _data+1;
				if(_length > (CMD_MAX_SIZE-3))
				{
					cmd_pos =0;
					_length =0;
				}
			}
			buffer[cmd_pos++] = _data;
		}


		//�õ�������֡
		if(_length >0 && cmd_pos>_length)
		{
			cmd_size = cmd_pos;
			cmd_pos = 0;
			
			if(!CheckSum(buffer,_length))//����У���
			{
				_length = 0;
				return 0;
			}
			_length = 0;
			
			return cmd_size;
		}
	}

	return 0;//û���γ�������һ֡
}


uint8_t CheckSum(uint8_t *buffer,uint8_t n)
{
	uint8_t i,chksum=0;
	for(i=1;i<(n);i++)
		chksum += buffer[i];
	
	if(chksum ==  buffer[i])
		return 0xff;
	else
		return 0;
}




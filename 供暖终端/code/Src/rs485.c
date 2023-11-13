#include "rs485.h"
#include "uart.h"

rs485_stru rs485_u;

rs485_stru get_rs485_recv()
{
	return rs485_u;
}


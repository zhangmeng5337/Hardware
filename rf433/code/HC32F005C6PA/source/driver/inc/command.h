#ifndef COMMAND_H
#define COMMAND_H
#define COMMAND_FUNC_CONFIG     			0x01
#define COMMAND_FUNC_CTRL				0x00
#define COMMAND_FUNC_SN_CONFIG		0x02
#define WAKE_HEAD						0xFF
#define WAKE_TAG						0xFE


#define CONFIGSN					0x0001
#define STARTWORK				0x0002
#define BREAKWORK				0x0004
#define STOPWORK				0x0008

#define COMMAND_FUNC_SN_CONFIG_LEN		0x0a
#define COMMAND_CONFIG_LEN       9
#define COMMAND_CTRL_LEN            8
#define MAX_COUNT				3
void uart_ack_response(unsigned char  cmd_type);
boolean_t command_process(void);
#endif


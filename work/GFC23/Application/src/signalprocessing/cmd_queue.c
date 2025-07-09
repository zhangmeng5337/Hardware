#include "cmd_queue.h"

typedef struct _QUEUE {
	qsize _head;
	qsize _tail;
	qdata _data[QUEUE_MAX_SIZE];	
} QUEUE;

static QUEUE que = {0,0,0};
static qsize cmd_pos = 0;
static qdata _length=0;

/**
  @brief reset the entire queue
*/
void queue_reset() {
	que._head = que._tail = 0;
	cmd_pos =  0;
}

/**
  @brief Push data to the queue
*/
void queue_push(qdata _data) {
	qsize pos = (que._head+1)%QUEUE_MAX_SIZE;
	if(pos!=que._tail) {
		que._data[que._head] = _data;
		que._head = pos;
	}
}

/**
  @brief pop data from the queue
*/
static void queue_pop(qdata* _data) {
	if(que._tail!=que._head) {
		*_data = que._data[que._tail];
		que._tail = (que._tail+1)%QUEUE_MAX_SIZE;
	}
}

/**
  @brief et the current queue size
*/
static qsize queue_size() {
	return ((que._head+QUEUE_MAX_SIZE-que._tail)%QUEUE_MAX_SIZE);
}

/**
  @brief Find a good frame from the que and return the size of that frame, along with the pointer to the data set.
*/
qsize queue_find_cmd(qdata *buffer, qsize buf_len) {
	qsize cmd_size = 0;
	qdata _data = 0;
	
	while(queue_size()>0) {
 		queue_pop(&_data);
		if(cmd_pos==0&&_data!=HEAD) {
		    continue;
    }
    
		if(cmd_pos<buf_len) {
			if(cmd_pos ==0) {
				_length = _data+1;
				if(_length > 63) {
					cmd_pos =0;
					_length =0;
				}
			}
			buffer[cmd_pos++] = _data;
		}

		if(_length >0 && cmd_pos>_length) {
			cmd_size = cmd_pos;
			cmd_pos = 0;			
			if(!CheckSum(buffer,_length)) {
				_length = 0;
				return 0;
			}
			_length = 0;			
			return cmd_size;
		}
	}
	return 0;
}

/**
  @brief Calcualte checksum of CDG frames. Nothing fancy here, it is just an 8-bit sum of data set starting from
         Array index = 1 to length of the packet
  @input buffer - pointer to data fram
         n - length of frame
  @output 
         0xFF - cheksum is valid
         0x00 - checksum in invalid
*/
uint8_t CheckSum(uint8_t *buffer, uint8_t n) {
	uint8_t i,chksum=0;
	for(i=1; i<n; i++) {
		chksum += buffer[i];	
  }
  // return corresponding output if checksum is good
	if(chksum == buffer[i]) {
		return 0xff;
  } else {
		return 0;
  }
}

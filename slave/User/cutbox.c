#include "cutbox.h"








uint8_t cutbox_number(uint8_t target, uint8_t command)
{
	if(4 == target){
		switch(command){ //兆欧表
			case 0x00:
			case 0x04:
			case 0x05:
			case 0x06:
			case 0x07:return 1;
			
			case 0x01:
			case 0x08:
			case 0x09:
			case 0x0A:
			case 0x0B:return 2;
			
			case 0x02:
			case 0x0C:
			case 0x0D:
			case 0x0E:
			case 0x0F:return 3;
		}
	}else if(5 == target){ //介损
		switch(command){
			case 0x00:
			case 0x03:
			case 0x04:
			case 0x05:
			case 0x06:return 1;
			case 0x01:
			case 0x07:
			case 0x08:
			case 0x09:
			case 0x0A:return 2;
			case 0x02:
			case 0x0B:
			case 0x0C:
			case 0x0D:
			case 0x0E:return 3;
		}
	}else if(6 == target){ //直流电阻
		switch(command){
			case 0x00:
			case 0x01:
			case 0x02:
			case 0x03:
			case 0x04:
			case 0x05:return 1;
			case 0x06:
			case 0x07:
			case 0x08:
			case 0x09:
			case 0x0A:
			case 0x0B:return 2;
			case 0x0C:
			case 0x0D:
			case 0x0E:
			case 0x0F:
			case 0x10:
			case 0x11:return 3;
		}
	}else if(7 == target){ //有载分接
		switch(command){
			case 0x00:return 1;
			case 0x01:return 2;
			case 0x02:return 3;
		}
	}
	return 0;
}


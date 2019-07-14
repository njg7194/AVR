/*
 * BluetoothSetting.c
 *
 * Created: 2018-11-03 오후 3:03:43
 *  Author: njg71
 */ 
#include "Bluetooth.h"

char BLTdiscrimination(const char* buffer)
{
	if(strstr(buffer, "TOGGLE_AUTO"))
	return 'T';
	else
	return whatDirection(buffer);
}

char whatDirection(const char* data_buffer)
{
	if (strstr(data_buffer,"stop"))
	return 'S';
	else
	{
		if (strstr(data_buffer,"FORWARD"))
		return 'F';
		
		else if(strstr(data_buffer, "LEFT"))
		return 'L';
		
		else if(strstr(data_buffer, "RIGHT"))
		return 'R';
		
		else if(strstr(data_buffer, "BACKWARD"))
		return 'B';
		
		else if(strstr(data_buffer, "LEFT_SPEED"))
		return '4';
		
		else if(strstr(data_buffer, "RIGHT_SPEED"))
		return '6';
	}
	return 0;
}
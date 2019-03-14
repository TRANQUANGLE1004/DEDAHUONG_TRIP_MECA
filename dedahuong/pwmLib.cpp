#include"pwmLib.h"
#include<Arduino.h>
void setDirection(bool _dirO, bool _dirL, bool _dirR){
	digitalWrite(DIRECTION_OMNI, _dirO);
	digitalWrite(DIRECTION_MECANUM_LEFT, _dirL);
	digitalWrite(DIRECTION_MECANUM_RIGHT, _dirR);
}

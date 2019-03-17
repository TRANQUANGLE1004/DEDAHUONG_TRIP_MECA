#include"pwmLib.h"
#include<Arduino.h>
void setDirection(bool _dirO, bool _dirL, bool _dirR){
	digitalWrite(PIN_DIR_OMNI, _dirO);
	digitalWrite(PIN_DIR_MECANUM_LEFT, _dirL);
	digitalWrite(PIN_DIR_MECANUM_RIGHT, _dirR);
}

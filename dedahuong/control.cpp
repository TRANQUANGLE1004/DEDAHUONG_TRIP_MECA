#include"control.h"
#include<Arduino.h>
void controlOmni(int _speed) {
	if (_speed >= 0) {
		analogWrite(PIN_PULSE_OMNI, MAX_VALUE - _speed);
		digitalWrite(PIN_DIR_OMNI, DIRECTION_OMNI);
	}
	else {
		analogWrite(PIN_PULSE_OMNI, MAX_VALUE + _speed);
		digitalWrite(PIN_DIR_OMNI, !DIRECTION_OMNI);
	}

}
void controlMecanumLeft(int _speed) {
	if (_speed >= 0) {
		analogWrite(PIN_PULSE_MECANUM_LEFT, MAX_VALUE - _speed);
		digitalWrite(PIN_DIR_MECANUM_LEFT, DIRECTION_MECANUM_LEFT);
	}
	else {
		analogWrite(PIN_PULSE_MECANUM_LEFT, MAX_VALUE + _speed);
		digitalWrite(PIN_DIR_MECANUM_LEFT, !DIRECTION_MECANUM_LEFT);
	}

}
void controlMecanumRight(int _speed) {
	if (_speed >= 0) {
		analogWrite(PIN_PULSE_MECANUM_RIGHT, MAX_VALUE - _speed);
		digitalWrite(PIN_DIR_MECANUM_RIGHT, DIRECTION_MECANUM_RIGHT);
	}
	else {
		analogWrite(PIN_PULSE_MECANUM_RIGHT, MAX_VALUE + _speed);
		digitalWrite(PIN_DIR_MECANUM_RIGHT, !DIRECTION_MECANUM_RIGHT);
	}

}
void goStraight(int _speed) {
	controlOmni(_speed);
	controlMecanumLeft(_speed);
	controlMecanumRight(_speed);
}

void move(double _omega, double _vRx, double _vRy) {
	double _vA = _vRx + _vRy + _omega / 2;
	double _vB = _vRx + _vRy + _omega / 2;
	double _vO = _vRx - _omega / 2;//
	//
	if (abs(_vO) <= 2) {
		_vO = 2;
	}
	if (abs(_vA) <= 2) {
		_vA = 2;
	}
	if (abs(_vB) <= 2) {
		_vB = 2;
	}
	//
	controlOmni(_vO);
	controlMecanumLeft(_vB);
	controlMecanumRight(_vA);
}

void goBack(int _speed) {
	controlOmni(-_speed);
	controlMecanumLeft(-_speed);
	controlMecanumRight(-_speed);
}

void run(double _alpha, double _speed) {
	move(0, (int)_speed*cos(_alpha), (int)_speed*sin(_alpha));
}

void circle(int _speed) {
	controlOmni(_speed);
	controlMecanumLeft(LOCK);
	controlMecanumRight(LOCK);
}

void freeRun(){
	controlOmni(FREE_STATE);
	controlMecanumLeft(FREE_STATE);
	controlMecanumRight(FREE_STATE);
}

void runLeft(int _speed){
	controlOmni(_speed);
	controlMecanumLeft(-_speed);
	controlMecanumRight(-_speed);
}

void runRight(int _speed){
	controlOmni(-_speed);
	controlMecanumLeft(_speed);
	controlMecanumRight(_speed);
}

void stopState(){
	TCCR1B &= 0xF8;
	TCCR4B &= 0xF8;
}

void hamtangtoc(int _time) {

}


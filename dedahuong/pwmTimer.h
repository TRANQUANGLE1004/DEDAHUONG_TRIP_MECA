#pragma once
#ifndef _PWM_TIMER_H_
#define _PWM_TIMER_H_
#include<avr/interrupt.h>
#include<Arduino.h>
#include"pwmLib.h"
#define fd			(1.0f/16000000.0f)	// frequency default atmega 2560
#define fre			16000
#define FIFTY_MS	62410	
/*
use pin ???
*/

// volatile variable
volatile int count = 0;
volatile int mode = 0;
volatile int task = 0;
volatile int taskTimer5 = 0;
//

//
unsigned short timer3;
void genPWMTimer1(int frequency);
void genPWMTimer4(int frequency);
unsigned short settingPulseUseTimer3(int _time);
void settingPulseUseTimer5(int _time);
unsigned short getBottomTimerNomalMode(int _time);
void runTripMeca(int _vO, int _vL, int _vR);
// _num is pulse/1s ex: 1000 pulse/second

void pulseGenMecanum(int _num, int _time = 0) { //use timer 1 & timer 3
	if (_time == 0) {
		if (_num > 0) {
			digitalWrite(PIN_DIR_MECANUM_LEFT, DIRECTION_MECANUM_LEFT);
			digitalWrite(PIN_DIR_MECANUM_RIGHT, DIRECTION_MECANUM_RIGHT);
			genPWMTimer1(_num);
		}
		else {
			digitalWrite(PIN_DIR_MECANUM_LEFT, !DIRECTION_MECANUM_LEFT);
			digitalWrite(PIN_DIR_MECANUM_RIGHT, !DIRECTION_MECANUM_RIGHT);
			genPWMTimer1(-_num);
		}

	}
	else {
		if (_num > 0) {
			digitalWrite(PIN_DIR_MECANUM_LEFT, DIRECTION_MECANUM_LEFT);
			digitalWrite(PIN_DIR_MECANUM_RIGHT, DIRECTION_MECANUM_RIGHT);
			genPWMTimer1(_num);
			//TCCR3B |= (1 << CS32);// bat timer3 || prescaler = 256
		}
		else {
			digitalWrite(PIN_DIR_MECANUM_LEFT, !DIRECTION_MECANUM_LEFT);
			digitalWrite(PIN_DIR_MECANUM_RIGHT, !DIRECTION_MECANUM_RIGHT);
			genPWMTimer1(-_num);
			//settingPulseUseTimer3(_time);
			//TCCR3B |= (1 << CS32);// bat timer3 || prescaler = 256
		}
		// khoi dong timer 3

	}
}
void pulseGenOmni(int _num, int _time = 0) { // use timer 4,5
	if (_time == 0) {
		if (_num > 0) {
			digitalWrite(PIN_DIR_OMNI, DIRECTION_OMNI);
			//delay(50);
			genPWMTimer4(_num);
		}
		else {
			digitalWrite(PIN_DIR_OMNI, !DIRECTION_OMNI);
			//delay(50);
			genPWMTimer4(-_num);
		}

	}
	else {
		// timer 5
	}
}
void initPwmTimer1() {
	TCCR1A = 0;// reset reg
	TCCR1B = 0;// reset reg
	DDRB |= (1 << PB5); // <=> pinMode(11,OUTPUT) ^^;
	DDRB |= (1 << PB7); // <=> pinMode(13,OUTPUT) ^^; OC1C
	TCCR1A |= (1 << COM1A1) | (1 << WGM11); // choose kenh A (COM1A1 COM1A0) || WGM choose mode (this mode is fast PWM )
	TCCR1A |= (1 << COM1C1); //choose clear on compare match chanel C
	TCCR1B |= (1 << WGM12) | (1 << WGM13);// CS10 choose prescaler (this case prescaler = f default)
	//TCCR1B |= (1 << CS10);
}

unsigned short getValuePwm(int frequency) {
	unsigned short _val;
	float t = (1.0f / (float)frequency) / (fd);
	_val = (unsigned short)t - 1;
	//Serial.println(_val);
	return _val;
}

void genPWMTimer1(int frequency) {//use timer1
	// caculate 
	unsigned short _val;
	float t = (1.0f / (float)frequency) / (fd);
	_val = (unsigned short)t - 1;
	//Serial.println(_val);
	ICR1 = _val;
	OCR1A = 1984;
	OCR1C = 1984;
	TCCR1B |= (1 << CS10); // khoi dong pwm
}
void initTimer3() {
	TCCR3B = 0;
	TCCR3A = 0;
	TIMSK3 = 0;
	TIMSK3 |= (1 << TOIE3);
	TCCR3B |= (1 << CS32);
	TCNT3 = getBottomTimerNomalMode(10);
}
void initTimer5() {
	//Serial.println("Init timer 5");
	TCCR5B = 0;
	TCCR5A = 0;
	TIMSK5 = 0;
	TIMSK5 |= (1 << TOIE5);
	TCCR5B |= (1 << CS52);
	TCNT3 = getBottomTimerNomalMode(10);
}

unsigned short settingPulseUseTimer3(int _time) {
	// time unit is ms
	//setting timer 3
	//reset reg
	//int prescaler;
	/*	table choose prescaler		CSn2	CSn1	CSn0
	0 -> 4ms	use		/1			0		0		1

	0 -> 32ms	use		/8			0		1		0

	0 -> 262ms	use		/64			0		1		1

	0 -> 1048ms use		/256		1		0		0

	0 -> 4194ms	use		/1024		1		0		1
	*/
	// this case time is 1s i choose  /256
	float t = (float)_time / (256.0f*(1.0f / 16000.0f));
	// I use Nomal Mode 
	unsigned short _val = 65535 - (unsigned short)t - 1;
	//Serial.println(_val);
	TCNT3 = _val;
	//TCCR3B |= (1 << CS32);
	TIMSK3 |= (1 << TOIE3); //cho phep ngat tran
	return _val;
}//use timer 3

void settingPulseUseTimer5(int _time) {
	// time unit is ms
	//setting timer 3
	//reset reg
	TCCR5A = 0;
	TCCR5B = 0;
	TIMSK5 = 0;
	TIMSK5 |= (1 << TOIE5); //cho phep ngat tran

	/*	table choose prescaler		CSn2	CSn1	CSn0
	0 -> 4ms	use		/1			0		0		1

	0 -> 32ms	use		/8			0		1		0

	0 -> 262ms	use		/64			0		1		1

	0 -> 1048ms use		/256		1		0		0

	0 -> 4194ms	use		/1024		1		0		1
	*/

	// this case time is 1s i choose  /256
	float t = (float)_time / (256.0f*(1.0f / 16000.0f));
	// I use Nomal Mode 
	unsigned short _val = 65535 - (unsigned short)t - 1;
	TCNT5 = _val;
	//Serial.println(_val);
}//use timer 
void initPwmTimer4() {
	TCCR4A = 0;// reset reg
	TCCR4B = 0;// reset reg
	DDRH |= (1 << PH3); // <=> pinMode(6,OUTPUT) ^^;
	TCCR4A |= (1 << COM4A1) | (1 << WGM41); // choose kenh A (COM1A1 COM1A0) || WGM choose mode (this mode is fast PWM )
	TCCR4B |= (1 << WGM42) | (1 << WGM43); // CS10 choose prescaler (this case prescaler = f default)
	//TCCR4B |= (1 << CS40);
}

void genPWMTimer4(int frequency) {//use timer4
		// caculate 
	unsigned short _val;
	float t = (1.0f / (float)frequency) / fd;
	_val = (unsigned short)t - 1;
	//
	//Serial.println(_val);
	ICR4 = _val;
	OCR4A = 1984;
	TCCR4B |= (1 << CS40);
}
void settingTimer1(int _valMecaLeft, int _valMecaRight, int _icrVal = 8000);
void settingTimer4(int _val, int _icrVal = 8000);
void rotateClockWise(int _num) {
	digitalWrite(PIN_DIR_MECANUM_LEFT, DIRECTION_MECANUM_LEFT);
	digitalWrite(PIN_DIR_MECANUM_RIGHT, !DIRECTION_MECANUM_RIGHT);
	digitalWrite(PIN_DIR_OMNI, !DIRECTION_OMNI);
	settingTimer1(_num, _num);
	settingTimer4(_num);
}

void rotateInvertClockWise(int _num) {
	digitalWrite(PIN_DIR_MECANUM_LEFT, !DIRECTION_MECANUM_LEFT);
	digitalWrite(PIN_DIR_MECANUM_RIGHT, DIRECTION_MECANUM_RIGHT);
	digitalWrite(PIN_DIR_OMNI, DIRECTION_OMNI);
	settingTimer1(_num, _num);
	settingTimer4(_num);
}



//***********************************************************************************************************************

void run(int maxFre, int _pulseSum, byte _mode = 0) { // 12000 pulse in 3s
	// mode default divide 

}


unsigned short getICRFastPWM(int _frequency) {
	float t = (1.0f / (float)_frequency) / (fd);
	unsigned short _val = (unsigned short)t - 1;
	return _val;
}
// _time (ms)
unsigned short getBottomTimerNomalMode(int _time) {
	float t = (float)_time / (256.0f*(1.0f / 16000.0f));
	// I use Nomal Mode 
	unsigned short _val = 65535 - (unsigned short)t - 1;
	return _val;
}

void settingTimer1(int _valMecaLeft, int _valMecaRight, int _icrVal = 8000) { // use Fast PWM
	unsigned short topPWM = getValuePwm(_icrVal);
	OCR1C = (int)((float)_valMecaLeft*(float)topPWM / 255.0f);
	OCR1A = (int)((float)_valMecaRight*(float)topPWM / 255.0f);
	ICR1 = topPWM;
	// Notice !!!
}
void settingTimer4(int _val, int _icrVal = 8000) { // use Fast PWM
	unsigned short topPwm = getValuePwm(_icrVal);
	OCR4A = (int)((float)_val*(float)topPwm / 255.0f);
	ICR4 = topPwm;
}
void runTimer1() { TCCR1B |= (1 << CS10); }
void runTimer4() { TCCR4B |= (1 << CS40); }
void stopTimer1() { TCCR1B &= 0xF8; }
void stopTimer4() { TCCR4B &= 0xF8; }
void stopTimer3() { TCCR3B &= 0xF8; }
void stopTimer5() { TCCR5B &= 0xF8; }
void stopState() {
	settingTimer1(253, 253);
	settingTimer4(253);
	runTimer1();
	runTimer4();
}

// 5 distance //
// _firstValueChanelA is ICR value
void timerFuncIncreFre(int _time, int _firstValue, int _endValue, int _smooth, int & _getSpeed, byte _mode = 0) {
	//Serial.println("Debug :: time Incre Func");
	_firstValue = 253 - _firstValue;
	_endValue = 253 - _endValue;
	unsigned short time = getBottomTimerNomalMode(_time);
	float step = (float)(_endValue - _firstValue) / (float)_smooth;
	if (count == 0) {
		//
		settingTimer1(_firstValue, _firstValue);
		settingTimer4(_firstValue);
		/*OCR1A = _icrVal;
		OCR1C = _icrVal;
		OCR4A = _icrVal;*/
		runTimer1();
		runTimer4();
		TCNT3 = time;
		TCCR3B |= (1 << CS32);
		//
		count++;
		//Serial.println("hihi0");
	}
	else {
		if (_smooth == count) {
			if (_mode == 0) {
				TCNT3 = time;
				count &= 0;
				mode++;
				return;
			}
			else if (_mode == 1) {
				count &= 0;
				stopState();
				stopTimer3();
				return;
			}
			else if (_mode == 2) {
				//
				TCNT3 = time;
				return;
			}
			//stopState();

		}
		_getSpeed = (int)((float)_firstValue + step * (float)count);
		//Serial.println(_getSpeed);
		settingTimer1(_getSpeed, _getSpeed);
		settingTimer4(_getSpeed);
		TCNT3 = time;
		//TCCR3B |= (1 << CS32);
		count++;
		//Serial.println("hihi");
	}
}

// mode 0: -> -> ->
//mode 1: dependent
void timerFuncDecreFre(int _time, int _firstValue, int _endValue, int _smooth, int & _getSpeed, byte _mode = 0) {
	_firstValue = 253 - _firstValue;
	_endValue = 253 - _endValue;
	unsigned short time = getBottomTimerNomalMode(_time);
	float step = (float)(_endValue - _firstValue) / (float)_smooth;
	if (count == 0) {
		//
		settingTimer1(_firstValue, _firstValue);
		settingTimer4(_firstValue);
		/*OCR1A = _icrVal;
		OCR1C = _icrVal;
		OCR4A = _icrVal;*/
		runTimer1();
		runTimer4();
		TCNT3 = time;
		TCCR3B |= (1 << CS32);
		//
		count++;
		//Serial.println("hihi0");
	}
	else {
		if (_smooth == count-1) {
			if (_mode == 0) {
				TCNT3 = time;
				count &= 0;
				mode &= 0;
				task++;
				return;
			}
			else {
				Serial.println("Stop giam toc");
				count &= 0;
				stopState();
				stopTimer3();
				task = 1234;
				return;
			}
		}
		_getSpeed = (int)((float)_firstValue + step * (float)count);
		settingTimer1(_getSpeed,_getSpeed);
		settingTimer4(_getSpeed);
		TCNT3 = time;
		//TCCR3B |= (1 << CS32);
		count++;
		//Serial.println("hihi");
	}
}

// delta is chenh lech 2 banh Right - Left
void runAndCalib(int _speed, int _delta) {
	// _angle and _speed ==> Vax ()g
	int _valTemp = (int)((float)_delta / 2.0);
	settingTimer1(_speed + _valTemp, _speed - _valTemp);
	settingTimer4(_speed - _valTemp);
	// out put change OCR1C abd OCR1A + OCR4A
}
void backAndCalib(int _speed, int _delta) {
	// _angle and _speed ==> Vax ()g
	int _valTemp = (int)((float)_delta / 2.0);
	settingTimer1(_speed - _valTemp, _speed + _valTemp);
	settingTimer4(_speed + _valTemp);
	// out put change OCR1C abd OCR1A + OCR4A
}
void stopAndCalib(int _delta) {
	int _current = 253 - abs(_delta);
	if (_delta >= 0) {
		setDirection(DIRECTION_OMNI,!DIRECTION_MECANUM_LEFT,DIRECTION_MECANUM_RIGHT);
		settingTimer1(_current, _current);
		settingTimer4(_current);
		runTimer1();
		runTimer4();
	}
	else {
		setDirection(!DIRECTION_OMNI, DIRECTION_MECANUM_LEFT, !DIRECTION_MECANUM_RIGHT);
		settingTimer1(_current, _current);
		settingTimer4(_current);
		runTimer1();
		runTimer4();
	}
}

void runAngle(int _angle,int _speed) {
	if (_angle > 360) {
		int _a = _angle / 360;
		_angle = _angle - _a * 360;
	}
	if (_angle >= 0 && _angle <= 90) {
		//
		if (_angle <= 45) {
			setDirection(!DIRECTION_OMNI, DIRECTION_MECANUM_LEFT, DIRECTION_MECANUM_RIGHT);
			int _temp = (int)(float(_speed) - float(_angle)*float(_speed)/45.0);
			settingTimer1(_speed,_speed);
			settingTimer4(_temp);
		}
		else {
			setDirection(DIRECTION_OMNI, DIRECTION_MECANUM_LEFT, DIRECTION_MECANUM_RIGHT);
			int _temp = (float(_angle) - 45.0)*float(_speed) / 45.0;
			settingTimer1(_speed, _speed);
			settingTimer4(_temp);
		}
	}
	else if (_angle > 90 && _angle <= 180) {
		//
		if (_angle <= 135) {
			setDirection(DIRECTION_OMNI, DIRECTION_MECANUM_LEFT, DIRECTION_MECANUM_RIGHT);
			int _temp = (int)(float(_speed) - (float(_angle)- 90.0)*float(_speed) / 45.0);
			settingTimer1(_temp, _temp);
			settingTimer4(_speed);
		}
		else {
			setDirection(DIRECTION_OMNI, !DIRECTION_MECANUM_LEFT, !DIRECTION_MECANUM_RIGHT);
			int _temp = (float(_angle) - 135.0)*float(_speed) / 45.0;
			settingTimer1(_temp, _temp);
			settingTimer4(_speed);
		}
	}
	else if (_angle > 180 && _angle <= 270) {
		//
		if (_angle <= 225) {
			setDirection(DIRECTION_OMNI, !DIRECTION_MECANUM_LEFT, !DIRECTION_MECANUM_RIGHT);
			int _temp = (int)(float(_speed) - (float(_angle) - 180.0)*float(_speed) / 45.0);
			settingTimer1(_speed, _speed);
			settingTimer4(_temp);
		}
		else {
			setDirection(!DIRECTION_OMNI, !DIRECTION_MECANUM_LEFT, !DIRECTION_MECANUM_RIGHT);
			int _temp = (float(_angle) - 225.0)*float(_speed) / 45.0;
			settingTimer1(_speed, _speed);
			settingTimer4(_temp);
		}
	}
	else if (_angle > 270 && _angle <= 360) {
		if (_angle <= 315) {
			setDirection(!DIRECTION_OMNI, !DIRECTION_MECANUM_LEFT, !DIRECTION_MECANUM_RIGHT);
			int _temp = (int)(float(_speed) - (float(_angle) - 270)*float(_speed) / 45.0);
			settingTimer1(_temp, _temp);
			settingTimer4(_speed);
		}
		else {
			setDirection(!DIRECTION_OMNI, DIRECTION_MECANUM_LEFT, DIRECTION_MECANUM_RIGHT);
			int _temp = (float(_angle) - 315.0)*float(_speed) / 45.0;
			settingTimer1(_temp, _temp);
			settingTimer4(_speed);
		}
	}
	else {
		// 
		Serial.println("ERROR ANGLE");
		return;
	}
}

float runAndCalibEncoderX(int _delta, bool runBack = 0) { //default chay ve phia truoc
	if (runBack == 0) {
		float _angle = 90.0 + float(_delta)*45.0 / NUMBER_CONFIG;
		return _angle * PI / 180.0;
	}
	else {
		int _angle = 270.0 - float(_delta)*45.0 / NUMBER_CONFIG;
		return _angle * PI / 180.0;
	}
}

void runAndCalibEncoderY(int _speed, int _delta, bool runLeft = 0) { //default chay ve phia truoc
	if (runLeft == 0) {
		int _angle = 360 - float(_delta)*45.0 / 600.0;
		runAngle(_angle, _speed);
	}
	else {
		int _angle = 180 + float(_delta)*45.0 / 600.0;
		runAngle(_angle, _speed);
	}
}

void runAngleUpdate(float _speed, float _angle, float _rotateAngle = 0) {
	if (_rotateAngle == 0) {
		Serial.println("Not Rotate");
		int _vOx, _vLx, _vRx; // =>  v generate from VX vector
		int _vO, _vL, _vR; // =>  v generate from Vy vector
		float _vX = _speed * cos(_angle);
		float _vY = _speed * sin(_angle);
		/*Serial.print("_vX : ");
		Serial.println(_vX);
		Serial.print("_vY : ");
		Serial.println(_vY);*/
		//
		_vOx = -_vX;
		_vLx = _vX;
		_vRx = _vX;
		_vO = _vOx + _vY;
		_vL = _vLx + _vY;
		_vR = _vRx + _vY;
		// run

		Serial.print("vO : ");
		Serial.println(_vO);
		Serial.print("vL : ");
		Serial.println(_vL);
		Serial.print("vR : ");
		Serial.println(_vR);
		runTripMeca(_vO, _vL, _vR);
		//
	}
	else {
		Serial.println("Rotate");
		int _vOx, _vLx, _vRx; // =>  v generate from VX vector
		int _vOr, _vLr, _vRr; // => v generate from rotate vector 
		int _vO, _vL, _vR; // =>  v generate from Vy vector
		float _vX = _speed * cos(_angle);
		float _vY = _speed * sin(_angle);
		/*Serial.print("_vX : ");
		Serial.println(_vX);
		Serial.print("_vY : ");
		Serial.println(_vY);*/
		//
		_vOx = -_vX;
		_vLx = _vX;
		_vRx = _vX;
		_vOr = _rotateAngle / 2.0;
		_vRr = _rotateAngle / 2.0;
		_vLr = -_rotateAngle / 2.0;

		_vO = _vOx + _vY + _vOr;
		_vL = _vLx + _vY + _vLr;
		_vR = _vRx + _vY + _vRr;
		// run
		Serial.print("vO : ");
		Serial.println(_vO);
		Serial.print("vL : ");
		Serial.println(_vL);
		Serial.print("vR : ");
		Serial.println(_vR);
		runTripMeca(_vO, _vL, _vR);
	}
}

void runTripMeca(int _vO, int _vL, int _vR) {
	/*_vO < 0 ? digitalWrite(PIN_DIR_OMNI, !DIRECTION_OMNI) : digitalWrite(PIN_DIR_OMNI, DIRECTION_OMNI);
	_vL < 0 ? digitalWrite(PIN_DIR_MECANUM_LEFT, !DIRECTION_MECANUM_LEFT) : digitalWrite(PIN_DIR_MECANUM_LEFT, DIRECTION_MECANUM_LEFT);
	_vR < 0 ? digitalWrite(PIN_DIR_MECANUM_RIGHT, !PIN_DIR_MECANUM_RIGHT) : digitalWrite(PIN_DIR_MECANUM_RIGHT, PIN_DIR_MECANUM_RIGHT);*/
	if (_vO < 0) {
		digitalWrite(PIN_DIR_OMNI, !DIRECTION_OMNI);
	}
	else {
		digitalWrite(PIN_DIR_OMNI, DIRECTION_OMNI);
	}
	if (_vL < 0) {
		digitalWrite(PIN_DIR_MECANUM_LEFT, !DIRECTION_MECANUM_LEFT);
	}
	else {
		digitalWrite(PIN_DIR_MECANUM_LEFT, DIRECTION_MECANUM_LEFT);
	}
	if (_vR < 0) {
		digitalWrite(PIN_DIR_MECANUM_RIGHT, !DIRECTION_MECANUM_RIGHT);
	}
	else {
		digitalWrite(PIN_DIR_MECANUM_RIGHT, DIRECTION_MECANUM_RIGHT);
	}
	settingTimer1(253 - abs(_vL), 253 - abs(_vR));
	settingTimer4(253 - abs(_vO));
	runTimer1();
	runTimer4();
}

#endif // !_PWM_TIMER_H_

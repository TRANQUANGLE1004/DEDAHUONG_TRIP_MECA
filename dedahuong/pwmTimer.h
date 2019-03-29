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
}
void initTimer5() {
	//Serial.println("Init timer 5");
	TCCR5B = 0;
	TCCR5A = 0;
	TIMSK5 = 0;
	TIMSK5 |= (1 << TOIE5);
	TCCR5B |= (1 << CS52);
	TCNT3 = getBottomTimerNomalMode(50);
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
void timerFuncIncreFre(int _time, int _firstValue, int _endValue, int _smooth, int _fre = 8000, byte _mode = 0) {
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
		settingTimer1((int)((float)_firstValue + step * (float)count), (int)((float)_firstValue + step * (float)count));
		settingTimer4((int)((float)_firstValue + step * (float)count));
		TCNT3 = time;
		//TCCR3B |= (1 << CS32);
		count++;
		//Serial.println("hihi");
	}
}

// mode 0: -> -> ->
//mode 1: dependent
void timerFuncDecreFre(int _time, int _firstValue, int _endValue, int _smooth, int _fre = 8000, byte _mode = 0) {
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
				mode &= 0;
				task++;
				return;
			}
			else {
				count &= 0;
				stopState();
				//stopTimer3();
				return;
			}
		}
		settingTimer1((int)((float)_firstValue + step * (float)count), (int)((float)_firstValue + step * (float)count));
		settingTimer4((int)((float)_firstValue + step * (float)count));
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

#endif // !_PWM_TIMER_H_

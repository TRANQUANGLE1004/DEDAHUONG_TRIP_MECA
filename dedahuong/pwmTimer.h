#pragma once
#ifndef _PWM_TIMER_H_
#define _PWM_TIMER_H_
#include<avr/interrupt.h>
#include<Arduino.h>
#define fd			(1.0f/16000000.0f)	// frequency default atmega 2560
#define fre			16000
#define FIFTY_MS	62410	
/*
use pin ???
*/

//
volatile int count = 0;
volatile int mode = 0;
volatile int task = 0;
//
unsigned short timer3;
void genPWMTimer1(int frequency);
void genPWMTimer4(int frequency);
unsigned short settingPulseUseTimer3(int _time);
void settingPulseUseTimer5(int _time);
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
		if (_num>0) {
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

void rotateClockWise(int _num) {
	digitalWrite(PIN_DIR_MECANUM_LEFT, DIRECTION_MECANUM_LEFT);
	digitalWrite(PIN_DIR_MECANUM_RIGHT, !DIRECTION_MECANUM_RIGHT);
	digitalWrite(PIN_DIR_OMNI, !DIRECTION_OMNI);
	genPWMTimer4((int)((float)_num*800.0f*1.55f));
	genPWMTimer1(_num*1000);
}

void rotateInvertClockWise(int _num) {
	digitalWrite(PIN_DIR_MECANUM_LEFT, !DIRECTION_MECANUM_LEFT);
	digitalWrite(PIN_DIR_MECANUM_RIGHT, DIRECTION_MECANUM_RIGHT);
	digitalWrite(PIN_DIR_OMNI, DIRECTION_OMNI);
	genPWMTimer4((int)((float)_num*800.0f*1.55f));
	genPWMTimer1(_num * 1000);
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

void settingTimer1(int _icrVal, int _val = 0) { // use Fast PWM
	if (_val == 0) { // default duty cycle : 50%
		OCR1A = _icrVal/2;
		OCR1C = _icrVal/2;
		ICR1 = _icrVal;
	}
	else {
		OCR1A = (int)((float)_val*(float)_icrVal/255.0f);
		OCR1C = (int)((float)_val*(float)_icrVal / 255.0f);
		ICR1 = _icrVal;
	}
	
	// Notice !!!
}
void settingTimer4(int _icrVal, int _val = 0) { // use Fast PWM
	if (_val == 0) { // default duty cycle : 50%
		OCR4A = _icrVal/2;
		ICR4 = _icrVal;
	}
	else {
		OCR4A = (int)((float)_val*(float)_icrVal / 255.0f);
		ICR4 = _icrVal;
	}

}
void runTimer1() {	TCCR1B |= (1 << CS10); }
void runTimer4() {	TCCR4B |= (1 << CS40); }
void stopTimer1(){	TCCR1B &= 0xF8; }
void stopTimer4(){	TCCR4B &= 0xF8; }
void stopTimer3(){	TCCR3B &= 0xF8; }
void stopState() {
	settingTimer1(8000, 253);
	settingTimer4(8000, 253);
}

// 5 distance //
// _firstValueChanelA is ICR value
void timerFuncIncreFre(int _time, int _firstValue, int _endValue, int _smooth, int _fre = 8000) {
	unsigned short time = getBottomTimerNomalMode(_time);
	int step = (int)((float)(_endValue - _firstValue) / (float)_smooth);
	if (count == 0) {
		//
		settingTimer1(_fre,_firstValue);
		settingTimer4(_fre,_firstValue);
		/*OCR1A = _icrVal;
		OCR1C = _icrVal;
		OCR4A = _icrVal;*/
		runTimer1();
		runTimer4();
		TCNT3 = time;
		TCCR3B |= (1 << CS32);
		//
		count++;
		Serial.println("hihi0");
	}
	else {
		if (_smooth == count) { 
			//stopState();
			TCNT3 = time;
			count &= 0;
			mode++;
			return; 
		}
		settingTimer1(_fre, _firstValue + step * count);
		settingTimer4(_fre, _firstValue + step * count);
		TCNT3 = time;
		//TCCR3B |= (1 << CS32);
		count++;
		Serial.println("hihi");
	}
}

void timerFuncDecreFre(int _time, int _firstValue, int _endValue, int _smooth, int _fre = 8000) {
	unsigned short time = getBottomTimerNomalMode(_time);
	int step = (int)((float)(_endValue - _firstValue) / (float)_smooth);
	if (count == 0) {
		//
		settingTimer1(_fre, _firstValue);
		settingTimer4(_fre, _firstValue);
		/*OCR1A = _icrVal;
		OCR1C = _icrVal;
		OCR4A = _icrVal;*/
		runTimer1();
		runTimer4();
		TCNT3 = time;
		TCCR3B |= (1 << CS32);
		//
		count++;
		Serial.println("hihi0");
	}
	else {
		if (_smooth == count) {
			TCNT3 = time;
			count &= 0;
			mode &= 0;
			task++;
			return;
		}
		settingTimer1(_fre, _firstValue + step * count);
		settingTimer4(_fre, _firstValue + step * count);
		TCNT3 = time;
		//TCCR3B |= (1 << CS32);
		count++;
		Serial.println("hihi");
	}
}


void timerFuncIncreSpeed(int _time, int* _arrA, int* _arrB) {
	unsigned short time = getBottomTimerNomalMode(_time);
	if (count == 0) {
		//
		settingTimer1(_arrA[count]);
		settingTimer4(_arrB[count]);
		runTimer1();
		runTimer4();
		TCNT3 = _time;
		TCCR3B |= (1 << CS32);
		//
		count++;
		//Serial.println("hihi");
	}
	else if (count == 1) {
		//Serial.println(micros() - _time);
		settingTimer1(_arrA[count]);
		settingTimer4(_arrB[count]);
		count++;
		TCNT3 = _time;
		//8000
	}
	else if (count == 2) {
		//Serial.println(micros() - _time);
		//Serial.println("hihi");
		settingTimer1(_arrA[count]);
		settingTimer4(_arrB[count]);
		count++;
		TCNT3 = _time;
		//4000
	}
	else if (count == 3) {
		//TCNT3 = 59285;
		//Serial.println(micros() - _time);
		//Serial.println("hihi");
		settingTimer1(_arrA[count]);
		settingTimer4(_arrB[count]);
		count++;
		TCNT3 = _time;
		//2000
	}
	else if (count == 4) {
		//TCNT3 = 59285;
		//Serial.println(micros() - _time);
		//Serial.println("hihi");
		settingTimer1(_arrA[count]);
		settingTimer4(_arrB[count]);
		count++;
		TCNT3 = _time;
		//mode += 1;
		//count &= 0;
		//1000
	}
	else {
		//stopTimer3();
		stopTimer1();
		stopTimer4();
	}
}


#endif // !_PWM_TIMER_H_

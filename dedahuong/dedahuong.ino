/*
 Name:		dedahuong.ino
 Created:	7/11/2018 2:05:53 PM
 Author:	quang
*/
#include"Configuration.h"
#include"control.h"
#include "pwmTimer.h"
//#include"hc05.h"
//
char recv;
int _time = getBottomTimerNomalMode(100);
int valueIcrA[5] = { 16000,8000,5333,4000,3200 };
int valueIcrB[5] = { 20000,10000,6666,5000,4000 };
int valueInvertA[5] = { 3200,4000,5333,8000,16000 };
int valueInvertB[5] = { 4000,5000,6666,10000,20000 };
//unsigned char speed = 1000;
// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	initPwmTimer1();
	initPwmTimer4();
	//initTimer3();
	Serial3.begin(9600);//HC05
	pinMode(PIN_PULSE_OMNI, OUTPUT);
	pinMode(PIN_DIR_OMNI, OUTPUT);
	//
	pinMode(PIN_PULSE_MECANUM_LEFT, OUTPUT);
	pinMode(PIN_DIR_MECANUM_LEFT, OUTPUT);
	//
	pinMode(PIN_PULSE_MECANUM_RIGHT, OUTPUT);
	pinMode(PIN_DIR_MECANUM_RIGHT, OUTPUT);
	//freeRun();
	sei();
	//
	//fucking(1, 100);
	//hamkhoidong();
	
	//TCCR1B |= (1 << CS12);
	initTimer3();
}

void loop() {

}


/*void loop() {
	if (Serial3.available() > 0) {
		recv = Serial3.read();
		Serial.print("Receive character: ");
		Serial.println(recv);
	}
	switch (recv)
	{
	case 'F':
		//Forward
		//runStraight(5000,4000);
		//hamkhoidong();
		Serial.println("run run straight");
		break;
	case 'B':
		runStraight(-5000, -4000);
		Serial.println("run run back");
		break;
	case 'L':
		// turn left
		runStraight(-5000, 4000);
		//runLeft(speed);
		break;
	case'R':
		runStraight(5000, -4000);
		//turn right
		//runRight(speed);
		break;

	case'J':
		rotateClockWise(5);
		break;
	case'G':
		rotateInvertClockWise(5);
		break;
	case'S':
		//free
		stopState();
		break;
	default:
		break;
	}
}
*/
ISR(TIMER3_OVF_vect) {
	if (mode == 0) { // tang toc
		timerFuncIncreSpeed(_time, valueIcrA, valueIcrB);
	}
	else if (mode == 1) { // chay on dinh
		if (count == 0) {
			TCNT3 = getBottomTimerNomalMode(1000);
			count++;
			//Serial.println("hihi");
		}
		else if (count == 1) {
			TCNT3 = getBottomTimerNomalMode(100);
			count++;
			mode += 1;
			count &= 0;
		}
		else {
			stopTimer1();
			stopTimer4();
		}
	}
	else if (mode == 2) {
		timerFuncIncreSpeed(_time, valueInvertA, valueInvertB);
	}
	else {
		stopTimer1();
		stopTimer4();
	}	
}
ISR(TIMER5_OVF_vect) {
	
}
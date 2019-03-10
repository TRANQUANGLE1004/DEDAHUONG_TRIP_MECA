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
//unsigned char speed = 1000;
// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	initPwmTimer1();
	initPwmTimer4();
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
	cli();
	pulseGenMecanum(1000,1000);
}

void loop() {

}

// the loop function runs over and over again until power down or reset
//void loop() {
//	if (Serial3.available() > 0) {
//		recv = Serial3.read();
//		Serial.print("Receive character: ");
//		Serial.println(recv);
//	}
//	switch (recv)
//	{
//	case 'F':
//		//Forward
//		runStraight(1000,800);
//		Serial.println("run run straight");
//		break;
//	case 'B':
//		runStraight(-10000, -8000);
//		Serial.println("run run back");
//		break;
//	case 'L':
//		// turn left
//		runStraight(-10000, 8000);
//		//runLeft(speed);
//		break;
//	case'R':
//		runStraight(10000, -8000);
//		//turn right
//		//runRight(speed);
//		break;
//	case'S':
//		//free
//		stopState();
//		break;
//	default:
//		break;
//	}
//}

ISR(TIMER3_OVF_vect) {
	//TCCR3B &= 0xF8; //stop timer 3
	TCCR1B &= 0xF8; //stop timer 1
	//OCR1A = pwmValA;
	TIMSK3 &= 0x00;
	TCCR3B &= 0xF8;
	Serial.println("ISR OVF vect 3");
}
ISR(TIMER5_OVF_vect) {
	TCCR5B &= 0xF8; //stop timer
	TCCR4B &= 0xF8;
	//OCR4A = pwmValB;
	//TCNT5 = timeValB;
	//Serial.println("hihi");
}
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
	sei();
	//
	//fucking(1, 100);
	hamkhoidong();
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
//		runStraight(5000,4000);
//		//hamkhoidong();
//		Serial.println("run run straight");
//		break;
//	case 'B':
//		runStraight(-5000, -4000);
//		Serial.println("run run back");
//		break;
//	case 'L':
//		// turn left
//		runStraight(-5000, 4000);
//		//runLeft(speed);
//		break;
//	case'R':
//		runStraight(5000, -4000);
//		//turn right
//		//runRight(speed);
//		break;
//
//	case'J':
//		rotateClockWise(5);
//		break;
//	case'G':
//		rotateInvertClockWise(5);
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
	if (count == 0) {
		TCCR3B |= (1 << CS32);
		TCNT3 = timer3;
		count++;
		Serial.println("count == 0");
	}
	else if (count==1) {
		/*TCCR3B &= 0xF8;
		TCCR1B &= 0xF8;
		count = 0;
		Serial.println("count != 0");*/
		TCNT3 = timer3;
		OCR1A = fre / 4;
		OCR1C = fre / 4;
		ICR1 = fre/2;
		count++;
		//Serial.println("count == 1");
	}
	else if (count == 2) {
		//TCCR3B &= 0xF8;
		TCNT3 = timer3;
		OCR1A = fre / 8;
		OCR1C = fre / 8;
		ICR1 = fre / 4;
		count++;
		//TCCR3B |= (1<<CS32);
	}
	else if (count == 3) {
		//TCCR3B &= 0xF8;
		TCNT3 = timer3;
		OCR1A = fre / 16;
		OCR1C = fre / 16;
		ICR1 = fre / 8;
		count++;

		//TCCR3B |= (1 << CS32);
	}
	else if (count == 4) {
		//TCCR3B &= 0xF8;
		TCNT3 = timer3;
		OCR1A = fre / 32;
		OCR1C = fre / 32;
		ICR1 = fre / 16;
		count++;

		//TCCR3B |= (1 << CS32);
	}
	else if (count == 5) {
		//TCCR3B &= 0xF8;
		TCNT3 = timer3;
		OCR1A = fre / 64;
		OCR1C = fre / 64;
		ICR1 = fre / 32;
		count++;
		//TCCR3B |= (1 << CS32);
		TCCR1B &= 0xF8;
	}
}
ISR(TIMER5_OVF_vect) {
	TCCR5B &= 0xF8; //stop timer
	TCCR4B &= 0xF8;
	//OCR4A = pwmValB;
	//TCNT5 = timeValB;
	Serial.println("hihi 5");
}
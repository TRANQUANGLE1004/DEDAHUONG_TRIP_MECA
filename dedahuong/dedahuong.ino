/*
 Name:		dedahuong.ino
 Created:	7/11/2018 2:05:53 PM
 Author:	quang
*/
#include"Configuration.h"
#include"control.h"
#include "pwmTimer.h"
#include"pwmLib.h"
//#include"hc05.h"
//
char recv;
int valueIcrA[5] = { 16000,8000,4000,1800,1600 };
int valueIcrB[5] = { 20000,10000,5000,2250,2000 };
int valueInvertA[5] = { 1600,1800,4000,8000,16000 };
int valueInvertB[5] = { 2000,2250,5000,10000,20000 };
int speed = 8000;
unsigned long time = 0;
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
	//
	//fucking(1, 100);
	//hamkhoidong();
	//TCCR1B |= (1 << CS12);
	//runStraight(speed, speed);
	initTimer3();
}

void loop() {

}


//void loop() {
//	if (Serial3.available() > 0) {
//		recv = Serial3.read();
//		Serial.print("Receive character: ");
//		Serial.println(recv);
//	}
//	switch (recv)
//	{
//	case 'F':
//		//runSpeed(speed, speed);
//		runStraight(speed, (int)((float)speed*0.8f));
//		//hamkhoidong();
//		//setDirection(DIRECTION_OMNI, DIRECTION_MECANUM_LEFT, DIRECTION_MECANUM_RIGHT); // chay thang
//		//initTimer3();
//		break;
//	case 'B':
//		runStraight(-speed, -(int)((float)speed*0.8f));
//		break;
//	case 'L':
//		//runSpeed(-speed, speed);
//		// turn left
//		runStraight(-speed, (int)((float)speed*0.8f));
//		//setDirection(DIRECTION_OMNI, !DIRECTION_MECANUM_LEFT, !DIRECTION_MECANUM_RIGHT); // chay thang
//		//initTimer3();
//		//runLeft(speed);
//		break;
//	case'R':
//		//runSpeed(speed, -speed);
//		runStraight(speed, -(int)((float)speed*0.8f));
//		//setDirection(!DIRECTION_OMNI, DIRECTION_MECANUM_LEFT, DIRECTION_MECANUM_RIGHT); // chay thang
//		//initTimer3();
//		//turn right
//		//runRight(speed);
//		break;
//	case'W':
//		speed = 15000;
//		break;
//	case'w':
//		speed = 10000;
//		break;
//	case'J':
//		rotateClockWise(5);
//		break;
//	case'G':
//		rotateInvertClockWise(5);
//		break;
//	case'S':
//		stopState();
//		//state = 0;
//		break;
//	default:
//		break;
//	}
//}

ISR(TIMER3_OVF_vect) {
	switch (task)
	{
	case 0:
		switch (mode)
		{
		case 0:// mode tang toc
			timerFuncIncreFre(10, 252, 0, 100);
			//Serial.println("hihi");
			break;
		case 1:// mode chay on dinh toc do trong bao nhieu ms
			if (count == 0) {
				TCNT3 = getBottomTimerNomalMode(1000);
				count++;
			}
			else if (count == 1) {
				TCNT3 = getBottomTimerNomalMode(1000);
				count &= 0;
				mode++;
				//count++;
			}
			break;
		case 2: // ham giam toc
			timerFuncDecreFre(10, 0, 252, 100);
			break;
		default:
			break;
		}
		break;
	case 1:
		break;
	default:
		break;
	}
		
	
	//if (mode == 0) { // tang toc
	////Serial.println("hello");
	//}
	//else if (mode == 1) { // chay on dinh
	//	if (count == 0) {
	//		TCNT3 = getBottomTimerNomalMode(1000);
	//		count++;
	//		Serial.println("hihi");
	//	}
	//	else if (count == 1) {
	//		TCNT3 = getBottomTimerNomalMode(100);
	//		//count++;
	//		mode += 1;
	//		count &= 0;
	//	}
	//	else {
	//		stopTimer1();
	//		stopTimer4();
	//	}
	//}
	//else if (mode == 2) {
	//	timerFuncIncreFre(_time, valueInvertA, valueInvertB);
	//}
	//else {
	//	stopTimer1();
	//	stopTimer4();
	//}	
}
ISR(TIMER5_OVF_vect) {
	
}
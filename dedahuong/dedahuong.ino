/*
 Name:		dedahuong.ino
 Created:	7/11/2018 2:05:53 PM
 Author:	quang
 {@Plot.MyPlotWindow.Temp.Blue temperature}
 double Kp = 3.15, Ki = 7.56, Kd = 0.01; stop 
*/
#include"Configuration.h"
#include <Wire.h>
#include "BNO055_support.h"
#include "pwmTimer.h"
#include"pwmLib.h"
#include "PID_v1.h"

//#include"hc05.h"
//
char recv = 'S';
char tempRecv;
int speed = 150;
int state = 0;
int valueSpeed;
unsigned long timeMillis;
//
struct bno055_t myBNO;
signed short eulerYaw;
int caliValue;
int angle;
int cnt = 0;
// the setup function runs once when you press reset or power the board
double Kp = 3.15, Ki = 7.56, Kd = 0.01;
double Setpoint, Input, Output;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, REVERSE);
//
void setup() {
	//
	Wire.begin();
	Serial.begin(9600);
	Serial3.begin(9600);//HC05
	Serial.println("Init Serial Monitor !!!");
	//
	initPwmTimer1();
	initPwmTimer4();
	//
	pinMode(PIN_PULSE_OMNI, OUTPUT);
	pinMode(PIN_DIR_OMNI, OUTPUT);
	//
	pinMode(PIN_PULSE_MECANUM_LEFT, OUTPUT);
	pinMode(PIN_DIR_MECANUM_LEFT, OUTPUT);
	//
	pinMode(PIN_PULSE_MECANUM_RIGHT, OUTPUT);
	pinMode(PIN_DIR_MECANUM_RIGHT, OUTPUT);
	//
	pinMode(PIN_KHI23, OUTPUT);
	pinMode(PIN_KHI24, OUTPUT);
	pinMode(PIN_KHI25, OUTPUT);
	pinMode(PIN_KHI26, OUTPUT);
	//
	digitalWrite(PIN_KHI23, LOW);
	digitalWrite(PIN_KHI24, LOW);
	digitalWrite(PIN_KHI25, LOW);
	digitalWrite(PIN_KHI26, LOW);
	// init BNO055
	BNO_Init(&myBNO);
	bno055_set_operation_mode(OPERATION_MODE_IMUPLUS);
	//calibrate
	for (size_t i = 0; i < 30; i++)
	{
		bno055_read_euler_h(&eulerYaw);
		caliValue = map(eulerYaw, 0, 5759, 0, 359);
	}
	////
	Serial.print("calibrate value: ");
	Serial.println(caliValue);
	//Setting PID Lib
	Setpoint = 0.0; 
	myPID.SetMode(AUTOMATIC);
	myPID.SetOutputLimits(-100, 100);
	myPID.SetSampleTime(100);
	//myPID.SetSampleTime(100);
	setDirection(DIRECTION_OMNI, DIRECTION_MECANUM_LEFT, DIRECTION_MECANUM_RIGHT);

	//initTimer5();
	//stopState();
}



void loop() {
	//runAndCalib(speed, 2*readAngle());
	if (Serial3.available() > 0) {
		recv = Serial3.read();
		Serial.println(recv);
		switch (recv)
		{
		case 'F':
			//Serial.println("F0");
			setDirection(DIRECTION_OMNI, DIRECTION_MECANUM_LEFT, DIRECTION_MECANUM_RIGHT);
			//tang toc
			task = 9998;
			initTimer3();
			//
			break;
		case 'B':
			setDirection(!DIRECTION_OMNI, !DIRECTION_MECANUM_LEFT, !DIRECTION_MECANUM_RIGHT);
			task = 9998;
			initTimer3();
			break;
		case 'L':
			setDirection(DIRECTION_OMNI, !DIRECTION_MECANUM_LEFT, !DIRECTION_MECANUM_RIGHT);
			task = 9998;
			initTimer3();
			break;
		case'R':
			setDirection(!DIRECTION_OMNI, DIRECTION_MECANUM_LEFT, DIRECTION_MECANUM_RIGHT);
			task = 9998;
			initTimer3();
			break;
		case'J':
			rotateClockWise(speed);
			break;
		case'G':
			rotateInvertClockWise(speed);
			break;
		case'S':
			if (tempRecv == 'F'|| tempRecv == 'B'||tempRecv == 'L'|| tempRecv == 'R') {
				//giam toc
				Serial.println("giam toc");
				task = 10000;
				initTimer3();
				timeMillis = millis();
				//
				count &= 0;
			}
			else {
				if (millis() - timeMillis >= DECELERATION_TIME) {
					count &= 0;
					stopState();
					stopTimer3();
				}
			}
			break;
		default:
			stopState();
			break;
		}
		tempRecv = recv;
	}
	//if (recv == 'S') {
	//	Input = readAngle();
	//	if (Input == 0) {
	//		stopAndCalib(0);
	//	}
	//	else {
	//		myPID.SetTunings(3.15, 7.56, 0.01);
	//		myPID.Compute();
	//		stopAndCalib(Output);
	//	}
	//	Serial.print(Input);
	//	Serial.print("------> ");
	//	Serial.println(Output);
	//}
	//else if (recv == 'F') {
	//	myPID.SetTunings(0, 0, 0);
	//	Input = readAngle();
	//	myPID.Compute();
	//	runAndCalib(speed, Output);
	//}
	////test
	//else if (recv == 'B') {
	//	myPID.SetTunings(0, 0, 0);
	//	Input = readAngle();
	//	myPID.Compute();
	//	backAndCalib(speed, Output);
	//}
	//else if (recv == 'R') {
	//	myPID.SetTunings(2, 4, 0.1); // change PID
	//	Input = readAngle();
	//	myPID.Compute();
	//	runAndCalib(speed, Output);
	//}
	//else if (recv == 'L') {
	//	myPID.SetTunings(2, 4, 0.1); // change PID
	//	Input = readAngle();
	//	myPID.Compute();
	//	backAndCalib(speed, Output);
	//}
	//else if (recv == 'G') {
	//	// update Setpoint
	//}
}


//void loop() {
//	if (Serial3.available() > 0) {
//		recv = Serial3.read();
//		Serial.println(recv);
//	}
//	switch (recv)
//	{
//	case 'F':
//		setDirection(DIRECTION_OMNI, DIRECTION_MECANUM_LEFT, DIRECTION_MECANUM_RIGHT);
//		//
//		settingTimer1(speed, speed);
//		settingTimer4(speed);
//		runTimer1();
//		runTimer4();
//		//
//		break;
//	case 'B':
//		setDirection(!DIRECTION_OMNI, !DIRECTION_MECANUM_LEFT, !DIRECTION_MECANUM_RIGHT);
//
//		break;
//	case 'L':
//		setDirection(DIRECTION_OMNI, !DIRECTION_MECANUM_LEFT, !DIRECTION_MECANUM_RIGHT);
//
//		break;
//	case'R':
//		setDirection(!DIRECTION_OMNI, DIRECTION_MECANUM_LEFT, DIRECTION_MECANUM_RIGHT);
//
//		break;
//	case'J':
//		rotateClockWise(5);
//		break;
//	case'G':
//		rotateInvertClockWise(5);
//		break;
//	case'S':
//		stopState();
//		break;
//	default:
//		stopState();
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
			timerFuncIncreFre(10, 252, speed, 100,valueSpeed);
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
		switch (mode)
		{
		case 0:
			timerFuncIncreFre(10, 252, speed, 100,valueSpeed);
			break;
		default:
			break;
		}
		break;
	case 9998: // tang toc va giu on dinh toc do
		//Serial.println("ham tang toc");
		timerFuncIncreFre(10, 220, speed, SAMPLE_NUMBER, valueSpeed, 2);
		break;
	case 9999: // ham tang toc doc lap
		timerFuncIncreFre(10, 220, speed, SAMPLE_NUMBER, valueSpeed, 1);
		break;
	case 10000:
		//Serial.println("chay ham giam toc");
		timerFuncDecreFre(10, valueSpeed, 220, SAMPLE_NUMBER, 8000, 1);
		break; // ham giam toc doc lap
	default:
		break;
	}
}
ISR(TIMER5_OVF_vect) { // read HC05
	
}

int readAngle() {
	bno055_read_euler_h(&eulerYaw);
	int _angle = map(eulerYaw, 0, 5759, 0, 359);
	int _val = calibrateFunc(_angle, caliValue);
	if (_val >180) {
		if (_val - 360 >= Setpoint - 1) {
			return 0;
		}
		return _val - 360;
	}
	if (_val <= Setpoint + 1) {
		return 0;
	}
	return _val;
}

int calibrateFunc(int _angle, int _valCalib) {
	if (_valCalib <= 180) {
		if (_angle - _valCalib >= 0) { return _angle - _valCalib; }
		else {
			return 360 + _angle - _valCalib;
		}
	}
	else {
		int _delta = 359 - _valCalib;
		if (_angle + _delta >= 360) {
			return (_angle + _delta) - 360;
		}
		return _angle + _delta;
	}
}

void getAndExeSensor(int _time) {
	unsigned short time = getBottomTimerNomalMode(_time);
	int a = readAngle();
	Serial.println("a");
	TCNT5 = time;
	TCCR5B |= (1 << CS52);
	cnt++;
}

void feedBackFunc(int _threshold, int _value) {
	int error = _threshold - _value;
}

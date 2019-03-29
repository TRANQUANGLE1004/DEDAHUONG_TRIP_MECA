/*
 Name:		dedahuong.ino
 Created:	7/11/2018 2:05:53 PM
 Author:	quang
 {@Plot.MyPlotWindow.Temp.Blue temperature}
*/
#include"Configuration.h"
#include <Wire.h>
#include "BNO055_support.h"
#include "pwmTimer.h"
#include"pwmLib.h"
#include "PID_v1.h"

//#include"hc05.h"
//
char recv= 'S';
int speed = 150;
int state = 0;
//
struct bno055_t myBNO;
signed short eulerYaw;
int caliValue;
int angle;
int cnt = 0;
// the setup function runs once when you press reset or power the board
double Kp = 1.5, Ki = 1, Kd = 1;
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
	myPID.SetSampleTime(10);
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
			setDirection(DIRECTION_OMNI, DIRECTION_MECANUM_LEFT, DIRECTION_MECANUM_RIGHT);
			//
			settingTimer1(speed, speed);
			settingTimer4(speed);
			runTimer1();
			runTimer4();
			//
			break;
		case 'B':
			setDirection(!DIRECTION_OMNI, !DIRECTION_MECANUM_LEFT, !DIRECTION_MECANUM_RIGHT);

			break;
		case 'L':
			setDirection(DIRECTION_OMNI, !DIRECTION_MECANUM_LEFT, !DIRECTION_MECANUM_RIGHT);

			break;
		case'R':
			setDirection(!DIRECTION_OMNI, DIRECTION_MECANUM_LEFT, DIRECTION_MECANUM_RIGHT);

			break;
		case'J':
			rotateClockWise(5);
			break;
		case'G':
			rotateInvertClockWise(5);
			break;
		case'S':
			stopState();
			break;
		default:
			stopState();
			break;
		}
	}
	if (recv == 'S') {
		Input = readAngle();
		Serial.print(Input);
		myPID.Compute();
		stopAndCalib(Output);
		Serial.print("--- ");
		Serial.println(Output);
	}
	else if (recv == 'F') {
		Input = readAngle();
		myPID.Compute();
		runAndCalib(speed, Output);
	}
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
		switch (mode)
		{
		case 0:
			timerFuncIncreFre(10, 252, 0, 100);
			break;
		default:
			break;
		}
		break;
	case 9998: // tang toc va giu on dinh toc do
		//Serial.println("ham tang toc");
		timerFuncIncreFre(10, 220, speed, 1000, 8000, 2);
		break;
	case 9999: // ham tang toc doc lap
		timerFuncIncreFre(5, 220, speed, 200, 8000, 1);
		break;
	case 10000:
		//Serial.println("chay ham giam toc");
		timerFuncDecreFre(5, speed, 220, 100, 8000, 1);
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
		return _val - 360;
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
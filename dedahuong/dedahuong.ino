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
signed short valX;
signed short valY;
signed long sumValX;// sum of encoder X axis
signed long sumValY;// sum of encoder Y axis
//
char recv = 'S';
char tempRecv;
char bienphu;
int speed = 100;
int state = 0;
int valueSpeedUp;
int valueSpeedDown;
int valueSpeed = 0;
unsigned long timeMillis;
//
struct bno055_t myBNO;
signed short eulerYaw;
int caliValue;
int angle;
int cnt = 0;
// the setup function runs once when you press reset or power the board
double Kp = 3.5, Ki = 0, Kd = 0;
double KpX = 1, KiX = 0, KdX = 0;
double Setpoint, Input, Output;
double SetpointXAxis, InputXAxis, OutputXAsix;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, REVERSE);
PID pidXAxis(&InputXAxis, &OutputXAsix, &Setpoint, KpX, KiX, KdX, REVERSE);
//
void setup() {
	//
	Wire.begin();
	Serial.begin(115200);
	Serial2.begin(115200); // encoder
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
	digitalWrite(PIN_KHI23, HIGH);
	digitalWrite(PIN_KHI24, HIGH);
	digitalWrite(PIN_KHI25, HIGH);
	digitalWrite(PIN_KHI26, HIGH);
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
	myPID.SetOutputLimits(-70, 70);
	myPID.SetSampleTime(10);
	//
	pidXAxis.SetMode(AUTOMATIC);
	pidXAxis.SetOutputLimits(-70, 70);
	pidXAxis.SetSampleTime(20);
	//myPID.SetSampleTime(100);
	setDirection(DIRECTION_OMNI, DIRECTION_MECANUM_LEFT, DIRECTION_MECANUM_RIGHT);

	//runAngleUpdate(100, 3*PI/4 , 10);
	task = 0;
	initTimer3();
	//stopState();
}

//**************************************************************
void loop() {
	switch (task)
	{
	case 0:
		// read laban
		Input = readAngle();
		myPID.Compute();
		//end read la ban
		// read encoder
		Serial2.println('g');
		while (Serial2.available() == 4) {
			readData(valX, valY);
			/*Serial.print("val x: ");
			Serial.println(valX);*/
		}
		float _x = runAndCalibEncoderX(valX);
		/*Serial.print("_x: ");
		Serial.println(_x);*/
		runAngleUpdate(253 - valueSpeed,_x,Output);
		//end
		break;
	case 1:
		
		break;
	default:
		stopState();
		break;
	}
	 //********************************************************************
	
}

//void loop() {
//	//runAndCalib(speed, 2*readAngle());
//	if (Serial3.available() > 0) {
//		recv = Serial3.read();
//		Serial.println(recv);
//		switch (recv)
//		{
//		case 'F':
//			Serial.println(" tang toc F");
//			setDirection(DIRECTION_OMNI, DIRECTION_MECANUM_LEFT, DIRECTION_MECANUM_RIGHT);
//			//tang toc
//			task = 9998;
//			initTimer3();
//			//
//			break;
//		case 'B':
//			Serial.println(" tang toc B");
//			setDirection(!DIRECTION_OMNI, !DIRECTION_MECANUM_LEFT, !DIRECTION_MECANUM_RIGHT);
//			task = 9998;
//			initTimer3();
//			break;
//		case 'L':
//			Serial.println(" tang toc L");
//			setDirection(DIRECTION_OMNI, !DIRECTION_MECANUM_LEFT, !DIRECTION_MECANUM_RIGHT);
//			task = 9998;
//			initTimer3();
//			break;
//		case'R':
//			Serial.println(" tang toc R");
//			setDirection(!DIRECTION_OMNI, DIRECTION_MECANUM_LEFT, DIRECTION_MECANUM_RIGHT);
//			task = 9998;
//			initTimer3();
//			break;
//		case'J':
//			rotateClockWise(speed);
//			break;
//		case'G':
//			rotateInvertClockWise(speed);
//			break;
//		case'S':
//			if (tempRecv == 'F'|| tempRecv == 'B'||tempRecv == 'L'|| tempRecv == 'R') {
//				//giam toc
//				Serial.println("giam toc");
//				count &= 0;
//				task = 10000;
//				initTimer3();
//				delay(15);
//				//timeMillis = millis();
//				//
//			}
//			else {
//				if (millis() - timeMillis >= DECELERATION_TIME) {
//					Serial.println("Dung lai hoan toan");
//					count &= 0;
//					stopState();
//					stopTimer3();
//				}
//			}
//			break;
//		default:
//			stopState();
//			break;
//		}
//		tempRecv = recv;
//	}
//	/*if (recv == 'S') {
//		Input = readAngle();
//		if (Input == 0) {
//			stopAndCalib(0);
//		}
//		else {
//			myPID.SetTunings(3.15, 7.56, 0.01);
//			myPID.Compute();
//			stopAndCalib(Output);
//		}
//		Serial.print(Input);
//		Serial.print("------> ");
//		Serial.println(Output);
//	}*/
//	if (tempRecv == 'F'&&count != 0) {
//		bienphu = 'F';
//		Serial.print("calib tang toc  --> Speed : ");
//		Serial.println(valueSpeedUp);
//		myPID.SetTunings(Kp_Y, Ki_Y, Kd_Y); // calib lai 
//		Input = readAngle();
//		myPID.Compute();
//		runAndCalib(valueSpeedUp, Output);
//	}
//	//test
//	else if (tempRecv == 'B'&&count != 0) {
//		bienphu = 'B';
//		Serial.println("calib giam toc");
//		myPID.SetTunings(Kp_Y, Ki_Y, Kd_Y);
//		Input = readAngle();
//		myPID.Compute();
//		backAndCalib(valueSpeedUp, Output);
//		
//	}
//	else if (tempRecv=='R'&&count!=0) {
//		bienphu == 'R';
//		myPID.SetTunings(Kp_X, Ki_X, Kd_X);//
//		Input = readAngle();
//		myPID.Compute();
//		runAndCalib(valueSpeedUp, Output);
//	}
//	else if (tempRecv=='L'&&count!=0) {
//		myPID.SetTunings(Kp_X, Ki_X, Kd_X);
//		Input = readAngle();
//		myPID.Compute();
//		backAndCalib(valueSpeedUp, Output);
//	}
//	else if (tempRecv == 'S'&&count != 0) {
//		if (bienphu == 'F') {
//			Serial.print("calib down F ");
//			Serial.println(count);
//			myPID.SetTunings(Kp_Y, Ki_Y, Kd_Y);
//			Input = readAngle();
//			myPID.Compute();
//			runAndCalib(valueSpeedDown, Output);
//		}
//		else if (bienphu == 'B') {
//			Serial.print("calib down B ");
//			Serial.println(count);
//			myPID.SetTunings(Kp_Y, Ki_Y, Kd_Y);
//			Input = readAngle();
//			myPID.Compute();
//			backAndCalib(valueSpeedDown, Output);
//		}
//		else if (bienphu == 'R') {
//			myPID.SetTunings(Kp_X, Ki_X, Kd_X);
//			Input = readAngle();
//			myPID.Compute();
//			runAndCalib(valueSpeedDown, Output);
//		}
//		else if (bienphu == 'L') {
//			myPID.SetTunings(Kp_X, Ki_X, Kd_X);
//			Input = readAngle();
//			myPID.Compute();
//			backAndCalib(valueSpeedDown, Output);
//		}
//	}
//	else if (tempRecv == 'S'&&count == 0) {
//		Serial.println("stop and calib :)) ");
//		Input = readAngle(); 
//		if (Input == 0) {
//			stopAndCalib(0);
//		}
//		else {
//			myPID.SetTunings(3, 0, 0);
//			myPID.Compute();
//			stopAndCalib(Output);
//		}
//		/*Serial.print(Input);
//		Serial.print("------> ");
//		Serial.println(Output);*/
//	}
//	
//}



ISR(TIMER3_OVF_vect) {
	switch (task)
	{
	case 0:
		switch (mode)
		{
		case 0:// mode tang toc
			timerFuncIncreFre(10, 0, speed, 50, valueSpeed);
			//Serial.println("hihi");
			break;
		case 1:// mode chay on dinh toc do trong bao nhieu ms
			if (count == 0) {
				TCNT3 = getBottomTimerNomalMode(500);
				count &= 0;
				mode++;
			}
			break;
		case 2: // ham giam toc
			myPID.SetTunings(15,30,0);
			timerFuncDecreFre(10, speed, 0, 50, valueSpeed,1);
			break;
		default:
			break;
		}
		break;
	case 1:
		switch (mode)
		{
		case 0:
			timerFuncIncreFre(10, 252, speed, 100,valueSpeedUp);
			break;
		default:
			break;
		}
		break;
	case 9998: // tang toc va giu on dinh toc do
		//Serial.println("ham tang toc");
		timerFuncIncreFre(10, 220, speed, SAMPLE_NUMBER, valueSpeedUp, 2);
		break;
	case 9999: // ham tang toc doc lap
		timerFuncIncreFre(10, 253, speed, SAMPLE_NUMBER, valueSpeedUp, 1);
		break;
	case 10000:
		//Serial.println("chay ham giam toc");
		timerFuncDecreFre(10, valueSpeedUp, 253, SAMPLE_NUMBER, valueSpeedDown, 1);
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

void readData(signed short &_valX, signed short &_valY) {
	uint8_t ptr[4];
	ptr[0] = Serial2.read();
	ptr[1] = Serial2.read();
	ptr[2] = Serial2.read();
	ptr[3] = Serial2.read();
	_valX = (ptr[0] << 8) | short(ptr[1]);
	_valY = (ptr[2] << 8) | short(ptr[3]);
	Serial.println(_valX);
}

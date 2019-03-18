#pragma once
/*decription*/
// I trung điểm mecanum
// A bánh Left
// B bánh Right
//
#include"Configuration.h"

#define _CONTROL_H_	// use potition mode

#ifndef _CONTROL_H_ // analog mode
#define _CONTROL_H_
void controlOmni(int _speed);
void controlMecanumLeft(int _speed);
void controlMecanumRight(int _speed);
void goStraight(int _speed);
void move(double _omega, double _vIx, double _vIy);
void goBack(int _speed);
void run(double _alpha,double _speed);
void circle(int _speed);
void freeRun();
void runLeft(int _speed);
void runRight(int _speed);
#endif // !_CONTROL_H_

#ifdef _CONTROL_H_	//potition mode
//void stopState();
void hamtangtoc(int _time);
//void runStraight(int _numMeca,int _numOm); //_num is pulse /s
#endif // _CONTROL_H_


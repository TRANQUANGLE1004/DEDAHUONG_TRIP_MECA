/*MECA(	O )		
	   / \	
      /   \
	 /     \
	/		\
MECA(L)---MECA(R)
*/
#pragma once
#ifndef _PWM_LIB_H_
#define _PWM_LIB_H_
#include"Configuration.h"
// this function setting direction //
void setDirection(bool _dirO, bool _dirL, bool _dirR);
//
void run(int maxFre, int _pulseSum, byte _mode = 0);
//
unsigned short getICRFastPWM(int _frequency);
//
unsigned short getBottomTimerNomalMode(int _time);
//
void settingTimer1(int _icrVal);
//
void settingTimer4(int _icrVal);
//
void runTimer1() { TCCR1B |= (1 << CS10); }
void runTimer4() { TCCR4B |= (1 << CS40); }
void stopTimer1() { TCCR1B &= 0xF8; }
void stopTimer4() { TCCR4B &= 0xF8; }
//
void timerFuncIncreSpeed(int _time, int* _arrA, int* _arrB)
//
#endif // !_PWM_LIB_H_

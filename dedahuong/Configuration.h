#pragma once
#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#define MAX_VALUE 255

#define PIN_PULSE_OMNI 6
#define PIN_DIR_OMNI 5
#define PIN_PULSE_MECANUM_LEFT 13
#define PIN_DIR_MECANUM_LEFT 12
#define PIN_PULSE_MECANUM_RIGHT 11
#define PIN_DIR_MECANUM_RIGHT 10

#define DIRECTION_OMNI 0 //quy dinh chieu duong +
#define DIRECTION_MECANUM_LEFT 1 
#define DIRECTION_MECANUM_RIGHT 0

#define LOCK 2
#define FREE_STATE 0

#define PIN_KHI23	23
#define PIN_KHI24	24
#define PIN_KHI25	25
#define PIN_KHI26	26

#define DECELERATION_TIME 500
#define SAMPLE_NUMBER	50

#define		Kp_Y	4
#define		Ki_Y	10
#define		Kd_Y	0.1

#define		Kp_X	2
#define		Ki_X	4
#define		Kd_X	0.1

#define		NUMBER_CONFIG	100.0
#endif // !_CONFIGURATION_H_

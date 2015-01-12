/*
	Written by Ben McChesney 1/11/2015

	stores calubration data per skeleton

*/

#pragma once

#include "ofMain.h"
#include "ofxSimpleTimer.h"

class UserCalibrationData
{
public :
	UserCalibrationData() { } 

	bool bCalibrated ;
	bool bSkeletonActive ; 
	int userId ; 

	ofPoint calibrationPoint ; 
	ofxSimpleTimer calibrationTimer ; 
	ofxSimpleTimer userLostTimer ;
	int calibratedJointIndex ;		//store whether it's right or left hand

	void setup ( ) ; 
	void update( ) ; 
	void reset( ) ; 

	void calibrationTimerComplete ( int & args ) ; 
	void userLostTimerComplete ( int & args ) ; 

};
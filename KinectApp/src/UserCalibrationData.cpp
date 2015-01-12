#include "UserCalibrationData.h"

void UserCalibrationData::setup ( ) 
{
	calibrationTimer.setup( 1000 , "calibration timer" , 20 ) ; 
	userLostTimer.setup( 6000 , "user lost timer" , 25 ) ; 

	ofAddListener( calibrationTimer.TIMER_COMPLETE , this, &UserCalibrationData::calibrationTimerComplete ) ; 
	ofAddListener( userLostTimer.TIMER_COMPLETE , this, &UserCalibrationData::userLostTimerComplete ) ; 
}

void UserCalibrationData::update( ) 
{
	calibrationTimer.update() ; 
	userLostTimer.update() ; 
}

void UserCalibrationData::reset( ) 
{
	bCalibrated = false ;
	calibrationTimer.reset() ; 
	userLostTimer.reset() ; 
	userId = -1 ; 
	bSkeletonActive = false ; 
	calibrationPoint = ofPoint( 0 , 0 ) ; 
	calibratedJointIndex = 0 ; 
}

void UserCalibrationData::calibrationTimerComplete ( int & args ) 
{
	ofLogNotice() << " UserCalibrationData::calibrationTimerComplete ! " ; 
	bCalibrated = true ; 
}

void UserCalibrationData::userLostTimerComplete ( int & args ) 
{
	bCalibrated = false ;
	ofLogNotice() << " UserCalibrationData::userLostTimerComplete ! " ; 
}
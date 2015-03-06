/* 

Written by Ben McChesney 11/1/2014

this class is a holder for all kinect related functions including drawing the depth feed representation. 

*/

#pragma once

#include "ofMain.h"
#include "ofxPanel.h"
#include "ofxKinectNui.h"
#include "ofxKinectNuiPlayer.h"
#include "ofxKinectNuiRecorder.h"
#include "ofxSimpleTimer.h"
#include "UserCalibrationData.h"
#include "Tweenzor.h"
#include "KinectCalibrationWidget.h"
#include "KinectCursor.h"
#include "ofxFontStashTextBlock.h"

class ofxKinectNuiDrawTexture;
class ofxKinectNuiDrawSkeleton;

class KinectNuiManager
{
public :
	KinectNuiManager() { } 

	void setup( ) ;
	void setupGui( ofxPanel * gui ) ; 
	void update( ) ; 
	void draw( ) ;
	void drawDebug( float x , float y ) ; 

	//Gui Parameters

	ofParameter< int > nearClipping ; 
	ofParameter< int > farClipping ; 
	ofParameter< int > angle ; 
	ofParameter< ofPoint > offset ; 

	void nearClipingHandler( int & nearClip ) ; 
	void farClippingHandler( int & farClip ) ; 
	void angleHandler( int & angle) ; 
	void offsetHandler( ofPoint & position ) ; 

	ofxKinectNui kinect;
	ofxBase3DVideo* kinectSource;
	//ofTexture calibratedTexture;

	ofxKinectNuiDrawTexture*	videoDraw_;
	ofxKinectNuiDrawTexture*	depthDraw_;
	ofxKinectNuiDrawTexture*	labelDraw_;
	ofxKinectNuiDrawSkeleton*	skeletonDraw_;

	bool bPlugged;
	bool bUnplugged;
	void kinectPlugged();
	void kinectUnplugged();

	//void drawCalibratedTexture();
	void exit( ) ; 

	/*
	static enum CALLIBRATION_STATE
	{
		NO_USER = 0 , 
		SEARCHING_NO_HERO = 1 , 
		HERO_CALIBRATED = 2 , 
		HERO_LOST = 3
	};*/

	//general debug
	string debugStream ; 

	//Calibration
	//CALLIBRATION_STATE calibrationState ;
	int getNumActiveUsers ( ) ; 
	//void changeState ( CALLIBRATION_STATE state ) ; 
	vector < UserCalibrationData * > userDataPool ; 

	//ofxSimpleTimer heroLostTimer ; 
	//void heroLostTimerComplete ( int & args ) ; 
	UserCalibrationData * hero ; 
	
	KinectCalibrationWidget calibrationWidget ; 
	void checkForCalibration() ;

	//Cursor detection
	ofPoint cursorRegionOrigin ; 
	ofParameter< ofPoint > cursorRegionDims ; 
	

	//Utils
	//string getStringFromCalibrationState ( CALLIBRATION_STATE state ) ;

	KinectCursor kinectCursor ; 
	ofParameter< float > interpolateTime ; 
	//ofParameter< float > heroLostDuration ; 
	//ofParameter< float > initialHeroTimeoutDuration ; 

	//ofxSimpleTimer initialHeroLostTimer ; 
	//void initialHeroLostTimerComplete ( int &args ); 

	ofxFontStashTextBlock instructionsBlock ; 

	ofxButton setClippingButton;
	void clippingButtonHit( ) ; 

	void clearAllUsers( ) ; 

	void setClosestSkeletonToHero(); 
	void calibrateCursorRegion(UserCalibrationData * data);


};
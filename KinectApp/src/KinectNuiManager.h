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

	static enum CALLIBRATION_STATE
	{
		NO_USER = 0 , 
		SEARCHING_NO_HERO = 1 , 
		HERO_CALIBRATED = 2 , 
		HERO_LOST = 3
	};

	CALLIBRATION_STATE calibrationState ;
	int getNumActiveUsers ( ) ; 
	void changeState ( CALLIBRATION_STATE state ) ; 
	vector < UserCalibrationData * > userDataPool ; 

	UserCalibrationData * hero ; 
	string debugStream ; 

	KinectCalibrationWidget calibrationWidget ; 
	void checkForCalibration() ;

	//Cursor detection
	float centerX ; 



};
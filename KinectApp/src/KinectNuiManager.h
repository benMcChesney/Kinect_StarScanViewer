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

	//Gui Parameters

	ofParameter< int > nearClipping ; 
	ofParameter< int > farClipping ; 
	ofParameter< int > angle ; 
	ofParameter< ofPoint > offset ; 

	void nearClipingHandler( int & nearClip ) ; 
	void farClipingHandler( int & farClip ) ; 
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


};
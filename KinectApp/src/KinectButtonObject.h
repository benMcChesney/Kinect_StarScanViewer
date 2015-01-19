#pragma once

/* quick and hacky interactive kinect button base class */

#include "ofMain.h"
#include "ofxSimpleTimer.h"
#include "ofxAlphaStackItem.h"
#include "ofxMatrixTransformObject.h"
#include "ofxMatrixTransformImage.h"

class KinectButtonObject : public ofxAlphaStackItem , public ofxMatrixTransformObject
{
public : 
	KinectButtonObject() { } 

	void setup( ) ; 
	void update( ) ; 
	void draw( ) ; 
	void drawDebug( ) ; 
	void reset( ) ; 

	bool isHit( ofPoint p ) ; 
	void setHitAreaFromImage( ofxMatrixTransformImage * image ) ; 

	void onOver( ) ; 
	void onOff( ) ; 
	void onInput( ofPoint ) ; 

	ofRectangle hitArea ; 
	ofxSimpleTimer hoverTimer ;

	bool bInteractive ; 
	bool bHoverSelected ; 
	bool bHover ; 

	ofColor debugColor ; 
	

};
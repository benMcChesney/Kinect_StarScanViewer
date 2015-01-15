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
	void reset( ) ; 
	bool isHit( ofPoint p ) ; 
	void onOver( ) ; 
	void onOff( ) ; 

	ofRectangle hitArea ; 
	ofxSimpleTimer hoverTimer ;

	bool bHoverSelected ; 
	bool bHover ; 

};
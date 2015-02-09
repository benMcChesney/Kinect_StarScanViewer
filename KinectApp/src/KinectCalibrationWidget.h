/*
	Written by Ben McChesney on 1/11/2015

	This class is the visual representation of the user calibration screen

*/

#pragma once

#include "ofMain.h"
#include "ofxMatrixTransformObject.h"
#include "ofxAlphaStackItem.h"
#include "ofxKinectNui.h"
#include "Tweenzor.h"

class KinectCalibrationWidget : public ofxMatrixTransformObject , public ofxAlphaStackItem
{
public : 
	KinectCalibrationWidget() { } 

	void setup() ; 
	void update() ; 
	void draw() ; 

	void transitionIn() ; 
	void transitionOut() ; 

	ofRectangle drawSize ; 
	ofxKinectNui * kinect ; 

	
};


#pragma once

#include "ofxKinectNui.h"

class KinectCursor
{
public : 
	KinectCursor( ) { } 

	void setup( ) ; 
	void draw( ) ; 

	ofPoint worldPosition ; 
	ofPoint normalizedScreenPosition ; 
	ofPoint screenPosition ;

};
#pragma once

#include "ofxKinectNui.h"

class KinectCursor
{
public : 
	KinectCursor( ) { } 


	void setup( ) ; 
	void draw( ) ; 

	ofPoint normalizedScreen ; 
	ofPoint screenPosition ;

};
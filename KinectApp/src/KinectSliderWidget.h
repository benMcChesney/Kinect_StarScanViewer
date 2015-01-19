/*
	Creatd by Ben McChesney 1/17/2015

	slider widget similar to the HuluPlus xbox 360 widget
	there is an active area, when the cursor first enters it the slider moves
	to loose control the user must pull away form the slider in the perpendicular direction

*/

#pragma once

#include "ofMain.h"
#include "KinectButtonObject.h"
#include "Tweenzor.h"

class KinectSliderWidget : public KinectButtonObject
{
public : 
	KinectSliderWidget( ) { }  

	void setup( ) ; 
	void update( ) ; 
	void draw( ) ; 
	void drawDebug( ) ; 

	void transitionIn ( ) ; 
	void transitionOut( ) ; 
	bool isHit( ofPoint p ) ; 
	void onOver( ) ; 
	void onOff( ) ; 
	void onInput( ofPoint p ) ; 

	ofPoint sliderPoint ; 
	float smoothedNormalized ; 
	bool bCursorInRange ; 

	void hoverTimerComplete ( int &args ) ; 
	float getNormalizedValue( )  ; 

	ofParameter< float > interpolateTime ; 
	

};

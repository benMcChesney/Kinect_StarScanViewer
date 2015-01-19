/*

Written by Ben McChesney on 1/10/2015

this class allows the guest to select which two layers to show inside ImageCompareView

*/

#pragma once

#include "ofMain.h"
#include "Tweenzor.h"
#include "ofxFontStashTextBlock.h"
#include "KinectButtonObject.h"
#include "AtmosphericImageData.h"
#include "StarScanViewerEvents.h"

class ThumbnailWidget : public KinectButtonObject
{
public :
	ThumbnailWidget( ) ; 

	void setup( ) ; 
	void update( ) ; 
	void draw( ) ; 

	void transitionIn ( ) ; 
	void transitionOut( ) ; 

	ofxFontStashTextBlock labelBlock ;

	AtmosphericImageData * data ; 
	int thumbWidth ; 

	void hoverTimerComplete ( int &args ) ; 
	bool bSelectedState ; 

};


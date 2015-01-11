/*
	Written by Ben McChesney 1/10/2015

	This class compares two different sets of data and displays some text blocks as well

*/
#pragma once

#include "Tweenzor.h"
#include "ofxSimpleMask.h"
#include "ofxMatrixTransformImage.h"
#include "ofxFontStashTextBlock.h"
#include "ImageDetailView.h"

class ImageCompareView
{
public : 
	ImageCompareView( ) { } 

	void setup ( ) ; 
	void update( ) ; 
	void draw( ) ; 
	void drawDebug( ) ; 

	ImageDetailView leftView ; 
	ImageDetailView rightView ; 
	ofxSimpleMask mask ; 
	float normalizedAmount ; 

	 
};
/*
	Written by Ben McChesney 1/10/2015

	This class compares two different sets of data and displays some text blocks as well

*/

#pragma once

#include "ofMain.h"
#include "ofxMatrixTransformObject.h"
#include "ofxMatrixTransformImage.h"
#include "ofxFontStashTextBlock.h"
#include "ofxAlphaStackItem.h"
#include "Tweenzor.h"

class ImageDetailView : public ofxMatrixTransformObject, public ofxAlphaStackItem
{
public :
	ImageDetailView( ) { }

	ofxMatrixTransformImage * detailImage ; 
	ofxFontStashTextBlock title ; 
	ofxFontStashTextBlock description ; 
	ofFbo maskFbo ; 

	

	void setup ( ) ; 
	void update( ) ; 
	void draw( ) ; 
	void drawDebug( ) ; 

};
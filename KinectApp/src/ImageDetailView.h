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

	void setup ( ) ; 
	void update( ) ; 
	void draw( ) ; 
	void drawDebug( ) ; 

};
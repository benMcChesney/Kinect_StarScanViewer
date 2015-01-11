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


	 
};
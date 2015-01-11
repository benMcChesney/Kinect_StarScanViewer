#include "ImageCompareView.h"

/*
	ImageDetailView leftView ; 
	ImageDetailView rightView ; 
	ofxSimpleMask mask ; 

		ImageDetailView leftView ; 
	ImageDetailView rightView ; 
	ofxSimpleMask mask ; 
	*/

void ImageCompareView::setup ( )
{
	mask.setup( "shaders/composite_rgb" , ofRectangle( 0 , 0 , 1024, 1024 ) ) ; 
	leftView.setup() ; 
	rightView.setup() ; 
}
	
void ImageCompareView::update( ) 
{
	normalizedAmount = ofMap(  sin( ofGetElapsedTimef() ) , -1.0f , 1.0f , 0.0f , 1.0f , true )  ; 
	leftView.update() ; 
	rightView.update( ) ;
}
	
void ImageCompareView::draw( ) 
{
	//Draw Left active area
	leftView.maskFbo.begin() ; 
		ofSetColor( ofColor::black ) ; 
		ofRect( 0 , 0 , 1024.0f , ofGetHeight() ) ; 
		ofSetColor( ofColor::white ) ; 
		ofRect( 0 , 0 , 1024.0f * normalizedAmount , ofGetHeight() ) ; 
	leftView.maskFbo.end() ; 

	//Draw Right active area
	rightView.maskFbo.begin( ) ; 
		//float _y = ofGetHeight() /2 ; 
		ofSetColor( ofColor::black ) ; 
		ofRect( 0 , 0 , 1024.0f , ofGetHeight() ) ; 
		ofSetColor( ofColor::white ) ; 
		ofRect( 1024.0f * normalizedAmount  , 0 , 1024.0f * ( 1.0f - normalizedAmount ) , ofGetHeight() ) ; 
	rightView.maskFbo.end( ) ; 
	
	
	ofPushMatrix() ; 
		float offsetX = ( ofGetWidth() - 1024 ) / 2 ;
		ofTranslate( offsetX , 0  ) ; 
		mask.drawMask( rightView.detailImage->image.getTextureReference() , rightView.maskFbo.getTextureReference() , 0 , 0 , 1.0f ) ; 
		mask.drawMask( leftView.detailImage->image.getTextureReference() , leftView.maskFbo.getTextureReference() , 0 , 0 , 1.0f ) ; 
		//rightView.maskFbo.draw( 0 , 0 ) ; 
		

		
		

	ofPopMatrix() ; 
}
	
void ImageCompareView::drawDebug( ) 
{
	stringstream ss ; 
	//ss << "rightView : " << rightView.detailImage->getAl
}


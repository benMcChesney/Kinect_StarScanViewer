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
	mask.setup( "shaders/composite" , ofRectangle( 0 , 0 , 1024, 1024 ) ) ; 
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
	ofPushMatrix() ; 
		float offsetX = ( ofGetWidth() - 1024 ) / 2 ;
		ofTranslate( offsetX , 0  ) ; 

		//Draw Left active area
		ofSetColor( ofColor::black ) ; 
		ofRect( 0 , 0 , 1024.0f , ofGetHeight() / 2 ) ; 
		ofSetColor( ofColor::white ) ; 
		ofRect( 0 , 0 , 1024.0f * normalizedAmount , ofGetHeight() / 2 ) ; 

		//Draw Right active area
		float _y = ofGetHeight() /2 ; 
		ofSetColor( ofColor::black ) ; 
		ofRect( 0 , _y , 1024.0f , ofGetHeight() / 2 ) ; 
		ofSetColor( ofColor::white ) ; 
		ofRect( 1024.0f * normalizedAmount  , _y , 1024.0f * ( 1.0f - normalizedAmount ) , ofGetHeight() / 2 ) ; 

	ofPopMatrix() ; 


}
	
void ImageCompareView::drawDebug( ) 
{

}


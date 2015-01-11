#include "ImageCompareView.h"

void ImageCompareView::setup ( )
{
	mask.setup( "shaders/composite_rgb" , ofRectangle( 0 , 0 , 1024, 1024 ) ) ; 
	leftView.setup() ; 
	rightView.setup() ; 

	float marginWidth = ( ofGetWidth() - 1024 ) / 2 ; 
	rightView.x = ofGetWidth() - marginWidth ; 
}
	
void ImageCompareView::update( ) 
{
	normalizedAmount = ofMap(  sin( ofGetElapsedTimef() ) , -1.0f , 1.0f , 0.0f , 1.0f , true )  ; 
	leftView.update() ; 
	leftView.alphaStackUpdate( 1.0f ) ; 
	rightView.update( ) ;
	rightView.alphaStackUpdate( 1.0f ) ; 
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
	
	float marginWidth = ( ofGetWidth() - 1024 ) / 2 ; 
	ofPushMatrix() ; 
		float offsetX = marginWidth ;
		ofTranslate( offsetX , 0  ) ; 

		//Prevent spamming error messages
		if ( rightView.detailImage->image.bAllocated() ) 
			mask.drawMask( rightView.detailImage->image.getTextureReference() , rightView.maskFbo.getTextureReference() , 0 , 0 , 1.0f ) ; 

		//Prevent spamming error messages
		if ( leftView.detailImage->image.bAllocated() ) 
			mask.drawMask( leftView.detailImage->image.getTextureReference() , leftView.maskFbo.getTextureReference() , 0 , 0 , 1.0f ) ; 

		//rightView.maskFbo.draw( 0 , 0 ) ; 
	ofPopMatrix() ; 

	leftView.draw( ) ; 

	rightView.x = ofGetWidth() - marginWidth ; 
	rightView.draw( ) ; 

}
	
void ImageCompareView::drawDebug( ) 
{
	stringstream ss ; 
	ss << "rightView alpha : " << rightView.getOFAlpha() << endl ;
	ss << "right view text alpha : " << rightView.title.getOFAlpha() << endl ; 

	ofDrawBitmapStringHighlight ( ss.str() , 50 , 50 ) ; 

	leftView.drawDebug() ; 
	rightView.drawDebug() ; 
}


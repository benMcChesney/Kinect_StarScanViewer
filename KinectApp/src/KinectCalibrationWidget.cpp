#include "KinectCalibrationWidget.h"
	
void KinectCalibrationWidget::setup() 
{
	ofxAlphaStackItem::setup( ) ; 
	ofxMatrixTransformObject::setup ( ) ; 
}

void KinectCalibrationWidget::update() 
{
	alphaStackUpdate( alpha ) ; 
}

void KinectCalibrationWidget::draw() 
{
	//if ( getOFAlpha() > 0.0f ) 
	//	return ; 

	pushMatrix( ) ; 

		ofTranslate( -drawSize.getWidth()/2 , -drawSize.getHeight()/2 ) ;
		
		ofPushMatrix() ; 
			ofEnableAlphaBlending() ;
			ofSetColor( 255 , getOFAlpha()) ; 
			kinect->drawDepth( 0, 0, drawSize.width , drawSize.height  );
			ofSetColor( 255 , 128 ) ; 
			kinect->drawLabel( 0, 0, drawSize.width , drawSize.height );
			ofSetColor( 255 ) ; 
			kinect->drawSkeleton(0 ,0, drawSize.width , drawSize.height );
		ofPopMatrix() ;
		
	popMatrix() ; 
}

void KinectCalibrationWidget::transitionIn() 
{
	ofLogNotice() << "KinectCalibrationWidget::transitionIn() " ; 
	Tweenzor::add( &alpha , 0.0f , 1.0f , 0.0f , 0.5f , EASE_OUT_QUAD ) ;  
	x = ofGetWidth() / 2 ; 
	y = ofGetHeight() / 2 ; 

	Tweenzor::add( &drawSize.width , 2.0f , 640.0f * 1.5f , 0.0f , 0.5f , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &drawSize.height , 2.0f , 480.0f * 1.5f, 0.05f , 0.45f , EASE_OUT_QUAD ) ; 
}

void KinectCalibrationWidget::transitionOut()
{
	Tweenzor::add( &alpha , 1.0f , 0.0f , 0.0f , 0.5f , EASE_OUT_QUAD ) ;  
	Tweenzor::add( &drawSize.width , drawSize.width , 2.0f , 0.0f , 0.5f , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &drawSize.height , drawSize.height , 2.0f , 0.05f , 0.45f , EASE_OUT_QUAD ) ;
}
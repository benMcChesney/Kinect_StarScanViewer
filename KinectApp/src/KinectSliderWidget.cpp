#include "KinectSliderWidget.h"

void KinectSliderWidget::setup( ) 
{
	KinectButtonObject::setup( ); 
	bCursorInRange = false ; 
	sliderPoint = ofPoint( hitArea.getWidth() / 2 + hitArea.x ,  hitArea.getHeight() / 2  + hitArea.y ) ; 
	alpha = 1.0f ; 
	bInteractive = true ; 
	smoothedNormalized = 0.5f ; 

}

void KinectSliderWidget::update( ) 
{
	KinectButtonObject::update( ) ; 
}

void KinectSliderWidget::draw( ) 
{
	ofPushStyle( ) ; 
		ofNoFill( ) ; 
		ofSetLineWidth( 2 ) ; 
		ofSetColor( 255 , getOFAlpha() * .75f ); 
		float padding = hitArea.height * .4f ; 
		ofRectRounded( hitArea.x , hitArea.y + padding , hitArea.getWidth() , hitArea.getHeight() - ( padding * 2 ) , 15 ) ; 

		ofFill( ) ; 
		ofSetLineWidth( 0 ) ; 
		ofSetCircleResolution( 180 ); 
		ofSetColor( 255 , getOFAlpha() ); 
		ofCircle(sliderPoint, 20); // hitArea.getHeight() / 4 );
	ofPopStyle( ) ; 
}

void KinectSliderWidget::transitionIn ( ) 
{
	Tweenzor::add( &alpha , 0.0f , 1.0f , 0.0f , 0.5f, EASE_OUT_QUAD ) ; 
}

void KinectSliderWidget::transitionOut( ) 
{
	Tweenzor::add( &alpha , 1.0f , 0.0f , 0.0f , 0.5f, EASE_OUT_QUAD ) ; 
}

void KinectSliderWidget::hoverTimerComplete ( int &args ) 
{

}

bool KinectSliderWidget::isHit( ofPoint p ) 
{
	if ( !bInteractive || calcAlpha < 1.0f ) return false ; 
	bool bResult = hitArea.inside ( p ) ; 
	if ( bResult == true && !hoverTimer.bIsRunning && bHover == false ) 
	{
		onOver( ) ; 
	}
	return  bResult ; 
}

float KinectSliderWidget::getNormalizedValue( ) 
{
	return ofMap( sliderPoint.x , hitArea.x , hitArea.x + hitArea.getWidth() , 0.0f , 1.0f , true ) ; 
}

void KinectSliderWidget::onOver( ) 
{

}

void KinectSliderWidget::onOff( ) 
{

}

void KinectSliderWidget::onInput( ofPoint p ) 
{
	if ( alpha < 1.0f ) return ; 
	//if ( !bHover ) return  ; 

	sliderPoint.x = p.x ; 
	sliderPoint.y = hitArea.y + hitArea.getHeight() / 2 ; 

	
	Tweenzor::add( &smoothedNormalized , smoothedNormalized , getNormalizedValue() , 0.0f , interpolateTime , EASE_OUT_QUAD ) ; 
}

void KinectSliderWidget::drawDebug( ) 
{
	ofSetColor( debugColor ) ; 
	ofRect( hitArea ) ; 
}
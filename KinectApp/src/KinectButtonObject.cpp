#include "KinectButtonObject.h"

void KinectButtonObject::setup( ) 
{
	hoverTimer.setup( 1500 ) ; 
	bInteractive = true ; 
	reset() ; 
	debugColor = ofColor( ofRandom( 255 ) , ofRandom ( 255 ) , ofRandom( 255 ) ) ; 
}

void KinectButtonObject::update( ) 
{
	hoverTimer.update(); 
}

void KinectButtonObject::draw( ) 
{
	//draw
}


void KinectButtonObject::drawDebug( ) 
{
	if ( getOFAlpha() < 255 ) return ;  
	ofPushMatrix( ) ; 
		ofPushStyle() ; 
			ofSetColor( debugColor ) ; 
			ofRect( hitArea ) ; 
		ofPopStyle() ; 
	ofPopMatrix() ; 
}


void KinectButtonObject::reset( ) 
{
	hoverTimer.stop( ) ; 
	bHover = false ; 
	bHoverSelected = false ; 
	//bInteractive = false ; 
}

bool KinectButtonObject::isHit( ofPoint p ) 
{
	if ( !bInteractive || calcAlpha < 1.0f ) return false ; 
	bool bResult = hitArea.inside ( p ) ; 
	if ( bResult == true && !hoverTimer.bIsRunning && bHover == false ) 
	{
		onOver( ) ; 
	}
	return  bResult ; 
}

void KinectButtonObject::onOver( ) 
{
	if ( !bInteractive || calcAlpha < 1.0f ) return ; 
	hoverTimer.start( false , true ) ; 
	bHover = true ; 
}

void KinectButtonObject::onOff( ) 
{
	if ( !bInteractive || calcAlpha < 1.0f ) return ; 
	bHover = false ; 
	hoverTimer.stop() ; 
}

void KinectButtonObject::setHitAreaFromImage( ofxMatrixTransformImage * image ) 
{
	hitArea.x = image->x - image->getWidth() * image->_anchor.x ; 
	hitArea.y = image->y - image->getHeight() * image->_anchor.y ; 
	hitArea.width = image->getWidth() ; 
	hitArea.height = image->getHeight() ; 
}



#include "KinectButtonObject.h"

void KinectButtonObject::setup( ) 
{
	hoverTimer.setup( 1500 ) ; 
}

void KinectButtonObject::update( ) 
{
	hoverTimer.update(); 
}

void KinectButtonObject::draw( ) 
{
	//
}

void KinectButtonObject::reset( ) 
{
	hoverTimer.stop( ) ; 
	bHover = false ; 
	bHoverSelected = false ; 
}

bool KinectButtonObject::isHit( ofPoint p ) 
{
	bool bResult = hitArea.inside ( p ) ; 
	if ( bResult == true && !hoverTimer.bIsRunning && bHover == false ) 
	{
		onOver( ) ; 
	}
	return  bResult ; 
}

void KinectButtonObject::onOver( ) 
{
	hoverTimer.start( false , true ) ; 
	bHover = true ; 
}

void KinectButtonObject::onOff( ) 
{
	bHover = false ; 
	hoverTimer.stop() ; 
}



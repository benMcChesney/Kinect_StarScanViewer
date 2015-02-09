#include "KinectCursor.h"

void KinectCursor::setup( ) 
{
	image.loadImage( "ui/cursor.png" ) ; 
	image.setAnchorPercent( 0.5, 0.5 ) ; 
}

void KinectCursor::draw( ) 
{
	ofPushMatrix() ; 
		ofPushStyle() ; 
			ofSetColor( ofColor::white ) ; 
			image.draw( screenPosition.x , screenPosition.y , 80 , 80 ) ; 
			//ofCircle( screenPosition.x , screenPosition.y , 40 ) ;
		ofPopStyle()  ;
	ofPopMatrix() ; 
}


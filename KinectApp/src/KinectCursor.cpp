#include "KinectCursor.h"

void KinectCursor::setup( ) 
{

}

void KinectCursor::draw( ) 
{
	ofPushMatrix() ; 
		ofPushStyle() ; 
			ofSetColor( ofColor::yellow ) ; 
			ofCircle( screenPosition.x , screenPosition.y , 40 ) ;
		ofPopStyle()  ;
	ofPopMatrix() ; 
}


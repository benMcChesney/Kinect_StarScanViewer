#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	Tweenzor::init() ; 
	bool bOpenResult = spectrumJson.open ( "Spectrum_Data.json" ) ; 
	if ( bOpenResult ) 
	{
		dataSyncManager.setup( ofToDataPath( "atmospheric_images" ) ) ; 
		dataSyncManager.loadFromJsonRef( spectrumJson ) ; 
	}
	else
	{
		ofLogError( "spectrumData.json did not load !" ) ; 
	}

	imageCompareView.setup() ; 
	//dataSyncManager.loadingManager.addToThreadedImageQueue( imageCompareView.leftView.detailImage->image , dataSyncManager.atmosphericImageData[0]->url , true ) ; 
	//dataSyncManager.loadingManager.addToThreadedImageQueue( imageCompareView.rightView.detailImage->image , dataSyncManager.atmosphericImageData[1]->url , true ) ; 
	imageCompareView.leftView.populateFromData( *dataSyncManager.atmosphericImageData[0] ) ; 
	imageCompareView.rightView.populateFromData( *dataSyncManager.atmosphericImageData[1] ) ; 

	ofSetVerticalSync( true ) ; 
	ofBackground( 0 ) ; 

	gui.setup() ; 
	gui.add( bDrawDebug.set( "DRAW DEBUG" , false ) ) ; 

	gui.add( imageCompareView.leftView.delayIncrement.set( "LEFT DELAY INC" , 0.1f , 0.0f , 1.0f ) ) ; 
	gui.add( imageCompareView.leftView.transitionDuration.set( "LEFT TRANSITION DURATION" , 0.1f , 0.0f , 1.0f ) ) ; 
	gui.add( imageCompareView.leftView.transitionSlideY.set( "LEFT TRANSITION SLIDE Y" , -20.0f , -40.0f , 40.0f ) ) ; 

	gui.add( imageCompareView.rightView.delayIncrement.set( "LEFT DELAY INC" , 0.1f , 0.0f , 1.0f ) ) ; 
	gui.add( imageCompareView.rightView.transitionDuration.set( "LEFT TRANSITION DURATION" , 0.1f , 0.0f , 1.0f ) ) ; 
	gui.add( imageCompareView.rightView.transitionSlideY.set( "LEFT TRANSITION SLIDE Y" , -20.f , -40.0f , 40.0f ) ) ; 

	imageCompareView.rightView.transitionOut() ; 
	imageCompareView.leftView.transitionOut() ; 

	//reset all transitions
	initialTweenDelay = 0 ;
	Tweenzor::add( &initialTweenDelay , 0.0f , 1.0f , 0.0f , 2.0f ) ; 
	Tweenzor::addCompleteListener( Tweenzor::getTween( &initialTweenDelay ) , this , &ofApp::initialTweenDelayComplete ) ; 
}

void ofApp::initialTweenDelayComplete ( float * args ) 
{
	imageCompareView.leftView.transitionIn() ; 
	imageCompareView.rightView.transitionIn() ; 
}

//--------------------------------------------------------------
void ofApp::update()
{
	Tweenzor::update( ofGetElapsedTimeMillis() ) ; 
	dataSyncManager.update( ) ; 

	//imageCompareView.alphaStackUpdate( imageCompareView.alpha ) ; 
	imageCompareView.update( ) ; 
}

//--------------------------------------------------------------
void ofApp::draw()
{
	imageCompareView.draw() ; 

	if ( bDrawDebug ) 
	{
		imageCompareView.drawDebug() ; 
	}

	if ( bShowGui == true ) 
	{
		gui.draw( ) ; 
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch ( key ) 
	{
		case 's':
		case 'S':
			bShowGui = !bShowGui ; 
			break ; 
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

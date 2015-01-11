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

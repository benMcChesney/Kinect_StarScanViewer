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
	dataSyncManager.loadingManager.addToThreadedImageQueue( imageCompareView.leftView.detailImage->image , dataSyncManager.atmosphericImageData[0]->url , true ) ; 
	dataSyncManager.loadingManager.addToThreadedImageQueue( imageCompareView.rightView.detailImage->image , dataSyncManager.atmosphericImageData[1]->url , true ) ; 
	
	ofSetVerticalSync( true ) ; 
	ofBackground( 0 ) ; 

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
	//dataSyncManager.draw( ) ; 
	/*
	ofPushMatrix() ; 
		ofScale( 0.2 , 0.2 ) ; 
		int increment = 0; 
		for ( auto data = dataSyncManager.atmosphericImageData.begin() ; data != dataSyncManager.atmosphericImageData.end() ; data++ ) 
		{
			(*data)->image.draw() ; 
			(*data)->image.x = increment * (*data)->image.getWidth() * 0.2 ; 
			(*data)->image.y = 250 ; 
			increment++ ; 
		}
	ofPopMatrix() ; */

	imageCompareView.draw() ; 
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	Tweenzor::init() ; 
	bool bOpenResult = spectrumJson.open ( "Spectrum_Data.json" ) ; 

	int nearThreshold = 300; 
	int farThreshold = 1000; 
	if ( bOpenResult ) 
	{
		dataSyncManager.setup( ofToDataPath( "atmospheric_images" ) ) ; 
		dataSyncManager.loadFromJsonRef( spectrumJson ) ; 
		nearThreshold = spectrumJson["nearThreshold"].asInt();
		farThreshold = spectrumJson["farThreshold"].asInt();
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

	gui.add( thumbnailDragSize.set( "THUMBNAIL DRAG SIZE" , 125 , 50 , 300 ) ) ; 

	gui.add( imageCompareView.leftView.delayIncrement.set( "LEFT DELAY INC" , 0.1f , 0.0f , 1.0f ) ) ; 
	gui.add( imageCompareView.leftView.transitionDuration.set( "LEFT TRANSITION DURATION" , 0.1f , 0.0f , 1.0f ) ) ; 
	gui.add( imageCompareView.leftView.transitionSlideY.set( "LEFT TRANSITION SLIDE Y" , -20.0f , -40.0f , 40.0f ) ) ; 

	gui.add( imageCompareView.rightView.delayIncrement.set( "LEFT DELAY INC" , 0.1f , 0.0f , 1.0f ) ) ; 
	gui.add( imageCompareView.rightView.transitionDuration.set( "LEFT TRANSITION DURATION" , 0.1f , 0.0f , 1.0f ) ) ; 
	gui.add( imageCompareView.rightView.transitionSlideY.set( "LEFT TRANSITION SLIDE Y" , -20.f , -40.0f , 40.0f ) ) ; 



	imageCompareView.rightView.transitionOut() ; 
	imageCompareView.leftView.transitionOut() ; 
	imageCompareView.dataSyncManager = &dataSyncManager ; 
	imageCompareView.populateThumbnailsFromDataSync() ;

	kinectManager.setup() ; 
	kinectManager.setupGui( &gui ) ; 

	gui.add( imageCompareView.slider.interpolateTime.set( "SLIDER INTERPOLATE TIME" , 0.2f , 0.01f , 0.4f ) ) ;  
	gui.add( imageCompareView.sliderHeight.set( "SLIDER HEIGHT" , 100.0f , 50.0f , 800.0f )) ; 
	
	kinectManager.nearClipping.addListener( &kinectManager , &KinectNuiManager::nearClipingHandler ) ; 
	kinectManager.farClipping.addListener( &kinectManager , &KinectNuiManager::farClippingHandler ) ; 	
	gui.loadFromFile( "settings.xml" ) ; 
	
	kinectManager.clippingButtonHit() ; 
	//reset all transitions
	initialTweenDelay = 0 ;
	Tweenzor::add( &initialTweenDelay , 0.0f , 1.0f , 0.0f , 2.0f ) ; 
	Tweenzor::addCompleteListener( Tweenzor::getTween( &initialTweenDelay ) , this , &ofApp::initialTweenDelayComplete ) ; 

	ofHideCursor();
}

void ofApp::initialTweenDelayComplete ( float * args ) 
{
	imageCompareView.transitionIn() ; 
}

//--------------------------------------------------------------
void ofApp::update()
{
	Tweenzor::update( ofGetElapsedTimeMillis() ) ; 
	dataSyncManager.update( ) ; 

	//imageCompareView.alphaStackUpdate( imageCompareView.alpha ) ; 
	imageCompareView.update( ) ;

	kinectManager.update( ) ; 


	if ( kinectManager.hero != NULL ) 
	{
		ofPoint screenPos =  kinectManager.kinectCursor.screenPosition ; 
		for ( auto t = imageCompareView.thumbnails.begin() ; t != imageCompareView.thumbnails.end() ; t++ ) 
		{
			checkKinectInput( (*t) , screenPos ) ; 
		}

		checkKinectInput( &imageCompareView.rightDropZone , screenPos ) ; 
		checkKinectInput( &imageCompareView.leftDropZone , screenPos ) ; 
		checkKinectInput( &imageCompareView.cancelDropZone , screenPos ) ; 

		if ( imageCompareView.slider.bHover == false ) 
			imageCompareView.slider.onOver( ) ; 
		if ( imageCompareView.slider.hitArea.inside( screenPos )  ) 
		{
			 //imageCompareView.slider.onOver() ; 
			//cout << " INSIDE THE SLIDER !!! " << endl ; 
			 imageCompareView.slider.onInput( screenPos ) ;
			
		}
	}
}

void ofApp::checkKinectInput( ThumbnailWidget * thumbnail , ofPoint screenPos ) 
{
	if ( thumbnail->isHit( screenPos ) == true ) 
	{
		//cout << " THERE IS A HIT ON : " << (*t)->data->label << endl ; 
	}
	else
	{
		thumbnail->onOff( ) ; 
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{

	ofPushMatrix(); 
	//ofScale(0.71145833333333333333333333333333, 0.71145833333333333333333333333333); 
	imageCompareView.draw( ) ; 
	kinectManager.draw( ) ; 

	if ( imageCompareView.dim.a > 0 ) 
	{
		ofPoint screenPos =  kinectManager.kinectCursor.screenPosition ; 
		int size = thumbnailDragSize.get() ; 
		ofSetColor( 255 ) ; 
		imageCompareView.lastData->image.image.draw( screenPos.x - size/2 , screenPos.y - size/2 , size , size ) ; 
	}

	if ( bDrawDebug ) 
	{
		imageCompareView.drawDebug() ; 
		kinectManager.drawDebug(200 , 50 ) ; 


		imageCompareView.rightDropZone.drawDebug() ; 
		imageCompareView.leftDropZone.drawDebug() ; 
		imageCompareView.cancelDropZone.drawDebug() ; 

		imageCompareView.rightDropZone.hoverTimer.draw( 600 , 10 ) ;
		imageCompareView.rightDropZone.hoverTimer.draw( 600 , 60 ) ;
		imageCompareView.rightDropZone.hoverTimer.draw( 600 , 110 ) ;

		imageCompareView.slider.drawDebug( ) ; 
		stringstream ss ; 
		ss << " SLIDER HIT AREA : " << imageCompareView.slider.hitArea << endl ; 

		ss << " nearClip : " << kinectManager.nearClipping << " <-> far Clip : " << kinectManager.farClipping << endl ; 
		ofDrawBitmapStringHighlight( ss.str() , 1300 , 400 ) ; 
	}

	if ( bShowGui == true ) 
	{
		gui.draw( ) ; 
	}


	ofPopMatrix(); 
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch ( key ) 
	{

		case 'c':
		case 'C':
			kinectManager.clearAllUsers(); 
			break; 

		case 'g':
		case 'G':
			bShowGui = !bShowGui ;
			if (bShowGui)
				ofShowCursor();
			else
				ofHideCursor(); 
			break ; 

		case 'f':
		case 'F':
			ofToggleFullscreen(); 
			break; 

		case OF_KEY_LEFT: // increase the far clipping distance
		if( kinectManager.farClipping > kinectManager.nearClipping + 10){
			kinectManager.farClipping -= 10;
			kinectManager.kinectSource->setFarClippingDistance(kinectManager.farClipping);
		}
		break;
	case OF_KEY_RIGHT: // decrease the far clipping distance
		if(kinectManager.farClipping < 4000){
			kinectManager.farClipping += 10;
			kinectManager.kinectSource->setFarClippingDistance(kinectManager.farClipping);
		}
		break;
	case '+': // increase the near clipping distance
		if(kinectManager.nearClipping < kinectManager.farClipping - 10){
			kinectManager.nearClipping += 10;
			kinectManager.kinectSource->setNearClippingDistance(kinectManager.nearClipping);
		}
		break;
	case '-': // decrease the near clipping distance
		if(kinectManager.nearClipping >= 10){
			kinectManager.nearClipping -= 10;
			kinectManager.kinectSource->setNearClippingDistance(kinectManager.nearClipping);
		}
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

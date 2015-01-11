#include "KinectNuiManager.h"
#include "ofxKinectNuiDraw.h"

void KinectNuiManager::setup( ) 
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	ofxKinectNui::InitSetting initSetting;
	initSetting.grabVideo = false;
	initSetting.grabDepth = true;
	initSetting.grabAudio = false;
	initSetting.grabLabel = true;
	initSetting.grabSkeleton = true;
	initSetting.grabCalibratedVideo = true;
	initSetting.grabLabelCv = true;
	initSetting.videoResolution = NUI_IMAGE_RESOLUTION_640x480;
	initSetting.depthResolution = NUI_IMAGE_RESOLUTION_640x480;
	kinect.init(initSetting);
//	kinect.setMirror(false); // if you want to get NOT mirror mode, uncomment here
//	kinect.setNearmode(true); // if you want to set nearmode, uncomment here
	kinect.open();

	kinect.addKinectListener(this, &KinectNuiManager::kinectPlugged, &KinectNuiManager::kinectUnplugged);

	ofSetLogLevel(OF_LOG_NOTICE);

	ofSetVerticalSync(true);
	kinectSource = &kinect;
	angle = kinect.getCurrentAngle();
	bPlugged = kinect.isConnected();
	nearClipping = kinect.getNearClippingDistance();
	farClipping = kinect.getFarClippingDistance();
	
	
	//Setup Textures
	//calibratedTexture.allocate(kinect.getDepthResolutionWidth(), kinect.getDepthResolutionHeight(), GL_RGB);


	videoDraw_ = ofxKinectNuiDrawTexture::createTextureForVideo( kinect.getVideoResolution() );
	depthDraw_ = ofxKinectNuiDrawTexture::createTextureForDepth( kinect.getDepthResolution() );
	labelDraw_ = ofxKinectNuiDrawTexture::createTextureForLabel( kinect.getDepthResolution() );
	skeletonDraw_ = new ofxKinectNuiDrawSkeleton( );
	kinect.setVideoDrawer(videoDraw_);
	kinect.setDepthDrawer(depthDraw_);
	kinect.setLabelDrawer(labelDraw_);
	kinect.setSkeletonDrawer(skeletonDraw_);
}

void KinectNuiManager::setupGui( ofxPanel * gui ) 
{
	gui->add( nearClipping.set( "NEAR CLIPPING" , 1000 , 1 , 10000 ) ) ; 
	gui->add( farClipping.set( "FAR CLIPPING" , 1000 , 1 , 10000 ) ) ; 
	gui->add( angle.set( "ANGLE" , 0 , -30 , 30 ) ) ; 
	gui->add( offset.set( "OFFSET" , ofPoint( 500 , 500 ) , ofPoint( 1 , 1 ) , ofPoint ( ofGetWidth() , ofGetHeight() ) ) ); 

		/*
		*/
}

void KinectNuiManager::nearClipingHandler( int & nearClip ) 
{
	kinectSource->setNearClippingDistance( nearClip ) ; 
}

void KinectNuiManager::farClipingHandler( int & farClip ) 
{
	kinectSource->setFarClippingDistance( farClip ) ; 
}

void KinectNuiManager::angleHandler( int & angle) 
{
	kinect.setAngle( angle ) ; 
}

void KinectNuiManager::offsetHandler( ofPoint & position ) 
{

}
	

void KinectNuiManager::update( ) 
{
	kinect.update() ; 
	kinectSource->update();
}

void KinectNuiManager::draw( ) 
{
	// Draw user rep

	ofPushMatrix() ; 
		ofTranslate( offset ) ; 
		ofSetColor( 255 ) ; 
		ofEnableAlphaBlending() ; 
		int w = 240 ; 
		int h = 180 ; 
		//kinect.drawVideo( 0, 0, w, h );
		
		kinect.drawDepth( 0, 0, w, h );
		ofSetColor( 255 , 128 ) ; 
		kinect.drawLabel( 0, 0, w, h );
		ofSetColor( 255 ) ; 
		kinect.drawSkeleton(0 ,0, w, h);
	ofPopMatrix() ; 
}

void KinectNuiManager::kinectPlugged()
{
	bPlugged = true;
	ofLogNotice() << "kinect plugged in!" << endl ; 
}

void KinectNuiManager::kinectUnplugged()
{
	bPlugged = false;
	ofLogNotice() << "kinect unplugged in!" << endl ; 
}

void KinectNuiManager::exit( ) 
{
	/*
	//if(videoDraw_) {
		videoDraw_->destroy();
		videoDraw_ = NULL;
	}
	if(depthDraw_) {
		depthDraw_->destroy();
		depthDraw_ = NULL;
	}*/
	//if(labelDraw_) {
		//labelDraw_->destroy();
		//labelDraw_ = NULL;
	//}
	//if(skeletonDraw_) {
		//delete skeletonDraw_;
		//skeletonDraw_ = NULL;
	//}
	kinect.setAngle(0);
	kinect.close();
	kinect.removeKinectListener(this);
}
		/*

	ofParameter< int > nearClipping ; 
	ofParameter< int > farClipping ; 
	ofParameter< int > angle ; 

	ofParameter< ofPoint > offset ; 

	ofxKinectNui kinect;
	ofxBase3DVideo* kinectSource;

	ofxKinectNuiDrawTexture*	videoDraw_;
	ofxKinectNuiDrawTexture*	depthDraw_;
	ofxKinectNuiDrawTexture*	labelDraw_;
	ofxKinectNuiDrawSkeleton*	skeletonDraw_;*/
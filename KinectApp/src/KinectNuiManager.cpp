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
	initSetting.grabCalibratedVideo = false;
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
	videoDraw_ = ofxKinectNuiDrawTexture::createTextureForVideo( kinect.getVideoResolution() );
	depthDraw_ = ofxKinectNuiDrawTexture::createTextureForDepth( kinect.getDepthResolution() );
	labelDraw_ = ofxKinectNuiDrawTexture::createTextureForLabel( kinect.getDepthResolution() );
	skeletonDraw_ = new ofxKinectNuiDrawSkeleton( );
	kinect.setVideoDrawer(videoDraw_);
	kinect.setDepthDrawer(depthDraw_);
	kinect.setLabelDrawer(labelDraw_);
	kinect.setSkeletonDrawer(skeletonDraw_);

	//There are a max of 6 skeletons being tracked at any time
	for ( int i = 0 ; i < 6; i++ ) 
	{
		UserCalibrationData * data = new UserCalibrationData( ); 
		data->setup( ) ; 
		data->reset( ) ; 
		data->userId = i ; 
		userDataPool.push_back( data ) ; 
	}

	calibrationState = NO_USER ;

	calibrationWidget.setup() ; 
	calibrationWidget.kinect = &kinect ; 
	hero = NULL ;  
}

void KinectNuiManager::setupGui( ofxPanel * gui ) 
{
	gui->add( nearClipping.set( "NEAR CLIPPING" , 300 , 100 , 4000 ) ) ; 
	gui->add( farClipping.set( "FAR CLIPPING" , 1000 , 100 , 4000 ) ) ; 
	gui->add( angle.set( "ANGLE" , 1 , -30 , 30 ) ) ; 
	gui->add( offset.set( "OFFSET" , ofPoint( 500 , 500 ) , ofPoint( 1 , 1 ) , ofPoint ( ofGetWidth() , ofGetHeight() ) ) ); 

	//nearClipping.addListener( this , &KinectNuiManager::nearClipingHandler ) ; 
	//farClipping.addListener( this , &KinectNuiManager::farClippingHandler ) ; 
	angle.addListener( this , &KinectNuiManager::angleHandler ) ; 
	offset.addListener( this , &KinectNuiManager::offsetHandler ) ; 
}

void KinectNuiManager::nearClipingHandler( int & nearClip ) 
{
	kinectSource->setNearClippingDistance( nearClip ) ; 
}

void KinectNuiManager::farClippingHandler( int & farClip ) 
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
	//kinect.update() ; 
	calibrationWidget.update() ; 
	kinectSource->update();
	int id = 0 ; 

	stringstream debug ; 
	for ( auto userData = userDataPool.begin() ;  userData != userDataPool.end() ; userData++ ) 
	{
		debug << " id # " << id << " - isActive" << kinect.isTrackedSkeleton( id ) << endl ; 
		bool bTrackedStatus = (*userData)->bSkeletonActive ; 
		bool bNewStatus = kinect.isTrackedSkeleton( id ) ; 
		if ( bNewStatus != bTrackedStatus ) 
		{
			ofLogNotice() << " id # " << id << " status is differnet " ; 
			(*userData)->bSkeletonActive = bNewStatus ; 
			if ( bNewStatus == true ) 
			{
				ofLogNotice() << "new user found @ id #" << id ; 
				switch ( calibrationState ) 
				{
					case NO_USER : 
						changeState( SEARCHING_NO_HERO ) ; 
						break ; 

					//case NO_ISER
				}
			}
			else
			{
				ofLogNotice() << "user lost @ id #" << id ; 
			}
		}
		else
		{
			(*userData)->bSkeletonActive = bNewStatus ; 
			switch ( calibrationState ) 
			{
				case NO_USER : 
					//nothing
					break ; 

				case SEARCHING_NO_HERO : 
					checkForCalibration( ) ; 
					break ; 

				//Update calibrated point ( current cursor ) 
				case HERO_CALIBRATED:
					cout << " calib ID : " << userDataPool[ id ]->calibratedJointIndex << endl ; 
					cout << " id: " << id << endl ;
					cout << " - " << userDataPool[ id ]->calibrationPoint << endl ; 

					userDataPool[ id ]->calibrationPoint = kinect.skeletonPoints[ id ][ userDataPool[ id ]->calibratedJointIndex ] ; 
					break ; 

				//case NO_ISER
			}
		}
		
		(*userData)->update() ; 
		id++ ; 
	}

	//debugStream = debug.str() ; 
}

/*
NUI_SKELETON_POSITION_HEAD
NUI_SKELETON_POSITION_SHOULDER_CENTER
NUI_SKELETON_POSITION_WRIST_LEFT
NUI_SKELETON_POSITION_WAIST
*/


void KinectNuiManager::checkForCalibration( ) 
{
	stringstream ss ; 
	for ( int i = 0 ; i < 6 ; i++ ) 
	{

		if ( userDataPool[ i ]->bCalibrated == false ) 
		{
			ofPoint * pts = kinect.skeletonPoints[i] ; 

			ofPoint leftCursor = pts[ NUI_SKELETON_POSITION_HAND_LEFT ] ; 
			ofPoint rightCursor = pts[ NUI_SKELETON_POSITION_HAND_RIGHT ] ; 
			ofPoint shoulder = pts[ NUI_SKELETON_POSITION_SHOULDER_CENTER ] ; 

			ofPoint targetPoint = shoulder ; 
			bool bCalibrating = false; 
			if ( leftCursor.y < targetPoint.y ) 
			{
				targetPoint = leftCursor ; 
				userDataPool[ i ]->calibratedJointIndex = NUI_SKELETON_POSITION_HAND_LEFT ; 
				bCalibrating = true ;
				
			}
			if ( rightCursor.y < targetPoint.y ) 
			{
				targetPoint = rightCursor ; 
				userDataPool[ i ]->calibratedJointIndex = NUI_SKELETON_POSITION_HAND_RIGHT ; 
				bCalibrating = true ; 
			}

			ss << "id " << i << " targetPoint : " << targetPoint << endl ;

			if ( bCalibrating == true )
			{
				if ( userDataPool[ i ]->calibrationTimer.bIsRunning == false ) 
				{
					userDataPool[ i ]->calibrationTimer.start( false, true ) ; 
					
					ofLogNotice( ) << "ID " << i << " calibration started!" ; 
				}
			}
			else
			{
				if ( userDataPool[ i ]->calibrationTimer.bIsRunning == true ) 
				{
					userDataPool[ i ]->calibrationTimer.stop() ; 
					ofLogNotice( ) << "ID " << i << " calibration stopped!" ; 
				}
			}

			debugStream = ss.str() ; 
		
			/*for ( int k = 0 ; k < NUI_SKELETON_POSITION_COUNT ; k++ ) 
			{
				//NUI_SKELETON_POSITION_SHOULDER_CENTER
			}*/
		}
		else
		{
			hero = userDataPool[ i ] ; 
			
			changeState( HERO_CALIBRATED ) ; 
		}
	}
}

void KinectNuiManager::changeState ( CALLIBRATION_STATE state ) 
{
	if ( state == calibrationState ) 
	{
		//no transition
		return ; 
	}

	//First we transiion out
	switch ( calibrationState ) 
	{
		case NO_USER : 
				break ; 

		case SEARCHING_NO_HERO: 
			calibrationWidget.transitionOut() ; 
				break ; 

		case HERO_CALIBRATED :
				break ; 

		case HERO_LOST:
			break ; 
	}


	//Second we transiion in
	switch ( state ) 
	{
		case NO_USER : 
				break ; 

		case SEARCHING_NO_HERO: 
			calibrationWidget.transitionIn(); 
				break ; 

		case HERO_CALIBRATED :
				break ; 

		case HERO_LOST:
			break ; 
	}

	calibrationState = state ; 
}

int KinectNuiManager::getNumActiveUsers ( ) 
{
	int numActive = 0 ; 
	
	for ( auto userData = userDataPool.begin() ;  userData != userDataPool.end() ; userData++ ) 
	{
		if ( (*userData)->bSkeletonActive == true ) 
			numActive++ ; 
	}

	return numActive ; 
}

void KinectNuiManager::draw( ) 
{
	calibrationWidget.draw( ) ; 

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


	if ( calibrationState == HERO_CALIBRATED && hero != NULL ) 
	{

		//userDataPool[ i ]->calibrationPoint = 
		float _x = ofMap( hero->calibrationPoint.x , 0 , 640 , 0 , calibrationWidget.drawSize.width , true ) ; 
		float _y = ofMap( hero->calibrationPoint.y , 0 , 480 , 0 , calibrationWidget.drawSize.height , true ) ; 

		float screenX = ofMap( _x , 320 , 640 , 0 , ofGetWidth() , true  ) ; 
		float screenY = ofMap( _y , 0 , 480 , 0 , ofGetHeight() , true  ) ; 

		ofSetColor ( ofColor::yellow ) ; 
		ofCircle( screenX , screenY , 40 ) ;
	}
}

void KinectNuiManager::drawDebug( float x , float y) 
{
	stringstream ss ; 
	ss << "total active users : " << getNumActiveUsers() << endl ; 
	for ( int i = 0 ; i < 6 ; i++ ) 
	{
		ss << "user[ " << i << " ] - bSkeletonActive ? " << userDataPool[ i ]->bSkeletonActive << " - bCalibrating ?" << userDataPool[ i ]->bCalibrated << endl ; 
	}

	ofDrawBitmapStringHighlight( ss.str() , x , y ) ; 

	ofDrawBitmapStringHighlight( debugStream , x + 600 , y ) ; 
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
	videoDraw_->destroy();
	videoDraw_ = NULL;
	
	depthDraw_->destroy();
	depthDraw_ = NULL;

	labelDraw_->destroy();
	labelDraw_ = NULL;

	delete skeletonDraw_;
	skeletonDraw_ = NULL;
	
	kinect.setAngle(0);
	kinect.close();
	kinect.removeKinectListener(this);
}

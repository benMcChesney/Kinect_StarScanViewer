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

	kinectCursor.setup() ;

	heroLostTimer.setup( heroLostDuration , "hero lost timer" ) ; 
	ofAddListener( heroLostTimer.TIMER_COMPLETE , this , &KinectNuiManager::heroLostTimerComplete ) ; 

	initialHeroLostTimer.setup( 400 , "initial hero lost timer" ) ;  
	ofAddListener( initialHeroLostTimer.TIMER_COMPLETE , this , &KinectNuiManager::initialHeroLostTimerComplete ) ; 

	instructionsBlock.setup( "type/OpenSans-Bold.ttf" , 1.1 ,  "WALK UP TO EXPLORE"  , 48 , ofGetWidth() / 2 , ofGetHeight() - 175   , ofColor::white  ) ;
	instructionsBlock.alignment = ofxFontStashTextBlock::CENTER ; 

}

void KinectNuiManager::initialHeroLostTimerComplete ( int &args )
{
	heroLostTimer.start( false , true ) ;
	hero = NULL ; 
	changeState( SEARCHING_NO_HERO ) ; 

	instructionsBlock.text = "GUEST LOST - STEP BACK INTO THE ACTIVE AREA" ; 
}

void KinectNuiManager::heroLostTimerComplete ( int & args ) 
{
	changeState( NO_USER ) ;
	heroLostTimer.reset() ; 
}

void KinectNuiManager::setupGui( ofxPanel * gui ) 
{
	//gui->add( nearClipping.set( "NEAR CLIPPING" , 300 , 100 , 4000 ) ) ; 
	//agui->add( farClipping.set( "FAR CLIPPING" , 1000 , 100 , 4000 ) ) ; 
	gui->add( angle.set( "ANGLE" , 1 , -30 , 30 ) ) ; 
	gui->add( offset.set( "OFFSET" , ofPoint( 500 , 500 ) , ofPoint( 1 , 1 ) , ofPoint ( ofGetWidth() , ofGetHeight() ) ) ); 

	gui->add( cursorRegionDims.set( "CURSOR REGION SIZE" , ofPoint( 50 ) , ofPoint( 0 ) , ofPoint ( 200 ) ) ) ; 
	gui->add( interpolateTime.set( "INTERPOLATE TIME" , 0.2f , 0.01 , .5f ) ) ; 
	gui->add( heroLostDuration.set ( "HERO LOST DURATION" , 5000.0f , 100.0f , 15000.0f ) ) ; 
	gui->add( initialHeroTimeoutDuration.set( "INIT HERO LOST (MS)" , 500.0f , 100.0f ,8000.0f ) ) ; 
	
	angle.addListener( this , &KinectNuiManager::angleHandler ) ; 
	offset.addListener( this , &KinectNuiManager::offsetHandler ) ; 
}

void KinectNuiManager::nearClipingHandler( int & nearClip ) 
{
	//


	//kinectSource->setNearClippingDistance( nearClip ) ; 
}

void KinectNuiManager::farClippingHandler( int & farClip ) 
{
	

	//kinectSource->setFarClippingDistance( farClip ) ; 
	//
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
	
		//kinectSource->setFarClippingDistance(farClipping);
		//kinectSource->setNearClippingDistance(nearClipping);
	


	//kinect.setNearClippingDistance( nearClipping ) ; 
	//kinect.setFarClippingDistance( farClipping ) ; 
	//kinect.update() ; 
	heroLostTimer.delayMillis = heroLostDuration ; 
	heroLostTimer.update( ) ; 

	instructionsBlock.alphaStackUpdate( 1.0f ); 

	initialHeroLostTimer.delayMillis = initialHeroTimeoutDuration ; 
	calibrationWidget.update() ; 
	kinectSource->update();
	int id = 0 ; 
	int numCalibrated = 0 ; 

	stringstream debug ; 
	for ( auto userData = userDataPool.begin() ;  userData != userDataPool.end() ; userData++ ) 
	{
		debug << " id # " << id << " - isActive" << kinect.isTrackedSkeleton( id ) << endl ; 
		bool bActiveStatus = (*userData)->bSkeletonActive ; 
		bool bNewActiveStatus = kinect.isTrackedSkeleton( id ) ; 

		if ( (*userData)->bCalibrated == true ) 
			numCalibrated++ ; 

		if ( bNewActiveStatus != bActiveStatus ) 
		{
			ofLogNotice() << " id # " << id << " status is differnet " ; 
			(*userData)->bSkeletonActive = bNewActiveStatus ; 
			if ( bNewActiveStatus == true ) 
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
				(*userData)->reset() ; 
				//heroLostTimer.start( false , true ) ; 
				heroLostTimer.start( false , true ) ;
				hero = NULL ; 
				changeState( SEARCHING_NO_HERO ) ; 
			}
		}
		else
		{
			(*userData)->bSkeletonActive = bNewActiveStatus ; 
			if ( (*userData)->bSkeletonActive == true  ) 
			{
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
						userDataPool[ id ]->calibrationPoint = kinect.skeletonPoints[ id ][ userDataPool[ id ]->calibratedJointIndex ] ; 
						break ; 

					//case NO_ISER
				}
			}
			else
			{
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
						//cout << " calib ID : " << userDataPool[ id ]->calibratedJointIndex << endl ; 
						//cout << " id: " << id << endl ;
						//cout << " - " << userDataPool[ id ]->calibrationPoint << endl ; 
						userDataPool[ id ]->calibrationPoint = kinect.skeletonPoints[ id ][ userDataPool[ id ]->calibratedJointIndex ] ; 
						break ; 

					//case NO_ISER
				}
			}
		}
		
		(*userData)->update() ; 
		id++ ; 
	}

	if ( calibrationState == SEARCHING_NO_HERO ) 
	{
		instructionsBlock.text = "STEP UP TO EXPLORE" ; 
		if ( numCalibrated > 0 ) 
		{
			changeState( HERO_LOST ) ; 
			instructionsBlock.text = "GUEST LOST... STEP UP TO CONTINUE" ; 
		}

		
	}

	if ( calibrationState == HERO_CALIBRATED && numCalibrated == 0 ) 
	{
		ofLogNotice() << "HERO CALIBRATED - but no users" ; 
		//initialLostTimer.start( false, true ) ; 
	}
	if ( hero != NULL ) 
	{
		kinectCursor.normalizedScreenPosition = ofPoint ( ofMap(  hero->calibrationPoint.x , cursorRegionOrigin.x - cursorRegionDims.get().x/2 , cursorRegionOrigin.x + cursorRegionDims.get().x/2 , 0.0f , 1.0f , true ) ,
												   ofMap( hero->calibrationPoint.y , cursorRegionOrigin.y - cursorRegionDims.get().y/2 , cursorRegionOrigin.y + cursorRegionDims.get().y/2 , 0.0f , 1.0f , true ) ) ; 			

		kinectCursor.worldPosition = hero->calibrationPoint ; 
		
		float screenX = ofMap( hero->calibrationPoint.x , cursorRegionOrigin.x - cursorRegionDims.get().x/2 , cursorRegionOrigin.x + cursorRegionDims.get().x/2 , 0 , ofGetWidth() , true  ) ; 
		float screenY = ofMap( hero->calibrationPoint.y , cursorRegionOrigin.y - cursorRegionDims.get().y/2 , cursorRegionOrigin.y + cursorRegionDims.get().y/2 , 0 , ofGetHeight() , true  ) ; 
		Tweenzor::add( &kinectCursor.screenPosition.x , kinectCursor.screenPosition.x , screenX, 0.0f, interpolateTime , EASE_OUT_QUAD ) ; 
		Tweenzor::add( &kinectCursor.screenPosition.y , kinectCursor.screenPosition.y , screenY, 0.0f, interpolateTime , EASE_OUT_QUAD ) ; 

		instructionsBlock.text = "EXPLORE DIFFERENT DATA SETS" ; 
	}
	
}

void KinectNuiManager::calibrateCursorRegion ( UserCalibrationData * data )
{
	if ( hero == NULL ) 
	{
		ofLogNotice() << "calibrateCursorRegion - somehow HERO is null. aborting!" ; 
		return ; 
	}

	ofPoint * pts = kinect.skeletonPoints[ data->userId ] ; 
	
	ofPoint leftCursor = pts[ NUI_SKELETON_POSITION_HAND_LEFT ] ; 
	ofPoint rightCursor = pts[ NUI_SKELETON_POSITION_HAND_RIGHT ] ; 
	ofPoint chest = pts[ NUI_SKELETON_POSITION_SPINE ] ;

	cursorRegionOrigin = ofPoint ( hero->calibrationPoint.x , chest.y ) ; 

	ofLogNotice() << "cursor region origin calibrated @ " << cursorRegionOrigin ; 
}

void KinectNuiManager::checkForCalibration( ) 
{
	stringstream ss ; 
	for ( int i = 0 ; i < 6 ; i++ ) 
	{

		if ( userDataPool[ i ]->bCalibrated == false ) 
		{
			instructionsBlock.text = "RAISE A HAND ABOVE YOUR HEAD TO CALIBRATE" ; 
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
				userDataPool[ i ]->calibrationPoint = kinect.skeletonPoints[ i ][ userDataPool[ i ]->calibratedJointIndex ] ; 
				bCalibrating = true ;
				
			}
			if ( rightCursor.y < targetPoint.y ) 
			{
				targetPoint = rightCursor ; 

				userDataPool[ i ]->calibratedJointIndex = NUI_SKELETON_POSITION_HAND_RIGHT ; 
				userDataPool[ i ]->calibrationPoint = kinect.skeletonPoints[ i ][ userDataPool[ i ]->calibratedJointIndex ] ; 
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
		}
		else
		{
			hero = userDataPool[ i ] ; 

			calibrateCursorRegion( hero ) ; 
			changeState( HERO_CALIBRATED ) ; 
			instructionsBlock.text = "MOVE YOUR CURSOR OVER THE CHANGE COMPARISON" ; 
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

		float scale = 3.0; 
		int w = 640 / scale ; 
		int h = 480 / scale ; 
		kinect.drawVideo( 0, 0, w, h );
		
		kinect.drawDepth( 0, 0, w, h );
		ofSetColor( 255 , 128 ) ; 
		kinect.drawLabel( 0, 0, w, h );
		ofSetColor( 255 ) ; 
		kinect.drawSkeleton(0 ,0, w, h);

		float iScale = 1.0f / scale ; 
		if ( calibrationState == HERO_CALIBRATED ) 
		{
			
			ofPushMatrix() ; 
				ofSetColor( ofColor::yellow ) ; 
				ofCircle( kinectCursor.worldPosition.x * iScale , kinectCursor.worldPosition.y * iScale , 14 ); 
			ofPopMatrix( ) ; 
		}

		if ( hero != NULL ) 
		{
			ofPushMatrix() ; 
				//ofScale( 1/scale , 1/scale ) ; 
				ofTranslate( (cursorRegionOrigin.x - cursorRegionDims.get().x/2) * iScale , 
							 (cursorRegionOrigin.y - cursorRegionDims.get().y/2) * iScale  ) ; 
				ofPushStyle() ; 
					ofNoFill() ; 
					ofSetLineWidth( 3 ) ; 
					ofSetColor( ofColor::cyan ) ; 

					ofRect(	0 , 0 , cursorRegionDims.get().x * iScale, cursorRegionDims.get().y * iScale ) ; 
					ofFill() ; 
					
				ofPopStyle() ; 
			ofPopMatrix() ; 
		}	

	ofPopMatrix() ; 

	ofSetColor( 15 , 200 ) ; 
	 w = instructionsBlock.getWidth()  ; 
	 h =  instructionsBlock.getHeight() * 1.5 ; //instructionsBlock.lineSpacing ;  
	float padding = 0 ; //	 40 ; //1 ; //.25 ; 
	ofRectangle r = ofRectangle( instructionsBlock.x - (padding/2), 
							 	 instructionsBlock.y - (padding/2),
								 w + padding, 
								 h + padding ) ; 

	
	ofPushStyle( ) ; 
		ofSetRectMode( OF_RECTMODE_CENTER ) ;
		ofRect( r )  ; 

	ofPopStyle() ; 
	ofSetColor( 255 ) ; 
	ofPushMatrix( ) ; 
	ofTranslate( instructionsBlock.getWidth() / 2 , 0 ) ; 
	instructionsBlock.draw( ) ; 

	ofPopMatrix() ; 

	if ( hero != NULL ) 
	{
		kinectCursor.draw() ;
	}
}

void KinectNuiManager::clearAllUsers() 
{
	for ( auto user = userDataPool.begin() ; user != userDataPool.end() ; user++ ) 
	{
		(*user)->reset() ; 
	}

	changeState( SEARCHING_NO_HERO ) ; 
}

void KinectNuiManager::drawDebug( float x , float y) 
{
	stringstream ss ; 
	ss << "total active users : " << getNumActiveUsers() << endl ; 
	for ( int i = 0 ; i < 6 ; i++ ) 
	{
		ss << "user[ " << i << " ] - bSkeletonActive ? " << userDataPool[ i ]->bSkeletonActive << " - bCalibrating ?" << userDataPool[ i ]->bCalibrated << endl ; 
	}

	ss << "CALIBRATION STATE " << getStringFromCalibrationState( calibrationState ) << endl ; 

	string heroString = "HERO IS NULL" ; 

	if ( hero != NULL ) 
	{
		heroString = "HERO active!" ; 
		ss << "TARGETPOINT : " << hero->calibrationPoint << endl ;
	}
	ss << heroString << endl; 
	
	ofDrawBitmapStringHighlight( ss.str() , x , y ) ; 

	ofDrawBitmapStringHighlight( debugStream , x + 600 , y ) ; 

	ofSetColor( 255 ) ; 
	//kinect.drawDepth( 400 , 400 , 640, 480 ) ; 
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

string KinectNuiManager::getStringFromCalibrationState ( CALLIBRATION_STATE state ) 
{
	switch ( state ) 
	{
		case NO_USER : 
			return "NO_USER" ;
			break ; 

		case SEARCHING_NO_HERO :
			return "SEARCHING NO HERO" ; 
			break ; 

		case HERO_CALIBRATED : 
			return "HERO CALIBRATED" ; 
			break ; 

		case HERO_LOST : 
			return "HERO LOST" ; 
			break ; 

		default : 
			return "DEFAULT" ; 
			break ;
	}
}
	

#pragma once

#include "ofMain.h"
#include "ofxJSONElement.h"
#include "DataSyncManager.h"
#include "Tweenzor.h"
#include "ImageCompareView.h"
#include "ofxGui.h"
#include "KinectNuiManager.h"
#include "ThumbnailWidget.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxJSONElement spectrumJson ; 
		DataSyncManager dataSyncManager ; 
		ImageCompareView imageCompareView ; 

		ofParameter<bool> bDrawDebug ;
		ofParameter<float> thumbnailDragSize ; 
		bool bShowGui ; 

		//Gui things
		ofxPanel gui;

		float initialTweenDelay ; 
		void initialTweenDelayComplete ( float * args ) ; 
		
		KinectNuiManager kinectManager ;

		void checkKinectInput( ThumbnailWidget * thumbnail , ofPoint screenPos ) ; 
		
};

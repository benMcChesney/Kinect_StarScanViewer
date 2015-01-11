#pragma once

#include "ofMain.h"
#include "ofxLoadingManager.h"
#include "ofxJSONElement.h"
#include "AtomosphericImageData.h"

class DataSyncManager
{
public : 
	DataSyncManager() ;

	void setup( string directoryPath ) ; 
	void update () ; 
	void draw( ) ; 

	void loadFromJsonRef( ofxJSONElement  data ) ;

	ofxLoadingManager loadingManager ; 
	vector < AtmosphericImageData * > atmosphericImageData ; 
};
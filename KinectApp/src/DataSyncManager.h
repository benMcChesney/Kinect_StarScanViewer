/*
	Written by Ben McChesney 1/10/2015

	parses JSON and caches remote resources to disk using LoadingManager

*/
#pragma once

#include "ofMain.h"
#include "ofxLoadingManager.h"
#include "ofxJSONElement.h"
#include "AtmosphericImageData.h"

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
#include "DataSyncManager.h"

DataSyncManager::DataSyncManager( ) 
{
	
}

void DataSyncManager::setup( string directoryPath  ) 
{
	loadingManager.setup( directoryPath ) ; 
}

void DataSyncManager::loadFromJsonRef( ofxJSONElement data )
{
	//ofxLoadingManager loadingManager ; 
	int numSpectrums = data[ "spectrums" ].size() ; 
	ofLogNotice() << "loadFromJsonRef - # spectrums " << numSpectrums << endl ; 
	
	string resolution = "1024" ; 
	string baseUrl = "http://sdo.gsfc.nasa.gov/assets/img/latest/latest_" + resolution + "_" ; 

	
	for ( int i = 0 ; i < numSpectrums ; i++ ) 
	{
		string label = data[ "spectrums" ][i]["label"].asString(); 
		int aia_id = data[ "spectrums" ][i]["id"].asInt() ; 
		string description = data[ "spectrums" ][i]["description"].asString() ; 
		
		string imagePath = ofToString( aia_id ) + ".jpg" ; 
		if ( aia_id < 1000 ) 
			imagePath = "0" + ofToString( aia_id ) +".jpg" ; 
		if ( aia_id < 100 ) 
			imagePath = "00" + ofToString( aia_id ) +".jpg" ; 

		string url = baseUrl + imagePath ; 
		ofLogNotice() << "#" << i << " " << label ; 

		atmosphericImageData.push_back( new AtmosphericImageData(	label , aia_id , description , url ) ) ; 
		atmosphericImageData[i]->image.setImageProperties( loadingManager.remoteUrlToLocal( url ) , 0 , 0 , ofPoint ( 0.5 , 0.5 ) , 1.0f ) ; 

		loadingManager.loadURL( url ) ; 
		loadingManager.addToThreadedImageQueue( atmosphericImageData[i]->image.image , atmosphericImageData[i]->url , true , false ) ; 
	}
	
}

void DataSyncManager::update( ) 
{
	loadingManager.update( ) ; 
}

void DataSyncManager::draw( ) 
{
	loadingManager.draw( ) ; 
}

	

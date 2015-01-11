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
	
	for ( int i = 0 ; i < numSpectrums ; i++ ) 
	{
		//Parse each field
		string label = data[ "spectrums" ][i]["label"].asString(); 
		string description = data[ "spectrums" ][i]["description"].asString() ; 
		string location = data[ "spectrums" ][i]["location"].asString() ;
		string wavelength = data[ "spectrums" ][i]["primaryIons"].asString() ; 
		string temperatureKelvin = data[ "spectrums" ][i]["temperatureKelvin"].asString() ; 
		string temperatureFahrenheight = data[ "spectrums" ][i]["temperatureFahrenheight"].asString() ; 
		string assetUrl = data[ "spectrums" ][i]["assetUrl"].asString(); 

		ofLogNotice() << "#" << i << " " << label ; 

		atmosphericImageData.push_back( new AtmosphericImageData(	label , description , location , wavelength , temperatureKelvin, temperatureFahrenheight , assetUrl  ) ) ; 
		atmosphericImageData[i]->image.setImageProperties( loadingManager.remoteUrlToLocal( assetUrl ) , 0 , 0 , ofPoint ( 0.5 , 0.5 ) , 1.0f ) ; 

		loadingManager.loadURL( assetUrl ) ; 
		loadingManager.addToThreadedImageQueue( atmosphericImageData[i]->image.image , atmosphericImageData[i]->assetUrl , true , false ) ; 
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

	

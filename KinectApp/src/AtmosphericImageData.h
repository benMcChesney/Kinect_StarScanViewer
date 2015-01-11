/*
	Written by Ben McChesney 1/10/2015

	A class for storing data associated with a spectrum object cached from the NASA website

*/

#pragma once

#include "ofMain.h"
#include "ofxMatrixTransformImage.h"

class AtmosphericImageData
{
public : 
	AtmosphericImageData( ) { }
	AtmosphericImageData( string _label , int _aia_id , string _description , string _url ) 
	{
		label = _label ; 
		aia_id = _aia_id ; 
		description = _description ;
		url = _url ; 
	}
	
	string label ; 
	int aia_id ; 
	string url ; 
	string description ; 
	ofxMatrixTransformImage image ; 

	void reloadTextureFromDisk() ; 
};
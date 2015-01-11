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
	AtmosphericImageData(	string _label , string _description , string _location , string _primaryIons ,
							string _temperatureFahrenheight , string _temperatureKelvin , string _assetUrl ) 
	{
		label = _label ; 
		description = _description ; 
		location = _location ; 
		primaryIons = _primaryIons  ; 
		temperatureFahrenheight = _temperatureFahrenheight ; 
		temperatureKelvin = _temperatureKelvin ; 
		assetUrl = _assetUrl ; 
	}
	
	string label ; 
	string description ; 
	string location ; 
	string wavelength ; 
	string primaryIons ; 
	string temperatureKelvin ;
	string temperatureFahrenheight ; 
	string assetUrl ; 

	ofxMatrixTransformImage image ; 

	void reloadTextureFromDisk() ; 
};
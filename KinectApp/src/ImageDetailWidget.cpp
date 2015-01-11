#include "ImageDetailWidget.h"

void ImageDetailWidget::setup ( ) 
{
	ofxAlphaStackItem::setup() ; 
	ofxMatrixTransformObject::setup() ; 

	detailImage = new ofxMatrixTransformImage() ; 
	detailImage->setup() ; 
	
	//Setup all text fields
	float startY =  ofGetHeight() * .3 ; 
	float ySpacing = 20 ; 
	title.setup( "type/OpenSans-Bold.ttf" , 1.2 , "TEST TITLE" , 64 , 15 , startY , ofColor::white ) ;
	title.draw() ; 

	description.setup( "type/OpenSans-Light.ttf" , 1.1 ,  "n/a"  , 24 , 15  , startY + 35 + title.getHeight()  , ofColor::white ) ;
	location.setup( "type/OpenSans-Regular.ttf" , 1.1 ,  "n/a"  , 32 , 15  , startY + 35 + title.getHeight()  , ofColor::white ) ;
	wavelength.setup( "type/OpenSans-Regular.ttf" , 1.1 , "n/a" , 32 , 15  , startY + 35 + title.getHeight()  , ofColor::white )  ;
	primaryIons.setup( "type/OpenSans-Regular.ttf" , 1.1 ,  "n/a"  , 32 , 15  , startY + 35 + title.getHeight()  , ofColor::white ) ;
	temperatureKelvin.setup( "type/OpenSans-Regular.ttf" , 1.1 ,  "n/a"  , 32 , 15  , startY + 35 + title.getHeight()  , ofColor::white ) ;
	temperatureFahrenheight.setup( "type/OpenSans-Regular.ttf" , 1.1 ,  "n/a"  , 32 , 15  , startY + 35 + title.getHeight()  , ofColor::white ) ;

	//Setup mask
	maskFbo.allocate( 1024 , 1024 ) ;

	maskFbo.begin() ; 
	ofClear( 1 , 1 , 1 , 0 ) ; 
	maskFbo.end() ; 
	
	alpha = 1.0f ; 
	maskAlpha = 0.0f ; 

	float marginWidth = ( ofGetWidth() - 1024 )  / 2 ; 
	marginWidth -= 20 ; 

	description.wrapTextX( marginWidth ) ; 

	delayIncrement = 0.1f ; 
	transitionDuration = 0.4f ; 
}

void ImageDetailWidget::update( ) 
{
	detailImage->alphaStackUpdate( alpha ) ; 
	title.alphaStackUpdate( alpha ) ; 
	description.alphaStackUpdate( alpha ) ; 
	location.alphaStackUpdate( alpha ) ; 
	wavelength.alphaStackUpdate( alpha ) ; 
	primaryIons.alphaStackUpdate( alpha ) ; 
	temperatureKelvin.alphaStackUpdate( alpha ) ; 
	temperatureFahrenheight.alphaStackUpdate( alpha ) ; 
}

void ImageDetailWidget::draw( ) 
{
	pushMatrix() ; 

		title.draw( ) ; 
		description.draw( ) ;
		location.draw( ) ; 
		wavelength.draw( ) ; 
		primaryIons.draw( ) ;
		temperatureKelvin.draw( ) ; 
		temperatureFahrenheight.draw( ) ;
	
	popMatrix() ; 
}

void ImageDetailWidget::drawDebug( ) 
{

}

void ImageDetailWidget::populateFromData( AtmosphericImageData data ) 
{
	title.text = data.label ; 
	description.text = data.description ; 
	detailImage->image.setFromPixels( data.image.image.getPixelsRef() ) ; 
	location.text = data.location ; 
	wavelength.text = data.wavelength ; 
	primaryIons.text = data.primaryIons ; 
	temperatureKelvin.text = data.temperatureKelvin ; 
	temperatureFahrenheight.text = data.temperatureFahrenheight ; 

	float marginWidth = ( ofGetWidth() - 1024 )  / 2 ; 
	marginWidth -= 20 ; 

	title.wrapTextX( marginWidth ) ; 
	description.wrapTextX( marginWidth ) ; 
	location.wrapTextX( marginWidth ) ; 
	primaryIons.wrapTextX( marginWidth ) ; 
	temperatureKelvin.wrapTextX( marginWidth ) ; 
	temperatureFahrenheight.wrapTextX( marginWidth ) ;  

	/*
	title 
	location
	description
	wavelength
	primaryIons
	temperatureKelvin
	temperatureFahrenheight
	*/
}

void ImageDetailWidget::transitionIn() 
{
	float startY =  ofGetHeight() * .3 ; 
	float ySpacing = 20 ; 

	title.y = startY ; 
	description.y = ySpacing + title.y + title.getHeight() ;
	location.y = ySpacing + description.y + description.getHeight() ;
	wavelength.y = ySpacing + location.y + location.getHeight()  ;
	primaryIons.y = ySpacing + wavelength.y + wavelength.getHeight()   ;
	temperatureKelvin.y = ySpacing + primaryIons.y + primaryIons.getHeight()  ;
	temperatureFahrenheight.y = ySpacing + temperatureKelvin.y + temperatureKelvin.getHeight()  ;

	Tweenzor::add( &maskAlpha , 0.0f , 1.0f , 0.0f , transitionDuration , EASE_OUT_QUAD ) ; 

	Tweenzor::add( &title.alpha , 0.0f , 1.0f , 0.0f , transitionDuration , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &title.y , title.y , title.y + 20  , 0.0f , transitionDuration , EASE_OUT_QUAD ) ; 

	Tweenzor::add( &description.alpha , 0.0f , 1.0f , delayIncrement , transitionDuration , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &description.y , title.y-20 , title.y , delayIncrement, transitionDuration , EASE_OUT_QUAD ) ; 

	Tweenzor::add( &location.alpha , location.alpha , 1.0f , delayIncrement * 2.0f , transitionDuration , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &location.y , location.y , location.y - 20 , delayIncrement * 2.0f , transitionDuration , EASE_OUT_QUAD ) ; 

	Tweenzor::add( &wavelength.alpha , wavelength.alpha , 1.0f , delayIncrement * 3.0f , transitionDuration , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &wavelength.y , wavelength.y , wavelength.y - 20 , delayIncrement * 3.0f , transitionDuration , EASE_OUT_QUAD ) ; 

	Tweenzor::add( &primaryIons.alpha , primaryIons.alpha , 1.0f , delayIncrement * 4.0f , transitionDuration , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &primaryIons.y , primaryIons.y , primaryIons.y - 20 , delayIncrement * 4.0f , transitionDuration , EASE_OUT_QUAD ) ; 

	Tweenzor::add( &temperatureKelvin.alpha , temperatureKelvin.alpha , 1.0f , delayIncrement * 5.0f , transitionDuration , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &temperatureKelvin.y , temperatureKelvin.y , temperatureKelvin.y - 20 , delayIncrement * 5.0f , transitionDuration , EASE_OUT_QUAD ) ; 

	Tweenzor::add( &temperatureFahrenheight.alpha , temperatureFahrenheight.alpha , 1.0f , delayIncrement * 6.0f , transitionDuration , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &temperatureFahrenheight.y , temperatureFahrenheight.y , temperatureFahrenheight.y - 20 , delayIncrement * 6.0f , transitionDuration , EASE_OUT_QUAD ) ; 

}

void ImageDetailWidget::transitionOut()
{
	float startY =  ofGetHeight() * .3 ; 
	float ySpacing = 20 ;

	title.y = startY ; 
	description.y = ySpacing + title.y + title.getHeight() ;
	location.y = ySpacing + description.y + description.getHeight() ;
	wavelength.y = ySpacing + location.y + location.getHeight()  ;
	primaryIons.y = ySpacing + wavelength.y + wavelength.getHeight()   ;
	temperatureKelvin.y = ySpacing + primaryIons.y + primaryIons.getHeight()  ;
	temperatureFahrenheight.y = ySpacing + temperatureKelvin.y + temperatureKelvin.getHeight()  ;


	Tweenzor::add( &maskAlpha , 1.0f , 0.0f , 0.0f , transitionDuration , EASE_OUT_QUAD ) ; 

	Tweenzor::add( &title.alpha , title.alpha , 0.0f , 0.0f , transitionDuration , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &title.y , title.y , title.y - 20 , 0.0f , transitionDuration , EASE_OUT_QUAD ) ; 

	Tweenzor::add( &description.alpha , description.alpha , 0.0f , delayIncrement * 1.0f , transitionDuration , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &description.y , description.y , description.y - 20 , delayIncrement * 1.0f , transitionDuration , EASE_OUT_QUAD ) ; 

	Tweenzor::add( &location.alpha , location.alpha , 0.0f , delayIncrement * 2.0f , transitionDuration , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &location.y , location.y , location.y - 20 , delayIncrement * 2.0f , transitionDuration , EASE_OUT_QUAD ) ; 

	Tweenzor::add( &wavelength.alpha , wavelength.alpha , 0.0f , delayIncrement * 3.0f , transitionDuration , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &wavelength.y , wavelength.y , wavelength.y - 20 , delayIncrement * 3.0f , transitionDuration , EASE_OUT_QUAD ) ; 

	Tweenzor::add( &primaryIons.alpha , primaryIons.alpha , 0.0f , delayIncrement * 4.0f , transitionDuration , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &primaryIons.y , primaryIons.y , primaryIons.y - 20 , delayIncrement * 4.0f , transitionDuration , EASE_OUT_QUAD ) ; 

	Tweenzor::add( &temperatureKelvin.alpha , temperatureKelvin.alpha , 0.0f , delayIncrement * 5.0f , transitionDuration , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &temperatureKelvin.y , temperatureKelvin.y , temperatureKelvin.y - 20 , delayIncrement * 5.0f , transitionDuration , EASE_OUT_QUAD ) ; 

	Tweenzor::add( &temperatureFahrenheight.alpha , temperatureFahrenheight.alpha , 0.0f , delayIncrement * 6.0f , transitionDuration , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &temperatureFahrenheight.y , temperatureFahrenheight.y , temperatureFahrenheight.y - 20 , delayIncrement * 6.0f , transitionDuration , EASE_OUT_QUAD ) ; 

}

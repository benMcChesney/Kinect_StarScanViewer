#include "ImageDetailWidget.h"

/*
ofxMatrixTransformImage * detailImage ; 
	ofxFontStashTextBlock title ; 
	ofxFontStashTextBlock description ; 

	*/

void ImageDetailWidget::setup ( ) 
{
	ofxAlphaStackItem::setup() ; 
	ofxMatrixTransformObject::setup() ; 

	detailImage = new ofxMatrixTransformImage() ; 
	detailImage->setup() ; 
	
	float startY =  ofGetHeight() * .4 ; 
	title.setup( "type/OpenSans-Bold.ttf" , 1.2 , "TEST TITLE" , 64 , 15 , startY , ofColor::white ) ;
	description.setup( "type/OpenSans-Regular.ttf" , 1.1 , "here is some test, this is a test thi sis the best test that we could have." , 32 , 15  , startY + 35 + title.getHeight()  , ofColor::white ) ;
	maskFbo.allocate( 1024 , 1024 ) ;

	maskFbo.begin() ; 
	ofClear( 1 , 1 , 1 , 0 ) ; 
	maskFbo.end() ; 
	
	alpha = 1.0f ; 

	float marginWidth = ( ofGetWidth() - 1024 )  / 2 ; 
	marginWidth -= 20 ; 

	description.wrapTextX( marginWidth ) ; 
}

void ImageDetailWidget::update( ) 
{
	detailImage->alphaStackUpdate( alpha ) ; 
	title.alphaStackUpdate( alpha ) ; 
	description.alphaStackUpdate( alpha ) ; 
}

void ImageDetailWidget::draw( ) 
{
	pushMatrix() ; 

	title.draw( ) ; 
	description.draw( ) ;
	
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

	float marginWidth = ( ofGetWidth() - 1024 )  / 2 ; 
	marginWidth -= 20 ; 

	description.wrapTextX( marginWidth ) ; 
}

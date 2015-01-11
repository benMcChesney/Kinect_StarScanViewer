#include "ImageDetailView.h"

/*
ofxMatrixTransformImage * detailImage ; 
	ofxFontStashTextBlock title ; 
	ofxFontStashTextBlock description ; 

	*/

void ImageDetailView::setup ( ) 
{
	detailImage = new ofxMatrixTransformImage() ; 
	detailImage->setup() ; 
	
	title.setup( "type/OpenSans-Bold.ttf" , 1.2 , "TEST TITLE" , 32 , 50 , ofGetHeight() * .4 , ofColor::white ) ;
	description.setup( "type/OpenSans-Regular.ttf" , 1.2 , "here is some test, this is a test thi sis the best test that we could have." , 32 , 50 , ofGetHeight() * .4 , ofColor::white ) ;
}
void ImageDetailView::update( ) 
{
	detailImage->alphaStackUpdate( alpha ) ; 
	title.alphaStackUpdate( alpha ) ; 
	description.alphaStackUpdate( alpha ) ; 
}

void ImageDetailView::draw( ) 
{
	title.draw( ) ; 
	description.draw( ) ;
}

void ImageDetailView::drawDebug( ) 
{

}

#include "ImageDetailView.h"



void ImageDetailView::setup ( ) 
{
	detailImage = new ofxMatrixTransformImage() ; 
	detailImage->setup() ; 
}
void ImageDetailView::update( ) 
{
	detailImage->alphaStackUpdate( alpha ) ; 
}

void ImageDetailView::draw( ) 
{

}

void ImageDetailView::drawDebug( ) 
{

}

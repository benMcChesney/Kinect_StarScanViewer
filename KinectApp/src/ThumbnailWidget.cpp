#include "ThumbnailWidget.h"

ThumbnailWidget::ThumbnailWidget( ) 
{

}

void ThumbnailWidget::setup( ) 
{
	KinectButtonObject::setup( ) ; 
	ofxMatrixTransformObject::setup() ; 
	ofxAlphaStackItem::setup() ; 

	thumbWidth = 50 ; 
	labelBlock.setup( "type/OpenSans-Bold.ttf" , 1.1 ,  data->label  , 24 , 0  , 0 , ofColor::white  ) ;
	labelBlock.alignment = ofxFontStashTextBlock::CENTER ; 

	ofAddListener( hoverTimer.TIMER_COMPLETE , this , &ThumbnailWidget::hoverTimerComplete ) ; 
	
}

void ThumbnailWidget::update( ) 
{
	KinectButtonObject::update( ) ; 
	labelBlock.alphaStackUpdate( alpha ) ; 

}

void ThumbnailWidget::draw( ) 
{
	KinectButtonObject::draw( ) ; 


	pushMatrix() ; 
		float _scale =   ( thumbWidth / data->image.getWidth() ) ;  
		ofPushMatrix(); 
			ofScale( _scale, _scale ) ; 
			ofSetColor( ofColor::white , getOFAlpha() ) ;

			data->image.draw() ; 
		ofPopMatrix(); 

		ofSetColor( labelBlock.color , labelBlock.getOFAlpha() ) ; 
		labelBlock.draw( 0 , ( data->image.getWidth() * _scale ) / -2 ) ; 

	popMatrix() ; 


	//if ( !bHoverSelected ) 
	//{
	ofPushStyle() ; 
		ofNoFill() ; 
		ofSetColor( 255 , hoverTimer.getNormalizedProgress() * 0.5f * 255.0f ) ; 
		if ( bHoverSelected ) 
			ofSetColor( 255 , 128.0f ) ; 

		ofSetLineWidth( 2 ) ; 
		ofRect( hitArea ) ; 
	ofPopStyle() ; 
	//}
}

void ThumbnailWidget::hoverTimerComplete ( int &args )
{
	ofLogNotice() << " hover timer complete on : " << data->label << endl ; 
	hoverTimer.reset() ;
	bHoverSelected = true ; 
	ofNotifyEvent( StarScanViewerEvents::getInstance().THUMBNAIL_SELECTED , data->label ) ; 
}

void ThumbnailWidget::transitionIn ( ) 
{
	KinectButtonObject::reset( ); 
}

void ThumbnailWidget::transitionOut( ) 
{

}
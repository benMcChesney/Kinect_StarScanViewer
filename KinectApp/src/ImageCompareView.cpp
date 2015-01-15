#include "ImageCompareView.h"

void ImageCompareView::setup ( )
{
	mask.setup( "shaders/composite_rgb" , ofRectangle( 0 , 0 , 1024, 1024 ) ) ; 
	leftView.setup() ; 
	rightView.setup() ; 

	float marginWidth = ( ofGetWidth() - 1024 ) / 2 ; 
	rightView.x = ofGetWidth() - marginWidth ; 

	ofAddListener( StarScanViewerEvents::getInstance().THUMBNAIL_SELECTED , this , &ImageCompareView::thumbnailSelectionEvent ) ; 
}
	
void ImageCompareView::update( ) 
{
	normalizedAmount = ofMap(  sin( ofGetElapsedTimef() ) , -1.0f , 1.0f , 0.0f , 1.0f , true )  ; 
	leftView.update() ; 
	leftView.alphaStackUpdate( 1.0f ) ; 
	rightView.update( ) ;
	rightView.alphaStackUpdate( 1.0f ) ; 

	for ( auto thumbnail = thumbnails.begin() ; thumbnail != thumbnails.end() ; thumbnail++ ) 
	{
		(*thumbnail)->update() ; 
		(*thumbnail)->alphaStackUpdate( 1.0f ) ; 
	}
}
	
void ImageCompareView::draw( ) 
{
	//Draw Left active area
	leftView.maskFbo.begin() ; 
		ofSetColor( ofColor::black ) ; 
		ofRect( 0 , 0 , 1024.0f , ofGetHeight() ) ; 
		ofSetColor( ofColor::white ) ; 
		ofRect( 0 , 0 , 1024.0f * normalizedAmount , ofGetHeight() ) ; 
	leftView.maskFbo.end() ; 

	//Draw Right active area
	rightView.maskFbo.begin( ) ; 
		//float _y = ofGetHeight() /2 ; 
		ofSetColor( ofColor::black ) ; 
		ofRect( 0 , 0 , 1024.0f , ofGetHeight() ) ; 
		ofSetColor( ofColor::white ) ; 
		ofRect( 1024.0f * normalizedAmount  , 0 , 1024.0f * ( 1.0f - normalizedAmount ) , ofGetHeight() ) ; 
	rightView.maskFbo.end( ) ; 
	
	float marginWidth = ( ofGetWidth() - 1024 ) / 2 ; 
	ofPushMatrix() ; 
		float offsetX = marginWidth ;
		ofTranslate( offsetX , 0  ) ; 
		ofScale( 0.93, 0.93 ) ; 

		//Prevent spamming error messages
		if ( rightView.detailImage->image.bAllocated() ) 
			mask.drawMask( rightView.detailImage->image.getTextureReference() , rightView.maskFbo.getTextureReference() , 0 , 0 , rightView.maskAlpha ) ; 

		//Prevent spamming error messages
		if ( leftView.detailImage->image.bAllocated() ) 
			mask.drawMask( leftView.detailImage->image.getTextureReference() , leftView.maskFbo.getTextureReference() , 0 , 0 , leftView.maskAlpha ) ; 

		//rightView.maskFbo.draw( 0 , 0 ) ; 
	ofPopMatrix() ; 

	leftView.draw( ) ; 

	rightView.x = ofGetWidth() - marginWidth ; 
	rightView.draw( ) ;

	ofSetColor( 0 , 255 ) ; 
	float h = 180 ; 
	ofRect( 0 , ofGetHeight() - h , ofGetWidth() , h ) ; 

	int i = 0 ; 


	for ( auto thumbnail = thumbnails.begin() ; thumbnail != thumbnails.end() ; thumbnail++ ) 
	{
		//cout << "drawing thumbnail! " << i << " - " << (*thumbnail)->scale << endl ; 

		(*thumbnail)->draw() ; 
		i++ ; 
	}

}
	
void ImageCompareView::populateThumbnailsFromDataSync( )
{
	float padding = 20 ; 

	float thumbSpacing = 50 ; 
	int thumbWidth = ( ofGetWidth() - ( ( padding + thumbSpacing ) * 2 )) / (dataSyncManager->atmosphericImageData.size()  + 2 ) ; 
	for ( int i = 0 ; i < dataSyncManager->atmosphericImageData.size() ; i++ )
	{
		thumbnails.push_back( new ThumbnailWidget() ) ; 
		thumbnails[ i ]->data = dataSyncManager->atmosphericImageData[ i ] ; 
		thumbnails[ i ]->setup( ) ;  
		thumbnails[ i ]->thumbWidth = thumbWidth ; 
		thumbnails[ i ]->x = padding + ( i + 0.5 ) * ( thumbWidth  + thumbSpacing ) ; 
		thumbnails[ i ]->y = ofGetHeight() - 90 ; 

		float hoverPadding = 10 ; 
		thumbnails[ i ]->hitArea = ofRectangle( thumbnails[ i ]->x -thumbWidth/2 - hoverPadding ,
												thumbnails[ i ]->y -thumbWidth/2 - hoverPadding , 
												thumbWidth + hoverPadding * 2 ,
												thumbWidth + hoverPadding * 2 ) ; 
		
	}
	
	ofLogNotice() << thumbnails.size() << " thumbnails created ! " ; 
}

void ImageCompareView::thumbnailSelectionEvent( string &args ) 
{
	for ( int i = 0 ; i < thumbnails.size() ; i++ )
	{
		if ( thumbnails[ i ]->data->label.compare( args ) == 0 ) 
		{
			int evenOrOdd = ((int)ofRandom ( 0 , 100 ) )% 2 ; 
			if ( evenOrOdd == 0 ) 
				rightView.populateFromData( (*thumbnails[ i ]->data) ) ; 
			else
				leftView.populateFromData( (*thumbnails[ i ]->data) ) ; 
			break ; 
		}
	}
}

void ImageCompareView::drawDebug( ) 
{
	stringstream ss ; 
	ss << "rightView alpha : " << rightView.getOFAlpha() << endl ;
	ss << "right view text alpha : " << rightView.title.getOFAlpha() << endl ; 

	for ( int i = 0 ; i < thumbnails.size() ; i++ )
	{
		thumbnails[ i ]->hoverTimer.draw( 500 , 100 + i * 50 ) ; 
	}

	ofDrawBitmapStringHighlight ( ss.str() , 50 , 50 ) ; 

	

	leftView.drawDebug() ; 
	rightView.drawDebug() ; 


}

void ImageCompareView::transitionIn( ) 
{
	leftView.transitionIn() ; 
	rightView.transitionIn() ; 

	for ( int i = 0 ; i < thumbnails.size() ; i++ )
	{
		Tweenzor::add ( &thumbnails[ i ]->alpha , 0.0f , 1.0f , 0.0f , 0.5f , EASE_OUT_QUAD ); 
	}
}

void ImageCompareView::transitionOut( ) 
{
	leftView.transitionOut() ; 
	rightView.transitionOut() ; 
}


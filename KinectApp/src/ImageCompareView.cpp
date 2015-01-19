#include "ImageCompareView.h"

void ImageCompareView::setup ( )
{
	mask.setup( "shaders/composite_rgb" , ofRectangle( 0 , 0 , 1024, 1024 ) ) ; 
	leftView.setup() ; 
	rightView.setup() ; 

	float marginWidth = ( ofGetWidth() - 1024 ) / 2 ; 
	rightView.x = ofGetWidth() - marginWidth ; 

	rightDropZone.data = new AtmosphericImageData() ; 
	rightDropZone.data->image.loadImage( "ui/right_dropZone.png" , ofGetWidth() - 300 , ofGetHeight()/2 , ofPoint( 0.5 , 0.5 ) , 0.0f ) ; 
	rightDropZone.data->label = "RIGHT DROP ZONE" ; 
	rightDropZone.alpha = 0.0f; 
	rightDropZone.bSelectedState = false ; 

	leftDropZone.data = new AtmosphericImageData() ; 
	leftDropZone.data->image.loadImage( "ui/left_dropZone.png" , 300 , ofGetHeight()/2 , ofPoint( 0.5 , 0.5 ) , 0.0f ) ; 
	leftDropZone.data->label = "LEFT DROP ZONE" ;
	leftDropZone.alpha = 0.0f; 
	leftDropZone.bSelectedState = false ; 

	cancelDropZone.data = new AtmosphericImageData() ;
	cancelDropZone.data->image.loadImage( "ui/cancel_dropZone.png" , ofGetWidth()/2 , ofGetHeight()/2 , ofPoint( 0.5 , 0.5 ) , 0.0f ) ; 
	cancelDropZone.data->label = "CANCEL DROP ZONE" ;
	cancelDropZone.alpha = 0.0f; 
	cancelDropZone.bSelectedState = false ; 


	rightDropZone.setup( ) ;  
	leftDropZone.setup( ) ; 
	cancelDropZone.setup( ) ;

	int thumbW = rightDropZone.data->image.getWidth() ; 
	rightDropZone.thumbWidth = thumbW ; // rightDropZone.data->image.getWidth() ;   
	leftDropZone.thumbWidth = thumbW ; //leftDropZone.data->image.getWidth() ;    
	cancelDropZone.thumbWidth = thumbW ; //cancelDropZone.data->image.getWidth() ;   

	rightDropZone.setHitAreaFromImage( &rightDropZone.data->image ) ;
	leftDropZone.setHitAreaFromImage( &leftDropZone.data->image ) ;
	cancelDropZone.setHitAreaFromImage( &cancelDropZone.data->image ) ;

	float _w = 1024 ; 
	float _h = 100 ; 
	slider.hitArea = ofRectangle( ofGetWidth()/2 - _w/2 , 1024/2 - _h/2 , _w , _h ) ; 
	slider.setup( ) ; 
	slider.alpha = 0.0f ; 

	ofAddListener( StarScanViewerEvents::getInstance().THUMBNAIL_SELECTED , this , &ImageCompareView::thumbnailSelectionEvent ) ; 
	dim = ofFloatColor( 0.1f , 0.1f , 0.1f , 0.0f ) ; 	
	lastData = NULL ; 

	//normalizedAmount = 0.5f ; 
}
	
void ImageCompareView::update( ) 
{
	//normalizedAmount = ofMap(  sin( ofGetElapsedTimef() ) , -1.0f , 1.0f , 0.0f , 1.0f , true )  ; 
	leftView.update() ; 
	leftView.alphaStackUpdate( 1.0f ) ; 
	rightView.update( ) ;
	rightView.alphaStackUpdate( 1.0f ) ; 

	for ( auto thumbnail = thumbnails.begin() ; thumbnail != thumbnails.end() ; thumbnail++ ) 
	{
		(*thumbnail)->update() ; 
		(*thumbnail)->alphaStackUpdate( 1.0f ) ; 
	}

	rightDropZone.update() ; 
	leftDropZone.update() ; 
	cancelDropZone.update( ); 
	slider.update() ; 

	rightDropZone.data->image.alphaStackUpdate( 1.0f )  ; 
	leftDropZone.data->image.alphaStackUpdate( 1.0f )  ; 
	cancelDropZone.data->image.alphaStackUpdate( 1.0f )  ; 
	slider.alphaStackUpdate( 1.0f ) ; 
}
	
void ImageCompareView::draw( ) 
{
	//Draw Left active area
	leftView.maskFbo.begin() ; 
		ofSetColor( ofColor::black ) ; 
		ofRect( 0 , 0 , 1024.0f , ofGetHeight() ) ; 
		ofSetColor( ofColor::white ) ; 
		ofRect( 0 , 0 , 1024.0f * slider.smoothedNormalized , ofGetHeight() ) ; 
	leftView.maskFbo.end() ; 

	//Draw Right active area
	rightView.maskFbo.begin( ) ; 
		//float _y = ofGetHeight() /2 ; 
		ofSetColor( ofColor::black ) ; 
		ofRect( 0 , 0 , 1024.0f , ofGetHeight() ) ; 
		ofSetColor( ofColor::white ) ; 
		ofRect( 1024.0f * slider.smoothedNormalized  , 0 , 1024.0f * ( 1.0f - slider.smoothedNormalized ) , ofGetHeight() ) ; 
	rightView.maskFbo.end( ) ; 
	
	float marginWidth = ( ofGetWidth() - 1024 ) / 2 ; 
	ofPushMatrix() ; 
		float offsetX = marginWidth ;
		float _x = ( ofGetWidth() - 1024 ) / 2 ; 
		ofTranslate( _x , 0  ) ; 
		//ofScale( 0.93, 0.93 ) ; 

		
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

	slider.draw( ) ; 
	//

	ofSetColor( dim ) ; 
	ofRect( 0 , 0 ,ofGetWidth(), ofGetHeight() ) ; 


	rightDropZone.draw( ) ; 
	leftDropZone.draw() ; 
	cancelDropZone.draw( ) ; 

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

	/*
	//Set the current mask to Two unique Ids
	int index1 = ofClamp ( ofRandom ( 0 , thumbnails.size() -1 ) , 0 , thumbnails.size() -1 ) ; 
	int index2 = ofClamp ( ofRandom ( 0 , thumbnails.size() -1 ) , 0 , thumbnails.size() -1 ) ; 
	while( index1 == index2 )
	{
		index2 =  ofClamp ( ofRandom ( 0 , thumbnails.size() -1 ) , 0 , thumbnails.size() -1 ) ; 
	}

	leftDropZone.data = thumbnails[ index1 ]->data ; 
	thumbnails[ index1 ]->bHoverSelected = true ; 
	leftDropZone.data = thumbnails[ index2 ]->data ;  
	thumbnails[ index2 ]->bHoverSelected = true ;  */
	ofLogNotice() << thumbnails.size() << " thumbnails created ! " ; 
}

void ImageCompareView::thumbnailSelectionEvent( string &args ) 
{
	for ( int i = 0 ; i < thumbnails.size() ; i++ )
	{
		if ( thumbnails[ i ]->data->label.compare( args ) == 0 ) 
		{
			// ! 
			lastData = thumbnails[ i ]->data ; 
			transitionInDropZones( ) ; 
		}
	}

	if ( lastData == NULL ) 
		return ;


	if ( args.compare( cancelDropZone.data->label ) == 0 ) 
	{
		ofLogNotice() << " CANCELLED : " << lastData->label ; 
		transitionOutDropZones( ) ; 
	}

	if ( args.compare( rightDropZone.data->label ) == 0 ) 
	{
		rightView.populateFromData( (*lastData) ) ; 
		ofLogNotice() << " RIGHT ZONE should be : " << lastData->label ; 
		transitionOutDropZones( ) ; 
	}

	if ( args.compare( leftDropZone.data->label ) == 0 ) 
	{
		leftView.populateFromData( (*lastData) ) ; 
		ofLogNotice() << " LEFT ZONE should be : " << lastData->label ; 
		transitionOutDropZones( ) ; 
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

	Tweenzor::add( &slider.alpha , slider.alpha , 1.0f , 0.0f , 0.5f , EASE_OUT_QUAD ) ; 
}

void ImageCompareView::transitionOut( ) 
{
	leftView.transitionOut() ; 
	rightView.transitionOut() ; 
}

void ImageCompareView::transitionInDropZones( ) 
{
	Tweenzor::add( &dim.a , 0.0f , 0.4f , 0.0f, 0.5f , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &rightDropZone.alpha , 0.0f , 1.0f , 0.25f, 0.5f , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &leftDropZone.alpha , 0.0f , 1.0f , 0.25f, 0.5f , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &cancelDropZone.alpha , 0.0f , 1.0f , 0.25f, 0.5f , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &rightDropZone.data->image.alpha , 0.0f , 1.0f , 0.25f, 0.5f , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &leftDropZone.data->image.alpha , 0.0f , 1.0f , 0.25f, 0.5f , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &cancelDropZone.data->image.alpha , 0.0f , 1.0f , 0.25f, 0.5f , EASE_OUT_QUAD ) ; 

	for ( int i = 0 ; i < thumbnails.size() ; i++ )
	{
		Tweenzor::add ( &thumbnails[ i ]->alpha , thumbnails[ i ]->alpha , 0.6f , 0.0f , 0.5f , EASE_OUT_QUAD ); 
	}

	Tweenzor::add( &slider.alpha , slider.alpha , 0.0f , 0.0f , 0.5f , EASE_OUT_QUAD ) ; 

	rightDropZone.bInteractive = true ;
	leftDropZone.bInteractive = true ;
	cancelDropZone.bInteractive = true ;
}

void ImageCompareView::transitionOutDropZones( ) 
{
	Tweenzor::add( &dim.a , 1.0f , 0.0f , 0.0f, 0.5f , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &rightDropZone.alpha , 1.0f , 0.0f , 0.25f, 0.5f , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &leftDropZone.alpha , 1.0f , 0.0f , 0.0f, 0.5f , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &cancelDropZone.alpha , 1.0f , 0.0f , 0.0f, 0.5f , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &rightDropZone.data->image.alpha , 1.0f , 0.0f , 0.25f, 0.5f , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &leftDropZone.data->image.alpha , 1.0f , 0.0f , 0.0f, 0.5f , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &cancelDropZone.data->image.alpha , 1.0f , 0.0f , 0.0f, 0.5f , EASE_OUT_QUAD ) ; 

	for ( int i = 0 ; i < thumbnails.size() ; i++ )
	{
		Tweenzor::add ( &thumbnails[ i ]->alpha , thumbnails[ i ]->alpha , 1.0f , 0.0f , 0.5f , EASE_OUT_QUAD ); 
	}

	Tweenzor::add( &slider.alpha , slider.alpha , 1.0f , 0.0f , 0.5f , EASE_OUT_QUAD ) ; 
	rightDropZone.bInteractive = false ;
	leftDropZone.bInteractive = false ;
	cancelDropZone.bInteractive = false ;
}

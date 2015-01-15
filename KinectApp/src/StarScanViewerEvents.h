/*

From http://stackoverflow.com/questions/1008019/c-singleton-design-pattern

*/

#pragma once

#include "ofMain.h"

 class StarScanViewerEvents
{


	public : 
		static StarScanViewerEvents& getInstance()
			{
				static StarScanViewerEvents    instance; // Guaranteed to be destroyed.
									  // Instantiated on first use.
				return instance;
			}

		ofEvent< string >  THUMBNAIL_SELECTED ; 

    private:
        StarScanViewerEvents() {};                   // Constructor? (the {} brackets) are needed here.
        // Dont forget to declare these two. You want to make sure they
        // are unaccessable otherwise you may accidently get copies of
        // your singleton appearing.
        StarScanViewerEvents(StarScanViewerEvents const&);              // Don't Implement
        void operator=(StarScanViewerEvents const&); // Don't implement

		


};
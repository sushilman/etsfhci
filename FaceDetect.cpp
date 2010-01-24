/*
 * FaceDetect.cpp
 *
 *  Created on: Nov 25, 2009
 *      Author: sushil
 */

#include "FaceDetect.h"
#include "X11/Xlib.h"

static CvHaarClassifierCascade* haarcascade;

CvRect oldFaceRect;
bool isFirst=true;

FaceDetect::FaceDetect() {
	// TODO Auto-generated constructor stub

}

void FaceDetect::extractFace(IplImage* img){
	/**
	 * Detect Faces, extract ROI -> largest face and return it
	 * Call EyeExtract??(instead of EyeDetect, rename EyeDetect->EyeExtract)
	 * Call EyeTracking
	 */
	////////////
	int radius=0,height=0,x=0,y=0;
	/////////
	int scale = 1;
	// Create two points to represent the face locations
	CvPoint pt1, pt2;
	int i;
	CvMemStorage* storage = cvCreateMemStorage(0);
	// Clear the memory storage which was used before

//	cvClearMemStorage( storage );

	CvRect* r1=0;
	CvRect* largestRect;
	int nFaces;
	// Find whether the cascade is loaded, to find the faces. If yes, then:
		if( haarcascade ) {
			// There can be more than one face in an image. So create a growable sequence of faces.
			// Detect the objects and store them in the sequence
			CvSeq* faces = cvHaarDetectObjects( img, haarcascade, storage, 1.1, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(40, 40) );
			int largeArea=0;
			// Loop the number of faces found.
			nFaces=faces->total;
			for( i = 0; i < (faces ? faces->total : 0); i++ ){
				// Create a new rectangle for drawing the face
				CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
				r1=r;
				// Find the dimensions of the face,and scale it if necessary
				pt1.x = r->x*scale;
				pt2.x = (r->x+r->width)*scale;
				pt1.y = r->y*scale;
				pt2.y = (r->y+r->height)*scale;
				// Draw the rectangle in the input image
				int area=(pt2.x-pt1.x)*(pt2.y-pt1.y);
				if(area>largeArea){
					largestRect=r;
				}
			}
			pt1.x = largestRect->x*scale;
			pt2.x = (largestRect->x+largestRect->width)*scale;
			pt1.y = largestRect->y*scale;
			pt2.y = (largestRect->y+largestRect->height)*scale;
			//cvRectangle( img, pt1, pt2, CV_RGB(255,0,0), 3, 8, 0 );
			//cout<<"FAceDEteCted";
		}
		//cvShowImage("detected face",img);
		//cvWaitKey(0);
		if(nFaces==0){
			cvReleaseMemStorage(&storage);
			return;
		}

		CvRect faceRect;
		faceRect.x=largestRect->x;
		faceRect.y=largestRect->y;
		faceRect.height=largestRect->height;
		faceRect.width=largestRect->width;



		/**
		 * If we remove this block Memory leak is significantly reduced !!! or none at all
		 */
//		IplImage* temp=cvCloneImage(img);
//		if(largestRect){
//			ExtractROI extractROI;
//			temp=extractROI.extractROI(img,faceRect);
//		}
		if(largestRect){
			if(isFirst){
				oldFaceRect=faceRect;
				isFirst=false;
			}else{
				if(abs(oldFaceRect.width-faceRect.width) < 3 || abs(oldFaceRect.x-faceRect.x) < 3){
//					img=0;
//					return;
					faceRect=oldFaceRect;
				}
			}
			cvSetImageROI(img,faceRect);
		}else{
			img=0;
		}
		cvReleaseMemStorage(&storage);
}

CvSeq* FaceDetect::detectFaces(IplImage* img){
    CvMemStorage* storage=cvCreateMemStorage(0);
    cvClearMemStorage(storage);
    // Clear the memory storage which was used before
    cvClearMemStorage( storage );
    CvSeq* faces;
    if( haarcascade ) {
	   // There can be more than one face in an image. So create a growable sequence of faces.
	   // Detect the objects and store them in the sequence
	   faces = cvHaarDetectObjects( img, haarcascade, storage,
										   1.1, 2, CV_HAAR_DO_CANNY_PRUNING,
										   cvSize(40, 40) );
    }else{
    	faces->total=0;
    }
    return faces;
}

CvRect* FaceDetect::getLargestFaceCoordinates(CvSeq* faces,IplImage* img){
		// Loop the number of faces found.
		CvRect r2;
		CvRect* r[faces->total];
		CvPoint pt1, pt2;
	    int scale = 1;
	    int largest=0;
	    int largeArea=0;
	    for( int i = 0; i < (faces ? faces->total : 0); i++ ){
	       // Create a new rectangle for drawing the face
	        r[i] = (CvRect*)cvGetSeqElem( faces, i );
	        r2=*r[i];
	        // Find the dimensions of the face,and scale it if necessary
	        pt1.x = r[i]->x*scale;
	        pt2.x = (r[i]->x+r[i]->width)*scale;
	        pt1.y = r[i]->y*scale;
	        pt2.y = (r[i]->y+r[i]->height)*scale;
	        int area=(pt2.x-pt1.x)*(pt2.y-pt1.y);
	        if(area>largeArea){
	        	largest=i;
	        }
	    }
		return r[largest];
}

CvRect** FaceDetect::getFaceCoordinates(CvSeq* faces,IplImage* img){
	// Loop the number of faces found.
	CvRect r2;
	CvRect* r[faces->total];
	CvPoint pt1, pt2;
    int scale = 1;
    for( int i = 0; i < (faces ? faces->total : 0); i++ )
    {
       // Create a new rectangle for drawing the face
        r[i] = (CvRect*)cvGetSeqElem( faces, i );
        r2=*r[i];
        // Find the dimensions of the face,and scale it if necessary
        pt1.x = r[i]->x*scale;
        pt2.x = (r[i]->x+r[i]->width)*scale;
        pt1.y = r[i]->y*scale;
        pt2.y = (r[i]->y+r[i]->height)*scale;

        // Draw the rectangle in the input image
  //      cvRectangle( img, pt1, pt2, CV_RGB(255,0,0), 3, 8, 0 );
    }
/*
	if(r){
		r2.y=r2.y+r2.height/4;
		r2.width=r2.width/2;
		r2.height=r2.height/4;

		r2.x=r2.x+r2.width/8;
	}

*/

	return r;
}

void FaceDetect::drawRectangleAroundFace(IplImage* img,CvRect** r,int numberOfFaces){
	for(int i=0;i<numberOfFaces;i++){
	// Draw the rectangle in the input image
	CvPoint pt1,pt2;
	pt1.x = r[i]->x;
	pt2.x = (r[i]->x+r[i]->width);
	pt1.y = r[i]->y;
	pt2.y = (r[i]->y+r[i]->height);

	cvRectangle( img, pt1, pt2, CV_RGB(255,0,0), 2, 8, 0 );
	}
}

void FaceDetect::setHaarClassifier(CvHaarClassifierCascade* haarcascade1){
	haarcascade=haarcascade1;
}

FaceDetect::~FaceDetect() {
	// TODO Auto-generated destructor stub
}

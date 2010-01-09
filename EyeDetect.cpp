/*
 * EyeDetect.cpp
 *
 *  Created on: Dec 30, 2009
 *      Author: sushil
 */

#include "EyeDetect.h"
CvHaarClassifierCascade* haarcascade_eye;

EyeDetect::EyeDetect() {
	// TODO Auto-generated constructor stub

}

IplImage* EyeDetect::extractEye(IplImage* img){

	// Create two points to represent the eye locations
	CvPoint pt1, pt2,ptTemp;
	int scale=1;
	int nEyes;
	CvMemStorage* storage = cvCreateMemStorage(0);

	int minX;
	int maxWidth;

	CvRect eyeRect;

	if( haarcascade_eye ) {
		// Detect the objects and store them in the sequence
		CvSeq* eyes = cvHaarDetectObjects( img, haarcascade_eye, storage,1.1, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(20, 20) );
		// Loop the number of eyes found.
		if(!eyes)
			return 0;
		nEyes=eyes->total;

		CvRect* r1 = (CvRect*)cvGetSeqElem( eyes, 0 );
		if(r1){
			minX=r1->x;
			maxWidth=r1->width;
		}
		//cvWaitKey(0);
		for( int i = 0; i < (eyes ? eyes->total : 0); i++ ){
		//for( int i = 0; i < 1; i++ ){
			// Create a new rectangle for drawing the face
			CvRect* r = (CvRect*)cvGetSeqElem( eyes, i );
			// Find the dimensions of the face,and scale it if necessary
			pt1.x = r->x*scale;
			pt2.x = (r->width)*scale;
			//ptTemp.x = (r->x+r->width)*scale;
			pt1.y = r->y*scale;
			pt2.y = (r->height)*scale;
			//ptTemp.x = (r->x+r->width)*scale;

			// Consider rectangle only if greater than
			if(r->width>img->width/5){
				if(r->x<minX)
					minX=r->x;
				if(r->width>maxWidth)
					maxWidth=r->width;
				// Draw the rectangle in the input image
				CvBox2D box;
				box.angle=1;
				box.center.x=(pt1.x+pt2.x)/2;
				box.center.y=(pt1.y+pt2.y)/2;
				box.size.height=10;
				box.size.width=10;
				cvRectangle( img, pt1, pt2, CV_RGB(255,0,0), 3, 8, 0 );
				//cvEllipseBox( img, box, CV_RGB(255,0,0), 3, CV_AA, 0 );
				cout<<"draw rect"<<endl;
			}
		}
		cvShowImage("Rec",img);
	}

	//set ROI rectangle
	eyeRect.x=minX;
	eyeRect.y=pt1.y+10;//pt1.y;
	eyeRect.width=maxWidth;
	eyeRect.height=pt2.y-10;//pt2.y;

	/*
	 * to Control the height of ROI so that it does not
	 */
//	if(eyeRect.x+eyeRect.height>img->height)
//		eyeRect.height-=eyeRect.x;

//	cout<<"X: "<<eyeRect.x<<"\n Y:"<<eyeRect.y<<"\n width:"<<eyeRect.width<<"\n height:"<<eyeRect.height<<endl;

	//cout<<"\n width:"<<img->width<<"\n height:"<<img->height<<endl;
	//cvWaitKey(1000);
	/**
	 * If we remove this block Memory leak is significantly reduced !!! or none at all
	 */
//	IplImage* temp=cvCloneImage(img);
//	if(eyeRect.x<1000){
//		ExtractROI extractROI;
//		temp=img;//extractROI.extractROI(img,eyeRect);
//	}
	if(eyeRect.x<1000){
		cvSetImageROI(img,eyeRect);
	}

	cvReleaseMemStorage(&storage);
//
			int ht=img->height;
			int wd=img->width;

			int ratio =128/ht; // to make image approximately 128 in height
			ht=128;//ht*ratio;
			wd=256;//wd*ratio;

			IplImage* temp2 = cvCreateImage( cvSize(wd,ht), 8, 3 );
			cvResize(img,temp2,CV_INTER_LINEAR);

			//img=cvCloneImage(temp2);
			//cvReleaseImage(&img);
			cvReleaseHaarClassifierCascade(&haarcascade_eye);
	//return temp2;
			return img;
}

void EyeDetect::setHaarClassifier(char* cascade_name_eye){
	haarcascade_eye = (CvHaarClassifierCascade*)cvLoad( cascade_name_eye, 0, 0, 0 );
}

EyeDetect::~EyeDetect() {
	// TODO Auto-generated destructor stub
}

/*
 * EyeExtract.cpp
 *
 *  Created on: Dec 9, 2009
 *      Author: sushil
 */

#include "EyeExtract.h"
int EYE_HEIGHT=128;//ht*ratio;
int EYE_WIDTH=256;//wd*ratio;
EyeExtract::EyeExtract() {
	// TODO Auto-generated constructor stub

}

IplImage* EyeExtract::extract(IplImage* img){
	/*
	 * Get size of image
	 * Set ROI
	 * return that image
	 */
//	IplImage* temp=0;
	if(!img->roi){
		cerr<<"Error !!!";
		cvReleaseImage(&img);
		return 0;
	}
	if(img->roi){
		int height=img->roi->height;
		int width=img->roi->width;

		//CvRect eyeRect=cvRect(img->roi->xOffset+width/9,img->roi->yOffset+height/4,width/3,height/5);
		int x = img->roi->xOffset+width/8;
		int y = img->roi->yOffset+height/4;
		int h =img->roi->height/5;
		int w =2*h;
		CvRect eyeRect=cvRect(x,y,w,h);

//		temp=cvCloneImage(img);
		cvSetImageROI(img,eyeRect);
		//cvWaitKey(0);
//		ExtractROI extractROI;
//		temp=extractROI.extractROI(temp,eyeRect);


////
//		int ht=temp->height;
//		int wd=temp->width;
////
////		//int ratio =128/ht; // to make image approximately 128 in height
		int ht=EYE_HEIGHT;;
		int wd=EYE_WIDTH;;
////

		IplImage* temp2= cvCreateImage( cvSize(wd,ht), 8, 3 );
		cvResize(img,temp2,CV_INTER_LINEAR);

//		cvShowImage("resized",temp2);
//		Memory leakage here
//		*img=*cvCloneImage(temp2);

	//	cvCopy( temp2, img, 0 );
		cvReleaseImage(&img);
		return temp2;

//		cvReleaseImage(&temp2);
//		cvReleaseImage(&img_copy);
	}
}

EyeExtract::~EyeExtract() {
	// TODO Auto-generated destructor stub
}

/*
 * EyeCorners.cpp
 *
 *  Created on: Dec 30, 2009
 *      Author: sushil
 */

#include "EyeCorners.h"

EyeCorners::EyeCorners() {
	// TODO Auto-generated constructor stub

}

void EyeCorners::detectEyeCorners(IplImage* img){

	CvSeq* polygon;
	CvMemStorage* storage1 = 0;
	CvMemStorage* storage;

	IplImage* edge = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1); //converting  color image to gray scale
	CannyEdge cannyEdge;

	edge=cannyEdge.detectEdge(img);

	CvContour contour;
	CvSeq* first_contour=NULL;
	if(edge){
		int Nc = cvFindContours(edge,storage,&first_contour,sizeof(CvContour),CV_RETR_LIST);
		cout<<"Total contours detected:"<<Nc<<endl;
		cvShowImage("Canny",edge);
		//cvDrawContours(img,first_contour,cvScalarAll(255),	cvScalarAll(255), 100 );
		//cvFindDominantPoints(first_contour,storage,CV_DOMINANT_IPAN,1,2,3,4);
		cout<<"Length="<<cvContourPerimeter(first_contour)<<endl;
		cout<<first_contour->first;

	}
	CvRect eyeRect=cvBoundingRect(first_contour,1);
	cvRectangle(img,cvPoint(eyeRect.x,eyeRect.y),cvPoint(eyeRect.x+eyeRect.width,eyeRect.y+eyeRect.height),cvScalar(255,255,0),1);
    polygon = cvApproxPoly(first_contour, sizeof(CvContour), storage1,CV_POLY_APPROX_DP, 100);

    if(polygon){
    	cout<<"draw polygon pts:"<<polygon->total<<endl;
		for( int i = 0; i < polygon->total; i++ ){
			float* p = (float*)cvGetSeqElem( polygon, i );
			CvPoint temppt=cvPoint(cvRound(p[0]),cvRound(p[1]));
			cvCircle( img, temppt, 2, CV_RGB(255,0,0), 2, 8, 0 );
		}
    }
}

EyeCorners::~EyeCorners() {
	// TODO Auto-generated destructor stub
}

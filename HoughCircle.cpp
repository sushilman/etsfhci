/*
 * HoughCircle.cpp
 *
 *  Created on: Nov 25, 2009
 *      Author: sushil
 */

#include "HoughCircle.h"
 float radius;
 CvPoint center;
 bool isDetected=false;
HoughCircle::HoughCircle() {
	// TODO Auto-generated constructor stub

}

CvSeq* HoughCircle::locateHoughCircles(IplImage* img,double min_distance, double upperThreshold, double accumulatorThreshold, double min_radius, double max_radius ){
	//IplImage* gray = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	//storage = cvCreateMemStorage(0);

    IplImage* gray=cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
    CvMemStorage* storage = cvCreateMemStorage(0);
    cvCvtColor( img, gray, CV_BGR2GRAY );

    cvShowImage("Gray",gray);
   // cvEqualizeHist(gray,gray);
    //cvShowImage("Hist Eq Gray",gray);

	CvSeq* circle = cvHoughCircles( gray, storage, CV_HOUGH_GRADIENT, 2, min_distance, upperThreshold, accumulatorThreshold, min_radius, max_radius);
	cvReleaseImage(&gray);
	cvReleaseMemStorage(&storage);

	return circle;
}

IplImage* HoughCircle::drawHoughCircles(IplImage* img,double min_distance, double upperThreshold, double accumulatorThreshold, double min_radius, double max_radius){
	isDetected=false;
	CvSeq* circle=locateHoughCircles(img,min_distance, upperThreshold, accumulatorThreshold, min_radius, max_radius);
	 for( int i = 0; i < circle->total; i++ ){
		 float* p = (float*)cvGetSeqElem( circle, i );
		 center=cvPoint(cvRound(p[0]),cvRound(p[1]));
		 radius=cvRound(p[2]);
		 cvCircle( img, center, radius, CV_RGB(255,0,0), 2, 8, 0 );
		 if(p){
			 isDetected=true;
			 break;
		 }
	 }
	 return img;
}

bool HoughCircle::isHoughDetected(){
	return isDetected;
}

void HoughCircle::drawHoughCircles_IntoNew(IplImage* originalImage, IplImage* newImage,double min_distance, double upperThreshold, double accumulatorThreshold, double min_radius, double max_radius){
	newImage=cvCloneImage(originalImage);
	drawHoughCircles(newImage,min_distance, upperThreshold, accumulatorThreshold, min_radius, max_radius);
}

float HoughCircle::getRadius(){
	return radius;
}
CvPoint HoughCircle::getCenter(){
	return center;
}

HoughCircle::~HoughCircle() {
	// TODO Auto-generated destructor stub
}

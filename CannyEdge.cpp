/*
 * CannyEdge.cpp
 *
 *  Created on: Dec 21, 2009
 *      Author: sushil
 */

#include "CannyEdge.h"

CannyEdge::CannyEdge() {
	// TODO Auto-generated constructor stub

}

IplImage* CannyEdge::detectEdge(IplImage* img){
	IplImage* edge = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	//converting  color image to gray scale
	cvCvtColor(img, edge, CV_BGR2GRAY);

	// Extracting edges
//	cvSmooth(edge,edge,CV_GAUSSIAN,5,5);
	cvSmooth(edge,edge,CV_GAUSSIAN,3,3);
	cvCanny(edge, edge, 30, 50, 3);
	return edge;
}

CannyEdge::~CannyEdge() {
	// TODO Auto-generated destructor stub
}

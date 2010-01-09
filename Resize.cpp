/*
 * Resize.cpp
 *
 *  Created on: Nov 25, 2009
 *      Author: sushil
 */

#include "Resize.h"

Resize::Resize() {
	// TODO Auto-generated constructor stub

}
//Private
void Resize::resize(IplImage* originalImage, IplImage* resizedImage, int width, int height, int depth, int channels){
    resize_IntoNew(originalImage, resizedImage, width, height, depth, channels);
    cvReleaseImage(&resizedImage);
}

//Public
void Resize::resize(IplImage* img, int width, int height){
	resize(img,img,width,height,8,3);
}

void Resize::resize(IplImage* img, int width, int height, int depth, int channels){
	resize(img,img,width,height,depth,channels);
}

void Resize::resize_IntoNew(IplImage* originalImage, IplImage* resizedImage, int width, int height, int depth, int channels){
    resizedImage = cvCreateImage( cvSize(width,height), 8, 3 );
    cvResize(originalImage,resizedImage,CV_INTER_LINEAR);
}

Resize::~Resize() {
	// TODO Auto-generated destructor stub
}

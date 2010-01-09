/*
 * FaceDetect.h
 *
 *  Created on: Nov 25, 2009
 *      Author: sushil
 */
#include "OpencvIncludes.h"
#include "Resize.h"
#include "MoveMouse.h"

#ifndef FACEDETECT_H_
#define FACEDETECT_H_

class FaceDetect {
public:
	FaceDetect();

	void extractFace(IplImage* img);
	CvRect* getLargestFaceCoordinates(CvSeq* faces,IplImage* img);
	void drawRectangleAroundFace(IplImage* img,CvRect** r,int number);
	CvRect** getFaceCoordinates(CvSeq* faces,IplImage* img);
	CvSeq* detectFaces(IplImage* img);
	void setHaarClassifier(CvHaarClassifierCascade* haarcascade1);

	virtual ~FaceDetect();
};

#endif /* FACEDETECT_H_ */

/*
 * HoughCircle.h
 *
 *  Created on: Nov 25, 2009
 *      Author: sushil
 */
#include "OpencvIncludes.h"
#ifndef HOUGHCIRCLE_H_
#define HOUGHCIRCLE_H_

class HoughCircle {

public:
	HoughCircle();
	CvSeq* locateHoughCircles(IplImage* img,double min_distance, double upperThreshold, double accumulatorThreshold, double min_radius, double max_radius );
	IplImage* drawHoughCircles(IplImage* img,double min_distance, double upperThreshold, double accumulatorThreshold, double min_radius, double max_radius);
	void drawHoughCircles_IntoNew(IplImage* originalImage, IplImage* newImage,double min_distance, double upperThreshold, double accumulatorThreshold, double min_radius, double max_radius);

	bool isHoughDetected();

	float getRadius();
	CvPoint getCenter();

	virtual ~HoughCircle();
};

#endif /* HOUGHCIRCLE_H_ */


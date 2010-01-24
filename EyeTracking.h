/*
 * EyeTracking.h
 *
 *  Created on: Dec 11, 2009
 *      Author: sushil
 */

#ifndef EYETRACKING_H_
#define EYETRACKING_H_
#include "OpencvIncludes.h"
#include "MoveMouse.h"
#include "CamShiftTracking.h"
#include "CamShiftIris.h"
#include "LucasKanadeTracking.h"
#include "Calibrate.h"

class EyeTracking {
public:
	EyeTracking();
	//CvPoint locateCoordinates(CvBox2D boxIris);
	CvPoint locateCoordinates(CvBox2D boxIris,CvPoint topLeftMin,CvPoint topLeftMax, CvPoint topRightMin,CvPoint topRightMax, CvPoint bottomRightMin,CvPoint bottomRightMax, CvPoint bottomLeftMin,CvPoint bottomLeftMax);
	bool trackCamShift(IplImage* image, CvPoint *center, int radius);
	bool trackLucasKanade(IplImage* img, CvPoint *center, int *add_remove_pt, bool *isIrisSet);
	virtual ~EyeTracking();
};

#endif /* EYETRACKING_H_ */

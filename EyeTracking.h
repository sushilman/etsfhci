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
#include "LucasKanadeTracking.h"

class EyeTracking {
public:
	EyeTracking();
	CvPoint locateCoordinates(CvBox2D boxIris, CvBox2D boxEye);
	bool trackCamShift(IplImage* image, CvPoint *center, int radius);
	bool trackLucasKanade(IplImage* img, CvPoint *center, int *add_remove_pt, bool *isIrisSet);
	virtual ~EyeTracking();
};

#endif /* EYETRACKING_H_ */

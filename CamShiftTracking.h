/*
 * CamShiftTracking.h
 *
 *  Created on: Jan 6, 2010
 *      Author: sushil
 */

#include "OpencvIncludes.h"

#ifndef CAMSHIFTTRACKING_H_
#define CAMSHIFTTRACKING_H_

class CamShiftTracking {
public:
	CamShiftTracking();
	CvBox2D track( IplImage* image, CvRect selection,bool isIris);
	void setVmax(int value);
	CvScalar hsv2rgb( float hue );
	virtual ~CamShiftTracking();
};

#endif /* CAMSHIFTTRACKING_H_ */

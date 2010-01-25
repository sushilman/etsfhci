/*
 * CamShiftIris.h
 *
 *  Created on: Jan 6, 2010
 *      Author: sushil
 */

#include "OpencvIncludes.h"

#ifndef CAMSHIFTIRIS_H_
#define CAMSHIFTIRIS_H_

class CamShiftIris {
public:
	CamShiftIris();
	CvBox2D track( IplImage* image, CvRect selection,bool isIris);
	void setVmax(int value);
	CvScalar hsvrgb( float hue );
	virtual ~CamShiftIris();
};

#endif /* CAMSHIFTIRIS_H_ */

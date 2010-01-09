/*
 * EyeCorners.h
 *
 *  Created on: Dec 30, 2009
 *      Author: sushil
 */
#include "OpencvIncludes.h"
#include "CannyEdge.h"

#ifndef EYECORNERS_H_
#define EYECORNERS_H_

class EyeCorners {
public:
	EyeCorners();
	void detectEyeCorners(IplImage* img);
	virtual ~EyeCorners();
};

#endif /* EYECORNERS_H_ */

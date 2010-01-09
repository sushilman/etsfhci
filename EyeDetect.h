/*
 * EyeDetect.h
 *
 *  Created on: Dec 30, 2009
 *      Author: sushil
 */
#include "OpencvIncludes.h"
#include "Resize.h"

#ifndef EYEDETECT_H_
#define EYEDETECT_H_

class EyeDetect {
public:
	EyeDetect();
	IplImage* extractEye(IplImage* img);
	void setHaarClassifier(char* cascade_name_eye);
	virtual ~EyeDetect();
};

#endif /* EYEDETECT_H_ */

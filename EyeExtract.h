/*
 * EyeExtract.h
 *
 *  Created on: Dec 9, 2009
 *      Author: sushil
 */

#ifndef EYEEXTRACT_H_
#define EYEEXTRACT_H_

#include "OpencvIncludes.h"



class EyeExtract {
public:
	EyeExtract();
	IplImage* extract(IplImage* img);
	virtual ~EyeExtract();
};

#endif /* EYEEXTRACT_H_ */

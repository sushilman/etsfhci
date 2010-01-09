/*
 * main.h
 *
 *  Created on: Nov 25, 2009
 *      Author: sushil
 */
#include "OpencvIncludes.h"
#include "FaceDetect.h"
#include "EyeDetect.h"
#include "EyeExtract.h"
#include "EyeTracking.h"
#include "HoughCircle.h"
#include "CannyEdge.h"


#ifndef MAIN_H_
#define MAIN_H_


void initialize(int argc, char** argv );
void extractFace(IplImage* img, CvHaarClassifierCascade* cascade);
void detect_and_draw( IplImage* img );
class BAD_ARGUMENT{};
#endif /* MAIN_H_ */

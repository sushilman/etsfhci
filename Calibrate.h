/*
 * Calibrate.h
 *
 *  Created on: Jan 21, 2010
 *      Author: sushil
 */

#ifndef CALIBRATE_H_
#define CALIBRATE_H_
#include "OpencvIncludes.h"

class Calibrate {
public:
	Calibrate();

	void startCalibration(CvBox2D box,CvBox2D boxEye);

	int center(CvBox2D box);
	int topRight(CvBox2D box);
	int topLeft(CvBox2D box);
	int bottomRight(CvBox2D box);
	int bottomLeft(CvBox2D box);
	bool isCalibrated();
//	bool isCalibrationOnProgress();
	void setCalibration(bool value);
//	void setCalibrationOnProgress(bool value);
	void init();

	CvPoint getTopLeftMin();
	CvPoint getTopLeftMax();

	CvPoint getTopRightMin();
	CvPoint getTopRightMax();

	CvPoint getBottomRightMin();
	CvPoint getBottomRightMax();

	CvPoint getBottomLeftMin();
	CvPoint getBottomLeftMax();

	virtual ~Calibrate();
};

#endif /* CALIBRATE_H_ */

/*
 * Calibrate.cpp
 *
 *  Created on: Jan 21, 2010
 *      Author: sushil
 */

#include "Calibrate.h"
CvPoint TL_MIN, TL_MAX;
CvPoint TR_MIN, TR_MAX;
CvPoint BR_MIN, BR_MAX;
CvPoint BL_MIN, BL_MAX;

//counters
int cTL=0,cTR=0,cBR=0,cBL=0;

int CHOICE;

bool isTLset=false,isTRset=false;
bool isBLset=false,isBRset=false;

bool CALIBRATION_STATUS=false;
Calibrate::Calibrate() {
	// TODO Auto-generated constructor stub
	//Initialize
	init();
}

void Calibrate::init(){
	TL_MIN=cvPoint(256,256);	TL_MAX=cvPoint(0,0);
	TR_MIN=cvPoint(256,256);	TR_MAX=cvPoint(0,0);
	BR_MIN=cvPoint(256,256);	BR_MAX=cvPoint(0,0);
	BL_MIN=cvPoint(256,256);	BL_MAX=cvPoint(0,0);
	cTL=0;cTR=0;cBL=0;cBR=0;
	CALIBRATION_STATUS=false;
	CHOICE=0;
	isTLset=false; isTRset=false;
	isBLset=false; isBRset=false;
}

void Calibrate::startCalibration(CvBox2D box){
	int ch=cvWaitKey(10);
	if(char(ch)=='0'||char(ch)=='1'||char(ch)=='2'||char(ch)=='3'||char(ch)=='4')
		CHOICE=ch;
	switch(char(CHOICE)){
		case '1':
			cout<<"Top Left";
			cvWaitKey(0);
			topLeft(box);
			break;
		case '2':
			cout<<"Top Right";
			cvWaitKey(0);
			topRight(box);
			break;
		case '3':
			cout<<"Bottom Right";
			cvWaitKey(0);
			bottomRight(box);
			break;
		case '4':
			cout<<"Bottom Left";
			cvWaitKey(0);
			bottomLeft(box);
			break;
		default:
			;
	}

	if(isTLset && isTRset && isBRset && isBLset)
		setCalibration(true);
}

int Calibrate::center(CvBox2D box){

}

int Calibrate::topRight(CvBox2D box){
	cTR++;
	if(cTR>5){ //ignore first few data
		TR_MIN.x=TR_MIN.x>box.center.x?box.center.x:TR_MIN.x;
		TR_MAX.x=TR_MAX.x<box.center.x?box.center.x:TR_MAX.x;

		TR_MIN.y=TR_MIN.y>box.size.width?box.size.width:TR_MIN.y;
		TR_MAX.y=TR_MAX.y<box.size.width?box.size.width:TR_MAX.y;

		isTRset=true;
	}
}

int Calibrate::topLeft(CvBox2D box){
	cTL++;
	if(cTL>5){//ignore first few data
		TL_MIN.x=TL_MIN.x>box.center.x?box.center.x:TL_MIN.x;
		TL_MAX.x=TL_MAX.x<box.center.x?box.center.x:TL_MAX.x;

		TL_MIN.y=TL_MIN.y>box.size.width?box.size.width:TL_MIN.y;
		TL_MAX.y=TL_MAX.y<box.size.width?box.size.width:TL_MAX.y;

		isTLset=true;
	}
}

int Calibrate::bottomRight(CvBox2D box){
	cBR++;

	if(cBR>5){//ignore first few data
		BR_MIN.x=BR_MIN.x>box.center.x?box.center.x:BR_MIN.x;
		BR_MAX.x=BR_MAX.x<box.center.x?box.center.x:BR_MAX.x;

		BR_MIN.y=BR_MIN.y>box.size.width?box.size.width:BR_MIN.y;
		BR_MAX.y=BR_MAX.y<box.size.width?box.size.width:BR_MAX.y;

		isBRset=true;
	}
}

int Calibrate::bottomLeft(CvBox2D box){
	cBL++;

	if(cBL>5){//ignore first few data
		BL_MIN.x=BL_MIN.x>box.center.x?box.center.x:BL_MIN.x;
		BL_MAX.x=BL_MAX.x<box.center.x?box.center.x:BL_MAX.x;

		BL_MIN.y=BL_MIN.y>box.size.width?box.size.width:BL_MIN.y;
		BL_MAX.y=BL_MAX.y<box.size.width?box.size.width:BL_MAX.y;

		isBLset=true;
	}
}

CvPoint Calibrate::getTopLeftMin(){
	return TL_MIN;
}
CvPoint Calibrate::getTopLeftMax(){
	return TL_MAX;
}
CvPoint Calibrate::getTopRightMin(){
	return TR_MIN;
}
CvPoint Calibrate::getTopRightMax(){
	return TR_MAX;
}
CvPoint Calibrate::getBottomRightMin(){
	return BR_MIN;
}
CvPoint Calibrate::getBottomRightMax(){
	return BR_MAX;
}
CvPoint Calibrate::getBottomLeftMin(){
	return BL_MIN;
}
CvPoint Calibrate::getBottomLeftMax(){
	return BL_MAX;
}


bool Calibrate::isCalibrated(){
	return CALIBRATION_STATUS;
}

void Calibrate::setCalibration(bool value){
	CALIBRATION_STATUS=value;
}

Calibrate::~Calibrate() {
	// TODO Auto-generated destructor stub
}

/*
 * EyeTracking.cpp
 *
 *  Created on: Dec 11, 2009
 *      Author: sushil
 */

#include "EyeTracking.h"

float heightEye; // it's actually width (as we see)

EyeTracking::EyeTracking() {
	// TODO Auto-generated constructor stub

}
bool EyeTracking::trackCamShift(IplImage* image, CvPoint *center, int radius){
	CamShiftTracking camshift,camshiftEye;
	CvRect selectionIris,selectionEye;
	int r;
	if(radius<10){
		radius=20;
		r=0;
	}else{
		r=radius;
	}

	selectionIris.x=center->x-r;
	selectionIris.y=center->y-r;
	selectionIris.height=radius*2;
	selectionIris.width=radius*2;


	/*********TO TRACK EYE as a whole*/
	selectionEye.x=center->x-radius*2-5;
	selectionEye.y=center->y-radius-5;
	selectionEye.height=radius*2+15;
	selectionEye.width=radius*2*2+15;
	/*********TO TRACK EYE as a whole*/


	//cvRectangle(image,cvPoint(selection.x,selection.y),cvPoint(selection.x+selection.width,selection.y+selection.height),CV_RGB(255,0,0),1,8,0);
//	vRectangle( img, pt1, pt2, CV_RGB(255,0,0), 3, 8, 0 )
//	cvShowImage("aaa",image);
//	cvWaitKey(0);
	 /************/
	cout<<"Time for CamShift"<<endl;
	CvBox2D boxIris,boxEye;

	try{
		camshift.setVmax(50);
		boxIris=camshift.track(image,selectionIris);

		camshiftEye.setVmax(75);
		boxEye=camshiftEye.track(image,selectionEye);
	}catch(...){
		cerr<<"Some Error";
		return false; // is exception generated when tracking fails??
	}

	MoveMouse m;
	CvPoint XY=this->locateCoordinates(boxIris,boxEye);

	cout<<"mouseX="<<XY.x<<"\nmouseY="<<XY.y<<endl;

	m.moveMouse(XY.x,XY.y);;

	int c=cvWaitKey(10);
	switch( (char) c ){
		case 'r': // To reset to detection mode
			return false;
		default:
			;
	}

	return true;
}

CvPoint EyeTracking::locateCoordinates(CvBox2D boxIris,CvBox2D boxEye){
	float xIris=boxIris.center.x;
	float yIris=boxIris.center.y;
	float angleIris=boxIris.angle;
	float heightIris=boxIris.size.height;
	float widthIris=boxIris.size.width;

	float xEye=boxEye.center.x;
	float yEye=boxEye.center.y;
	float angleEye=boxEye.angle;
	float heightEye=boxEye.size.height;
	float widthEye=boxEye.size.width;

	int resX=1280;
	int resY=800;

	cout<<"Iris:";
	cout<<"\nX: "<<xIris<<", Y:"<<yIris;
	cout<<"\nAngle: "<<angleIris;
	cout<<"\nH: "<<heightIris<<", W:"<<widthIris<<endl;

	cout<<"\n Eye:";
	cout<<"\nX: "<<xEye<<", Y:"<<yEye;
	cout<<"\nAngle: "<<angleEye;
	cout<<"\nH: "<<heightEye<<", W:"<<widthEye<<endl;

	cout<<"\n Difference:";
	cout<<"\nX: "<<(xEye-xIris)<<", Y:"<<(yEye-yIris);
	cout<<"\nAngle: "<<(angleEye-angleIris);
	cout<<"\nH: "<<(heightEye-heightIris)<<", W:"<<(widthEye-widthIris)<<endl;

	CvPoint blocks[6] ={cvPoint(resX/6,resY/4),cvPoint(resX*3/6,resY/4),cvPoint(resX*5/6,resY/4),
					  cvPoint(resX/6,resY*3/4),cvPoint(resX*3/6,resY*3/4),cvPoint(resX*5/6,resY*3/4)};

//	if(yIris<=85){ // top
//		if(xIris>=110 && xIris<=120)
//			return blocks[1];//top-middle;
//		else if(xIris>=130 && (angleIris>=3 && angleIris<=40))
//			return blocks[0];//top-left corner;
//		else if(xIris<=90 && (angleIris>=-60 && angleIris<=-20))
//			return blocks[2];//top-right corner;
//	}else if(yIris>=90){ //bottom
//		if(xIris>110 && xIris<120)
//			return blocks[4];//bottom-middle;
//		else if(xIris>=115 && (angleIris>=-10 && angleIris<=15))
//			return blocks[3];//bottom-left corner;
//		else if(xIris<=110 && (angleIris>=-10 && angleIris<=3))
//			return blocks[5];//bottom-right corner;
//	}


	if((xEye-xIris)<9)
		return blocks[0];
//	else if((xEye-xIris)>9 && (xEye-xIris)<18)
//		return blocks[1];
	else if((xEye-xIris)>18)
		return blocks[2];

//	if((angleIris)<-10)
//		return blocks[1];
//	else if((angleIris)<0)
//		return blocks[0];
//	else if((angleIris)>0)
//		return blocks[2];

	return blocks[4];
}

bool EyeTracking::trackLucasKanade(IplImage* img, CvPoint *center, int *add_remove_pt, bool *isIrisSet){
	LucasKanadeTracking lucaskanade;
	lucaskanade.track(img,center,add_remove_pt,isIrisSet);
	return isIrisSet;
}

EyeTracking::~EyeTracking() {
	// TODO Auto-generated destructor stub
}

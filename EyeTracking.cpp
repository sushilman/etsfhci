/*
 * EyeTracking.cpp
 *
 *  Created on: Dec 11, 2009
 *      Author: sushil
 */

#include "EyeTracking.h"

int IRIS_VMAX=12,EYE_VMAX=150;
int count_discarded=0;
Calibrate calibrate;

CvPoint currentBlock;

EyeTracking::EyeTracking() {
	currentBlock=cvPoint(0,0);
	// TODO Auto-generated constructor stub

}
bool EyeTracking::trackCamShift(IplImage* image, CvPoint *center, int radius){
	CamShiftTracking camshiftEye;
	CamShiftIris camshift;
	CvRect selectionIris,selectionEye;

	/*********TO TRACK EYE as a whole*/
	selectionEye.x=center->x-radius*2-5;
	selectionEye.y=center->y-radius-5;
	selectionEye.height=radius*2+10;
	selectionEye.width=radius*2*2+15;
	/*********end of TO TRACK EYE as a whole*/


	//cvRectangle(image,cvPoint(selection.x,selection.y),cvPoint(selection.x+selection.width,selection.y+selection.height),CV_RGB(255,0,0),1,8,0);
//	vRectangle( img, pt1, pt2, CV_RGB(255,0,0), 3, 8, 0 )
//	cvShowImage("aaa",image);
//	cvWaitKey(0);
	 /************/
	//cout<<"Time for CamShift"<<endl;
	CvBox2D boxIris,boxEye;

	try{
		camshiftEye.setVmax(EYE_VMAX);
			//cvSetImageROI(image,cvRect(0,0,image->width,image->height));
			boxEye=camshiftEye.track(image,selectionEye,false);
			cout<<"EyeBox: "<<boxEye.angle<<"\n"<<endl;
			//boxEye.angle=0; // setting angle to 0
//
//			if(!isHeightNoted){
//				heightEye=boxEye.size.height;
//				initial_widthEye=boxEye.size.width;
//				isHeightNoted=true;
//			}
//
//			int x=boxEye.center.x-boxEye.size.height/2 -20;
//			int y=boxEye.center.y-boxEye.size.width/2 - 10;
//			int wd=boxEye.size.height+60;
//			//int wd=heightEye+80;
//			int ht=wd/2;
//			CvRect eyeRect=cvRect(x,y,wd,ht);
		//cvResetImageROI(image);
//		cvSetImageROI(image,eyeRect);

//		IplImage* temp2= cvCreateImage( cvSize(128,64), 8, 3 );
//		cvResize(image,temp2,CV_INTER_LINEAR);

		//cvShowImage("Only Eye",temp2);
		//cvWaitKey(0);

//		int r;
//		if(radius<10){
//			radius=20;
//			r=0;
//		}else{
//			r=radius;
//		}
		selectionIris.x=center->x-radius;
		selectionIris.y=center->y-radius;
		selectionIris.height=radius*2;
		selectionIris.width=radius*2;

		camshift.setVmax(IRIS_VMAX);


		int c=cvWaitKey(10);
		switch( (char) c ){
	//////// TO ADJUST VMAX threshold in runtime /////////////////////
	//		case 'm':
	//			cout<<"IRIS_VMAX:"<<IRIS_VMAX<<endl;
	//			if(IRIS_VMAX<240)
	//				IRIS_VMAX+=5;
	//			if(EYE_VMAX<240)
	//				EYE_VMAX+=5;
	//			break;
	//		case 'n':
	//			cout<<"IRIS_VMAX:"<<IRIS_VMAX<<endl;
	//			if(IRIS_VMAX>10)
	//				IRIS_VMAX-=5;
	//			if(EYE_VMAX>10)
	//				EYE_VMAX-=5;
	//			break;
	//////// end of TO ADJUST VMAX threshold in runtime /////////////////////

			case 'r': // To reset to detection mode
				calibrate.init(); // reset all values for re-calibration
				return false;
			default:
				;
		}


		try{
			//cvSmooth(image,image,CV_GAUSSIAN,5,5);
			boxIris=camshift.track(image,selectionIris,true);

			//Discard ambiguous data here
			/*
			 * Discard ambiguous data
			 */
			if((boxIris.center.y>128 && boxIris.center.x>256) || boxIris.size.height>180 || boxIris.size.height<=1 || boxIris.size.width<=1){
				cout<<"Discarding this data";
				cout<<"Iris:";
				cout<<"\nX: "<<boxIris.center.x<<", Y:"<<boxIris.center.y;
				cout<<"\nAngle: "<<boxIris.angle;
				cout<<"\nH: "<<boxIris.size.height<<", W:"<<boxIris.size.width;
				cout<<"\n\n"<<endl;
				count_discarded++;
				if(count_discarded>12){ // if 10 consecutive data are discarded detect again
					count_discarded=0;
					//cvReleaseImage(&image);
					return false;
				}
				return true;
			}else{
				count_discarded=0;
			}


			if(!calibrate.isCalibrated()){
				cout<<"CALIBRATION MODE:"<<endl;
				calibrate.startCalibration(boxIris,boxEye);
				return true;
			}

		}catch(...){
			//cvReleaseImage(&temp2);
			cerr<<"Iris not detected!!"<<endl;
		}
		//cvReleaseImage(&temp2);
	}catch(...){
		cerr<<"Some Error";
		calibrate.setCalibration(false);
		return false; // is exception generated when tracking fails??
	}

	MoveMouse m;
	cout<<"Top Left"<<calibrate.getTopLeftMin().x<<" - "<<calibrate.getTopLeftMax().x<<endl;
	cout<<"Top Right"<<calibrate.getTopRightMin().x<<" - "<<calibrate.getTopRightMax().x<<endl;
	cout<<"Bottom Left"<<calibrate.getBottomLeftMin().x<<" - "<<calibrate.getBottomLeftMax().x<<endl;
	cout<<"Bottom Right"<<calibrate.getBottomRightMin().x<<" - "<<calibrate.getBottomRightMax().x<<endl;

	cout<<"\n\nTop Left"<<calibrate.getTopLeftMin().y<<" - "<<calibrate.getTopLeftMax().y<<endl;
	cout<<"Top Right"<<calibrate.getTopRightMin().y<<" - "<<calibrate.getTopRightMax().y<<endl;
	cout<<"Bottom Left"<<calibrate.getBottomLeftMin().y<<" - "<<calibrate.getBottomLeftMax().y<<endl;
	cout<<"Bottom Right"<<calibrate.getBottomRightMin().y<<" - "<<calibrate.getBottomRightMax().y<<endl;

	//cvWaitKey(0);

	CvPoint XY=this->locateCoordinates(boxIris,boxEye,
										calibrate.getTopLeftMin(), calibrate.getTopLeftMax(),
										calibrate.getTopRightMin(), calibrate.getTopRightMax(),
										calibrate.getBottomRightMin(), calibrate.getBottomRightMax(),
										calibrate.getBottomLeftMin(), calibrate.getBottomLeftMax()
										); // pass thresholds as arguments?

	cout<<"mouseX="<<XY.x<<"\nmouseY="<<XY.y<<endl;

	m.moveMouse(XY.x,XY.y);

	return true;
}

CvPoint EyeTracking::locateCoordinates(CvBox2D boxIris,CvBox2D boxEye,CvPoint topLeftMin,CvPoint topLeftMax, CvPoint topRightMin,CvPoint topRightMax, CvPoint bottomRightMin,CvPoint bottomRightMax, CvPoint bottomLeftMin,CvPoint bottomLeftMax){
	float xIris=boxIris.center.x;
	float yIris=boxIris.center.y;
	float angleIris=boxIris.angle;
	float heightIris=boxIris.size.height;
	float widthIris=boxIris.size.width;
	float widthEye=boxEye.size.width;

	int resX=1280;
	int resY=800;

	cout<<"Iris:";
	cout<<"\nX: "<<xIris<<", Y:"<<yIris;
	cout<<"\nAngle: "<<angleIris;
	cout<<"\nH: "<<heightIris<<", W:"<<widthIris<<endl;

	CvPoint blocks[5] ={cvPoint(0,0), cvPoint(resX/4,resY/4),cvPoint(resX*3/4,resY/4),
					  cvPoint(resX/4,resY*3/4),cvPoint(resX*3/4,resY*3/4)};


	/*
	 * Considering X and Y of Iris center
	 */

	if(xIris>topLeftMin.x){ //Left
		if(widthEye<bottomLeftMax.y) // instead of y, width (height as we perceive) wud be better
			currentBlock=blocks[3];
		else
			currentBlock=blocks[1];

	}else if(xIris<topRightMax.x){ //Right
		if(widthEye<bottomRightMax.y)
			currentBlock=blocks[4];
		else
			currentBlock=blocks[2];
	}


	return currentBlock;
}

bool EyeTracking::trackLucasKanade(IplImage* img, CvPoint *center, int *add_remove_pt, bool *isIrisSet){
	LucasKanadeTracking lucaskanade;
	lucaskanade.track(img,center,add_remove_pt,isIrisSet);
	return isIrisSet;
}

EyeTracking::~EyeTracking() {
	// TODO Auto-generated destructor stub
}

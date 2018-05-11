#include <stdio.h>
#include "icp.h"
#include <highgui.h>

int main(int,char **) {
	int i, num_pts ;
	std::vector<CvPoint2D32f> ref_points;
	std::vector<CvPoint2D32f> new_points;
	IplImage * image_base ;
	IplImage * image ;
	num_pts = 200;
	image_base = cvCreateImage(cvSize(500,500),8,3);
	image = cvCreateImage(cvSize(500,500),8,3);
	cvZero(image_base);
	double norm = 200;
	float a = 0.;
	for( i=0; i<num_pts; i++ ) {
		float xx = (float)(norm/2.f)*cos(a);
		float yy = (float)(norm)*sin(a);
		float x = (float)(xx * cos(CV_PI/4) + yy *sin(CV_PI/4) +250);
		float y = (float)(xx * -sin(CV_PI/4) + yy *cos(CV_PI/4)+250);
		ref_points.push_back(cvPoint2D32f(x,y));
		cvDrawCircle(image_base,cvPoint((int)x,(int)y),1,CV_RGB(0,255,255),1);
		a += (float)(2*CV_PI/num_pts);
	}
	a = 0.;
	for( i=0; i< num_pts/5; i++ ) {
		float xx = (float)((norm/1.9)*cos(a));
		float yy = (float)((norm/1.1)*sin(a));
		float x = (float)(xx * cos(-CV_PI/8) + yy *sin(-CV_PI/8) +150);
		float y = (float)(xx * -sin(-CV_PI/8) + yy *cos(-CV_PI/8)+250);
		new_points.push_back(cvPoint2D32f(x,y));
		a += (float)(2*CV_PI/(float)(num_pts/5));
		cvDrawCircle(image_base,cvPoint((int)x,(int)y),1,CV_RGB(255,255,0),1);
	}

    float R[4] = {1.f,0.f,0.f,1.f},T[2] = {0.,0.};
    CvMat r = cvMat(2,2,CV_32F,R);
    CvMat t = cvMat(2,1,CV_32F,T);
	cvCopy(image_base,image);
	float err = icp(&new_points[0],new_points.size(),
                    &ref_points[0],ref_points.size(),
                    &r,&t,cvTermCriteria(CV_TERMCRIT_ITER,30,0.1));
	printf("err = %f\n",err);
	for(int i = 0; i < (int)new_points.size() ; i++ ) {
		float x = new_points[i].x;
		float y = new_points[i].y;
		float X = (R[0]*x + R[1]*y + T[0]);
		float Y = (R[2]*x + R[3]*y + T[1]);
		new_points[i].x = X;
		new_points[i].y = Y;
		cvDrawCircle(image,cvPoint((int)X,(int)Y),5,CV_RGB(255,0,255),1);
	}
    printf("Final transformation : \n");
    printf("R[0]=%f R[1]=%f T[0]=%f\n",R[0],R[1],T[0]);
    printf("R[2]=%f R[3]=%f T[1]=%f\n",R[2],R[3],T[1]);
	cvShowImage("image",image);
	cvWaitKey(0);
    cvReleaseImage(&image_base);
	cvReleaseImage(&image);
	return 0;
}


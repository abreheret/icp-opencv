#include <cxcore.h>
#include <highgui.h>
#include "kdtree.h"

static void getRTMatrixSVD( const CvPoint2D32f* a, const CvPoint2D32f* b, int count ,float R[4],float T[2]) {
	int i;
	float H[4] = {0.f,0.f,0.f,0.f};
	float U[4] = {0.f,0.f,0.f,0.f};
	float W[4] = {0.f,0.f,0.f,0.f};
	float V[4] = {0.f,0.f,0.f,0.f};
	CvMat h = cvMat(2,2,CV_32F,H);
	CvMat u = cvMat(2,2,CV_32F,U);
	CvMat w = cvMat(2,2,CV_32F,W);
	CvMat v = cvMat(2,2,CV_32F,V);
	CvPoint2D32f mean_a = cvPoint2D32f(0.f,0.f);
	CvPoint2D32f mean_b = cvPoint2D32f(0.f,0.f);
	for( i = 0 ; i < count ; i ++) {
		mean_a.x += a[i].x;
		mean_a.y += a[i].y;
		mean_b.x += b[i].x;
		mean_b.y += b[i].y;
	}
	mean_a.x /= (float)count;
	mean_a.y /= (float)count;
	mean_b.x /= (float)count;
	mean_b.y /= (float)count;

	for( i = 0 ; i < count ; i ++) {
		float AX = (a[i].x-mean_a.x);
		float AY = (a[i].y-mean_a.y);
		float BX = (b[i].x-mean_b.x);
		float BY = (b[i].y-mean_b.y);
		H[0] += AX*BX;
		H[1] += AX*BY;
		H[2] += AY*BX;
		H[3] += AY*BY;
	}
	cvSVD(&h,&w,&u,&v,CV_SVD_MODIFY_A);
	// R = V * traspose(U)
	R[0] = V[0]*U[0] + V[1]*U[1];
	R[1] = V[0]*U[2] + V[1]*U[3];
	R[2] = V[2]*U[0] + V[3]*U[1];
	R[3] = V[2]*U[2] + V[3]*U[3];

	// T = -R*meanA+meanB
	T[0] = -R[0]*mean_a.x - R[1]*mean_a.y + mean_b.x;
	T[1] = -R[2]*mean_a.x - R[3]*mean_a.y + mean_b.y;
}

/* returns the distance squared between two dims-dimensional double arrays */
static double dist_sq( double *a1, double *a2, int dims ) {
	double dist_sq = 0, diff;
	while( --dims >= 0 ) {
		diff = (a1[dims] - a2[dims]);
		dist_sq += diff*diff;
	}
	return dist_sq;
}

int main_icp() {
	int i, num_pts ;
	void *ptree;
	char *pch;
	struct kdres *presults;
	double pos[3];
	std::vector<CvPoint2D32f> pt0;
	std::vector<CvPoint2D32f> pt1;
	IplImage * image_base ;
	IplImage * image ;
	num_pts = 200;
	image_base = cvCreateImage(cvSize(500,500),8,3);
	image = cvCreateImage(cvSize(500,500),8,3);
	cvZero(image_base);
	CvMat *   h = cvCreateMat(2,3, CV_64F);
	ptree = kd_create( 3 );
	double norm = 200;
	double a = 0.;
	for( i=0; i<num_pts; i++ ) {
		float xx = (norm/2.)*cos(a);
		float yy = (norm)*sin(a);
		float x = xx * cos(CV_PI/4) + yy *sin(CV_PI/4) +250;
		float y = xx * -sin(CV_PI/4) + yy *cos(CV_PI/4)+250;
		pt0.push_back(cvPoint2D32f(x,y));
		cvDrawCircle(image_base,cvPoint((int)x,(int)y),1,CV_RGB(0,255,255),1);
		assert( 0 == kd_insert3f((kdtree*) ptree, x, y, 0., 0) );
		a += 2*CV_PI/(float)(num_pts);
	}
	a = 0.;
	for( i=0; i< num_pts/5; i++ ) {
		float xx = (norm/1.9)*cos(a);
		float yy = (norm/1.1)*sin(a);
		float x = xx * cos(-CV_PI/8) + yy *sin(-CV_PI/8) +150;
		float y = xx * -sin(-CV_PI/8) + yy *cos(-CV_PI/8)+250;
		pt1.push_back(cvPoint2D32f(x,y));
		a += 2*CV_PI/(float)(num_pts/5);
		cvDrawCircle(image_base,cvPoint((int)x,(int)y),1,CV_RGB(255,255,0),1);
	}
	for(;;) {
		cvCopy(image_base,image);
		double err = 0.;
		pt0.clear();
		for( i = 0 ; i < pt1.size() ; i++) {
			double pt[3] = {  pt1[i].x,  pt1[i].y , 0 };
			cvDrawCircle(image,cvPoint((int)pt[0],(int)pt[1]),3,CV_RGB(0,255,0),1);
			presults =  kd_nearest( (kdtree *)ptree, pt);
			//printf("%d\n",presults);
			kd_res_end( presults );
			pch = (char*)kd_res_item( presults, pos );
			err += sqrt( dist_sq( pt, pos, 3 ) );

			cvDrawLine(image,cvPoint((int)pos[0],(int)pos[1]),cvPoint((int)pt[0],(int)pt[1]),CV_RGB(0,0,255));
			cvDrawCircle(image,cvPoint((int)pos[0],(int)pos[1]),3,CV_RGB(255,0,0),1);
			pt0.push_back(cvPoint2D32f(pos[0],pos[1]));
		}

		float R[4],T[2];
		getRTMatrixSVD(&pt1[0],&pt0[0],pt1.size(),R,T);
		printf("err = %f\n",err);
		for(int i = 0; i < pt1.size() ; i++ ) {
			double x = pt1[i].x;
			double y = pt1[i].y;
			double X = (R[0]*x + R[1]*y + T[0]);
			double Y = (R[2]*x + R[3]*y + T[1]);
			pt1[i].x = X;
			pt1[i].y = Y;
			cvDrawCircle(image,cvPoint((int)X,(int)Y),5,CV_RGB(255,255,0),1);
		}
		cvShowImage("image",image);

		if( cvWaitKey(0)== 27) break;
	}
	kd_res_free( presults );
	kd_free( (kdtree *)ptree );
	cvReleaseImage(&image);
	cvReleaseMat(&h);

	return 0;
}


int main(int,char **) {
	main_icp();
}


#include "icp.h"
// return the rigid transformation (rotation and tranlation matrix), for more inforation see http://nghiaho.com/?page_id=671
static void getRTMatrixSVD( const CvPoint2D32f* a, const CvPoint2D32f* b, int count ,CvMat *r,CvMat *t) {
	int i;
	float H[4] = {0.f,0.f,0.f,0.f}; CvMat h = cvMat(2,2,CV_32F,H);
	float U[4] = {0.f,0.f,0.f,0.f}; CvMat u = cvMat(2,2,CV_32F,U);
	float W[4] = {0.f,0.f,0.f,0.f}; CvMat w = cvMat(2,2,CV_32F,W);
	float V[4] = {0.f,0.f,0.f,0.f}; CvMat v = cvMat(2,2,CV_32F,V);
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

	{// get result :
		float * R = r->data.fl;
		float * T = t->data.fl;
		// R = V * transpose(U)
		R[0] = V[0]*U[0] + V[1]*U[1];
		R[1] = V[0]*U[2] + V[1]*U[3];
		R[2] = V[2]*U[0] + V[3]*U[1];
		R[3] = V[2]*U[2] + V[3]*U[3];
		
		// special reflection case
		if ( cvDet(r) < 0 ) {
			R[0] = V[0]*U[0] - V[1]*U[1];
			R[1] = V[0]*U[2] - V[1]*U[3];
			R[2] = V[2]*U[0] - V[3]*U[1];
			R[3] = V[2]*U[2] - V[3]*U[3];
		}

		// T = -R*meanA+meanB
		T[0] = -R[0]*mean_a.x - R[1]*mean_a.y + mean_b.x;
		T[1] = -R[2]*mean_a.x - R[3]*mean_a.y + mean_b.y;
	}
}

/* returns the distance squared between two dims-dimensional double arrays */
static float dist_sq( float *a1, float*a2, int dims ) {
	float dist_sq = 0, diff;
	while( --dims >= 0 ) {
		diff = (a1[dims] - a2[dims]);
		dist_sq += diff*diff;
	}
	return dist_sq;
}

float icp(const CvPoint2D32f* new_points,int nb_point_new,
          const CvPoint2D32f* ref_points,int nb_point_ref,
          CvMat * r_final,CvMat *t_final,
	      CvTermCriteria criteria,IplImage * image) {
	int k,i;
	float prev_err = (float)9e33;
	float err;
	struct kdtree *ptree = kd_create( 2 );
	CvPoint2D32f * input_correlation_old = (CvPoint2D32f *)malloc(sizeof(CvPoint2D32f)*nb_point_new );
	CvPoint2D32f * input_correlation_new = (CvPoint2D32f *)malloc(sizeof(CvPoint2D32f)*nb_point_new );
    
	r_final->data.fl[0] = 1.f; r_final->data.fl[1] = 0.f;
	r_final->data.fl[2] = 0.f; r_final->data.fl[3] = 1.f;
	t_final->data.fl[0] = 0.f;
 	t_final->data.fl[1] = 0.f;

	for( i = 0; i < nb_point_ref; i++ ) 
		kd_insertf((struct kdtree*) ptree, (float*)&ref_points[i], 0);

	for( i = 0; i < nb_point_new; i++ )
		input_correlation_new[i] = new_points[i];

	for ( k = 0 ; k < criteria.max_iter; k++ ) {
		float R[4]; CvMat r = cvMat(2,2,CV_32F,R);
		float T[2]; CvMat t = cvMat(2,1,CV_32F,T);

		err = 0.;
		for( i = 0 ; i < nb_point_new ; i++) {
			struct kdres * presults = kd_nearestf( (struct kdtree *)ptree, (float*)&input_correlation_new[i]);
			kd_res_end( presults );
			kd_res_itemf( presults, (float*)&input_correlation_old[i] );
			err += sqrtf( dist_sq( (float*)&input_correlation_old[i], (float*)&input_correlation_new[i], 2 ) );
			if( image ) {
				cvDrawCircle(image,cvPoint((int)input_correlation_new[i].x,(int)input_correlation_new[i].y),3,CV_RGB(0,255,0),1,8,0);
				cvDrawLine(image,cvPoint((int)input_correlation_old[i].x,(int)input_correlation_old[i].y),
					             cvPoint((int)input_correlation_new[i].x,(int)input_correlation_new[i].y),CV_RGB(0,0,255),1,8,0);
				cvDrawCircle(image,cvPoint((int)input_correlation_old[i].x,(int)input_correlation_old[i].y),3,CV_RGB(255,0,0),1,8,0);
			}
			kd_res_free( presults );
		}
		getRTMatrixSVD(&input_correlation_new[0],&input_correlation_old[0],nb_point_new,&r,&t);
		for(i = 0; i < nb_point_new ; i++ ) {
			float x = input_correlation_new[i].x;
			float y = input_correlation_new[i].y;
			float X = (R[0]*x + R[1]*y + T[0]);
			float Y = (R[2]*x + R[3]*y + T[1]);
			input_correlation_new[i].x = X;
			input_correlation_new[i].y = Y;
		}
		if ( fabs(err - prev_err) < criteria.epsilon )  break;
		else prev_err = err;
    }
    getRTMatrixSVD(&new_points[0],&input_correlation_new[0],nb_point_new,r_final,t_final);
	
	kd_free( ptree );
	free(input_correlation_old);
	free(input_correlation_new);
	return err;
}
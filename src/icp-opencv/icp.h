#ifndef __icp_h__
#define __icp_h__

#include <cxcore.h>
#include "kdtree.h"


#ifdef __cplusplus
extern "C" {
#endif

//= CV_cvTermCriteria( CV_TERMCRIT_ITER, 10, 0.1 )
// return err
float icp(const CvPoint2D32f* new_pt,int nb_point_new_pt, const CvPoint2D32f* old_pt,int nb_point_old_pt,
	CvTermCriteria criteria,CvMat * r_final,CvMat *t_final, IplImage * image CV_DEFAULT(NULL)) ;

#ifdef __cplusplus
}
#endif
#endif //__icp_h__
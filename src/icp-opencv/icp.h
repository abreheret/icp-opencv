#ifndef __icp_h__
#define __icp_h__

#include <cxcore.h>
#include "kdtree.h"


#ifdef __cplusplus
extern "C" {
#endif

    
// Compute icp with 2 set points (between ref points and new points)
// inputs :
//   - new_points   : set of new input points (source point clouds)
//   - nb_point_new : number of input new points
//   - ref_points   : set of reference input points.
//   - nb_point_ref : number of input old points
//   - criteria     : default is cvTermCriteria( CV_TERMCRIT_ITER, nb_max_iteration, stop_epsilon )
//   - image        : optional (for draw nearest points )
// output 2 matrix :
//   - r_out : rotation matrix, 2x2 with type = CV_32F
//   - t_out : translation maxtrix 2x1 with type = CV_32F
// return :
//   - err : cumulative sum of distance points
float icp(const CvPoint2D32f* new_points,int nb_point_new,
          const CvPoint2D32f* ref_points,int nb_point_ref,
          CvMat * r_out,CvMat *t_out,
          CvTermCriteria criteria, IplImage * image CV_DEFAULT(NULL)) ;

#ifdef __cplusplus
}
#endif
#endif //__icp_h__
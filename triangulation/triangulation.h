#ifndef MESHING_H_
#define MESHING_H_

#include <Eigen/Dense>

namespace  jy_mesh {
int triangulate_polygon(const Eigen::Matrix2Xd &V, //position of boundary verts
                        const Eigen::Matrix2Xi &E, //id for boundarye edges
                        const Eigen::Matrix2Xd &H, //position for verts of of inner boundary
                        double max_area,  //maximum area constraint
                        double min_angle, //minmial angle constraint
                        Eigen::Matrix2Xd &NV, //position of output verts
                        Eigen::Matrix3Xi &T,  //id of output triangles
                        Eigen::Matrix2Xi &NS); //id of output segmentations
}


#endif

//
// Created by 徐溶延 on 2019/9/11.
//
#include <Eigen/Dense>
#include <pmp/SurfaceMesh.h>
#include "../contour/ContourCollection.h"
#include "triangulation.h"

using namespace Eigen;
using namespace pmp;
using namespace jy_mesh;

namespace xry_mesh {
    Vector2f calcBarycenter(Matrix2Xf &points);

    void prepareVertex(ContourCollection& contours, Matrix2Xd& V);

    void prepareSegmentation(ContourCollection& contours, Matrix2Xi& segmentation);

    void prepareHoles(ContourCollection& contours, Matrix2Xd& holes);

    void prepareDataForTriangulation(ContourCollection& contours, Matrix2Xd& V, Matrix2Xi& S, Matrix2Xd& H);

    SurfaceMesh triangulate(ContourCollection& contours);
}

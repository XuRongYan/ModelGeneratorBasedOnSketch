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

    void prepareVertex(ContourCollection &contours, Matrix2Xd &V);

    void prepareSegmentation(ContourCollection &contours, Matrix2Xi &segmentation);

    void prepareHoles(ContourCollection &contours, Matrix2Xd &holes);

    void prepareDataForTriangulation(ContourCollection &contours, Matrix2Xd &V, Matrix2Xi &S, Matrix2Xd &H);

    SurfaceMesh triangulate(ContourCollection &contours);

    SurfaceMesh refineMesh(const SurfaceMesh &mesh, const vector<pmp::Point> &details);

    void addEdgeProperty(SurfaceMesh &mesh, const Matrix2Xi &segmentation, const vector<Vertex> &vids);

    void findNewContours(SurfaceMesh &mesh, vector<Vertex> vertices, size_t segmentNum, ContourCollection &contours);

    void prepareVertex(const SurfaceMesh &mesh, Matrix2Xd &V);

    void prepareSegmentations(const SurfaceMesh &mesh, Matrix2Xi &E);

    void prepareTriangles(const SurfaceMesh &mesh, Matrix3Xi &T);

    void prepareDataForRefinement(const SurfaceMesh &mesh, Matrix2Xd &V, Matrix2Xi &S, Matrix3Xi &T);

    void buildMesh(SurfaceMesh &mesh, Matrix2Xd points2D, Matrix3Xi triangles, Matrix2Xi segmentations,
                   vector<pmp::Vertex> &vecVertex);

    void prepareMeshBoundary(const SurfaceMesh& mesh, Eigen::Matrix2Xd &points, Eigen::Matrix2Xi &segments, const vector<pmp::Point> &details);
}

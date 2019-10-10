//
// Created by 徐溶延 on 2019/9/11.
//
#include <gtest/gtest.h>
#include "../triangulation/TriangulationUtil.h"
#include "../io/MeshIO.h"
using namespace xry_mesh;

class TriangulationUtilTest : public ::testing::Test {
public:
    Matrix2Xf m1;
    Matrix3Xf m3, m4;
    void prepare_simple_points() {
        m1.resize(2, 4);
        m1 << 0, 1, 1, 0,
              0, 0, 1, 1;
    }

    ContourCollection prepare_collection() {
        m3.resize(3, 4);
        m3 << 0, 3, 3, 0,
              0, 0, 3, 3,
              0, 0, 0, 0;
        m4.resize(3, 4);
        m4 << 1, 2, 2, 1,
              1, 1, 2, 2,
              0, 0, 0, 0;
        vector<Contour> vecContour = {Contour(m3, IS_OUTSKIRT), Contour(m4)};
        return ContourCollection(vecContour);
    }
};

TEST_F(TriangulationUtilTest, calcBarycenterTest) {
    prepare_simple_points();
    Vector2f p = calcBarycenter(m1);
    EXPECT_FLOAT_EQ(p[0], 0.5);
    EXPECT_FLOAT_EQ(p[1], 0.5);
}

TEST_F(TriangulationUtilTest, prepareDataTest) {
    Matrix2Xd V, H, m5(2, 8), m6(2, 1);
    Matrix2Xi S, m7(2, 8);
    ContourCollection contours = prepare_collection();
    prepareDataForTriangulation(contours, V, S, H);
    m5 << 0, 3, 3, 0, 1, 2, 2, 1,
          0, 0, 3, 3, 1, 1, 2, 2;
    m6 << 1.5,
          1.5;
    m7 << 1, 2, 3, 0, 5, 6, 7, 4,
          0, 1, 2, 3, 4, 5, 6, 7;
    EXPECT_EQ(V.cols(), 8);
    EXPECT_EQ(V, m5);
    EXPECT_EQ(S, m7);
    EXPECT_EQ(H, m6);
}

TEST_F(TriangulationUtilTest, TriangulateTest) {
    ContourCollection contours = prepare_collection();
    SurfaceMesh mesh = triangulate(contours);
    MeshIO::save_mesh(mesh, "triangulation.obj");
    MeshIO::show_mesh("triangulation.obj");
}

TEST_F(TriangulationUtilTest, TriangulationOrderTest) {
    ContourCollection contours = prepare_collection();
    SurfaceMesh mesh = triangulate(contours);
    size_t idx = 0;
    vector<Contour> vecContours = contours.getContours();
    size_t maxId = vecContours[0].rows();
    for(auto v : mesh.vertices()) {
        if(idx >= maxId) break;
        const pmp::Point& p = mesh.position(v);
        EXPECT_FLOAT_EQ(p[0], vecContours[0].getX(idx));
        EXPECT_FLOAT_EQ(p[1], vecContours[0].getY(idx));
        EXPECT_FLOAT_EQ(p[2], vecContours[0].getZ(idx));
        idx++;
    }
}
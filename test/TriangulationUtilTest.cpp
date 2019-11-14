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
    SurfaceMesh mesh;
    vector<pmp::Point> details;
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

    void prepareMesh() {
        MeshIO::read_mesh(mesh, "region2D.obj");
    }

    void prepareDetails() {
        pmp::Point p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12;
        p1 = pmp::Point(-0.969224, -8.12097, 0);
        p2 = pmp::Point(3.5033, -4.0603, 0);
        p3 = pmp::Point(7.91102, 0, 0);
        p4 = pmp::Point(8.2279, 4.6325, 0);
        p5 = pmp::Point(7.94057, 9.39549, 0);
        p6 = pmp::Point(3.2537, 10.7458, 0);
        p7 = pmp::Point(-1.239, 7.748, 0);
        p8 = pmp::Point(-9.432, 3.686, 0);
        p9 = pmp::Point(-8.768, 0.286, 0);
        p10 = pmp::Point(-9.036, -3.658, 0);
        p11 = pmp::Point(-8.778, -7.359, 0);
        p12 = pmp::Point(-4.573, -8.323, 0);
        details.push_back(p1);
        details.push_back(p2);
        details.push_back(p3);
        details.push_back(p4);
        details.push_back(p5);
        details.push_back(p6);
        details.push_back(p7);
        details.push_back(p8);
        details.push_back(p9);
        details.push_back(p10);
        details.push_back(p11);
        details.push_back(p12);
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

TEST_F(TriangulationUtilTest, TriangulationRefineTest) {
    prepareMesh();
    prepareDetails();
    SurfaceMesh result = xry_mesh::refineMesh(mesh, details);
    MeshIO::save_mesh(result, "refinedRegion2D.obj");
}
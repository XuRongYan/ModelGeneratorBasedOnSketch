//
// Created by 徐溶延 on 2019/10/8.
//
#include <gtest/gtest.h>
#include "../detail_builder/ExponentialMap.h"
#include "../contour/Contour.h"
#include "../utils/PmpUtils.h"
#include "../io/MeshIO.h"

class PmpUtilTest : public ::testing::Test {

public:
    Contour contour1, contour2, contour3, contour4;
    Matrix3Xf m1, m2, m3, m4;
    vector<Contour> contours;
    SurfaceMesh mesh, mesh1;

    void readMesh() {
        MeshIO::read_mesh(mesh, "result.obj");
    }

    void readMesh2D() {
        MeshIO::read_mesh(mesh1, "region2D.obj");
    }

    void generate_contours() {
        m1.resize(3, 4);
        m2.resize(3, 7);

        m1 << 0, 1, 1, 0,
              0, 0, 1, 1,
              0, 0, 0, 0;
        m2 << 0, 1, 1.5, 2, 3, 2, 1,
              1, 0, 1, 0, 1, 2, 2,
              0, 0, 0, 0, 0, 0, 0;

        contour1.setContour(m1);
        contour2.setContour(m2);
    }
};

TEST_F(PmpUtilTest, isVertexInsideContourTest) {
    generate_contours();
    pmp::Point p1(0.5, 0.5, 0);
    pmp::Point p2(3, 3, 0);
    pmp::Point p3(1, 1, 0);
    pmp::Point p4(1.5, 0.5, 0);
    pmp::Point p5(0, 0, 0);
    bool test1 = PmpUtils::isVertexInsideContour(p1, contour1);
    bool test2 = PmpUtils::isVertexInsideContour(p2, contour1);
    bool test3 = PmpUtils::isVertexInsideContour(p3, contour2);
    bool test4 = PmpUtils::isVertexInsideContour(p4, contour2);
    bool test5 = PmpUtils::isVertexInsideContour(p5, contour1);
    EXPECT_TRUE(test1);
    EXPECT_TRUE(test3);
    EXPECT_TRUE(test5);
    EXPECT_FALSE(test2);
    EXPECT_FALSE(test4);
}

TEST_F(PmpUtilTest, chooseVertexTest) {
    readMesh();
    pmp::Point p(160.392, -85.8431, 10.732);
    Vertex v = PmpUtils::findVertexByPosition(mesh, p);
    EXPECT_TRUE(mesh.is_valid(v));
    pmp::Point pos = mesh.position(v);
    EXPECT_EQ(v.idx(), 991);
}

TEST_F(PmpUtilTest, projectToPlaneTest) {
    pmp::Point n(0, 0, 1);
    pmp::Point p(1, 1, 1);
    pmp::Point target(1, 1, 0);
    pmp::Point projected = PmpUtils::projectToPlane(n, p);
    EXPECT_EQ(projected, target);
}

TEST_F(PmpUtilTest, pmpAngleTest) {
    const float pi = 2 * acos(0);
    pmp::Point p1(1, 0, 0);
    pmp::Point p2(1, 1, 0);
    pmp::Point p3(0, 1, 0);
    pmp::Point p4(-1, 1, 0);
    pmp::Point p5(-1, 0, 0);
    pmp::Point p6(0, -1, 0);
    pmp::Scalar a1 = ExponentialMap::angle(p1, p1);
    pmp::Scalar a2 = ExponentialMap::angle(p1, p2);
    pmp::Scalar a3 = ExponentialMap::angle(p1, p3);
    pmp::Scalar a4 = ExponentialMap::angle(p1, p4);
    pmp::Scalar a5 = ExponentialMap::angle(p1, p5);
    pmp::Scalar a6 = ExponentialMap::angle(p1, p6);
    EXPECT_FLOAT_EQ(a1, 0);
    EXPECT_FLOAT_EQ(a2, pi / 4);
    EXPECT_FLOAT_EQ(a3, pi / 2);
    EXPECT_FLOAT_EQ(a4, 3 * (pi / 4));
    EXPECT_FLOAT_EQ(a5, pi);
    EXPECT_FLOAT_EQ(a6, 1.5 * pi);
}

TEST_F(PmpUtilTest, findBoundaryTest) {
    readMesh2D();
    vector<pmp::Vertex> boundary;
    PmpUtils::findBoundary(mesh1, boundary);
    for (auto v : boundary) {
        EXPECT_TRUE(mesh1.is_boundary(v));
    }
}

TEST_F(PmpUtilTest, pointInPlaneTest) {
    pmp::Point p1(1, 1, 0), p2(4, 0, 0), p3(1, 1, 1), A(0, 0, 0), B(2, 0, 0), C(1, 2, 0);
    bool res1 = PmpUtils::pointInFace(p1, A, B, C);
    bool res2 = PmpUtils::pointInFace(p2, A, B, C);
    bool res3 = PmpUtils::pointInFace(p3, A, B, C);
    EXPECT_TRUE(res1);
    EXPECT_FALSE(res2);
    EXPECT_FALSE(res3);
}
//
// Created by 徐溶延 on 2019/9/9.
//

#include <Eigen/Core>
#include <Eigen/Dense>
#include <gtest/gtest.h>
#include "../spherical_harmonic/ContourSphericalCoordinate.h"
#include "../spherical_harmonic/MeshSphericalCoordinate.h"
#include "../spherical_harmonic/TriangulationSpericalCoordinate.h"
#include "../io/MeshIO.h"
#include "../contour/ContourCollection.h"
#include "../triangulation/TriangulationUtil.h"

using namespace std;
using namespace Eigen;
class SphericalCoordinateTest : public ::testing::Test {
public:
    Matrix3Xf m1, m2, m3, m4;
    void generate_data() {
        m1.resize(3, 6);
        m2.resize(3, 4);
        m1 << 0, 1, 2, 3, 3, 0,
              0, 0, 0, 0, 3, 3,
              0, 0, 0, 0, 0, 0;

        m2 << 1, 2, 2, 1,
              1, 1, 2, 2,
              0, 0, 0, 0;
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

TEST_F(SphericalCoordinateTest, SphericalCoordinateOutskirtTest) {
    generate_data();
    ContourSphericalCoordinate sph1(m1, 12, true);
    //SphericalCoordinate sph2(m2, 12, false);
    EXPECT_FLOAT_EQ(sph1.getPhi(0), pi / 2);
    EXPECT_FLOAT_EQ(sph1.getPhi(m1.cols() - 1), pi * 2);
    EXPECT_FLOAT_EQ(sph1.getTheta(0), pi / 2);
    EXPECT_FLOAT_EQ(sph1.getTheta(2), pi / 2);
}

TEST_F(SphericalCoordinateTest, SphericalCoordinateNotOutskirtTest) {
    generate_data();
    ContourSphericalCoordinate sph2(m2, 12, false);
    EXPECT_FLOAT_EQ(sph2.getPhi(0), pi / 4);
    EXPECT_FLOAT_EQ(sph2.getPhi(m1.cols() - 1), atan2(2, 1));
    EXPECT_FLOAT_EQ(sph2.getTheta(0), asin(sqrt(2) / 6 * pi));
}

TEST_F(SphericalCoordinateTest, MeshSphericalCoordinateTest) {
    SurfaceMesh mesh;
    MeshIO::read_mesh(mesh, "ball.obj");
    MeshSphericalCoordinate sphericalCoordinate(mesh);
    ASSERT_EQ(sphericalCoordinate.size(), mesh.n_vertices());
}

TEST_F(SphericalCoordinateTest, TriangulationCoordinateTest) {
    ContourCollection contours = prepare_collection();
    SurfaceMesh mesh = xry_mesh::triangulate(contours);
    Contour contour = contours.getContours().at(0);
    ContourSphericalCoordinate contourSphericalCoordinate(contour, contours.getSphereCircumference());
    TriangulationSpericalCoordinate spericalCoordinate(mesh, contourSphericalCoordinate.getCircumference());
    EXPECT_EQ(spericalCoordinate.size(), mesh.n_vertices());
    for(size_t i = 0; i < contour.rows(); i++) {
        EXPECT_FLOAT_EQ(spericalCoordinate.getTheta(i), pi / 2);
    }
    for(size_t i = contour.rows(); i < contours.getMergedContour().rows(); i += 2) {
        EXPECT_FLOAT_EQ(spericalCoordinate.getPhi(i), pi / 4);
    }
}





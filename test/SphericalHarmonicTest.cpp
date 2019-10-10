//
// Created by 徐溶延 on 2019/9/10.
//
#include <gtest/gtest.h>
#include <Eigen/Dense>
#include "../spherical_harmonic/ContourSphericalCoordinate.h"
#include "../spherical_harmonic/SphericalHarmonics.h"
using namespace Eigen;
class SphericalHarmonicTest : public ::testing::Test {
public:
    Matrix3Xf m1, m2;
    ContourSphericalCoordinate coordinate;
    void generate_data() {
        m1.resize(3, 4);
        m2.resize(3, 4);
        m1 << 0, 3, 3, 0,
                0, 0, 3, 3,
                0, 0, 0, 0;

        m2 << 1, 2, 2, 1,
                1, 1, 2, 2,
                0, 0, 0, 0;

        coordinate = ContourSphericalCoordinate(m1, 12, true);
    }
};

TEST_F(SphericalHarmonicTest, SphericalHarmonicGenerationTest) {
    SphericalHarmonics sph(3, &coordinate);
    MatrixXf matrix = sph.getMatrix();
    EXPECT_EQ(matrix.rows(), coordinate.size());
    EXPECT_EQ(matrix.cols(), 9);
}

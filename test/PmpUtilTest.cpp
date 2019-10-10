//
// Created by 徐溶延 on 2019/10/8.
//
#include <gtest/gtest.h>
#include "../contour/Contour.h"
#include "../utils/PmpUtils.h"

class PmpUtilTest : public ::testing::Test {

public:
    Contour contour1, contour2, contour3, contour4;
    Matrix3Xf m1, m2, m3, m4;
    vector<Contour> contours;

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
//
// Created by 徐溶延 on 2019/9/7.
//
#include <Eigen/Core>
#include <gtest/gtest.h>
#include <vector>
#include "../utils/PmpException.h"
#include "../contour/Contour.h"
#include "../contour/ContourCollection.h"
#include "../contour/ContourFactory.h"

using namespace std;
using namespace Eigen;

class ContoursTest : public ::testing::Test {

public:
    Contour contour1, contour2, contour3, contour4;
    Matrix3Xf m1, m2, m3, m4;
    vector<Contour> contours;

    void generate_contours() {
        m1.resize(3, 3);
        m2.resize(3, 3);
        m3.resize(3, 6);
        m1 << 1, 2, 3,
              4, 5, 6,
              7, 8, 9;
        m2 << 1, 0, 0,
              0, 1, 0,
              0, 0, 1;
        m3 << 1, 2, 3, 1, 0, 0,
              4, 5, 6, 0, 1, 0,
              7, 8, 9, 0, 0, 1;
        contour1.setContour(m1);
        contour2.setContour(m2);
        contour3.setContour(m3);
    }

    void generate_circum() {
        m4.resize(3, 4);
        m4 << 0, 1, 1, 0,
              0, 0, 1, 1,
              0, 0, 0, 0;
        contour4.setContour(m4, IS_OUTSKIRT);
    }

    ContourFactory generate_factory() {
        ContourLoader loader = ContourLoader::Builder()
                                            .setFileName("anim0.pgm")
                                            .setOperator(THRESHOLD)
                                            .setDraw(true)
                                            .build();
        return ContourFactory(loader, ContourFactory::IMAGE_STRATEGY);
    }

    ContourFactory generate_factory_operator_exception() {
        ContourLoader loader = ContourLoader::Builder()
                .setFileName("anim0.pgm")
                .setOperator(9)
                .setDraw(true)
                .build();
        return ContourFactory(loader, ContourFactory::IMAGE_STRATEGY);
    }

    ContourFactory generate_factory_file_exception() {
        ContourLoader loader = ContourLoader::Builder()
                .setFileName("anim0.pgm")
                .setOperator(THRESHOLD)
                .setDraw(true)
                .build();
        return ContourFactory(loader, 9);
    }

    ContourCollection generate_collections() {
        generate_contours();
        contours.push_back(contour1);
        contours.push_back(contour2);
        return ContourCollection(contours);
    }
};

TEST_F(ContoursTest, ContourMergeTest) {
    generate_contours();
    contour1.merge(contour2);
    EXPECT_EQ(contour1.rows(), 6);
    EXPECT_EQ(contour1, contour3);
}

TEST_F(ContoursTest, ContourEuqalsContourTest) {
    generate_contours();
    EXPECT_FALSE(contour1 == contour2);
}

TEST_F(ContoursTest, ContourCollectionGenerationTest) {
    ContourCollection collection = generate_collections();
    const vector<Contour>& contoursInCollection = collection.getContours();
    Contour mergedContour = collection.getMergedContour();
    EXPECT_EQ(collection.size(), 2);
    contours.push_back(contour1);
    contours.push_back(contour2);
    EXPECT_EQ(contoursInCollection, contours);
    EXPECT_EQ(mergedContour, contour3);

}

TEST_F(ContoursTest, DefaultContourFactoryTest) {
    ContourFactory factory = generate_factory();
    ContourCollection contours = factory.loadContours();
    EXPECT_EQ(contours.size(), 1);
    vector<Contour> vecContour = contours.getContours();
    for(auto contour : vecContour) {
        for(size_t i = 0; i < contour.rows(); i++) {
            EXPECT_FLOAT_EQ(contour.getZ(i), 0);
        }
    }
}

TEST_F(ContoursTest, ContourFactoryOperatorExceptionTest) {
    ContourFactory factory = generate_factory_operator_exception();
    EXPECT_THROW(factory.loadContours(), TypeNotFoundException);
}

TEST_F(ContoursTest, ContourFactoryFileTypeExceptionTest) {
    EXPECT_THROW(generate_factory_file_exception(), TypeNotFoundException);
}

TEST_F(ContoursTest, ContourCircumferenceTest) {
    generate_circum();
    EXPECT_FLOAT_EQ(contour4.getCircumference(), 4);
}

TEST_F(ContoursTest, ContourCircumferenceExceptionTest) {
    ContourFactory factory = generate_factory();
    ContourCollection contours = factory.loadContours();
    vector<Contour> vecContours = contours.getContours();
    EXPECT_NO_THROW(vecContours[0].getCircumference());
    for (int i = 0; i < vecContours.size(); ++i) {
        if (vecContours[i].isOutskirt()) {
            EXPECT_NO_THROW(vecContours[i].getCircumference());
        } else {
            EXPECT_THROW(vecContours[i].getCircumference(), InvalidCircumferenceException);
        }
    }
}

TEST_F(ContoursTest, ContourCollectionSphericalCoordinateTest) {
    ContourFactory factory = generate_factory();
    ContourCollection contours = factory.loadContours();
    vector<ContourSphericalCoordinate> sphericalCoordinates = contours.getSphericalCoordinates();
    vector<Contour> vecContours = contours.getContours();
    ASSERT_EQ(sphericalCoordinates.size(), vecContours.size());
    for (size_t i = 0; i < sphericalCoordinates.size(); i++) {
        EXPECT_EQ(sphericalCoordinates[i].size(), vecContours[i].rows());
    }
}

TEST_F(ContoursTest, ContourCollectionOutSkirtTest) {
    ContourFactory factory = generate_factory();
    ContourCollection contours = factory.loadContours();
    Contour contour = contours.getContours().at(0);
    ASSERT_TRUE(contour.isOutskirt());
}
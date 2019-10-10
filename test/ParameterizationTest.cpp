//
// Created by 徐溶延 on 2019/9/22.
//
#include <gtest/gtest.h>
#include <pmp/SurfaceMesh.h>
#include "../triangulation/TriangulationUtil.h"
#include "../spherical_harmonic/ContourSphericalCoordinate.h"
#include "../contour/ContourCollection.h"
#include "../contour/ContourFactory.h"
#include "../Parameterization.h"
#include "../io/MeshIO.h"

using namespace pmp;
using namespace xry_mesh;

class ParameterizationTest : public ::testing::Test {
public:
    Matrix3Xf m3, m4;
    ContourCollection contourCollection;
    ContourSphericalCoordinate coordinate;
    SurfaceMesh mesh;
    size_t boundaryNum;
public:
    void loadContour() {
        m3.resize(3, 12);
        m3 << 0, 1, 2, 3, 3, 3, 3, 2, 1, 0, 0, 0,
              0, 0, 0, 0, 1, 2, 3, 3, 3, 3, 2, 1,
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
        m4.resize(3, 4);
        m4 << 1, 2, 2, 1,
              1, 1, 2, 2,
              0, 0, 0, 0;
        vector<Contour> vecContour = {Contour(m3, IS_OUTSKIRT), Contour(m4)};
        contourCollection = ContourCollection(vecContour);
    }

    void loadContour2() {
        ContourLoader contourLoader = ContourLoader::Builder()
                .setFileName("animx.tif")
                .setOperator(THRESHOLD)
                .setDraw(true)
                .build();
        ContourFactory contourFactory(contourLoader, ContourFactory::IMAGE_STRATEGY);
        contourCollection = contourFactory.loadContours();
    }

    void convertImg2Mesh() {
        mesh = triangulate(contourCollection);
        MeshIO::save_mesh(mesh, "triangulation.obj");
    }

    void prepareCoordinate() {
        Contour contour = contourCollection.getContours().at(0);
        boundaryNum = contour.rows();
        coordinate = ContourSphericalCoordinate(contour, contourCollection.getSphereCircumference());
    }

    void prepareData() {
        loadContour();
        prepareCoordinate();
        convertImg2Mesh();
    }
};

TEST_F(ParameterizationTest, ParameterizationCircleTest) {
    prepareData();
    Parameterization parameterization(mesh, boundaryNum, coordinate);
    parameterization.parameterize();
    MeshIO::save_mesh(mesh, "testParameterization.obj");
    MeshIO::show_mesh("testParameterization.obj");
}

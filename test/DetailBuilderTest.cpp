//
// Created by 徐溶延 on 2019/11/14.
//

#include <gtest/gtest.h>
#include "../io/MeshIO.h"
#include "../detail_builder/DetailBuilder.h"

class DetailBuilderTest : public ::testing::Test {
public:
    SurfaceMesh mesh;
public:
    void readMesh() {
        MeshIO::read_mesh(mesh, "region2D.obj");
    }
};

TEST_F(DetailBuilderTest, kdTreeTest) {
    readMesh();
    pmp::Point p(1.54532, 9.75679, 0);
    DetailBuilder::faceOfPoint(p, mesh);
}


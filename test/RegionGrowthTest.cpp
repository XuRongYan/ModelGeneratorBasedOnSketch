//
// Created by 徐溶延 on 2019/10/28.
//

#include <gtest/gtest.h>
#include <pmp/SurfaceMesh.h>
#include "../detail_builder/RegionGrowth.h"
#include "../io/MeshIO.h"
#include "../detail_builder/ExponentialMap.h"
#include "../utils/PmpUtils.h"
#include "../detail_builder/RegionGrowth.h"

using namespace std;
using namespace pmp;

class RegionGrowthTest : public ::testing::Test {
public:
    SurfaceMesh mesh, mesh1;
    Region region;

public:
    void readMesh() {
        MeshIO::read_mesh(mesh, "result.obj");
    }

    void getRegion() {
        readMesh();
        pmp::Point p(160.392, -85.8431, 10.732);
        //pmp::Point p(-6.0842, -11.5097, 62.2371);
        Vertex v = PmpUtils::findVertexByPosition(mesh, p);
        RegionGrowth regionGrowth(2);
        region = regionGrowth.generateRegion(mesh, v);
    }
};

TEST_F(RegionGrowthTest, regionGenerationTest) {
    readMesh();
    pmp::Point p(160.392, -85.8431, 10.732);
    Vertex v = PmpUtils::findVertexByPosition(mesh, p);
    RegionGrowth regionGrowth(3);
    Region region = regionGrowth.generateRegion(mesh, v);
    EXPECT_EQ(region.points.size(), 19);
    EXPECT_EQ(region.faces.size(), 24);
}

TEST_F(RegionGrowthTest, meshGenerateTest) {
    getRegion();
    RegionGrowth::buildMesh(mesh1, region);
    MeshIO::save_mesh(mesh1, "region3D.obj");
    MeshIO::show_mesh("region3D.obj");
}

TEST_F(RegionGrowthTest, expMapTest) {
    getRegion();
    ExponentialMap exponentialMap(mesh, region);
    exponentialMap.expMap();
    exponentialMap.printPath();
    exponentialMap.buildMesh(mesh1);
    MeshIO::save_mesh(mesh1, "region2D.obj");
    MeshIO::show_mesh("region2D.obj");
}

TEST_F(RegionGrowthTest, vectorRotateTest) {
    Eigen::Vector2f rotate(0, -1);
    ExponentialCoordinate coordinate(Eigen::Vector2f(-1, 1));
    ExponentialCoordinate expect(Eigen::Vector2f(1, 1));
    ExponentialCoordinate result = ExponentialCoordinate::cross(rotate, coordinate);
    EXPECT_EQ(result, expect);
}

TEST_F(RegionGrowthTest, rotate3DTest) {
    pmp::Point p(0, 1, 0), a(1, 0, 0), target(0, 0, 1);
    pmp::Scalar theta = pi / 2;
    pmp::Point result = ExponentialMap::rotate3D(p, a, theta);
    EXPECT_FLOAT_EQ(result[0], target[0]);
    EXPECT_FLOAT_EQ(result[1], target[1]);
    EXPECT_FLOAT_EQ(result[2], target[2]);
}
//
// Created by 徐溶延 on 2019/9/6.
//
#include <gtest/gtest.h>
#include <pmp/SurfaceMesh.h>
#include "../laplace/UniformWeight.h"
#include "../laplace/CotWeight.h"
#include "../laplace/MVCWeight.h"
#include "../laplace/LaplacianFactory.h"

using namespace pmp;

class LaplaceMatrixTest : public ::testing::Test {
public:
    SurfaceMesh mesh;
    Vertex v0, v1, v2, v3;
    Face f0, f1, f2, f3;

    void add_triangle() {
        v0 = mesh.add_vertex(pmp::Point(0, 0, 0));
        v1 = mesh.add_vertex(pmp::Point(1, 0, 0));
        v2 = mesh.add_vertex(pmp::Point(0, 1, 0));
        v3 = mesh.add_vertex(pmp::Point(0, 0, 1));
        f0 = mesh.add_triangle(v0, v1, v2);
        f1 = mesh.add_triangle(v0, v1, v3);
        f2 = mesh.add_triangle(v2, v3, v0);
        f3 = mesh.add_triangle(v0, v2, v3);
    }

    void one_ring() {
        ASSERT_TRUE(mesh.read("pmp-data/off/vertex_onering.off"));
        EXPECT_EQ(mesh.n_vertices(), size_t(7));
        EXPECT_EQ(mesh.n_faces(), size_t(6));
        v0 = Vertex(3); // the central vertex
        auto points = mesh.get_vertex_property<pmp::Point>("v:point");
        points[v0][2] = 0.1; // lift central vertex
    }

    void unit_sphere() {
        ASSERT_TRUE(mesh.read("pmp-data/off/sphere.off"));
        EXPECT_EQ(mesh.n_vertices(), size_t(16070));
        EXPECT_EQ(mesh.n_faces(), size_t(32136));
    }
};



TEST_F(LaplaceMatrixTest, LaplaceMatrixUniformValid) {
    add_triangle();
    auto* weight = new UniformWeight(mesh);
    LaplacianFactory laplacianFactory(weight);
    xry_mesh::Laplacian L = laplacianFactory.create();
    //cout << L.getLaplacianMatrix();
    ASSERT_TRUE(L.isValid());
}

TEST_F(LaplaceMatrixTest, LaplaceMatrixCotValid) {
    add_triangle();
    auto* weight = new CotWeight(mesh);
    LaplacianFactory laplacianFactory(weight);
    xry_mesh::Laplacian L = laplacianFactory.create();
    //cout << L.getLaplacianMatrix();
    ASSERT_TRUE(L.isValid());
}

TEST_F(LaplaceMatrixTest, LaplaceMatrixMVCValid) {
    add_triangle();
    auto* weight = new MVCWeight(mesh);
    LaplacianFactory laplacianFactory(weight);
    xry_mesh::Laplacian L = laplacianFactory.create();
    //cout << L.getLaplacianMatrix();
    ASSERT_TRUE(L.isValid());
}
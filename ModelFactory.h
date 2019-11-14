//
// Created by 徐溶延 on 2019/9/12.
//

#ifndef MODELGENERATORBASEDONSKETCH_MODELFACTORY_H
#define MODELGENERATORBASEDONSKETCH_MODELFACTORY_H

#include <string>
#include <cmath>
#include <pmp/SurfaceMesh.h>
#include <pmp/algorithms/SurfaceRemeshing.h>
#include "spherical_harmonic/ConstrainedSphericalCoordinate.h"
#include "contour/ContourFactory.h"
#include "spherical_harmonic/SphericalHarmonics.h"
#include "triangulation/TriangulationUtil.h"
#include "spherical_harmonic/TriangulationSpericalCoordinate.h"
#include "spherical_harmonic/MeshSphericalCoordinate.h"
#include "laplace/LaplacianFactory.h"
#include "laplace/MVCWeight.h"
#include "laplace/CotWeight.h"
#include "laplace/UniformWeight.h"
#include "optimizer/LdltOptimizer.h"
#include "optimizer/LLTOptimizer.h"
#include "optimizer/SVDOptimizer.h"
#include "optimizer/LuOptimizer.h"
#include "Parameterization.h"
#include "detail_builder/DetailBuilder.h"
#include "io/MeshIO.h"
#include "utils/PmpUtils.h"

using namespace std;
using namespace pmp;
using namespace xry_mesh;

const string OWNERSHIP = "ownership";


class ModelFactory {
private:
    string imgPath;
    string resultPath;
    pmp::Scalar lambda_xy;
    pmp::Scalar lambda_z;
    int level;
    int interval;
    ContourCollection contourCollection;
    SphericalHarmonics sphericalHarmonics;
    SphericalHarmonics meshSphericalHarmonics;
    SphericalHarmonics zSphericalHarmonics;
    ContourSphericalCoordinate contourSphericalCoordinate;
    xry_mesh::Laplacian L;
    SurfaceMesh mesh;
    MatrixXf ci;

public:
    class Builder {
    private:
        string imgPath;
        string resultPath;
        pmp::Scalar lambda_xy;
        pmp::Scalar lambda_z;
        int level;
        int interval;

    public:
        const string &getImgPath() const {
            return imgPath;
        }

        Builder &setImgPath(const string &imgPath) {
            Builder::imgPath = imgPath;
            return *this;
        }

        const string &getResultPath() const {
            return resultPath;
        }

        Builder &setResultPath(const string &resultPath) {
            Builder::resultPath = resultPath;
            return *this;
        }

        pmp::Scalar getLambdaXy() const {
            return lambda_xy;
        }

        Builder &setLambdaXy(pmp::Scalar lambdaXy) {
            lambda_xy = lambdaXy;
            return *this;
        }

        pmp::Scalar getLambdaZ() const {
            return lambda_z;
        }

        Builder &setLambdaZ(pmp::Scalar lambdaZ) {
            lambda_z = lambdaZ;
            return *this;
        }

        int getLevel() const {
            return level;
        }

        Builder &setLevel(int level) {
            Builder::level = level;
            return *this;
        }

        int getInterval() const {
            return interval;
        }

        Builder setInterval(int interval) {
            Builder::interval = interval;
            return *this;
        }

        ModelFactory build() {
            return {*this};
        }
    };

    ModelFactory(const Builder &builder) {
        this->interval = builder.getInterval();
        this->level = builder.getLevel();
        this->lambda_xy = builder.getLambdaXy();
        this->lambda_z = builder.getLambdaZ();
        this->imgPath = builder.getImgPath();
        this->resultPath = builder.getResultPath();
    }

    const string &getImgPath() const {
        return imgPath;
    }

    const string &getResultPath() const {
        return resultPath;
    }

    pmp::Scalar getLambdaXy() const {
        return lambda_xy;
    }

    pmp::Scalar getLambdaZ() const {
        return lambda_z;
    }

    int getLevel() const {
        return level;
    }

    int getInterval() const {
        return interval;
    }

    void buildModel() {
        loadContour();
        calcSphericalHarmonics();
        loadMesh();
        calcZSphericalHarmonics();
        calcMeshSphericalHarmonics();
        calcUniformLaplacian(L);
        solve_xy();
        solve_z();
        rebuild();
        MeshIO::save_mesh(mesh, "rough_result.obj");
        DetailBuilder detailBuilder(mesh);
        detailBuilder.build();
        MeshIO::save_mesh(mesh, "result.obj");
    }

private:
    void loadContour() {
        ContourLoader contourLoader = ContourLoader::Builder()
                                                    .setFileName(imgPath)
                                                    .setOperator(THRESHOLD)
                                                    .setDraw(true)
                                                    .setInterval(interval)
                                                    .build();
        ContourFactory contourFactory(contourLoader, ContourFactory::IMAGE_STRATEGY);
        contourCollection = contourFactory.loadContours();
        logDebug() << "contour num =" << contourCollection.size();
        vector<Contour> vecContours = contourCollection.getContours();
        for(const auto& contour : vecContours) {
            logDebug() << "contour verticies nums" << contour.rows();
        }

        assert(contourCollection.size() > 0);
    }

    void loadMesh() {
        MeshIO::read_mesh(mesh, "ball.obj");
        moveToOrigin(mesh);
        MeshIO::save_mesh(mesh, "origin_point_ball.obj");
    }

    void moveToOrigin(SurfaceMesh &mesh) {
        pmp::Scalar MAX = INT_MIN, MIN = INT_MAX;
        Vertex vMax;
        for (const auto &v : mesh.vertices()) {
            pmp::Point p = mesh.position(v);
            if (p[2] > MAX) {
                MAX = p[2];
                vMax = v;
            }
            if (p[2] < MIN) {
                MIN = p[2];
            }
        }
        pmp::Point p = mesh.position(vMax);
        pmp::Scalar r = (MAX - MIN) / 2;
        pmp::Scalar deltaX = p[0], deltaY = p[1];
        pmp::Scalar deltaZ = p[2] - r;
        for (auto v : mesh.vertices()) {
            pmp::Point &p = mesh.position(v);
            p[0] -= deltaX;
            p[1] -= deltaY;
            p[2] -= deltaZ;
        }
    }

    void paratemerization() {
        Contour contour = contourCollection.getContours()[0];
        Parameterization parameterization(mesh, contour.rows(), contourSphericalCoordinate);
        parameterization.parameterize();
    }

    void calcSphericalHarmonics() {
        contourSphericalCoordinate = contourCollection.getSphericalCoordinates().at(0);
        assert(contourCollection.getContours().at(0).isOutskirt());
        sphericalHarmonics = SphericalHarmonics(level, &contourSphericalCoordinate);
    }

    void calcZSphericalHarmonics() {
        Contour contour = contourCollection.getContours()[0];
        ConstrainedSphericalCoordinate coordinate(contour, contourCollection.getSphereCircumference(), 5);
        zSphericalHarmonics = SphericalHarmonics(level, &coordinate);
    }

    void convertImg2Mesh() {
        mesh = triangulate(contourCollection);
    }

    void calcTriangulationSphericalHarmonics() {
        TriangulationSpericalCoordinate coordinate(mesh, contourSphericalCoordinate.getCircumference());
        meshSphericalHarmonics = SphericalHarmonics(level, &coordinate);
    }

    void calcMeshSphericalHarmonics() {
        MeshSphericalCoordinate coordinate(mesh);
        meshSphericalHarmonics = SphericalHarmonics(level, &coordinate);
    }

    void addPointOwnership() {
        int num = 0;
        auto ownership = mesh.add_vertex_property<int>(OWNERSHIP);
        for(auto v : mesh.vertices()) {
            ownership[v] = 0;
        }
        size_t contourNum = contourCollection.getMergedContour().rows();
        size_t counter = 0;
        for(auto v : mesh.vertices()) {
            if (counter < contourNum) {
                counter++;
                continue;
            }
            vector<Contour> contours = contourCollection.getContours();
            for(size_t i = 1; i < contours.size(); i++) {
                pmp::Point p = mesh.position(v);
                if(PmpUtils::isVertexInsideContour(p, contours[i])) {
                    ownership[v] = i;
                    num++;
                }
            }
        }
        logDebug() << "inside num =" << num;
    }

    void projectMeshZ() {
        auto* coordinate = meshSphericalHarmonics.getSphericalCoordinate();
        assert(coordinate);
        pmp::Scalar circumference = contourSphericalCoordinate.getCircumference();
        for(auto v : mesh.vertices()) {
            pmp::Point &p = mesh.position(v);
            p[2] = -coordinate->getZ(v.idx());
        }
    }

    void calcUniformLaplacian(xry_mesh::Laplacian &L) {
        UniformWeight weight(mesh);
        LaplacianFactory laplacianFactory(&weight);
        L = laplacianFactory.create();
    }

    void calcCotLaplacian(xry_mesh::Laplacian &L) {
        CotWeight weight(mesh);
        LaplacianFactory laplacianFactory(&weight);
        L = laplacianFactory.create();
    }

    void solve_xy() {
        MatrixXf contourHar = sphericalHarmonics.getMatrix();
        logDebug() << "contourHar size =" << contourHar.rows() << "*" << contourHar.cols();
        MatrixXf meshHar = meshSphericalHarmonics.getMatrix();
        logDebug() << "meshHar size =" << meshHar.rows() << "*" << meshHar.cols();
        SparseMatrixType LaplaceMatrix = L.getLaplacianMatrix();
        logDebug() << "L size =" << LaplaceMatrix.rows() << "*" << LaplaceMatrix.cols();
        MatrixXf HTLTLH = meshHar.transpose() * LaplaceMatrix.transpose() * LaplaceMatrix * meshHar;
        MatrixXf HcTHc = contourHar.transpose() * contourHar;
        MatrixXf A = lambda_xy * HTLTLH + HcTHc;
        logDebug() << "A size =" << A.rows() << "*" << A.cols();
        MatrixXf b = contourHar.transpose() * contourCollection.getContours().at(0).getContour().transpose();
        logDebug() << "b size =" << b.rows() << "*" << b.cols();
        LuOptimizer optimizer(A, b);
        ci = optimizer.solve();
    }

    void solve_z() {
        VectorXf G;
        VectorXf vz = this->calcVz();
        MatrixXf contourHar = zSphericalHarmonics.getMatrix();
        logDebug() << "contourHar size =" << contourHar.rows() << "*" << contourHar.cols();
        MatrixXf meshHar = meshSphericalHarmonics.getMatrix();
        logDebug() << "meshHar size =" << meshHar.rows() << "*" << meshHar.cols();
        SparseMatrixType LaplaceMatrix = L.getLaplacianMatrix();
        logDebug() << "L size =" << LaplaceMatrix.rows() << "*" << LaplaceMatrix.cols();
        G.setConstant(meshHar.rows(), 3);
        MatrixXf A = lambda_z * (meshHar.transpose() * LaplaceMatrix.transpose() * LaplaceMatrix * meshHar) + contourHar.transpose() * contourHar;
        logDebug() << "A size =" << A.rows() << "*" << A.cols();
        MatrixXf b = -lambda_z * meshHar.transpose() * LaplaceMatrix.transpose() * G - contourHar.transpose() * vz;
        logDebug() << "b size =" << b.rows() << "*" << b.cols();
        LdltOptimizer optimizer(A, b);
        MatrixXf cz = optimizer.solve();
        ci.col(2) = cz.col(0);
    }

    VectorXf calcVz() {
        vector<pmp::Scalar> vecVz;
        int z = 5;
        for(const auto& contour : contourCollection.getContours()) {
            logDebug() << "contour size =" << contour.rows();
            for(size_t i = 0; i < contour.rows(); i++) {
                vecVz.push_back(0);
            }
            for(size_t i = 0; i < contour.rows(); i++) {
                vecVz.push_back(z);
            }
            break;
        }
        return EigenUtils::mapVectorXf(vecVz);
    }

    void rebuild() {
        MatrixX3f V = meshSphericalHarmonics.getMatrix() * ci;
        size_t row = 0;
        for(auto v : mesh.vertices()) {
            pmp::Point &p = mesh.position(v);
            p[0] = V(row, 0);
            p[1] = V(row, 1);
            p[2] = V(row, 2);
            row++;
        }
    }

    void rebuildInside(const MatrixXf &meshHar, const MatrixXf &ci) {
        MatrixX3f V = meshHar * ci;
        size_t row = 0;
        for(auto v : mesh.vertices()) {
            pmp::Point &p = mesh.position(v);
            p[0] = V(row, 0);
            p[1] = V(row, 1);
            p[2] = V(row, 2);
            row++;
        }
    }

    void buildInside() {
        MatrixX3f G;
        MatrixXf ci;
        xry_mesh::Laplacian L;
        calcUniformLaplacian(L);
        calcG(L, G);
        //calcMeshSphericalHarmonics();
        modifyG(G);
        MatrixXf meshHar = meshSphericalHarmonics.getMatrix();
        solveInsideVertices(L.getLaplacianMatrix(), meshHar, G, ci);
        rebuildInside(meshHar, ci);
    }

    void solveInsideVertices(const MatrixXf &L, const MatrixXf &meshHar, const MatrixX3f &G, MatrixXf &ci) {
        MatrixXf HtLtLH = meshHar.transpose() * L.transpose() * L * meshHar;
        MatrixXf HtLtG = meshHar.transpose() * L.transpose() * G;
        LdltOptimizer optimizer(HtLtLH, HtLtG);
        ci = optimizer.solve();
    }

    void calcG(xry_mesh::Laplacian &L, MatrixX3f &G) {
        Matrix3Xf x;
        PmpUtils::vertices2Matrix(mesh, x);
        G = L.getLaplacianMatrix() * x.transpose();
    }

    void modifyG(MatrixX3f &G) {
        vector<int> g = {0, 50};
        auto ownership = mesh.get_vertex_property<int>(OWNERSHIP);
        size_t index = 0;
        for(auto v : mesh.vertices()) {
            if(ownership[v] != 0) {
                G(index, 0) = ownership[v];
                G(index, 1) = ownership[v];
                G(index, 2) = ownership[v];
            }
            index++;
        }
    }
};


#endif //MODELGENERATORBASEDONSKETCH_MODELFACTORY_H

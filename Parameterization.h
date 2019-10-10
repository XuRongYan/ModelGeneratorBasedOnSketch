//
// Created by 徐溶延 on 2019/9/22.
//

#ifndef MODELGENERATORBASEDONSKETCH_PARAMETERIZATION_H
#define MODELGENERATORBASEDONSKETCH_PARAMETERIZATION_H

#include <pmp/SurfaceMesh.h>
#include "laplace/MVCWeight.h"
#include "laplace/CotWeight.h"
#include "laplace/UniformWeight.h"
#include "laplace/LaplaceWithBoundary.h"
#include "laplace/LaplacianFactory.h"
#include "spherical_harmonic/ContourSphericalCoordinate.h"
#include "optimizer/LdltOptimizer.h"
#include "optimizer/LLTOptimizer.h"
#include "utils/log_utils/Logger.h"

using namespace std;
using namespace pmp;

class Parameterization {
private:
    SurfaceMesh& mesh;
    int boundaryNum;
    MVCWeight *weight;
    xry_mesh::LaplaceWithBoundary laplaceWithBoundary;
    ContourSphericalCoordinate coordinate;
    MatrixX3f xf, xp;

public:
    Parameterization(SurfaceMesh &mesh, int boundaryNum, ContourSphericalCoordinate& coordinate) : mesh(mesh), boundaryNum(boundaryNum), coordinate(coordinate) {
        weight = new MVCWeight(mesh);
        LaplacianFactory factory(weight, boundaryNum);
        laplaceWithBoundary = factory.createWithBoundary();
        prepareX();
    }

    ~Parameterization() {
        delete weight;
    }

    void parameterize() {
        MatrixXf A = laplaceWithBoundary.getL();
        logDebug() << "A" << A.rows() << "*" << A.cols();
        MatrixXf b = laplaceWithBoundary.getLBoundary() * xp;
        logDebug() << "b" << b.rows() << "*" << b.cols();
        MatrixXf AtA = A.transpose() * A;
        logDebug() << "AtA" << AtA.rows() << "*" << AtA.cols();
        MatrixXf Atb = A.transpose() * b;
        logDebug() << "Atb" << Atb.rows() << "*" << Atb.cols();
        LdltOptimizer optimizer(AtA, -Atb);
        xf = optimizer.solve();
        rebuild();
    }

private:
    void prepareX() {
        xp.resize(boundaryNum, 3);
        for(size_t i = 0; i < boundaryNum; i++) {
            pmp::Scalar x = cos(coordinate.getPhi(i)) * coordinate.getCircumference() / (2 * pi);
            pmp::Scalar y = sin(coordinate.getPhi(i)) * coordinate.getCircumference() / (2 * pi);
            pmp::Scalar z = 0;
            xp.row(i) << x, y, z;
        }
    }

    void rebuild() {
        for(auto v : mesh.vertices()) {
            pmp::Point& p = mesh.position(v);
            if(v.idx() >= boundaryNum) {
                p[0] = xf(v.idx() - boundaryNum, 0);
                p[1] = xf(v.idx() - boundaryNum, 1);
                p[2] = xf(v.idx() - boundaryNum, 2);
            } else {
                p[0] = xp(v.idx(), 0);
                p[1] = xp(v.idx(), 1);
                p[2] = xp(v.idx(), 2);
            }
        }
    }
};


#endif //MODELGENERATORBASEDONSKETCH_PARAMETERIZATION_H

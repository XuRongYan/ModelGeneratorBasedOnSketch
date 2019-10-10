//
// Created by 徐溶延 on 2019/9/18.
//

#ifndef MODELGENERATORBASEDONSKETCH_TRIANGULATIONSPERICALCOORDINATE_H
#define MODELGENERATORBASEDONSKETCH_TRIANGULATIONSPERICALCOORDINATE_H

#include <pmp/SurfaceMesh.h>
#include "../utils/log_utils/Logger.h"
#include "SphericalCoordinate.h"
#include "ContourSphericalCoordinate.h"

using namespace pmp;

class TriangulationSpericalCoordinate : public SphericalCoordinate {
private:
    SurfaceMesh mesh;
    size_t contourSize;
    pmp::Scalar circumference;

public:
    TriangulationSpericalCoordinate(const SurfaceMesh &mesh, pmp::Scalar circumference) : mesh(mesh), circumference(circumference) {
        initPhis();
        initThetas();
        initZ();
    }

    pmp::Scalar getZ(size_t row) override {
        return vecZ[row];
    }

private:
    void initPhis() {
        for (const auto &v : mesh.vertices()) {
            initPhi(v);
        }
    }

    void initThetas() {
        for (const auto &v : mesh.vertices()) {
            initTheta(v);
        }
    }


    void initPhi(const Vertex &v) {
        const pmp::Point &p = mesh.position(v);
        pmp::Scalar x = p[0];
        pmp::Scalar y = p[1];
        phis.push_back(atan2(y, x));
    }

    void initTheta(const Vertex &v) {
        const pmp::Point &p = mesh.position(v);
        pmp::Scalar x = p[0];
        pmp::Scalar y = p[1];
        pmp::Scalar c_r = 2 * pi * sqrt(x * x + y * y) / circumference;
        if (c_r > 1) c_r = 1;
        else if (c_r < -1) c_r = -1;
        pmp::Scalar theta = asin(c_r);
        thetas.push_back(theta);
    }

    void initZ() {
        for (size_t i = 0; i < thetas.size(); i++) {
            vecZ.push_back(cos(thetas[i]) * circumference / (2 * pi));
        }
    }
};


#endif //MODELGENERATORBASEDONSKETCH_TRIANGULATIONSPERICALCOORDINATE_H

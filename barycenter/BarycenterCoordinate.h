//
// Created by 徐溶延 on 2019/11/6.
//

#ifndef MODELGENERATORBASEDONSKETCH_BARYCENTERCOORDINATE_H
#define MODELGENERATORBASEDONSKETCH_BARYCENTERCOORDINATE_H

#include <iostream>
#include <pmp/SurfaceMesh.h>
#include <pmp/algorithms/DifferentialGeometry.h>

using namespace std;
using namespace pmp;

class BarycenterCoordinate {
private:
    pmp::Scalar x, y, z;

public:
    BarycenterCoordinate() = default;

    BarycenterCoordinate(pmp::Scalar x, pmp::Scalar y, pmp::Scalar z) : x(x), y(y), z(z) {}

    BarycenterCoordinate(const pmp::Point &p, const SurfaceMesh &mesh, const pmp::Face f) {
        vector<pmp::Point> vecP;
        for (auto v : mesh.vertices(f)) {
            vecP.push_back(mesh.position(v));
        }
        new (this)BarycenterCoordinate(p, vecP[0], vecP[1], vecP[2]);
    }

    BarycenterCoordinate(const pmp::Point &p, const pmp::Point &A, const pmp::Point &B, const pmp::Point &C) {
        pmp::Scalar s_abc = pmp::triangle_area(A, B, C);
        pmp::Scalar s_pab = pmp::triangle_area(p, A, B);
        pmp::Scalar s_pac = pmp::triangle_area(p, A, C);
        pmp::Scalar s_pbc = pmp::triangle_area(p, B, C);
        x = s_pbc / s_abc;
        y = s_pac / s_abc;
        z = s_pab / s_abc;

    }

    pmp::Scalar getX() const {
        return x;
    }

    void setX(pmp::Scalar x) {
        BarycenterCoordinate::x = x;
    }

    pmp::Scalar getY() const {
        return y;
    }

    void setY(pmp::Scalar y) {
        BarycenterCoordinate::y = y;
    }

    pmp::Scalar getZ() const {
        return z;
    }

    void setZ(pmp::Scalar z) {
        BarycenterCoordinate::z = z;
    }



};


#endif //MODELGENERATORBASEDONSKETCH_BARYCENTERCOORDINATE_H

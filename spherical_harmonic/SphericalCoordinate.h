//
// Created by 徐溶延 on 2019/9/9.
//

#ifndef MODELGENERATORBASEDONSKETCH_SPHERICALCOORDINATE_H
#define MODELGENERATORBASEDONSKETCH_SPHERICALCOORDINATE_H

#include <Eigen/Core>
#include <Eigen/Dense>
#include <pmp/SurfaceMesh.h>
#include <vector>
#include <cassert>
#include "../contour/Contour.h"

using namespace std;
using namespace Eigen;
const pmp::Scalar pi = 2 * acos(0);
class SphericalCoordinate {
protected:
    vector<pmp::Scalar> phis;
    vector<pmp::Scalar> thetas;
    vector<pmp::Scalar> vecZ;

public:
    SphericalCoordinate() {}

    pmp::Scalar getPhi(size_t row) {
        return phis[row];
    }

    pmp::Scalar getTheta(size_t row) {
        return thetas[row];
    }

    void setPhis(const vector<pmp::Scalar> &phis) {
        SphericalCoordinate::phis = phis;
    }

    void setThetas(const vector<pmp::Scalar> &thetas) {
        SphericalCoordinate::thetas = thetas;
    }

    void setVecZ(const vector<pmp::Scalar> &vecZ) {
        SphericalCoordinate::vecZ = vecZ;
    }

    const vector<pmp::Scalar> &getVecZ() const {
        return vecZ;
    }

    virtual pmp::Scalar getZ(size_t row) {
        if(vecZ.size() == 0) return 0;
        return vecZ[row];
    }

    size_t size() {
        assert(phis.size() == thetas.size());
        return phis.size();
    }

    const vector<pmp::Scalar> &getPhis() const {
        return phis;
    }

    const vector<pmp::Scalar> &getThetas() const {
        return thetas;
    }



};


#endif //MODELGENERATORBASEDONSKETCH_SPHERICALCOORDINATE_H

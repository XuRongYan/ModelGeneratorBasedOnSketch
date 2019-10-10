//
// Created by 徐溶延 on 2019/9/10.
//

#ifndef MODELGENERATORBASEDONSKETCH_SPHERICALHARMONICS_H
#define MODELGENERATORBASEDONSKETCH_SPHERICALHARMONICS_H

#include <boost/math/special_functions/spherical_harmonic.hpp>
#include "SphericalCoordinate.h"
#include "../utils/EigenUtils.h"

using namespace boost::math;

class SphericalHarmonics {
private:
    size_t level;
    size_t pointSize;
    SphericalCoordinate *sphericalCoordinate;
    MatrixXf sphericalHarmonicMatrix;
    vector<pmp::Scalar> vecSphericalHarmonic;

public:
    SphericalHarmonics() {}

    SphericalHarmonics(size_t level,
                       SphericalCoordinate *sphericalCoordinate) : level(level) {
        assert(sphericalCoordinate);
        this->sphericalCoordinate = new SphericalCoordinate();
        this->sphericalCoordinate->setPhis(sphericalCoordinate->getPhis());
        this->sphericalCoordinate->setThetas(sphericalCoordinate->getThetas());
        this->sphericalCoordinate->setVecZ(sphericalCoordinate->getVecZ());
        pointSize = this->sphericalCoordinate->size();
        calcSphericalHarmonics();
    }


    const MatrixXf &getMatrix() const {
        return sphericalHarmonicMatrix;
    }

    SphericalCoordinate *getSphericalCoordinate() const {
        return sphericalCoordinate;
    }

private:
    void calcSphericalHarmonics() {
        for (size_t row = 0; row < pointSize; row++) {
            calcSphericalHarmonicsPoint(row);
        }
        sphericalHarmonicMatrix = EigenUtils::mapMatrixXf(vecSphericalHarmonic, level * level,
                                                          vecSphericalHarmonic.size() / (level * level)).transpose();
    }

    void calcSphericalHarmonicsPoint(size_t row) {
        for (size_t L = 0; L < level; L++) {
            calcUnreliablePart(row, L);
            vecSphericalHarmonic.push_back(spherical_harmonic_r(L, 0, sphericalCoordinate->getTheta(row),
                                           sphericalCoordinate->getPhi(row)));
            calcReliablePart(row, L);
        }
    }

    void calcUnreliablePart(size_t row, size_t L) {
        for (int m = (int) -L; m != 0; m++) {
            vecSphericalHarmonic.push_back(sqrt(2) * spherical_harmonic_i(L, m, sphericalCoordinate->getTheta(row),
                                                                          sphericalCoordinate->getPhi(row)));
        }
    }

    void calcReliablePart(size_t row, size_t L) {
        for (size_t m = 1; m <= L; m++) {
            vecSphericalHarmonic.push_back(sqrt(2) * spherical_harmonic_r(L, m, sphericalCoordinate->getTheta(row),
                                                                          sphericalCoordinate->getPhi(row)));
        }
    }
};


#endif //MODELGENERATORBASEDONSKETCH_SPHERICALHARMONICS_H

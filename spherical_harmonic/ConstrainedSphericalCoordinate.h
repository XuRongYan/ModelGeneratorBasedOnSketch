//
// Created by 徐溶延 on 2019/10/12.
//

#ifndef MODELGENERATORBASEDONSKETCH_CONSTRAINEDSPHERICALCOORDINATE_H
#define MODELGENERATORBASEDONSKETCH_CONSTRAINEDSPHERICALCOORDINATE_H

#include "SphericalCoordinate.h"

#include <utility>

class ConstrainedSphericalCoordinate : public SphericalCoordinate {
private:
    Contour contour;
    pmp::Scalar circumference;
    pmp::Scalar z;
    vector<pmp::Scalar > vecZ;

public:
    ConstrainedSphericalCoordinate() {

    }

    ConstrainedSphericalCoordinate(Contour &contour, pmp::Scalar circumference, pmp::Scalar z) : contour(contour), circumference(circumference) {
        generateVecZ(z);
        initPhis();
        initThetas();
        initConstrainedPhis();
        initConstrainedThetas();
    }

    ConstrainedSphericalCoordinate(Contour &contour, pmp::Scalar circumference, vector<pmp::Scalar > vecZ) : contour(contour), circumference(circumference), vecZ(std::move(vecZ)) {
        initPhis();
        initThetas();
        initConstrainedPhis();
        initConstrainedThetas();
    }

public:
    pmp::Scalar getCircumference() const {
        return circumference;
    }

    void setCircumference(pmp::Scalar circumference) {
        ConstrainedSphericalCoordinate::circumference = circumference;
    }

    pmp::Scalar getZ1() const {
        return z;
    }

    void setZ(pmp::Scalar z) {
        ConstrainedSphericalCoordinate::z = z;
    }

private:
    void initPhis() {
        pmp::Scalar subLength = 0;
        for (size_t row = 1; row < contour.rows(); row++) {
            subLength += sqrt(pow(contour.getX(row) - contour.getX(row - 1), 2) +
                              pow(contour.getY(row) - contour.getY(row - 1), 2));
            phis.push_back(subLength / circumference * 2 * pi);
        }
        phis.push_back(2 * pi);
    }

    void initThetas() {
        for(size_t row = 0; row < contour.rows(); row++) {
            thetas.push_back(acos(0));
        }
    }

    void initConstrainedPhis() {
        phis.insert(phis.end(), phis.begin(), phis.end());
    }

    void initConstrainedThetas() {
        for (size_t i = 0; i < vecZ.size(); i++) {
            thetas.push_back(acos(vecZ[i] * 2 * pi / circumference));
        }
    }

    void generateVecZ(pmp::Scalar z) {
        for(size_t i = 0; i < contour.rows(); i++) {
            vecZ.push_back(z);
        }
    }
};


#endif //MODELGENERATORBASEDONSKETCH_CONSTRAINEDSPHERICALCOORDINATE_H

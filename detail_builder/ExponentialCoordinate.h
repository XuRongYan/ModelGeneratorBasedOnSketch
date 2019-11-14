//
// Created by 徐溶延 on 2019/10/29.
//

#ifndef MODELGENERATORBASEDONSKETCH_EXPONENTIALCOORDINATE_H
#define MODELGENERATORBASEDONSKETCH_EXPONENTIALCOORDINATE_H

#include <pmp/SurfaceMesh.h>
#include <Eigen/Dense>
#include <utility>
#include <ostream>
#include "../utils/PmpUtils.h"

using namespace Eigen;
class ExponentialCoordinate {
private:
    Eigen::Vector2f coordinate;

public:
    ExponentialCoordinate() {}

    ExponentialCoordinate(Vector2f coordinate) : coordinate(std::move(coordinate)) {}

    const Vector2f &getCoordinate() const {
        return coordinate;
    }

    void setCoordinate(const Vector2f &coordinate) {
        ExponentialCoordinate::coordinate = coordinate;

    }

    pmp::Scalar theta() const {
        return coordinate[0];
    }

    void setTheta(pmp::Scalar theta) {
        coordinate[0] = theta;
    }

    pmp::Scalar r() const {
        return coordinate[1];
    }

    void setR(pmp::Scalar r) {
        coordinate[1] = r;
    }

    static ExponentialCoordinate cross(const Eigen::Vector2f &rotate, const ExponentialCoordinate &c) {
        return {Eigen::Vector2f(c.theta() * rotate[0] - c.r() * rotate[1], c.theta() * rotate[1] + c.r() * rotate[0])};
    }

    bool operator==(const ExponentialCoordinate &rhs) const {
        return coordinate == rhs.coordinate;
    }

    bool operator!=(const ExponentialCoordinate &rhs) const {
        return !(rhs == *this);
    }

    friend ostream &operator<<(ostream &os, const ExponentialCoordinate &coordinate) {
        os << "coordinate: " << coordinate.coordinate;
        return os;
    }
};


#endif //MODELGENERATORBASEDONSKETCH_EXPONENTIALCOORDINATE_H

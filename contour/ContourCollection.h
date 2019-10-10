//
// Created by 徐溶延 on 2019/9/7.
//

#ifndef MODELGENERATORBASEDONSKETCH_CONTOURCOLLECTION_H
#define MODELGENERATORBASEDONSKETCH_CONTOURCOLLECTION_H

#include <Eigen/Core>
#include <utility>
#include <vector>
#include "Contour.h"
#include "../spherical_harmonic/ContourSphericalCoordinate.h"
#include "../utils/log_utils/Logger.h"
#include "../spherical_harmonic/ContourSphericalCoordinate.h"

using namespace std;
using namespace Eigen;

class ContourCollection {
private:
    vector<Contour> contours;
    vector<ContourSphericalCoordinate> sphericalCoordinates;
    Contour mergedContour;
    ContourSphericalCoordinate mergedSphericalCoordinate;
    pmp::Scalar sphereCircumference;
public:

    ContourCollection() {}

    explicit ContourCollection(vector<Contour>& contours) : contours(std::move(contours)) {
        mergeContours(this->contours);
        if(this->contours[0].isOutskirt()) {
            sphereCircumference = this->contours[0].getCircumference();
        } else {
            sphereCircumference = 0;
        }
        initSphericalCoordinates();
    }

    const vector<Contour> &getContours() const {
        return contours;
    }

    const Contour &getMergedContour() const {
        return mergedContour;
    }

    size_t size() {
        return contours.size();
    }

    pmp::Scalar getSphereCircumference() const {
        return sphereCircumference;
    }

    const vector<ContourSphericalCoordinate> &getSphericalCoordinates() const {
        return sphericalCoordinates;
    }

    const ContourSphericalCoordinate &getMergedSphericalCoordinate() const {
        return mergedSphericalCoordinate;
    }


private:
    void mergeContours(vector<Contour>& contour_vec) {
        for(auto & contour : contour_vec) {
            mergedContour.merge(contour);
        }
    }

    void initSphericalCoordinates() {
        for(auto & contour : contours) {
            sphericalCoordinates.emplace_back(ContourSphericalCoordinate(contour, sphereCircumference));

        }
        mergedSphericalCoordinate = ContourSphericalCoordinate(sphericalCoordinates);
    }
};
#endif //MODELGENERATORBASEDONSKETCH_CONTOURCOLLECTION_H

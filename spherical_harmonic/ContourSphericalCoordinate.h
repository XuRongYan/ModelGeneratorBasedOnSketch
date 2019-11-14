//
// Created by 徐溶延 on 2019/9/10.
//

#ifndef MODELGENERATORBASEDONSKETCH_CONTOURSPHERICALCOORDINATE_H
#define MODELGENERATORBASEDONSKETCH_CONTOURSPHERICALCOORDINATE_H

#include "SphericalCoordinate.h"
#include "ConstrainedSphericalCoordinate.h"

class ContourSphericalCoordinate : public SphericalCoordinate {
private:
    pmp::Scalar circumference;

public:
    ContourSphericalCoordinate() {}


    ContourSphericalCoordinate(Contour &contour, pmp::Scalar circumference) {
        this->circumference = circumference;
        initPhis(contour);
        initThetas(contour);
    }

    ContourSphericalCoordinate(const Matrix3Xf &points, pmp::Scalar circumference, bool isOutskirt) {
        Contour contour(points, isOutskirt ? IS_OUTSKIRT : NOT_OUTSKIRT);
        new (this)ContourSphericalCoordinate(contour, circumference);
    }

    ContourSphericalCoordinate(vector<ContourSphericalCoordinate>& vec) {
        circumference = vec[0].getCircumference();
        for(auto &sphericalCoordinate : vec) {
            const vector<pmp::Scalar>& tempPhis = sphericalCoordinate.getPhis();
            const vector<pmp::Scalar>& tempThetas = sphericalCoordinate.getThetas();
            phis.insert(phis.end(), tempPhis.begin(), tempPhis.end());
            thetas.insert(thetas.end(), tempThetas.begin(), tempThetas.end());
        }
    }

    pmp::Scalar getCircumference() const {
        return circumference;
    }

private:
    void initPhis(Contour &contour) {
        if (contour.isOutskirt()) {
            initOutskirtPhi(contour);
        } else {
            initNotOutskirtPhi(contour);
        }
    }

    void initThetas(Contour &contour) {
        if(contour.isOutskirt()) {
            initOutskirtTheta(contour);
        } else {
            initNotOutskirtTheta(contour);
        }
    }

    void initOutskirtPhi(Contour &contour) {
        pmp::Scalar subLength = 0;
        for (size_t row = 1; row < contour.rows(); row++) {
            subLength += sqrt(pow(contour.getX(row) - contour.getX(row - 1), 2) +
                              pow(contour.getY(row) - contour.getY(row - 1), 2));
            phis.push_back(subLength / circumference * 2 * pi);
        }
        phis.push_back(2 * pi);
    }

    void initNotOutskirtPhi(Contour &contour) {
        for(size_t row = 0; row < contour.rows(); row++) {
            pmp::Scalar x = contour.getX(row);
            pmp::Scalar y = contour.getY(row);
            phis.push_back(atan2(y, x));
        }
    }

    void initOutskirtTheta(Contour &contour) {
        for(size_t row = 0; row < contour.rows(); row++) {
            thetas.push_back(acos(0));
        }
    }

    void initNotOutskirtTheta(Contour &contour) {
        for(size_t row = 0; row < contour.rows(); row++) {
            pmp::Scalar x = contour.getX(row);
            pmp::Scalar y = contour.getY(row);
            thetas.push_back(asin(2 * pi * sqrt(x * x + y * y) / circumference));
        }
    }
};


#endif //MODELGENERATORBASEDONSKETCH_CONTOURSPHERICALCOORDINATE_H

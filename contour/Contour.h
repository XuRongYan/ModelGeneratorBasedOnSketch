//
// Created by 徐溶延 on 2019/9/7.
//

#ifndef MODELGENERATORBASEDONSKETCH_CONTOUR_H
#define MODELGENERATORBASEDONSKETCH_CONTOUR_H

#include <algorithm>
#include <Eigen/Core>
#include <utility>
#include <pmp/Types.h>
#include <ostream>
#include "ContourLoader.h"
#include "../utils/PmpException.h"

using namespace Eigen;
enum Outskirt {
    IS_OUTSKIRT,
    NOT_OUTSKIRT
};
class Contour {
private:
    Eigen::Matrix3Xf contour;
    pmp::Scalar circumference;
    Outskirt outskirt;
public:
    Contour(Matrix3Xf contour, Outskirt outskirt = NOT_OUTSKIRT) : contour(std::move(contour)), outskirt(outskirt) {
        if(isOutskirt()) {
            calcCircumference();
        }
    }

    /**
     * 为确保安全性，除测试代码以外不应该调用该构造函数
     */
    Contour() {
        outskirt = NOT_OUTSKIRT;
    }

    size_t rows() const {
        return contour.cols();
    }

    Block<Eigen::Matrix<float, 3, -1, 0, 3, -1>, 3, 1, 1> getRow(size_t rowNum) {
        return contour.col(rowNum);
    }

    pmp::Scalar getX(size_t row) {
        return contour(0, row);
    }

    pmp::Scalar getY(size_t row) {
        return contour(1, row);
    }

    pmp::Scalar getZ(size_t row) {
        return contour(2, row);
    }

    const Matrix3Xf &getContour() const {
        return contour;
    }

    pmp::Scalar getCircumference() const {
        if (isOutskirt()) {
            return circumference;
        }
        throw InvalidCircumferenceException();
    }

    /**
     * 为确保安全性，除了测试代码以外不应该传入outskirt参数
     * @param contour
     * @param outskirt
     */
    void setContour(const Matrix3Xf &contour, Outskirt outskirt = NOT_OUTSKIRT) {
        Contour::contour = contour;
        this->outskirt = outskirt;
        if(isOutskirt()) {
            calcCircumference();
        }
    }

    bool isOutskirt() const {
        return outskirt == IS_OUTSKIRT;
    }

    void merge(Contour &contour) {
        size_t tmpRow = this->contour.cols();
        this->contour.conservativeResize(3, tmpRow + contour.rows());
        for (size_t i = tmpRow; i < this->contour.cols(); i++) {
            this->getRow(i) << contour.getRow(i - tmpRow);
        }
    }

    friend bool operator==(const Contour &lhs, const Contour &rhs) {
        return lhs.getContour() == rhs.getContour();
    }

    friend bool operator!=(const Contour &lhs, const Contour &rhs) {
        return lhs.getContour() != rhs.getContour();
    }

    friend std::ostream &operator<<(std::ostream &os, const Contour &contour) {
        os << "contour: " << contour.contour;
        return os;
    }

    Contour &operator=(const Contour contour) {
        this->contour = contour.getContour();
        return *this;
    }

private:
    void calcCircumference() {
        circumference = 0;
        for (size_t row = 1; row < rows(); row++) {
            circumference += sqrt(pow(getX(row) - getX(row - 1), 2)
                                  + pow(getY(row) - getY(row - 1), 2));
        }
        circumference += sqrt(pow(getX(0) - getX(rows() - 1), 2)
                              + pow(getY(0) - getY(rows() - 1), 2));
    }
};


#endif //MODELGENERATORBASEDONSKETCH_CONTOUR_H

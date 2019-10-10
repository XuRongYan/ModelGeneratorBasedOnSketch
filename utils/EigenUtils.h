//
// Created by 徐溶延 on 2019/9/8.
//

#ifndef MODELGENERATORBASEDONSKETCH_EIGENUTILS_H
#define MODELGENERATORBASEDONSKETCH_EIGENUTILS_H

#include <Eigen/Core>
#include <Eigen/Dense>
#include <vector>

using namespace std;
using namespace Eigen;


class EigenUtils {
public:
    static Matrix3Xf mapMatrix3Xf(vector<float>& vec) {
        return Map<Matrix3Xf>(vec.data(), 3, vec.size() / 3);
    }

    static MatrixXf mapMatrixXf(vector<float >& vec, size_t rows, size_t cols) {
        assert(rows * cols == vec.size());
        return Map<MatrixXf>(vec.data(), rows, cols);
    }

    static VectorXf mapVectorXf(vector<float>& vec) {
        return Map<VectorXf>(vec.data(), vec.size());
    }


};


#endif //MODELGENERATORBASEDONSKETCH_EIGENUTILS_H

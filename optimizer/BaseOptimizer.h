//
// Created by 徐溶延 on 2019/9/8.
//

#ifndef MODELGENERATORBASEDONSKETCH_BASEOPTIMIZER_H
#define MODELGENERATORBASEDONSKETCH_BASEOPTIMIZER_H

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <utility>
using namespace Eigen;

class BaseOptimizer {
protected:
    MatrixXf A, b;
public:
    BaseOptimizer(MatrixXf a, MatrixXf b) : A(std::move(a)), b(std::move(b)) {}
    virtual MatrixXf solve() = 0;
};


#endif //MODELGENERATORBASEDONSKETCH_BASEOPTIMIZER_H

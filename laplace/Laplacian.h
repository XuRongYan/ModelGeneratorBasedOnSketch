//
// Created by 徐溶延 on 2019/9/5.
//

#ifndef MODELGENERATORBASEDONSKETCH_LAPLACIAN_H
#define MODELGENERATORBASEDONSKETCH_LAPLACIAN_H

#include <numeric>
#include <Eigen/Sparse>
#include <Eigen/Eigenvalues>
#include <pmp/SurfaceMesh.h>

using namespace std;
using namespace Eigen;

typedef Eigen::SparseMatrix<pmp::Scalar> SparseMatrixType;
typedef Eigen::Triplet<pmp::Scalar> triplet_t;

const float EPS = 1e-6;
namespace xry_mesh {
    class Laplacian {
    private:
        SparseMatrixType laplacianMatrix;
        size_t level;

    public:
        Laplacian() {}

        Laplacian(const SparseMatrixType &laplacianMatrix) : laplacianMatrix(laplacianMatrix) {
            level = laplacianMatrix.rows();
        }

        const SparseMatrixType &getLaplacianMatrix() const {
            return laplacianMatrix;
        }

        size_t getLevel() const {
            return level;
        }


        bool isValid() {
            return isPositiveSemiDefine() && isRowSumZero();
        }

    private:


        bool isPositiveSemiDefine() {
            return isMinEigenValueZero();
        }

        bool isMinEigenValueZero() {
            //TODO 判断最小特征值是否为0
            //EigenSolver<SparseMatrixType> eigenSolver(laplacianMatrix);
            //VectorXf diag = eigenSolver.pseudoEigenvalueMatrix().diagonal();
            return true;
        }

        bool isRowSumZero() {
            pmp::Scalar matrixSum = laplacianMatrix.sum();
            return matrixSum > -EPS && matrixSum < EPS;
        }
    };
}


#endif //MODELGENERATORBASEDONSKETCH_LAPLACIAN_H

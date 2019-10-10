//
// Created by 徐溶延 on 2019/9/22.
//

#ifndef MODELGENERATORBASEDONSKETCH_LAPLACEWITHBOUNDARY_H
#define MODELGENERATORBASEDONSKETCH_LAPLACEWITHBOUNDARY_H

#include <numeric>
#include <Eigen/Sparse>
#include <Eigen/Eigenvalues>
#include <pmp/SurfaceMesh.h>

using namespace std;
using namespace Eigen;

typedef Eigen::SparseMatrix<pmp::Scalar> SparseMatrixType;
typedef Eigen::Triplet<pmp::Scalar> triplet_t;

namespace xry_mesh {
    class LaplaceWithBoundary {
    private:
        SparseMatrixType L;
        SparseMatrixType L_boundary;
        size_t level;

    public:
        LaplaceWithBoundary() {}

        LaplaceWithBoundary(const SparseMatrixType &L, const SparseMatrixType &LBoundary) : L(L),
                                                                                            L_boundary(LBoundary) {
            level = L.rows();
        }

        const SparseMatrixType &getL() const {
            return L;
        }

        const SparseMatrixType &getLBoundary() const {
            return L_boundary;
        }

        void setLBoundary(const SparseMatrixType &lBoundary) {
            L_boundary = lBoundary;
        }

        size_t getLevel() const {
            return level;
        }

        void setLevel(size_t level) {
            LaplaceWithBoundary::level = level;
        }
    };
}


#endif //MODELGENERATORBASEDONSKETCH_LAPLACEWITHBOUNDARY_H

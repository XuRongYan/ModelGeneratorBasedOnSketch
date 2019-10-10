//
// Created by 徐溶延 on 2019/9/5.
//
#include <gtest/gtest.h>
#include "../laplace/Laplacian.h"
#include <iostream>
#include <vector>

using namespace std;

SparseMatrixType generateSparseMatrix();

void setVecData(vector<pmp::Scalar> &vec);

void resizeMatrix (SparseMatrixType& matrix, size_t size);

void getTripleList(vector<triplet_t> &tripletList, const vector<pmp::Scalar> &vecData);

TEST(Laplacian, levelEqualsRows) {
    SparseMatrixType sparseMatrix = generateSparseMatrix();
    xry_mesh::Laplacian laplacian(sparseMatrix);
    EXPECT_EQ(laplacian.getLevel(), sparseMatrix.rows());
}


SparseMatrixType generateSparseMatrix() {
    SparseMatrixType matrix;
    vector<pmp::Scalar> vecData;
    vector<triplet_t> tripletList;
    setVecData(vecData);
    getTripleList(tripletList, vecData);
    resizeMatrix (matrix, vecData.size());
    matrix.setFromTriplets(tripletList.begin(), tripletList.end());
    return matrix;
}

void setVecData(vector<pmp::Scalar> &vec) {
    for (int i = 0; i < 9; i++) {
        vec.push_back(i);
    }
}

void getTripleList(vector<triplet_t> &tripletList, const vector<pmp::Scalar> &vecData) {
    for (size_t i = 0; i < vecData.size(); i++) {
        tripletList.emplace_back(i, i, vecData[i]);
    }
}

void resizeMatrix (SparseMatrixType& matrix, size_t size) {
    matrix.resize(10, 10);
}

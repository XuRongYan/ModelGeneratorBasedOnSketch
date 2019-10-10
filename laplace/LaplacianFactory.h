//
// Created by 徐溶延 on 2019/9/5.
//

#ifndef MODELGENERATORBASEDONSKETCH_LAPLACIANFACTORY_H
#define MODELGENERATORBASEDONSKETCH_LAPLACIANFACTORY_H

#include <pmp/SurfaceMesh.h>
#include "Laplacian.h"
#include "LaplaceWithBoundary.h"
#include "../utils/PmpUtils.h"
#include "LaplaceWeight.h"
#include <vector>

using namespace std;
using namespace pmp;

class LaplacianFactory {
private:
    SurfaceMesh mesh;
    LaplaceWeight *laplaceWeight;
    vector<size_t> beginIndex;
    size_t vertexNum;
    size_t tripletNum;
    size_t boundaryNum;
    vector<triplet_t> tripletList;
    vector<triplet_t> tripletBoundaryList;
    SparseMatrixType L;
    SparseMatrixType L_boundary;
private:
    void getBeginIndex(vector<size_t> &beginIndex);

    void fillTripletList();

    void fillBoundaryTripletList();

    void fillRelatedTripletList(const Vertex &v);

    void fillRelatedBoundaryTripletList(const Vertex &v);

    pmp::Scalar fillNeighborTripletListAndReturnSumWeight(const Vertex &v);

    pmp::Scalar fillNeighborBoundaryTripletListAndReturnSumWeight(const Vertex &v);

    void fillVertexTripletList(const Vertex &v, pmp::Scalar sumWeight);

    void fillVertexBoundaryTripletList(const Vertex &v, pmp::Scalar sumWeight);

    void getL();

    void getLAndLBoundary();

public:
    LaplacianFactory(LaplaceWeight *laplaceWeight);

    LaplacianFactory(LaplaceWeight *laplceWeight, size_t boundaryNum);

    ~LaplacianFactory() {
        //delete laplaceWeight;
    }

    xry_mesh::Laplacian create();

    xry_mesh::LaplaceWithBoundary createWithBoundary();

    size_t getTripletNum() const;
};

#endif //MODELGENERATORBASEDONSKETCH_LAPLACIANFACTORY_H

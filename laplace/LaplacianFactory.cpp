//
// Created by 徐溶延 on 2019/9/5.
//

#include "LaplacianFactory.h"


xry_mesh::Laplacian LaplacianFactory::create() {
    beginIndex.resize(vertexNum);
    getBeginIndex(beginIndex);
    fillTripletList();
    getL();
    return xry_mesh::Laplacian(L);
}

xry_mesh::LaplaceWithBoundary LaplacianFactory::createWithBoundary() {
    beginIndex.resize(vertexNum);
    getBeginIndex(beginIndex);
    fillBoundaryTripletList();
    getLAndLBoundary();
    return xry_mesh::LaplaceWithBoundary(L, L_boundary);
}

void LaplacianFactory::getBeginIndex(vector<size_t> &beginIndex) {
    for (auto v : mesh.vertices()) {
        if (!mesh.is_boundary(v)) {
            beginIndex[v.idx()] = tripletNum;
            tripletNum += mesh.valence(v) + 1;
        }
    }
}

size_t LaplacianFactory::getTripletNum() const {
    return tripletNum;
}

void LaplacianFactory::fillTripletList() {
    tripletList.resize(tripletNum);
    for (auto v : mesh.vertices()) {
        if (!mesh.is_boundary(v)) {
            fillRelatedTripletList(v);
        }
    }
}

void LaplacianFactory::fillBoundaryTripletList() {
    for (auto v : mesh.vertices()) {
        if (!mesh.is_boundary(v)) {
            fillRelatedBoundaryTripletList(v);
        }
    }
}

void LaplacianFactory::fillRelatedTripletList(const Vertex &v) {
    pmp::Scalar sumWeight = fillNeighborTripletListAndReturnSumWeight(v);
    fillVertexTripletList(v, sumWeight);
}

void LaplacianFactory::fillRelatedBoundaryTripletList(const Vertex &v) {
    pmp::Scalar sumWeight = fillNeighborBoundaryTripletListAndReturnSumWeight(v);
    fillVertexBoundaryTripletList(v, sumWeight);
}

pmp::Scalar LaplacianFactory::fillNeighborTripletListAndReturnSumWeight(const Vertex &v) {
    size_t idx = 0;
    pmp::Scalar sumWeight = 0;
    for (auto neighborVertex : vvNeighbors(&mesh, v)) {
        pmp::Scalar weight = laplaceWeight->getWeight(v, neighborVertex);
        tripletList.emplace_back(triplet_t(v.idx(), neighborVertex.idx(), -weight));
        idx++;
        sumWeight += weight;
    }
    return sumWeight;
}

pmp::Scalar LaplacianFactory::fillNeighborBoundaryTripletListAndReturnSumWeight(const Vertex &v) {
    pmp::Scalar sumWeight = 0;
    for (auto neighborVertex : vvNeighbors(&mesh, v)) {
        pmp::Scalar weight = laplaceWeight->getWeight(v, neighborVertex);
        if(neighborVertex.idx() < boundaryNum) {
            tripletBoundaryList.emplace_back(v.idx(), neighborVertex.idx(), weight);
        } else {
            tripletList.emplace_back(triplet_t(v.idx(), neighborVertex.idx() - boundaryNum, weight));
        }
        sumWeight += weight;
    }
    return sumWeight;
}

void LaplacianFactory::fillVertexTripletList(const Vertex &v, pmp::Scalar sumWeight) {
    tripletList.emplace_back(v.idx(), v.idx(), sumWeight);
}

void LaplacianFactory::fillVertexBoundaryTripletList(const Vertex &v, pmp::Scalar sumWeight) {
    if (v.idx() < boundaryNum) {
        tripletBoundaryList.emplace_back(triplet_t(v.idx(), v.idx(), -sumWeight));
    } else {
        tripletList.emplace_back(triplet_t(v.idx(), v.idx() - boundaryNum, -sumWeight));
    }
}

void LaplacianFactory::getL() {
    L.resize(vertexNum, vertexNum);
    L.setFromTriplets(tripletList.begin(), tripletList.end());
}

void LaplacianFactory::getLAndLBoundary() {
    L.resize(vertexNum, vertexNum - boundaryNum);
    L_boundary.resize(vertexNum, boundaryNum);
    L.setFromTriplets(tripletList.begin(), tripletList.end());
    L_boundary.setFromTriplets(tripletBoundaryList.begin(), tripletBoundaryList.end());
}

LaplacianFactory::LaplacianFactory(LaplaceWeight *laplaceWeight) : laplaceWeight(laplaceWeight) {
    tripletNum = 0;
    boundaryNum = 0;
    mesh = laplaceWeight->getMesh();
    vertexNum = mesh.n_vertices();
}

LaplacianFactory::LaplacianFactory(LaplaceWeight *laplaceWeight, size_t boundaryNum) : laplaceWeight(laplaceWeight), boundaryNum(boundaryNum) {
    tripletNum = 0;
    mesh = laplaceWeight->getMesh();
    vertexNum = mesh.n_vertices();
}














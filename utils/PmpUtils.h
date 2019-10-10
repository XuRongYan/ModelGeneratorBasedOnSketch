//
// Created by 徐溶延 on 2019/9/5.
//

#ifndef MODELGENERATORBASEDONSKETCH_PMPUTILS_H
#define MODELGENERATORBASEDONSKETCH_PMPUTILS_H

#include <vector>
#include <pmp/SurfaceMesh.h>
#include <pmp/algorithms/DifferentialGeometry.h>
#include "PmpException.h"
#include "EigenUtils.h"
#include "../contour/Contour.h"

using namespace pmp;
using namespace std;
typedef SurfaceMesh::VertexAroundFaceCirculator vfNeighbors;
typedef SurfaceMesh::VertexAroundVertexCirculator vvNeighbors;
typedef SurfaceMesh::FaceAroundVertexCirculator fvNeighbors;
typedef SurfaceMesh::HalfedgeAroundFaceCirculator hfNeighbors;
typedef SurfaceMesh::HalfedgeAroundVertexCirculator hvNeighbors;

const int FIRST_FACE = 0;
const int SECOND_FACE = 1;
const int FIRST_VERTEX = 0;
const int SECOND_VERTEX = 1;

class PmpUtils {
public:
    static pmp::Point vertex2Point(const SurfaceMesh &mesh, const Vertex v) {
        return mesh.position(v);
    }

    static void vertex2Point(const SurfaceMesh &mesh, const vector<Vertex> &vec_v, vector<pmp::Point> &points) {
        for (auto v : vec_v) {
            points.emplace_back(vertex2Point(mesh, v));
        }
    }

    static pair<Vertex, Vertex> findVerticesOppositeEdge(const SurfaceMesh &mesh, const Vertex &v0, const Vertex &v1) {
        Edge e = mesh.find_edge(v0, v1);
        Halfedge h1 = mesh.halfedge(e, 0);
        Halfedge h2 = mesh.halfedge(e, 1);
        Vertex vl = mesh.to_vertex(mesh.next_halfedge(h1));
        Vertex vr = mesh.to_vertex(mesh.next_halfedge(h2));
        return make_pair(vl, vr);
    }

    static void facesAroundEdge(const SurfaceMesh &mesh, const Edge &e, pair<Face, Face> &faces) {
        faces.first = mesh.face(e, FIRST_FACE);
        faces.second = mesh.face(e, SECOND_FACE);
    }

    static pair<Vertex, Vertex>
    findFaceVerticesNotOnEdge(const SurfaceMesh &mesh, const Edge &e, const pair<Face, Face> &faces) {
        Vertex v1 = findFaceVertexNotOnEdge(mesh, e, faces.first);
        Vertex v2 = findFaceVertexNotOnEdge(mesh, e, faces.second);
        return make_pair(v1, v2);
    }

    static Vertex findFaceVertexNotOnEdge(const SurfaceMesh &mesh, const Edge &e, const Face &face) {
        for (auto v : mesh.vertices(face)) {
            if (!belongToEdge(mesh, e, v))
                return v;
        }
        throw VertexNotFoundException();
    }

    static bool belongToEdge(const SurfaceMesh &mesh, const Edge &e, const Vertex &v) {
        return (v == mesh.vertex(e, FIRST_VERTEX) || v == mesh.vertex(e, SECOND_VERTEX));
    }

    static void vertices2Matrix(const SurfaceMesh &mesh, Matrix3Xf &matrix) {
        vector<pmp::Scalar> vecVertices;
        for (auto v : mesh.vertices()) {
            pmp::Point p = mesh.position(v);
            vecVertices.push_back(p[0]);
            vecVertices.push_back(p[1]);
            vecVertices.push_back(p[2]);
        }
        matrix = EigenUtils::mapMatrix3Xf(vecVertices);
    }

    static bool isVertexInsideContour(const pmp::Point &p, Contour &contour) {
        int vNum = contour.rows();
        bool result = false;
        for (int i = 0, j = vNum - 1; i < vNum; j = i++) {
            if (((contour.getY(i) > p[1]) != (contour.getY(j) > p[1])) && (p[0] < (contour.getX(j) - contour.getX(i)) *
                                                                                  (p[1] - contour.getY(i)) /
                                                                                  (contour.getY(j) - contour.getY(i)) +
                                                                                  contour.getX(i))) {
                result = !result;
            }
        }
        return result;
    }
};


#endif //MODELGENERATORBASEDONSKETCH_PMPUTILS_H

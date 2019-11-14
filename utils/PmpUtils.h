//
// Created by 徐溶延 on 2019/9/5.
//

#ifndef MODELGENERATORBASEDONSKETCH_PMPUTILS_H
#define MODELGENERATORBASEDONSKETCH_PMPUTILS_H

#include <vector>
#include <pmp/SurfaceMesh.h>
#include <pmp/algorithms/DifferentialGeometry.h>
#include <pmp/algorithms/SurfaceNormals.h>
#include "PmpException.h"
#include "EigenUtils.h"
#include "../contour/Contour.h"
#include "log_utils/Logger.h"
#include "../barycenter/BarycenterCoordinate.h"

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
const double delta = 1e-5;
const pmp::Scalar pi = 2 * acos(0);

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

    static Vertex findVertexByPosition(const SurfaceMesh &mesh, const pmp::Point p) {
        pmp::Scalar MIN = INT_MAX;
        Vertex res;
        for (const auto v : mesh.vertices()) {
            pmp::Point tmpP = mesh.position(v);
            if (pmp::norm(tmpP - p) < MIN) {
                MIN = pmp::norm(tmpP - p);
                res = v;
            }
        }
        return res;
    }

    static pmp::Normal getVertexNormal(const pmp::SurfaceMesh &mesh, const pmp::Vertex v) {
        return SurfaceNormals::compute_vertex_normal(mesh, v);
    }

    static pmp::Point projectToPlane(const pmp::Normal &n, const pmp::Point &p) {
        return p - n * pmp::dot(n, p) / (pmp::norm(n) * pmp::norm(n));
    }

    static void buildMesh(SurfaceMesh &mesh, const vector<pmp::Point> &points, const Eigen::Matrix3Xi& faces) {
        vector<pmp::Vertex> vertices;
        for (auto p : points) {
            Vertex v = mesh.add_vertex(p);
            vertices.push_back(v);
        }
        for (size_t i = 0; i < faces.cols(); i++) {
            mesh.add_triangle(vertices[faces(0, i)], vertices[faces(1, i)], vertices[faces(2 , i)]);
        }
    }

    static void findBoundary(const SurfaceMesh &mesh, vector<pmp::Vertex> &boundary) {
        pmp::Vertex startV;
        pmp::Halfedge halfedge;
        for (auto v : mesh.vertices()) {
            if (mesh.is_boundary(v)) {
                startV = v;
                break;
            }
        }
        boundary.push_back(startV);
        halfedge = mesh.halfedge(startV);
        if (mesh.is_valid(mesh.face(halfedge))) halfedge = mesh.opposite_halfedge(halfedge);
        pmp::Vertex nextV = mesh.to_vertex(halfedge);
        assert(mesh.is_boundary(nextV));
        while (nextV != startV) {
            boundary.push_back(nextV);
            halfedge = mesh.next_halfedge(halfedge);
            nextV = mesh.to_vertex(halfedge);
            assert(mesh.is_boundary(nextV));
        }
    }

    /**
     * 判断点是否位于一个三角形内部
     * @param p
     * @param A
     * @param B
     * @param C
     * @return
     */
    static bool pointInFace(const pmp::Point &p, const pmp::Point &A, const pmp::Point &B, const pmp::Point &C) {
        if (!pointOnPlane(p, A, B, C)) return false;
        pmp::Point AC = C - A;
        pmp::Point AB = B - A;
        pmp::Point AP = p - A;
        pmp::Scalar fi = pmp::dot(AP, AC) * pmp::dot(AB, AB) - pmp::dot(AP, AB) * pmp::dot(AC, AB);
        pmp::Scalar fj = pmp::dot(AP, AB) * pmp::dot(AC, AC) - pmp::dot(AP, AC) * pmp::dot(AC, AB);
        pmp::Scalar fd = pmp::dot(AC, AC) * pmp::dot(AB, AB) - pmp::dot(AC, AB) * pmp::dot(AC, AB);
        if (fd < 0) logError() << "error fd < 0";
        return fi >= 0 && fj >= 0 && fi + fj - fd <= 0;
    }

    static bool pointInFace(const SurfaceMesh &mesh, const pmp::Point &p, const pmp::Face &f) {
        vector<pmp::Point> vecP;
        for (auto v : mesh.vertices(f)) {
            vecP.push_back(mesh.position(v));
        }
        return pointInFace(p, vecP[0], vecP[1], vecP[2]);
    }

    /**
     * 判断点是否与三点组成的面位于一个平面
     * @param p
     * @param A
     * @param B
     * @param C
     * @return
     */
    static bool pointOnPlane(const pmp::Point &p, const pmp::Point &A, const pmp::Point &B, const pmp::Point &C) {
        pmp::Point AP = p - A;
        pmp::Point AB = B - A;
        pmp::Point AC = C - A;
        pmp::Point n = pmp::cross(AB, AC);
        pmp::Scalar f = pmp::dot(n, AP);

        return abs(f) < delta;
    }

    static pmp::Point getPointByBarycenterCoor(const pmp::Point &A, const pmp::Point &B, const pmp::Point &C, const BarycenterCoordinate &coordinate) {
        pmp::Point p;
        p = coordinate.getX() * A + coordinate.getY() * B + coordinate.getZ() * C;
        return p;
    }
};


#endif //MODELGENERATORBASEDONSKETCH_PMPUTILS_H

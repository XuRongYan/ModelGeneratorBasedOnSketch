//
// Created by 徐溶延 on 2019/9/11.
//
#include "TriangulationUtil.h"
#include "../utils/PmpUtils.h"
#include "../utils/EigenUtils.h"

namespace xry_mesh {
    SurfaceMesh triangulate(ContourCollection &contours) {
        Matrix2Xd V, H;
        Matrix2Xi S;
        Matrix3Xi T;
        Matrix2Xd points2D;
        Matrix3Xi triangles;
        Matrix2Xi segmentations;
        prepareDataForTriangulation(contours, V, S, H);
        triangulate_polygon(V, S, H, 60, 30, points2D, triangles, segmentations);
        SurfaceMesh mesh;
        vector<pmp::Vertex> vecVertex;
        buildMesh(mesh, points2D, triangles, segmentations, vecVertex);
        addEdgeProperty(mesh, segmentations, vecVertex);
        vector<Matrix3Xf> vecContours;
        findNewContours(mesh, vecVertex, segmentations.cols(), contours);
        return mesh;
    }

    SurfaceMesh refineMesh(const SurfaceMesh &mesh, const vector<pmp::Point> &details) {
        Eigen::Matrix2Xd V, H;
        Eigen::Matrix2Xi S;
        Eigen::Matrix3Xi T;
        Matrix2Xd points2D;
        Matrix3Xi triangles;
        Matrix2Xi segmentations;
        prepareMeshBoundary(mesh, V, S, details);
        triangulate_polygon(V, S, H,20, 35, points2D, triangles, segmentations);
        vector<pmp::Vertex> vecVertex;
        SurfaceMesh refinedMesh;
        buildMesh(refinedMesh, points2D, triangles, segmentations, vecVertex);
        return refinedMesh;
    }

    void prepareDataForTriangulation(ContourCollection &contours, Matrix2Xd &V, Matrix2Xi &S, Matrix2Xd &H) {
        prepareVertex(contours, V);
        prepareSegmentation(contours, S);
        //prepareHoles(contours, H);
    }

    void prepareDataForRefinement(const SurfaceMesh &mesh, Matrix2Xd &V, Matrix2Xi &S, Matrix3Xi &T) {
        prepareVertex(mesh, V);
        //prepareSegmentations(mesh, S);
        prepareTriangles(mesh, T);
    }

    void prepareVertex(const SurfaceMesh& mesh, Matrix2Xd &V) {
        V.resize(2, mesh.n_vertices());
        for (const auto &v : mesh.vertices()) {
            pmp::Point p = mesh.position(v);
            V.col(v.idx()) << p[0], p[1];
        }
    }

    void prepareVertex(ContourCollection &contours, Matrix2Xd &V) {
        const Matrix3Xf &points = contours.getMergedContour().getContour();
        V.resize(2, points.cols());
        for (size_t i = 0; i < points.cols(); i++) {
            V.col(i) << points(0, i), points(1, i);
        }
    }

    void prepareTriangles(const SurfaceMesh &mesh, Matrix3Xi &T) {
        T.resize(3, mesh.n_faces());
        for (auto f : mesh.faces()) {
            vector<pmp::Vertex> vecV;
            for (const auto &v : mesh.vertices(f)) {
                vecV.push_back(v);
            }
            T.col(f.idx()) << vecV[0].idx(), vecV[1].idx(), vecV[2].idx();
        }
    }

    void prepareSegmentation(ContourCollection &contours, Matrix2Xi &segmentation) {
        const vector<Contour> &vecContour = contours.getContours();
        segmentation.resize(2, contours.getMergedContour().rows());
        size_t row = 0;
        for (const auto &contour : vecContour) {
            for (size_t i = 0; i < contour.rows(); i++) {
                segmentation.col(row + i) << row + (i + 1) % contour.rows(), row + i;
            }
            row += contour.rows();
        }
    }

    void prepareSegmentations(const SurfaceMesh &mesh, Matrix2Xi &E) {
        E.resize(2, mesh.n_edges());
        size_t i = 0;
        for (const auto &e : mesh.edges()) {
            pmp::Vertex v1 = mesh.vertex(e, 0);
            pmp::Vertex v2 = mesh.vertex(e, 1);
            E.col(i) << v1.idx(), v2.idx();
        }
    }

    void prepareHoles(ContourCollection &contours, Matrix2Xd &holes) {
        Matrix3Xf matrix;
        vector<Contour> vecContour = contours.getContours();
        holes.resize(2, contours.size() - 1);
        for (size_t i = 1; i < contours.size(); i++) {
            matrix = vecContour[i].getContour();
            Matrix2Xf matrix2D(2, matrix.cols());
            matrix2D.row(0) << matrix.row(0);
            matrix2D.row(1) << matrix.row(1);
            Vector2f point = calcBarycenter(matrix2D);
            holes.col(i - 1) << point[0], point[1];
        }
    }

    Vector2f calcBarycenter(Matrix2Xf &points) {
        pmp::Scalar tmp;
        pmp::Scalar area = 0;
        pmp::Scalar x = 0, y = 0;
        for (size_t i = 0; i < points.cols(); i++) {
            tmp = points(0, i) * points(1, (i + 1) % points.cols()) - points(1, i) * points(0, (i + 1) % points.cols());
            area += tmp;
            x += tmp * (points(0, i) + points(0, (i + 1) % points.cols()));
            y += tmp * (points(1, i) + points(1, (i + 1) % points.cols()));
        }
        area /= 2;
        x /= 6 * area;
        y /= 6 * area;
        Vector2f res;
        res << x, y;
        return res;
    }

    void addEdgeProperty(SurfaceMesh &mesh, const Matrix2Xi &segmentation, const vector<Vertex> &vids) {
        auto property = mesh.add_edge_property<bool>("contour_level");
        for(auto e : mesh.edges()) {
            property[e] = false;
        }
        for(size_t i = 0; i < segmentation.cols(); i++) {
            pmp::Edge e = mesh.find_edge(vids[segmentation(0, i)], vids[segmentation(1, i)]);
            assert(mesh.is_valid(e));
            property[e] = true;
        }
    }

    void prepareMeshBoundary(const SurfaceMesh& mesh, Eigen::Matrix2Xd &points, Eigen::Matrix2Xi &segments, const vector<pmp::Point> &details) {
        vector<pmp::Vertex> vecBoundary;
        vector<double> vecPoints;
        vector<int> vecSegments;
        PmpUtils::findBoundary(mesh, vecBoundary);
        size_t idx = 0;
        size_t maxNum = vecBoundary.size() + details.size();
        for (auto v : vecBoundary) {
            pmp::Point p = mesh.position(v);
            vecPoints.push_back(p[0]);
            vecPoints.push_back(p[1]);
            vecSegments.push_back(idx);
            vecSegments.push_back((idx + 1) % vecBoundary.size());
            idx++;
        }
        size_t tmp = idx;
        for (auto p : details) {
            vecPoints.push_back(p[0]);
            vecPoints.push_back(p[1]);
            vecSegments.push_back(idx);
            vecSegments.push_back(idx + 1 == maxNum ? tmp : idx + 1);
            idx++;
        }
        points = EigenUtils::mapMatrix2Xd(vecPoints);
        segments = EigenUtils::mapMatrix2Xi(vecSegments);
    }

    void buildMesh(SurfaceMesh &mesh, Matrix2Xd points2D, Matrix3Xi triangles, Matrix2Xi segmentations, vector<pmp::Vertex> &vecVertex) {
        for (size_t i = 0; i < points2D.cols(); i++) {
            Vertex v = mesh.add_vertex(pmp::Point(points2D(0, i), points2D(1, i), 0));
            vecVertex.push_back(v);
        }
        for (size_t i = 0; i < triangles.cols(); i++) {
            mesh.add_triangle(vecVertex[triangles(0, i)], vecVertex[triangles(1, i)], vecVertex[triangles(2, i)]);
        }

    }

    void findNewContours(SurfaceMesh &mesh, vector<Vertex> vertices, size_t segmentNum, ContourCollection &contours) {
        //首先定id为0为第一圈轮廓的基准点
        vector<Contour> newVecContours;
        size_t level = 0, start = 0, lastSize = 0;
        const vector<Contour>& vecContours = contours.getContours();
        auto isContour = mesh.get_edge_property<bool>("contour_level");
        vector<int> contourId;
        vector<bool> visit(vertices.size());
        //用这个量标记寻找完所有contours
        while (segmentNum > 0) {
            //首先传入基准点
            contourId.clear();
            contourId.push_back(start);
            visit[start] = true;
            while(contourId.size() != lastSize) {
                //寻找最后一个点的邻居
                lastSize = contourId.size();
                for(auto v : mesh.vertices(vertices[contourId.back()])) {
                    pmp::Edge e = mesh.find_edge(v, vertices[contourId.back()]);
                    if(isContour[e] && !visit[v.idx()]) {
                        contourId.push_back(v.idx());
                        visit[v.idx()] = true;
                        break;
                    }
                }
            }
            assert(vecContours[level].rows() <= contourId.size());
            segmentNum -= contourId.size();
            //更新初始节点
            start = vecContours[level].rows();
            //生成contour
            Matrix3Xf matrixC;
            matrixC.resize(3, contourId.size());
            for(size_t i = 0; i < contourId.size(); i++) {
                pmp::Point p = mesh.position(vertices[contourId[i]]);
                matrixC.col(i) << p[0], p[1], p[2];
            }
            Contour contour(matrixC);
            newVecContours.push_back(contour);
            level++;
        }
        ContourCollection newCollection(newVecContours);
        contours = newCollection;
    }
}


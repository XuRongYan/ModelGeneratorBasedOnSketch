//
// Created by 徐溶延 on 2019/9/6.
//

#ifndef MODELGENERATORBASEDONSKETCH_PMPEXCEPTION_H
#define MODELGENERATORBASEDONSKETCH_PMPEXCEPTION_H

#include <exception>
#include <string>

using namespace std;

class PmpException : public exception {
protected:
    const string VERTEX_NOT_FOUND = "vertex not found";
    const string TYPE_NOT_FOUND = "type not found";
public:
    PmpException() {}

    ~PmpException() override {

    }
};

class VertexNotFoundException : public PmpException {
public:
    VertexNotFoundException() {}

    ~VertexNotFoundException() override {

    }

    const char *what() const noexcept override {
        return VERTEX_NOT_FOUND.c_str();
    }
};

class TypeNotFoundException : public PmpException {
public:
    TypeNotFoundException() {}

    ~TypeNotFoundException() override {

    }

    const char *what() const noexcept override {
        return TYPE_NOT_FOUND.c_str();
    }
};

class InvalidCircumferenceException : public PmpException {
public:
    InvalidCircumferenceException() {}

    ~InvalidCircumferenceException() override {

    }

    const char *what() const noexcept override {
        return "it is not permitted to get the circumference of a not-outskirt contour";
    }
};


#endif //MODELGENERATORBASEDONSKETCH_PMPEXCEPTION_H

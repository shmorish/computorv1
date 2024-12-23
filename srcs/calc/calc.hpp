#ifndef CALC_HPP
#define CALC_HPP

#include <iostream>
#include <parser.hpp>

class Calc
{
private:
    Parser _parser;
    double _squared_x;
    double _x;
    double _constant;

    int _degree;

    void solveQuadraticEquation();
    void solveLinearEquation();
    void solveConstantEquation();

public:
    Calc(Parser &parser);
    ~Calc();
};

#endif // CALC_HPP
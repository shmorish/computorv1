#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <list>
#include <sstream>
#include <map>

class Parser
{
private:
    // double _squared_x;
    // double _x;
    // double _constant;

    // [0] = constant, [1] = x, [2] = x^2, [3] = x^3, ...
    std::map<int, double> _formula;
    int _degree;

    std::list<std::list<std::string>> split_equation_into_tokens(const std::string &line);
    void check_tokens(std::list<std::string> &tokens);
    void store_tokens(std::list<std::list<std::string>> &tokens);
    void printReducedForm();
    void printPolynomialDegree();
public:
    Parser(std::string &line);
    ~Parser();
    double get_sqared_x() const { return _formula.at(2); }
    double get_x() const { return _formula.at(1); }
    double get_constant() const { return _formula.at(0); }
    int get_degree() const { return _degree; }
};

#endif // PARSER_HPP
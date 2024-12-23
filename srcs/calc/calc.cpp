#include <calc.hpp>

Calc::Calc(Parser &parser) : _parser(parser) , _squared_x(parser.get_sqared_x()), _x(parser.get_x()), _constant(parser.get_constant()), _degree(parser.get_degree())
{
    if (_degree == 2)
    {
        solveQuadraticEquation();
    }
    else if (_degree == 1)
    {
        solveLinearEquation();
    }
    else if (_degree == 0)
    {
        solveConstantEquation();
    }
    else
    {
        throw std::invalid_argument("The polynomial degree is stricly greater than 2, I can't solve.");
    }
}

Calc::~Calc()
{
}

void Calc::solveQuadraticEquation()
{
    double delta = _x * _x - 4 * _squared_x * _constant;
    if (delta > 0)
    {
        double x1 = (-_x + sqrt(delta)) / (2 * _squared_x);
        double x2 = (-_x - sqrt(delta)) / (2 * _squared_x);
        std::cout << "Discriminant is strictly positive, the two solutions are:" << std::endl;
        std::cout << "x1 = " << x1 << std::endl;
        std::cout << "x2 = " << x2 << std::endl;
    }
    else if (delta == 0)
    {
        double x = -_x / (2 * _squared_x);
        std::cout << "Discriminant is zero, the solution is:" << std::endl;
        std::cout << "x = " << x << std::endl;
    }
    else
    {
        std::cout << "Discriminant is strictly negative, the two solutions are:" << std::endl;
        std::cout << "x1 = (-" << _x << " + i√" << -delta << ") / " << 2 * _squared_x << std::endl;
        std::cout << "x2 = (-" << _x << " - i√" << -delta << ") / " << 2 * _squared_x << std::endl;
    }
}

void Calc::solveLinearEquation()
{
    double x = -_constant / _x;
    std::cout << "The solution is:" << std::endl;
    std::cout << "x = " << x << std::endl;
}

void Calc::solveConstantEquation()
{
    std::cout << "The equation has no solution" << std::endl;
}
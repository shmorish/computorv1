#include <iostream>
#include <string>
#include <parser.hpp>
#include <calc.hpp>

using namespace std;

int main(int ac, char **av) {
    if (ac != 2) {
        cout << "Usage: " << av[0] << " [equation]" << endl;
        return 1;
    }

    string line = av[1];
    Parser parser(line);
    Calc calc(parser);
    return 0;
}
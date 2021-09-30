#include <iostream>

#include "switch.hpp"


int main() {
    int a = 12;
    Switch{a,
        Case{5, []{
            std::cout << "Five";
        }},
        Case{11, []{
            std::cout << "From 0 to 10";
        }},
        Default{[]{
            std::cout << "Default";
        }}
    };

    Switch{
        Case{false, []{
            std::cout << false;
        }},
        Case{a < 10, []{
            std::cout << "LT 10";
        }},
        Case{a > 10 && a < 20, []{
            std::cout << "Range";
        }},
        Default{[]{
            std::cout << "Default";
        }}
    };
}

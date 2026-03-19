#include "Vector.hpp"
#include <iostream>

int main() {
    Vector v1{1.0, 2.0, 3.0};
    Vector v2{4.0, 5.0, 6.0};

    Vector v3 = v1 + v2; 
    Vector v4 = v1 - v2; 
    Vector v5 = v1 * 2.0;
    Vector v6 = 2.0 * v1;
    Vector v7 = v1;
    Vector v8 = 2 / v1;
    Vector v9 = 0 / v1;

    std::cout << "v1: "; v1.afficher();
    std::cout << "expected v1: Vector(1.0, 2.0, 3.0)" << std::endl;

    std::cout << "v2: "; v2.afficher();
    std::cout << "expected v2: Vector(4.0, 5.0, 6.0)" << std::endl;

    std::cout << "v1 + v2: "; v3.afficher();
    std::cout << "expected v1 + v2: Vector(5.0, 7.0, 9.0)" << std::endl;

    std::cout << "v1 - v2: "; v4.afficher();
    std::cout << "expected v1 - v2: Vector(-3.0, -3.0, -3.0)" << std::endl;

    std::cout << "v1 * 2.0: "; v5.afficher();
    std::cout << "expected v1 * 2.0: Vector(2.0, 4.0, 6.0)" << std::endl;

    std::cout << "2.0 * v1: "; v6.afficher();
    std::cout << "expected 2.0 * v1: Vector(2.0, 4.0, 6.0)" << std::endl;

    std::cout << "v7: "; v7.afficher();
    std::cout << "expected v7: Vector(1.0, 2.0, 3.0)" << std::endl;

    std::cout << "v8: "; v8.afficher();
    std::cout << "expected v8: Vector(0.5, 1.0, 1.5)" << std::endl;
    
    std::cout << "v9: "; v9.afficher();
    std::cout << "expected v9: Vector(0.5, 1.0, 1.5)" << std::endl;
    return 0;
}
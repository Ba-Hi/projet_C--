#include <iostream>
#include "Vector.hpp"


Vector::Vector(double x_init, double y_init, double z_init) :
        x(x_init), y(y_init), z(z_init) {}

    
Vector Vector::operator+(const Vector& autre) const {
        return Vector(x+ autre.x, y + autre.y , z+autre.z);
    }

Vector Vector::operator-(const Vector& autre) const {
        return Vector(x - autre.x, y - autre.y , z - autre.z);
    }

Vector Vector::operator*(double scalaire) const {
    return Vector(x*scalaire, y*scalaire, z*scalaire);
}

Vector &Vector::operator=(const Vector& source){
    if (this != &source) {
    x = source.x;
    y = source.y;
    z = source.z;
    }
    return *this; // chainage
}

void Vector::afficher() const {
    std::cout << "Vector(" << x << ", " << y << ", " << z << ")" << std::endl;
}

Vector operator*(double s, const Vector&p) {
    return Vector(p.x * s, p.y * s, p.z * s);
}
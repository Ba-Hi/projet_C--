#ifndef VECTOR_HPP
#define VECTOR_HPP

class Vector{
public :
    double x;
    double y;
    double z;

public :
    Vector(double x_init, double y_iniy, double z_init);
    Vector operator+(const Vector& autre) const;
    Vector operator-(const Vector& autre) const;
    Vector operator*(double scalaire) const;
    Vector& operator=(const Vector& source);
    void afficher() const;
};

Vector operator*(double s, const Vector&p);

#endif
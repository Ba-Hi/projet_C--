#ifndef VECTOR_HPP
#define VECTOR_HPP

class Vector{
private :
    double x_;
    double y_;
    double z_;

public :
    Vector(double x_init, double y_iniy, double z_init);
    double x() const;
    double y() const;
    double z() const;
    Vector operator+(const Vector& autre) const;
    Vector operator-(const Vector& autre) const;
    Vector operator*(double scalaire) const;
    Vector& operator=(const Vector& source);
    Vector operator/(const Vector& autre) const;
    void afficher() const;

    bool operator==(const Vector& v) const;
    Vector& operator+=(const Vector& v);
    Vector& operator-=(const Vector& v);
    Vector& operator*=(double scalar);
    Vector& operator/=(double scalar);
    friend Vector operator*(double scalar, const Vector& v);
    
};


#endif
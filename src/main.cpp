#include <iostream>
#include <cmath>
#include "Univers.hpp"
#include "Particule.hpp"
#include "Vector.hpp"

int main() {
    std::cout << "=== Solar system simulation ===\n";

    Univers u(3);

    // Circular orbit speed: v = sqrt(G*M/r), G=1, M_sun=1
    double r_earth   = 1.0;
    double r_jupiter = 5.36;
    double r_halley  = 34.75;

    double v_earth   = 1;    // = 1.0
    double v_jupiter = 0.425;  // = 0.425
    double v_halley  = 0.0296;   // = 0.0296
    // Note: Halley is a comet so its speed differs from circular — keeping lab value
    
    // Sun
    u.ajouterParticule(Particule(
        Vector(0, 0, 0), Vector(0, 0, 0), 1.0, 0, 0, Vector(0,0,0)));

    // Earth — circular orbit
    u.ajouterParticule(Particule(
        Vector(0, r_earth, 0), Vector(-v_earth, 0, 0), 3.0e-6, 1, 0, Vector(0,0,0)));

    // Jupiter — circular orbit
    u.ajouterParticule(Particule(
        Vector(0, r_jupiter, 0), Vector(-v_jupiter, 0, 0), 9.55e-4, 2, 0, Vector(0,0,0)));

    // Halley — comet, keeping lab value
    u.ajouterParticule(Particule(
        Vector(r_halley, 0, 0), Vector(0, v_halley, 0), 1.0e-14, 3, 0, Vector(0,0,0)));

    std::cout << "Running...\n";
    u.avancerParticules(468.5, 0.015);  // smaller dt for stability
    std::cout << "Done. Results saved to output.csv\n";

    return 0;
}
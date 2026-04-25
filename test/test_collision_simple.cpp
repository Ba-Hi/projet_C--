#include <iostream>
#include <fstream>
#include <cmath>
#include "UniversLJ.hpp"
#include "Particule.hpp"
#include "Vector.hpp"

int main() {

    const double sigma   = 1.0;
    const double epsilon = 1.0;
    const double r_cut   = 2.5 * sigma;
    const double dt      = 0.0001;
    const double tEnd    = 5.0;
    const double m       = 1.0;

    const double d = std::pow(2.0, 1.0/6.0) * sigma;

    UniversLJ u(2, Vector(30.0, 30.0, 0.0), r_cut, epsilon, sigma);

    // Groupe A : 3 particules fixes
    for (int i = 0; i < 3; i++) {
        Vector pos(10.0 + i * d, 10.0, 0.0);
        u.ajouterParticule(Particule(pos, Vector(0,0,0), m, i, 0, Vector(0,0,0)));
    }

    // Groupe B : 1 particule tombante
    u.ajouterParticule(Particule(
        Vector(10.0 + d, 14.0, 0.0),
        Vector(0.0, -3.0, 0.0),
        m, 3, 1, Vector(0,0,0)));

    u.initialiserCellules();
    u.calculerForces();

    
    std::ofstream csv("collision_simple.csv");
    csv << "t x0 y0 x1 y1 x2 y2 x3 y3\n";

    std::vector<Vector> forces(4, Vector(0,0,0));
    for (size_t i = 0; i < u.getParticules().size(); i++)
        forces[i] = u.getParticules()[i].getForce();

    double t = 0.0;
    int step = 0;
    const int save_every = 5;

    while (t < tEnd) {
        t += dt;
        step++;

        std::vector<Vector> forces_old = forces;

        for (size_t i = 0; i < u.getParticules().size(); ++i) {
            Particule& p = u.getParticules()[i];
            Vector accel  = forces[i] * (1.0 / p.getMasse());
            p.setPosition(p.getPosition() + p.getVitesse()*dt + accel*(0.5*dt*dt));
        }

        u.mettreAJourCellules();
        u.calculerForces();

        for (size_t i = 0; i < u.getParticules().size(); i++)
            forces[i] = u.getParticules()[i].getForce();

        for (size_t i = 0; i < u.getParticules().size(); ++i) {
            Particule& p = u.getParticules()[i];
            p.setVitesse(p.getVitesse() + (forces[i] + forces_old[i]) * (0.5*dt / p.getMasse()));
        }

        // Détection explosion
        double fmax = 0.0;
        for (const auto& p : u.getParticules())
            fmax = std::max(fmax, p.getForce().norm());
        if (fmax > 1e6) {
            std::cerr << "EXPLOSION t=" << t << " fmax=" << fmax << "\n";
            return 1;
        }

        if (step % save_every == 0) {
            csv << t;
            for (const auto& p : u.getParticules())
                csv << " " << p.getPosition().x() << " " << p.getPosition().y();
            csv << "\n";
        }
    }

    std::cout << "Done. collision_simple.csv written.\n";
    return 0;
}
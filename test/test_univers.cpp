#include <vector>
#include "Particule.hpp"
#include "Vector.hpp"
#include "Univers.hpp"
#include <cstdlib>

#include <cmath>
#include <iostream>
#include <chrono> // Pour la mesure du temps


int main() {
    Univers u(3, pow(2, 15));

    // creation de 2^15 particules uniformément distribuées sur le cube [0; 1] × [0; 1] × [0; 1].
    for (int i = 0; i < pow(2, 15); ++i) {
        double x = static_cast<double>(rand()) / RAND_MAX;
        double y = static_cast<double>(rand()) / RAND_MAX;
        double z = static_cast<double>(rand()) / RAND_MAX;
        Vector position(x, y, z);
        Vector vitesse(0.0, 0.0, 0.0);
        double masse = 1.0;
        int identifiant = i;
        int categorie = 0;
        Vector force(0.0, 0.0, 0.0);

        Particule p(position, vitesse, masse, identifiant, categorie, force);
        u.ajouterParticule(p);
    }

    std::cout << "Univers créé avec " << u.n_particules << " particules." << std::endl;
    


    int n = 32768; // 2^15


    // sans reserve
    Univers u1(3, 0); 
    auto start1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        u1.ajouterParticule(Particule(Vector(0,0,0), Vector(0,0,0), 1.0, i, 0, Vector(0,0,0)));
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff1 = end1 - start1;
    std::cout << "Temps SANS reserve: " << diff1.count() << " s" << std::endl;


    // avec reserve
    Univers u2(3, n); 
    auto start2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        u2.ajouterParticule(Particule(Vector(0,0,0), Vector(0,0,0), 1.0, i, 0, Vector(0,0,0)));
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff2 = end2 - start2;
    std::cout << "Temps AVEC reserve: " << diff2.count() << " s" << std::endl;



    for (int k = 1; k<8; k++) {
        auto start1 = std::chrono::high_resolution_clock::now();
        int n = pow(8, k);
        Univers u(3, n);
        for (int i = 0; i < n; ++i) {
            u1.ajouterParticule(Particule(Vector(0,0,0), Vector(0,0,0), 1.0, i, 0, Vector(0,0,0)));
        }
        auto end1 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff1 = end1 - start1;
        std::cout << "Temps pour :" << n << " particules est " << diff1.count() << " s" << std::endl;

    }

    return 0;
}

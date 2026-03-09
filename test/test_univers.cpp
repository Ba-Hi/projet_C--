#include <vector>
#include "Particule.hpp"
#include "Vector.hpp"
#include "Univers.hpp"
#include <cstdlib>

int main() {
    Univers u(3, 2^15);

    // creation de 2^15 particules uniformément distribuées sur le cube [0; 1] × [0; 1] × [0; 1].
    for (int i = 0; i < 2^15; ++i) {
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

    return 0;
}

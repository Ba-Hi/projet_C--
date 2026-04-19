#include <iostream>
#include <cmath>
#include "Univers_tp4.hpp"
#include "Particule.hpp"
#include "Vector.hpp"

int main() {

    double sigma = 1.0;
    double epsilon = 5.0;

    Univers_tp4 u(2, Vector(250, 40, 0), 2.5*sigma);

    // rectangle de 160*40 particules
    // remplir ligne par ligne
    for (int j = 0; j < 40; j++) {
        for (int i = 0; i < 160; i++) {
            int id = j*160 + i;
            u.ajouterParticule(Particule(
                Vector(i, j, 0), Vector(0, 0, 0), 1.0, id, 0, Vector(0,0,0)));
        }
    }

    double distance_carré_rectangle = pow(2, 1/6) / sigma;

    // carré de 40*40 particules en dessous du rectangle au milieu
    for (int j = 0; j < 40; j++) {
        for (int i = 0; i < 40; i++) {
            int id = (i + 60)* (40 + j + distance_carré_rectangle);
            u.ajouterParticule(Particule(
                Vector(i + 60, j + 40 + distance_carré_rectangle, 0), Vector(0, 10, 0), 1.0, id, 2, Vector(0,0,0)));
        }
    }

    
    return 0;
}
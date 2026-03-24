#include "Univers_tp4.hpp"
#include <iostream>



Univers_tp4::Univers_tp4(const int& dimension, const Vector& l_d, const double& r_cut)
            : 
            dimension(dimension),
            l_d(l_d),
            r_cut(r_cut),
            n_cd((1 / r_cut) * l_d),
{
    if (dimension > 3){
        throw std::invalid_argument("La dimension est soit 1D, 2D, ou 3D.");
    }
}

void Univers_tp4::initialiser() {
    celluleList.clear();

    int nx = (int)n_cd.x();
    int ny = (int)n_cd.y();
    int nz = (int)n_cd.z();

    int nbCellules = 0;
    if (dimension == 1) {
        nbCellules = n_cd.x();
    }
    else if (dimension == 2) {
        nbCellules = n_cd.x() * n_cd.y();
    }
    else {
        nbCellules = n_cd.x() * n_cd.y() * n_cd.z();
    }

    celluleList.reserve(nbCellules);

    // Création des cellules :
    for (int i=0; i<nx; i++){
        for (int j=0; j<ny; j++){
            for (int k=0; k<nz; k++){
                celluleList.emplace_back(r_cut, i, j, k);
            }
        }
    }

    // Création des voisins :
    for (int i=0; i<nx; i++){
        for (int j=0; j<ny; j++){
            for (int k=0; k<nz; k++){
                Cellule& c = celluleList[indice1D(i, j, k)];
                for (int dk = -1; dk <= 1; dk++)
                    for (int dj = -1; dj <= 1; dj++)
                        for (int di = -1; di <= 1; di++) {
                            int ni = i + di, nj = j + dj, nk = k + dk;
                            if (ni >= 0 && ni < nx &&
                                nj >= 0 && nj < ny &&
                                nk >= 0 && nk < nz) c.ajouterVoisine(&celluleList[indice1D(ni, nj, nk)]);
               }
            }
        }
    }
}

int Univers_tp4::indice1D(int i, int j, int k){
    return i + n_cd.x() * (j + n_cd.y() * k);

}




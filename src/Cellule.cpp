#include "Cellule.hpp"

Cellule::Cellule(const double tailleCellule,int i, int j, int k, std::vector<Particule> particuleListInitiale):
tailleCellule(tailleCellule), i(i), j(j), k(k), particuleList(particuleListInitiale), centreCellule(i*tailleCellule + tailleCellule/2, j*tailleCellule + tailleCellule/2, k*tailleCellule + tailleCellule/2)
{}

double Cellule::getTailleCellule() const{
    return tailleCellule;
}

std::vector<Particule>& Cellule::getParticuleList() {
    return particuleList;
}

Cellule& Cellule::setTailleCellule(const double& taille){
    tailleCellule = taille;
    return *this;
}

Cellule& Cellule::setTailleCellule(const double& taille){
    tailleCellule = taille;
    return *this;
}

Cellule& Cellule::setListParticule(const std::vector<Particule>& listPart){
    particuleList = listPart;
    return *this;
}

Vector Cellule::getPositionCellule() const{
    return Vector(i, j, k);
}

void Cellule::ajouterVoisine(Cellule* voisine) {
    voisines.push_back(voisine);
}

void Cellule::ajouterParticule(const Particule& p) {
    particuleList.push_back(p);
}


std::vector<Cellule*>& Cellule::getVoisines(){
    return voisines;
};

void Cellule::viderParticules() {
    particuleList.clear();
}


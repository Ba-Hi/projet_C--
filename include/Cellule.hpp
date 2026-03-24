#ifndef CELLULE_HPP
#define CELLULE_HPP


#include "Particule.hpp"
#include "Vector.hpp"

#include <vector>

class Cellule {
private :
    int i;
    int j;
    int k;
    double tailleCellule;
    std::vector<Particule> particuleList;
    std::vector<Cellule*> voisines;

public :
    Cellule(const double tailleCellule, int i, int j, int k, std::vector<Particule> particuleListInitiale);
    
    double getTailleCellule() const; 
    
    std::vector<Particule>& getParticuleList();

    std::vector<Cellule*>& getVoisines();

    Cellule& setTailleCellule(const double& taille);
    
    Cellule& setListParticule(const std::vector<Particule>& listPart);

    Vector getPositionCellule() const;

    void ajouterVoisine(Cellule* voisine);

    void ajouterParticule(const Particule& p);
}



#endif
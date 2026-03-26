#ifndef UNIVERS_HPP
#define UNIVERS_HPP

#include <vector>
#include "Particule.hpp"
#include "Vector.hpp"

/**
 * @brief Représente un univers (TP3) contenant un ensemble de particules
 * 
 * Cette classe gère une collection de particules dans un espace de dimension donnée.
 * Elle permet d'ajouter des particules, de modifier leurs vitesses, de calculer les forces
 * et de faire évoluer le système au cours du temps.
 */

class Univers
{
private:
    int dimension; ///< Dimension de l'espace (1D, 2D, 3D)
    int n_particules; ///< Nombre de particules 
    std::vector<Particule> particuleList; ///> Liste des particules

public:
    /**
     * @brief Constructeur de l'univers
     * @param dim Dimension de l'espace
     * @param reserveCount Nombre de particules à réserver (optionnel)
     */
    Univers(int dim, int reserveCount = 0);

    /**
     * @brief Ajoute une particule à l'univers
     * @param p La particule à ajouter
     */
    void ajouterParticule(const Particule& p);

    /**
     * @brief Applique une vitesse uniforme à toutes les particules
     * @param v Vecteur vitesse appliqué
     */
    void modifierVitesseUniforme(const Vector& v);

    /**
     * @brief Affiche les informations de l'univers
     */
    void afficherUnivers() const;

    /**
     * @brief Fait évoluer les particules dans le temps
     * @param tEnd Temps final de la simulation
     * @param dt Pas de temps
     */
    void avancerParticules(double tEnd, double dt);

    /**
     * @brief Calcule les forces appliquées sur chaque particule
     * @return Un vecteur contenant les forces associées à chaque particule
     */
    std::vector<Vector> calculerForces() const;


    /**
     * @brief Getter de la dimension
     * @return La dimension de l'univers
     */
    int getDimension() const;

    /**
     * @brief Getter du nombre de particules
     * @return Nombre de particules
     */
    int getNombreParticules() const;

    /**
     * @brief Getter de la liste des particules
     * @return Référence à la liste de particules
     */
    const std::vector<Particule>& getParticules() const;

    /**
     * @brief Modifie la dimension.
     * @param dim Nouvelle dimension.
     * @return Référence sur l'objet courant.
     */
    Univers& setDimension(int dim) ;

    /**
     * @brief Modifie le nombre de particule.
     * @param n nombre de particules
     * @return Référence sur l'objet courant.
     */
    Univers& setNombreParticules(int n) ;

    /**
     * @brief Modifie la liste des particules.
     * @param particules nouvelle liste de particules
     * @return Référence sur l'objet courant.
     */
    Univers& setParticules(const std::vector<Particule>& particules) ;

};

#endif
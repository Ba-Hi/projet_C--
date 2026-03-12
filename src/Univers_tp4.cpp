#include "Univers_tp4.hpp"


Univers_tp4::Univers_tp4(const int& dimension, const Vector& l_d, const double& r_cut)
            : 
            dimension(dimension),
            l_d(l_d),
            r_cut(r_cut),
            n_cd((1 / r_cut) * l_d)
{
}

void Univers_tp4::initialiser() {
    celluleList.clear();
}




/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Automne 2014
 *  Squelette pour le TP3
*/

#ifndef CARTE_HEADER
#define CARTE_HEADER

#include <assert.h>
#include <istream>
#include <list>
#include <set>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <verto.h>
#include "coordonnee.h"

using namespace std;

class Carte {
public:

    void ajouterLieu(string const &nomlieu, const Coordonnee &coordonnee);

    void ajouterRoute(const string &nom, const list<string> &noms);

    double calculerTrajet(const string &origine, const list<string> &destination,
            std::list<string> &out_cheminnoeuds, std::list<string> &out_cheminroutes) const;

    double calculerChemin(const int origine, const int destination,
            std::list<string> &out_cheminnoeuds, std::list<string> &out_cheminroutes) const;

private:

    struct Lieu { // sommet
        string nomlieu;

        Coordonnee coordonnee;

        vector<int> aretes;    // aretes (sommets des aretes sortantes)
        vector<string> routes; // nom des routes des aretes (meme indice)
        vector<int> destinations;
        mutable bool visite;
    };

    map<string, unsigned int> indices; // todo changer pour unorderd_map O(1)
    vector<Lieu> lieux; // sommets du graphe

    void ajouterArete(const int io, const int id, string const &nomRoute);

    friend istream &operator>>(istream &is, Carte &carte);

    friend ostream &operator<<(ostream &os, Carte &carte);

    inline double distanceEuclidienne(int const iOrigine, int const iDestination) const {
        Coordonnee const &coordonneeD = lieux[iDestination].coordonnee;
        return lieux[iOrigine].coordonnee.distance(coordonneeD);

    }

    void AStarAlgorithm(int const iOrigine, int const iDestination, vector<double> &distancesmin, vector<int> &parents) const;

    void chemin(int iorigine, vector<int> const &parents, list<string> &out_cheminnoeuds, list<string> &out_cheminroutes) const;


    inline double heuristique(int const iOrigine, int const iDestination) const {
        return distanceEuclidienne(iOrigine, iDestination);
    };

    set<int> getIndices(string const &nomorigine, list<string> const &nomdestinations)const;

    int trouveLePlusPres(int origine, set<int> vector1) const;
};

#endif


/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Automne 2014
 *  Squelette pour le TP3
*/

#include "carte.h"
#include <limits>
#include <bits/stream_iterator.h>

const double INFINI = std::numeric_limits<double>::infinity();
int const &INDEFINI = -1;

void Carte::ajouterLieu(const string &nomlieu, const Coordonnee &c) {
    Lieu lieu = Lieu(nomlieu, c);
    lieux.push_back(lieu);
    int indice = indices.size();
    indices[nomlieu] = indice; // todo: alouer sur le heap ?
}

void Carte::ajouterRoute(const string &nomroute, const list<string> &route) {
    // Exemple de ligne dans une carte : «Jeanne-Mance : a b c d ; »
    // Donc, route est une séquence (liste) : <a, b, c, d, e>.
    // Il faut alors ajouter les segments de route (arêtes/arcs) : (a,b), (b,c), (c,d), (d,e).
    // Il ne faut pas ajouter (e,d), (d,c), (c,b), (b,a).
    // Les sens uniques doivent être considérées.
    // todo ajouter nom de routes
    int io, id; // indice origine et destination
    list<string>::const_iterator it = route.begin();
    io = indices.at(*it);
    ++it;
    while (it != route.end()) {
        id = indices.at(*it);
        ajouterAreteNonOrientee(io, id);
        io = id;
        ++it;
    }
}

void Carte::ajouterAreteNonOrientee(const int io, const int id) {

    lieux[io].voisins.push_back(id);
}

double Carte::calculerTrajet(const string &nomorigine, const list<string> &nomsdestinations,
        std::list<string> &out_cheminnoeuds, std::list<string> &out_cheminroutes) const {
    // À compléter. La version actuelle génère un trajet valide, mais généralement non optimal pour plusieurs destinations.
    // todo: creer un graphe des destinations (avec les distances calcules en 1) et calculer le minimum hamilton cycle ? parcourt min en passant par chaques

    string position = nomorigine;
    double total = 0;
    for (list<string>::const_reverse_iterator iter = nomsdestinations.rbegin(); iter != nomsdestinations.rend(); ++iter) {
        total += calculerChemin(*iter, position, out_cheminnoeuds, out_cheminroutes);
        position = *iter;
    }
    total += calculerChemin(nomorigine, position, out_cheminnoeuds, out_cheminroutes);
    return total;
}

double Carte::calculerChemin(const string &nomorigine, const string &nomdestination,
        std::list<string> &out_cheminnoeuds, std::list<string> &out_cheminroutes) const {
    // todo: 1)utiliser djiktra ou A* pour les distances entres les différentes destination

    std::vector<double> distances;
    std::vector<int> parents;

    const int iOrigine = indices.at(nomorigine);
    const int iDestination = indices.at(nomdestination);

    DijkstraAlgorithm(iOrigine, distances, parents);

    DijkstraChemin(iDestination, parents, out_cheminnoeuds, out_cheminroutes);

    return distances[iDestination];
}


void Carte::DijkstraChemin(int indexLieu, const std::vector<int> &parents, std::list<string> &out_cheminnoeuds, std::list<string> &out_cheminroutes) const {
    if (!out_cheminnoeuds.empty()) indexLieu = parents[indexLieu];
    for (; indexLieu != -1; indexLieu = parents[indexLieu]) {
        out_cheminnoeuds.push_front(lieux[indexLieu].nomlieu);
        // todo ajouter noms de routes
    }
}

void Carte::DijkstraAlgorithm(const int iOrigine, vector<double> &distances, vector<int> &parents) const {
    // distances[v] <- infini
    distances.clear();
    distances.resize(lieux.size(), INFINI);

    // parents[v] <- indefini
    parents.clear();
    parents.resize(lieux.size(), INDEFINI);

    distances[iOrigine] = 0;

    set<int> filePrioritaire; // todo : utiliser une fileprioritaire ? ou de fibonachi ?
    filePrioritaire.insert(iOrigine);

    while (!filePrioritaire.empty()) {
        int v = *filePrioritaire.begin();
        double distance = distances[v];
        filePrioritaire.erase(filePrioritaire.begin());

        //std::cout << "active indexLieu " << v << " distance " << distance << std::endl;

        Coordonnee const &coorV = lieux[v].coor;
        // pour toutes les arretes (v,w) depuis sommet v
        const std::vector<int> &voisins = lieux[v].voisins;
        for (int w : voisins) {

            // la distance est calcule ici plutot que lors de la creation du graphe : on ne calcule que les sommets visites
            // signifie moins de memoire et moins de calculs
            double d = distance + lieux[w].coor.distance(coorV);

            if (d < distances[w]) {
                filePrioritaire.erase(w); // different de la version des notes de cours // todo a verifier
                parents[w] = v;
                distances[w] = d;
                filePrioritaire.insert(w);
            }
            // for (unsigned int j = 0; j < distances.size(); ++j) {
            //     std::cout << j << " d = " << distances[j] << ", ";
            // }
            // std::cout << std::endl;
        }
    }
}

// afficher les données de la carte
ostream &operator<<(ostream &os, Carte &carte) {

    for (unsigned int v = 0; v < carte.lieux.size(); ++v) {
        os << endl << carte.lieux[v].nomlieu << " -> ";
        const std::vector<int> &voisins = carte.lieux[v].voisins;
        for (int w : voisins) {
            os << carte.lieux[w].nomlieu << ", ";
        }
    }

    os << endl;
    return os;
}


/* Lire une carte. */
istream &operator>>(istream &is, Carte &carte) {
    // Lire les lieux
    while (is) {
        string nomlieu;
        is >> nomlieu;
        if (nomlieu == "---") break;
        Coordonnee coor;
        is >> coor;
        carte.ajouterLieu(nomlieu, coor);
    }

    // Lire les routes
    while (is) {
        string nomroute;
        is >> nomroute;
        if (nomroute == "---" || nomroute == "" || !is) break;

        char deuxpoints;
        is >> deuxpoints;
        assert(deuxpoints == ':');

        std::list<std::string> listenomslieux;

        string nomlieu;
        while (is) {
            is >> nomlieu;
            if (nomlieu == ";") break;
            assert(nomlieu != ":");
            assert(nomlieu.find(";") == string::npos);
            listenomslieux.push_back(nomlieu);
        }

        assert(nomlieu == ";");
        carte.ajouterRoute(nomroute, listenomslieux);
    }

    return is;
}


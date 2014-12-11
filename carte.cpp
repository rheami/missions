/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Automne 2014
 *  Squelette pour le TP3
*/

#include "carte.h"
#include <limits>
#include "filePrioritaire.h"

const double INFINI = std::numeric_limits<double>::infinity();
int const &INDEFINI = -1;

void Carte::ajouterLieu(const string &nomlieu, const Coordonnee &coordonnee) {
    lieux.push_back(Lieu());
    int indice = lieux.size()-1;
    lieux[indice].nomlieu = nomlieu;
    lieux[indice].coordonnee = coordonnee;
    indices[nomlieu] = indice;
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
        ajouterArete(io, id, nomroute);
        io = id;
        ++it;
    }
}

void Carte::ajouterArete(int io, int id, string const &nomroute) {
    lieux[io].aretes.push_back(id);
    lieux[io].routes.push_back(nomroute);
}

double Carte::calculerTrajet(const string &nomorigine, const list<string> &nomsdestinations,
        std::list<string> &out_cheminnoeuds, std::list<string> &out_cheminroutes) const {
    // À compléter. La version actuelle génère un trajet valide, mais généralement non optimal pour plusieurs destinations.

    std::set<int> destinations = getIndices(nomorigine, nomsdestinations);

    int iOrigine= indices.at(nomorigine);
    int iCourent= iOrigine;
    int lepluspres;
    double total = 0;
    for (unsigned int i = 0; i < destinations.size(); ++i){
        lepluspres = -1;
        double distanceMin = INFINI;
        std::vector<double> distances;
        std::vector<int> parents;
        cout << " le plus pres de " << lieux[iCourent].nomlieu << endl;
        for (int it : destinations){
            cout << it << " - ";
            if (it!=iCourent && destinations.find(it)!=destinations.end()){
                AStarAlgorithm(iCourent, it, distances, parents); // on peut arreter si > distanceMin
                double distance = distances[it];
                cout << "distance a " << lieux[it].nomlieu << " = " << distance << endl;
                if (distance < distanceMin){
                    distanceMin = distance;
                    lepluspres = it;
                }
            }
        }
        if (lepluspres != -1) {
            chemin(lepluspres, parents, out_cheminnoeuds, out_cheminroutes);
            destinations.erase(lepluspres);

            total += distances[lepluspres];
            iCourent = lepluspres;
        }

    }
    if (lepluspres == -1) { // retourne au point de depart
        iCourent = *destinations.begin();
        total += calculerChemin(iCourent, iOrigine, out_cheminnoeuds, out_cheminroutes);

    }
    return total;
}

double Carte::calculerChemin(const int iOrigine, const int iDestination,
        std::list<string> &out_cheminnoeuds, std::list<string> &out_cheminroutes) const {

    std::vector<double> distances;
    std::vector<int> parents;

    AStarAlgorithm(iOrigine, iDestination, distances, parents);

    chemin(iDestination, parents, out_cheminnoeuds, out_cheminroutes);

    return distances[iDestination];
}


void Carte::chemin(int iorigine, const std::vector<int> &parents, std::list<string> &out_cheminnoeuds, std::list<string> &out_cheminroutes) const {
    //if (!out_cheminnoeuds.empty()) iorigine = parents[iorigine];
    int suivant = -1;
    for (; iorigine != -1; iorigine = parents[iorigine]) {
        if (lieux[iorigine].nomlieu != out_cheminnoeuds.front()){
            out_cheminnoeuds.push_front(lieux[iorigine].nomlieu);
        }

        // ajouter noms de routes
        if (suivant != -1) {
            unsigned int i = 0;
            std::vector<int> voisins = lieux[iorigine].aretes;
            for (; i != voisins.size(); ++i) { // cherche dans un tableau de 4 environ
                if (voisins[i] == suivant) break;
            }
            assert(i != voisins.size()); // la route est dans le tableau
            string nomroute = lieux[iorigine].routes[i];
            if (nomroute != out_cheminroutes.front()){
                out_cheminroutes.push_front(nomroute);
            }
        }
        suivant = iorigine;
    }
}

void Carte::AStarAlgorithm(const int iOrigine, const int iDestination, vector<double> &distances, vector<int> &parents) const {
    // distances[v] <- infini
    distances.clear();
    distances.resize(lieux.size(), INFINI);

    // parents[v] <- indefini
    parents.clear();
    parents.resize(lieux.size(), INDEFINI);

    distances[iOrigine] = 0.0;

    FilePrioritaire<int, double> filePrioritaire; // zone exploree // todo : utiliser une file de fibonachi O(1)

    filePrioritaire.inserer(iOrigine, 0.0);

    while (!filePrioritaire.estVide()) {

        auto v = filePrioritaire.enleverMinimum();
        
        if (v == iDestination) { 
            break;
        }
        
        double distance_a_v = distances[v];

        // pour toutes les arretes (v,w) depuis sommet v
        const std::vector<int> &voisins = lieux[v].aretes;
        for (unsigned int i = 0; i < voisins.size(); i++) {
            const int w=voisins[i];
            double d = distance_a_v + distanceEuclidienne(v, w);
            if (d < distances[w]) {
                distances[w] = d;
                parents.at(w) = v;
                const double &prioritee = d + heuristique(w, iDestination); // A* ou DijkstraAlgorithm si heuristique = 0
                filePrioritaire.inserer(w, prioritee); // ou modifier la priorite si possible
            }
        }
    }
}

// afficher les données de la carte
ostream &operator<<(ostream &os, Carte &carte) {

    for (unsigned int v = 0; v < carte.lieux.size(); ++v) {
        os << endl << carte.lieux[v].nomlieu << " -> ";
        const std::vector<int> &voisins = carte.lieux[v].aretes;
        for (auto & element : voisins) {
            os << carte.lieux[element].nomlieu << ", ";
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

set<int> Carte::getIndices(string const &nomorigine, list<string> const &nomdestinations)const {
    std::set<int> destinations;

    destinations.insert(indices.at(nomorigine));
    for (auto it : nomdestinations) {
        destinations.insert(indices.at(it));
    }

    return destinations;
}



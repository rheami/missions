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
    int indice = lieux.size();
    lieux.push_back(Lieu());
    lieux[indice].nomlieu = nomlieu;
    lieux[indice].coordonnee = coordonnee;
    indices[nomlieu] = indice;
}

void Carte::ajouterArete(int io, int id, string const &nomroute) {
    int indice = lieux[io].aretes.size();
    lieux[io].aretes.push_back(Arete());
    lieux[io].aretes[indice].indicelieu = id;
    int route = routes.size();
    routes.push_back(nomroute);
    lieux[io].aretes[indice].indiceroute = route;
}

void Carte::ajouterRoute(const string &nomroute, const list <string> &route) {
    // Exemple de ligne dans une carte : «Jeanne-Mance : a b c d ; »
    // Donc, route est une séquence (liste) : <a, b, c, d, e>.
    // Il faut alors ajouter les segments de route (arêtes/arcs) : (a,b), (b,c), (c,d), (d,e).
    // Il ne faut pas ajouter (e,d), (d,c), (c,b), (b,a).
    // Les sens uniques doivent être considérées.
    // indices origine et destination
    int origine = INDEFINI;
    int destination = INDEFINI;
    for (auto it :route) {

        destination = indices.at(it);
        if (origine != INDEFINI) {
            ajouterArete(origine, destination, nomroute);
        }
        origine = destination;
    }
}


double Carte::calculerTrajet_1(const string &nomorigine, const list <string> &nomsdestinations,
        std::list <string> &out_cheminnoeuds, std::list <string> &out_cheminroutes) const {
    // À compléter. La version actuelle génère un trajet valide, mais généralement non optimal pour plusieurs destinations.

    int origine = indices.at(nomorigine);
    int destination;
    int position = origine;
    double total = 0;
    for (list<string>::const_reverse_iterator iter = nomsdestinations.rbegin(); iter != nomsdestinations.rend(); ++iter) {
        destination = indices.at(*iter);
        total += calculerChemin(destination, position, out_cheminnoeuds, out_cheminroutes);
        position = destination;
    }

    total += calculerChemin(origine, position, out_cheminnoeuds, out_cheminroutes);
    return total;
}

double Carte::calculerChemin(const int iOrigine, const int iDestination,
        std::list <string> &out_cheminnoeuds, std::list <string> &out_cheminroutes) const {

    std::unordered_map<int, int> parents;
    double distance = AStarAlgorithm(iOrigine, iDestination, parents);
    chemin(iDestination, parents, out_cheminnoeuds, out_cheminroutes);
    return distance;
}


void Carte::chemin(int courant, unordered_map<int, int> parents, std::list <string> &out_cheminnoeuds, std::list <string> &out_cheminroutes) const {

    int suivant = INDEFINI;
    for (; courant != INDEFINI; courant = parents[courant]) { // de la destination en remontant vers l origine
        // ajouter les noeuds
        string nomlieu = lieux[courant].nomlieu;
        if (out_cheminnoeuds.size()==0 || nomlieu != out_cheminnoeuds.front()) {
            out_cheminnoeuds.push_front(nomlieu);
        }

        // ajouter noms de routes :
        if (suivant != INDEFINI) {
            string nomroute = "";

            for (auto arete = lieux[courant].aretes.rbegin(); arete != lieux[courant].aretes.rend(); ++arete)
            {
                if (suivant == (*arete).indicelieu) {
                    string nomroute = routes[(*arete).indiceroute];
                    //assert(nomroute != "");
                    if (out_cheminroutes.size() == 0 || nomroute != out_cheminroutes.front()) {
                        out_cheminroutes.push_front(nomroute);
                    }
                    break;
                }
            }
        }
        suivant = courant;
    }
}

double Carte::AStarAlgorithm(const int origine, const int destination, unordered_map<int, int> &parents) const {
    std::unordered_map<int, double> distances;
    parents[origine] = INDEFINI;
    distances[origine] = 0.0;

    FilePrioritaire<int, double> filePrioritaire; // zone exploree

    filePrioritaire.inserer(origine, 0.0);

    while (!filePrioritaire.estVide()) {

        auto v = filePrioritaire.enleverMinimum();

        if (v == destination) break;

        double distance_v = distances[v];

        const std::vector <Arete> &voisins = lieux[v].aretes;
        // pour toutes les arretes sortantes (v,w) depuis sommet v
        for (unsigned int i = 0; i < voisins.size(); i++) {
            const int w = voisins[i].indicelieu;
            double d = distance_v + distanceEuclidienne(v, w);
            if (!distances.count(w) || d < distances[w]) {
                parents[w] = v;
                distances[w] = d;
                double h = heuristique(w, destination); // A* ou Dijkstra si heuristique = 0
                const double &prioritee = d + h;
                filePrioritaire.inserer(w, prioritee);
            }
        }
    }
    return distances[destination];
}

// afficher les données de la carte
ostream &operator<<(ostream &os, Carte &carte) {

    for (unsigned int v = 0; v < carte.lieux.size(); ++v) {
        os << endl << carte.lieux[v].nomlieu << " -> ";
        for (auto &element : carte.lieux[v].aretes) {
            os << carte.lieux[element.indicelieu].nomlieu << "=" << carte.routes[element.indiceroute] << ", ";
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

        std::list <std::string> listenomslieux;

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



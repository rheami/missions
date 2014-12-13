/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Automne 2014
 *  Squelette pour le TP3
*/

#include "carte.h"
#include <limits>
#include <unordered_map>
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

double Carte::calculerTrajet_1(const string& nomorigine, const list<string>& nomsdestinations,
        std::list<string>& out_cheminnoeuds, std::list<string>& out_cheminroutes) const
{
    // À compléter. La version actuelle génère un trajet valide, mais généralement non optimal pour plusieurs destinations.

    int origine = indices.at(nomorigine);
    int destination;
    int position = origine;
    double total = 0;
    for(list<string>::const_reverse_iterator iter=nomsdestinations.rbegin();iter!=nomsdestinations.rend();++iter){
        destination = indices.at(*iter);
        total += calculerChemin(destination, position, out_cheminnoeuds, out_cheminroutes);
        position = destination;
    }

    total += calculerChemin(origine, position, out_cheminnoeuds, out_cheminroutes);
    return total;
}

double Carte::calculerTrajet_2(const string &nomorigine, const list<string> &nomsdestinations,
        std::list<string> &out_cheminnoeuds, std::list<string> &out_cheminroutes) const {
    // on cherche la destination la plus pres pour le retour, puis la prochaine destination la plus pres de ce point etc
    // on ferme la boucle avec origine et derniere destination trouve

    std::set<int> destinations = getIndices(nomorigine, nomsdestinations);

    std::list<string> cheminnoeudsPlusPres;
    std::list<string> cheminroutesPlusPres;
    std::list<string> cheminnoeuds;
    std::list<string> cheminroutes;
    int iOrigine= indices.at(nomorigine);
    int lepluspres;
    int dernier = -1;
    int courent = iOrigine;
    double total = 0;

    for (; courent != INDEFINI; courent = lepluspres){
        lepluspres = INDEFINI;
        double distanceMin = INFINI;
        ////cerr << "lieu le + proche pour aller a " << lieux[courent].nomlieu << endl;
        for (int suivant : destinations){
            if (suivant != courent && destinations.find(suivant)!=destinations.end()){
                cheminnoeuds.clear();
                cheminroutes.clear();

                double distance = calculerChemin(suivant, courent, cheminnoeuds, cheminroutes);
                //cerr << "distance " << lieux[suivant].nomlieu << " a " << lieux[courent].nomlieu << " = " << distance << endl;
                if (distance < distanceMin){
                    distanceMin = distance;
                    lepluspres = suivant;
                    //cerr << "le plus pres est " << lieux[lepluspres].nomlieu << endl;
                    // remplacer liste1 par liste2
                    cheminnoeudsPlusPres.clear();
                    cheminroutesPlusPres.clear();
                    cheminnoeudsPlusPres.splice(cheminnoeudsPlusPres.begin(), cheminnoeuds);
                    cheminroutesPlusPres.splice(cheminroutesPlusPres.begin(), cheminroutes);

                    //cerr << "chemin : ";
                    //for (list<string>::const_iterator iter = out_cheminnoeuds.begin(); iter != out_cheminnoeuds.end(); ++iter)
                        //cerr << *iter << " ";
                    //cerr << endl;
                    //cerr << "route : ";
                    //for (list<string>::const_iterator iter = out_cheminroutes.begin(); iter != out_cheminroutes.end(); ++iter)
                        //cerr << *iter << " ";
                    //cerr << endl;
                }
            }
        }
        if (lepluspres != INDEFINI) {

            //cerr << "retire " << lieux[lepluspres].nomlieu << endl;
            destinations.erase(lepluspres);
            total += distanceMin;
            dernier = lepluspres; // correspond a la premiere destinationc++c=cccccccceegqrdahafjnHSGFADSA
            out_cheminnoeuds.splice(out_cheminnoeuds.begin(), cheminnoeudsPlusPres);
            out_cheminroutes.splice(out_cheminroutes.begin(), cheminroutesPlusPres); // O (1)
        }
    }

    assert(destinations.size()==0);
    // point de depart
    //cerr << "retour =  " << lieux[iOrigine].nomlieu << " a " << lieux[dernier].nomlieu << endl;
    total += calculerChemin(iOrigine, dernier, out_cheminnoeuds, out_cheminroutes);

    return total;
}

double Carte::calculerChemin(const int iOrigine, const int iDestination,
        std::list<string> &out_cheminnoeuds, std::list<string> &out_cheminroutes) const {


    std::unordered_map<int, int> parents;

    double distance = AStarAlgorithm(iOrigine, iDestination, parents);

    chemin(iDestination, parents, out_cheminnoeuds, out_cheminroutes);

    return distance;
}


void Carte::chemin(int courant, unordered_map<int, int> parents, std::list<string> &out_cheminnoeuds, std::list<string> &out_cheminroutes) const {

    int suivant = INDEFINI;
    for (; courant != INDEFINI; courant = parents[courant]) { // de la destination en remontant vers l origine
        // ajouter les noeuds
        if (lieux[courant].nomlieu != out_cheminnoeuds.front()){
            out_cheminnoeuds.push_front(lieux[courant].nomlieu);
        }

        // ajouter noms de routes
        if (suivant != INDEFINI) {
            unsigned int i = 0;
            std::vector<int> voisins = lieux[courant].aretes;

            for (; i != voisins.size(); ++i) { // cherche dans un tableau de 4 environ // utiliser un unordered set ?
                if (voisins[i] == suivant) break;
            }
            assert(i != voisins.size()); // la route est dans le tableau
            string nomroute = lieux[courant].routes[i];
            if (nomroute != out_cheminroutes.front()){
                out_cheminroutes.push_front(nomroute);
            }
        }
        suivant = courant;
    }
    //cerr << "affichage du chemin calcule : ";
    //for (list<string>::const_iterator iter = out_cheminnoeuds.begin(); iter != out_cheminnoeuds.end(); ++iter)
        //cerr << *iter << " ";
    //cerr << endl;
    //cerr << "affichage du chemin calcule : ";
    //for (list<string>::const_iterator iter = out_cheminroutes.begin(); iter != out_cheminroutes.end(); ++iter)
        //cerr << *iter << " ";
    //cerr << endl;

}

double Carte::AStarAlgorithm(const int origine, const int destination, unordered_map<int, int> &parents) const {
    //cerr << "A * algo" << endl;
    // distances[v] <- infini
    std::unordered_map<int, double> distances;
    // distances.clear();
    //distances.resize(lieux.size(), INFINI);


    // parents[v] <- indefini
    //parents.clear();
    //parents.resize(lieux.size(), INDEFINI);
    parents[origine] = INDEFINI;
    distances[origine] = 0.0;

    FilePrioritaire<int, double> filePrioritaire; // zone exploree // todo : utiliser une file de fibonachi O(1)

    filePrioritaire.inserer(origine, 0.0);

    while (!filePrioritaire.estVide()) {

        auto v = filePrioritaire.enleverMinimum();

        //if (distances[v] == INFINI)  break;
        if (v == destination)  break;
        
        double distance_v = distances[v];

        const std::vector<int> &voisins = lieux[v].aretes;
        // pour toutes les arretes sortantes (v,w) depuis sommet v
        for (unsigned int i = 0; i < voisins.size(); i++) {
            const int w=voisins[i];
            double d = distance_v + distanceEuclidienne(v, w);
            if (!distances.count(w) || d < distances[w]) {
                parents[w] = v;
                distances[w] = d;
                const double &prioritee = d + heuristique(w, destination); // A* ou DijkstraAlgorithm si heuristique = 0
                filePrioritaire.inserer(w, prioritee); // ou modifier la priorite si possible
            }
        }
    }
    return distances[destination];
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

    //destinations.insert(indices.at(nomorigine));
    for (auto it : nomdestinations) {
        //cout << "ajout de " << it;
        int indice = indices.at(it);
        destinations.insert(indice);
        //cout << " indice = " << indice << " lieux = " << lieux[indice].nomlieu;
    }

    return destinations;
}



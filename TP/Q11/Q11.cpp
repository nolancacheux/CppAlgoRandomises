
#include <algorithm> // Pour la fonction de tri std::sort()
#include <limits> // Pour la valeur num�rique maximale numeric_limits<int>::max()
#include <vector> // Pour utiliser des vecteurs
#include <tuple> // Pour stocker les donn�es de P1 dans un tuple
#include <string> // Pour stocker les noms de villes
#include <iostream> // Pour afficher les r�sultats interm�diaires
#include <fstream>
#include <random>
#include <iomanip>

using namespace std;

// Structure de donn�es pour les donn�es du probl�me P1
struct P1Data {
    int capacity; // Capacit� du sac � dos
    int num_objects; // Nombre d'objets disponibles
    vector<tuple<int, int, int>> objects; // (nom/index, consommation, b�n�fice)
};

// Structure de donn�es pour les donn�es du probl�me P2
struct P2Data {
    int num_cities; // Nombre de villes
    vector<string> city_names; // Noms des villes
    vector<vector<int>> distances; // Matrice des distances
};


struct SolutionP2 {
    vector<string> chemin;
    int distance;
};

SolutionP2 MeilleureSolution = {{}, -1 };

// Fonction pour lire les donn�es du probl�me P2 � partir d'un fichier
P2Data readP2File(std::string filename) {
    std::ifstream infile(filename);
    P2Data data;
    if (infile.is_open()) {
        infile >> data.num_cities;
        std::string city_name;
        for (int i = 0; i < data.num_cities; i++) {
            infile >> city_name;
            data.city_names.push_back(city_name);
        }
        data.distances.resize(data.num_cities, std::vector<int>(data.num_cities));
        for (int i = 0; i < data.num_cities; i++) {
            for (int j = 0; j < data.num_cities; j++) {
                infile >> data.distances[i][j];
            }
        }
        return data;
    }
    else {
        cerr << "Le fichier P2 ne peut pas �tre ouvert" << endl;
        exit(1);
    }
}
// Fonction pour afficher les donn�es du probl�me P2
void printP2Data(P2Data data) {
    std::cout << "Nombre de villes : " << data.num_cities << std::endl;
    std::cout << "Noms des villes :" << std::endl;
    for (int i = 0; i < data.num_cities; i++) {
        std::cout << data.city_names[i] << std::endl;
    }
    std::cout << "Matrice des distances :" << std::endl;
    for (const auto& row : data.distances) {
        for (const auto& distance : row) {
            std::cout << distance << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// Fonction pour r�soudre le probl�me P1 avec un algorithme glouton
int solveP2(P2Data& data, int seed) {
    vector<int> lst(data.num_cities, 0);
    int total_distance = 0;
    int finish = 0;

    //Choix al�atoire de la ville intiale
    std::uniform_int_distribution<int> distribution(0, lst.size() - 1);
    std::default_random_engine generator(seed);
    auto lastville = distribution(generator);
    vector<int> Solution = { lastville };
    lst[lastville] = 1; //On marque la ville choisie pour ne pas la r�utiliser

    //cout << "ville de depart : " << data.city_names[lastville] << " " << lastville << endl;

    //Calcul des 2 meilleurs villes et choix entre les 2
    while (finish == 0) {

        vector<pair<int, int>> Distance;
        //Calcul des distances entre la derni�re ville et toutes les autres villes
        for (int i = 0; i < lst.size(); ++i) {
            if (lst[i] == 0) { //On v�rifie que la ville n'est pas marqu�e
                Distance.push_back(make_pair(data.distances[lastville][i], i)); //On cr�e un vecteur contenant toutes les distances restantes
            }
        }
        std::sort(Distance.begin(), Distance.end()); //Tri du vecteur des distances par ordre croissant

        //Si il ne reste plus qu'une ville
        if (Distance.size() == 1) {
            //On enregistre la ville en donn�es
            //cout << "Il ne reste plus qu'une ville : " << data.city_names[Distance[0].second] << " " << Distance[0].second << endl;
            finish = 1; //Fin des boucles
            Solution.push_back(Distance[0].second);
            lst[Distance[0].second] = 1;
            total_distance += Distance[0].first;
            //cout << "Distance : " << Distance[0].first << endl;
            lastville = Distance[0].second;
        }
        else {
            //Choix al�atoire entre les 2 villes les plus proches
            std::uniform_int_distribution<int> distribution(0, 1);
            std::default_random_engine generator(seed);
            auto choosen = distribution(generator);

            //Ajout de la soluton dans la liste
            Solution.push_back(Distance[choosen].second);
            lst[Distance[choosen].second] = 1;
            total_distance += Distance[choosen].first;
            lastville = Distance[choosen].second;

            //Partie Affichage et d�boggage
            //cout << "Distance : " << Distance[choosen].first << endl;
            //cout << "On choisit entre : " << data.city_names[Distance[0].second] << " " << Distance[0].second << " et " << data.city_names[Distance[1].second] << " " << Distance[1].second << endl;
           //cout << "La ville choisie al�atoirement est : " << data.city_names[Distance[choosen].second] << " " << Distance[choosen].second << endl;
        }

    }
    //cout << endl << "Chemin :" << endl;

    //On ajoute la premi�re ville � la fin car c'est un cycle
    total_distance += data.distances[Solution[Solution.size() - 1]][Solution[0]];

    ////Affichage du chemin solution
    //for (int i = 0; i < Solution.size(); ++i) {
    //    cout << data.city_names[Solution[i]] << " -> ";
    //}
    //cout << data.city_names[Solution[0]] << endl;

    if ((total_distance < MeilleureSolution.distance) || (MeilleureSolution.distance == -1)) {
        MeilleureSolution.chemin = {};
        for (int i = 0; i < Solution.size(); ++i) {
            MeilleureSolution.chemin.push_back(data.city_names[Solution[i]]);
        }
        MeilleureSolution.distance = total_distance;
    }
    return total_distance;
}

// Impl�mentation de la m�thode du Brut Force pour le probl�me P2
int BrutForceP2(P2Data& data) {
    // Initialisation des variables
    int n = data.num_cities;
    vector<int> perm(n); // Vecteur pour stocker la permutation courante
    for (int i = 0; i < n; i++) {
        perm[i] = i; // Initialisation de la permutation � l'ordre naturel
    }
    int best_distance = numeric_limits<int>::max(); // Initialisation de la meilleure distance � la plus grande valeur possible
        // Boucle sur toutes les permutations possibles
        do {
            int distance = 0; // Initialisation de la distance pour cette permutation
            for (int i = 0; i < n; i++) {
                distance += data.distances[perm[i]][perm[(i + 1) % n]]; // Calcul de la distance totale pour cette permutation
            }
            if (distance < best_distance) { // Si cette permutation donne une distance plus courte que la meilleure connue jusqu'� pr�sent
                best_distance = distance; // Mettre � jour la meilleure distance
                MeilleureSolution.chemin.clear(); // R�initialise la meilleure solution courante
                for (int i = 0; i < n; i++) {
                    MeilleureSolution.chemin.push_back(data.city_names[perm[i]]); // Stocke le chemin correspondant � la meilleure permutation
                }
                MeilleureSolution.distance = best_distance; // Mettre � jour la meilleure solution connue
            }
        } while (next_permutation(perm.begin(), perm.end())); // Calcule la prochaine permutation possible

        return best_distance; // Retourne la distance correspondant � la meilleure solution trouv�e
}

int main(int argc, char* argv[]) {

    setlocale(LC_ALL, "FR_fr");

    // V�rifier si les noms de fichiers ont �t� fournis en tant que param�tres
    if (argc !=2) {
        cerr << "un nom de fichier doivent �tre fournis en tant que param�tres." << endl;
        return 1;
    }

    unsigned int seed;
    vector<int> p1result;
    int p2result;
    P2Data p2_data = readP2File(argv[1]);

    for (int i = 1; i < 5; i++) {

        seed = i; // valeur de la graine 
        std::default_random_engine generator(seed); // initialisation de la graine

        //cout << "\nNumero de la replication (graine) : " << seed << endl;

        //// R�solution du probl�me P1
        //cout << "\nProbleme P2 :" << endl;
        ////cout << "\nDonn�es :" << endl;
        //cout << "Chemin trouve : " << endl;
        p2result = solveP2(p2_data, seed);/*
        cout << "Distance : " << p2result << endl;
        cout << endl << "Chemin optimal : ";
        for (const auto& city : p2_data.city_names) {
            cout << city << " -> ";
        }
        cout << p2_data.city_names[0] << "\n\n" << endl;*/
        //cout << "\n\n" << endl;*/
    }
    cout << "-----------------------------------------\n\n" << endl;


    cout << "\n\nMeilleure Solution du probl�me P2 avec m�thode de l'heuristique :" << endl;

    cout << endl;
    cout << "Chemin : ";
    for (const auto& city : MeilleureSolution.chemin) {
        cout << city << " -> ";
    }
    cout << MeilleureSolution.chemin[0] << endl;
    cout << "Distance : " << MeilleureSolution.distance << endl;
    auto distance_heuristique = MeilleureSolution.distance;

    cout << "\n\n-----------------------------------------" << endl;

    p2result = BrutForceP2(p2_data);

    cout << "\n\nMeilleure Solution du probl�me P2 avec BrutForce :" << endl;

    cout << endl;
    cout << "Chemin : ";
    for (const auto& city : MeilleureSolution.chemin) {
        cout << city << " -> ";
    }
    cout << MeilleureSolution.chemin[0] << endl;
    cout << "Distance : " << MeilleureSolution.distance << endl;
    auto distance_brut_force = MeilleureSolution.distance;

    cout << "\n\n-----------------------------------------" << endl;
    // Calcul du pourcentage de diff�rence entre la solution optimale (BrutForce) et la solution trouv�e par l'heuristique (solveP2)
    float pourcentage_difference = 100.0 * (float)(distance_heuristique - distance_brut_force) / (float)distance_heuristique;

    cout << "\nPourcentage de diff�rence entre la solution optimale (BrutForce) et l'heuristique (solveP2) : " << fixed << setprecision(2) << pourcentage_difference << "%" << endl;
    
    cout << "\n\n-----------------------------------------" << endl;
    return 0;
}




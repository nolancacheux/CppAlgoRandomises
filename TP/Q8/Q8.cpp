#include <algorithm> // Pour la fonction de tri std::sort()
#include <limits> // Pour la valeur numérique maximale numeric_limits<int>::max()
#include <vector> // Pour utiliser des vecteurs
#include <tuple> // Pour stocker les données de P1 dans un tuple
#include <string> // Pour stocker les noms de villes
#include <iostream> // Pour afficher les résultats intermédiaires
#include <fstream>
using namespace std;

// Structure de données pour les données du problème P1
struct P1Data {
    int capacity; // Capacité du sac à dos
    int num_objects; // Nombre d'objets disponibles
    vector<tuple<int, int, int>> objects; // (nom/index, consommation, bénéfice)
};

// Structure de données pour les données du problème P2
struct P2Data {
    int num_cities; // Nombre de villes
    vector<string> city_names; // Noms des villes
    vector<vector<int>> distances; // Matrice des distances
};

// Fonction pour lire les données du problème P1 à partir d'un fichier
P1Data readP1File(std::string filename) {
    std::ifstream infile(filename);
    P1Data data;
    if (infile.is_open()) {
        infile >> data.capacity >> data.num_objects;
        int index, consumption, profit;
        for (int i = 0; i < data.num_objects; i++) {
            infile >> index >> consumption >> profit;
            data.objects.push_back(std::make_tuple(index, consumption, profit));
        }
        return data;
    }
    else {
        cerr << "Le fichier P1 ne peut pas être ouvert" << endl;
        exit(1);
    }
}

// Fonction pour lire les données du problème P2 à partir d'un fichier
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
        cerr << "Le fichier P2 ne peut pas être ouvert" << endl;
        exit(1);
    }
}

// Fonction pour afficher les données du problème P1
void printP1Data(P1Data data) {
    std::cout << "Capacite du sac a dos : " << data.capacity << std::endl;
    std::cout << "Nombre d'objets disponibles : " << data.num_objects << std::endl;
    for (int i = 0; i < data.num_objects; i++) {
        int index, consumption, profit;
        std::tie(index, consumption, profit) = data.objects[i];
        std::cout << "Objet " << index << " : consommation = " << consumption << ", bénéfice = " << profit << std::endl;
    }
}

// Fonction pour afficher les données du problème P2
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

// Fonction pour résoudre le problème P1 avec un algorithme glouton
vector<int> solveP1(P1Data& data) {

    auto copie_data = data; //On fait une copie pour garder la structure non triée de data afin de ne pas nous perdre lors de l'affichage des indices

    // Tri des objets en fonction du ratio bénéfice/consommation décroissant
    sort(data.objects.begin(), data.objects.end(), [](auto& o1, auto& o2) {
        double ratio1 = (double)std::get<2>(o1) / (double)std::get<1>(o1);
    double ratio2 = (double)std::get<2>(o2) / (double)std::get<1>(o2);
    return ratio1 > ratio2;
        });
    // Variables pour le suivi de la solution
    int total_profit = 0;
    int total_consumption = 0;
    vector<int> solution(data.num_objects, 0);
    // Boucle sur les objets triés
    for (int i = 0; i < data.num_objects; i++) {
        // Vérification si l'objet peut être inclus dans la solution
        if (total_consumption + std::get<1>(data.objects[i]) <= data.capacity) {
            solution[std::get<0>(data.objects[i]) - 1] = 1; // Marquage de l'objet inclus
            total_profit += std::get<2>(data.objects[i]);
            total_consumption += std::get<1>(data.objects[i]);
        }
    }

    // Affichage de la solution
    std::cout << "\nBenefice total : " << total_profit << std::endl;
    std::cout << "Energie consommée : " << total_consumption << std::endl;
    std::cout << "Composition de la solution :" << std::endl;
    for (int i = 0; i < data.num_objects; i++) {
        if (solution[i] == 1) {
            int index, consumption, profit;
            std::tie(index, consumption, profit) = copie_data.objects[i];
            std::cout << "Objet " << index << " inclus" << std::endl;
        }
    }
    return solution;
}
// Fonction pour calculer la distance entre deux villes
int distanceBetweenCities(const P2Data& data, int city1, int city2) {
    return data.distances[city1][city2];
}

// Fonction pour trouver la ville la plus proche de la dernière ville ajoutée à chaque itération dans le problème P2
int findNearestCity(const P2Data& data, const vector<int>& visited_cities, int last_city) {
    int nearest_city = -1;
    int min_distance = numeric_limits<int>::max(); // initialiser avec la plus grande valeur possible
    for (int i = 0; i < data.num_cities; i++) {
        // Vérifier si la ville a déjà été visitée
        if (find(visited_cities.begin(), visited_cities.end(), i) == visited_cities.end()) {
            int distance = distanceBetweenCities(data, last_city, i);
            // Vérifier si la ville est plus proche que la ville actuellement la plus proche
            if (distance < min_distance) {
                min_distance = distance;
                nearest_city = i;
            }
        }
    }
    return nearest_city;
}


// Fonction pour résoudre le problème P2 avec un algorithme glouton
int solveP2(const P2Data& data) {
    int total_distance = 0;
    int current_city = 0; // On commence à la première ville
    vector<int> visited_cities(data.num_cities, 0); // Initialisation à 0
    visited_cities[current_city] = 1; // Marquer la première ville comme visitée
    for (int i = 0; i < data.num_cities - 1; i++) {
        int next_city = -1;
        int shortest_distance = numeric_limits<int>::max();
        for (int j = 0; j < data.num_cities; j++) {
            if (visited_cities[j] == 0 && distanceBetweenCities(data, current_city, j) < shortest_distance) {
                next_city = j;
                shortest_distance = distanceBetweenCities(data, current_city, j);
            }
        }
        current_city = next_city;
        visited_cities[current_city] = 1;
        total_distance += shortest_distance;
    }
    total_distance += distanceBetweenCities(data, current_city, 0); // Retour à la première ville
    return total_distance;
}



int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "FR_fr");
    // Vérifier si les noms de fichiers ont été fournis en tant que paramètres
    if (argc != 4) {
        cerr << "Deux noms de fichiers doivent être fournis en tant que parametres." << endl;
        return 1;
    }
    auto graine = strtol(argv[1], NULL, 10);
    srand(graine);
    cout << "Solution de la graine numero " << graine  << " :\n"<< endl;
    // Résolution du problème P1
    P1Data p1_data = readP1File(argv[2]);
    cout << "\nProblème P1 :" << endl;
    auto p1result = solveP1(p1_data);
   
    // Résolution du problème P2
    P2Data p2_data = readP2File(argv[3]);
    cout << "\nProblème P2 :" << endl;
    int p2result = solveP2(p2_data);
    cout << "Distance minimale : " << p2result << endl;
    cout << "Chemin optimal : ";
    for (const auto& city : p2_data.city_names) {
        cout << city << " -> ";
    }
    cout << p2_data.city_names[0] << "\n\n" << endl;
    return 0;
}



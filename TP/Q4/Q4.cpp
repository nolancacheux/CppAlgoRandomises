#include <algorithm> // Pour la fonction de tri std::sort()
#include <limits> // Pour la valeur num�rique maximale numeric_limits<int>::max()
#include <vector> // Pour utiliser des vecteurs
#include <tuple> // Pour stocker les donn�es de P1 dans un tuple
#include <string> // Pour stocker les noms de villes
#include <iostream> // Pour afficher les r�sultats interm�diaires
#include <fstream>
#include <random>
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

// Fonction pour lire les donn�es du probl�me P1 � partir d'un fichier
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
        cerr << "Le fichier P1 ne peut pas �tre ouvert" << endl;
        exit(1);
    }
}

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

// Fonction pour afficher les donn�es du probl�me P1
void printP1Data(P1Data data) {
    std::cout << "Capacit� du sac � dos : " << data.capacity << std::endl;
    std::cout << "Nombre d'objets disponibles : " << data.num_objects << std::endl;
    for (int i = 0; i < data.num_objects; i++) {
        int index, consumption, profit;
        std::tie(index, consumption, profit) = data.objects[i];
        std::cout << "Objet " << index << " : consommation = " << consumption << ", b�n�fice = " << profit << std::endl;
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
vector<int> solveP1(P1Data& data) {

    auto copie_data = data; //On fait une copie pour garder la structure non tri�e de data afin de ne pas nous perdre lors de l'affichage des indices

    // Tri des objets en fonction du ratio b�n�fice/consommation d�croissant
    sort(data.objects.begin(), data.objects.end(), [](auto& o1, auto& o2) {
        double ratio1 = (double)std::get<2>(o1) / (double)std::get<1>(o1);
    double ratio2 = (double)std::get<2>(o2) / (double)std::get<1>(o2);
    return ratio1 > ratio2;
        });
    // Variables pour le suivi de la solution
    int total_profit = 0;
    int total_consumption = 0;
    vector<int> solution(data.num_objects, 0);
    std::cout << "\nOn affiche les objets selon le meilleur rapport �b�n�fice/capacit� consomm�e : " << std::endl;
    printP1Data(data);
    // Boucle sur les objets tri�s
    for (int i = 0; i < data.num_objects; i++) {
        // V�rification si l'objet peut �tre inclus dans la solution
        if (total_consumption + std::get<1>(data.objects[i]) <= data.capacity) {
            solution[std::get<0>(data.objects[i]) - 1] = 1; // Marquage de l'objet inclus
            total_profit += std::get<2>(data.objects[i]);
            total_consumption += std::get<1>(data.objects[i]);
        }
    }

    // Affichage de la solution
    std::cout << "\nB�n�fice total : " << total_profit << std::endl;
    std::cout << "�nergie consomm�e : " << total_consumption << std::endl;
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

// Fonction pour trouver la ville la plus proche de la derni�re ville ajout�e � chaque it�ration dans le probl�me P2
int findNearestCity(const P2Data& data, const vector<int>& visited_cities, int last_city) {
    int nearest_city = -1;
    int min_distance = numeric_limits<int>::max(); // initialiser avec la plus grande valeur possible
    for (int i = 0; i < data.num_cities; i++) {
        // V�rifier si la ville a d�j� �t� visit�e
        if (find(visited_cities.begin(), visited_cities.end(), i) == visited_cities.end()) {
            int distance = distanceBetweenCities(data, last_city, i);
            // V�rifier si la ville est plus proche que la ville actuellement la plus proche
            if (distance < min_distance) {
                min_distance = distance;
                nearest_city = i;
            }
        }
    }
    return nearest_city;
}


int solveP2(P2Data& data) {
    vector<int> lst(data.num_cities, 0);
    int total_distance = 0;
    int finish = 0;

    //Choix al�atoire de la ville intiale
    std::default_random_engine generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, lst.size() - 1);
    auto lastville = distribution(generator);
    vector<int> Solution = { lastville };
    lst[lastville] = 1; //On marque la ville choisie pour ne pas la r�utiliser

    cout << "ville de d�part : " << data.city_names[lastville] << " " << lastville << endl;

    //Calcul de la meilleure ville
    while (finish == 0) {

        vector<pair<int, int>> Distance;
        //Calcul des distances entre la derni�re ville et toutes les autres villes
        for (int i = 0; i < lst.size(); ++i) {
            if (lst[i] == 0) { //On v�rifie que la ville n'est pas marqu�e
                Distance.push_back(make_pair(data.distances[lastville][i], i)); //On cr�e un vecteur contenant toutes les distances restantes
            }
        }
        std::sort(Distance.begin(), Distance.end()); //Tri du vecteur des distances par ordre croissant

        //On enregistre la ville en donn�es
        Solution.push_back(Distance[0].second);
        lst[Distance[0].second] = 1;
        total_distance += Distance[0].first;
        lastville = Distance[0].second;

        //S'il ne reste plus qu'une ville
        if (Distance.size() == 1) {
            finish = 1; //Fin des boucles
        }
    }

    cout << endl << "Chemin :" << endl;

    //On ajoute la premi�re ville � la fin car c'est un cycle
    total_distance += data.distances[Solution[Solution.size() - 1]][Solution[0]];

    //Affichage du chemin solution
    for (int i = 0; i < Solution.size(); ++i) {
        cout << data.city_names[Solution[i]] << " -> ";
    }
    cout << data.city_names[Solution[0]] << endl;
    return total_distance;
}



int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "FR_fr");
    // V�rifier si les noms de fichiers ont �t� fournis en tant que param�tres
    if (argc != 3) {
        cerr << "Deux noms de fichiers doivent �tre fournis en tant que param�tres." << endl;
        return 1;
    }

    // R�solution du probl�me P1
    P1Data p1_data = readP1File(argv[1]);
    cout << "\nProbl�me P1 :" << endl;
    cout << "\nDonn�es :" << endl;
    printP1Data(p1_data);
    auto p1result = solveP1(p1_data);
    std::cout << "On remarque que cela n'est pas LA MEILLEURE solution car il aurait fallu prendre les objets 1, 4 et 5";
    // R�solution du probl�me P2
    P2Data p2_data = readP2File(argv[2]);
    cout << "\nProbl�me P2 :" << endl;
    cout << "\nDonn�es :" << endl;
    printP2Data(p2_data);
    int p2result = solveP2(p2_data);
    cout << "R�sultat :\n" << endl;
    cout << "Distance minimale : " << p2result << endl;
    cout << "Chemin optimal : ";
    for (const auto& city : p2_data.city_names) {
        cout << city << " -> ";
    }
    cout << p2_data.city_names[0] << "\n\n" << endl;
    return 0;
}



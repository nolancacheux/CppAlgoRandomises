
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


struct Solution {
    int benefice;
    int energie;
    vector<int> objets_inclus;
    int resultat_pondere;
    vector<string> chemin;
    int distance;
};

Solution MeilleureSolution = { 0, 0, {}, 0, {}, -1 };

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
vector<int> solveP1(P1Data& data, int seed) {
    auto copie_data = data;
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

    //std::cout << "\nOn affiche les objets selon le meilleur rapport �b�n�fice/capacit� consomm�e : " << std::endl;
    //printP1Data(data);

    // Boucle sur les objets tri�s
    for (int i = 0; i < data.num_objects; i++) {
        // V�rification si l'objet peut �tre inclus dans la solution
        if ((total_consumption + std::get<1>(data.objects[i]) <= data.capacity) && ((solution[std::get<0>(data.objects[i]) - 1]) != 1)) {
            // Les deux meilleurs candidats
            auto candidate1 = data.objects[i];
            auto candidate2 = std::make_tuple(0, 0, 0);
            //std::cout << "best candidate : object in pos" << i << endl;
            //std::cout << "boucle sur les objets: de pos" << i + 1 << " � " << data.num_objects;
            for (int j = i + 1; j < data.num_objects; j++) {
                //std::cout << "enter" << j;
                if ((total_consumption + std::get<1>(data.objects[j]) <= data.capacity) && ((solution[std::get<0>(data.objects[j]) - 1]) != 1)) {
                    candidate2 = data.objects[j];
                    break;
                }
            }

            // std::cout << endl << endl << total_consumption << " / " << data.capacity;
             //std::cout << "\nOn CHOISIt d�sormais al�atoirement entre les deux meilleurs candidats d'objets : " << std::endl;

             //std::cout << get<0>(candidate1) << " et " << get<0>(candidate2) << endl;

            tuple<int, int, int> chosen_candidate;
            if (get<0>(candidate2) == 0) {
                chosen_candidate = candidate1;
            }
            else {
                // Choix al�atoire entre les deux candidats
                std::default_random_engine generator(seed);
                std::uniform_int_distribution<int> distribution(1, 2);
                auto random_choice = distribution(generator);
                chosen_candidate = (random_choice == 1) ? candidate1 : candidate2;
            }


            //std::cout << "Objet choisit : " << get<0>(chosen_candidate) << endl;

            // Mise � jour de la solution
            solution[std::get<0>(chosen_candidate) - 1] = 1; // Marquage de l'objet inclus
            total_profit += std::get<2>(chosen_candidate);
            total_consumption += std::get<1>(chosen_candidate);

        }
    }

    // Affichage de la solution
    std::cout << "\nBenefice total : " << total_profit << std::endl;
    std::cout << "Energie consommee : " << total_consumption << std::endl;
    std::cout << "Composition de la solution :" << std::endl;
    for (int i = 0; i < data.num_objects; i++) {
        if (solution[i] == 1) {
            int index, consumption, profit;
            std::tie(index, consumption, profit) = copie_data.objects[i];
            std::cout << "Objet " << index << " inclus" << std::endl;
        }
    }
    std::cout << "\nAvec coefficient de ponderation de 0.5 pour le benefice et de 0.5 pour l'energie consommee :" << std::endl;
    int pondere = static_cast<int>(round((0.5 * total_profit) + (0.5 * total_consumption)));
    std::cout << "Resultat pondere : " << pondere << std::endl;

    if (pondere > MeilleureSolution.resultat_pondere) {
        MeilleureSolution.resultat_pondere = pondere;
        MeilleureSolution.benefice = total_profit;
        MeilleureSolution.energie = total_consumption;
        MeilleureSolution.objets_inclus = {};
        for (int i = 0; i < data.num_objects; i++) {
            if (solution[i] == 1) {
                int index, consumption, profit;
                std::tie(index, consumption, profit) = copie_data.objects[i];
                MeilleureSolution.objets_inclus.push_back(index);
            }
        }

    }

    return solution;

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

    //Affichage du chemin solution
    for (int i = 0; i < Solution.size(); ++i) {
        cout << data.city_names[Solution[i]] << " -> ";
    }
    cout << data.city_names[Solution[0]] << endl;

    if ((total_distance < MeilleureSolution.distance) || (MeilleureSolution.distance == -1)) {
        MeilleureSolution.chemin = {};
        for (int i = 0; i < Solution.size(); ++i) {
            MeilleureSolution.chemin.push_back(data.city_names[Solution[i]]);
        }
        MeilleureSolution.distance = total_distance;
    }
    return total_distance;
}



int main(int argc, char* argv[]) {

    setlocale(LC_ALL, "FR_fr");

    // V�rifier si les noms de fichiers ont �t� fournis en tant que param�tres
    if (argc != 3) {
        cerr << "Deux noms de fichiers doivent �tre fournis en tant que param�tres." << endl;
        return 1;
    }

    unsigned int seed;
    vector<int> p1result;
    int p2result;

    for (int i = 1; i < 5; i++) {

        seed = i; // valeur de la graine 
        std::default_random_engine generator(seed); // initialisation de la graine

        cout << "\nNumero de la replication (graine) : " << seed << endl;

        // R�solution du probl�me P1
        P1Data p1_data = readP1File(argv[1]);
        cout << "\nProbleme P1 :" << endl;
        //cout << "\nDonnees :" << endl;
        p1result = solveP1(p1_data, seed);
        // R�solution du probl�me P2
        P2Data p2_data = readP2File(argv[2]);
        cout << "\nProbleme P2 :" << endl;
        //cout << "\nDonn�es :" << endl;
        cout << "Chemin trouve : " << endl;
        p2result = solveP2(p2_data, seed);
        cout << "Distance : " << p2result << endl;/*
        cout << endl << "Chemin optimal : ";
        for (const auto& city : p2_data.city_names) {
            cout << city << " -> ";
        }
        cout << p2_data.city_names[0] << "\n\n" << endl;*/
        cout << "\n\n" << endl;
    }
    cout << "-----------------------------------------\n\n" << endl;
    // Affichage des �l�ments de MeilleureSolution
    cout << "Meilleure Solution du probl�me P1:\n" << endl;
    cout << "B�n�fice : " << MeilleureSolution.benefice << endl;
    cout << "�nergie : " << MeilleureSolution.energie << endl;
    cout << "Objets inclus :\n";
    for (auto objet : MeilleureSolution.objets_inclus) {
        std::cout << "Objet " << objet << " inclus" << std::endl;
    }
    cout << "R�sultat pond�r� : " << MeilleureSolution.resultat_pondere << endl;

    cout << "\n\nMeilleure Solution du probl�me P2:" << endl;

    cout << endl;
    cout << "Chemin : ";
    for (const auto& city : MeilleureSolution.chemin) {
        cout << city << " -> ";
    }
    cout << MeilleureSolution.chemin[0] << endl;
    cout << "Distance : " << MeilleureSolution.distance << endl;

    cout << "\n\n-----------------------------------------" << endl;


    return 0;
}




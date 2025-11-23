#include <bits/stdc++.h>
using namespace std;

#define FOOD_NUMBER 20   // S? ngu?n th?c an (t?c s? ong th?)
#define D 4              // S? chi?u nghi?m (có th? thay d?i)
#define LOWER -5        // Gi?i h?n du?i mi?n tìm ki?m
#define UPPER 5         // Gi?i h?n trên mi?n tìm ki?m
#define LIMIT 30         // Ngu?ng trial d? chuy?n thành ong do thám
#define MAXCYCLE 2000    // S? vòng l?p t?i da

// C?u trúc luu thông tin 1 ngu?n th?c an
struct FoodSource {
    vector<double> x;   // V? trí nghi?m (vector D chi?u)
    double f;           // Giá tr? hàm m?c tiêu t?i nghi?m
    double fitness;     // Giá tr? fitness du?c chuy?n d?i t? f
    int trial;          // S? l?n c?i thi?n th?t b?i liên ti?p

    FoodSource() {
        x.assign(D, 0.0);
        f = 0;
        fitness = 0;
        trial = 0;
    }
};

FoodSource foods[FOOD_NUMBER];  // Danh sách ngu?n th?c an
FoodSource bestSolution;        // Luu nghi?m t?t nh?t tìm du?c

double objectiveFunction(const vector<double>& x) {
    double x1 = x[0];
    double x2 = x[1];
    double x3 = x[2];
    double x4 = x[3];
    return pow(x1,2) + pow(x2,2) + pow(x3, 3) * pow(x4, 3) + 5;
}

double calculateFitness(double f) {
    if (f >= 0) return 1 / (1 + f); 
    return 1 + fabs(f);
}

// KH?I T?O NG?U NHIÊN QU?N TH?
void initialize() {
    for (int i = 0; i < FOOD_NUMBER; i++) {
        for (int j = 0; j < D; j++) {
            foods[i].x[j] = LOWER + (double)rand() / RAND_MAX * (UPPER - LOWER);
        }

        foods[i].f = objectiveFunction(foods[i].x);
        foods[i].fitness = calculateFitness(foods[i].f);
        foods[i].trial = 0;

        if (i == 0 || foods[i].fitness > bestSolution.fitness) {
            bestSolution = foods[i];
        }
    }
}

// PHA ONG TH? (Employed Bee Phase)
void employedBeePhase() {
    for (int i = 0; i < FOOD_NUMBER; i++) {
        FoodSource newFood = foods[i]; 
        int j = rand() % D;
		int k;            
        do { k = rand() % FOOD_NUMBER; } while (k == i);
        double phi = ((double)rand() / RAND_MAX) * 2 - 1;
        newFood.x[j] = foods[i].x[j] + phi * (foods[i].x[j] - foods[k].x[j]);
        newFood.x[j] = max((double)LOWER, min((double)UPPER, newFood.x[j]));
        newFood.f = objectiveFunction(newFood.x);
        newFood.fitness = calculateFitness(newFood.f);
        if (newFood.fitness > foods[i].fitness) {
            foods[i] = newFood;
            foods[i].trial = 0;
        } else {
            foods[i].trial++; 
        }
    }
}

// PHA ONG QUAN SÁT (Onlooker Bee Phase)
// Ch?n ngu?n theo roulette wheel
void onlookerBeePhase() {
    double sumFit = 0;
    for (int i = 0; i < FOOD_NUMBER; i++) sumFit += foods[i].fitness;

    int t = 0;
    while (t < FOOD_NUMBER) {

        double r = ((double)rand() / RAND_MAX) * sumFit;
        double accum = 0;
        int i;
		for (i = 0; i < FOOD_NUMBER; i++) {
            accum += foods[i].fitness;
            if (accum >= r) break;
        }

        FoodSource newFood = foods[i];
        int j = rand() % D;

        int k;
        do { k = rand() % FOOD_NUMBER; } while (k == i);

        double phi = ((double)rand() / RAND_MAX) * 2 - 1;

        newFood.x[j] = foods[i].x[j] + phi * (foods[i].x[j] - foods[k].x[j]);
        newFood.x[j] = max((double)LOWER, min((double)UPPER, newFood.x[j]));

        newFood.f = objectiveFunction(newFood.x);
        newFood.fitness = calculateFitness(newFood.f);

        if (newFood.fitness > foods[i].fitness) {
            foods[i] = newFood;
            foods[i].trial = 0;
        } else {
            foods[i].trial++;
        }

        t++;
    }
}

// PHA ONG DO THÁM (Scout Bee Phase)
void scoutBeePhase() {
    for (int i = 0; i < FOOD_NUMBER; i++) {
        if (foods[i].trial > LIMIT) {

            for (int j = 0; j < D; j++) {
                foods[i].x[j] = LOWER + (double)rand() / RAND_MAX * (UPPER - LOWER);
            }

            foods[i].f = objectiveFunction(foods[i].x);
            foods[i].fitness = calculateFitness(foods[i].f);
            foods[i].trial = 0;
        }
    }
}

void memorizeBest() {
    for (int i = 0; i < FOOD_NUMBER; i++) {
        if (foods[i].fitness > bestSolution.fitness) {
            bestSolution = foods[i];
        }
    }
}

int main() {
    srand(time(NULL));
    initialize(); 
    for (int cycle = 0; cycle < MAXCYCLE; cycle++) {
        employedBeePhase();
        onlookerBeePhase();
        scoutBeePhase();
        memorizeBest();
    }
    cout << "Best solution found:\n";
    cout << "x = ";
    for (double v : bestSolution.x) cout << v << " ";
    cout << "\nf(min) = " << bestSolution.f << endl;

    return 0;
}

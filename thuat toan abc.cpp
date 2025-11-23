#include <bits/stdc++.h>
using namespace std;

#define FOOD_NUMBER 20   // Số nguồn thức ăn (tức số ong thợ)
#define D 4              // Số chiều nghiệm (có thể thay đổi)
#define LOWER -10        // Giới hạn dưới miền tìm kiếm
#define UPPER 10         // Giới hạn trên miền tìm kiếm
#define LIMIT 30         // Ngưỡng trial để chuyển thành ong do thám
#define MAXCYCLE 2000    // Số vòng lặp tối đa

// Cấu trúc lưu thông tin 1 nguồn thức ăn
struct FoodSource {
    vector<double> x;   // Vị trí nghiệm (vector D chiều)
    double f;           // Giá trị hàm mục tiêu tại nghiệm
    double fitness;     // Giá trị fitness được chuyển đổi từ f
    int trial;          // Số lần cải thiện thất bại liên tiếp

    FoodSource() {
        x.assign(D, 0.0);
        f = 0;
        fitness = 0;
        trial = 0;
    }
};

FoodSource foods[FOOD_NUMBER];  // Danh sách nguồn thức ăn
FoodSource bestSolution;        // Lưu nghiệm tốt nhất tìm được

/// Hàm mục tiêu ví dụ (có thể thay đổi tùy bài toán) Sphere function f(x) = sum(x_i^2)
double objectiveFunction(const vector<double>& x) {
    double sum = 0;
    for (double v : x) sum += v * v;
    return sum;
}

double calculateFitness(double f) {
    if (f >= 0) return 1 / (1 + f); 
    return 1 + fabs(f);
}

// KHỞI TẠO NGẪU NHIÊN QUẦN THỂ
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

// PHA ONG THỢ (Employed Bee Phase)
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
// Chọn nguồn theo roulette wheel
void onlookerBeePhase() {
    double sumFit = 0;
    for (int i = 0; i < FOOD_NUMBER; i++) sumFit += foods[i].fitness;

    int t = 0;
    while (t < FOOD_NUMBER) {

        double r = ((double)rand() / RAND_MAX) * sumFit;
        double accum = 0;
        int i;
        
        for (int i = 0; i < FOOD_NUMBER; i++) {
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

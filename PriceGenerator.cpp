#include"PriceGenerator.h"

double PriceGenerator(double price, double std) {

    static std::mt19937 gen(std::random_device{}());

    std::normal_distribution<double> change(0.2, std);

    return price + change(gen);
}

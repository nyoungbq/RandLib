#ifndef UNIFORMDISCRETERAND_H
#define UNIFORMDISCRETERAND_H

#include "DiscreteRand.h"
#include "../BasicRandGenerator.h"

/**
 * @brief The UniformDiscreteRand class
 */
class RANDLIBSHARED_EXPORT UniformDiscreteRand : public DiscreteRand<int>
{
    int n, a, b;
    double nInv; // 1 / n

    BasicRandGenerator B;

    void swapBoundaries();
public:
    UniformDiscreteRand(int minValue = 0, int maxValue = 1);

    void setBoundaries(int minValue, int maxValue);
    inline int getA() { return a; }
    inline int getB() { return b; }

    virtual double P(int k) const override;
    virtual double F(double x) const override;
    virtual int value() override;

    double E() const override { return .5 * (b + a); }
    double Var() const override {
        double n2 = n * n;
        return (n2 * n2 - 1) / 12;
    }

    inline double Median() const { return .5 * (b + a); }
    static constexpr double Skewness() { return 0; }
    inline double ExcessKurtosis() {
        double n2 = n * n;
        return -1.2 * (n2 + 1) / (n2 - 1);
    }

    inline double Entropy() { return std::log(n); }
};

#endif // UNIFORMDISCRETERAND_H

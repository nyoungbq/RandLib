#include "BinomialRand.h"

BinomialRand::BinomialRand(int number, double probability)
{
    setNumber(number);
    setProbability(probability);
}

void BinomialRand::setName()
{
    nameStr = "Binomial(" + toStringWithPrecision(getNumber()) + ", " + toStringWithPrecision(getProbability()) + ")";
}

void BinomialRand::setNumber(int number)
{
    n = std::max(number, 1);
}

void BinomialRand::setProbability(double probability)
{
    p = std::min(std::max(probability, MIN_POSITIVE), 1.0);
    B.setProbability(p);

    setName();
}

double BinomialRand::P(int k) const
{
    if (k < 0 || k > n)
        return 0;
    return RandMath::binomialCoef(n, k) * std::pow(p, k) * std::pow(1 - p, n - k);
}

double BinomialRand::F(double x) const
{
    return x;
}

double BinomialRand::variate() const
{
    unsigned sum = 0;
    for (int i = 0; i != n; ++i)
        sum += B.variate();
    return sum;
}

#include "PlanckRand.h"

PlanckRand::PlanckRand(double shape, double scale)
{
    SetParameters(shape, scale);
}

String PlanckRand::Name() const
{
    return "Planck(" + toStringWithPrecision(GetShape()) + ", " + toStringWithPrecision(GetScale()) + ")";
}

void PlanckRand::SetParameters(double shape, double scale)
{
    if (shape <= 0.0)
        throw std::invalid_argument("Planck distribution: shape should be positive");
    if (scale <= 0.0)
        throw std::invalid_argument("Planck distribution: scale should be positive");
    a = shape;
    b = scale;

    double ap1 = a + 1;
    Z.SetExponent(ap1);
    G.SetParameters(ap1, b);

    pdfCoef = -Z.GetLogZetaFunction();
    pdfCoef += ap1 * std::log(b);
    pdfCoef -= G.GetLogGammaShape();
}

double PlanckRand::h(double t) const
{
    if (t <= 0)
        return 0.0;
    double y = pdfCoef + a * std::log(t);
    double expY = std::exp(y);
    double bt = b * t;
    double z = 1.0 / std::expm1l(bt) - 1.0 / bt;
    return expY * z;
}

double PlanckRand::f(const double & x) const
{
    if (x < 0)
        return 0;
    if (x == 0)
    {
        if (a > 1)
            return 0.0;
        if (a == 1)
            return std::exp(pdfCoef) / b;
        return INFINITY;
    }
    double y = pdfCoef + a * std::log(x);
    return std::exp(y) / std::expm1l(b * x);
}

double PlanckRand::logf(const double & x) const
{
    if (x < 0.0)
        return -INFINITY;
    if (x == 0)
    {
        if (a > 1)
            return -INFINITY;
        if (a == 1)
            return pdfCoef - G.GetLogRate();
        return INFINITY;
    }
    double y = pdfCoef + a * std::log(x);
    return y - RandMath::logexpm1l(b * x);
}

double PlanckRand::F(const double & x) const
{
    if (x <= 0)
        return 0.0;

    if (a >= 1) {
        return RandMath::integral([this] (double t)
        {
            return f(t);
        },
        0, x);
    }

    /// split F(x) by two integrals
    double aux = pdfCoef + a * std::log(x);
    double integral1 = std::exp(aux) / (b * a);
    double integral2 = RandMath::integral([this] (double t)
    {
        return h(t);
    },
    0, x);
    return integral1 + integral2;
}

double PlanckRand::Variate() const
{
    return G.Variate() / Z.Variate();
}

void PlanckRand::Sample(std::vector<double> &outputData) const
{
    G.Sample(outputData);
    for (double & var : outputData)
        var /= Z.Variate();
}

long double PlanckRand::Mean() const
{
    double y = (a + 1) / b;
    y *= std::riemann_zetal(a + 2);
    return y / Z.GetZetaFunction();
}

long double PlanckRand::SecondMoment() const
{
    long double secondMoment = (a + 1) * (a + 2);
    secondMoment /= (b * b);
    secondMoment *= std::riemann_zetal(a + 3);
    secondMoment /= Z.GetZetaFunction();
    return secondMoment;
}

long double PlanckRand::Variance() const
{
    double mean = Mean();
    return SecondMoment() - mean * mean;
}

double PlanckRand::Mode() const
{
    if (a <= 1)
        return 0.0;
    double y = -a * std::exp(-a);
    y = RandMath::W0Lambert(y);
    return (y + a) / b;
}

long double PlanckRand::ThirdMoment() const
{
    long double thirdMoment = (a + 3) * (a + 2) * (a + 1);
    thirdMoment /= (b * b * b);
    thirdMoment *= std::riemann_zetal(a + 4);
    thirdMoment /= Z.GetZetaFunction();
    return thirdMoment;
}

long double PlanckRand::Skewness() const
{
    long double mean = Mean();
    long double secondMoment = SecondMoment();
    long double thirdMoment = ThirdMoment();
    long double meanSq = mean * mean;
    long double variance = secondMoment - meanSq;
    long double numerator = thirdMoment - 3 * mean * variance - mean * meanSq;
    long double denominator = std::pow(variance, 1.5);
    return numerator / denominator;
}

long double PlanckRand::FourthMoment() const
{
    long double fourthMoment = (a + 4) * (a + 3) * (a + 2) * (a + 1);
    long double bSq = b * b;
    fourthMoment /= (bSq * bSq);
    fourthMoment *= std::riemann_zetal(a + 5);
    fourthMoment /= Z.GetZetaFunction();
    return fourthMoment;
}

long double PlanckRand::ExcessKurtosis() const
{
    long double mean = Mean();
    long double secondMoment = SecondMoment();
    long double thirdMoment = ThirdMoment();
    long double fourthMoment = FourthMoment();
    long double meanSq = mean * mean;
    long double variance = secondMoment - meanSq;
    long double numerator = fourthMoment - 4 * thirdMoment * mean + 6 * secondMoment * meanSq - 3 * meanSq * meanSq;
    long double denominator = variance * variance;
    return numerator / denominator - 3.0;
}

std::complex<double> PlanckRand::CFImpl(double t) const
{
    if (a >= 1)
        return ContinuousDistribution::CFImpl(t);

    /// We have singularity point at 0 for real part,
    /// so we split the integral in two intervals:
    /// First one from 0 to 1, for which we integrate
    /// numerically leveled pdf and add known solution for level.
    /// Second one from 1 to infinity, for which we use
    /// simple expected value for the rest of the function
    double re1 = RandMath::integral([this, t] (double x)
    {
        return std::cos(t * x) * h(x);
    },
    0.0, 1.0);

    double re2 = this->ExpectedValue([this, t] (double x)
    {
        return std::cos(t * x);
    },
    1.0, INFINITY);

    double re3 = t * RandMath::integral([this, t] (double x)
    {
        if (x <= 0.0)
            return 0.0;
        return std::sin(t * x) * std::pow(x, a);
    },
    0.0, 1.0);

    re3 += std::cos(t);
    re3 *= std::exp(pdfCoef) / (b * a);

    double im = this->ExpectedValue([this, t] (double x)
    {
        return std::sin(t * x);
    },
    0.0, INFINITY);

    double re = re1 + re2 + re3;
    return std::complex<double>(re, im);
}

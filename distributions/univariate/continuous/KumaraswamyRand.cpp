#include "KumaraswamyRand.h"
#include "ParetoRand.h"
#include "UniformRand.h"

KumaraswamyRand::KumaraswamyRand(double shape1, double shape2)
{
    SetShapes(shape1, shape2);
}

std::string KumaraswamyRand::Name() const
{
    return "Kumaraswamy(" + toStringWithPrecision(GetA()) + ", "
                          + toStringWithPrecision(GetB()) + ")";
}

void KumaraswamyRand::SetShapes(double shape1, double shape2)
{
    a = (shape1 > 0) ? shape1 : 1.0;
    b = (shape2 > 0) ? shape2 : 1.0;
}

double KumaraswamyRand::f(double x) const
{
    if (x < 0.0 || x > 1.0)
        return 0.0;

    /// Deal with boundary cases
    if (x == 0) {
        if (a == 1)
            return a * b;
        return (a > 1) ? 0 : INFINITY;
    }
    if (x == 1) {
        if (b == 1)
            return a * b;
        return (b > 1) ? 0 : INFINITY;
    }

    if (a == b) {
        double y = 1.0 - std::pow(x, a);
        y *= x;
        y = std::pow(y, a - 1);
        return a * a * y;
    }
    double y = std::log1p(-std::pow(x, a));
    y *= b - 1;
    y += (a - 1) * std::log(x);
    return a * b * std::exp(y);
}

double KumaraswamyRand::F(double x) const
{
    if (x <= 0.0)
        return 0.0;
    if (x >= 1.0)
        return 1.0;
    double y = 1.0 - std::pow(x, a);
    return 1.0 - std::pow(y, b);
}

double KumaraswamyRand::S(double x) const
{
    if (x <= 0.0)
        return 1.0;
    if (x >= 1.0)
        return 0.0;
    double y = 1.0 - std::pow(x, a);
    return std::pow(y, b);
}

double KumaraswamyRand::Variate() const
{
    double X = ParetoRand::Variate(b, 1);
    return std::pow(1.0 - 1.0 / X, 1.0 / a);
}

void KumaraswamyRand::Sample(std::vector<double> &outputData) const
{
    ParetoRand X(b);
    X.Sample(outputData);
    double aInv = 1.0 / a;
    for (double & var : outputData)
        var = std::pow(1.0 - 1.0 / var, aInv);
}

double KumaraswamyRand::Mean() const
{
    return Moment(1);
}

double KumaraswamyRand::Variance() const
{
    double m1 = Moment(1), m2 = Moment(2);
    return m2 - m1 * m1;
}

double KumaraswamyRand::Median() const
{
    return std::pow(-std::expm1(-M_LN2 / b), 1.0 / a);
}

double KumaraswamyRand::Mode() const
{
    if (a > 1)
        return (b > 1) ? std::pow((a - 1) / (a * b - 1), 1.0 / a) : 1.0;
    return (b > 1) ? 0.0 : (a > b);
}

double KumaraswamyRand::Skewness() const
{
    double m1 = Moment(1), m2 = Moment(2), m3 = Moment(3);
    double denominator = std::pow(m2 - m1 * m1, 1.5);
    double skewness = m3 - 3 * m1 * m2 + 2 * std::pow(m1, 3);
    return skewness / denominator;
}

double KumaraswamyRand::ExcessKurtosis() const
{
    double m1 = Moment(1), m2 = Moment(2), m3 = Moment(3), m4 = Moment(4);
    double var = m2 - m1 * m1;
    double denominator = var * var;
    double kurt = m4 - 4 * m1 * m3 + 6 * m1 * m1 * m2 - 3 * std::pow(m1, 4);
    return kurt / denominator;
}

double KumaraswamyRand::Moment(int n) const
{
    return b * RandMath::betaFun((a + n) / a, b);
}

double KumaraswamyRand::quantileImpl(double p) const
{
    double x = std::pow(1.0 - p, 1.0 / b);
    return std::pow(1.0 - x, 1.0 / a);
}

double KumaraswamyRand::quantileImpl1m(double p) const
{
    double x = std::pow(p, 1.0 / b);
    return std::pow(1.0 - x, 1.0 / a);
}
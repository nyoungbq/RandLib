#ifndef GEOMETRICSTABLERAND_H
#define GEOMETRICSTABLERAND_H

#include "LimitingDistribution.h"
#include "StableRand.h"

/**
 * @brief The GeometricStableRand class
 */
class RANDLIBSHARED_EXPORT GeometricStableRand : public LimitingDistribution
{
    StableRand Z;

protected:
    /// parameters for α = 2
    double k; /// asymmetry coefficient
    double kInv, kSq; /// 1 / k and k * k
    double pdfCoef; /// 1 / (σ * (k + 1 / k))
    double cdfCoef; /// 1 / (1 + k * k)

public:
    GeometricStableRand(double exponent, double skewness, double scale = 1, double location = 0);
    virtual ~GeometricStableRand() {}

    std::string name() const override;
    SUPPORT_TYPE supportType() const override { return (alpha < 1 && beta == 1) ? RIGHTSEMIFINITE_T : INFINITE_T; }
    double MinValue() const override { return (alpha < 1 && beta == 1) ? 0 : -INFINITY; }
    double MaxValue() const override { return INFINITY; }

public:
    void setParameters(double exponent, double skewness, double scale, double location);

protected:
    double pdfLaplace(double x) const;
    double cdfLaplace(double x) const;

private:
    double pdfByLevy(double x) const;

public:
    double f(double x) const override;
    double F(double x) const override;

private:
    double variateForUnityExponent() const;
    double variateForCommonExponent() const;
    double variateByLevy(bool positive) const;
    double variateByCauchy() const;
public:
    double variate() const override;
    void sample(std::vector<double> &outputData) const override;

    double Variance() const override;
    double Median() const override;
    double Mode() const override;
    double Skewness() const override;
    double ExcessKurtosis() const override;

    std::complex<double> CF(double t) const override;
};

#endif // GEOMETRICSTABLERAND_H

#ifndef LOGISTICRAND_H
#define LOGISTICRAND_H

#include "ContinuousDistribution.h"
#include "UniformRand.h"

/**
 * @brief The LogisticRand class
 */
class RANDLIBSHARED_EXPORT LogisticRand : public ContinuousDistribution
{
    double mu, s;

public:
    LogisticRand(double location = 0, double scale = 1);

    std::string name() const override;
    SUPPORT_TYPE supportType() const override { return INFINITE_T; }
    double MinValue() const override { return -INFINITY; }
    double MaxValue() const override { return INFINITY; }

    void setLocation(double location);
    void setScale(double scale);
    inline double getLocation() const { return mu; }
    inline double getScale() const { return s; }

    double f(double x) const override;
    double F(double x) const override;
    double variate() const override;

    double Mean() const override;
    double Variance() const override;
    double Median() const override;
    double Mode() const override;
    double Skewness() const override;
    double ExcessKurtosis() const override;

    std::complex<double> CF(double t) const override;
private:
    double quantileImpl(double p) const override;
    double quantileImpl1m(double p) const override;

public:
    /// Method of moments
    bool fitLocationMM(const std::vector<double> &sample);
    bool fitScaleMM(const std::vector<double> &sample);
    bool fitMM(const std::vector<double> &sample);

    /// Maximum-likelihood
    bool fitLocationMLE(const std::vector<double> &sample);
};

#endif // LOGISTICRAND_H

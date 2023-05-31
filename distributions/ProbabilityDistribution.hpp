#pragma once

#include "distributions/BasicRandGenerator.hpp"

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

/**
 * @brief The ProbabilityDistribution class <BR>
 * Abstract class for all probability distributions
 */
template <typename T, class Engine = JLKiss64RandEngine>
class ProbabilityDistribution
{
protected:
  ProbabilityDistribution() = default;

  virtual ~ProbabilityDistribution() = default;

public:
  //-------------------------------------------------------------------------------------------
  // PURE VIRTUAL
  //-------------------------------------------------------------------------------------------
  /**
   * @fn Name
   * @return title of distribution, for instance "Normal(0, 1)"
   */
  virtual std::string Name() const = 0;

  /**
   * @fn MinValue
   * @return minimum possible value that can be achieved by random variable
   */
  virtual T MinValue() const = 0;

  /**
   * @fn MaxValue
   * @return maximum possible value that can be achieved by random variable
   */
  virtual T MaxValue() const = 0;

  /**
   * @fn F
   * @param x
   * @return P(X ≤ x)
   */
  virtual double F(const T& x) const = 0;

  /**
   * @fn Variate()
   * @return random variable
   */
  virtual T Variate() const = 0;

  //-------------------------------------------------------------------------------------------
  // VIRTUAL
  //-------------------------------------------------------------------------------------------

  /**
   * @fn S
   * @param x
   * @return P(X > x)
   */
  virtual double S(const T& x) const
  {
    return 1.0 - this->F(x);
  }

  /**
   * @fn Sample
   * @param outputData
   */
  virtual void Sample(std::vector<T>& outputData) const
  {
    for(T& var : outputData)
      var = this->Variate();
  }

  /**
   * @brief Reseed
   * @param seed
   */
  virtual void Reseed(unsigned long seed) const
  {
    this->localRandGenerator.Reseed(seed);
  }

  //-------------------------------------------------------------------------------------------
  // NON-VIRTUAL
  //-------------------------------------------------------------------------------------------
  /**
   * @fn CumulativeDistributionFunction
   * @param x input vector
   * @param y output vector: y = P(X ≤ x)
   */
  void CumulativeDistributionFunction(const std::vector<T>& x, std::vector<double>& y) const
  {
    size_t size = x.size();
    if(size > y.size())
      return;
    for(size_t i = 0; i != size; ++i)
      y[i] = this->F(x[i]);
  }

  /**
   * @fn SurvivalFunction
   * @param x input vector
   * @param y output vector: y = P(X > x)
   */
  void SurvivalFunction(const std::vector<T>& x, std::vector<double>& y) const
  {
    size_t size = x.size();
    if(size > y.size())
      return;
    for(size_t i = 0; i != size; ++i)
      y[i] = this->S(x[i]);
  }

protected:
  static BasicRandGenerator<Engine> staticRandGenerator;

  mutable BasicRandGenerator<Engine> localRandGenerator = BasicRandGenerator<Engine>{};

  /**
   * @brief MAX_ITER_REJECTION
   * upper boundary for maximum amount of iterations in rejection methods
   * one thousand should be enough to be sure there is a bug
   * (or rejection method is too slow to be used)
   */
  static constexpr size_t MAX_ITER_REJECTION = 1000;

  std::string toStringWithPrecision(const double a_value, const int n = 6) const
  {
    std::ostringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
  }

  enum FIT_ERROR_TYPE
  {
    WRONG_SAMPLE,
    NOT_APPLICABLE,
    WRONG_RETURN,
    TOO_FEW_ELEMENTS,
    WRONG_LEVEL,
    UNDEFINED_ERROR
  };

  static constexpr char UPPER_LIMIT_VIOLATION[] = "No element should be greater than ";
  static constexpr char LOWER_LIMIT_VIOLATION[] = "No element should be smaller than ";

  std::string fitErrorDescription(FIT_ERROR_TYPE fet, const std::string& explanation)
  {
    std::string error = this->Name() + ": ";
    switch(fet)
    {
    case WRONG_SAMPLE:
      error += "Sample couldn't be generated by this distribution. ";
      break;
    case NOT_APPLICABLE:
      error += "Method cannot be applied here. ";
      break;
    case WRONG_RETURN:
      error += "Method returns invalid parameters. ";
      break;
    case TOO_FEW_ELEMENTS:
      error += "Sample is too small. ";
      break;
    case WRONG_LEVEL:
      error += "Significance level should be positive and smaller than 1. ";
      break;
    case UNDEFINED_ERROR:
    default:
      error += "Unknown type of error. ";
    }
    return error + explanation;
  }
};
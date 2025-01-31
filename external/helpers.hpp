/*################################################################################
  ##
  ##   Copyright (C) 2016-2023 Keith O'Hara
  ##
  ##   This file is part of the GCE-Math C++ library.
  ##
  ##   Licensed under the Apache License, Version 2.0 (the "License");
  ##   you may not use this file except in compliance with the License.
  ##   You may obtain a copy of the License at
  ##
  ##       http://www.apache.org/licenses/LICENSE-2.0
  ##
  ##   Unless required by applicable law or agreed to in writing, software
  ##   distributed under the License is distributed on an "AS IS" BASIS,
  ##   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  ##   See the License for the specific language governing permissions and
  ##   limitations under the License.
  ##
  ################################################################################*/

#pragma once

#include "gcem_options.hpp"
#include "sgn.hpp"

namespace helpers
{
template <class T>
using GCLIM = std::numeric_limits<T>;

using namespace gcem;

/**
 * Compile-time absolute value function
 *
 * @param x a real-valued input.
 * @return the absolute value of \c x, \f$ |x| \f$.
 */

template <typename T>
constexpr T abs(const T x) noexcept
{
  return ( // deal with signed-zeros
      x == T(0) ? T(0) :
                  // else
          x < T(0) ? -x :
                     x);
}

template <typename T>
constexpr T mantissa(const T x) noexcept
{
  return (x < T(1)  ? mantissa(x * T(10)) :
          x > T(10) ? mantissa(x / T(10)) :
                      // else
                      x);
}

constexpr bool is_odd(const llint_t x) noexcept
{
  // return( x % llint_t(2) == llint_t(0) ? false : true );
  return (x & 1U) != 0;
}

template <typename T>
constexpr bool is_nan(const T x) noexcept
{
  return x != x;
}

template <typename T>
constexpr bool is_posinf(const T x) noexcept
{
  return x == GCLIM<T>::infinity();
}

template <typename T>
constexpr bool is_neginf(const T x) noexcept
{
  return x == -GCLIM<T>::infinity();
}

template <typename T>
constexpr bool is_inf(const T x) noexcept
{
  return (is_neginf(x) || is_posinf(x));
}

template <typename T>
constexpr bool is_finite(const T x) noexcept
{
  return (!is_nan(x)) && (!is_inf(x));
}
} // namespace helpers

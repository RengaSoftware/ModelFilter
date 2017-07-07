//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "DoublePrecise.h"

const double DoublePrecise::m_eps = 10e-6;

bool DoublePrecise::eq(const double a, const double b)
{
  return std::abs(a - b) < m_eps;
}

bool DoublePrecise::neq(const double a, const double b)
{
  return !eq(a, b);
}

bool DoublePrecise::geq(const double a, const double b)
{
  return !less(a, b);
}

bool DoublePrecise::leq(const double a, const double b)
{
  return less(a, b) || eq(a, b);
}

bool DoublePrecise::less(const double a, const double b)
{
  return (a - b) <= -m_eps;
}

bool DoublePrecise::greater(const double a, const double b)
{
  return !leq(a, b);
}

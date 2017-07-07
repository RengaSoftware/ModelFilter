//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

class DoublePrecise
{
public:
  static bool eq(const double a, const double b);
  static bool neq(const double a, const double b);
  static bool geq(const double a, const double b);
  static bool leq(const double a, const double b);
  static bool less(const double a, const double b);
  static bool greater(const double a, const double b);

private:
  static const double m_eps;
};
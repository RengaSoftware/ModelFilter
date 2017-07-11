//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include "FilterData.h"

struct DoubleOperatorData
{
  DoubleOperatorData::DoubleOperatorData(const QString& operatorName, std::function<bool(double, double)> function)
    : m_function(function)
  {
    m_name = QApplication::translate("operator", operatorName.toStdString().data());
  }

  QString m_name;
  const std::function<bool(double, double)> m_function;
};

struct StringOperatorData
{
  StringOperatorData::StringOperatorData(const QString& operatorName, std::function<bool(QString, QString)> function)
    : m_function(function)
  {
    m_name = QApplication::translate("operator", operatorName.toStdString().data());
  }

  QString m_name;
  const std::function<bool(QString, QString)> m_function;
};

class OperatorData
{
public:
  static QString getOperatorName(const OperatorType& type);
  static const std::list<std::pair<OperatorType, QString>>& getOperators(const ValueType valueType);
  static const DoubleOperatorData& doubleOperator(OperatorType dblOperatorType);
  static const StringOperatorData& stringOperator(OperatorType strOperatorType);

private:
  OperatorData();
  OperatorData(OperatorData const&) = delete;
  OperatorData& operator= (OperatorData const&) = delete;
  static std::shared_ptr<OperatorData> Instance();

private:
  static std::shared_ptr<OperatorData> data;

  const std::map<OperatorType, StringOperatorData> m_string;
  const std::map<OperatorType, DoubleOperatorData> m_double;
};
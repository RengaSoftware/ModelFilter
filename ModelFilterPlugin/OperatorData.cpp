//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "DoublePrecise.h"
#include "OperatorData.h"

std::shared_ptr<OperatorData> OperatorData::m_instance = nullptr;

OperatorData::OperatorData()
  : m_stringOperators({
  std::make_pair(OperatorType::Equal, StringOperatorData(QApplication::translate("mf_operator", "equal"), [](const QString one, const QString two) {return one.compare(two) == 0; })),
  std::make_pair(OperatorType::NotEqual, StringOperatorData(QApplication::translate("mf_operator", "notEqual"), [](const QString one, const QString two) {return one.compare(two) != 0; })),
  std::make_pair(OperatorType::Contain, StringOperatorData(QApplication::translate("mf_operator", "contain"), [](const QString one, const QString two) {return one.contains(two); })),
  std::make_pair(OperatorType::NotContain, StringOperatorData(QApplication::translate("mf_operator", "notContain"), [](const QString one, const QString two) {return !one.contains(two); })),
  std::make_pair(OperatorType::Prefix, StringOperatorData(QApplication::translate("mf_operator", "prefix"), [](const QString one, const QString two) {return one.startsWith(two); })),
  std::make_pair(OperatorType::NotPrefix, StringOperatorData(QApplication::translate("mf_operator", "notPrefix"), [](const QString one, const QString two) {return !one.startsWith(two); })),
  std::make_pair(OperatorType::Suffix, StringOperatorData(QApplication::translate("mf_operator", "suffix"), [](const QString one, const QString two) {return one.endsWith(two); })),
  std::make_pair(OperatorType::NotSuffix, StringOperatorData(QApplication::translate("mf_operator", "notSuffix"), [](const QString one, const QString two) {return !one.endsWith(two); })),
  std::make_pair(OperatorType::All, StringOperatorData(QApplication::translate("mf_operator", "all"), [](const QString, const QString) {return true; })),
  std::make_pair(OperatorType::None, StringOperatorData(QApplication::translate("mf_operator", "none"), [](const QString, const QString) {return false; })) })
  , m_doubleOperators({
  std::make_pair(OperatorType::EQ, DoubleOperatorData(QApplication::translate("mf_operator", "eq"), DoublePrecise::eq)),
  std::make_pair(OperatorType::NEQ, DoubleOperatorData(QApplication::translate("mf_operator", "neq"), DoublePrecise::neq)),
  std::make_pair(OperatorType::GEQ, DoubleOperatorData(QApplication::translate("mf_operator", "geq"), DoublePrecise::geq)),
  std::make_pair(OperatorType::LEQ, DoubleOperatorData(QApplication::translate("mf_operator", "leq"), DoublePrecise::leq)),
  std::make_pair(OperatorType::GR, DoubleOperatorData(QApplication::translate("mf_operator", "greater"), DoublePrecise::greater)),
  std::make_pair(OperatorType::LS, DoubleOperatorData(QApplication::translate("mf_operator", "less"), DoublePrecise::less)),
  std::make_pair(OperatorType::All, DoubleOperatorData(QApplication::translate("mf_operator", "all"), [](const double, const double) {return true; })),
  std::make_pair(OperatorType::None, DoubleOperatorData(QApplication::translate("mf_operator", "none"), [](const double, const double) {return false; })) })
{}

std::shared_ptr<OperatorData> OperatorData::Instance()
{
  if (m_instance.get() == nullptr)
  {
    m_instance = std::shared_ptr<OperatorData>(new OperatorData());
  }
  return m_instance;
}

QString OperatorData::getOperatorName(const OperatorType& type)
{
  auto it = Instance()->m_doubleOperators.find(type);
  if (it != Instance()->m_doubleOperators.end())
    return it->second.m_name;
  auto it2 = Instance()->m_stringOperators.find(type);
  if (it2 != Instance()->m_stringOperators.end())
    return it2->second.m_name;

  assert(false);
  return QString("");
}

std::list<std::pair<OperatorType, QString>> OperatorData::getOperators(const ValueType valueType)
{
  std::list<std::pair<OperatorType, QString>> operatorList;
  switch (valueType)
  {
  case ValueType::Double:
  {
    for (auto& it : Instance()->m_doubleOperators)
      operatorList.push_back(std::make_pair(it.first, it.second.m_name));
    break;
  }
  case ValueType::String:
  {
    for (auto& it : Instance()->m_stringOperators)
      operatorList.push_back(std::make_pair(it.first, it.second.m_name));
    break;
  }
  default:
    break;
  }
  return operatorList;
}

const StringOperatorData & OperatorData::stringOperator(OperatorType strOperatorType)
{
  return OperatorData::Instance()->m_stringOperators.at(strOperatorType);
}

const DoubleOperatorData & OperatorData::doubleOperator(OperatorType dblOperatorType)
{
  return OperatorData::Instance()->m_doubleOperators.at(dblOperatorType);
}

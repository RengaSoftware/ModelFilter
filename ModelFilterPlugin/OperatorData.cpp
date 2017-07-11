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
  std::make_pair(OperatorType::Equal, StringOperatorData("equal", [](const QString one, const QString two) {return one.compare(two) == 0; })),
  std::make_pair(OperatorType::NotEqual, StringOperatorData("notEqual", [](const QString one, const QString two) {return one.compare(two) != 0; })),
  std::make_pair(OperatorType::Contain, StringOperatorData("contain", [](const QString one, const QString two) {return one.contains(two); })),
  std::make_pair(OperatorType::NotContain, StringOperatorData("notContain", [](const QString one, const QString two) {return !one.contains(two); })),
  std::make_pair(OperatorType::Prefix, StringOperatorData("prefix", [](const QString one, const QString two) {return one.startsWith(two); })),
  std::make_pair(OperatorType::NotPrefix, StringOperatorData("notPrefix", [](const QString one, const QString two) {return !one.startsWith(two); })),
  std::make_pair(OperatorType::Suffix, StringOperatorData("suffix", [](const QString one, const QString two) {return one.endsWith(two); })),
  std::make_pair(OperatorType::NotSuffix, StringOperatorData("notSuffix", [](const QString one, const QString two) {return !one.endsWith(two); })),
  std::make_pair(OperatorType::All, StringOperatorData("all", [](const QString, const QString) {return true; })) })
  , m_doubleOperators({
  std::make_pair(OperatorType::EQ, DoubleOperatorData("eq", DoublePrecise::eq)),
  std::make_pair(OperatorType::NEQ, DoubleOperatorData("neq", DoublePrecise::neq)),
  std::make_pair(OperatorType::GEQ, DoubleOperatorData("geq", DoublePrecise::geq)),
  std::make_pair(OperatorType::LEQ, DoubleOperatorData("leq", DoublePrecise::leq)),
  std::make_pair(OperatorType::GR, DoubleOperatorData("greater", DoublePrecise::greater)),
  std::make_pair(OperatorType::LS, DoubleOperatorData("less", DoublePrecise::less)),
  std::make_pair(OperatorType::All, DoubleOperatorData("all", [](const double, const double) {return true; })) })
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

const std::list<std::pair<OperatorType, QString>>& OperatorData::getOperators(const ValueType valueType)
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

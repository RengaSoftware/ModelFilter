//
// Copyright ìRenga Softwareî LLC, 2017. All rights reserved.
//
// ìRenga Softwareî LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// ìRenga Softwareî LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include <list>
#include <functional>

#include <QtCore/QString>
#include <QtCore/QFile.h>
#include <QtCore/QXmlStream.h>
#include <QtWidgets/QApplication.h>

#include <RengaAPI/ModelObjectTypes.h>

enum ValueType {
  Double = 0,
  String = 1,
};

enum PropertyType {
  Name,
  Material,
  Mark,
  UserAttribute,
  LevelHeight,
  Offset,
  Length,
  Width,
  Height,
  Depth,
  Thickness,
  Perimeter,
  Area,
  CrossSectionArea,
  FloorArea,
  FootprintArea,
  OuterSurfaceArea,
  SideArea,
  TotalSurfaceArea,
  Volume,
  Mass,
  NumberOfRiser,
  NumberOfTreads,
  RiserHeight,
  TreadLength,
  RoomNumber,
  Elevation,
  Invalid,
};

enum OperatorType {
  Equal,
  NotEqual,
  Contain,
  NotContain,
  Prefix,
  NotPrefix,
  Suffix,
  NotSuffix,
  EQ,
  NEQ,
  GEQ,
  LEQ,
  GR, // greater
  LS, //less
};

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

struct Property {
  Property::Property(const PropertyType& propertyType, const ValueType valueType, const QString& name)
    : m_propertyType(propertyType)
    , m_valueType(valueType)
    , m_propertyName(name)
  {}

  PropertyType m_propertyType;
  ValueType m_valueType;
  QString m_propertyName;
};

typedef std::list<Property> PropertyList;

struct Search—riteriaData {
  Search—riteriaData::Search—riteriaData(const Property& property, const OperatorType& operatorType, const QString& value)
    : m_property(property)
    , m_operatorType(operatorType)
    , m_value(value)
  {}

  Property m_property;
  OperatorType m_operatorType;
  QString m_value;
};

struct GroupData {
  GroupData::GroupData(const rengaapi::ObjectType objectType)
    : m_groupType(objectType)
  {}

  rengaapi::ObjectType m_groupType;
  std::list<Search—riteriaData> m_propertyList;
};

struct FilterData {
  FilterData::FilterData(const QString& name)
    : m_filterName(name)
  {}

  static FilterData importData(QFile* filterFile);
  void exportData(QFile* filterFile);

private:
  static GroupData parseGroup(QXmlStreamReader& reader);
  static Search—riteriaData parseProperty(QXmlStreamReader& reader, rengaapi::ObjectType type);
  static QString parseTagText(QXmlStreamReader& reader);

public:
  QString m_filterName;
  std::list<GroupData> m_groupList;
};
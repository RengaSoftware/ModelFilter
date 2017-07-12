//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
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
  All,
  None
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

struct SearchCriteriaData {
  SearchCriteriaData::SearchCriteriaData(const Property& property, const OperatorType& operatorType, const QString& value)
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

  bool isValid();

  rengaapi::ObjectType m_groupType;
  std::list<SearchCriteriaData> m_propertyList;
};

struct FilterData {
  FilterData::FilterData()
  {}

  FilterData::FilterData(const FilterData& copy)
    : m_filterName(copy.m_filterName)
    , m_groupList(copy.m_groupList)
  {}

  static FilterData importData(QFile* filterFile);
  void exportData(QFile* filterFile);
  bool isValid();

private:
  static GroupData parseGroup(QXmlStreamReader& reader);
  static SearchCriteriaData parseSearchCriteria(QXmlStreamReader& reader, const rengaapi::ObjectType& type);
  static bool getTypeNumber(const QString& value, int& res);
  static bool getPropertyName(const PropertyType propertyType, const rengaapi::ObjectType& type, QString& propertyName);
  static QString parseTagText(QXmlStreamReader& reader);

public:
  QString m_filterName;
  std::list<GroupData> m_groupList;
};
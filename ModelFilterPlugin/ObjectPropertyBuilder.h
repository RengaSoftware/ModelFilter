//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include "FilterData.h"

#include <RengaAPI/MaterialId.h>
#include <RengaAPI/ModelObject.h>
#include <RengaBase/LengthMeasureOptional.h>
#include <RengaBase/AreaMeasureOptional.h>
#include <RengaBase/VolumeMeasureOptional.h>

enum MeasureUnit
{
  Meter = 0,
  Centimeter = 1,
  Millimeter = 2,
  Inch = 3
};

class ObjectPropertyBuilder
{
public:
  ObjectPropertyBuilder();
  virtual ~ObjectPropertyBuilder();
  virtual PropertyList getObjectProperties() = 0;
  virtual bool isObjectMatchFilter(const SearchCriteriaData& data, rengaapi::ModelObject* pObject) = 0;

  static PropertyList getUserAttributes(const rengaapi::ObjectType& objectType);

  QString getOperatorName(const OperatorType& type);
  QString getMaterialName(const rengaapi::MaterialId id);
  std::list<std::pair<OperatorType, QString>> getOperators(const ValueType valueType);

private:
  const std::map<OperatorType, StringOperatorData> m_stringOperatorData;
  const std::map<OperatorType, DoubleOperatorData> m_doubleOperatorData;

protected:
  bool apply(const rengabase::LengthMeasureOptional& measure, const SearchCriteriaData& data, MeasureUnit unit = MeasureUnit::Millimeter);
  bool apply(const rengabase::AreaMeasureOptional& measure, const SearchCriteriaData& data, MeasureUnit unit = MeasureUnit::Meter);
  bool apply(const rengabase::VolumeMeasureOptional& measure, const SearchCriteriaData& data, MeasureUnit unit = MeasureUnit::Meter);
  bool apply(const rengabase::LengthMeasure& lengthMeasure, const SearchCriteriaData& data);
  bool apply(const rengabase::AreaMeasure& areaMeasure, const SearchCriteriaData& data);
  bool apply(const rengabase::VolumeMeasure& volumeMeasure, const SearchCriteriaData& data);
  bool apply(const rengaapi::MaterialId& materialId, const rengabase::VolumeMeasureOptional& volumeMeasure, const SearchCriteriaData& data);
  bool apply(const rengaapi::MaterialId& materialId, const std::vector<rengabase::VolumeMeasureOptional>& volumeMeasureCollection, const SearchCriteriaData& data);
  bool apply(const rengaapi::ObjectId& levelId, const SearchCriteriaData& data);
  bool apply(const uint number, const SearchCriteriaData& data);
  bool apply(const double value, const SearchCriteriaData& data);

  bool apply(const rengabase::String& rengaString, const SearchCriteriaData& data);
  bool apply(const rengaapi::MaterialId& materialId, const SearchCriteriaData& data);
  bool apply(const QString& value, const SearchCriteriaData& data);

  bool isUserAttributeMatchFilter(const rengaapi::ModelObject* pObject, const SearchCriteriaData& data);

private:
  double countOneLayeredMass(const rengaapi::MaterialId& materialId, const rengabase::VolumeMeasureOptional& volumeMeasure);
  double countMultiLayeredMass(const rengaapi::MaterialId& materialId, const std::vector<rengabase::VolumeMeasureOptional>& volumeMeasureCollection);
};
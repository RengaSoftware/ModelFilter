//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "ObjectFilter.h"

#include <RengaAPI/Level.h>
#include <RengaAPI/Materials.h>
#include <RengaAPI/ModelObjectTypes.h>
#include <RengaAPI/Project.h>
#include <RengaAPI/UserAttributeRegister.h>
#include <RengaAPI/UserAttributeDescription.h>
#include <RengaBase/LengthMeasure.h>
#include <RengaBase/AreaMeasure.h>
#include <RengaBase/VolumeMeasure.h>

ObjectFilter::ObjectFilter()
  : m_operatorData(OperatorData::Instance())
{}

ObjectFilter::~ObjectFilter()
{}

bool ObjectFilter::apply(const rengabase::LengthMeasureOptional& measure, const SearchCriteriaData& data, MeasureUnit unit)
{
  if (!measure.hasValue())
    return false;

  double value;
  switch (unit)
  {
  case Meter:
    value = measure.getValue()->inMeters();
    break;
  case Centimeter:
    value = measure.getValue()->inCentimeters();
    break;
  case Inch:
    value = measure.getValue()->inInches();
    break;
  case Millimeter:
  default:
    value = measure.getValue()->inMillimeters();
    break;
  }

  return apply(value, data);
}

bool ObjectFilter::apply(const rengabase::AreaMeasureOptional& measure, const SearchCriteriaData& data, MeasureUnit unit)
{
  if (!measure.hasValue())
    return false;

  double value;
  switch (unit)
  {
  case Centimeter:
    value = measure.getValue()->inCentimeters2();
    break;
  case Millimeter:
    value = measure.getValue()->inMillimeters2();
    break;
  case Meter:
  default:
    value = measure.getValue()->inMeters2();
    break;
  }

  return apply(value, data);
}

bool ObjectFilter::apply(const rengabase::VolumeMeasureOptional& measure, const SearchCriteriaData& data, MeasureUnit unit)
{
  if (!measure.hasValue())
    return false;

  double value;
  switch (unit)
  {
  case Centimeter:
    value = measure.getValue()->inCentimeters3();
    break;
  case Millimeter:
    value = measure.getValue()->inMillimeters3();
    break;
  case Meter:
  default:
    value = measure.getValue()->inMeters3();
    break;
  }

  return apply(value, data);
}

bool ObjectFilter::apply(const rengabase::LengthMeasure& lengthMeasure, const SearchCriteriaData& data)
{
  return apply(lengthMeasure.inMillimeters(), data);
}

bool ObjectFilter::apply(const rengabase::AreaMeasure& areaMeasure, const SearchCriteriaData& data)
{
  return apply(areaMeasure.inMeters2(), data);
}

bool ObjectFilter::apply(const rengabase::VolumeMeasure& volumeMeasure, const SearchCriteriaData& data)
{
  return apply(volumeMeasure.inMeters3(), data);
}

bool ObjectFilter::apply(const rengaapi::MaterialId& materialId, const rengabase::VolumeMeasureOptional& volumeMeasure, const SearchCriteriaData& data)
{
  const double mass = countOneLayeredMass(materialId, volumeMeasure);

  if (mass < 0)
    return false;

  return apply(mass, data);
}

bool ObjectFilter::apply(const rengaapi::MaterialId& materialId, const std::vector<rengabase::VolumeMeasureOptional>& volumeMeasureCollection, const SearchCriteriaData& data)
{
  const double mass = countMultiLayeredMass(materialId, volumeMeasureCollection);

  if (mass < 0)
    return false;

  return apply(mass, data);
}

bool ObjectFilter::apply(const rengaapi::ObjectId& levelId, const SearchCriteriaData& data)
{
  rengaapi::Model rengaProjectModel = rengaapi::Project::model();
  rengaapi::ObjectFilter levelsFilter = rengaapi::ObjectFilter::createObjectFilterByType(rengaapi::ModelObjectTypes::LevelType);
  rengaapi::ModelObjectCollection levelsCollection = rengaProjectModel.objects(levelsFilter);

  for (auto pObject : levelsCollection)
  {
    if (pObject->objectId() == levelId)
    {
      rengaapi::Level* pLevel = dynamic_cast<rengaapi::Level*>(pObject);
      assert(pLevel != nullptr);
      return apply(pLevel->levelName(), data);
    }
  }

  assert(false);
  return false;
}

bool ObjectFilter::apply(const uint number, const SearchCriteriaData& data)
{
  return apply(static_cast<double>(number), data);
}

bool ObjectFilter::apply(const double value, const SearchCriteriaData& data)
{
  if (data.m_value.length() == 0)
    return true;

  auto it = m_operatorData->m_double.find(data.m_operatorType);
  assert(it != m_doubleOperatorData.end());
  return it->second.m_function(value, data.m_value.toDouble());
}

bool ObjectFilter::apply(const rengabase::String& rengaString, const SearchCriteriaData& data)
{
  return apply(rengaStringToQString(rengaString), data);
}

bool ObjectFilter::apply(const rengaapi::MaterialId& materialId, const SearchCriteriaData& data)
{
  return apply(getMaterialName(materialId), data);
}

bool ObjectFilter::apply(const QString & value, const SearchCriteriaData& data)
{
  if (data.m_value.length() == 0)
    return true;

  auto it = m_operatorData->m_string.find(data.m_operatorType);
  assert(it != m_stringOperatorData.end());
  return it->second.m_function(value, data.m_value);
}

bool ObjectFilter::isUserAttributeMatchFilter(const rengaapi::ModelObject* pObject, const SearchCriteriaData& data)
{
  rengaapi::UserAttributeRegister userAttributeRegister = rengaapi::Project::userAttributeRegister();
  rengaapi::UserAttributeIdCollection attributeCollection = userAttributeRegister.attributes();
  const rengaapi::UserAttributeType attributeType = data.m_property.m_valueType == ValueType::Double ? rengaapi::UserAttributeType::Double : rengaapi::UserAttributeType::String;
  // check all attributes
  for (size_t i = 0; i < attributeCollection.size(); ++i)
  {
    const rengaapi::UserAttributeId& attributeId = attributeCollection.get(i);
    // check if object has attribute
    if (userAttributeRegister.typeHasAttribute(pObject->type(), attributeId))
    {
      rengaapi::UserAttributeDescription userAttributeDescription;
      // get attribute description
      if (userAttributeRegister.attributeDescription(attributeId, userAttributeDescription).code() == rengaapi::Status::Success)
      {
        // check attribute name and type
        QString attributeDescriptionName = rengaStringToQString(userAttributeDescription.name());
        if (attributeDescriptionName.compare(data.m_property.m_propertyName) == 0 && userAttributeDescription.type() == attributeType)
        {
          // get value and apply filter
          rengaapi::UserAttributeValue userAttributeValue;
          rengaapi::Status status = pObject->getUserAttributeValue(attributeId, userAttributeValue);
          assert(status.code() == rengaapi::Status::Code::Success);
          if (attributeType == rengaapi::UserAttributeType::Double)
            return apply(userAttributeValue.asDouble(), data);
          else if (attributeType == rengaapi::UserAttributeType::String)
            return apply(userAttributeValue.asString(), data);
        }
      }
    }
  }
  // if attribute was not found, don't apply filter and return true
  return true;
}

QString ObjectFilter::getMaterialName(const rengaapi::MaterialId materialId)
{
  rengaapi::Materials::MaterialType materialType = rengaapi::Materials::materialType(materialId);
  QString materialName = QString();
  switch (materialType)
  {
  case rengaapi::Materials::MaterialType::Grid:
  {
    rengaapi::GridMaterial gridMaterial;
    rengaapi::Materials::gridMaterial(materialId, gridMaterial);
    // grid materials are nameless, return collection name
    materialName = rengaStringToQString(materialId.collectionName());
  }
  break;
  case rengaapi::Materials::MaterialType::OneLayered:
  {
    rengaapi::Material material;
    rengaapi::Materials::material(materialId, material);
    materialName = rengaStringToQString(material.name_());
  }
  break;
  case rengaapi::Materials::MaterialType::MultiLayered:
  {
    rengaapi::LayeredMaterial layeredMaterial;
    rengaapi::Materials::layeredMaterial(materialId, layeredMaterial);
    materialName = rengaStringToQString(layeredMaterial.name_());
  }
  break;
  }
  return materialName;
}

double ObjectFilter::countOneLayeredMass(const rengaapi::MaterialId& materialId, const rengabase::VolumeMeasureOptional& volumeMeasure)
{
  assert(rengaapi::Materials::materialType(materialId) == rengaapi::Materials::MaterialType::OneLayered);

  if (!volumeMeasure.hasValue())
    return -1.0;

  rengaapi::Material material;
  rengaapi::Materials::material(materialId, material);
  if (material.isNull())
    return -1.0;

  const double mass = material.density() * volumeMeasure.getValue()->inMeters3();
  return mass;
}

double ObjectFilter::countMultiLayeredMass(const rengaapi::MaterialId& materialId, const std::vector<rengabase::VolumeMeasureOptional>& volumeMeasureCollection)
{
  assert(rengaapi::Materials::materialType(materialId) == rengaapi::Materials::MaterialType::MultiLayered);

  rengaapi::LayeredMaterial layeredMaterial;
  rengaapi::Materials::layeredMaterial(materialId, layeredMaterial);
  if (layeredMaterial.isNull())
    return -1.0;

  rengaapi::MaterialLayersCollection materialLayers = layeredMaterial.layers();
  assert(volumeMeasureCollection.size() == materialLayers.size());

  double mass = 0.0;
  for (size_t i = 0; i < materialLayers.size(); ++i)
  {
    if (volumeMeasureCollection[i].hasValue())
    {
      rengaapi::Material materialLayer = materialLayers.get(i).material();
      if (!materialLayer.isNull())
        mass += materialLayer.density() * volumeMeasureCollection[i].getValue()->inMeters3();
    }
  }

  return mass;
}
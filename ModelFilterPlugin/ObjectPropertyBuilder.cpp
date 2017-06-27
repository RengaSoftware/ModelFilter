//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "ObjectPropertyBuilder.h"

#include <RengaAPI/Level.h>
#include <RengaAPI/Materials.h>
#include <RengaAPI/ModelObjectTypes.h>
#include <RengaAPI/Project.h>
#include <RengaAPI/UserAttributeRegister.h>
#include <RengaAPI/UserAttributeDescription.h>
#include <RengaBase/LengthMeasure.h>
#include <RengaBase/AreaMeasure.h>
#include <RengaBase/VolumeMeasure.h>

ObjectPropertyBuilder::ObjectPropertyBuilder()
  : m_stringOperatorData({
      std::make_pair(OperatorType::Equal, StringOperatorData("equal", [](const QString one, const QString two) {return one.compare(two) == 0; })),
      std::make_pair(OperatorType::NotEqual, StringOperatorData("notEqual", [](const QString one, const QString two) {return one.compare(two) != 0; })),
      std::make_pair(OperatorType::Contain, StringOperatorData("contain", [](const QString one, const QString two) {return one.contains(two); })),
      std::make_pair(OperatorType::NotContain, StringOperatorData("notContain", [](const QString one, const QString two) {return !one.contains(two); })),
      std::make_pair(OperatorType::Prefix, StringOperatorData("prefix", [](const QString one, const QString two) {return one.startsWith(two); })),
      std::make_pair(OperatorType::NotPrefix, StringOperatorData("notPrefix", [](const QString one, const QString two) {return !one.startsWith(two); })),
      std::make_pair(OperatorType::Suffix, StringOperatorData("suffix", [](const QString one, const QString two) {return one.endsWith(two); })),
      std::make_pair(OperatorType::NotSuffix, StringOperatorData("notSuffix", [](const QString one, const QString two) {return !one.endsWith(two); })),
      std::make_pair(OperatorType::All, StringOperatorData("all", [](const QString, const QString) {return true; })) })
      , m_doubleOperatorData({
          std::make_pair(OperatorType::EQ, DoubleOperatorData("eq", [](const double one, const double two) {return one == two; })),
          std::make_pair(OperatorType::NEQ, DoubleOperatorData("neq", [](const double one, const double two) {return one != two; })),
          std::make_pair(OperatorType::GEQ, DoubleOperatorData("geq", [](const double one, const double two) {return one >= two; })),
          std::make_pair(OperatorType::LEQ, DoubleOperatorData("leq", [](const double one, const double two) {return one <= two; })),
          std::make_pair(OperatorType::GR, DoubleOperatorData("greater", [](const double one, const double two) {return one > two; })),
          std::make_pair(OperatorType::LS, DoubleOperatorData("less", [](const double one, const double two) {return one < two; })),
          std::make_pair(OperatorType::All, DoubleOperatorData("all", [](const double, const double) {return true; })) })
{}

ObjectPropertyBuilder::~ObjectPropertyBuilder()
{}

PropertyList ObjectPropertyBuilder::getUserAttributes(const rengaapi::ObjectType& objectType)
{
  std::list<Property> attributeList;
  rengaapi::UserAttributeRegister userAttributeRegister = rengaapi::Project::userAttributeRegister();
  rengaapi::UserAttributeIdCollection attributeCollection = userAttributeRegister.attributes();

  // check all attributes
  for (size_t i = 0; i < attributeCollection.size(); ++i)
  {
    // check if objectType has attribute
    const rengaapi::UserAttributeId& attributeId = attributeCollection.get(i);
    if (userAttributeRegister.typeHasAttribute(objectType, attributeId))
    {
      // get attribute name
      rengaapi::UserAttributeDescription userAttributeDescription;
      if (userAttributeRegister.attributeDescription(attributeId, userAttributeDescription).code() == rengaapi::Status::Success)
      {
        QString attributeName = rengaStringToQString(userAttributeDescription.name());
        ValueType attributeType = userAttributeDescription.type() == rengaapi::UserAttributeType::Double ? ValueType::Double : ValueType::String;
        attributeList.push_back(Property(PropertyType::UserAttribute, attributeType, attributeName));
      }
    }
  }

  return attributeList;
}

QString ObjectPropertyBuilder::getOperatorName(const OperatorType& type)
{
  auto it = m_doubleOperatorData.find(type);
  if (it != m_doubleOperatorData.end())
    return it->second.m_name;
  auto it2 = m_stringOperatorData.find(type);
  if (it2 != m_stringOperatorData.end())
    return it2->second.m_name;

  assert(false);
  return QString("");
}

QString ObjectPropertyBuilder::getMaterialName(const rengaapi::MaterialId materialId)
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

std::list<std::pair<OperatorType, QString>> ObjectPropertyBuilder::getOperators(const ValueType valueType)
{
  std::list<std::pair<OperatorType, QString>> operatorList;
  switch (valueType)
  {
  case ValueType::Double:
  {
    for (auto& it : m_doubleOperatorData)
      operatorList.push_back(std::make_pair(it.first, it.second.m_name));
    break;
  }
  case ValueType::String:
  {
    for (auto& it : m_stringOperatorData)
      operatorList.push_back(std::make_pair(it.first, it.second.m_name));
    break;
  }
  default:
    break;
  }
  return operatorList;
}

bool ObjectPropertyBuilder::apply(const rengabase::LengthMeasureOptional& measure, const SearchCriteriaData& data, MeasureUnit unit)
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

bool ObjectPropertyBuilder::apply(const rengabase::AreaMeasureOptional& measure, const SearchCriteriaData& data, MeasureUnit unit)
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

bool ObjectPropertyBuilder::apply(const rengabase::VolumeMeasureOptional& measure, const SearchCriteriaData& data, MeasureUnit unit)
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

bool ObjectPropertyBuilder::apply(const rengabase::LengthMeasure& lengthMeasure, const SearchCriteriaData& data)
{
  return apply(lengthMeasure.inMillimeters(), data);
}

bool ObjectPropertyBuilder::apply(const rengabase::AreaMeasure& areaMeasure, const SearchCriteriaData& data)
{
  return apply(areaMeasure.inMeters2(), data);
}

bool ObjectPropertyBuilder::apply(const rengabase::VolumeMeasure& volumeMeasure, const SearchCriteriaData& data)
{
  return apply(volumeMeasure.inMeters3(), data);
}

bool ObjectPropertyBuilder::apply(const rengaapi::MaterialId& materialId, const rengabase::VolumeMeasureOptional& volumeMeasure, const SearchCriteriaData& data)
{
  const double mass = countOneLayeredMass(materialId, volumeMeasure);

  if (mass < 0)
    return false;

  return apply(mass, data);
}

bool ObjectPropertyBuilder::apply(const rengaapi::MaterialId& materialId, const std::vector<rengabase::VolumeMeasureOptional>& volumeMeasureCollection, const SearchCriteriaData& data)
{
  const double mass = countMultiLayeredMass(materialId, volumeMeasureCollection);

  if (mass < 0)
    return false;

  return apply(mass, data);
}

bool ObjectPropertyBuilder::apply(const rengaapi::ObjectId& levelId, const SearchCriteriaData& data)
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
      return apply(pLevel->elevation().inMillimeters(), data);
    }
  }

  assert(false);
  return false;
}

bool ObjectPropertyBuilder::apply(const uint number, const SearchCriteriaData& data)
{
  return apply(static_cast<double>(number), data);
}

bool ObjectPropertyBuilder::apply(const double value, const SearchCriteriaData& data)
{
  if (data.m_value.length() == 0)
    return true;

  auto it = m_doubleOperatorData.find(data.m_operatorType);
  assert(it != m_doubleOperatorData.end());
  return it->second.m_function(value, data.m_value.toDouble());
}

bool ObjectPropertyBuilder::apply(const rengabase::String& rengaString, const SearchCriteriaData& data)
{
  return apply(rengaStringToQString(rengaString), data);
}

bool ObjectPropertyBuilder::apply(const rengaapi::MaterialId& materialId, const SearchCriteriaData& data)
{
  return apply(getMaterialName(materialId), data);
}

bool ObjectPropertyBuilder::apply(const QString & value, const SearchCriteriaData& data)
{
  if (data.m_value.length() == 0)
    return true;

  auto it = m_stringOperatorData.find(data.m_operatorType);
  assert(it != m_stringOperatorData.end());
  return it->second.m_function(value, data.m_value);
}

bool ObjectPropertyBuilder::isUserAttributeMatchFilter(const rengaapi::ModelObject* pObject, const SearchCriteriaData& data)
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

double ObjectPropertyBuilder::countOneLayeredMass(const rengaapi::MaterialId& materialId, const rengabase::VolumeMeasureOptional& volumeMeasure)
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

double ObjectPropertyBuilder::countMultiLayeredMass(const rengaapi::MaterialId& materialId, const std::vector<rengabase::VolumeMeasureOptional>& volumeMeasureCollection)
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
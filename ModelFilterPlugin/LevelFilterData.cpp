//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "LevelFilterData.h"

#include <RengaAPI/Level.h>

LevelProperty::LevelProperty()
  : ObjectProperty()
{
  properties = { Property(PropertyType::Name, ValueType::String, QApplication::translate("mf_level", "name")),
    Property(PropertyType::Elevation, ValueType::Double, QApplication::translate("mf_level", "elevation")),
    Property(PropertyType::LevelName, ValueType::String, QApplication::translate("mf_level", "levelName")) };
}

PropertyList LevelProperty::getObjectProperties()
{
  return properties;
}

bool LevelFilter::isObjectMatchFilter(const SearchCriteriaData& data, rengaapi::ModelObject* pObject)
{
  rengaapi::Level* pLevel = dynamic_cast<rengaapi::Level*>(pObject);
  assert(pLevel != nullptr);
  switch (data.m_property.m_propertyType)
  {
  case PropertyType::Elevation:
    return apply(pLevel->elevation(), data, MeasureUnit::Meter);
  case PropertyType::Name:
    return apply(pLevel->name(), data);
  case PropertyType::LevelName:
    return apply(pLevel->levelName(), data);
  case PropertyType::UserAttribute:
    return isUserAttributeMatchFilter(pObject, data);
  default:
    assert(false);
    return false;
  }
}
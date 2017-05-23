//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "LevelPropertyBuilder.h"

#include <RengaAPI/Level.h>

LevelPropertyBuilder::LevelPropertyBuilder()
  : ObjectPropertyBuilder()
{
  properties = { Property(PropertyType::Name, ValueType::String, QApplication::translate("level", "name")),
    Property(PropertyType::Elevation, ValueType::Double, QApplication::translate("level", "elevation")) };
}

PropertyList LevelPropertyBuilder::getObjectProperties()
{
  return properties;
}

bool LevelPropertyBuilder::isObjectMatchFilter(const SearchÑriteriaData& data, rengaapi::ModelObject* pObject)
{
  rengaapi::Level* pLevel = dynamic_cast<rengaapi::Level*>(pObject);
  assert(pLevel != nullptr);
  switch (data.m_property.m_propertyType)
  {
  case PropertyType::Elevation:
    return apply(pLevel->elevation(), data);
  case PropertyType::Name:
    return apply(pLevel->levelName(), data);
  case PropertyType::UserAttribute:
    return isUserAttributeMatchFilter(pObject, data);
  default:
    assert(false);
    return false;
  }
}
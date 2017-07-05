//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "WindowFilterData.h"

#include <RengaAPI/Window.h>

WindowProperty::WindowProperty()
  : ObjectProperty()
{
  properties = { Property(PropertyType::Name, ValueType::String, QApplication::translate("window", "name")),
                 Property(PropertyType::Mark, ValueType::String, QApplication::translate("window", "mark")),
                 Property(PropertyType::LevelHeight, ValueType::String, QApplication::translate("window", "level")),
                 Property(PropertyType::Offset, ValueType::Double, QApplication::translate("window", "offset")),
                 Property(PropertyType::Height, ValueType::Double, QApplication::translate("window", "overallHeight")),
                 Property(PropertyType::Width, ValueType::Double, QApplication::translate("window", "overallWidth")),
                 Property(PropertyType::Perimeter, ValueType::Double, QApplication::translate("window", "perimeter")),
                 Property(PropertyType::Area, ValueType::Double, QApplication::translate("window", "area")) };
}

PropertyList WindowProperty::getObjectProperties()
{
  return properties;
}

bool WindowFilter::isObjectMatchFilter(const SearchCriteriaData& data, rengaapi::ModelObject* pObject)
{
  rengaapi::Window* pWindow = dynamic_cast<rengaapi::Window*>(pObject);
  assert(pWindow != nullptr);
  switch (data.m_property.m_propertyType)
  {
  case PropertyType::LevelHeight:
    return apply(pWindow->levelId(), data);
  case PropertyType::Offset:
    return apply(pWindow->offset(), data);
  case PropertyType::Height:
    return apply(pWindow->quantities().overallHeight(), data);
  case PropertyType::Width:
    return apply(pWindow->quantities().overallWidth(), data);
  case PropertyType::Perimeter:
    return apply(pWindow->quantities().perimeter(), data);
  case PropertyType::Area:
    return apply(pWindow->quantities().area(), data);
  case PropertyType::Name:
    return apply(pWindow->name(), data);
  case PropertyType::Mark:
    return apply(pWindow->mark(), data);
  case PropertyType::UserAttribute:
    return isUserAttributeMatchFilter(pObject, data);
  default:
    assert(false);
    return false;
  }
}
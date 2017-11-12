//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "DoorFilterData.h"

#include <RengaAPI/Door.h>

DoorProperty::DoorProperty()
  : ObjectProperty()
{
  properties = { Property(PropertyType::Name, ValueType::String, QApplication::translate("mf_door", "name")),
    Property(PropertyType::Mark, ValueType::String, QApplication::translate("mf_door", "mark")),
    Property(PropertyType::LevelHeight, ValueType::String, QApplication::translate("mf_door", "level")),
    Property(PropertyType::Offset, ValueType::Double, QApplication::translate("mf_door", "offset")),
    Property(PropertyType::Height, ValueType::Double, QApplication::translate("mf_door", "overallHeight")),
    Property(PropertyType::Width, ValueType::Double, QApplication::translate("mf_door", "overallWidth")),
    Property(PropertyType::Perimeter, ValueType::Double, QApplication::translate("mf_door", "perimeter")),
    Property(PropertyType::Area, ValueType::Double, QApplication::translate("mf_door", "area")) };
}

PropertyList DoorProperty::getObjectProperties()
{
  return properties;
}

bool DoorFilter::isObjectMatchFilter(const SearchCriteriaData& data, rengaapi::ModelObject* pObject)
{
  rengaapi::Door* pDoor = dynamic_cast<rengaapi::Door*>(pObject);
  assert(pDoor != nullptr);
  switch (data.m_property.m_propertyType)
  {
  case PropertyType::LevelHeight:
    return apply(pDoor->levelId(), data);
  case PropertyType::Offset:
    return apply(pDoor->offset(), data);
  case PropertyType::Height:
    return apply(pDoor->quantities().overallHeight(), data);
  case PropertyType::Width:
    return apply(pDoor->quantities().overallWidth(), data);
  case PropertyType::Perimeter:
    return apply(pDoor->quantities().perimeter(), data);
  case PropertyType::Area:
    return apply(pDoor->quantities().area(), data);
  case PropertyType::Name:
    return apply(pDoor->name(), data);
  case PropertyType::Mark:
    return apply(pDoor->mark(), data);
  case PropertyType::UserAttribute:
    return isUserAttributeMatchFilter(pObject, data);
  default:
    assert(false);
    return false;
  }
}
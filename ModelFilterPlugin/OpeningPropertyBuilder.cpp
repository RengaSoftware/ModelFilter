//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "OpeningPropertyBuilder.h"

#include <RengaAPI/Opening.h>

OpeningPropertyBuilder::OpeningPropertyBuilder()
  : ObjectPropertyBuilder()
{
  properties = { Property(PropertyType::Name, ValueType::String, QApplication::translate("opening", "name")),
    Property(PropertyType::Mark, ValueType::String, QApplication::translate("opening", "mark")),
    Property(PropertyType::LevelHeight, ValueType::Double, QApplication::translate("opening", "level")),
    Property(PropertyType::Offset, ValueType::Double, QApplication::translate("opening", "offset")),
    Property(PropertyType::Thickness, ValueType::Double, QApplication::translate("opening", "thickness")),
    Property(PropertyType::Volume, ValueType::Double, QApplication::translate("opening", "volume")),
    Property(PropertyType::Perimeter, ValueType::Double, QApplication::translate("opening", "perimeter")),
    Property(PropertyType::Area , ValueType::Double, QApplication::translate("opening", "area")) };
}

PropertyList OpeningPropertyBuilder::getObjectProperties()
{
  return properties;
}

bool OpeningPropertyBuilder::isObjectMatchFilter(const SearchCriteriaData& data, rengaapi::ModelObject* pObject)
{
  rengaapi::Opening* pOpening = dynamic_cast<rengaapi::Opening*>(pObject);
  assert(pOpening != nullptr);
  switch (data.m_property.m_propertyType)
  {
  case PropertyType::LevelHeight:
    return apply(pOpening->levelId(), data);
  case PropertyType::Offset:
    return apply(pOpening->offset(), data);
  case PropertyType::Thickness:
    return apply(pOpening->quantities().nominalThickness(), data);
  case PropertyType::Volume:
    return apply(pOpening->quantities().volume(), data);
  case PropertyType::Perimeter:
    return apply(pOpening->quantities().perimeter(), data);
  case PropertyType::Area:
    return apply(pOpening->quantities().area(), data);
  case PropertyType::Name:
    return apply(pOpening->name(), data);
  case PropertyType::Mark:
    return apply(pOpening->mark(), data);
  case PropertyType::UserAttribute:
    return isUserAttributeMatchFilter(pObject, data);
  default:
    assert(false);
    return false;
  }
}
//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "OpeningFilterData.h"

#include <RengaAPI/Opening.h>

OpeningProperty::OpeningProperty()
  : ObjectProperty()
{
  properties = { Property(PropertyType::Name, ValueType::String, QApplication::translate("mf_opening", "name")),
    Property(PropertyType::Mark, ValueType::String, QApplication::translate("mf_opening", "mark")),
    Property(PropertyType::LevelHeight, ValueType::String, QApplication::translate("mf_opening", "level")),
    Property(PropertyType::Offset, ValueType::Double, QApplication::translate("mf_opening", "offset")),
    Property(PropertyType::Thickness, ValueType::Double, QApplication::translate("mf_opening", "thickness")),
    Property(PropertyType::Volume, ValueType::Double, QApplication::translate("mf_opening", "volume")),
    Property(PropertyType::Perimeter, ValueType::Double, QApplication::translate("mf_opening", "perimeter")),
    Property(PropertyType::Area , ValueType::Double, QApplication::translate("mf_opening", "area")) };
}

PropertyList OpeningProperty::getObjectProperties()
{
  return properties;
}

bool OpeningFilter::isObjectMatchFilter(const SearchCriteriaData& data, rengaapi::ModelObject* pObject)
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
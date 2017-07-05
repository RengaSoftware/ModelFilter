//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "RailingFilterData.h"

#include <RengaAPI/Railing.h>

RailingProperty::RailingProperty()
  : ObjectProperty()
{
  properties = { Property(PropertyType::Name, ValueType::String, QApplication::translate("railing", "name")),
    Property(PropertyType::Mark, ValueType::String, QApplication::translate("railing", "mark")),
    Property(PropertyType::LevelHeight, ValueType::String, QApplication::translate("railing", "level")),
    Property(PropertyType::Offset, ValueType::Double, QApplication::translate("railing", "offset")),
    Property(PropertyType::Height, ValueType::Double, QApplication::translate("railing", "height")),
    Property(PropertyType::Length, ValueType::Double, QApplication::translate("railing", "length")) };

}

PropertyList RailingProperty::getObjectProperties()
{
  return properties;
}

bool RailingFilter::isObjectMatchFilter(const SearchCriteriaData& data, rengaapi::ModelObject* pObject)
{
  rengaapi::Railing* pRailing = dynamic_cast<rengaapi::Railing*>(pObject);
  assert(pRailing != nullptr);
  switch (data.m_property.m_propertyType)
  {
  case PropertyType::LevelHeight:
    return apply(pRailing->levelId(), data);
  case PropertyType::Offset:
    return apply(pRailing->offset(), data);
  case PropertyType::Height:
    return apply(pRailing->quantities().nominalHeight(), data);
  case PropertyType::Length:
    return apply(pRailing->quantities().nominalLength(), data);
  case PropertyType::Name:
    return apply(pRailing->name(), data);
  case PropertyType::Mark:
    return apply(pRailing->mark(), data);
  case PropertyType::UserAttribute:
    return isUserAttributeMatchFilter(pObject, data);
  default:
    assert(false);
    return false;
  }
}
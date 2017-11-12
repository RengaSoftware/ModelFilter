//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "RampFilterData.h"

#include <RengaAPI/Ramp.h>

RampProperty::RampProperty()
  : ObjectProperty()
{
  properties = { Property(PropertyType::Name, ValueType::String, QApplication::translate("mf_ramp", "name")),
    Property(PropertyType::Material, ValueType::String, QApplication::translate("mf_ramp", "material")),
    Property(PropertyType::Mark, ValueType::String, QApplication::translate("mf_ramp", "mark")),
    Property(PropertyType::LevelHeight, ValueType::String, QApplication::translate("mf_ramp", "level")),
    Property(PropertyType::Offset, ValueType::Double, QApplication::translate("mf_ramp", "offset")),
    Property(PropertyType::Height, ValueType::Double, QApplication::translate("mf_ramp", "height")),
    Property(PropertyType::Width, ValueType::Double, QApplication::translate("mf_ramp", "width")),
    Property(PropertyType::Thickness, ValueType::Double, QApplication::translate("mf_ramp", "thickness")),
    Property(PropertyType::Volume, ValueType::Double, QApplication::translate("mf_ramp", "netVolume")),
    Property(PropertyType::Area, ValueType::Double, QApplication::translate("mf_ramp", "netArea")),
    Property(PropertyType::FloorArea, ValueType::Double, QApplication::translate("mf_ramp", "netFloorArea")),
    Property(PropertyType::Mass, ValueType::Double, QApplication::translate("mf_ramp", "mass")) };
}

PropertyList RampProperty::getObjectProperties()
{
  return properties;
}


bool RampFilter::isObjectMatchFilter(const SearchCriteriaData& data, rengaapi::ModelObject* pObject)
{
  rengaapi::Ramp* pRamp = dynamic_cast<rengaapi::Ramp*>(pObject);
  assert(pRamp != nullptr);
  switch (data.m_property.m_propertyType)
  {
  case PropertyType::LevelHeight:
    return apply(pRamp->levelId(), data);
  case PropertyType::Offset:
    return apply(pRamp->offset(), data);
  case PropertyType::Height:
    return apply(pRamp->quantities().nominalHeight(), data);
  case PropertyType::Width:
    return apply(pRamp->quantities().nominalWidth(), data);
  case PropertyType::Thickness:
    return apply(pRamp->quantities().nominalThickness(), data);
  case PropertyType::Volume:
    return apply(pRamp->quantities().netVolume(), data);
  case PropertyType::Area:
    return apply(pRamp->quantities().netArea(), data);
  case PropertyType::FloorArea:
    return apply(pRamp->quantities().netFloorArea(), data);
  case PropertyType::Mass:
    return apply(pRamp->material(), pRamp->quantities().netVolume(), data);
  case PropertyType::Name:
    return apply(pRamp->name(), data);
  case PropertyType::Material:
    return apply(pRamp->material(), data);
  case PropertyType::Mark:
    return apply(pRamp->mark(), data);
  case PropertyType::UserAttribute:
    return isUserAttributeMatchFilter(pObject, data);
  default:
    assert(false);
    return false;
  }
}
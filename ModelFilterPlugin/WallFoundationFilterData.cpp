//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "WallFoundationFilterData.h"

#include <RengaAPI/WallFoundation.h>

WallFoundationProperty::WallFoundationProperty()
  : ObjectProperty()
{
  properties = { Property(PropertyType::Name, ValueType::String, QApplication::translate("mf_wallFoundation", "name")),
    Property(PropertyType::Material, ValueType::String, QApplication::translate("mf_wallFoundation", "material")),
    Property(PropertyType::Mark, ValueType::String, QApplication::translate("mf_wallFoundation", "mark")),
    Property(PropertyType::LevelHeight, ValueType::String, QApplication::translate("mf_wallFoundation", "level")),
    Property(PropertyType::Offset, ValueType::Double, QApplication::translate("mf_wallFoundation", "offset")),
    Property(PropertyType::Width, ValueType::Double, QApplication::translate("mf_wallFoundation", "overallWidth")),
    Property(PropertyType::Height, ValueType::Double, QApplication::translate("mf_wallFoundation", "overallHeight")),
    Property(PropertyType::Length, ValueType::Double, QApplication::translate("mf_wallFoundation", "length")),
    Property(PropertyType::FootprintArea, ValueType::Double, QApplication::translate("mf_wallFoundation", "netFootprintArea")),
    Property(PropertyType::SideArea, ValueType::Double, QApplication::translate("mf_wallFoundation", "netSideArea")),
    Property(PropertyType::Volume, ValueType::Double, QApplication::translate("mf_wallFoundation", "netVolume")),
    Property(PropertyType::Mass, ValueType::Double, QApplication::translate("mf_wallFoundation", "mass")) };
}

PropertyList WallFoundationProperty::getObjectProperties()
{
  return properties;
}

bool WallFoundationFilter::isObjectMatchFilter(const SearchCriteriaData& data, rengaapi::ModelObject* pObject)
{
  rengaapi::WallFoundation* pWallFoundation = dynamic_cast<rengaapi::WallFoundation*>(pObject);
  assert(pWallFoundation != nullptr);
  switch (data.m_property.m_propertyType)
  {
  case PropertyType::LevelHeight:
    return apply(pWallFoundation->levelId(), data);
  case PropertyType::Offset:
    return apply(pWallFoundation->offset(), data);
  case PropertyType::Width:
    return apply(pWallFoundation->quantities().overallWidth(), data);
  case PropertyType::Height:
    return apply(pWallFoundation->quantities().overallHeight(), data);
  case PropertyType::Length:
    return apply(pWallFoundation->quantities().nominalLength(), data);
  case PropertyType::FootprintArea:
    return apply(pWallFoundation->quantities().netFootprintArea(), data);
  case PropertyType::SideArea:
    return apply(pWallFoundation->quantities().netSideArea(), data);
  case PropertyType::Volume:
    return apply(pWallFoundation->quantities().netVolume(), data);
  case PropertyType::Mass:
    return apply(pWallFoundation->material(), pWallFoundation->quantities().netVolume(), data);
  case PropertyType::Name:
    return apply(pWallFoundation->name(), data);
  case PropertyType::Material:
    return apply(pWallFoundation->material(), data);
  case PropertyType::Mark:
    return apply(pWallFoundation->mark(), data);
  case PropertyType::UserAttribute:
    return isUserAttributeMatchFilter(pObject, data);
  default:
    assert(false);
    return false;
  }
}
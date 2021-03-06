//
// Copyright �Renga Software� LLC, 2017. All rights reserved.
//
// �Renga Software� LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// �Renga Software� LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "ColumnFilterData.h"

#include <RengaAPI/Column.h>

ColumnProperty::ColumnProperty()
  : ObjectProperty()
{
  properties = { Property(PropertyType::Name, ValueType::String, QApplication::translate("mf_column", "name")),
    Property(PropertyType::Material, ValueType::String, QApplication::translate("mf_column", "material")),
    Property(PropertyType::Mark, ValueType::String, QApplication::translate("mf_column", "mark")),
    Property(PropertyType::LevelHeight, ValueType::String, QApplication::translate("mf_column", "level")),
    Property(PropertyType::Offset, ValueType::Double, QApplication::translate("mf_column", "offset")),
    Property(PropertyType::Length, ValueType::Double, QApplication::translate("mf_column", "length")),
    Property(PropertyType::Volume, ValueType::Double, QApplication::translate("mf_column", "netVolume")),
    Property(PropertyType::CrossSectionArea, ValueType::Double, QApplication::translate("mf_column", "crossSectionArea")),
    Property(PropertyType::OuterSurfaceArea, ValueType::Double, QApplication::translate("mf_column", "outerSurfaceArea")),
    Property(PropertyType::Perimeter, ValueType::Double, QApplication::translate("mf_column", "perimeter")),
    Property(PropertyType::Mass, ValueType::Double, QApplication::translate("mf_column", "mass")) };
}

PropertyList ColumnProperty::getObjectProperties()
{
  return properties;
}

bool ColumnFilter::isObjectMatchFilter(const SearchCriteriaData& data, rengaapi::ModelObject* pObject)
{
  rengaapi::Column* pColumn = dynamic_cast<rengaapi::Column*>(pObject);
  assert(pColumn != nullptr);
  switch (data.m_property.m_propertyType)
  {
  case PropertyType::LevelHeight:
    return apply(pColumn->levelId(), data);
  case PropertyType::Offset:
    return apply(pColumn->offset(), data);
  case PropertyType::Length:
    return apply(pColumn->quantities().nominalLength(), data);
  case PropertyType::Volume:
    return apply(pColumn->quantities().netVolume(), data);
  case PropertyType::CrossSectionArea:
    return apply(pColumn->quantities().crossSectionArea(), data);
  case PropertyType::OuterSurfaceArea:
    return apply(pColumn->quantities().outerSurfaceArea(), data);
  case PropertyType::Perimeter:
    return apply(pColumn->quantities().perimeter(), data);
  case PropertyType::Mass:
    return apply(pColumn->material(), pColumn->quantities().netVolume(), data);
  case PropertyType::Name:
    return apply(pColumn->name(), data);
  case PropertyType::Material:
    return apply(pColumn->material(), data);
  case PropertyType::Mark:
    return apply(pColumn->mark(), data);
  case PropertyType::UserAttribute:
    return isUserAttributeMatchFilter(pObject, data);
  default:
    assert(false);
    return false;
  }
}
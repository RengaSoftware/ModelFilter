//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "StairPropertyBuilder.h"

#include <RengaAPI/Stair.h>

StairPropertyBuilder::StairPropertyBuilder()
  : ObjectPropertyBuilder()
{
  properties = { Property(PropertyType::Name, ValueType::String, QApplication::translate("stair", "name")),
    Property(PropertyType::Material, ValueType::String, QApplication::translate("stair", "material")),
    Property(PropertyType::Mark, ValueType::String, QApplication::translate("stair", "mark")),
    Property(PropertyType::LevelHeight, ValueType::Double, QApplication::translate("stair", "level")),
    Property(PropertyType::Offset, ValueType::Double, QApplication::translate("stair", "offset")),
    Property(PropertyType::Height, ValueType::Double, QApplication::translate("stair", "height")),
    Property(PropertyType::Width, ValueType::Double, QApplication::translate("stair", "width")),
    Property(PropertyType::Volume, ValueType::Double, QApplication::translate("stair", "netVolume")),
    Property(PropertyType::NumberOfRiser, ValueType::Double, QApplication::translate("stair", "numberOfRiser")),
    Property(PropertyType::NumberOfTreads, ValueType::Double, QApplication::translate("stair", "numberOfTreads")),
    Property(PropertyType::RiserHeight, ValueType::Double, QApplication::translate("stair", "riserHeight")),
    Property(PropertyType::TreadLength, ValueType::Double, QApplication::translate("stair", "treadLength")),
    Property(PropertyType::Area, ValueType::Double, QApplication::translate("stair", "netArea")),
    Property(PropertyType::FloorArea, ValueType::Double, QApplication::translate("stair", "netFloorArea")),
    Property(PropertyType::Mass, ValueType::Double, QApplication::translate("stair", "mass")) };
}

PropertyList StairPropertyBuilder::getObjectProperties()
{
  return properties;
}

bool StairPropertyBuilder::isObjectMatchFilter(const SearchÑriteriaData& data, rengaapi::ModelObject* pObject)
{
  rengaapi::Stair* pStair = dynamic_cast<rengaapi::Stair*>(pObject);
  assert(pStair != nullptr);
  switch (data.m_property.m_propertyType)
  {
  case PropertyType::LevelHeight:
    return apply(pStair->levelId(), data);
  case PropertyType::Offset:
    return apply(pStair->offset(), data);
  case PropertyType::Height:
    return apply(pStair->quantities().nominalHeight(), data);
  case PropertyType::Width:
    return apply(pStair->quantities().nominalWidth(), data);
  case PropertyType::Volume:
    return apply(pStair->quantities().netVolume(), data);
  case PropertyType::NumberOfRiser:
    return apply(pStair->quantities().numberOfRiser(), data);
  case PropertyType::NumberOfTreads:
    return apply(pStair->quantities().numberOfTreads(), data);
  case PropertyType::RiserHeight:
    return apply(pStair->quantities().riserHeight(), data);
  case PropertyType::TreadLength:
    return apply(pStair->quantities().treadLength(), data);
  case PropertyType::Area:
    return apply(pStair->quantities().netArea(), data);
  case PropertyType::FloorArea:
    return apply(pStair->quantities().netFloorArea(), data);
  case PropertyType::Mass:
    return apply(pStair->material(), pStair->quantities().netVolume(), data);
  case PropertyType::Name:
    return apply(pStair->name(), data);
  case PropertyType::Material:
    return apply(pStair->material(), data);
  case PropertyType::Mark:
    return apply(pStair->mark(), data);
  case PropertyType::UserAttribute:
    return isUserAttributeMatchFilter(pObject, data);
  default:
    assert(false);
    return false;
  }
}
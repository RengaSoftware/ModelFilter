//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "BeamPropertyBuilder.h"

#include <RengaAPI/Beam.h>


BeamPropertyBuilder::BeamPropertyBuilder()
  : ObjectPropertyBuilder()
{
  properties = { Property(PropertyType::Name, ValueType::String, QApplication::translate("beam", "name")),
    Property(PropertyType::Material, ValueType::String, QApplication::translate("beam", "material")),
    Property(PropertyType::Mark, ValueType::String, QApplication::translate("beam", "mark")),
    Property(PropertyType::LevelHeight, ValueType::Double, QApplication::translate("beam", "level")),
    Property(PropertyType::Offset, ValueType::Double, QApplication::translate("beam", "offset")),
    Property(PropertyType::Length, ValueType::Double, QApplication::translate("beam", "length")),
    Property(PropertyType::Volume, ValueType::Double, QApplication::translate("beam", "netVolume")),
    Property(PropertyType::CrossSectionArea, ValueType::Double, QApplication::translate("beam", "crossSectionArea")),
    Property(PropertyType::OuterSurfaceArea, ValueType::Double, QApplication::translate("beam", "outerSurfaceArea")),
    Property(PropertyType::Perimeter, ValueType::Double, QApplication::translate("beam", "perimeter")),
    Property(PropertyType::Mass, ValueType::Double, QApplication::translate("beam", "mass")) };

}

PropertyList BeamPropertyBuilder::getObjectProperties()
{
  return properties;
}

bool BeamPropertyBuilder::isObjectMatchFilter(const SearchCriteriaData& data, rengaapi::ModelObject* pObject)
{
  rengaapi::Beam* pBeam = dynamic_cast<rengaapi::Beam*>(pObject);
  assert(pBeam != nullptr);
  switch (data.m_property.m_propertyType)
  {
  case PropertyType::LevelHeight:
    return apply(pBeam->levelId(), data);
  case PropertyType::Offset:
    return apply(pBeam->offset(), data);
  case PropertyType::Length:
    return apply(pBeam->quantities().nominalLength(), data);
  case PropertyType::Volume:
    return apply(pBeam->quantities().netVolume(), data);
  case PropertyType::CrossSectionArea:
    return apply(pBeam->quantities().crossSectionArea(), data, MeasureUnit::Centimeter);
  case PropertyType::OuterSurfaceArea:
    return apply(pBeam->quantities().outerSurfaceArea(), data);
  case PropertyType::Perimeter:
    return apply(pBeam->quantities().perimeter(), data);
  case PropertyType::Mass:
    return apply(pBeam->material(), pBeam->quantities().netVolume(), data);
  case PropertyType::Name:
    return apply(pBeam->name(), data);
  case PropertyType::Material:
    return apply(pBeam->material(), data);
  case PropertyType::Mark:
    return apply(pBeam->mark(), data);
  case PropertyType::UserAttribute:
    return isUserAttributeMatchFilter(pObject, data);
  default:
    assert(false);
    return false;
  }
}
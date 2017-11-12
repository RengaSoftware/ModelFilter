//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "BeamFilterData.h"

#include <RengaAPI/Beam.h>


BeamProperty::BeamProperty()
  : ObjectProperty()
{
  properties = { Property(PropertyType::Name, ValueType::String, QApplication::translate("mf_beam", "name")),
    Property(PropertyType::Material, ValueType::String, QApplication::translate("mf_beam", "material")),
    Property(PropertyType::Mark, ValueType::String, QApplication::translate("mf_beam", "mark")),
    Property(PropertyType::LevelHeight, ValueType::String, QApplication::translate("mf_beam", "level")),
    Property(PropertyType::Offset, ValueType::Double, QApplication::translate("mf_beam", "offset")),
    Property(PropertyType::Length, ValueType::Double, QApplication::translate("mf_beam", "length")),
    Property(PropertyType::Volume, ValueType::Double, QApplication::translate("mf_beam", "netVolume")),
    Property(PropertyType::CrossSectionArea, ValueType::Double, QApplication::translate("mf_beam", "crossSectionArea")),
    Property(PropertyType::OuterSurfaceArea, ValueType::Double, QApplication::translate("mf_beam", "outerSurfaceArea")),
    Property(PropertyType::Perimeter, ValueType::Double, QApplication::translate("mf_beam", "perimeter")),
    Property(PropertyType::Mass, ValueType::Double, QApplication::translate("mf_beam", "mass")) };

}

PropertyList BeamProperty::getObjectProperties()
{
  return properties;
}

bool BeamFilter::isObjectMatchFilter(const SearchCriteriaData& data, rengaapi::ModelObject* pObject)
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
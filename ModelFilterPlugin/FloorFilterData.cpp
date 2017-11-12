//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "FloorFilterData.h"

#include <RengaAPI/Floor.h>

FloorProperty::FloorProperty()
  : ObjectProperty()
{
  properties = { Property(PropertyType::Name, ValueType::String, QApplication::translate("mf_floor", "name")),
    Property(PropertyType::Material, ValueType::String, QApplication::translate("mf_floor", "material")),
    Property(PropertyType::Mark, ValueType::String, QApplication::translate("mf_floor", "mark")),
    Property(PropertyType::LevelHeight, ValueType::String, QApplication::translate("mf_floor", "level")),
    Property(PropertyType::Offset, ValueType::Double, QApplication::translate("mf_floor", "offset")),
    Property(PropertyType::Thickness, ValueType::Double, QApplication::translate("mf_floor", "thickness")),
    Property(PropertyType::Volume, ValueType::Double, QApplication::translate("mf_floor", "netVolume")),
    Property(PropertyType::Perimeter, ValueType::Double, QApplication::translate("mf_floor", "perimeter")),
    Property(PropertyType::Area, ValueType::Double, QApplication::translate("mf_floor", "netArea")),
    Property(PropertyType::Mass, ValueType::Double, QApplication::translate("mf_floor", "mass")) };
}

PropertyList FloorProperty::getObjectProperties()
{
  return properties;
}
bool FloorFilter::isObjectMatchFilter(const SearchCriteriaData& data, rengaapi::ModelObject* pObject)
{
  rengaapi::Floor* pFloor = dynamic_cast<rengaapi::Floor*>(pObject);
  assert(pFloor != nullptr);
  switch (data.m_property.m_propertyType)
  {
  case PropertyType::LevelHeight:
    return apply(pFloor->levelId(), data);
  case PropertyType::Offset:
    return apply(pFloor->offset(), data);
  case PropertyType::Thickness:
    return apply(pFloor->quantities().nominalThickness(), data);
  case PropertyType::Volume:
    return apply(pFloor->quantities().netVolume(), data);
  case PropertyType::Perimeter:
    return apply(pFloor->quantities().perimeter(), data);
  case PropertyType::Area:
    return apply(pFloor->quantities().netArea(), data);
  case PropertyType::Mass:
  {
    rengaapi::FloorQuantities floorQuantities = pFloor->quantities();
    rengaapi::FloorMaterialLayerQuantityCollection floorMaterialLayerQuantityCollection = floorQuantities.materialQuantities();
    std::vector<rengabase::VolumeMeasureOptional> volumeCollection;
    for (size_t i = 0; i < floorMaterialLayerQuantityCollection.size(); ++i)
      volumeCollection.push_back(floorMaterialLayerQuantityCollection.get(i).netVolume());
    return apply(pFloor->material(), volumeCollection, data);
  }
  case PropertyType::Name:
    return apply(pFloor->name(), data);
  case PropertyType::Material:
    return apply(pFloor->material(), data);
  case PropertyType::Mark:
    return apply(pFloor->mark(), data);
  case PropertyType::UserAttribute:
    return isUserAttributeMatchFilter(pObject, data);
  default:
    assert(false);
    return false;
  }
}
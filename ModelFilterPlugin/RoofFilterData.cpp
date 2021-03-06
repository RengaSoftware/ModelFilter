//
// Copyright �Renga Software� LLC, 2017. All rights reserved.
//
// �Renga Software� LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// �Renga Software� LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "RoofFilterData.h"

#include <RengaAPI/Roof.h>

RoofProperty::RoofProperty()
  : ObjectProperty()
{
  properties = { Property(PropertyType::Name, ValueType::String, QApplication::translate("mf_roof", "name")),
    Property(PropertyType::Material, ValueType::String, QApplication::translate("mf_roof", "material")),
    Property(PropertyType::Mark, ValueType::String, QApplication::translate("mf_roof", "mark")),
    Property(PropertyType::LevelHeight, ValueType::String, QApplication::translate("mf_roof", "level")),
    Property(PropertyType::Offset, ValueType::Double, QApplication::translate("mf_roof", "offset")),
    Property(PropertyType::Thickness, ValueType::Double, QApplication::translate("mf_roof", "thickness")),
    Property(PropertyType::Volume, ValueType::Double, QApplication::translate("mf_roof", "netVolume")),
    Property(PropertyType::TotalSurfaceArea, ValueType::Double, QApplication::translate("mf_roof", "totalSurfaceArea")),
    Property(PropertyType::Mass, ValueType::Double, QApplication::translate("mf_roof", "mass")) };
}

PropertyList RoofProperty::getObjectProperties()
{
  return properties;
}

bool RoofFilter::isObjectMatchFilter(const SearchCriteriaData& data, rengaapi::ModelObject* pObject)
{
  rengaapi::Roof* pRoof = dynamic_cast<rengaapi::Roof*>(pObject);
  assert(pRoof != nullptr);
  switch (data.m_property.m_propertyType)
  {
  case PropertyType::LevelHeight:
    return apply(pRoof->levelId(), data);
  case PropertyType::Offset:
    return apply(pRoof->offset(), data);
  case PropertyType::Thickness:
    return apply(pRoof->quantities().nominalThickness(), data);
  case PropertyType::Volume:
    return apply(pRoof->quantities().netVolume(), data);
  case PropertyType::TotalSurfaceArea:
    return apply(pRoof->quantities().totalSurfaceArea(), data);
  case PropertyType::Mass:
  {
    rengaapi::RoofQuantities roofQuantities = pRoof->quantities();
    rengaapi::RoofMaterialLayerQuantityCollection roofMaterialLayerQuantityCollection = roofQuantities.materialQuantities();
    std::vector<rengabase::VolumeMeasureOptional> volumeCollection;
    for (size_t i = 0; i < roofMaterialLayerQuantityCollection.size(); ++i)
      volumeCollection.push_back(roofMaterialLayerQuantityCollection.get(i).netVolume());
    return apply(pRoof->material(), volumeCollection, data);
  }
  case PropertyType::Name:
    return apply(pRoof->name(), data);
  case PropertyType::Material:
    return apply(pRoof->material(), data);
  case PropertyType::Mark:
    return apply(pRoof->mark(), data);
  case PropertyType::UserAttribute:
    return isUserAttributeMatchFilter(pObject, data);
  default:
    assert(false);
    return false;
  }
}
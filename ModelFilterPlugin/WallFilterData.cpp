//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "WallFilterData.h"

#include <RengaAPI/Wall.h>

WallProperty::WallProperty()
  : ObjectProperty()
{
  properties = { Property(PropertyType::Name, ValueType::String, QApplication::translate("mf_wall", "name")),
    Property(PropertyType::Material, ValueType::String, QApplication::translate("mf_wall", "material")),
    Property(PropertyType::Mark, ValueType::String, QApplication::translate("mf_wall", "mark")),
    Property(PropertyType::LevelHeight, ValueType::String, QApplication::translate("mf_wall", "level")),
    Property(PropertyType::Offset, ValueType::Double, QApplication::translate("mf_wall", "offset")),
    Property(PropertyType::Length, ValueType::Double, QApplication::translate("mf_wall", "length")),
    Property(PropertyType::Thickness, ValueType::Double, QApplication::translate("mf_wall", "thickness")),
    Property(PropertyType::Height, ValueType::Double, QApplication::translate("mf_wall", "height")),
    Property(PropertyType::Volume, ValueType::Double, QApplication::translate("mf_wall", "netVolume")),
    Property(PropertyType::FootprintArea, ValueType::Double, QApplication::translate("mf_wall", "netFootprintArea")),
    Property(PropertyType::SideArea, ValueType::Double, QApplication::translate("mf_wall", "netSideArea")),
    Property(PropertyType::Mass, ValueType::Double, QApplication::translate("mf_wall", "mass")) };
}

PropertyList WallProperty::getObjectProperties()
{
  return properties;
}

bool WallFilter::isObjectMatchFilter(const SearchCriteriaData& data, rengaapi::ModelObject* pObject)
{
  rengaapi::Wall* pWall = dynamic_cast<rengaapi::Wall*>(pObject);
  assert(pWall != nullptr);
  switch (data.m_property.m_propertyType)
  {
  case PropertyType::LevelHeight:
    return apply(pWall->levelId(), data);
  case PropertyType::Offset:
    return apply(pWall->offset(), data);
  case PropertyType::Length:
    return apply(pWall->quantities().nominalLength(), data);
  case PropertyType::Thickness:
    return apply(pWall->quantities().nominalThickness(), data);
  case PropertyType::Height:
    return apply(pWall->quantities().nominalHeight(), data);
  case PropertyType::Volume:
    return apply(pWall->quantities().netVolume(), data);
  case PropertyType::FootprintArea:
    return apply(pWall->quantities().netFootprintArea(), data);
  case PropertyType::SideArea:
    return apply(pWall->quantities().netSideArea(), data);
  case PropertyType::Mass:
  {
    rengaapi::WallQuantities wallQuantities = pWall->quantities();
    rengaapi::WallMaterialLayerQuantityCollection wallMaterialLayerQuantityCollection = wallQuantities.materialQuantities();
    std::vector<rengabase::VolumeMeasureOptional> volumeCollection;
    for (size_t i = 0; i < wallMaterialLayerQuantityCollection.size(); ++i)
      volumeCollection.push_back(wallMaterialLayerQuantityCollection.get(i).netVolume());
    return apply(pWall->material(), volumeCollection, data);
  }
  case PropertyType::Name:
    return apply(pWall->name(), data);
  case PropertyType::Material:
    return apply(pWall->material(), data);
  case PropertyType::Mark:
    return apply(pWall->mark(), data);
  case PropertyType::UserAttribute:
    return isUserAttributeMatchFilter(pObject, data);
  default:
    assert(false);
    return false;
  }
}
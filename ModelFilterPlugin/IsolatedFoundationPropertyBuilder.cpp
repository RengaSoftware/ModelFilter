//
// Copyright �Renga Software� LLC, 2017. All rights reserved.
//
// �Renga Software� LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// �Renga Software� LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "IsolatedFoundationPropertyBuilder.h"

#include <RengaAPI/IsolatedFoundation.h>

IsolatedFoundationPropertyBuilder::IsolatedFoundationPropertyBuilder()
  : ObjectPropertyBuilder()
{
  properties = { Property(PropertyType::Name, ValueType::String, QApplication::translate("isolatedFoundation", "name")),
    Property(PropertyType::Material, ValueType::String, QApplication::translate("isolatedFoundation", "material")),
    Property(PropertyType::Mark, ValueType::String, QApplication::translate("isolatedFoundation", "mark")),
    Property(PropertyType::LevelHeight, ValueType::Double, QApplication::translate("isolatedFoundation", "level")),
    Property(PropertyType::Offset, ValueType::Double, QApplication::translate("isolatedFoundation", "offset")),
    Property(PropertyType::Width, ValueType::Double, QApplication::translate("isolatedFoundation", "overallWidth")),
    Property(PropertyType::Depth, ValueType::Double, QApplication::translate("isolatedFoundation", "overallDepth")),
    Property(PropertyType::Height, ValueType::Double, QApplication::translate("isolatedFoundation", "overallHeight")),
    Property(PropertyType::FootprintArea, ValueType::Double, QApplication::translate("isolatedFoundation", "netFootprintArea")),
    Property(PropertyType::Volume, ValueType::Double, QApplication::translate("isolatedFoundation", "netVolume")),
    Property(PropertyType::Mass, ValueType::Double, QApplication::translate("isolatedFoundation", "mass")) };
}

PropertyList IsolatedFoundationPropertyBuilder::getObjectProperties()
{
  return properties;
}

bool IsolatedFoundationPropertyBuilder::isObjectMatchFilter(const SearchCriteriaData& data, rengaapi::ModelObject* pObject)
{
  rengaapi::IsolatedFoundation* pIsolatedFoundation = dynamic_cast<rengaapi::IsolatedFoundation*>(pObject);
  assert(pIsolatedFoundation != nullptr);
  switch (data.m_property.m_propertyType)
  {
  case PropertyType::LevelHeight:
    return apply(pIsolatedFoundation->levelId(), data);
  case PropertyType::Offset:
    return apply(pIsolatedFoundation->offset(), data);
  case PropertyType::Width:
    return apply(pIsolatedFoundation->quantities().overallWidth(), data);
  case PropertyType::Depth:
    return apply(pIsolatedFoundation->quantities().overallDepth(), data);
  case PropertyType::Height:
    return apply(pIsolatedFoundation->quantities().overallHeight(), data);
  case PropertyType::FootprintArea:
    return apply(pIsolatedFoundation->quantities().netFootprintArea(), data);
  case PropertyType::Volume:
    return apply(pIsolatedFoundation->quantities().netVolume(), data);
  case PropertyType::Mass:
    return apply(pIsolatedFoundation->material(), pIsolatedFoundation->quantities().netVolume(), data);
  case PropertyType::Name:
    return apply(pIsolatedFoundation->name(), data);
  case PropertyType::Material:
    return apply(pIsolatedFoundation->material(), data);
  case PropertyType::Mark:
    return apply(pIsolatedFoundation->mark(), data);
  case PropertyType::UserAttribute:
    return isUserAttributeMatchFilter(pObject, data);
  default:
    assert(false);
    return false;
  }
}
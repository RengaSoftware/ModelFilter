//
// Copyright �Renga Software� LLC, 2017. All rights reserved.
//
// �Renga Software� LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// �Renga Software� LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "RoomFilterData.h"

#include <RengaAPI/Room.h>

RoomProperty::RoomProperty()
  : ObjectProperty()
{
  properties = { Property(PropertyType::Name, ValueType::String, QApplication::translate("mf_room", "name")),
    Property(PropertyType::RoomNumber, ValueType::String, QApplication::translate("mf_room", "roomNumber")),
    Property(PropertyType::LevelHeight, ValueType::String, QApplication::translate("mf_room", "level")),
    Property(PropertyType::Offset, ValueType::Double, QApplication::translate("mf_room", "offset")),
    Property(PropertyType::Height, ValueType::Double, QApplication::translate("mf_room", "nominalHeight")),
    Property(PropertyType::Volume, ValueType::Double, QApplication::translate("mf_room", "grossVolume")),
    Property(PropertyType::FloorArea, ValueType::Double, QApplication::translate("mf_room", "grossFloorArea")),
    Property(PropertyType::Perimeter, ValueType::Double, QApplication::translate("mf_room", "grossPerimeter")),
    Property(PropertyType::RoomName, ValueType::String, QApplication::translate("mf_room", "roomName")) };
}

PropertyList RoomProperty::getObjectProperties()
{
  return properties;
}

bool RoomFilter::isObjectMatchFilter(const SearchCriteriaData& data, rengaapi::ModelObject* pObject)
{
  rengaapi::Room* pRoom = dynamic_cast<rengaapi::Room*>(pObject);
  assert(pRoom != nullptr);
  switch (data.m_property.m_propertyType)
  {
  case PropertyType::LevelHeight:
    return apply(pRoom->levelId(), data);
  case PropertyType::Offset:
    return apply(pRoom->offset(), data);
  case PropertyType::Height:
    return apply(pRoom->quantities().nominalHeight(), data);
  case PropertyType::Volume:
    return apply(pRoom->quantities().grossVolume(), data);
  case PropertyType::FloorArea:
    return apply(pRoom->quantities().grossFloorArea(), data);
  case PropertyType::Perimeter:
    return apply(pRoom->quantities().grossPerimeter(), data);
  case PropertyType::Name:
    return apply(pRoom->name(), data);
  case PropertyType::RoomNumber:
    return apply(pRoom->roomNumber(), data);
  case PropertyType::RoomName:
    return apply(pRoom->roomName(), data);
  case PropertyType::UserAttribute:
    return isUserAttributeMatchFilter(pObject, data);
  default:
    assert(false);
    return false;
  }
}
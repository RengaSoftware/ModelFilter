//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "ObjectFactory.h"

#include "BeamFilterData.h"
#include "ColumnFilterData.h"
#include "DoorFilterData.h"
#include "FloorFilterData.h"
#include "IsolatedFoundationFilterData.h"
#include "LevelFilterData.h"
#include "OpeningFilterData.h"
#include "RailingFilterData.h"
#include "RampFilterData.h"
#include "RoofFilterData.h"
#include "RoomFilterData.h"
#include "StairFilterData.h"
#include "WallFilterData.h"
#include "WallFoundationFilterData.h"
#include "WindowFilterData.h"

#include <RengaAPI/ModelObjectTypes.h>
#include <RengaAPI/Project.h>

ObjectProperty* ObjectPropertyFactory::createObjectProperty(const rengaapi::ObjectType objectType)
{
  ObjectProperty* pResult = nullptr;

  if (objectType == rengaapi::ModelObjectTypes::LevelType)
    pResult = new LevelProperty();
  else if (objectType == rengaapi::ModelObjectTypes::WallType)
    pResult = new WallProperty();
  else if (objectType == rengaapi::ModelObjectTypes::ColumnType)
    pResult = new ColumnProperty();
  else if (objectType == rengaapi::ModelObjectTypes::FloorType)
    pResult = new FloorProperty();
  else if (objectType == rengaapi::ModelObjectTypes::OpeningType)
    pResult = new OpeningProperty();
  else if (objectType == rengaapi::ModelObjectTypes::RoofType)
    pResult = new RoofProperty();
  else if (objectType == rengaapi::ModelObjectTypes::BeamType)
    pResult = new BeamProperty();
  else if (objectType == rengaapi::ModelObjectTypes::StairType)
    pResult = new StairProperty();
  else if (objectType == rengaapi::ModelObjectTypes::RampType)
    pResult = new RampProperty();
  else if (objectType == rengaapi::ModelObjectTypes::WindowType)
    pResult = new WindowProperty();
  else if (objectType == rengaapi::ModelObjectTypes::DoorType)
    pResult = new DoorProperty();
  else if (objectType == rengaapi::ModelObjectTypes::RailingType)
    pResult = new RailingProperty();
  else if (objectType == rengaapi::ModelObjectTypes::RoomType)
    pResult = new RoomProperty();
  else if (objectType == rengaapi::ModelObjectTypes::IsolatedFoundationType)
    pResult = new IsolatedFoundationProperty();
  else if (objectType == rengaapi::ModelObjectTypes::WallFoundationType)
    pResult = new WallFoundationProperty();

  return pResult;
}

ObjectFilter* ObjectFilterFactory::createObjectFilter(const rengaapi::ObjectType objectType)
{
  ObjectFilter* pResult = nullptr;

  if (objectType == rengaapi::ModelObjectTypes::LevelType)
    pResult = new LevelFilter();
  else if (objectType == rengaapi::ModelObjectTypes::WallType)
    pResult = new WallFilter();
  else if (objectType == rengaapi::ModelObjectTypes::ColumnType)
    pResult = new ColumnFilter();
  else if (objectType == rengaapi::ModelObjectTypes::FloorType)
    pResult = new FloorFilter();
  else if (objectType == rengaapi::ModelObjectTypes::OpeningType)
    pResult = new OpeningFilter();
  else if (objectType == rengaapi::ModelObjectTypes::RoofType)
    pResult = new RoofFilter();
  else if (objectType == rengaapi::ModelObjectTypes::BeamType)
    pResult = new BeamFilter();
  else if (objectType == rengaapi::ModelObjectTypes::StairType)
    pResult = new StairFilter();
  else if (objectType == rengaapi::ModelObjectTypes::RampType)
    pResult = new RampFilter();
  else if (objectType == rengaapi::ModelObjectTypes::WindowType)
    pResult = new WindowFilter();
  else if (objectType == rengaapi::ModelObjectTypes::DoorType)
    pResult = new DoorFilter();
  else if (objectType == rengaapi::ModelObjectTypes::RailingType)
    pResult = new RailingFilter();
  else if (objectType == rengaapi::ModelObjectTypes::RoomType)
    pResult = new RoomFilter();
  else if (objectType == rengaapi::ModelObjectTypes::IsolatedFoundationType)
    pResult = new IsolatedFoundationFilter();
  else if (objectType == rengaapi::ModelObjectTypes::WallFoundationType)
    pResult = new WallFoundationFilter();

  return pResult;
}
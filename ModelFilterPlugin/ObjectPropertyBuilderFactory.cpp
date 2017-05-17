//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "ObjectPropertyBuilderFactory.h"

#include "LevelPropertyBuilder.h"
#include "WallPropertyBuilder.h"
#include "ColumnPropertyBuilder.h"
#include "FloorPropertyBuilder.h"
#include "OpeningPropertyBuilder.h"
#include "RoofPropertyBuilder.h"
#include "BeamPropertyBuilder.h"
#include "StairPropertyBuilder.h"
#include "RampPropertyBuilder.h"
#include "WindowPropertyBuilder.h"
#include "DoorPropertyBuilder.h"
#include "RailingPropertyBuilder.h"
#include "RoomPropertyBuilder.h"
#include "IsolatedFoundationPropertyBuilder.h"
#include "WallFoundationPropertyBuilder.h"

#include <RengaAPI/ModelObjectTypes.h>
#include <RengaAPI/Project.h>

ObjectPropertyBuilderFactory::ObjectPropertyBuilderFactory()
{}

ObjectPropertyBuilder* ObjectPropertyBuilderFactory::createBuilder(const rengaapi::ObjectType objectType)
{
  ObjectPropertyBuilder* pResult = nullptr;

  if (objectType == rengaapi::ModelObjectTypes::LevelType)
    pResult = new LevelPropertyBuilder();
  else if (objectType == rengaapi::ModelObjectTypes::WallType)
    pResult = new WallPropertyBuilder();
  else if (objectType == rengaapi::ModelObjectTypes::ColumnType)
    pResult = new ColumnPropertyBuilder();
  else if (objectType == rengaapi::ModelObjectTypes::FloorType)
    pResult = new FloorPropertyBuilder();
  else if (objectType == rengaapi::ModelObjectTypes::OpeningType)
    pResult = new OpeningPropertyBuilder();
  else if (objectType == rengaapi::ModelObjectTypes::RoofType)
    pResult = new RoofPropertyBuilder();
  else if (objectType == rengaapi::ModelObjectTypes::BeamType)
    pResult = new BeamPropertyBuilder();
  else if (objectType == rengaapi::ModelObjectTypes::StairType)
    pResult = new StairPropertyBuilder();
  else if (objectType == rengaapi::ModelObjectTypes::RampType)
    pResult = new RampPropertyBuilder();
  else if (objectType == rengaapi::ModelObjectTypes::WindowType)
    pResult = new WindowPropertyBuilder();
  else if (objectType == rengaapi::ModelObjectTypes::DoorType)
    pResult = new DoorPropertyBuilder();
  else if (objectType == rengaapi::ModelObjectTypes::RailingType)
    pResult = new RailingPropertyBuilder();
  else if (objectType == rengaapi::ModelObjectTypes::RoomType)
    pResult = new RoomPropertyBuilder();
  else if (objectType == rengaapi::ModelObjectTypes::IsolatedFoundationType)
    pResult = new IsolatedFoundationPropertyBuilder();
  else if (objectType == rengaapi::ModelObjectTypes::WallFoundationType)
    pResult = new WallFoundationPropertyBuilder();

  return pResult;
}
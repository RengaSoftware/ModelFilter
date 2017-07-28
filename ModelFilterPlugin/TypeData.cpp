//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "TypeData.h"

TypeData::TypeData(rengaapi::ObjectType type, QString typeNodeName) : 
  m_type(type),
  m_typeNodeName(typeNodeName)
{}

TypeDataArray::TypeDataArray() : m_array({
  TypeData(rengaapi::ModelObjectTypes::LevelType, QApplication::translate("modelObjects", "Levels")),
  TypeData(rengaapi::ModelObjectTypes::WallType, QApplication::translate("modelObjects", "Walls")),
  TypeData(rengaapi::ModelObjectTypes::ColumnType, QApplication::translate("modelObjects", "Columns")),
  TypeData(rengaapi::ModelObjectTypes::FloorType, QApplication::translate("modelObjects", "Floors")),
  TypeData(rengaapi::ModelObjectTypes::OpeningType, QApplication::translate("modelObjects", "Openings")),
  TypeData(rengaapi::ModelObjectTypes::RoofType, QApplication::translate("modelObjects", "Roofs")),
  TypeData(rengaapi::ModelObjectTypes::BeamType, QApplication::translate("modelObjects", "Beams")),
  TypeData(rengaapi::ModelObjectTypes::StairType, QApplication::translate("modelObjects", "Stairs")),
  TypeData(rengaapi::ModelObjectTypes::RampType, QApplication::translate("modelObjects", "Ramps")),
  TypeData(rengaapi::ModelObjectTypes::WindowType, QApplication::translate("modelObjects", "Windows")),
  TypeData(rengaapi::ModelObjectTypes::DoorType, QApplication::translate("modelObjects", "Doors")),
  TypeData(rengaapi::ModelObjectTypes::RailingType, QApplication::translate("modelObjects", "Railings")),
  TypeData(rengaapi::ModelObjectTypes::RoomType, QApplication::translate("modelObjects", "Rooms")),
  TypeData(rengaapi::ModelObjectTypes::IsolatedFoundationType, QApplication::translate("modelObjects", "Isolated foundations")),
  TypeData(rengaapi::ModelObjectTypes::WallFoundationType, QApplication::translate("modelObjects", "Wall foundations")) })
{}

QString TypeDataArray::getObjectTypeName(const rengaapi::ObjectType objectType)
{
  for (auto& typeData : m_array)
  {
    if (typeData.m_type == objectType)
    {
      return typeData.m_typeNodeName;
    }
  }

  return QString("Undefined type");
}
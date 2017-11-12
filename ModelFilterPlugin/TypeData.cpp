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
  TypeData(rengaapi::ModelObjectTypes::LevelType, QApplication::translate("mf_modelObjects", "Levels")),
  TypeData(rengaapi::ModelObjectTypes::WallType, QApplication::translate("mf_modelObjects", "Walls")),
  TypeData(rengaapi::ModelObjectTypes::ColumnType, QApplication::translate("mf_modelObjects", "Columns")),
  TypeData(rengaapi::ModelObjectTypes::FloorType, QApplication::translate("mf_modelObjects", "Floors")),
  TypeData(rengaapi::ModelObjectTypes::OpeningType, QApplication::translate("mf_modelObjects", "Openings")),
  TypeData(rengaapi::ModelObjectTypes::RoofType, QApplication::translate("mf_modelObjects", "Roofs")),
  TypeData(rengaapi::ModelObjectTypes::BeamType, QApplication::translate("mf_modelObjects", "Beams")),
  TypeData(rengaapi::ModelObjectTypes::StairType, QApplication::translate("mf_modelObjects", "Stairs")),
  TypeData(rengaapi::ModelObjectTypes::RampType, QApplication::translate("mf_modelObjects", "Ramps")),
  TypeData(rengaapi::ModelObjectTypes::WindowType, QApplication::translate("mf_modelObjects", "Windows")),
  TypeData(rengaapi::ModelObjectTypes::DoorType, QApplication::translate("mf_modelObjects", "Doors")),
  TypeData(rengaapi::ModelObjectTypes::RailingType, QApplication::translate("mf_modelObjects", "Railings")),
  TypeData(rengaapi::ModelObjectTypes::RoomType, QApplication::translate("mf_modelObjects", "Rooms")),
  TypeData(rengaapi::ModelObjectTypes::IsolatedFoundationType, QApplication::translate("mf_modelObjects", "Isolated foundations")),
  TypeData(rengaapi::ModelObjectTypes::WallFoundationType, QApplication::translate("mf_modelObjects", "Wall foundations")) })
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
//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "TypeData.h"

TypeData::TypeData(rengaapi::ObjectType type, QString translationLiteral) : m_type(type)
{
  m_typeNodeName = QApplication::translate("modelObjects", translationLiteral.toStdString().data());
}

TypeDataArray::TypeDataArray() : m_array({
  TypeData(rengaapi::ModelObjectTypes::LevelType, "Levels"),
  TypeData(rengaapi::ModelObjectTypes::WallType, "Walls"),
  TypeData(rengaapi::ModelObjectTypes::ColumnType, "Columns"),
  TypeData(rengaapi::ModelObjectTypes::FloorType, "Floors"),
  TypeData(rengaapi::ModelObjectTypes::OpeningType, "Openings"),
  TypeData(rengaapi::ModelObjectTypes::RoofType, "Roofs"),
  TypeData(rengaapi::ModelObjectTypes::BeamType, "Beams"),
  TypeData(rengaapi::ModelObjectTypes::StairType, "Stairs"),
  TypeData(rengaapi::ModelObjectTypes::RampType, "Ramps"),
  TypeData(rengaapi::ModelObjectTypes::WindowType, "Windows"),
  TypeData(rengaapi::ModelObjectTypes::DoorType, "Doors"),
  TypeData(rengaapi::ModelObjectTypes::RailingType, "Railings"),
  TypeData(rengaapi::ModelObjectTypes::RoomType, "Rooms"),
  TypeData(rengaapi::ModelObjectTypes::IsolatedFoundationType, "Isolated foundations"),
  TypeData(rengaapi::ModelObjectTypes::WallFoundationType, "Wall foundations") })
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
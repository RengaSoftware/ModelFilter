//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include <RengaAPI/ModelObjectTypes.h>
#include <QtWidgets/QApplication.h>
#include <QtCore/QString.h>

struct TypeData
{
  TypeData(rengaapi::ObjectType type, QString translationLiteral);

  rengaapi::ObjectType m_type;
  QString m_typeNodeName;
};

struct TypeDataArray
{
  TypeDataArray();
  QString getObjectTypeName(const rengaapi::ObjectType objectType);

  const std::vector<TypeData> m_array;
};


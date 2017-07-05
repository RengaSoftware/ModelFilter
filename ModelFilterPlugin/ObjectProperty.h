//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include "FilterData.h"

#include <RengaAPI/MaterialId.h>
#include <RengaAPI/ModelObject.h>
#include <RengaBase/LengthMeasureOptional.h>
#include <RengaBase/AreaMeasureOptional.h>
#include <RengaBase/VolumeMeasureOptional.h>
class ObjectProperty
{
public:
  ObjectProperty();
  virtual ~ObjectProperty();
  virtual PropertyList getObjectProperties() = 0;

  static PropertyList getUserAttributes(const rengaapi::ObjectType& objectType);
};
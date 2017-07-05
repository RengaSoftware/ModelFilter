//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include "ObjectProperty.h"
#include "ObjectFilter.h"

#include <RengaAPI/ModelObjectTypes.h>

class ObjectPropertyFactory
{
public:
  ObjectProperty* createObjectProperty(const rengaapi::ObjectType objectType);
};

class ObjectFilterFactory
{
public:
  ObjectFilter* createObjectFilter(const rengaapi::ObjectType objectType);
};
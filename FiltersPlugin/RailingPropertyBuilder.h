//
// Copyright �Renga Software� LLC, 2016. All rights reserved.
//
// �Renga Software� LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// �Renga Software� LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include "ObjectPropertyBuilder.h"

class RailingPropertyBuilder : public ObjectPropertyBuilder
{
public:
  RailingPropertyBuilder();

  PropertyList getObjectProperties();
  bool isObjectMatchFilter(const Search�riteriaData& data, rengaapi::ModelObject* pObject);

private:
  PropertyList properties;
};
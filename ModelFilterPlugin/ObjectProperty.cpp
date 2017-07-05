//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "ObjectProperty.h"

#include <RengaAPI/Materials.h>
#include <RengaAPI/ModelObjectTypes.h>
#include <RengaAPI/Project.h>
#include <RengaAPI/UserAttributeRegister.h>
#include <RengaAPI/UserAttributeDescription.h>
#include <RengaBase/LengthMeasure.h>
#include <RengaBase/AreaMeasure.h>
#include <RengaBase/VolumeMeasure.h>

ObjectProperty::ObjectProperty()
{}

ObjectProperty::~ObjectProperty()
{}

PropertyList ObjectProperty::getUserAttributes(const rengaapi::ObjectType& objectType)
{
  std::list<Property> attributeList;
  rengaapi::UserAttributeRegister userAttributeRegister = rengaapi::Project::userAttributeRegister();
  rengaapi::UserAttributeIdCollection attributeCollection = userAttributeRegister.attributes();

  // check all attributes
  for (size_t i = 0; i < attributeCollection.size(); ++i)
  {
    // check if objectType has attribute
    const rengaapi::UserAttributeId& attributeId = attributeCollection.get(i);
    if (userAttributeRegister.typeHasAttribute(objectType, attributeId))
    {
      // get attribute name
      rengaapi::UserAttributeDescription userAttributeDescription;
      if (userAttributeRegister.attributeDescription(attributeId, userAttributeDescription).code() == rengaapi::Status::Success)
      {
        QString attributeName = rengaStringToQString(userAttributeDescription.name());
        ValueType attributeType = userAttributeDescription.type() == rengaapi::UserAttributeType::Double ? ValueType::Double : ValueType::String;
        attributeList.push_back(Property(PropertyType::UserAttribute, attributeType, attributeName));
      }
    }
  }

  return attributeList;
}
//
// Copyright ìRenga Softwareî LLC, 2016. All rights reserved.
//
// ìRenga Softwareî LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// ìRenga Softwareî LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "FilterData.h"
#include "ObjectPropertyBuilderFactory.h"

void FilterData::exportData(QFile* filterFile)
{
  QXmlStreamWriter writer(filterFile);
  writer.setAutoFormatting(true);
  writer.setAutoFormattingIndent(2);
  writer.writeStartDocument();
  
  writer.writeStartElement("filter");
  writer.writeTextElement("name", m_filterName);
  for (auto& group : m_groupList)
  {
    writer.writeStartElement("group");
    writer.writeTextElement("type", rengaStringToQString(group.m_groupType.id().toString()));
    for (auto& property: group.m_propertyList)
    {
      writer.writeStartElement("property");
      writer.writeTextElement("propertyType", QString::number(property.m_property.m_propertyType));
      writer.writeTextElement("operatorType", QString::number(property.m_operatorType));
      writer.writeTextElement("valueType", QString::number(property.m_property.m_valueType));
      writer.writeTextElement("value", property.m_value);
      writer.writeEndElement();
    }
    writer.writeEndElement();
  }
  writer.writeEndElement();
  writer.writeEndDocument();
  return;
}

FilterData FilterData::importData(QFile* filterFile)
{
  FilterData data("empty name");
  QXmlStreamReader reader;
  reader.setDevice(filterFile);
  while (!reader.atEnd())
  {
    QXmlStreamReader::TokenType token = reader.readNext();
    if (token == QXmlStreamReader::StartDocument)
      continue;

    if (token == QXmlStreamReader::StartElement)
    {
      const QStringRef ref = reader.name();
      if (ref == "filter")
        continue;
      if (ref == "name")
      {
        data.m_filterName = parseTagText(reader);
      }
      else if (reader.name() == "group")
      {
        GroupData group = parseGroup(reader);
        if (group.m_propertyList.size() > 0)
          data.m_groupList.push_back(group);
      }
    }
  }

  return data;
}

GroupData FilterData::parseGroup(QXmlStreamReader& reader)
{
  GroupData groupData(rengaapi::ModelObjectTypes::UndefinedType);
  reader.readNext();
  while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "group"))
  {
    if (reader.tokenType() == QXmlStreamReader::StartElement)
    {
      if (reader.name() == "type")
      {
        QString typeUUID = parseTagText(reader);
        rengabase::UUID uuid = rengabase::UUID::fromString(QStringToRengaString(typeUUID));
        groupData.m_groupType = rengaapi::ObjectType(uuid);
      }
      else
      {
        Search—riteriaData propertyData = parseProperty(reader, groupData.m_groupType);
        if (propertyData.m_property.m_propertyType != PropertyType::Invalid)
          groupData.m_propertyList.push_back(propertyData);
      }
    }
    reader.readNext();
  }
  return groupData;
}

Search—riteriaData FilterData::parseProperty(QXmlStreamReader& reader, rengaapi::ObjectType type)
{
  reader.readNext();

  // get propertyList of ObjectType
  ObjectPropertyBuilderFactory factory;
  std::unique_ptr<ObjectPropertyBuilder> builder(factory.createBuilder(type));
  const PropertyList propertyList = builder->getObjectProperties();

  // parse property
  PropertyType propertyType;
  OperatorType operatorType;
  ValueType valueType;
  QString value;
  QString propertyName;
  int counter = 0;
  while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "property"))
  {
    if (reader.tokenType() == QXmlStreamReader::StartElement)
    {
      if (reader.name() == "propertyType")
      {
        propertyType = PropertyType(parseTagText(reader).toInt());
        counter++;
        for (auto it = propertyList.begin(); it != propertyList.end(); ++it) {
          if (it->m_propertyType == propertyType) {
            propertyName = it->m_propertyName;
            counter++;
            break;
          }
        }
      }
      if (reader.name() == "operatorType") {
        operatorType = OperatorType(parseTagText(reader).toInt());
        counter++;
      }
      if (reader.name() == "valueType") {
        valueType = ValueType(parseTagText(reader).toInt());
        counter++;
      }
      if (reader.name() == "value") {
        value = parseTagText(reader);
        counter++;
      }
    }
    reader.readNext();
  }

  if (counter == 5)
    return Search—riteriaData(Property(propertyType, valueType, propertyName), operatorType, value);
  else
    // object don't have this PropertyType, return empty PropertyData
    return Search—riteriaData(Property(PropertyType::Invalid, valueType, propertyName), operatorType, value);
}

QString FilterData::parseTagText(QXmlStreamReader& reader)
{
  if (reader.tokenType() != QXmlStreamReader::StartElement)
    return QString();
  QString elementName = reader.name().toString();
  reader.readNext();
  QString value = reader.text().toString();
  return value;
}
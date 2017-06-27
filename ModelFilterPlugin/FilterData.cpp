//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "FilterData.h"
#include "ObjectPropertyBuilderFactory.h"

#include <map>

bool GroupData::isValid()
{
  return m_propertyList.size() > 0;
}

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
    for (auto& property : group.m_propertyList)
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
  FilterData data;
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
        if (group.isValid())
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
        SearchCriteriaData propertyData = parseProperty(reader, groupData.m_groupType);
        if (propertyData.m_property.m_propertyType != PropertyType::Invalid)
          groupData.m_propertyList.push_back(propertyData);
      }
    }
    reader.readNext();
  }
  return groupData;
}

bool FilterData::isValid()
{
  return m_filterName.length() > 0 && m_groupList.size() > 0;
}

SearchCriteriaData FilterData::parseProperty(QXmlStreamReader& reader, rengaapi::ObjectType type)
{
  // parse property 
  reader.readNext();
  std::map<QString, QString> data;
  QStringList keyList = { "propertyType", "operatorType", "valueType", "value" };
  while (!(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "property"))
  {
    if (reader.tokenType() == QXmlStreamReader::StartElement)
    {
      const QString tagName = reader.name().toString();
      if (keyList.contains(tagName))
      {
        data[tagName] = parseTagText(reader);
        keyList.removeOne(tagName);
      }
    }
    reader.readNext();
  }

  // check if all data fields present exactly once
  bool ok, flag = keyList.empty();
  PropertyType propertyType = PropertyType(data["propertyType"].toInt(&ok));
  flag &= ok;
  OperatorType operatorType = OperatorType(data["operatorType"].toInt(&ok));
  flag &= ok;
  ValueType valueType = ValueType(data["valueType"].toInt(&ok));
  flag &= ok;
  QString value = data["value"];

  // check if object has property
  // also get property name
  ObjectPropertyBuilderFactory factory;
  std::unique_ptr<ObjectPropertyBuilder> builder(factory.createBuilder(type));
  const PropertyList propertyList = builder->getObjectProperties();
  QString propertyName;
  for (auto it = propertyList.begin(); it != propertyList.end(); ++it) {
    if (it->m_propertyType == propertyType) {
      propertyName = it->m_propertyName;
      break;
    }
  }

  if (!flag || propertyName.isEmpty())
  {
    // property not valid
    propertyType = PropertyType::Invalid;
  }
    
  return SearchCriteriaData(Property(propertyType, valueType, propertyName), operatorType, value);
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
//
// Copyright �Renga Software� LLC, 2017. All rights reserved.
//
// �Renga Software� LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// �Renga Software� LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "FilterData.h"
#include "ObjectFactory.h"

#include <map>

const QString currentVersion = "1";

bool GroupData::isValid()
{
  return m_propertyList.size() > 0;
}

void FilterData::exportData(QFile* filterFile) const
{
  QXmlStreamWriter writer(filterFile);
  writer.setAutoFormatting(true);
  writer.setAutoFormattingIndent(2);
  writer.writeStartDocument();

  writer.writeStartElement("filter");
  writer.writeTextElement("version", currentVersion);
  writer.writeTextElement("name", m_filterName);
  for (auto& group : m_groupList)
  {
    writer.writeStartElement("group");
    writer.writeTextElement("type", rengaStringToQString(group.m_groupType.id().toString()));
    for (auto& property : group.m_propertyList)
    {
      writer.writeStartElement("searchCriteria");
      writer.writeTextElement("propertyType", QString::number(property.m_property.m_propertyType));
      if (property.m_property.m_propertyType == PropertyType::UserAttribute)
      {
        writer.writeTextElement("attributeName", property.m_property.m_propertyName);
      }
      writer.writeTextElement("operatorType", QString::number(property.m_operatorType));
      writer.writeTextElement("valueType", QString::number(property.m_property.m_valueType));
      
      if (property.m_property.m_valueType == ValueType::Double)
      {
        // saving double value in English locale
        // better to rewrite SearchCriteria, Operator and Property to use different types inside
        writer.writeTextElement("value", property.m_value);
      }
      else
      {
        writer.writeTextElement("value", property.m_value);
      }
      
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
  while (!reader.atEnd() && !(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "group"))
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
        SearchCriteriaData propertyData = parseSearchCriteria(reader, groupData.m_groupType);
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

SearchCriteriaData FilterData::parseSearchCriteria(QXmlStreamReader& reader, const rengaapi::ObjectType& type)
{
  reader.readNext();
  std::map<QString, int> typeData;
  QStringList typeNameList = { "propertyType", "operatorType", "valueType" };
  QString propertyValue;
  QString propertyName;

  while (!reader.atEnd() && !(reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "searchCriteria"))
  {
    if (reader.tokenType() == QXmlStreamReader::StartElement)
    {
      const QString tagName = reader.name().toString();
      if (typeNameList.contains(tagName))
      {
        int number;
        if (getTypeNumber(parseTagText(reader), number))
        {
          typeData[tagName] = number;
          typeNameList.removeOne(tagName);
        }
      }
      else if (tagName.compare("value") == 0)
      {
        propertyValue = parseTagText(reader);
      }
      else if (tagName.compare("attributeName") == 0)
      {
        propertyName = parseTagText(reader);
      }
    }
    reader.readNext();
  }

  PropertyType propertyType = PropertyType(typeData["propertyType"]);
  OperatorType operatorType = OperatorType(typeData["operatorType"]);
  ValueType valueType = ValueType(typeData["valueType"]);

  // get property name
  bool hasName;
  if (propertyType == PropertyType::UserAttribute)
  {
    hasName = !propertyName.isEmpty();
  }
  else
  {
    hasName = getPropertyName(propertyType, type, propertyName);
  }

  // check if property valid
  // TODO: remove this magic conditions
  bool emptyValue = operatorType != OperatorType::All && operatorType != OperatorType::None && propertyValue.isEmpty();
  if (!typeNameList.empty() || !hasName || emptyValue)
  {
    propertyType = PropertyType::Invalid;
  }

  // if it is double value it should be in English locale
  // better to rewrite SearchCriteria, Operator and Property to use different types inside
  return SearchCriteriaData(Property(propertyType, valueType, propertyName), operatorType, propertyValue);
}

bool FilterData::getTypeNumber(const QString& value, int& res)
{
  bool ok;
  res = value.toInt(&ok);
  return ok;
}

bool FilterData::getPropertyName(const PropertyType propertyType, const rengaapi::ObjectType& type, QString& propertyName)
{
  ObjectPropertyFactory factory;
  std::unique_ptr<ObjectProperty> builder(factory.createObjectProperty(type));
  const PropertyList propertyList = builder->getObjectProperties();

  for (auto it = propertyList.begin(); it != propertyList.end(); ++it)
  {
    if (it->m_propertyType == propertyType)
    {
      propertyName = it->m_propertyName;
      return true;
    }
  }

  return false;
}

QString FilterData::parseTagText(QXmlStreamReader& reader)
{
  if (reader.tokenType() != QXmlStreamReader::StartElement)
  {
    return QString();
  }

  QString elementName = reader.name().toString();
  reader.readNext();
  QString value = reader.text().toString();
  return value;
}

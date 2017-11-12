//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "GroupDialog.h"
#include "ui_GroupDialog.h"
#include "TypeData.h"
#include "OperatorData.h"

static const unsigned int c_treeViewColumnsCount = 3;

class DoublePropertyValueValidator : public QDoubleValidator
{
public:
  DoublePropertyValueValidator()
  {
    setLocale(QLocale(QLocale::English));
  }

  QValidator::State validate(QString& input, int &pos) const
  {
    if (QDoubleValidator::validate(input, pos) == QValidator::State::Invalid)
      return QValidator::State::Invalid;
    bool isConvertedToDoubleInEnglishLocale = false;
    QLocale(QLocale::English).toDouble(input, &isConvertedToDoubleInEnglishLocale);
    if (isConvertedToDoubleInEnglishLocale)
      return QValidator::State::Acceptable;
    else
      return QValidator::State::Invalid;
  }
};

GroupDialog::GroupDialog(QDialog* parent)
  : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint)
{
  m_pUi.reset(new Ui::GroupDialog());
  m_pUi->setupUi(this);

  // load treeView
  m_pTreeModel.reset(new QStandardItemModel());
  m_pTreeModel->setColumnCount(c_treeViewColumnsCount);
  m_pUi->treeView->setModel(m_pTreeModel.get());

  // set treeView columns
  QHeaderView* pHeader = m_pUi->treeView->header();
  pHeader->setSelectionMode(QAbstractItemView::SingleSelection);

  const std::vector<QString> headerLabels = {
    QApplication::translate("mf_header", "property"),
    QApplication::translate("mf_header", "operator"),
    QApplication::translate("mf_header", "value")
  };

  for (size_t i = 0; i < c_treeViewColumnsCount; i++)
  {
    m_pTreeModel->setHeaderData(i, Qt::Orientation::Horizontal, QVariant(headerLabels[i]));
    pHeader->setSectionResizeMode(pHeader->logicalIndex(i), QHeaderView::ResizeMode::Interactive);
  }

  // load comboboxes
  loadTypeBox();
  loadPropertyBox();
  loadOperatorBox();

  onTypeBoxIndexChanged(0);

  // set buttons
  connect(m_pUi->addButton, SIGNAL(clicked()), this, SLOT(onAddButton()));
  connect(m_pUi->deleteButton, SIGNAL(clicked()), this, SLOT(onDeleteButton()));

  connect(m_pUi->valueLine, SIGNAL(textEdited(const QString&)), this, SLOT(onValueChanged(const QString&)));

  itemCountChanged(false);
}

GroupDialog::GroupDialog(QDialog* parent, const GroupData& groupData)
  : GroupDialog(parent)
{
  setWindowTitle(QApplication::translate("GroupDialog", "EditWindowName"));

  // set data in tree
  buildProperties(groupData);

  // select first item once: intitially tree hasn't selected item
  m_pUi->treeView->setCurrentIndex(m_pTreeModel->index(0, 0));

  // set buttons
  itemCountChanged(true);

  // set current comboBox
  setTypeBox(groupData.m_groupType);
}

GroupDialog::~GroupDialog()
{}

GroupData GroupDialog::getGroupDescription()
{
  // get rengaapi::ObjectType
  const int itemIndex = m_pTypeBoxSortModel->mapToSource(m_pTypeBoxSortModel->index(m_pUi->typeBox->currentIndex(), 0)).row();
  QVariant currentData = m_pTypeBoxModel->item(itemIndex, 0)->data();
  assert(currentData.isValid());
  QString typeUUID = currentData.toString();
  rengabase::UUID uuid = rengabase::UUID::fromString(QStringToRengaString(typeUUID));
  rengaapi::ObjectType objectType = rengaapi::ObjectType(uuid);
  GroupData groupData(objectType);

  for (size_t i = 0; i < m_pTreeModel->rowCount(); i++)
  {
    // get property data
    QModelIndex propertyIndex = m_pTreeModel->index(i, 0);
    QModelIndex operatorIndex = m_pTreeModel->index(i, 1);
    QModelIndex valueIndex = m_pTreeModel->index(i, 2);
    QStandardItem* pPropertyItem = m_pTreeModel->itemFromIndex(propertyIndex);
    QStandardItem* pOperatorItem = m_pTreeModel->itemFromIndex(operatorIndex);
    QStandardItem* pValueItem = m_pTreeModel->itemFromIndex(valueIndex);

    bool ok = false;
    PropertyType propertyType = PropertyType(pPropertyItem->data().toInt(&ok));
    assert(ok);
    ok = false;
    ValueType valueType = ValueType(pPropertyItem->data(Qt::UserRole + 2).toInt(&ok));
    assert(ok);
    QString propertyName = pPropertyItem->text();
    Property property(propertyType, valueType, propertyName);
    ok = false;
    OperatorType operatorType = OperatorType(pOperatorItem->data().toInt(&ok));
    assert(ok);
    SearchCriteriaData propertyData(property, operatorType, pValueItem->text());
    groupData.m_propertyList.push_back(propertyData);
  }

  return groupData;
}

void GroupDialog::onAddButton()
{
  // list of items to treeView
  QList<QStandardItem*> list = { new QStandardItem(m_pUi->propertyBox->currentText()),
                                 new QStandardItem(m_pUi->operatorBox->currentText()),
                                 new QStandardItem(m_pUi->valueLine->text()) };
  // set property data
  const QModelIndex realIndex = m_pPropertyBoxSortModel->index(m_pUi->propertyBox->currentIndex(), 0);
  const QModelIndex orderIndex = m_pPropertyBoxSortModel->mapToSource(realIndex);
  QStandardItem* pPropertyItem = m_pPropertyBoxModel->itemFromIndex(orderIndex);
  list.at(0)->setData(pPropertyItem->data());
  list.at(0)->setData(pPropertyItem->data(Qt::UserRole + 2), Qt::UserRole + 2);
  // set operator data
  list.at(1)->setData(m_pOperatorBoxModel->item(m_pUi->operatorBox->currentIndex())->data());

  m_pTreeModel->appendRow(list);
  m_pUi->treeView->setCurrentIndex(m_pTreeModel->index(m_pTreeModel->rowCount() - 1, 0));

  // set comboBox and buttons state
  itemCountChanged(true);
  m_pUi->okButton->setFocus();
  m_pUi->valueLine->clear();
}

void GroupDialog::onDeleteButton()
{
  QModelIndex currentIndex = m_pUi->treeView->currentIndex();
  const int selectedRow = currentIndex.row();
  m_pTreeModel->removeRow(selectedRow);

  if (m_pTreeModel->rowCount() == 0)
  {
    itemCountChanged(false);
    m_pUi->cancelButton->setFocus();
  }
  else
  {
    m_pUi->treeView->setCurrentIndex(m_pTreeModel->index(std::min(m_pTreeModel->rowCount() - 1, selectedRow), 0));
  }
}

void GroupDialog::onTypeBoxIndexChanged(const int changedIndex)
{
  // change factory
  const int itemIndex = m_pTypeBoxSortModel->mapToSource(m_pTypeBoxSortModel->index(changedIndex, 0)).row();
  QVariant currentData = m_pTypeBoxModel->item(itemIndex, 0)->data();
  assert(currentData.isValid());
  const rengabase::UUID uuid = rengabase::UUID::fromString(QStringToRengaString(currentData.toString()));
  rengaapi::ObjectType objectType = rengaapi::ObjectType(uuid);

  ObjectPropertyFactory factory;
  m_pBuilder.reset(factory.createObjectProperty(objectType));

  // reload property combobox
  reloadPropertyBox();
  reloadOperatorBox();
}

void GroupDialog::onPropertyBoxIndexChanged(const int changedIndex)
{
  reloadOperatorBox();
}

void GroupDialog::onOperatorBoxIndexChanged(const int changedIndex)
{
  reloadValueLine(changedIndex);
}

void GroupDialog::onValueChanged(const QString& value)
{
  m_pUi->addButton->setEnabled(value.length() > 0);
}

void GroupDialog::buildProperties(const GroupData& groupData) {
  for (auto& rowData : groupData.m_propertyList)
  {
    QStandardItem* pPropertyItem = new QStandardItem(rowData.m_property.m_propertyName);
    pPropertyItem->setData(QVariant(rowData.m_property.m_propertyType));
    pPropertyItem->setData(QVariant(rowData.m_property.m_valueType), Qt::UserRole + 2);
    
    QStandardItem* pOperatorItem = new QStandardItem(OperatorData::getOperatorName(rowData.m_operatorType));
    pOperatorItem->setData(QVariant(rowData.m_operatorType));

    QStandardItem* pValueItem = new QStandardItem(rowData.m_value);

    m_pTreeModel->appendRow({ pPropertyItem, pOperatorItem, pValueItem });
  }
}

void GroupDialog::setTypeBox(const rengaapi::ObjectType& groupType) {
  TypeDataArray typeDataArray;
  for (size_t i = 0; i < typeDataArray.m_array.size(); i++)
  {
    if (typeDataArray.m_array[i].m_type == groupType)
    {
      QModelIndex modelIndex = m_pTypeBoxModel->index(i, 0);
      int realRow = m_pTypeBoxSortModel->mapFromSource(modelIndex).row();
      m_pUi->typeBox->setCurrentIndex(realRow);
      break;
    }
  }
}

void GroupDialog::itemCountChanged(bool isZero) {
  m_pUi->okButton->setEnabled(isZero);
  m_pUi->deleteButton->setEnabled(isZero);
  m_pUi->typeBox->setEnabled(!isZero);
}

void GroupDialog::loadTypeBox()
{
  // load model for combobox
  m_pTypeBoxModel.reset(new QStandardItemModel());
  m_pTypeBoxSortModel.reset(new QSortFilterProxyModel());
  m_pTypeBoxSortModel->setSourceModel(m_pTypeBoxModel.get());
  m_pUi->typeBox->setModel(m_pTypeBoxSortModel.get());

  TypeDataArray typeDataArray;
  for (auto& typeData : typeDataArray.m_array) {
    QString uuid = rengaStringToQString(typeData.m_type.id().toString());
    QStandardItem* typeItem = new QStandardItem(typeData.m_typeNodeName);
    typeItem->setData(QVariant(uuid));
    m_pTypeBoxModel->appendRow(typeItem);
  }

  // sort combo box once
  m_pTypeBoxSortModel->sort(0);

  // set default builder
  ObjectPropertyFactory factory;
  m_pBuilder.reset(factory.createObjectProperty(rengaapi::ModelObjectTypes::LevelType));
  m_pUi->typeBox->setCurrentIndex(0);

  // connect type combobox
  connect(m_pUi->typeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeBoxIndexChanged(int)));
}

void GroupDialog::loadPropertyBox()
{
  // load model
  m_pPropertyBoxModel.reset(new QStandardItemModel());
  m_pPropertyBoxSortModel.reset(new QSortFilterProxyModel());
  m_pPropertyBoxSortModel->setSourceModel(m_pPropertyBoxModel.get());
  m_pUi->propertyBox->setModel(m_pPropertyBoxSortModel.get());

  // load current state of property box
  reloadPropertyBox();
}

void GroupDialog::reloadPropertyBox()
{
  disconnect(m_pUi->propertyBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onPropertyBoxIndexChanged(int)));
  m_pPropertyBoxModel->clear();

  // get properties
  PropertyList propertyList = m_pBuilder->getObjectProperties();

  // get user attributes
  const int currentIndex = m_pUi->typeBox->currentIndex();
  const int realIndex = m_pTypeBoxSortModel->mapToSource(m_pTypeBoxSortModel->index(currentIndex, 0)).row();
  TypeDataArray typeDataArray;
  PropertyList userAttrList = m_pBuilder->getUserAttributes(typeDataArray.m_array[realIndex].m_type);
  propertyList.splice(propertyList.end(), userAttrList);

  for (auto& property : propertyList)
  {
    QStandardItem* comboBoxItem = new QStandardItem(property.m_propertyName);
    comboBoxItem->setData(QVariant(property.m_propertyType));
    comboBoxItem->setData(QVariant(property.m_valueType), Qt::UserRole + 2);
    m_pPropertyBoxModel->appendRow(comboBoxItem);
  }

  // sort properties
  m_pPropertyBoxSortModel->sort(0);
  m_pUi->propertyBox->setCurrentIndex(0);

  // connect property box
  connect(m_pUi->propertyBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onPropertyBoxIndexChanged(int)));
}

void GroupDialog::loadOperatorBox()
{
  // load model
  m_pOperatorBoxModel.reset(new QStandardItemModel());
  m_pUi->operatorBox->setModel(m_pOperatorBoxModel.get());

  reloadOperatorBox();
}

void GroupDialog::reloadOperatorBox()
{
  disconnect(m_pUi->operatorBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onOperatorBoxIndexChanged(int)));
  m_pOperatorBoxModel->clear();

  // get type of selected property
  const int propertyIndex = m_pUi->propertyBox->currentIndex();
  const int realIndex = m_pPropertyBoxSortModel->mapToSource(m_pPropertyBoxSortModel->index(propertyIndex, 0)).row();
  QModelIndex modelIndex = m_pPropertyBoxModel->index(realIndex, 0);
  QVariant data = m_pPropertyBoxModel->itemFromIndex(modelIndex)->data(Qt::UserRole + 2);
  assert(data.isValid());
  bool ok = false;
  ValueType valueType = ValueType(data.toInt(&ok));

  // load operators
  auto operators = OperatorData::getOperators(valueType);
  for (auto& it : operators)
  {
    QStandardItem* operatorItem = new QStandardItem(it.second);
    operatorItem->setData(QVariant(it.first));
    m_pOperatorBoxModel->appendRow(operatorItem);
  }

  connect(m_pUi->operatorBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onOperatorBoxIndexChanged(int)));

  if (valueType == ValueType::Double)
  {
    // DoubleValidator::setLocale(QLocale(QLocale::English)) does not work, it allows coma
    // Custom validator added
    auto pDoubleValidator = new DoublePropertyValueValidator();
    m_pUi->valueLine->setValidator(pDoubleValidator);
  }
  else
  {
    m_pUi->valueLine->setValidator(nullptr);
  }
  reloadValueLine(m_pUi->operatorBox->currentIndex());
}

void GroupDialog::reloadValueLine(int changedIndex)
{
  m_pUi->valueLine->clear();

  QStandardItem* pItem = m_pOperatorBoxModel->item(changedIndex, 0);
  bool ok = true;
  OperatorType type = OperatorType(pItem->data().toInt(&ok));
  assert(ok);

  if (type == OperatorType::All || type == OperatorType::None)
  {
    m_pUi->valueLine->setEnabled(false);
    m_pUi->addButton->setEnabled(true);
  }
  else
  {
    m_pUi->valueLine->setEnabled(true);
    onValueChanged("");
  }
}

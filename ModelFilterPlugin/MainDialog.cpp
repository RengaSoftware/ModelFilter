//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "MainDialog.h"
#include "FilterDialog.h"
#include "ui_MainDialog.h"
#include "NameGenerator.h"
#include "ObjectPropertyBuilderFactory.h"

#include <QtCore/QFile.h>
#include <QtWidgets/QFileDialog.h>
#include <QtWidgets/QMessageBox.h>

#include <RengaAPI/Application.h>
#include <RengaAPI/LevelView.h>
#include <RengaAPI/Model.h>
#include <RengaAPI/ModelObjectTypes.h>
#include <RengaAPI/ObjectVisibility.h>
#include <RengaAPI/Project.h>

MainDialog::MainDialog()
  : QDialog(nullptr, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint)
{
  m_pUi.reset(new Ui::MainDialog());
  m_pUi->setupUi(this);

  // load list view
  m_pListModel.reset(new QStandardItemModel);
  m_pUi->listView->setModel(m_pListModel.get());
  connect(m_pUi->listView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onEditFilter()));

  // connect buttons
  connect(m_pUi->addButton, SIGNAL(clicked()), this, SLOT(onAddFilter()));
  connect(m_pUi->copyButton, SIGNAL(clicked()), this, SLOT(onCopyFilter()));
  connect(m_pUi->editButton, SIGNAL(clicked()), this, SLOT(onEditFilter()));
  connect(m_pUi->deleteButton, SIGNAL(clicked()), this, SLOT(onDeleteFilter()));
  connect(m_pUi->exportButton, SIGNAL(clicked()), this, SLOT(onExportFilter()));
  connect(m_pUi->importButton, SIGNAL(clicked()), this, SLOT(onImportFilter()));
  connect(m_pUi->okButton, SIGNAL(clicked()), this, SLOT(onApplyFilter()));
  enableButtons(false);
}

MainDialog::~MainDialog()
{}

void MainDialog::onAddFilter()
{
  std::unique_ptr<FilterDialog> pFilterDialog(new FilterDialog(this));
  int exitValue = pFilterDialog->exec();
  if (exitValue != QDialog::Accepted)
    return;

  // store FilterData
  FilterData filterData = pFilterDialog->getFilterDescription();
  setUniqueName(filterData);
  m_filterDataArray.push_back(filterData);

  // add filter to the list
  QStandardItem* filterItem = new QStandardItem(filterData.m_filterName);
  m_pListModel->appendRow(filterItem);
  m_pUi->listView->setCurrentIndex(m_pListModel->indexFromItem(filterItem));

  // set buttons
  enableButtons(true);
}

void MainDialog::onEditFilter()
{
  // get filter item
  QModelIndex selectedItemIndex = m_pUi->listView->currentIndex();
  QStandardItem* pFilterItem = m_pListModel->itemFromIndex(selectedItemIndex);

  // show FilterDialog
  std::unique_ptr<FilterDialog> pFilterDialog(new FilterDialog(this, m_filterDataArray[selectedItemIndex.row()]));
  int exitValue = pFilterDialog->exec();
  if (exitValue != QDialog::Accepted)
    return;

  // update filter name
  FilterData filterData = pFilterDialog->getFilterDescription();
  const int filterItemRow = m_pListModel->indexFromItem(pFilterItem).row();
  if (m_filterDataArray[filterItemRow].m_filterName.compare(filterData.m_filterName) != 0)
  {
    setUniqueName(filterData);
    pFilterItem->setText(filterData.m_filterName);
  }

  // store FilterData
  m_filterDataArray[filterItemRow] = filterData;
}

void MainDialog::onDeleteFilter()
{
  QString filterName = " " + m_pListModel->itemFromIndex(m_pUi->listView->currentIndex())->text() + "?";
  QMessageBox deleteMessageBox(
    QMessageBox::Question,
    QApplication::translate("deleteMessageBox", "title"),
    QApplication::translate("deleteMessageBox", "text").append(filterName),
    QMessageBox::Ok | QMessageBox::Cancel,
    this);
  int result = deleteMessageBox.exec();
  if (result == QMessageBox::Cancel)
    return;

  // get filter item
  QModelIndex filterIndex = m_pUi->listView->currentIndex();
  assert(filterIndex.row() >= 0);

  // remove filter from tree
  const int filterRow = filterIndex.row();
  m_pListModel->removeRow(filterRow);

  // remove FilterData
  auto it = m_filterDataArray.begin() + filterRow;
  m_filterDataArray.erase(it);

  enableButtons(m_pListModel->rowCount() > 0);
  if (m_pListModel->rowCount() > 0)
    m_pUi->listView->setCurrentIndex(m_pListModel->index(std::min(filterRow, m_pListModel->rowCount() - 1), 0));
}

void MainDialog::onCopyFilter()
{
  const int filterRow = m_pUi->listView->currentIndex().row();
  FilterData filterCopy = m_filterDataArray[filterRow];
  setUniqueName(filterCopy);
  QStandardItem* copyItem = new QStandardItem(filterCopy.m_filterName);
  m_pListModel->appendRow(copyItem);
  m_filterDataArray.push_back(filterCopy);
}

void MainDialog::onApplyFilter()
{
  // divide modelObjects on 2 sets
  const int filterRow = m_pUi->listView->currentIndex().row();
  assert(filterRow >= 0);
  auto idCollection = collectObjects(m_filterDataArray[filterRow]);

  // apply selected action (hide or show)
  setObjectsVisibility(idCollection);
  return;
}

void MainDialog::onExportFilter()
{
  QFileDialog fileDialog(this, QApplication::translate("MainDialog", "Export filter"), "", "Renga Filter (*.rnf)");
  fileDialog.setAcceptMode(QFileDialog::AcceptSave);
  fileDialog.setFileMode(QFileDialog::ExistingFile);
  if (fileDialog.exec() == QDialog::Accepted)
  {
    QString fileName = fileDialog.selectedFiles().first();
    std::unique_ptr<QFile> filterFile(new QFile(fileName));
    if (!filterFile->open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QMessageBox exportMessageBox(
        QMessageBox::Icon::Critical,
        QApplication::translate("exportMessageBox", "title"),
        QApplication::translate("exportMessageBox", "openError"),
        QMessageBox::Ok,
        this);
      exportMessageBox.exec();
      return;
    }
    m_filterDataArray[m_pUi->listView->currentIndex().row()].exportData(filterFile.get());
  }
}

void MainDialog::onImportFilter()
{
  QFileDialog fileDialog(this, QApplication::translate("MainDialog", "Import filter"), "", "Renga Filter (*.rnf)");
  fileDialog.setFileMode(QFileDialog::ExistingFile);
  if (fileDialog.exec() == QDialog::Accepted)
  {
    QString fileName = fileDialog.selectedFiles().first();
    std::unique_ptr<QFile> filterFile(new QFile(fileName));
    if (!filterFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QMessageBox importMessageBox(
        QMessageBox::Icon::Critical,
        QApplication::translate("importMessageBox", "title"),
        QApplication::translate("importMessageBox", "openError"),
        QMessageBox::Ok,
        this);
      importMessageBox.exec();
      return;
    }
    FilterData filterData = FilterData::importData(filterFile.get());
    if (filterData.m_groupList.size() == 0) {
      QMessageBox importMessageBox(
        QMessageBox::Icon::Critical,
        QApplication::translate("importMessageBox", "title"),
        QApplication::translate("importMessageBox", "parseError"),
        QMessageBox::Ok,
        this);
      importMessageBox.exec();
      return;
    }
    setUniqueName(filterData);
    m_filterDataArray.push_back(filterData);

    QStandardItem* item = new QStandardItem(filterData.m_filterName);
    m_pListModel->appendRow(item);
    m_pUi->listView->setCurrentIndex(m_pListModel->indexFromItem(item));
    enableButtons(true);
  }
}

objectIdCollection MainDialog::collectObjects(const FilterData& data) {
  // collect 2 separate sets of objectId: first set is match to filter, but second not
  rengaapi::Model rengaProjectModel = rengaapi::Project::model();
  rengaapi::ModelObjectCollection objectCollection = rengaProjectModel.objects();
  rengaapi::ObjectIdCollection matchIdCollection;
  rengaapi::ObjectIdCollection notMatchIdCollection;

  // check all model objects
  for (auto pObject : objectCollection)
  {
    // apply all groups on object
    bool isObjectMatchFilter = false;
    for (auto& groupData : data.m_groupList)
    {
      if (groupData.m_groupType != pObject->type())
        continue;

      // apply all properties from group
      bool isObjectMatchGroup = true;
      for (auto& propertyData : groupData.m_propertyList)
      {
        bool isObjectMatchProperty;
        if (propertyData.m_value.length() == 0)
        {
          // empty value from user: means all objects match to this property
          isObjectMatchProperty = true;
        }
        else
        {
          // apply filter
          ObjectPropertyBuilderFactory m_propertyBuilderFactory;
          std::unique_ptr<ObjectPropertyBuilder> pObjectBuilder(m_propertyBuilderFactory.createBuilder(pObject->type()));
          isObjectMatchProperty = pObjectBuilder->isObjectMatchFilter(propertyData, pObject);
        }
        // if object does not match property -> object does not match group
        if (!isObjectMatchProperty)
        {
          isObjectMatchGroup = false;
          break;
        }
      }

      // if object match group -> object match filter
      if (isObjectMatchGroup)
      {
        isObjectMatchFilter = true;
        break;
      }
    }

    // check filter matching
    if (isObjectMatchFilter)
      matchIdCollection.add(pObject->objectId());
    else
      notMatchIdCollection.add(pObject->objectId());
  }

  return std::make_pair(matchIdCollection, notMatchIdCollection);
}

void MainDialog::setObjectsVisibility(const objectIdCollection& idCollection) {
  // show or hide matching objects
  bool isShow = m_pUi->izolateRadioButton->isChecked();

  rengaapi::View* pView = rengaapi::Application::activeView();
  switch (pView->type())
  {
  case rengaapi::ViewType::View3D:
    rengaapi::ObjectVisibility::setVisibleIn3DView(idCollection.first, isShow);
    rengaapi::ObjectVisibility::setVisibleIn3DView(idCollection.second, !isShow);
    break;
  case rengaapi::ViewType::Level:
  {
    rengaapi::ObjectId levelId = dynamic_cast<rengaapi::LevelView*>(pView)->levelId();
    rengaapi::ObjectVisibility::setVisibleOnLevel(idCollection.first, levelId, isShow);
    rengaapi::ObjectVisibility::setVisibleOnLevel(idCollection.second, levelId, !isShow);
    break;
  }
  default:
    break;
  }
}

// TODO: this should be a slot (or observer) for "selection changed" or "current index changed" or smth like that?
void MainDialog::enableButtons(bool isEnable)
{
  m_pUi->copyButton->setEnabled(isEnable);
  m_pUi->editButton->setEnabled(isEnable);
  m_pUi->deleteButton->setEnabled(isEnable);
  m_pUi->exportButton->setEnabled(isEnable);
  m_pUi->okButton->setEnabled(isEnable);
}

void MainDialog::setUniqueName(FilterData& data)
{
  std::map<int, QString> filterNames;
  for (int i = 0; i < m_pListModel->rowCount(); i++)
    filterNames.insert(std::make_pair(i, m_pListModel->item(i)->text()));
  std::wstring uniqueFilterName = CUniqueNameGenerator::generate(filterNames, data.m_filterName.toStdWString(), true);
  data.m_filterName = QString::fromStdWString(uniqueFilterName);
}

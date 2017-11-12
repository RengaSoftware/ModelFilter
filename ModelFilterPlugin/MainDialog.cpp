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
#include "ObjectFactory.h"
#include "FiltersManager.h"
#include "FilterAlgo.h"

#include <QtCore/QFile.h>
#include <QtCore/QStandardPaths.h>
#include <QtWidgets/QFileDialog.h>
#include <QtWidgets/QMessageBox.h>

#include <RengaAPI/Application.h>
#include <RengaAPI/LevelView.h>
#include <RengaAPI/Model.h>
#include <RengaAPI/ModelObjectTypes.h>
#include <RengaAPI/ObjectVisibility.h>
#include <RengaAPI/Project.h>

MainDialog::MainDialog(FiltersManager& filtersManager)
  : QDialog(nullptr, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint)
  , m_filtersManager(filtersManager)
{
  m_pUi.reset(new Ui::MainDialog());
  m_pUi->setupUi(this);

  // load list view
  m_pFiltersItemModel.reset(new QStandardItemModel);
  m_pUi->listView->setModel(m_pFiltersItemModel.get());
  connect(m_pUi->listView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onEditFilter()));
  connect(m_pUi->listView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(updateButtons(const QItemSelection&)));

  // connect buttons
  connect(m_pUi->addButton, SIGNAL(clicked()), this, SLOT(onAddFilter()));
  connect(m_pUi->copyButton, SIGNAL(clicked()), this, SLOT(onCopyFilter()));
  connect(m_pUi->editButton, SIGNAL(clicked()), this, SLOT(onEditFilter()));
  connect(m_pUi->deleteButton, SIGNAL(clicked()), this, SLOT(onDeleteFilter()));
  connect(m_pUi->exportButton, SIGNAL(clicked()), this, SLOT(onExportFilter()));
  connect(m_pUi->importButton, SIGNAL(clicked()), this, SLOT(onImportFilter()));
  connect(m_pUi->okButton, SIGNAL(clicked()), this, SLOT(onApplyFilter()));

  initFiltersItemModel();
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
  m_filtersManager.addFilter(filterData);

  // add filter to the list
  QStandardItem* filterItem = new QStandardItem(filterData.m_filterName);
  m_pFiltersItemModel->appendRow(filterItem);
  m_pUi->listView->setCurrentIndex(m_pFiltersItemModel->indexFromItem(filterItem));
}

void MainDialog::onEditFilter()
{
  // get filter item
  QModelIndex selectedItemIndex = m_pUi->listView->currentIndex();
  QStandardItem* pFilterItem = m_pFiltersItemModel->itemFromIndex(selectedItemIndex);

  // show FilterDialog
  std::unique_ptr<FilterDialog> pFilterDialog(new FilterDialog(this, m_filtersManager.filter(selectedItemIndex.row())));
  int exitValue = pFilterDialog->exec();
  if (exitValue != QDialog::Accepted)
    return;

  const int filterItemRow = m_pFiltersItemModel->indexFromItem(pFilterItem).row();
  
  // set unique filter name
  FilterData filterData = pFilterDialog->getFilterDescription();
  if (m_filtersManager.filter(filterItemRow).m_filterName.compare(filterData.m_filterName) != 0)
  {
    setUniqueName(filterData);
    pFilterItem->setText(filterData.m_filterName);
  }

  // update filter
  m_filtersManager.updateFilter(filterItemRow, filterData);
}

void MainDialog::onDeleteFilter()
{
  QString filterName = " " + m_pFiltersItemModel->itemFromIndex(m_pUi->listView->currentIndex())->text() + "?";
  QMessageBox deleteMessageBox(
    QMessageBox::Question,
    QApplication::translate("mf_deleteMessageBox", "title"),
    QApplication::translate("mf_deleteMessageBox", "text").append(filterName),
    QMessageBox::Ok | QMessageBox::Cancel,
    this);
  int exitValue = deleteMessageBox.exec();
  if (exitValue == QMessageBox::Cancel)
    return;

  // get filter item index
  QModelIndex filterIndex = m_pUi->listView->currentIndex();
  assert(filterIndex.row() >= 0);

  // remove filter item from tree
  const int filterRow = filterIndex.row();
  m_pFiltersItemModel->removeRow(filterRow);

  // delete filter
  m_filtersManager.deleteFilter(filterRow);
}

void MainDialog::onCopyFilter()
{
  // copy original filterData
  const int filterRow = m_pUi->listView->currentIndex().row();
  FilterData filterCopy = m_filtersManager.filter(filterRow);

  // set unique filter name
  setUniqueName(filterCopy);
  QStandardItem* copyItem = new QStandardItem(filterCopy.m_filterName);
  m_pFiltersItemModel->appendRow(copyItem);

  // store FilterData
  m_filtersManager.addFilter(filterCopy);
}

void MainDialog::onApplyFilter()
{
  // divide modelObjects on 2 sets
  const int filterIndex = m_pUi->listView->currentIndex().row();
  assert(filterIndex >= 0);
  const FilterData& filter = m_filtersManager.filter(filterIndex);

  // apply selected action
  if (m_pUi->selectRadioButton->isChecked())
  {
    SelectAlgo algo;
    algo.execute(filter);
  }
  else if (m_pUi->isolateRadioButton->isChecked())
  {
    IsolateAlgo algo;
    algo.execute(filter); 
  }
  else if (m_pUi->hideRadioButton->isChecked())
  {
    HideAlgo algo;
    algo.execute(filter);
  }
}

void MainDialog::onExportFilter()
{
  const int filterDataRow = m_pUi->listView->currentIndex().row();
  const FilterData& filterToExport = m_filtersManager.filter(filterDataRow);

  QFileDialog fileDialog(
    this,
    QApplication::translate("MainDialog", "Export filter"),
    filterToExport.m_filterName,
    QApplication::translate("MainDialog", "Filter"));
  fileDialog.setAcceptMode(QFileDialog::AcceptSave);
  fileDialog.setFileMode(QFileDialog::ExistingFile);
  int exitValue = fileDialog.exec();
  if (exitValue == QDialog::Accepted)
  {
    QString fileName = fileDialog.selectedFiles().first();
    std::unique_ptr<QFile> filterFile(new QFile(fileName));
    if (!filterFile->open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QMessageBox exportMessageBox(
        QMessageBox::Icon::Critical,
        QApplication::translate("mf_exportMessageBox", "title"),
        QApplication::translate("mf_exportMessageBox", "openError"),
        QMessageBox::Ok,
        this);
      exportMessageBox.exec();
      return;
    }
    filterToExport.exportData(filterFile.get());
  }
}

void MainDialog::onImportFilter()
{
  QFileDialog fileDialog(
    this,
    QApplication::translate("MainDialog", "Import filter"),
    "",
    QApplication::translate("MainDialog", "Filter"));
  fileDialog.setFileMode(QFileDialog::ExistingFile);
  int exitValue = fileDialog.exec();
  if (exitValue == QDialog::Accepted)
  {
    QString fileName = fileDialog.selectedFiles().first();
    std::unique_ptr<QFile> filterFile(new QFile(fileName));
    if (!filterFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QMessageBox importMessageBox(
        QMessageBox::Icon::Critical,
        QApplication::translate("mf_importMessageBox", "title"),
        QApplication::translate("mf_importMessageBox", "openError"),
        QMessageBox::Ok,
        this);
      importMessageBox.exec();
      return;
    }
    FilterData filterData = FilterData::importData(filterFile.get());
    if (!filterData.isValid()) {
      QMessageBox importMessageBox(
        QMessageBox::Icon::Critical,
        QApplication::translate("mf_importMessageBox", "title"),
        QApplication::translate("mf_importMessageBox", "parseError"),
        QMessageBox::Ok,
        this);
      importMessageBox.exec();
      return;
    }
    setUniqueName(filterData);
    m_filtersManager.addFilter(filterData);

    QStandardItem* item = new QStandardItem(filterData.m_filterName);
    m_pFiltersItemModel->appendRow(item);
    m_pUi->listView->setCurrentIndex(m_pFiltersItemModel->indexFromItem(item));
  }
}

void MainDialog::initFiltersItemModel()
{
  for (size_t i = 0; i < m_filtersManager.count(); ++i) 
  {
    QStandardItem* item = new QStandardItem(m_filtersManager.filter(i).m_filterName);
    m_pFiltersItemModel->appendRow(item);
  }

  if (m_pFiltersItemModel->rowCount() > 0)
  {
    m_pUi->listView->setCurrentIndex(m_pFiltersItemModel->index(0, 0));
    updateButtons(m_pUi->listView->selectionModel()->selection());
  }
}

void MainDialog::updateButtons(const QItemSelection& selectedItems)
{
  bool isEnable = selectedItems.size() > 0;
  m_pUi->copyButton->setEnabled(isEnable);
  m_pUi->editButton->setEnabled(isEnable);
  m_pUi->deleteButton->setEnabled(isEnable);
  m_pUi->exportButton->setEnabled(isEnable);
  m_pUi->okButton->setEnabled(isEnable);
}

void MainDialog::setUniqueName(FilterData& data)
{
  std::map<int, QString> filterNames;
  for (int i = 0; i < m_pFiltersItemModel->rowCount(); i++)
    filterNames.insert(std::make_pair(i, m_pFiltersItemModel->item(i)->text()));
  std::wstring uniqueFilterName = CUniqueNameGenerator::generate(filterNames, data.m_filterName.toStdWString(), true);
  data.m_filterName = QString::fromStdWString(uniqueFilterName);
}
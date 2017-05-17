//
// Copyright “Renga Software” LLC, 2016. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "DeleteDialog.h"

#include "ui_DeleteDialog.h"

#include <QtCore/QFile>

DeleteDialog::DeleteDialog()
  : QDialog(nullptr, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint)
{
  m_pUi.reset(new Ui::DeleteDialog());
  m_pUi->setupUi(this);
}

DeleteDialog::~DeleteDialog()
{

}
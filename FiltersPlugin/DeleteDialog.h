//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once

#include <QtCore/QTranslator>
#include <QtWidgets/QDialog>

namespace Ui
{
  class DeleteDialog;
}

class DeleteDialog : public QDialog
{
  Q_OBJECT

public:
  DeleteDialog();
  ~DeleteDialog();

  //private slots:

private:
  std::unique_ptr<Ui::DeleteDialog> m_pUi;
};

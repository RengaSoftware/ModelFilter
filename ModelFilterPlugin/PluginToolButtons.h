//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#pragma once
#include <QtCore/QObject>
#include <QtCore/QTranslator>

#include <RengaAPI/Image.h>
#include <RengaAPI/Toolbutton.h>

class FilterDialogButtonHandler;

class PluginToolButtons : public QObject
{
  Q_OBJECT

public:
  PluginToolButtons(const std::wstring& pluginPath);

signals:
  void filterButtonClicked();

private:
  rengaapi::ToolButton createFilterButton(const std::wstring& pluginPath);
  bool loadIcon(const std::wstring& iconPath, rengaapi::Image::Format iconFormat, rengaapi::Image& outIcon) const;

private:
  std::unique_ptr<FilterDialogButtonHandler> m_pFilterButtonHandler;
};
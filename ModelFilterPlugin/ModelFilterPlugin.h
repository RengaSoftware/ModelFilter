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
#include <QtCore/QDir>

#ifdef _DEBUG
#include <QtWidgets/QApplication>
#endif

#include <IPlugin.h>

class RengaEventsHandler;
class PluginToolButtons;
class FiltersManager;
class ContextMenu;

class ModelFilterPlugin : public QObject, public plugins::IPlugin
{
  Q_OBJECT

public:
  ModelFilterPlugin();
  ~ModelFilterPlugin();

  // plugins::IPlugin
  bool initialize(const wchar_t* pluginPath);
  void stop();

private slots:
  void onFilterButtonClicked();
  void onProjectAboutToClose();

private:
  void addPluginButtons(const std::wstring& pluginPath);
  void subscribeOnRengaEvents();
  void updateContextMenu();

private:
#ifdef _DEBUG
  // NOTE: To debug plugin you should create debug QApplication in plugin.
  std::unique_ptr<QApplication> m_pApp;
  int m_argc = 0;
  char** m_argv = 0;
  void createDebugApplicationInstance(const std::wstring& pluginPath);
#endif

  QTranslator m_translator;
  QDir m_pluginDataDir;

  std::unique_ptr<PluginToolButtons> m_pPluginToolButtons;
  std::unique_ptr<RengaEventsHandler> m_pRengaEventsHandler;
  std::unique_ptr<FiltersManager> m_pFiltersManager;
  std::unique_ptr<ContextMenu> m_pContextMenu;
};

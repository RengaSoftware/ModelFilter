//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "MainDialog.h"
#include "ModelFilterPlugin.h"
#include "PluginToolButtons.h"
#include "RengaEventsHandler.h"
#include "FiltersManager.h"
#include "ContextMenu.h"
#include "FilterAlgo.h"
#include "FilterContextMenuHandler.h"
#include "FilterContextMenuActionItem.h"

#include <QtCore/QFile.h>
#include <QtCore/QStandardPaths.h>
#include <QtWidgets/QApplication.h>

#include <RengaAPI/Localization.h>
#include <RengaAPI/Message.h>

static const QString russianLocaleFileName = "ModelFilterPlugin_ru.qm";
static const QString englishLocaleFileName = "ModelFilterPlugin_en.qm";
static const QString pluginSubPath = "ModelFilterPlugin";
static const QString defaultLocaleFileName = englishLocaleFileName;

static const QString c_russianLocale = "ru_RU";
static const QString c_englishLocale = "en_EN";

static const rengabase::String c_loadLocalizationFileError = L"Cannot open localization file.";
static const rengabase::String c_error = L"Error";

static const QString view3DContextMenuId = "C22DCE93 - 4B1D - 4746 - A464 - 77CDB3DDF368";

namespace
{
  QString translationFileName()
  {
    // get application locale
    QString appLocale = rengaStringToQString(rengaapi::Localization::currentLocale_());

    // get translation file name
    if (appLocale == c_russianLocale)
      return russianLocaleFileName;
    else if (appLocale == c_englishLocale)
      return englishLocaleFileName;
    else
      return defaultLocaleFileName;
  }

  bool loadAndInstallTranslator(const std::wstring& pluginPath, QTranslator& translator)
  {
    // load QTranslator
    if (!translator.load(translationFileName(), QString::fromStdWString(pluginPath)))
    {
      rengaapi::Message::showMessageBox(rengaapi::Message::Error, c_error, c_loadLocalizationFileError);
      return false;
    }
    else
    {
      QApplication::installTranslator(&translator);
      return true;
    }
  }

  bool getPluginDataDir(QDir& pluginDataDir)
  {
    QString dataLocationPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    QDir userDataDir(dataLocationPath);

    // create plugin folder if necessary
    if (userDataDir.entryList({ pluginSubPath }, QDir::Dirs).empty())
      if (!userDataDir.mkdir(pluginSubPath))
        return false;

    pluginDataDir = QDir(QString("%1/%2").arg(dataLocationPath).arg(pluginSubPath));
    return true;
  }
}

ModelFilterPlugin::ModelFilterPlugin()
{}

ModelFilterPlugin::~ModelFilterPlugin()
{}

#ifdef _DEBUG
void ModelFilterPlugin::createDebugApplicationInstance(const std::wstring & pluginPath)
{
  //   NOTE: To debug a plugin you should create a debug QApplication in the plugin, 
  //   put the debug Qt dlls into the plugin folder (Renga installation folder/Plugins/PluginName), 
  //   put qwindowsd.dll into the "platform" folder in the plugin folders

  QStringList paths = QCoreApplication::libraryPaths();
  paths.append(QString::fromStdWString(pluginPath));
  QCoreApplication::setLibraryPaths(paths);

  m_pApp.reset(new QApplication(m_argc, m_argv));
}
#endif

bool ModelFilterPlugin::initialize(const wchar_t* pluginPath)
{
#ifdef _DEBUG
  createDebugApplicationInstance(std::wstring(pluginPath));
#endif

  if (!loadAndInstallTranslator(pluginPath, m_translator))
    return false;

  if (!getPluginDataDir(m_pluginDataDir))
    return false;

  m_pFiltersManager.reset(new FiltersManager(m_pluginDataDir));
  
  updateContextMenu();
  subscribeOnRengaEvents();
  addPluginButtons(pluginPath);
  return true;
}

void ModelFilterPlugin::stop()
{
  m_pPluginToolButtons.reset(nullptr);
}

void ModelFilterPlugin::subscribeOnRengaEvents()
{
  m_pRengaEventsHandler.reset(new RengaEventsHandler());
  connect(m_pRengaEventsHandler.get(), SIGNAL(projectAboutToClose()), this, SLOT(onProjectAboutToClose()));
}

void ModelFilterPlugin::updateContextMenu()
{
  m_pContextMenu.reset(new ContextMenu(rengaapi::ViewType::View3D, rengaapi::ContextMenuShowCase::Scene, rengabase::UUID::fromString(QStringToRengaString(view3DContextMenuId))));

  ContextMenu::NodeItem* pIsolateSubTree = new ContextMenu::NodeItem(QApplication::translate("contextmenu", "Isolate").toStdWString());
  ContextMenu::NodeItem* pHideSubTree = new ContextMenu::NodeItem(QApplication::translate("contextmenu", "Hide").toStdWString());

  IFilterAlgoPtr isolateAlgo(new IsolateAlgo());
  IFilterAlgoPtr hideAlgo(new HideAlgo());

  for (size_t i = 0; i < m_pFiltersManager->count(); ++i)
  {
    const FilterData& filter = m_pFiltersManager->filter(i);
    std::wstring filterName = filter.m_filterName.toStdWString();

    FilterContextMenuHandlerPtr pIsolateHandler(new FilterContextMenuHandler(m_pFiltersManager->filter(i), isolateAlgo));
    pIsolateSubTree->add(new FilterContextMenuActionItem(filterName, pIsolateHandler));

    FilterContextMenuHandlerPtr pHideHandler(new FilterContextMenuHandler(m_pFiltersManager->filter(i), hideAlgo));
    pHideSubTree->add(new FilterContextMenuActionItem(filterName, pHideHandler));
  }

  m_pContextMenu->add(new ContextMenu::SeparatorItem());
  m_pContextMenu->add(pIsolateSubTree);
  m_pContextMenu->add(pHideSubTree);
  m_pContextMenu->add(new ContextMenu::SeparatorItem());
  m_pContextMenu->update();
}

void ModelFilterPlugin::addPluginButtons(const std::wstring& pluginPath)
{
  m_pPluginToolButtons.reset(new PluginToolButtons(pluginPath));
  connect(m_pPluginToolButtons.get(), SIGNAL(filterButtonClicked()), this, SLOT(onFilterButtonClicked()));
}

void ModelFilterPlugin::onFilterButtonClicked()
{
  MainDialog mainDialog(*m_pFiltersManager);
  if (mainDialog.exec())
    updateContextMenu();
}

void ModelFilterPlugin::onProjectAboutToClose()
{}

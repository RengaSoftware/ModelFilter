//
// Copyright �Renga Software� LLC, 2017. All rights reserved.
//
// �Renga Software� LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// �Renga Software� LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "MainDialog.h"
#include "FiltersPlugin.h"
#include "PluginToolButtons.h"
#include "RengaEventsHandler.h"

#include <QtWidgets/QApplication.h>

#include <RengaAPI/Localization.h>
#include <RengaAPI/Message.h>

static const QString russianLocaleFileName = "filtersplugin_ru.qm";
static const QString englishLocaleFileName = "filtersplugin_en.qm";
static const QString defaultLocaleFileName = englishLocaleFileName;

static const QString c_russianLocale = "ru_RU";
static const QString c_englishLocale = "en_EN";

static const rengabase::String c_loadLocalizationFileError = L"Cannot open localization file.";
static const rengabase::String c_error = L"Error";

FiltersPlugin::FiltersPlugin()
{
#ifdef _DEBUG
  //   NOTE: To debug a plugin you should create a debug QApplication in the plugin, 
  //   put the debug Qt dlls (can be found in separate repository) into the plugin folder (Renga installation folder/Plugins/PluginName), 
  //   put qwindowsd.dll into the "platform" folder in the plugin folder
  //   and set library paths to the application, as shown below

  //QStringList paths = QCoreApplication::libraryPaths();
  //paths.append("your plugin folder path");
  //QCoreApplication::setLibraryPaths(paths);

  //int argc = 0;
  //char** argv = 0;
  //m_pApp.reset(new QApplication(argc, argv));
#endif
}

FiltersPlugin::~FiltersPlugin()
{}

bool FiltersPlugin::initialize(const wchar_t* pluginPath)
{
  if (!loadTranslator(pluginPath))
    return false;

  m_pMainDialog.reset(new MainDialog());

  subscribeOnRengaEvents();
  addPluginButtons(pluginPath);

  return true;
}

void FiltersPlugin::stop()
{
  m_pPluginToolButtons.reset(nullptr);
  m_pMainDialog.reset(nullptr);
}

QString FiltersPlugin::translationFileName()
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

bool FiltersPlugin::loadTranslator(const std::wstring& pluginPath)
{
  // load QTranslator
  if (!m_translator.load(translationFileName(), QString::fromStdWString(pluginPath)))
  {
    rengaapi::Message::showMessageBox(rengaapi::Message::Error, c_error, c_loadLocalizationFileError);
    return false;
  }
  else
  {
    QApplication::installTranslator(&m_translator);
    return true;
  }
}

void FiltersPlugin::subscribeOnRengaEvents()
{
  m_pRengaEventsHandler.reset(new RengaEventsHandler());
  connect(m_pRengaEventsHandler.get(), SIGNAL(projectAboutToClose()), this, SLOT(onProjectAboutToClose()));
}

void FiltersPlugin::addPluginButtons(const std::wstring& pluginPath)
{
  m_pPluginToolButtons.reset(new PluginToolButtons(pluginPath));
  connect(m_pPluginToolButtons.get(), SIGNAL(filterButtonClicked()), this, SLOT(onFilterButtonClicked()));
}

void FiltersPlugin::onFilterButtonClicked()
{
  int result = m_pMainDialog->exec();
}

void FiltersPlugin::onProjectAboutToClose()
{}

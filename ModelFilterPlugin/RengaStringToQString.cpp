//
// Copyright “Renga Software” LLC, 2017. All rights reserved.
//
// “Renga Software” LLC PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// “Renga Software” LLC  DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//

#include "stdafx.h"
#include "RengaStringToQString.h"

QString rengaStringToQString(const rengabase::String& rengaString)
{
  return QString::fromStdWString(rengaString.data());
}

rengabase::String QStringToRengaString(const QString& sourceString)
{
  return rengabase::String(sourceString.toStdWString().data());
}
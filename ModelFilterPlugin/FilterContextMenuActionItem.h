#pragma once
#include "ContextMenuCommon.h"
#include "FilterContextMenuHandler.h"

class FilterContextMenuActionItem : public ActionItem
{
public:
  FilterContextMenuActionItem(const std::wstring& name, FilterContextMenuHandlerPtr handler);

private:
  FilterContextMenuHandlerPtr m_handler;
};
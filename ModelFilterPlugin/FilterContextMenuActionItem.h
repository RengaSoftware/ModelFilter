#pragma once
#include "ContextMenu.h"
#include "FilterContextMenuHandler.h"

class FilterContextMenuActionItem : public ContextMenu::ActionItem
{
public:
  FilterContextMenuActionItem(const std::wstring& name, FilterContextMenuHandlerPtr handler);

private:
  FilterContextMenuHandlerPtr m_handler;
};
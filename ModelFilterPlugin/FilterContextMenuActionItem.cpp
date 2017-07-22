#include "stdafx.h"
#include "FilterContextMenuActionItem.h"

FilterContextMenuActionItem::FilterContextMenuActionItem(const std::wstring& name, FilterContextMenuHandlerPtr handler):
  m_handler(handler)
{
  m_action.setDisplayName(rengabase::String(name.c_str()));
  m_action.setTriggerHandler(handler.get());
  
  setAction(m_action);
}

#include "stdafx.h"
#include "ContextMenu.h"

ContextMenu::BaseItem::~BaseItem()
{}

rengaapi::ContextMenuItem ContextMenu::SeparatorItem::createRengaItem() const
{
  return rengaapi::ContextMenuItem::makeSeparator();
}

ContextMenu::ActionItem::ActionItem()
  : m_action(rengaapi::UIControls::createAction())
{}

ContextMenu::ActionItem::ActionItem(const rengaapi::Action& action) :
  m_action(action)
{}

rengaapi::ContextMenuItem ContextMenu::ActionItem::createRengaItem() const
{
  return rengaapi::ContextMenuItem(m_action);
}

void ContextMenu::ActionItem::setAction(const rengaapi::Action & action)
{
  m_action = action;
}

const rengaapi::Action& ContextMenu::ActionItem::action() const
{
  return m_action;
}

ContextMenu::NodeItem::NodeItem(const std::wstring& name)
{
  m_name = name;
}

rengaapi::ContextMenuItem ContextMenu::NodeItem::createRengaItem() const
{
  m_childContextMenu = rengaapi::ContextMenuItemCollection();
  for (auto& item : m_childItemContainer)
    m_childContextMenu.addItem(item->createRengaItem());

  return rengaapi::ContextMenuItem(m_name.c_str(), &m_childContextMenu);
}

void ContextMenu::NodeItem::setName(const std::wstring & name)
{
  m_name = name;
}

const std::wstring & ContextMenu::NodeItem::name() const
{
  return m_name;
}

void ContextMenu::NodeItem::add(BaseItem* pItem)
{
  m_childItemContainer.emplace_back(std::unique_ptr<BaseItem>(pItem));
}

void ContextMenu::NodeItem::remove(const std::size_t n)
{
  m_childItemContainer.erase(m_childItemContainer.begin() + n);
}

std::size_t ContextMenu::NodeItem::size() const
{
  return m_childItemContainer.size();
}

ContextMenu::BaseItem* ContextMenu::NodeItem::get(const std::size_t n)
{
  return m_childItemContainer[n].get();
}

ContextMenu::ContextMenu(const rengaapi::ViewType viewType, const rengaapi::ContextMenuShowCase showCase, const rengabase::UUID& id) :
  m_id(id),
  m_viewType(viewType),
  m_showCase(showCase)
{
}

ContextMenu::ContextMenu(const rengaapi::ViewType viewType, const rengaapi::ContextMenuShowCase showCase) :
  m_id(rengabase::UUID::createUnique()),
  m_viewType(viewType),
  m_showCase(showCase)
{
}

void ContextMenu::add(BaseItem* pItem)
{
  m_itemContainer.emplace_back(std::unique_ptr<BaseItem>(pItem));
}

void ContextMenu::remove(size_t index)
{
  m_itemContainer.erase(m_itemContainer.begin() + index);
}

size_t ContextMenu::size() const
{
  return m_itemContainer.size();
}

void ContextMenu::update()
{
  m_contextMenu = rengaapi::ContextMenuItemCollection();

  for (auto& item : m_itemContainer)
    m_contextMenu.addItem(item->createRengaItem());

  rengaapi::UIControls::addContextMenuItems(m_id, m_contextMenu, m_viewType, m_showCase);
}
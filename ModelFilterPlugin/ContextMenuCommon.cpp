#include "stdafx.h"
#include "ContextMenuCommon.h"

#include <list>


BaseItem::BaseItem() :
  m_pParent(nullptr)
{
}

BaseItem::~BaseItem()
{
}

void BaseItem::setParent(const NodeItem* pParent)
{
  m_pParent = pParent;
}

rengaapi::ContextMenuItem SeparatorItem::createRengaItem() const
{
  return rengaapi::ContextMenuItem::makeSeparator();
}


ActionItem::ActionItem()
  : m_action(rengaapi::UIControls::createAction())
{
}

ActionItem::ActionItem(const rengaapi::Action& action) :
  m_action(action)
{}

rengaapi::ContextMenuItem ActionItem::createRengaItem() const
{
  return rengaapi::ContextMenuItem(m_action);
}

void ActionItem::setAction(const rengaapi::Action & action)
{
  m_action = action;
}

const rengaapi::Action& ActionItem::action() const
{
  return m_action;
}

NodeItem::NodeItem(const std::wstring& name)
{
  m_name = name;
}

rengaapi::ContextMenuItem NodeItem::createRengaItem() const
{
  m_childContextMenu = rengaapi::ContextMenuItemCollection();
  for (auto& item : m_childItemContainer)
    m_childContextMenu.addItem(item->createRengaItem());

  return rengaapi::ContextMenuItem(m_name.c_str(), &m_childContextMenu);
}

void NodeItem::setName(const std::wstring & name)
{
  m_name = name;
}

const std::wstring & NodeItem::name() const
{
  return m_name;
}

void NodeItem::add(BaseItem* pItem)
{
  m_childItemContainer.emplace_back(std::unique_ptr<BaseItem>(pItem));
  m_childItemContainer.back()->setParent(this);
}

void NodeItem::remove(const std::size_t n)
{
  m_childItemContainer.erase(m_childItemContainer.begin() + n);
}

std::size_t NodeItem::size() const
{
  return m_childItemContainer.size();
}

BaseItem* NodeItem::get(const std::size_t n)
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


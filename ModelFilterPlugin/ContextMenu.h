#pragma once

#include <vector>
#include <memory>

#include <RengaAPI/ContextMenuItem.h>
#include <RengaAPI/ContextMenuItemCollection.h>
#include <RengaAPI/UIControls.h>

class ContextMenu
{
public:

  class BaseItem
  {
  public:
    virtual ~BaseItem();
    virtual rengaapi::ContextMenuItem createRengaItem() const = 0;
  };

  class SeparatorItem : public BaseItem
  {
  public:
    virtual rengaapi::ContextMenuItem createRengaItem() const override;
  };

  class ActionItem : public BaseItem
  {
  public:
    ActionItem();
    ActionItem(const rengaapi::Action& action);

    void setAction(const rengaapi::Action& action);
    const rengaapi::Action& action() const;

    virtual rengaapi::ContextMenuItem createRengaItem() const override;

  protected:
    rengaapi::Action m_action;
  };

  class NodeItem : public BaseItem
  {
  public:
    NodeItem(const std::wstring& name);

    virtual rengaapi::ContextMenuItem createRengaItem() const override;

    void setName(const std::wstring& name);
    const std::wstring& name() const;

    void add(BaseItem* item);
    void remove(const std::size_t n);

    std::size_t size() const;
    BaseItem* get(const std::size_t n);

  protected:
    std::wstring m_name;
    std::vector<std::unique_ptr<BaseItem>> m_childItemContainer;

    mutable rengaapi::ContextMenuItemCollection m_childContextMenu;
  };

  ContextMenu(const rengaapi::ViewType viewType, const rengaapi::ContextMenuShowCase showCase, const rengabase::UUID& id);
  ContextMenu(const rengaapi::ViewType viewType, const rengaapi::ContextMenuShowCase showCase);
  void add(BaseItem* pItem);
  void remove(size_t index);
  size_t size() const;

  void update();

private:
  rengabase::UUID m_id;
  rengaapi::ViewType m_viewType;
  rengaapi::ContextMenuShowCase m_showCase;

  std::vector<std::unique_ptr<BaseItem>> m_itemContainer;
  rengaapi::ContextMenuItemCollection m_contextMenu;
};

#include "stdafx.h"
#include "FilterAlgo.h"

#include <RengaAPI/Application.h>
#include <RengaAPI/Project.h>
#include <RengaAPI/ModelObjectCollection.h>
#include <RengaAPI/Model.h>
#include <RengaAPI/ObjectVisibility.h>
#include <RengaAPI/LevelView.h>

#include "ObjectFactory.h"

namespace
{
  class FilterResult
  {
  public:
    rengaapi::ObjectIdCollection matchedIds;
    rengaapi::ObjectIdCollection notMatchedIds;
  };

  FilterResult collectObjects(const FilterData& data)
  {
    // collect 2 separate sets of objectId: first set is match to filter, but second not
    rengaapi::Model rengaProjectModel = rengaapi::Project::model();
    rengaapi::ModelObjectCollection objectCollection = rengaProjectModel.objects();
    rengaapi::ObjectIdCollection matchIdCollection;
    rengaapi::ObjectIdCollection notMatchIdCollection;

    // check all model objects
    for (auto pObject : objectCollection)
    {
      // apply all groups on object
      bool isObjectMatchFilter = false;
      for (auto& groupData : data.m_groupList)
      {
        if (groupData.m_groupType != pObject->type())
          continue;

        // apply all properties from group
        bool isObjectMatchGroup = true;
        for (auto& propertyData : groupData.m_propertyList)
        {
          // apply filter
          ObjectFilterFactory m_Factory;
          std::unique_ptr<ObjectFilter> pObjectBuilder(m_Factory.createObjectFilter(pObject->type()));
          bool isObjectMatchProperty = pObjectBuilder->isObjectMatchFilter(propertyData, pObject);
          // if object does not match property -> object does not match group
          if (!isObjectMatchProperty)
          {
            isObjectMatchGroup = false;
            break;
          }
        }
        // if object match group -> object match filter
        if (isObjectMatchGroup)
        {
          isObjectMatchFilter = true;
          break;
        }
      }

      // check filter matching
      if (isObjectMatchFilter)
        matchIdCollection.add(pObject->objectId());
      else
        notMatchIdCollection.add(pObject->objectId());
    }
    return FilterResult{ matchIdCollection, notMatchIdCollection };
  }

  void setObjectsVisibility(const FilterResult& filteredIds, bool isVisible)
  {
    rengaapi::View* pView = rengaapi::Application::activeView();
    switch (pView->type())
    {
    case rengaapi::ViewType::View3D:
      rengaapi::ObjectVisibility::setVisibleIn3DView(filteredIds.matchedIds, isVisible);
      rengaapi::ObjectVisibility::setVisibleIn3DView(filteredIds.notMatchedIds, !isVisible);
      break;
    case rengaapi::ViewType::Level:
    {
      rengaapi::ObjectId levelId = dynamic_cast<rengaapi::LevelView*>(pView)->levelId();
      rengaapi::ObjectVisibility::setVisibleOnLevel(filteredIds.matchedIds, levelId, isVisible);
      rengaapi::ObjectVisibility::setVisibleOnLevel(filteredIds.notMatchedIds, levelId, !isVisible);
      break;
    }
    default:
      break;
    }
  }
}

HideAlgo::HideAlgo()
{
}

void HideAlgo::execute(const FilterData& filter)
{
  FilterResult filterResult = collectObjects(filter);
  setObjectsVisibility(filterResult, false);
}

IsolateAlgo::IsolateAlgo()
{
}

void IsolateAlgo::execute(const FilterData& filter)
{
  FilterResult filterResult = collectObjects(filter);
  setObjectsVisibility(filterResult, true);
}

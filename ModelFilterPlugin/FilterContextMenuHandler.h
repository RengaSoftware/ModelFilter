#pragma once
#include <RengaAPI/IInvokable.h>

#include "FilterData.h"
#include "FilterAlgo.h"

class FilterContextMenuHandler : public rengaapi::IInvokable
{
public:
  FilterContextMenuHandler(const FilterData& filter, IFilterAlgoPtr filterAlgo);

  // rengaapi::IInvokable
  virtual void invoke();

private:
  FilterData m_filter;
  IFilterAlgoPtr m_filterAlgo;
};

typedef std::shared_ptr<FilterContextMenuHandler> FilterContextMenuHandlerPtr;
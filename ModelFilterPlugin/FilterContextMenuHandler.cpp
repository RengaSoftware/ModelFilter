#include "stdafx.h"
#include "FilterContextMenuHandler.h"

FilterContextMenuHandler::FilterContextMenuHandler(const FilterData & filter, IFilterAlgoPtr filterAlgo):
  m_filter(filter),
  m_filterAlgo(filterAlgo)
{
}

void FilterContextMenuHandler::invoke()
{
  m_filterAlgo->execute(m_filter);
}

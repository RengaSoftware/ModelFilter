#pragma once

struct FilterData;

class IFilteringAlgo
{
public:
  virtual void execute(const FilterData& filter) = 0;
};

class HideAlgo : public IFilteringAlgo
{
public:
  HideAlgo();
  void execute(const FilterData& filter) override;
};

class IsolateAlgo : public IFilteringAlgo
{
public:
  IsolateAlgo();
  void execute(const FilterData& filter) override;
};
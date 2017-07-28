#pragma once

struct FilterData;

class IFilterAlgo
{
public:
  virtual void execute(const FilterData& filter) = 0;
};

typedef std::shared_ptr<IFilterAlgo> IFilterAlgoPtr;

class HideAlgo : public IFilterAlgo
{
public:
  HideAlgo();
  void execute(const FilterData& filter) override;
};

class IsolateAlgo : public IFilterAlgo
{
public:
  IsolateAlgo();
  void execute(const FilterData& filter) override;
};

class SelectAlgo : public IFilterAlgo
{
public:
  SelectAlgo();
  void execute(const FilterData& filter) override;
};
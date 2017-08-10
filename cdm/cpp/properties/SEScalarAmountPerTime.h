/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#pragma once
#include "properties/SEScalar.h"

class CDM_DECL AmountPerTimeUnit : public CCompoundUnit
{
public:
  AmountPerTimeUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~AmountPerTimeUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const AmountPerTimeUnit& GetCompoundUnit(const std::string& unit);


  static AmountPerTimeUnit mol_Per_day;
  static AmountPerTimeUnit mol_Per_s;
  static AmountPerTimeUnit umol_Per_s;
  static AmountPerTimeUnit mmol_Per_min;
  static AmountPerTimeUnit pmol_Per_min;
  static AmountPerTimeUnit umol_Per_min;
};

class CDM_DECL SEScalarAmountPerTime : public SEScalarQuantity<AmountPerTimeUnit>
{

public:
  SEScalarAmountPerTime() {}
  virtual ~SEScalarAmountPerTime() {}

  static void Load(const cdm::ScalarAmountPerTimeData& src, SEScalarAmountPerTime& dst);
  static cdm::ScalarAmountPerTimeData* Unload(const SEScalarAmountPerTime& src);
protected:
  static void Serialize(const cdm::ScalarAmountPerTimeData& src, SEScalarAmountPerTime& dst);
  static void Serialize(const SEScalarAmountPerTime& src, cdm::ScalarAmountPerTimeData& dst);
};

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

class CDM_DECL ElectricResistanceUnit : public CCompoundUnit
{
public:
  ElectricResistanceUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~ElectricResistanceUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const ElectricResistanceUnit& GetCompoundUnit(const std::string& unit);

  static const ElectricResistanceUnit Ohm;
};

class CDM_DECL SEScalarElectricResistance : public SEScalarQuantity<ElectricResistanceUnit>
{
public:
  SEScalarElectricResistance() {}
  virtual ~SEScalarElectricResistance() {}

  static void Load(const cdm::ScalarElectricResistanceData& src, SEScalarElectricResistance& dst);
  static cdm::ScalarElectricResistanceData* Unload(const SEScalarElectricResistance& src);
protected:
  static void Serialize(const cdm::ScalarElectricResistanceData& src, SEScalarElectricResistance& dst);
  static void Serialize(const SEScalarElectricResistance& src, cdm::ScalarElectricResistanceData& dst);
};

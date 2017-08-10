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

class CDM_DECL MassPerAreaTimeUnit : public CCompoundUnit
{
public:
  MassPerAreaTimeUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~MassPerAreaTimeUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const MassPerAreaTimeUnit& GetCompoundUnit(const std::string& unit);

  static const MassPerAreaTimeUnit g_Per_cm2_s;
};

class CDM_DECL SEScalarMassPerAreaTime : public SEScalarQuantity<MassPerAreaTimeUnit>
{
public:
  SEScalarMassPerAreaTime() {}
  virtual ~SEScalarMassPerAreaTime() {}

  static void Load(const cdm::ScalarMassPerAreaTimeData& src, SEScalarMassPerAreaTime& dst);
  static cdm::ScalarMassPerAreaTimeData* Unload(const SEScalarMassPerAreaTime& src);
protected:
  static void Serialize(const cdm::ScalarMassPerAreaTimeData& src, SEScalarMassPerAreaTime& dst);
  static void Serialize(const SEScalarMassPerAreaTime& src, cdm::ScalarMassPerAreaTimeData& dst);
};


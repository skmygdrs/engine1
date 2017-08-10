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
#include "patient/actions/SEConsciousRespirationCommand.h"

class CDM_DECL SEForcedExhale : public SEConsciousRespirationCommand
{
  friend class SEConsciousRespiration;
  SEForcedExhale();
public:

  virtual ~SEForcedExhale();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  static void Load(const cdm::ConsciousRespirationData_ForcedExhaleData& src, SEForcedExhale& dst);
  static cdm::ConsciousRespirationData_ForcedExhaleData* Unload(const SEForcedExhale& src);
protected:
  static void Serialize(const cdm::ConsciousRespirationData_ForcedExhaleData& src, SEForcedExhale& dst);
  static void Serialize(const SEForcedExhale& src, cdm::ConsciousRespirationData_ForcedExhaleData& dst);

public:

  virtual void ToString(std::ostream &str) const;

  virtual bool HasExpiratoryReserveVolumeFraction() const;
  virtual SEScalar0To1& GetExpiratoryReserveVolumeFraction();

  virtual bool HasPeriod() const;
  virtual SEScalarTime& GetPeriod();
protected:
  SEScalar0To1* m_ExpiratoryReserveVolumeFraction;
  SEScalarTime* m_Period;
};  
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
#include "bind/cdm/Substance.pb.h"

class CDM_DECL SESubstanceTissuePharmacokinetics : public Loggable
{
public:

  SESubstanceTissuePharmacokinetics(const std::string& name, Logger* logger);
  virtual ~SESubstanceTissuePharmacokinetics();

  virtual void Clear();

  static void Load(const cdm::SubstanceData_TissuePharmacokineticsData& src, SESubstanceTissuePharmacokinetics& dst);
  static cdm::SubstanceData_TissuePharmacokineticsData* Unload(const SESubstanceTissuePharmacokinetics& src);
protected:
  static void Serialize(const cdm::SubstanceData_TissuePharmacokineticsData& src, SESubstanceTissuePharmacokinetics& dst);
  static void Serialize(const SESubstanceTissuePharmacokinetics& src, cdm::SubstanceData_TissuePharmacokineticsData& dst);

public:
  const SEScalar* GetScalar(const std::string& name);

  virtual std::string GetName() const;

  virtual bool HasPartitionCoefficient() const;
  virtual SEScalar& GetPartitionCoefficient();
  virtual double GetPartitionCoefficient() const;


protected:
  std::string    m_Name;
  SEScalar*     m_PartitionCoefficient;
};
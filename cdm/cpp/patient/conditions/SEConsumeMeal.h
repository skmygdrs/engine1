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
#include "patient/conditions/SEPatientCondition.h"
#include "patient/SEMeal.h"

class CDM_DECL SEConsumeMeal : public SEPatientCondition
{
public:

  SEConsumeMeal();
  virtual ~SEConsumeMeal();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;

  static void Load(const cdm::ConsumeMealData& src, SEConsumeMeal& dst);
  static cdm::ConsumeMealData* Unload(const SEConsumeMeal& src);
protected:
  static void Serialize(const cdm::ConsumeMealData& src, SEConsumeMeal& dst);
  static void Serialize(const SEConsumeMeal& src, cdm::ConsumeMealData& dst);

public:
  virtual std::string GetName() const { return "ConsumeMeal"; }

  bool HasMeal() const;
  SEMeal& GetMeal();
  const SEMeal* GetMeal() const;

  virtual std::string GetMealFile() const;
  virtual void SetMealFile(const std::string& fileName);
  virtual bool HasMealFile() const;
  virtual void InvalidateMealFile();

  virtual void ToString(std::ostream &str) const;

protected:
  SEMeal*      m_Meal;
  std::string  m_MealFile;
};

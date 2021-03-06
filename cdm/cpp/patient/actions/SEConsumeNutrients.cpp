/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEConsumeNutrients.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerTime.h"
#include "properties/SEScalarVolume.h"
PROTO_PUSH
#include "bind/cdm/PatientActions.pb.h"
PROTO_POP

SEConsumeNutrients::SEConsumeNutrients() : SEPatientAction()
{
  m_Nutrition = nullptr;
  InvalidateNutritionFile();
}

SEConsumeNutrients::~SEConsumeNutrients()
{
  Clear();
}

void SEConsumeNutrients::Clear()
{
  SEPatientAction::Clear();
  SAFE_DELETE(m_Nutrition);
  InvalidateNutritionFile();
}

bool SEConsumeNutrients::IsValid() const
{
  return SEPatientAction::IsValid() && (HasNutrition() || HasNutritionFile());
}

bool SEConsumeNutrients::IsActive() const
{
  return IsValid();
}

void SEConsumeNutrients::Load(const cdm::ConsumeNutrientsData& src, SEConsumeNutrients& dst)
{
  SEConsumeNutrients::Serialize(src, dst);
}
void SEConsumeNutrients::Serialize(const cdm::ConsumeNutrientsData& src, SEConsumeNutrients& dst)
{
  SEPatientAction::Serialize(src.patientaction(), dst);
  if (src.has_nutrition())
    SENutrition::Load(src.nutrition(), dst.GetNutrition());
  else
    dst.SetNutritionFile(src.nutritionfile());
}

cdm::ConsumeNutrientsData* SEConsumeNutrients::Unload(const SEConsumeNutrients& src)
{
  cdm::ConsumeNutrientsData* dst = new cdm::ConsumeNutrientsData();
  SEConsumeNutrients::Serialize(src, *dst);
  return dst;
}
void SEConsumeNutrients::Serialize(const SEConsumeNutrients& src, cdm::ConsumeNutrientsData& dst)
{
  SEPatientAction::Serialize(src, *dst.mutable_patientaction());
  if (src.HasNutrition())
    dst.set_allocated_nutrition(SENutrition::Unload(*src.m_Nutrition));
  else if (src.HasNutritionFile())
    dst.set_nutritionfile(src.m_NutritionFile);
}

bool SEConsumeNutrients::HasNutrition() const
{
  return m_Nutrition != nullptr;
}
SENutrition& SEConsumeNutrients::GetNutrition()
{
  m_NutritionFile = "";
  if (m_Nutrition == nullptr)
    m_Nutrition = new SENutrition(nullptr);
  return *m_Nutrition;
}
const SENutrition* SEConsumeNutrients::GetNutrition() const
{
  return m_Nutrition;
}

std::string SEConsumeNutrients::GetNutritionFile() const
{
  return m_NutritionFile;
}
void SEConsumeNutrients::SetNutritionFile(const std::string& fileName)
{
  m_NutritionFile = fileName;
  if (m_Nutrition != nullptr)
    SAFE_DELETE(m_Nutrition);
}
bool SEConsumeNutrients::HasNutritionFile() const
{
  return m_NutritionFile.empty() ? false : true;
}
void SEConsumeNutrients::InvalidateNutritionFile()
{
  m_NutritionFile = "";
}

void SEConsumeNutrients::ToString(std::ostream &str) const
{
  str << "Patient Action : Consume Nutrients"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  if (HasNutrition())
  {
    str << "\n\tCharbohydrates: "; m_Nutrition->HasCarbohydrate() ? str << m_Nutrition->GetCarbohydrate() : str << "None";
    str << "\n\tCharbohydrates Digestion Rate: "; m_Nutrition->HasCarbohydrateDigestionRate() ? str << m_Nutrition->GetCarbohydrateDigestionRate() : str << "None";
    str << "\n\tFat: "; m_Nutrition->HasFat() ? str << m_Nutrition->GetFat() : str << "None";
    str << "\n\tFat Digestion Rate: "; m_Nutrition->HasFatDigestionRate() ? str << m_Nutrition->GetFatDigestionRate() : str << "None";
    str << "\n\tProtein: "; m_Nutrition->HasProtein() ? str << m_Nutrition->GetProtein() : str << "None";
    str << "\n\tProtein Digestion Rate: "; m_Nutrition->HasProteinDigestionRate() ? str << m_Nutrition->GetProteinDigestionRate() : str << "None";
    str << "\n\tCalcium: "; m_Nutrition->HasCalcium() ? str << m_Nutrition->GetCalcium() : str << "None";
    str << "\n\tSodium: "; m_Nutrition->HasSodium() ? str << m_Nutrition->GetSodium() : str << "None";
    str << "\n\tWater: "; m_Nutrition->HasWater() ? str << m_Nutrition->GetWater() : str << "None";
  }
  if (HasNutritionFile())
    str << "\n\tNutrition File: "; str << m_NutritionFile;
  str << std::flush;
}
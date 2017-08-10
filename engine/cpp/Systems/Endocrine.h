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

#include "../Controller/PulseSystem.h"
#include "system/physiology/SEEndocrineSystem.h"
#include "properties/SEScalarMass.h"

/**
 * @brief @copydoc Physiology_EndocrineSystemData
 * @details
 * The Pulse� endocrine system is a rudimentary system with only one stimulus (increased carbon dioxide partial pressure in the blood stream)
 * and two hormones (epinephrine and norepinephrine). The release of the hormones in response to the stimuli to represent the response of the sympathetic nervous system.
 * In the future, additional stimuli and additional hormones will be added.
 */  
class PULSE_DECL Endocrine : public SEEndocrineSystem, public PulseSystem
{
  friend Pulse;
  friend class PulseEngineTest;
protected:
  Endocrine(Pulse& bg);
  Pulse& m_data;

public:
  virtual ~Endocrine();

  void Clear();

  // Set members to a stable homeostatic state
  void Initialize();

  static void Load(const pulse::EndocrineSystemData& src, Endocrine& dst);
  static pulse::EndocrineSystemData* Unload(const Endocrine& src);
protected:
  static void Serialize(const pulse::EndocrineSystemData& src, Endocrine& dst);
  static void Serialize(const Endocrine& src, pulse::EndocrineSystemData& dst);

  // Set pointers and other member variables common to both homeostatic initialization and loading a state
  void SetUp();

public:
  void AtSteadyState();
  void PreProcess(){}
  void Process();
  void PostProcess(){}

private:
  void SynthesizeInsulin();
  void ReleaseEpinephrine();

  // Serializable member variables (Set in Initialize and in schema)

  // Stateless member variable (Set in SetUp())
  double                     m_dt_s;
  double                     m_insulinMolarMass_g_Per_mol;
  SELiquidSubstanceQuantity* m_aortaGlucose = nullptr;
  SELiquidSubstanceQuantity* m_aortaEpinephrine = nullptr;
  SELiquidSubstanceQuantity* m_rKidneyEpinephrine = nullptr;  //Don't forget to tell amb we added these
  SELiquidSubstanceQuantity* m_lKidneyEpinephrine = nullptr;
  SELiquidSubstanceQuantity* m_splanchnicInsulin = nullptr;
};

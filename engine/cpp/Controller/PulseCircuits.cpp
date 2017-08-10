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

#include "stdafx.h"
#include "Pulse.h"

PulseCircuits::PulseCircuits(Pulse& bg) : SECircuitManager(bg.GetLogger()), m_data(bg)
{
  Clear();
}

PulseCircuits::~PulseCircuits()
{
  Clear();
}

void PulseCircuits::Clear()
{
  SECircuitManager::Clear(); 
  m_CombinedCardiovascularCircuit = nullptr;
  m_CardiovascularCircuit = nullptr;
  m_RenalCircuit = nullptr;
  m_RespiratoryCircuit = nullptr;
  m_AnesthesiaMachineCircuit = nullptr;
  m_CombinedRespiratoryAnesthesiaCircuit = nullptr;
  m_CombinedRespiratoryInhalerCircuit = nullptr;
  m_CombinedRespiratoryMechanicalVentilatorCircuit = nullptr;
  m_TemperatureCircuit = nullptr;
  m_InternalTemperatureCircuit = nullptr;
  m_ExternalTemperatureCircuit = nullptr;
}

void PulseCircuits::StateChange()
{
  m_CombinedCardiovascularCircuit = GetFluidCircuit(BGE::Circuits::FullCardiovascular);
  if (m_CombinedCardiovascularCircuit == nullptr)
  {
    Error("PulseCircuits::Load could not find circuit : " + std::string(BGE::Circuits::FullCardiovascular));
  }
  m_CardiovascularCircuit = GetFluidCircuit(BGE::Circuits::Cardiovascular);
  if (m_CardiovascularCircuit == nullptr)
  {
    Error("PulseCircuits::Load could not find circuit : " + std::string(BGE::Circuits::Cardiovascular));
  }
  m_RenalCircuit = GetFluidCircuit(BGE::Circuits::Renal);
  if (m_RenalCircuit == nullptr)
  {
    Error("PulseCircuits::Load could not find circuit : " + std::string(BGE::Circuits::Renal));
  }
  m_RespiratoryCircuit = GetFluidCircuit(BGE::Circuits::Respiratory);
  if (m_RespiratoryCircuit == nullptr)
  {
    Error("PulseCircuits::Load could not find circuit : " + std::string(BGE::Circuits::Respiratory));
  }
  m_AnesthesiaMachineCircuit = GetFluidCircuit(BGE::Circuits::AnesthesiaMachine);
  if (m_AnesthesiaMachineCircuit == nullptr)
  {
    Error("PulseCircuits::Load could not find circuit : " + std::string(BGE::Circuits::AnesthesiaMachine));
  }
  m_CombinedRespiratoryAnesthesiaCircuit = GetFluidCircuit(BGE::Circuits::RespiratoryAnesthesia);
  if (m_CombinedRespiratoryAnesthesiaCircuit == nullptr)
  {
    Error("PulseCircuits::Load could not find circuit : " + std::string(BGE::Circuits::RespiratoryAnesthesia));
  }
  m_CombinedRespiratoryInhalerCircuit = GetFluidCircuit(BGE::Circuits::RespiratoryInhaler);
  if (m_CombinedRespiratoryInhalerCircuit == nullptr)
  {
    Error("PulseCircuits::Load could not find circuit : " + std::string(BGE::Circuits::RespiratoryInhaler));
  }
  m_CombinedRespiratoryMechanicalVentilatorCircuit = GetFluidCircuit(BGE::Circuits::RespiratoryMechanicalVentilator);
  if (m_CombinedRespiratoryMechanicalVentilatorCircuit == nullptr)
  {
    Error("PulseCircuits::Load could not find circuit : " + std::string(BGE::Circuits::RespiratoryMechanicalVentilator));
  }
  m_TemperatureCircuit = GetThermalCircuit(BGE::Circuits::Temperature);
  if (m_TemperatureCircuit == nullptr)
  {
    Error("PulseCircuits::Load could not find circuit : " + std::string(BGE::Circuits::Temperature));
  }
  m_InternalTemperatureCircuit = GetThermalCircuit(BGE::Circuits::InternalTemperature);
  if (m_InternalTemperatureCircuit == nullptr)
  {
    Error("PulseCircuits::Load could not find circuit : " + std::string(BGE::Circuits::InternalTemperature));
  }
  m_ExternalTemperatureCircuit = GetThermalCircuit(BGE::Circuits::ExternalTemperature);
  if (m_ExternalTemperatureCircuit == nullptr)
  {
    Error("PulseCircuits::Load could not find circuit : " + std::string(BGE::Circuits::ExternalTemperature));
  }
}  

void PulseCircuits::SetReadOnlyFluid(bool b)
{
  SECircuitManager::SetReadOnlyFluid(b);
  // These don't need to be locked
  GetFluidNode(BGE::EnvironmentNode::Ambient)->GetPressure().SetReadOnly(false);
  GetFluidNode(BGE::EnvironmentNode::Ambient)->GetNextPressure().SetReadOnly(false);
  GetFluidNode(BGE::RespiratoryNode::Stomach)->GetPressure().SetReadOnly(false);
  GetFluidNode(BGE::RespiratoryNode::Stomach)->GetNextPressure().SetReadOnly(false);
  GetFluidNode(BGE::InhalerNode::Mouthpiece)->GetPressure().SetReadOnly(false);
  GetFluidNode(BGE::InhalerNode::Mouthpiece)->GetNextPressure().SetReadOnly(false);  
}

SEFluidCircuit& PulseCircuits::GetActiveCardiovascularCircuit()
{
  if (m_CombinedCardiovascularCircuit == nullptr)
    m_CombinedCardiovascularCircuit = &CreateFluidCircuit(BGE::Circuits::FullCardiovascular);
  return *m_CombinedCardiovascularCircuit;
}
SEFluidCircuit& PulseCircuits::GetCardiovascularCircuit()
{
  if (m_CardiovascularCircuit == nullptr)
    m_CardiovascularCircuit = &CreateFluidCircuit(BGE::Circuits::Cardiovascular);
  return *m_CardiovascularCircuit;
}
SEFluidCircuit& PulseCircuits::GetRenalCircuit()
{
  if (m_RenalCircuit == nullptr)
    m_RenalCircuit = &CreateFluidCircuit(BGE::Circuits::Renal);
  return *m_RenalCircuit;
}

SEFluidCircuit& PulseCircuits::GetActiveRespiratoryCircuit()
{
  switch (m_data.GetAirwayMode())
  {
  case pulse::eAirwayMode::Free:
    return *m_RespiratoryCircuit;
  case pulse::eAirwayMode::AnesthesiaMachine:
    return *m_CombinedRespiratoryAnesthesiaCircuit;
  case pulse::eAirwayMode::Inhaler:
    return *m_CombinedRespiratoryInhalerCircuit;
  case pulse::eAirwayMode::MechanicalVentilator:
    return *m_CombinedRespiratoryMechanicalVentilatorCircuit;
  default:
    throw CommonDataModelException("Unknown airway mode");
  }
}
SEFluidCircuit& PulseCircuits::GetRespiratoryCircuit()
{
  if (m_RespiratoryCircuit == nullptr)
    m_RespiratoryCircuit = &CreateFluidCircuit(BGE::Circuits::Respiratory);
  return *m_RespiratoryCircuit;
}
SEFluidCircuit& PulseCircuits::GetAnesthesiaMachineCircuit()
{
  if (m_AnesthesiaMachineCircuit == nullptr)
    m_AnesthesiaMachineCircuit = &CreateFluidCircuit(BGE::Circuits::AnesthesiaMachine);
  return *m_AnesthesiaMachineCircuit;
}
SEFluidCircuit& PulseCircuits::GetRespiratoryAndAnesthesiaMachineCircuit()
{
  if (m_CombinedRespiratoryAnesthesiaCircuit == nullptr)
    m_CombinedRespiratoryAnesthesiaCircuit = &CreateFluidCircuit(BGE::Circuits::RespiratoryAnesthesia);
  return *m_CombinedRespiratoryAnesthesiaCircuit;
}
SEFluidCircuit& PulseCircuits::GetRespiratoryAndInhalerCircuit()
{
  if (m_CombinedRespiratoryInhalerCircuit == nullptr)
    m_CombinedRespiratoryInhalerCircuit = &CreateFluidCircuit(BGE::Circuits::RespiratoryInhaler);
  return *m_CombinedRespiratoryInhalerCircuit;
}
SEFluidCircuit& PulseCircuits::GetRespiratoryAndMechanicalVentilatorCircuit()
{
  if (m_CombinedRespiratoryMechanicalVentilatorCircuit == nullptr)
    m_CombinedRespiratoryMechanicalVentilatorCircuit = &CreateFluidCircuit(BGE::Circuits::RespiratoryMechanicalVentilator);
  return *m_CombinedRespiratoryMechanicalVentilatorCircuit;
}
SEThermalCircuit& PulseCircuits::GetTemperatureCircuit()
{
  if (m_TemperatureCircuit == nullptr)
    m_TemperatureCircuit = &CreateThermalCircuit(BGE::Circuits::Temperature);
  return *m_TemperatureCircuit;
}
SEThermalCircuit& PulseCircuits::GetInternalTemperatureCircuit()
{
  if (m_InternalTemperatureCircuit == nullptr)
    m_InternalTemperatureCircuit = &CreateThermalCircuit(BGE::Circuits::InternalTemperature);
  return *m_InternalTemperatureCircuit;
}
SEThermalCircuit& PulseCircuits::GetExternalTemperatureCircuit()
{
  if (m_ExternalTemperatureCircuit == nullptr)
    m_ExternalTemperatureCircuit = &CreateThermalCircuit(BGE::Circuits::ExternalTemperature);
  return *m_ExternalTemperatureCircuit;
}

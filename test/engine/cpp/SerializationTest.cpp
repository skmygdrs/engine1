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

#include "EngineTest.h"
#include "utils/FileUtils.h"

#include "engine/SETimedStabilization.h"
#include "engine/SEEngineTracker.h"
#include "scenario/SEDataRequestManager.h"

#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"

#include "patient/actions/SEConsciousRespiration.h"
#include "patient/actions/SEForcedExhale.h"
#include "patient/actions/SEForcedInhale.h"
#include "patient/actions/SEUseInhaler.h"
#include "patient/actions/SEBreathHold.h"
#include "patient/actions/SESubstanceBolus.h"

#include "system/equipment/Anesthesia/SEAnesthesiaMachine.h"
#include "system/equipment/Anesthesia/SEAnesthesiaMachineOxygenBottle.h"

#include "properties/SEScalar0To1.h"
#include "properties/SEScalarElectricPotential.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarHeatResistanceArea.h"
#include "properties/SEScalarLengthPerTime.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarTemperature.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEFunctionVolumeVsTime.h"
#include "properties/SEScalarPower.h"
#include "properties/SEScalarFlowResistance.h"

class HowToTracker
{
private:
  double m_dT_s;  // Cached Engine Time Step
  PhysiologyEngine& m_Engine;
public:
  HowToTracker(PhysiologyEngine& engine) : m_Engine(engine)
  {
    m_dT_s = m_Engine.GetTimeStep(TimeUnit::s);
  }
  ~HowToTracker(){}

  // This class will operate on seconds
  void AdvanceModelTime(double time_s)
  {
    int count = static_cast<int>(time_s / m_dT_s);
    for (int i = 0; i <= count; i++)
    {
      m_Engine.AdvanceModelTime();  // Compute 1 time step

      // Pull Track will pull data from the engine and append it to the file
      m_Engine.GetEngineTracker()->TrackData(m_Engine.GetSimulationTime(TimeUnit::s));
    }
  }
};

void PulseEngineTest::InhalerState(PhysiologyEngine* bg, HowToTracker& tracker)
{
  bg->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("InhalerResults.txt");
  if (!bg->InitializeEngine("StandardMale.pba"))
  {
    std::cerr << "Could not load initialize engine, check the error" << std::endl;
    return;
  }

  tracker.AdvanceModelTime(30);

  SEInhalerConfiguration config(bg->GetSubstanceManager());
  config.GetConfiguration().SetSubstance(bg->GetSubstanceManager().GetSubstance("Albuterol"));
  config.GetConfiguration().GetMeteredDose().SetValue(90.0, MassUnit::ug);
  config.GetConfiguration().GetNozzleLoss().SetValue(0.040);
  bg->ProcessAction(config);

  SEConsciousRespiration cResp;
 
  SEForcedExhale& exhale = cResp.AddForcedExhale();
  exhale.GetExpiratoryReserveVolumeFraction().SetValue(1.0);
  exhale.GetPeriod().SetValue(3.0, TimeUnit::s);  

  SEForcedInhale& inhale = cResp.AddForcedInhale();
  inhale.GetInspiratoryCapacityFraction().SetValue(1.0);
  inhale.GetPeriod().SetValue(5.0, TimeUnit::s);

  SEUseInhaler& inhaler = cResp.AddUseInhaler();

  SEBreathHold& hold = cResp.AddBreathHold();
  hold.GetPeriod().SetValue(10, TimeUnit::s);

  bg->ProcessAction(cResp);
  tracker.AdvanceModelTime(5);

  SEScalarTime now;// Make sure to tell the engine that we are at the same time
  // Save and Load the Engine State
  bg->GetLogger()->Info("Serializing");
  bg->SaveState("./MidInhalerState.pba");
  now.SetValue(bg->GetSimulationTime(TimeUnit::s), TimeUnit::s);
  bg->LoadState("./MidInhalerState.pba", &now);

  // Change the results file
  bg->GetLogger()->ResetLogFile("InhalerSerialization.log");
  std::remove("InhalerSerializationResults.txt");
  bg->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("InhalerSerializationResults.txt");

  tracker.AdvanceModelTime(145);
}

void PulseEngineTest::InjectSuccsState(PhysiologyEngine* bg, HowToTracker& tracker, const SESubstance& succs)
{
  bg->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("InjectSuccsResults.txt");
  if (!bg->InitializeEngine("StandardMale.pba"))
  {
    std::cerr << "Could not load initialize engine, check the error" << std::endl;
    return;
  }

  SESubstanceBolus injection(succs);
  injection.SetAdminRoute(cdm::SubstanceBolusData_eAdministrationRoute_Intravenous);
  injection.GetConcentration().SetValue(4820, MassPerVolumeUnit::ug_Per_mL);
  injection.GetDose().SetValue(30, VolumeUnit::mL);
  bg->ProcessAction(injection);
  bg->AdvanceModelTime(1, TimeUnit::s);// Not tracking

  SEScalarTime now;// Make sure to tell the engine that we are at the same time

  // Change our results file name
  bg->GetLogger()->ResetLogFile("InjectSuccsSerialization.log");
  std::remove("InjectSuccsSerialization.txt");
  bg->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("InjectSuccsSerialization.txt");

  // Save and Load the Engine State
  bg->SaveState("./MidBolusState.pba");
  now.SetValue(bg->GetSimulationTime(TimeUnit::s), TimeUnit::s);
  bg->LoadState("./MidBolusState.pba",&now);

  tracker.AdvanceModelTime(15);

  SEAnesthesiaMachineConfiguration amConfig(bg->GetSubstanceManager());
  amConfig.GetConfiguration().SetConnection(cdm::AnesthesiaMachineData_eConnection_Mask);
  amConfig.GetConfiguration().GetInletFlow().SetValue(5, VolumePerTimeUnit::L_Per_min);
  amConfig.GetConfiguration().GetInspiratoryExpiratoryRatio().SetValue(0.5);
  amConfig.GetConfiguration().GetOxygenFraction().SetValue(0.4);
  amConfig.GetConfiguration().SetOxygenSource(cdm::AnesthesiaMachineData_eOxygenSource_Wall);
  amConfig.GetConfiguration().GetPositiveEndExpiredPressure().SetValue(3.0, PressureUnit::cmH2O);
  amConfig.GetConfiguration().SetPrimaryGas(cdm::AnesthesiaMachineData_ePrimaryGas_Nitrogen);
  amConfig.GetConfiguration().GetReliefValvePressure().SetValue(20, PressureUnit::cmH2O);
  amConfig.GetConfiguration().GetRespiratoryRate().SetValue(16.0, FrequencyUnit::Per_min);
  amConfig.GetConfiguration().GetVentilatorPressure().SetValue(10, PressureUnit::cmH2O);
  amConfig.GetConfiguration().GetOxygenBottleOne().GetVolume().SetValue(660, VolumeUnit::L);
  amConfig.GetConfiguration().GetOxygenBottleTwo().GetVolume().SetValue(660, VolumeUnit::L);
  bg->ProcessAction(amConfig);
  tracker.AdvanceModelTime(5);

  bg->SaveState("./AnesthesiaMachineState.pba");
  now.SetValue(bg->GetSimulationTime(TimeUnit::s), TimeUnit::s);
  bg->LoadState("./AnesthesiaMachineState.pba", &now);

  tracker.AdvanceModelTime(40);
}

void PulseEngineTest::SerializationTest(const std::string& sTestDirectory)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreatePulseEngine("SerializationTestSetup.log");
  HowToTracker tracker(*bg);

  SESubstance* O2 = bg->GetSubstanceManager().GetSubstance("Oxygen");
  SESubstance* CO2 = bg->GetSubstanceManager().GetSubstance("CarbonDioxide");
/*
  SESubstanceDataRequest* subRequest;
  

  SECompartmentDataRequest* cmptRequest;
  cmptRequest = new SECompartmentDataRequest();
  cmptRequest->Set("Aorta", CDM::enumCompartmentType::Liquid, *O2, "PartialPressure", PressureUnit::mmHg);
  tracker.m_Requests.push_back(cmptRequest);
  cmptRequest = new SECompartmentDataRequest();
  cmptRequest->Set("Carina", CDM::enumCompartmentType::Gas, *O2, "PartialPressure", PressureUnit::cmH2O);
  tracker.m_Requests.push_back(cmptRequest);
  cmptRequest = new SECompartmentDataRequest();
  cmptRequest->Set("Aorta", CDM::enumCompartmentType::Liquid, *CO2, "PartialPressure", PressureUnit::mmHg);
  tracker.m_Requests.push_back(cmptRequest);
  cmptRequest = new SECompartmentDataRequest();
  cmptRequest->Set("Carina", CDM::enumCompartmentType::Gas, *CO2, "PartialPressure", PressureUnit::cmH2O);
  tracker.m_Requests.push_back(cmptRequest);
  

  SEPhysiologySystemDataRequest* physRequest;
  physRequest = new SEPhysiologySystemDataRequest();
  physRequest->Set("ArterialPressure", PressureUnit::mmHg);
  tracker.m_Requests.push_back(physRequest);
  physRequest = new SEPhysiologySystemDataRequest();
  physRequest->Set("BloodVolume", VolumeUnit::mL);
  tracker.m_Requests.push_back(physRequest);
  physRequest = new SEPhysiologySystemDataRequest();
  physRequest->Set("CarbonDioxideProductionRate", VolumePerTimeUnit::mL_Per_min);
  tracker.m_Requests.push_back(physRequest);
  physRequest = new SEPhysiologySystemDataRequest();
  physRequest->Set("CarbonDioxideSaturation");
  tracker.m_Requests.push_back(physRequest);
  physRequest = new SEPhysiologySystemDataRequest();
  physRequest->Set("CardiacOutput", VolumePerTimeUnit::mL_Per_min);
  tracker.m_Requests.push_back(physRequest);
  physRequest = new SEPhysiologySystemDataRequest();
  physRequest->Set("CentralVenousPressure", PressureUnit::mmHg);
  tracker.m_Requests.push_back(physRequest);
  physRequest = new SEPhysiologySystemDataRequest();
  physRequest->Set("DiastolicArterialPressure", PressureUnit::mmHg);
  tracker.m_Requests.push_back(physRequest);
  physRequest = new SEPhysiologySystemDataRequest();
  physRequest->Set("GlomerularFiltrationRate", VolumePerTimeUnit::mL_Per_day);
  tracker.m_Requests.push_back(physRequest);
  physRequest = new SEPhysiologySystemDataRequest();
  physRequest->Set("HeartRate", FrequencyUnit::Per_min);
  tracker.m_Requests.push_back(physRequest);
  physRequest = new SEPhysiologySystemDataRequest();
  physRequest->Set("HeartStrokeVolume", VolumeUnit::mL);
  tracker.m_Requests.push_back(physRequest);
  physRequest = new SEPhysiologySystemDataRequest();
  physRequest->Set("LeftAfferentArterioleResistance", FlowResistanceUnit::mmHg_min_Per_mL);
  tracker.m_Requests.push_back(physRequest);  
  physRequest = new SEPhysiologySystemDataRequest();
  physRequest->Set("MeanArterialPressure", PressureUnit::mmHg);
  tracker.m_Requests.push_back(physRequest);
  physRequest = new SEPhysiologySystemDataRequest();
  physRequest->Set("NeuromuscularBlockLevel");
  tracker.m_Requests.push_back(physRequest);
  physRequest = new SEPhysiologySystemDataRequest();
  physRequest->Set("OxygenConsumptionRate", VolumePerTimeUnit::mL_Per_min);
  tracker.m_Requests.push_back(physRequest);
  physRequest = new SEPhysiologySystemDataRequest();
  physRequest->Set("OxygenSaturation");
  tracker.m_Requests.push_back(physRequest);
  physRequest = new SEPhysiologySystemDataRequest();
  physRequest->Set("RenalBloodFlow",VolumePerTimeUnit::mL_Per_min);
  tracker.m_Requests.push_back(physRequest);
  physRequest = new SEPhysiologySystemDataRequest();
  physRequest->Set("RespirationRate", FrequencyUnit::Per_min);
  tracker.m_Requests.push_back(physRequest);  
  physRequest = new SEPhysiologySystemDataRequest();
  physRequest->Set("SystolicArterialPressure", PressureUnit::mmHg);
  tracker.m_Requests.push_back(physRequest);
  physRequest = new SEPhysiologySystemDataRequest();
  physRequest->Set("TotalAlveolarVentilation", VolumePerTimeUnit::L_Per_min);
  tracker.m_Requests.push_back(physRequest);
  physRequest = new SEPhysiologySystemDataRequest();
  physRequest->Set("TotalLungVolume", VolumeUnit::mL);
  tracker.m_Requests.push_back(physRequest);
  physRequest = new SEPhysiologySystemDataRequest();
  physRequest->Set("TranspulmonaryPressure", PressureUnit::cmH2O);
  tracker.m_Requests.push_back(physRequest);
  SEEquipmentSystemDataRequest* equipRequest = new SEEquipmentSystemDataRequest();
  equipRequest->Set("Lead3ElectricPotential", ElectricPotentialUnit::mV);
  tracker.m_Requests.push_back(equipRequest);
*/


  {// Basic Standard    

    // Gen Basic Standard Baseline
   /* {
      bg->GetLogger()->ResetLogFile("BasicStandardResults.log");
      bg->GetEngineTrack()->RequestData(tracker.m_Requests, "BasicStandardResults.txt");
      if (!bg->InitializeEngine("StandardMale.pba"))
      {
        std::cerr << "Could not load initialize engine, check the error" << std::endl;
        return;
      }
      tracker.AdvanceModelTime(120);
    }*/

    // Gen Basic Standard State
    /*{
      bg->GetLogger()->ResetLogFile("BasicStandardStateSetupResults.log");
      bg->GetEngineTrack()->RequestData(tracker.m_Requests, "BasicStandardStateSetupResults.txt");
      if (!bg->InitializeEngine("StandardMale.pba"))
      {
        std::cerr << "Could not load initialize engine, check the error" << std::endl;
        return;
      }
      tracker.AdvanceModelTime(60);
      bg->SaveState("./BasicStandardState@60s.pba");
    }*/

    // Run Basic Standard State
    {
      bg->GetLogger()->ResetLogFile("BasicStandardStateResults.log");
      bg->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("BasicStandardStateResults.txt");
      bg->LoadState("./BasicStandardState@60s.pba");
      tracker.AdvanceModelTime(60);
    }
  }
// amb, we want these methods back!
  // Several Options to choose how to set up our engine before we save and load
  if(false)
  {
    SESubstance* Albert = bg->GetSubstanceManager().GetSubstance("Albuterol");
//    bg->GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest(*Albert, "MassInBody", MassUnit::ug);
//    bg->GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest(*Albert, "PlasmaConcentration", MassPerVolumeUnit::ug_Per_mL);
//    bg->GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest(*Albert, "RemainingSystemicMassCleared", MassUnit::ug);
    InhalerState(bg.get(), tracker);
  }
  else if (false)
  {
    SESubstance* Succs = bg->GetSubstanceManager().GetSubstance("Succinylcholine");
//    bg->GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest(*Succs, "MassInBody", MassUnit::ug);
//    bg->GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest(*Succs, "PlasmaConcentration", MassPerVolumeUnit::ug_Per_mL);
//    bg->GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest(*Succs, "RemainingSystemicMassCleared", MassUnit::ug);
//    bg->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest("BrainVasculature", *Succs, "Concentration", MassPerVolumeUnit::ug_Per_mL);
//    bg->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest("BrainTissueExtracellular", *Succs, "Concentration", MassPerVolumeUnit::ug_Per_mL);
    InjectSuccsState(bg.get(), tracker, *Succs);
  }
  bg->SaveState("./FinalEngineState.pba");
}
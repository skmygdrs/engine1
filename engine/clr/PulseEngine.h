/* Distributed under the Apache License, Version 2.0.
See accompanying NOTICE file for details.*/

#pragma once 
#include <string>

class PhysiologyEngine;

public ref class PulseEngineRef
{
public:

  PulseEngineRef();
  PulseEngineRef(System::String^ logfile);
  //PulseEngineRef(Logger*);
  virtual ~PulseEngineRef();

  bool LoadStateFile(System::String^ filename);

  void AdvanceModelTime(double time_s);

  double GetLungVolume();

  double GetMusclePressure();

  double GetRespiratoryRate();

  double GetPulmonaryResistance();

  double GetPulmonaryCompliance();

  double GetInspTime();

  double GetExpTime();

internal: // Should be hidden from C#, which is what we want for these.
  PhysiologyEngine* _pulse;
  std::string MarshalString(System::String^ s);
};
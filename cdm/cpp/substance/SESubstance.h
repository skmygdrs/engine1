/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
PROTO_PUSH 
#include "bind/cdm/Substance.pb.h"
PROTO_POP

#include "substance/SESubstanceAerosolization.h"
#include "substance/SESubstanceClearance.h"
#include "substance/SESubstancePharmacokinetics.h"
#include "substance/SESubstancePharmacodynamics.h"

class CDM_DECL SESubstance : public Loggable
{
public:

  SESubstance(Logger* logger);
  virtual ~SESubstance();

  virtual void Clear();

  virtual const SEScalar* GetScalar(const std::string& name);

  static void Load(const cdm::SubstanceData& src, SESubstance& dst);
  static cdm::SubstanceData* Unload(const SESubstance& src);
protected:
  static void Serialize(const cdm::SubstanceData& src, SESubstance& dst);
  static void Serialize(const SESubstance& src, cdm::SubstanceData& dst);

public:
  
  virtual std::string GetName() const;
  virtual void SetName(const std::string& name);
  virtual bool HasName() const;
  virtual void InvalidateName();

  virtual cdm::SubstanceData_eState GetState() const;
  virtual void SetState(cdm::SubstanceData_eState state);
  virtual bool HasState() const;
  virtual void InvalidateState();

  virtual bool HasDensity() const;
  virtual SEScalarMassPerVolume& GetDensity();
  virtual double GetDensity(const MassPerVolumeUnit& unit) const;

  virtual bool HasMolarMass() const;
  virtual SEScalarMassPerAmount& GetMolarMass();
  virtual double GetMolarMass(const MassPerAmountUnit& unit) const;

  // Diffusion-ish
  virtual bool HasMaximumDiffusionFlux() const;
  virtual SEScalarMassPerAreaTime& GetMaximumDiffusionFlux();
  virtual double GetMaximumDiffusionFlux(const MassPerAreaTimeUnit& unit) const;

  virtual bool HasMichaelisCoefficient() const;
  virtual SEScalar& GetMichaelisCoefficient();
  virtual double GetMichaelisCoefficient() const;

  // Liquid-ish
  virtual bool HasAerosolization() const;
  virtual SESubstanceAerosolization& GetAerosolization();
  virtual const SESubstanceAerosolization* GetAerosolization() const;
  virtual void RemoveAerosolization();

  virtual bool HasBloodConcentration() const;
  virtual SEScalarMassPerVolume& GetBloodConcentration();
  virtual double GetBloodConcentration(const MassPerVolumeUnit& unit) const;

  virtual bool HasMassInBody() const;
  virtual SEScalarMass& GetMassInBody();
  virtual double GetMassInBody(const MassUnit& unit) const;

  virtual bool HasMassInBlood() const;
  virtual SEScalarMass& GetMassInBlood();
  virtual double GetMassInBlood(const MassUnit& unit) const;

  virtual bool HasMassInTissue() const;
  virtual SEScalarMass& GetMassInTissue();
  virtual double GetMassInTissue(const MassUnit& unit) const;

  virtual bool HasPlasmaConcentration() const;
  virtual SEScalarMassPerVolume& GetPlasmaConcentration();
  virtual double GetPlasmaConcentration(const MassPerVolumeUnit& unit) const;

  virtual bool HasSystemicMassCleared() const;
  virtual SEScalarMass& GetSystemicMassCleared();
  virtual double GetSystemicMassCleared(const MassUnit& unit) const;

  virtual bool HasTissueConcentration() const;
  virtual SEScalarMassPerVolume& GetTissueConcentration();
  virtual double GetTissueConcentration(const MassPerVolumeUnit& unit) const;

  // Gas-ish
  virtual bool HasAlveolarTransfer() const;
  virtual SEScalarVolumePerTime& GetAlveolarTransfer();
  virtual double GetAlveolarTransfer(const VolumePerTimeUnit& unit) const;

  virtual bool HasDiffusingCapacity() const;
  virtual SEScalarVolumePerTimePressure& GetDiffusingCapacity();
  virtual double GetDiffusingCapacity(const VolumePerTimePressureUnit& unit) const;

  virtual bool HasEndTidalFraction() const;
  virtual SEScalar0To1& GetEndTidalFraction();
  virtual double GetEndTidalFraction() const;

  virtual bool HasEndTidalPressure() const;
  virtual SEScalarPressure& GetEndTidalPressure();
  virtual double GetEndTidalPressure(const PressureUnit& unit) const;

  virtual bool HasRelativeDiffusionCoefficient() const;
  virtual SEScalar& GetRelativeDiffusionCoefficient();
  virtual double GetRelativeDiffusionCoefficient() const;

  virtual bool HasSolubilityCoefficient() const;
  virtual SEScalarInversePressure& GetSolubilityCoefficient();
  virtual double GetSolubilityCoefficient(const InversePressureUnit& unit) const;

  virtual bool HasClearance() const;
  virtual SESubstanceClearance& GetClearance();
  virtual const SESubstanceClearance* GetClearance() const;
  virtual void RemoveClearance();

  virtual bool HasPK() const;
  virtual SESubstancePharmacokinetics& GetPK();
  virtual const SESubstancePharmacokinetics* GetPK() const;
  virtual void RemovePK();

  virtual bool HasPD() const;
  virtual SESubstancePharmacodynamics& GetPD();
  virtual const SESubstancePharmacodynamics* GetPD() const;
  virtual void RemovePD();

  
protected: 

  std::string                       m_Name;
  cdm::SubstanceData_eState         m_State;
  SEScalarMassPerVolume*            m_Density;
  SEScalarMassPerAmount*            m_MolarMass;

  SEScalarMassPerAreaTime*          m_MaximumDiffusionFlux;
  SEScalar*                         m_MichaelisCoefficient;

  SESubstanceAerosolization*        m_Aerosolization;
  SEScalarMassPerVolume*            m_BloodConcentration;
  SEScalarMass*                     m_MassInBody;
  SEScalarMass*                     m_MassInBlood;
  SEScalarMass*                     m_MassInTissue;
  SEScalarMassPerVolume*            m_PlasmaConcentration;
  SEScalarMass*                     m_SystemicMassCleared;
  SEScalarMassPerVolume*            m_TissueConcentration;

  SEScalarVolumePerTime*            m_AlveolarTransfer;
  SEScalarVolumePerTimePressure*    m_DiffusingCapacity;
  SEScalar0To1*                     m_EndTidalFraction;
  SEScalarPressure*                 m_EndTidalPressure;
  SEScalar*                         m_RelativeDiffusionCoefficient;
  SEScalarInversePressure*          m_SolubilityCoefficient;

  SESubstanceClearance*             m_Clearance;
  SESubstancePharmacokinetics*      m_PK;
  SESubstancePharmacodynamics*      m_PD;
};
/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "compartment/substances/SESubstanceQuantity.h"
#include "substance/SESubstanceTransport.h"
CDM_BIND_DECL(LiquidSubstanceQuantityData)
class SELiquidCompartment;

enum class BalanceLiquidBy { Mass, Concentration, Molarity, PartialPressure };

class CDM_DECL SELiquidSubstanceQuantity : public SESubstanceQuantity, public SELiquidTransportSubstance
{
  friend class SELiquidCompartment;
protected:
  SELiquidSubstanceQuantity(SESubstance& sub, SELiquidCompartment& compartment);
public:
  virtual ~SELiquidSubstanceQuantity();

  virtual void Clear(); 
  virtual void Invalidate();

  static void Load(const cdm::LiquidSubstanceQuantityData& src, SELiquidSubstanceQuantity& dst);
  static cdm::LiquidSubstanceQuantityData* Unload(const SELiquidSubstanceQuantity& src);
protected:
  static void Serialize(const cdm::LiquidSubstanceQuantityData& src, SELiquidSubstanceQuantity& dst);
  static void Serialize(const SELiquidSubstanceQuantity& src, cdm::LiquidSubstanceQuantityData& dst);

public:
  virtual void SetToZero();
  virtual const SEScalar* GetScalar(const std::string& name);

  virtual void Balance(BalanceLiquidBy e);// Balance Substance based on a specific property

  virtual bool HasConcentration() const;
  virtual SEScalarMassPerVolume& GetConcentration();
  virtual double GetConcentration(const MassPerVolumeUnit& unit) const;

  virtual bool HasMass() const;
  virtual SEScalarMass& GetMass();
  virtual double GetMass(const MassUnit& unit) const;

  virtual bool HasMassCleared() const;
  virtual SEScalarMass& GetMassCleared();
  virtual double GetMassCleared(const MassUnit& unit) const;

  virtual bool HasMassDeposited() const;
  virtual SEScalarMass& GetMassDeposited();
  virtual double GetMassDeposited(const MassUnit& unit) const;

  virtual bool HasMassExcreted() const;
  virtual SEScalarMass& GetMassExcreted();
  virtual double GetMassExcreted(const MassUnit& unit) const;
  
  virtual bool HasMolarity() const;
  virtual SEScalarAmountPerVolume& GetMolarity();
  virtual double GetMolarity(const AmountPerVolumeUnit& unit) const;
  
  virtual bool HasPartialPressure() const;
  virtual SEScalarPressure& GetPartialPressure();
  virtual double GetPartialPressure(const PressureUnit& unit) const;

  virtual bool HasSaturation() const;
  virtual SEScalar0To1& GetSaturation();
  virtual double GetSaturation() const;
  virtual void SetHemoglobins(SESubstance& Hb, SESubstance& HbO2, SESubstance& HbCO2, SESubstance& HbO2CO2, SESubstance& HbCO);

protected:
  virtual void AddChild(SELiquidSubstanceQuantity& subQ);

  virtual bool HasExtensive() const { return HasMass(); }
  virtual SEScalarMass& GetExtensive() { return GetMass(); }

  virtual bool HasIntensive() const { return HasConcentration(); }
  virtual SEScalarMassPerVolume& GetIntensive() { return GetConcentration(); }

  SEScalarMassPerVolume*   m_Concentration;
  SEScalarMass*            m_Mass;
  SEScalarMass*            m_MassCleared;
  SEScalarMass*            m_MassDeposited;
  SEScalarMass*            m_MassExcreted;
  SEScalarAmountPerVolume* m_Molarity;
  SEScalarPressure*        m_PartialPressure;
  SEScalar0To1*            m_Saturation;

  SELiquidCompartment&     m_Compartment;
  std::vector<SELiquidSubstanceQuantity*> m_Children;

  bool m_isO2;
  bool m_isCO;
  bool m_isCO2;
  // Inorder to caluclate a hierarchical saturation, we need these substances
  SESubstance* m_Hb;
  SESubstance* m_HbO2;
  SESubstance* m_HbCO2;
  SESubstance* m_HbO2CO2;
  SESubstance* m_HbCO;
};
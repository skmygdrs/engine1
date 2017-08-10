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
#include "bind/cdm/Physiology.pb.h"

class CDM_DECL SEPupillaryResponse
{
public:

  SEPupillaryResponse(Logger* logger);
  virtual ~SEPupillaryResponse();

  virtual void Clear();// Deletes all members
  
  virtual const SEScalar* GetScalar(const std::string& name);

  static void Load(const cdm::PupillaryResponseData& src, SEPupillaryResponse& dst);
  static cdm::PupillaryResponseData* Unload(const SEPupillaryResponse& src);
protected:
  static void Serialize(const cdm::PupillaryResponseData& src, SEPupillaryResponse& dst);
  static void Serialize(const SEPupillaryResponse& src, cdm::PupillaryResponseData& dst);
  
public:

  virtual bool HasReactivityModifier() const;
  virtual SEScalarNegative1To1& GetReactivityModifier();
  virtual double GetReactivityModifier() const;

  virtual bool HasShapeModifier() const;
  virtual SEScalarNegative1To1& GetShapeModifier();
  virtual double GetShapeModifier() const;

  virtual bool HasSizeModifier() const;
  virtual SEScalarNegative1To1& GetSizeModifier();
  virtual double GetSizeModifier() const;

protected:

  SEScalarNegative1To1* m_ReactivityModifier;
  SEScalarNegative1To1* m_ShapeModifier;
  SEScalarNegative1To1* m_SizeModifier;

};

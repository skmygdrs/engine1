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
#include "patient/actions/SEChestCompression.h"

class DLL_DECL SEChestCompressionForce : public SEChestCompression
{
public:

  SEChestCompressionForce();
  virtual ~SEChestCompressionForce();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;
  virtual bool IsActive() const;


  static void Load(const cdm::ChestCompressionForceData& src, SEChestCompressionForce& dst);
  static cdm::ChestCompressionForceData* Unload(const SEChestCompressionForce& src);
protected:
	static void Serialize(const cdm::ChestCompressionForceData& src, SEChestCompressionForce& dst);
	static void Serialize(const SEChestCompressionForce& src, cdm::ChestCompressionForceData& dst);

public:

  virtual bool HasForce() const;
  virtual SEScalarForce& GetForce();

  virtual void ToString(std::ostream &str) const;

protected:
  SEScalarForce*           m_Force;
  //jbw - Why is there no ForcePeriod (like in ForceScale)?
};
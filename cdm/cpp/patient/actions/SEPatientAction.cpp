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
#include "patient/actions/SEPatientAction.h"

SEPatientAction::SEPatientAction() : SEAction()
{
  
}

SEPatientAction::~SEPatientAction()
{
  Clear();
}

void SEPatientAction::Clear()
{
  SEAction::Clear();
}

bool SEPatientAction::IsValid() const
{
  return SEAction::IsValid();
}

void SEPatientAction::Load(const cdm::PatientActionData& src, SEPatientAction& dst)
{
	SEPatientAction::Serialize(src, dst);
}
void SEPatientAction::Serialize(const cdm::PatientActionData& src, SEPatientAction& dst)
{
	dst.Clear();
}

cdm::PatientActionData* SEPatientAction::Unload(const SEPatientAction& src)
{
	cdm::PatientActionData* dst = new cdm::PatientActionData();
	SEPatientAction::Serialize(src, *dst);
	return dst;
}
void SEPatientAction::Serialize(const SEPatientAction& src, cdm::PatientActionData& dst)
{
	
}

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
#include "system/equipment/Anesthesia/actions/SEInspiratoryValveLeak.h"
#include "properties/SEScalar0To1.h"

SEInspiratoryValveLeak::SEInspiratoryValveLeak() : SEAnesthesiaMachineAction()
{
  m_Severity = nullptr;
}

SEInspiratoryValveLeak::~SEInspiratoryValveLeak()
{
  Clear();
}

void SEInspiratoryValveLeak::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  SAFE_DELETE(m_Severity);
}

bool SEInspiratoryValveLeak::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SEInspiratoryValveLeak::IsActive() const
{
  return HasSeverity() ? !m_Severity->IsZero() : false;
}

void SEInspiratoryValveLeak::Load(const cdm::InspiratoryValveLeakData& src, SEInspiratoryValveLeak& dst)
{
  SEInspiratoryValveLeak::Serialize(src, dst);
}
void SEInspiratoryValveLeak::Serialize(const cdm::InspiratoryValveLeakData& src, SEInspiratoryValveLeak& dst)
{
  SEAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    SEScalar0To1::Load(src.severity(), dst.GetSeverity());
}

cdm::InspiratoryValveLeakData* SEInspiratoryValveLeak::Unload(const SEInspiratoryValveLeak& src)
{
  cdm::InspiratoryValveLeakData* dst = new cdm::InspiratoryValveLeakData();
  SEInspiratoryValveLeak::Serialize(src, *dst);
  return dst;
}
void SEInspiratoryValveLeak::Serialize(const SEInspiratoryValveLeak& src, cdm::InspiratoryValveLeakData& dst)
{
  SEAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(SEScalar0To1::Unload(*src.m_Severity));
}

bool SEInspiratoryValveLeak::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SEInspiratoryValveLeak::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}

void SEInspiratoryValveLeak::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Inspiratory Valve Leak";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity :"; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}
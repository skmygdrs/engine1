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
#include "compartment/SECompartmentLink.h"
#include "circuit/SECircuitManager.h"


SECompartmentLink::SECompartmentLink(const std::string& name, Logger* logger) : Loggable(logger), m_Name(name)
{
  
}

SECompartmentLink::~SECompartmentLink()
{
  Clear();
}

void SECompartmentLink::Clear()
{
 
}

void SECompartmentLink::Serialize(const cdm::CompartmentLinkData& src, SECompartmentLink& dst)
{
  dst.Clear();
  if (!src.name().empty())
    dst.m_Name = src.name();
}

void SECompartmentLink::Serialize(const SECompartmentLink& src, cdm::CompartmentLinkData& dst)
{
  dst.set_name(src.m_Name);
}

std::string SECompartmentLink::GetName() const
{
  return m_Name;
}




// Copyright (C) 2023 CERN for the benefit of the ATLAS collaboration

// Local include(s).
#include "RDFTrigDecision/AddTriggerDecisionColumn.h"

AddTriggerDecisionColumn::AddTriggerDecisionColumn(
    std::string_view chainName, TrigDecisionToolArray& tdtArray)
    : m_chain(chainName), m_array(tdtArray) {}

bool AddTriggerDecisionColumn::operator()(unsigned int slot) {

   // Return the trigger decision.
   return m_array.getTool(slot).isPassed(m_chain);
}

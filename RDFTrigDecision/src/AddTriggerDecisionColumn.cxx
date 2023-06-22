// Copyright (C) 2023 CERN for the benefit of the ATLAS collaboration

// Local include(s).
#include "RDFTrigDecision/AddTriggerDecisionColumn.h"

// Trigger include(s).
#include "TrigConfxAOD/xAODConfigTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"

// System include(s).
#include <stdexcept>

namespace details {

/// Internal data object needed for technical reasons
class xAODConfigTool : public TrigConf::xAODConfigTool {

public:
   // Inherit all constructors from the base class
   using TrigConf::xAODConfigTool::xAODConfigTool;

   // Make the beginEvent() function publicly accessible.
   using TrigConf::xAODConfigTool::beginEvent;

}; // class xAODConfigTool

struct AddTriggerDecisionColumnData {

   /// Constructor with a unique postfix for the tools.
   ///
   /// @param postfix Postfix to use for the tool names.
   ///
   AddTriggerDecisionColumnData(std::string_view postfix)
       : m_postfix(postfix) {}

   /// Initialize the tools.
   void initialize() {

      // Create the configuration tool.
      m_configTool =
          std::make_unique<xAODConfigTool>("xAODConfigTool" + m_postfix);
      // Initialize it.
      if (!m_configTool->initialize().isSuccess()) {
         throw std::runtime_error(
             "Failed to initialize the trigger configuration tool");
      }
      // Since by now an event is loaded, tell the tool to do whatever it needs
      // to do to handle that event.
      if (!m_configTool->beginEvent().isSuccess()) {
         throw std::runtime_error(
             "Failed to initialize the trigger configuration tool");
      }
      // Create the decision tool.
      m_decisionTool = std::make_unique<Trig::TrigDecisionTool>(
          "TrigDecisionTool" + m_postfix);
      // Set its properties before initializing it.
      if (!m_decisionTool
               ->setProperty("ConfigTool", ("TrigConf::xAODConfigTool/" +
                                            m_configTool->name()))
               .isSuccess()) {
         throw std::runtime_error(
             "Failed to configure the trigger decision tool");
      }
      if (!m_decisionTool->setProperty("TrigDecisionKey", "xTrigDecision")
               .isSuccess()) {
         throw std::runtime_error(
             "Failed to configure the trigger decision tool");
      }
      // Initialize it.
      if (!m_decisionTool->initialize().isSuccess()) {
         throw std::runtime_error(
             "Failed to initialize the trigger decision tool");
      }
      // Since by now an event is loaded, tell the tool to do whatever it needs
      // to do to handle that event.
      if (!m_decisionTool->beginEvent().isSuccess()) {
         throw std::runtime_error(
             "Failed to initialize the trigger decision tool");
      }
   }

   /// The postfix to use for the tool names.
   std::string m_postfix;
   /// The trigger configuration tool.
   std::unique_ptr<xAODConfigTool> m_configTool;
   /// The trigger decision tool.
   std::unique_ptr<Trig::TrigDecisionTool> m_decisionTool;

}; // struct AddTriggerDecisionColumnData

} // namespace details

AddTriggerDecisionColumn::AddTriggerDecisionColumn(std::string_view chainName,
                                                   std::size_t nThreads)
    : m_chainName(chainName) {

   // Create the internal data object(s).
   m_data.reserve(nThreads);
   for (std::size_t i = 0; i < nThreads; ++i) {
      m_data.emplace_back(
          new details::AddTriggerDecisionColumnData("_" + std::to_string(i)));
   }
}

AddTriggerDecisionColumn::AddTriggerDecisionColumn(
    const AddTriggerDecisionColumn& parent)
    : m_chainName(parent.m_chainName) {

   // Copy the internal data object(s).
   m_data.reserve(parent.m_data.size());
   for (std::size_t i = 0; i < parent.m_data.size(); ++i) {
      m_data.emplace_back(new details::AddTriggerDecisionColumnData(
          parent.m_data[i]->m_postfix + "_copy"));
   }
}

AddTriggerDecisionColumn::~AddTriggerDecisionColumn() {}

AddTriggerDecisionColumn&
AddTriggerDecisionColumn::operator=(const AddTriggerDecisionColumn& rhs) {

   // Perform the assignment if necessary
   if (this != &rhs) {
      m_chainName = rhs.m_chainName;
      m_data.clear();
      m_data.reserve(rhs.m_data.size());
      for (std::size_t i = 0; i < rhs.m_data.size(); ++i) {
         m_data.emplace_back(new details::AddTriggerDecisionColumnData(
             rhs.m_data[i]->m_postfix + "_copy"));
      }
   }

   // Return the object.
   return *this;
}

bool AddTriggerDecisionColumn::operator()(unsigned int slot) {

   // A small sanity check.
   if (slot >= m_data.size()) {
      throw std::runtime_error("Invalid slot number requested");
   }

   // Access the internal data object for this slot.
   auto& data = *m_data[slot];

   // Set up / configure the tools if necessary.
   if (!data.m_decisionTool) {
      data.initialize();
   }

   // Add the trigger decision to the data frame.
   return data.m_decisionTool->isPassed(m_chainName);
}

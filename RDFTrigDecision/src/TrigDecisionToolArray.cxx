// Copyright (C) 2023 CERN for the benefit of the ATLAS collaboration

// Local include(s).
#include "RDFTrigDecision/TrigDecisionToolArray.h"

// Trigger include(s).
#include "TrigConfxAOD/xAODConfigTool.h"

// System include(s).
#include <stdexcept>

/// Helper macro for checking the return code of a function call.
///
/// @param EXP The expression to execute.
/// @param MSG The message to print if the expression fails.
///
#define EXCEPT_CHECK(EXP, MSG)                                                 \
   do {                                                                        \
      if (!EXP.isSuccess()) {                                                  \
         throw std::runtime_error(MSG);                                        \
      }                                                                        \
   } while (false)

namespace {

/// Internal data object needed for technical reasons
class xAODConfigTool : public TrigConf::xAODConfigTool {

public:
   // Inherit all constructors from the base class
   using TrigConf::xAODConfigTool::xAODConfigTool;

   // Make the beginEvent() function publicly accessible.
   using TrigConf::xAODConfigTool::beginEvent;

}; // class xAODConfigTool

} // namespace

namespace details {

struct TrigDecisionToolArrayData {

   /// Constructor with a unique postfix for the tools.
   ///
   /// @param postfix Postfix to use for the tool names.
   ///
   TrigDecisionToolArrayData(const std::string_view prefix,
                             const std::string_view postfix)
       : m_prefix(prefix), m_postfix(postfix) {}

   /// Initialize the tools.
   void initialize() {

      // Create the configuration tool.
      m_configTool = std::make_unique<xAODConfigTool>(
          m_prefix + "xAODConfigTool" + m_postfix);
      // Initialize it.
      EXCEPT_CHECK(m_configTool->initialize(),
                   "Failed to initialize the trigger configuration tool");
      // Since by now an event is loaded, tell the tool to do whatever it needs
      // to do to handle that event.
      EXCEPT_CHECK(
          m_configTool->beginEvent(),
          "Failed to call beginEvent() on the trigger configuration tool");

      // Create the decision tool.
      m_decisionTool = std::make_unique<Trig::TrigDecisionTool>(
          m_prefix + "TrigDecisionTool" + m_postfix);
      // Set its properties before initializing it.
      EXCEPT_CHECK(m_decisionTool->setProperty(
                       "ConfigTool",
                       ("TrigConf::xAODConfigTool/" + m_configTool->name())),
                   "Failed to configure the trigger decision tool");
      EXCEPT_CHECK(
          m_decisionTool->setProperty("TrigDecisionKey", "xTrigDecision"),
          "Failed to configure the trigger decision tool");
      // Initialize it.
      EXCEPT_CHECK(m_decisionTool->initialize(),
                   "Failed to initialize the trigger decision tool");
      // Since by now an event is loaded, tell the tool to do whatever it needs
      // to do to handle that event.
      EXCEPT_CHECK(
          m_decisionTool->beginEvent(),
          "Failed to call beginEvent() on the the trigger decision tool");
   }

   /// The prefix to use for the tool names.
   std::string m_prefix;
   /// The postfix to use for the tool names.
   std::string m_postfix;
   /// The trigger configuration tool.
   std::unique_ptr<::xAODConfigTool> m_configTool;
   /// The trigger decision tool.
   std::unique_ptr<Trig::TrigDecisionTool> m_decisionTool;

}; // struct TrigDecisionToolArrayData

} // namespace details

TrigDecisionToolArray::TrigDecisionToolArray(const std::string_view prefix,
                                             const std::string_view postfix,
                                             const std::size_t slots)
    : m_prefix(prefix), m_postfix(postfix), m_data(slots) {}

TrigDecisionToolArray::~TrigDecisionToolArray() {}

Trig::TrigDecisionTool& TrigDecisionToolArray::getTool(const std::size_t slot) {

   // Check if the slot number is valid.
   if (slot >= m_data.size()) {
      throw std::runtime_error("Invalid slot number requested");
   }

   // Access the data object.
   auto& data = m_data[slot];

   // Check if the tool was set up already.
   if (!data) {
      // Create the tool.
      data = std::make_unique<details::TrigDecisionToolArrayData>(
          m_prefix, m_postfix + "_" + std::to_string(slot));
      // Initialize it.
      data->initialize();
   }

   // Return the TrigDecisionTool.
   return *(data->m_decisionTool);
}

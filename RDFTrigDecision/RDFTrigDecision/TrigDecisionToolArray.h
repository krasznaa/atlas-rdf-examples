// Copyright (C) 2023 CERN for the benefit of the ATLAS collaboration
#ifndef RDFTRIGDECISIONTOOL_TRIGDECISIONTOOLARRAY_H
#define RDFTRIGDECISIONTOOL_TRIGDECISIONTOOLARRAY_H

// Trigger include(s).
#include "TrigDecisionTool/TrigDecisionTool.h"

// System include(s).
#include <memory>
#include <string>
#include <string_view>
#include <vector>

/// Internal data object needed for technical reasons
namespace details {
struct TrigDecisionToolArrayData;
}

/// Class holding an array of @c Trig::TrigDecisionTool objects
///
/// It is needed during trigger analysis from @c ROOT::RDataFrame to make sure
/// that the non-thread-safe tool code can be used in a thread-safe way.
///
class TrigDecisionToolArray {

public:
   /// Constructor with a tool name prefix and postfix
   ///
   /// @param prefix The prefix to use for the tool names.
   /// @param postfix The postfix to use for the tool names.
   /// @param slots The (maximum) number of slots to use for the tool array.
   ///
   TrigDecisionToolArray(const std::string_view prefix = "",
                         const std::string_view postfix = "",
                         std::size_t slots = 32);
   /// Destructor
   ~TrigDecisionToolArray();

   /// Get the trig decision tool for a given slot
   Trig::TrigDecisionTool& getTool(std::size_t slot);

private:
   /// The prefix to use for the tool names.
   std::string m_prefix;
   /// The postfix to use for the tool names.
   std::string m_postfix;
   /// The internal data objects, holding the used tool(s).
   std::vector<std::unique_ptr<details::TrigDecisionToolArrayData>> m_data;

}; // class TrigDecisionToolArray

#endif // not RDFTRIGDECISIONTOOL_TRIGDECISIONTOOLARRAY_H

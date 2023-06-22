// Copyright (C) 2023 CERN for the benefit of the ATLAS collaboration
#ifndef RDFTRIGDECISIONTOOL_ADDTRIGGERDECISIONCOLUMN_H
#define RDFTRIGDECISIONTOOL_ADDTRIGGERDECISIONCOLUMN_H

// Local include(s).
#include "RDFTrigDecision/TrigDecisionToolArray.h"

// System include(s).
#include <string>
#include <string_view>

/// Functor adding the trigger decitions from a single chain to a data frame
///
/// This is just a demonstrator, showing how to add a single boolean column to
/// a @c ROOT::RDataFrame object.
///
class AddTriggerDecisionColumn {

public:
   /// Constructor with the name of the trigger chain.
   ///
   /// @param chainName The name of the trigger chain to add to the data frame.
   /// @param tdtArray  The array of trigger decision tools to use.
   ///
   AddTriggerDecisionColumn(std::string_view chainName,
                            TrigDecisionToolArray& tdtArray);

   /// Functor operator adding the trigger decision to the data frame.
   bool operator()(unsigned int slot);

private:
   /// The name of the trigger chain to add to the data frame.
   std::string m_chain;
   /// The array of trigger decision tools to use.
   TrigDecisionToolArray& m_array;

}; // class AddTriggerDecisionColumn

#endif // not RDFTRIGDECISIONTOOL_ADDTRIGGERDECISIONCOLUMN_H

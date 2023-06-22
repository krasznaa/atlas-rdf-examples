// Copyright (C) 2023 CERN for the benefit of the ATLAS collaboration
#ifndef RDFTRIGDECISIONTOOL_ADDTRIGGERDECISIONCOLUMN_H
#define RDFTRIGDECISIONTOOL_ADDTRIGGERDECISIONCOLUMN_H

// System include(s).
#include <memory>
#include <string>
#include <string_view>
#include <vector>

/// Internal data object needed for technical reasons
namespace details {
struct AddTriggerDecisionColumnData;
}

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
   /// @param nThreads  The (maximum) number of threads to use for the
   /// operation.
   ///
   AddTriggerDecisionColumn(std::string_view chainName,
                            std::size_t nThreads = 32);
   /// Copy constructor
   AddTriggerDecisionColumn(const AddTriggerDecisionColumn& parent);
   /// Destructor
   ~AddTriggerDecisionColumn();

   /// Assignment operator
   AddTriggerDecisionColumn& operator=(const AddTriggerDecisionColumn& rhs);

   /// Functor operator adding the trigger decision to the data frame.
   bool operator()(unsigned int slot);

private:
   /// The name of the trigger chain to add to the data frame.
   std::string m_chainName;
   /// The internal data objects, holding the used tool(s).
   std::vector<std::unique_ptr<details::AddTriggerDecisionColumnData>> m_data;

}; // class AddTriggerDecisionColumn

#endif // not RDFTRIGDECISIONTOOL_ADDTRIGGERDECISIONCOLUMN_H

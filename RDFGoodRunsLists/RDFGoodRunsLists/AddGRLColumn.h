// Copyright (C) 2023 CERN for the benefit of the ATLAS collaboration
#ifndef RDFGOODRUNSLISTS_ADDGRLCOLUMN_H
#define RDFGOODRUNSLISTS_ADDGRLCOLUMN_H

// EDM include(s).
#include "xAODEventInfo/EventInfo.h"

// System include(s).
#include <memory>
#include <string_view>

/// Forward declaration of the internal data type.
struct AddGRLColumnData;

class AddGRLColumn {

public:
   /// Constructor with a single GRL XML name
   AddGRLColumn(std::string_view grlFileName);
   /// Move constructor
   AddGRLColumn(AddGRLColumn&& parent);
   /// Copy constructor
   AddGRLColumn(const AddGRLColumn& parent);
   /// Destructor
   ~AddGRLColumn();

   /// Functor calculating the GRL decision for the current event
   bool operator()(const xAOD::EventInfo& ei) const;

private:
   /// Opaque internal data
   std::unique_ptr<AddGRLColumnData> m_data;

}; // class AddGRLColumn

#endif // RDFGOODRUNSLISTS_ADDGRLCOLUMN_H

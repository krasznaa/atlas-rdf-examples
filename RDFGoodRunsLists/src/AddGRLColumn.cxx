// Copyright (C) 2023 CERN for the benefit of the ATLAS collaboration

// Local include(s).
#include "RDFGoodRunsLists/AddGRLColumn.h"

// Athena include(s).
#include "GoodRunsLists/TGRLCollection.h"
#include "GoodRunsLists/TGoodRunsListReader.h"
#include "PathResolver/PathResolver.h"

// System include(s).
#include <stdexcept>
#include <string>

struct AddGRLColumnData {

   /// GRL used by the functor
   Root::TGRLCollection m_grl;

}; // struct AddGRLColumnData

AddGRLColumn::AddGRLColumn(std::string_view grlFileName)
    : m_data{new AddGRLColumnData()} {

   // Find the concrete file name for the XML.
   const std::string fn = PathResolverFindCalibFile(std::string{grlFileName});
   if (fn == "") {
      throw std::runtime_error("Could not find file: " +
                               std::string{grlFileName});
   }

   // Read in the GRL XML.
   Root::TGoodRunsListReader reader;
   reader.AddXMLFile(fn);
   if (!reader.Interpret()) {
      throw std::runtime_error("Could not parse file: " +
                               std::string{grlFileName});
   }

   // Remember the GRL object.
   m_data->m_grl = reader.GetMergedGRLCollection();
}

AddGRLColumn::AddGRLColumn(AddGRLColumn&& parent)
    : m_data(std::move(parent.m_data)) {}

AddGRLColumn::AddGRLColumn(const AddGRLColumn& parent)
    : m_data{new AddGRLColumnData()} {

   m_data->m_grl = parent.m_data->m_grl;
}

AddGRLColumn::~AddGRLColumn() {}

bool AddGRLColumn::operator()(const xAOD::EventInfo& ei) const {

   // Give the decision using the GRL.
   return m_data->m_grl.HasRunLumiBlock(ei.runNumber(), ei.lumiBlock());
}

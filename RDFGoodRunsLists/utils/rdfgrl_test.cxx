// Copyright (C) 2023 CERN for the benefit of the ATLAS collaboration

// Local include(s).
#include "RDFGoodRunsLists/AddGRLColumn.h"

// Analysis include(s).
#include "xAODDataSource/MakeDataFrame.h"
#include "xAODRootAccess/Init.h"

// ROOT include(s).
#include <TCanvas.h>
#include <TSystem.h>

// System include(s).
#include <cstdlib>

int main() {

   // Figure out the number of CPUs/threads to use.
   SysInfo_t sysInfo;
   if (gSystem->GetSysInfo(&sysInfo) != 0) {
      return EXIT_FAILURE;
   }
   const std::size_t nCPUs = static_cast<std::size_t>(sysInfo.fCpus);

   // Set up the runtime environment.
   ROOT::EnableImplicitMT(nCPUs);
   if (xAOD::Init().isFailure()) {
      return EXIT_FAILURE;
   }

   // Create a data frame from the standard test file.
   auto df = xAOD::MakeDataFrame("${ASG_TEST_FILE_DATA}");

   // Add the GRL column to the data frame.
   static const char* GRL_NAME =
      "GoodRunsLists/data18_13TeV/20200426/"
      "data18_13TeV.periodAllYear_DetStatus-v102-pro22-04_PHYS_StandardGRL_All_Good_25ns_BjetHLT.xml";
   auto df_grl =
       df.Define("grl_passed", AddGRLColumn{GRL_NAME}, {"EventInfo"});

   // Create a simple histogram with the GRL decisions.
   auto h = df_grl.Histo1D("grl_passed");

   // Save the histogram to a file.
   TCanvas canvas("canvas", "My Canvas", 800, 600);
   h->Draw();
   canvas.SaveAs("grl_passed.png");

   // Return gracefully.
   return EXIT_SUCCESS;
}

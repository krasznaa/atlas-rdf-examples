// Copyright (C) 2023 CERN for the benefit of the ATLAS collaboration

// Local include(s).
#include "RDFTrigDecision/AddTriggerDecisionColumn.h"
#include "RDFTrigDecision/TrigDecisionToolArray.h"

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
   auto df = xAOD::MakeDataFrame("${ASG_TEST_FILE_LITE_MC}");

   // Create the object holding the trigger tools.
   TrigDecisionToolArray tdtArray{"Cpp", "", nCPUs};

   // Name of the trigger to test.
   static const std::string trigName = "HLT_mu26_ivarmedium";

   // Add the trigger decision column to the data frame.
   auto df_trig =
       df.DefineSlot(trigName, AddTriggerDecisionColumn(trigName, tdtArray));

   // Create a simple histogram with the trigger decisions.
   auto h = df_trig.Histo1D(trigName);

   // Save the histogram to a file.
   TCanvas canvas("canvas", "My Canvas", 800, 600);
   h->Draw();
   canvas.SaveAs((trigName + ".png").c_str());

   // Return gracefully.
   return EXIT_SUCCESS;
}

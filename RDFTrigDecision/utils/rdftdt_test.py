#!/usr/bin/env python
#
# Copyright (C) 2023 CERN for the benefit of the ATLAS collaboration
#

# Necessary import(s).
import os
import sys
import ROOT
from xAODDataSource.Helpers import MakexAODDataFrame

def main():
   '''C(++) style main function'''

   # Set up the environment.
   ROOT.EnableImplicitMT(os.cpu_count())
   if ROOT.xAOD.Init().isFailure():
      print('Couldn\'t initialize the xAOD environment')
      return 1

   # Create a data frame from the standard test file.
   df = MakexAODDataFrame('${ASG_TEST_FILE_LITE_MC}')

   # Create the object holding the trigger tools.
   tdtArray = ROOT.TrigDecisionToolArray('Python', '', os.cpu_count())

   # Name of the trigger to test.
   trigName = 'HLT_mu26_ivarmedium'

   # Add the trigger decision column to the data frame.
   df_trig = \
      df.DefineSlot(trigName, ROOT.AddTriggerDecisionColumn(trigName, tdtArray))

   # Create a simple histogram with the trigger decisions.
   hist = df_trig.Histo1D(trigName);

   # Save the histogram to a file.
   canvas = ROOT.TCanvas('canvas', 'My Canvas', 800, 600)
   hist.Draw()
   canvas.SaveAs(trigName + '.png')

   # Return gracefully.
   return 0

# Execute the main function, when in "script mode".
if __name__ == "__main__":
   ROOT.gROOT.SetBatch()
   sys.exit(main())

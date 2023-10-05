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
   df = MakexAODDataFrame('${ASG_TEST_FILE_DATA}')

   # Add the trigger decision column to the data frame.
   GRL_NAME = \
      'GoodRunsLists/data18_13TeV/20200426/' \
      'data18_13TeV.periodAllYear_DetStatus-v102-pro22-04_PHYS_StandardGRL_All_Good_25ns_BjetHLT.xml'
   df_grl = \
      df.Define('grl_passed', ROOT.AddGRLColumn(GRL_NAME), ['EventInfo'])

   # Create a simple histogram with the trigger decisions.
   hist = df_grl.Histo1D('grl_passed')

   # Save the histogram to a file.
   canvas = ROOT.TCanvas('canvas', 'My Canvas', 800, 600)
   hist.Draw()
   canvas.SaveAs('grl_passed.png')

   # Return gracefully.
   return 0

# Execute the main function, when in "script mode".
if __name__ == "__main__":
   ROOT.gROOT.SetBatch()
   sys.exit(main())

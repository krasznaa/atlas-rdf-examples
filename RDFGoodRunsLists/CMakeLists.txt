# Copyright (C) 2023 CERN for the benefit of the ATLAS collaboration

# Set the name of the package.
atlas_subdir(RDFGoodRunsLists)

# External dependencies.
find_package(ROOT COMPONENTS Core Gpad)

# Component(s) in the package.
atlas_add_library(RDFGoodRunsListsLib
  RDFGoodRunsLists/*.h src/*.cxx
  PUBLIC_HEADERS RDFGoodRunsLists
  LINK_LIBRARIES xAODEventInfo
  PRIVATE_LINK_LIBRARIES GoodRunsListsLib PathResolver)

atlas_add_dictionary(RDFGoodRunsListsDict
  RDFGoodRunsLists/RDFGoodRunsListsDict.h
  RDFGoodRunsLists/selection.xml
  LINK_LIBRARIES RDFGoodRunsListsLib)

# Test application(s) in the package.
atlas_add_executable(rdfgrl_test
  utils/rdfgrl_test.cxx
  INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
  LINK_LIBRARIES ${ROOT_LIBRARIES} xAODRootAccess xAODDataSourceLib
                 RDFGoodRunsListsLib)

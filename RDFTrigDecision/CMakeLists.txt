# Copyright (C) 2023 CERN for the benefit of the ATLAS collaboration

# Set the name of the package.
atlas_subdir(RDFTrigDecision)

# External dependencies.
find_package(ROOT COMPONENTS Core Gpad)

# Component(s) in the package.
atlas_add_library(RDFTrigDecisionLib
  RDFTrigDecision/*.h src/*.cxx
  PUBLIC_HEADERS RDFTrigDecision
  PRIVATE_LINK_LIBRARIES TrigDecisionToolLib TrigConfxAODLib)

# Test application(s) in the package.
atlas_add_executable(rdftdt_test
  utils/rdftdt_test.cxx
  INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
  LINK_LIBRARIES ${ROOT_LIBRARIES} xAODDataSourceLib RDFTrigDecisionLib)
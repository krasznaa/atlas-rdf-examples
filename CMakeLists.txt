# Copyright (C) 2023 CERN for the benefit of the ATLAS collaboration

# Set up the project.
cmake_minimum_required(VERSION 3.20)
project(ATLASRDFExamples VERSION 0.2.0 LANGUAGES CXX)

# Find the base release.
find_package(AnalysisBase 24.2 REQUIRED)

# Generate a compile_commands.json file by default.
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE CACHE BOOL
   "Generate compile_commands.json" FORCE)

# Set up CTest.
atlas_ctest_setup()

# Set up the analysis project from the repository.
atlas_project(USE AnalysisBase ${AnalysisBase_VERSION})

# Generate an environment setup script. This call makes sure that whatever
# "external" we use in this analysis project, is set up correctly in the
# runtime environment.
lcg_generate_env(SH_FILE "${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/env_setup.sh")
install(FILES "${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/env_setup.sh"
   DESTINATION .)

# Set up CPack. This is necessary to be able to build TGZ files correctly from
# the compiled code. Making it possible to use the code on the grid.
atlas_cpack_setup()

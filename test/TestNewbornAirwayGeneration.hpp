/*

Copyright (c) 2005-2023, University of Oxford.
All rights reserved.

University of Oxford means the Chancellor, Masters and Scholars of the
University of Oxford, having an administrative office at Wellington
Square, Oxford OX1 2JD, UK.

This file is part of Chaste.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
 * Neither the name of the University of Oxford nor the names of its
   contributors may be used to endorse or promote products derived from this
   software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef TESTHELLO_NEWBORNAIRWAYGENERATION_HPP_
#define TESTHELLO_NEWBORNAIRWAYGENERATION_HPP_

// Test facilities
#include <cxxtest/TestSuite.h>

// VTK dependencies
#include "vtkTriangleFilter.h"
#include "vtkMassProperties.h"
#include "vtkSmartPointer.h"
#include "vtkSTLReader.h"

// Mesh handling
#include "TrianglesMeshWriter.hpp"
#include "TetrahedralMesh.hpp"

// Lung dependencies
#include "MultiLobeAirwayGenerator.hpp"

/* Most Chaste code uses PETSc to solve linear algebra problems.  This involves starting PETSc at the beginning of a test-suite
 * and closing it at the end.  (If you never run code in parallel then it is safe to replace PetscSetupAndFinalize.hpp with FakePetscSetup.hpp)
 */
#include "PetscSetupAndFinalize.hpp"

/**
 * @file
 *
 * This test is used to generate a full anatomical surrogate of a
 * Newborn airways.  You have to provide it with two files
 * (airways.node and airways.edge), which are related to the first
 * airway generations.  Read the example files to know more about
 * their structures.  Provide also the stl models for the lobes: they
 * will be treated as physical limits to the airway growth.  Be sure
 * you have set up CHASTE_OUTPUT_DIR environmental variable first (see
 * Chaste Documentation).  Run on your Chaste build folder the
 * following command:
 *
 * make -j4 TestNewbornAirwayGeneration
 * ctest -V -R TestNewbornAirwayGeneration
 */

class TestNewbornAirwayGeneration : public CxxTest::TestSuite
{
public:
  void TestNewbornAirwayGenerationClass()
  {
    // Instantiate Mesh and its Reader.
    TetrahedralMesh<1,3> airways_mesh;
    // TrianglesMeshReader<1,3> airways_mesh_reader("projects/NewbornAirwayGeneration/test/data/example_airways");
    TrianglesMeshReader<1,3> airways_mesh_reader("projects/NewbornAirwayGeneration/test/data/airways");
    airways_mesh.ConstructFromMeshReader(airways_mesh_reader);

    // Instantiate the complete airway generator.
    MultiLobeAirwayGenerator generator(airways_mesh);

    // Set up generator parameters. See this class in the
    // Documentation for more info.
    generator.SetNumberOfPointsPerLung(15000);
    generator.SetBranchingFraction(0.4);
    generator.SetDiameterRatio(1.15);

    generator.SetMinimumBranchLength(0.00001);
    generator.SetPointLimit(1);
    generator.SetAngleLimit(180.0);

    // Set up physical limits.
    vtkSmartPointer<vtkSTLReader> lll_reader = vtkSmartPointer<vtkSTLReader>::New();
    lll_reader->SetFileName("projects/NewbornAirwayGeneration/test/data/LLL.stl");
    lll_reader->Update();
    generator.AddLobe(lll_reader->GetOutput(), LEFT);

    vtkSmartPointer<vtkSTLReader> lul_reader = vtkSmartPointer<vtkSTLReader>::New();
    lul_reader->SetFileName("projects/NewbornAirwayGeneration/test/data/LUL.stl");
    lul_reader->Update();
    generator.AddLobe(lul_reader->GetOutput(), LEFT);

    vtkSmartPointer<vtkSTLReader> rll_reader = vtkSmartPointer<vtkSTLReader>::New();
    rll_reader->SetFileName("projects/NewbornAirwayGeneration/test/data/RLL.stl");
    rll_reader->Update();
    generator.AddLobe(rll_reader->GetOutput(), RIGHT);

    // vtkSmartPointer<vtkSTLReader> rml_reader = vtkSmartPointer<vtkSTLReader>::New();
    // rml_reader->SetFileName("lung/test/data/rml.stl");
    // rml_reader->Update();
    // generator.AddLobe(rml_reader->GetOutput(), RIGHT);

    vtkSmartPointer<vtkSTLReader> rul_reader = vtkSmartPointer<vtkSTLReader>::New();
    rul_reader->SetFileName("projects/NewbornAirwayGeneration/test/data/RUL.stl");
    rul_reader->Update();
    generator.AddLobe(rul_reader->GetOutput(), RIGHT);

    // Start growing airways
    generator.AssignGrowthApices();
    generator.DistributePoints();
    
    // Save results in `$CHASTE_OUTPUT_DIR/TestNewbornAirwayGeneration/`
    generator.Generate("TestNewbornAirwayGeneration", "complete_conducting_airway");
  }
};

#endif /*TESTHELLO_NEWBORNAIRWAYGENERATION_HPP_*/

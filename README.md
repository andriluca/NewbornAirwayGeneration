# Generation of Newborn Airways' Anatomy

This repository contains a Chaste User Project used to generate the
anatomical surrogate of newborn airways.

## Before starting

Edit your `~/.bashrc` by adding the following lines at the end of the
file:

```bash
	# ...at the end of ~/.bashrc
	export CHASTE_TEST_OUTPUT=$HOME/Chaste-Tests-Output
	export CHASTE_BUILD_DIR=$HOME/Builds/Chaste
	export CHASTE_REPO_DIR=$HOME/Repos/Chaste
```

Replace `Chaste-Tests-Output` with the preferred location for Tests'
output.

It's possible to do that by using this command

```bash
	# Run this to append the former line.
	echo "export CHASTE_TEST_OUTPUT=$HOME/Chaste-Tests-Output" >> ~/.bashrc
	echo "export CHASTE_BUILD_DIR=$HOME/Builds/Chaste" >> ~/.bashrc
	echo "export CHASTE_REPO_DIR=$HOME/Repos/Chaste" >> ~/.bashrc
```

Source the file just modified and create the required folders:

```bash
	source ~/.bashrc
	mkdir -p $CHASTE_TEST_OUTPUT ~/Repos $CHASTE_BUILD_DIR
```

Clone Chaste repository.

```bash
	cd ~/Repos
	git clone https://github.com/Chaste/Chaste
```

Please refer to [Chaste
guide](https://chaste.github.io/docs/installguides/) to complete this
passage.

Navigate to `$CHASTE_REPO_DIR/projects` and clone this project in that
location.

```bash
	cd $CHASTE_REPO_DIR/projects
	git clone https://github.com/andriluca/NewbornAirwayGeneration
```

Now Chaste repository is set. You can proceed and install Chaste and
this project from source!


## Compile the User Project

Run `cmake` from Chaste build folder.

```bash
	cd $CHASTE_BUILD_DIR
	cmake $CHASTE_REPO_DIR
```

Compile with `make`.

```bash
	# Number of available threads on your machine.
	N=4
	
	# Navigate to Chaste build directory and compile both `lung` and `TestNewbornAirwayGeneration` target.
	cd $CHASTE_BUILD_DIR
	make -j$N lung
	make -j$N TestNewbornAirwayGeneration
	
	# Run this test.
	ctest -V -R TestNewbornAirwayGeneration
```

Checkout if the test is 100% passed.  If so congratulations, you have
successfully installed this User Project.

## Input

It's required to provide two pieces of input:
1. Major airways
2. Lobe segmentations

### Major airways

It is possible to start from two source files:
1. `airway.node`: List of nodes.  Each node represent a point in the
   major airways centreline with 3 coordinates (x, y, z) and two
   attributes: `radius` and `start_id`.  `radius` (self explainatory)
   is evaluated in that point.  `start_id` is a flag being 1 for
   generative points (i.e. points from which branching is allowed), 0
   otherwise.
2. `airway.edge`: List of edges.  Each edge is connecting two
   endpoints.

You can learn more about such files by referring to Chaste tests
`TestMultiLobeAirwayGenerator.hpp` and
`TestAirwayGenerationTutorial.hpp`.

It's also possible to start from one single vtu files:
- `major_airways.vtu`: vtk unstructured grid file

### Lobe segmentations

They represent physical limits to airway "growth" process and should
be five:
1. `LLL.stl`: Left, Lower Lung
1. `LUL.stl`: Left, Upper Lung
1. `RLL.stl`: Right Lower Lung
1. `RML.stl`: Right Mid Lung
1. `RUL.stl`: Right Upper Lung

## Output

Generated output is located in
`$CHASTE_TEST_OUTPUT/TestNewbornAirwayGeneration/`.  Navigate to that
folder and open the files `complete_conducting_airway.node` and
`complete_conducting_airway.edge`.  Other output files are available
in the same folder.  Use them at your will.

## References

1. [Chaste website](https://chaste.github.io/)
2. [My MSc. thesis](https://github.com/andriluca/Thesis/)

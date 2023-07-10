# On-the-fly critical-path tool
<img src="media/OTF-CPT.png" hspace="5" vspace="5" height="45%" width="45%" alt="OTF-CPT Logo" title="OTF-CPT" align="right" />

Tool to collect and report model factors (aka. fundamental performance factors) for hybrid MPI + OpenMP applications on-the-fly.

## Build with CMake
The C/C++ compiler needs to support OMPT (which is provided by icc/clang and others, but not gcc)
### Basic Cmake
```
mkdir BUILD
cd BUILD
cmake ../
make -j8
```

### Cmake with Clang
To use clang with IntelMPI/OpenMPI/MPICH, export the following env variables first:
```
export MPICH_CC=clang; export MPICH_CXX=clang++;
export OMPI_CC=clang; export OMPI_CXX=clang++;
mkdir BUILD
cd BUILD
cmake ../
make -j8
```

### Cmake with MPICH + Clang on Ubuntu
The MPICH compiler wrapper on Ubuntu bricks MPI detection in cmake, work-around as following:
```
export MPICH_CC=clang; export MPICH_CXX=clang++;
mkdir BUILD
cd BUILD
CC=mpicc.mpich CXX=mpicxx.mpich LDFLAGS=-flto=auto cmake ../
make -j8
```

### Cmake with OpenMPI + Clang on Ubuntu
```
export OMPI_CC=clang; export OMPI_CXX=clang++;
mkdir BUILD
cd BUILD
CC=mpicc.openmpi CXX=mpicxx.openmpi cmake ../
make -j8
```

## Build using the Makefile
The Makefile accepts several configuration variables to be overwritten:
- MPI defaults to `openmpi`. This will be used as infix in most generated files
- MPICC defaults to `mpicc.$(MPI)`. In combination with the MPI variable this allows easy switching of MPI on Ubuntu
- MPICXX defaults to `mpicxx.$(MPI)`. In combination with the MPI variable this allows easy switching of MPI on Ubuntu
- MPIRUN defaults to `mpirun.$(MPI)`. In combination with the MPI variable this allows easy switching of MPI on Ubuntu

### Run targets in the Makefile
- `run-test` - runs 4 tests loading the shared library tool implementation
- `run-test-static` - runs 4 tests that have the tool implementation statically built into the test

## Using the tool with an application
Depending on the system and how libomp.so is built, LD_PRELOAD and OMP_TOOL_LIBRARIES might both be necessary. Assuming a cmake build as described above, an application with OTF-CPT is executed like:
```
$(MPIRUN) -np 2 env OMP_NUM_THREADS=4 LD_PRELOAD=./BUILD/libOTFCPT.so OMP_TOOL_LIBRARIES=./BUILD/libOTFCPT.so ./app
```

At the moment, the tool supports selective instrumentation with a single pair of start/stop markers:
```
MPI_Pcontrol(1); // start
// region of interest
MPI_Pcontrol(0); // stop
```



## Files
### MPI Function Wrappers
- completion-wrappers.cpp  - Request completion calls
- man-wrappers.cpp     - Manually modified MPI wrappers with special semantics regarding handles
- gen-nb-wrappers.cpp  - Generated non-blocking communication calls
- gen-wrappers.cpp     - Generated MPI wrappers with simple IN or OUT semantics regarding handles
- tracking.cpp         - implementation of some class functions defined in handle-data.h and tracking.h 

### OTF-CPT
- critical-core.cpp  - OTF-CPT core functions
- ompt-critical.cpp  - OMPT specific code for OTF-CPT
- mpi-critical.cpp   - MPI specific code for OTF-CPT

### wrap.py Templates
- gen-nb-wrappers.w
- gen-wrappers.w

```
external/wrap/wrap.py -s -n gen-wrappers.w -o gen-wrappers.cpp
```
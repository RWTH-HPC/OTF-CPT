#CXXFLAGS=-gdwarf-4 -fopenmp -O3 -std=c++17 -stdlib=libc++ -DUSE_MPI -fPIC
CXXFLAGS=-gdwarf-4 -fopenmp -O3 -std=c++17 -DUSE_MPI -fPIC -I$(PWD)
LDFLAGS=
#-flto -rdynamic -Wl,--export-dynamic -Wl,--no-as-needed
#MPI=mpich
MPI?=openmpi
CXX=time mpicxx.$(MPI)
WRAP?=external/wrap/wrap.py
MPICXX?=time mpicxx.$(MPI)
MPICC?=mpicc.$(MPI)
MPIRUN?=mpirun.$(MPI)

ifeq ($(MPI), mpich)
CXXFLAGS+= -DHAVE_COUNT -DHAVE_SESSION -DHAVE_PCOLL
endif

.PHONY: all lib test
all: test lib test-static
lib: libOTFCPT.$(MPI).so
test: test.$(MPI) test-wc-tag.$(MPI) test-wc-source.$(MPI) test-grp.$(MPI)
test-static: test-statictool.$(MPI) test-wc-tag-statictool.$(MPI) test-wc-source-statictool.$(MPI) test-grp-statictool.$(MPI)

.PRECIOUS: %.$(MPI).cpp

TOOL-OBJS = tracking.$(MPI).o completion-wrappers.$(MPI).o gen-nb-wrappers.$(MPI).$(MPI).o gen-wrappers.$(MPI).$(MPI).o man-wrappers.$(MPI).o critical-core.$(MPI).o mpi-critical.$(MPI).o ompt-critical.$(MPI).o

%.$(MPI).cpp: %.w
	$(WRAP) -s -n -c $(MPICC) $< -o $@
	sed -i -e '/ = PMPI_[^(]*_init_\?c\?(/{n;s/postRequest/postRequestInit/}' -e 's/^_EXTERN_C_ //' $@
	if (which clang-format > /dev/null 2>&1); then clang-format -i $@; fi

#gen-nb-wrappers.$(MPI).$(MPI).o: completion-wrappers.cpp

%.$(MPI).o : %.cpp tracking.h handle-data.h timing.h criticalPath.h mpi-critical.h Makefile
	$(MPICXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

%.$(MPI).$(MPI).o : %.$(MPI).cpp tracking.h handle-data.h timing.h criticalPath.h mpi-critical.h Makefile
	$(MPICXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

libOTFCPT.$(MPI).so : $(TOOL-OBJS)
	$(MPICXX) -shared -fPIC $^ $(CXXFLAGS) $(LDFLAGS) $(CPPFLAGS) -o $@

test.$(MPI) : tests/test.$(MPI).o 
	$(MPICXX) $^ $(CXXFLAGS) $(LDFLAGS) $(CPPFLAGS) -o $@

test-statictool.$(MPI) : tests/test.$(MPI).o $(TOOL-OBJS)
	$(MPICXX) $^ $(CXXFLAGS) $(LDFLAGS) $(CPPFLAGS) -o $@

test-wc-tag.$(MPI) : tests/test-wc-tag.$(MPI).o 
	$(MPICXX) $^ $(CXXFLAGS) $(LDFLAGS) $(CPPFLAGS) -o $@

test-wc-tag-statictool.$(MPI) : tests/test-wc-tag.$(MPI).o $(TOOL-OBJS)
	$(MPICXX) $^ $(CXXFLAGS) $(LDFLAGS) $(CPPFLAGS) -o $@

test-wc-source.$(MPI) : tests/test-wc-source.$(MPI).o 
	$(MPICXX) $^ $(CXXFLAGS) $(LDFLAGS) $(CPPFLAGS) -o $@

test-wc-source-statictool.$(MPI) : tests/test-wc-source.$(MPI).o $(TOOL-OBJS)
	$(MPICXX) $^ $(CXXFLAGS) $(LDFLAGS) $(CPPFLAGS) -o $@

test-grp.$(MPI) : tests/test-group.$(MPI).o 
	$(MPICXX) $(CXXFLAGS) $(LDFLAGS) $(CPPFLAGS) $^ -o $@

test-grp-statictool.$(MPI) : tests/test-group.$(MPI).o $(TOOL-OBJS)
	$(MPICXX) $(CXXFLAGS) $(LDFLAGS) $(CPPFLAGS) $^ -o $@

run-test: test lib
	$(MPIRUN) -np 2 env OMP_NUM_THREADS=1 LD_PRELOAD=./libOTFCPT.$(MPI).so OMP_TOOL_LIBRARIES=./libOTFCPT.$(MPI).so ./test.$(MPI)
	$(MPIRUN) -np 2 env OMP_NUM_THREADS=2 LD_PRELOAD=./libOTFCPT.$(MPI).so OMP_TOOL_LIBRARIES=./libOTFCPT.$(MPI).so ./test.$(MPI)
	$(MPIRUN) -np 2 env OMP_NUM_THREADS=1 LD_PRELOAD=./libOTFCPT.$(MPI).so OMP_TOOL_LIBRARIES=./libOTFCPT.$(MPI).so ./test-wc-source.$(MPI)
	$(MPIRUN) -np 2 env OMP_NUM_THREADS=1 LD_PRELOAD=./libOTFCPT.$(MPI).so OMP_TOOL_LIBRARIES=./libOTFCPT.$(MPI).so ./test-wc-tag.$(MPI)

run-test-static: test-static 
	$(MPIRUN) -np 2 env OMP_NUM_THREADS=1 ./test-statictool.$(MPI)
	$(MPIRUN) -np 2 env OMP_NUM_THREADS=2 ./test-statictool.$(MPI)
	$(MPIRUN) -np 2 env OMP_NUM_THREADS=1 ./test-wc-source-statictool.$(MPI)
	$(MPIRUN) -np 2 env OMP_NUM_THREADS=1 ./test-wc-tag-statictool.$(MPI)

clean:
	rm -f *.o *~ tests/*.o
mrproper: clean
	rm -f *.mpich *.openmpi *.intelmpi *.so

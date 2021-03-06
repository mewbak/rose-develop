include $(top_srcdir)/config/Makefile.for.ROSE.includes.and.libs

noinst_PROGRAMS = parseFailSafe
parseFailSafe_SOURCES = parseFailSafe.C

SUBDIRS=
#if ROSE_BUILD_FORTRAN_LANGUAGE_SUPPORT
#  SUBDIRS += fortran 
#endif

AM_CPPFLAGS = $(ROSE_INCLUDES)
AM_LDFLAGS = $(ROSE_RPATHS)
LDADD    = $(ROSE_LIBS)

C_TESTCODES_REQUIRED_TO_PASS = \
	assertion_region.c \
	status_predicate.c \
	data_predicate.c

CXX_TESTCODES_REQUIRED_TO_PASS = 
TESTCODE_CURRENTLY_FAILING = \
	data_predicate_macro.c

ROSE_FLAGS = -rose:failsafe #--edg:no_warnings -w -rose:verbose 0 --edg:restrict #-rose:wave 
# used to find omp.h, added it as one of  rose headers
TESTCODE_INCLUDES = -I$(top_srcdir)/src/frontend/SageIII

PASSING_C_TEST_Objects = $(C_TESTCODES_REQUIRED_TO_PASS:.c=.o)

PASSING_CXX_TEST_Objects = $(CXX_TESTCODES_REQUIRED_TO_PASS:.cpp=.o)

$(PASSING_C_TEST_Objects): %.o: $(srcdir)/%.c parseFailSafe
	./parseFailSafe $(ROSE_FLAGS) $(TESTCODE_INCLUDES) -c $<

$(PASSING_CXX_TEST_Objects): %.o: $(srcdir)/%.cpp parseOmp
	./parseFailSafe ${ROSE_FLAGS} ${TESTCODE_INCLUDES} -c $<

check-c:$(PASSING_C_TEST_Objects) 
check-cxx:$(PASSING_CXX_TEST_Objects)

ALL_TESTCODES = \
$(C_TESTCODES_REQUIRED_TO_PASS) \
$(CXX_TESTCODES_REQUIRED_TO_PASS) \
$(C_OMP_ACC_TESTCODES_REQUIRED_TO_PASS) \
$(TESTCODE_CURRENTLY_FAILING)

if ROSE_BUILD_CXX_LANGUAGE_SUPPORT
conditional-check-local:
	@echo "Test for ROSE FailSafe Parser."
	@$(MAKE) $(PASSING_C_TEST_Objects) 
#	@$(MAKE) $(PASSING_CXX_TEST_Objects)
	@echo "***********************************************************************************************************"
	@echo "****** ROSE/tests/nonsmoke/functional/CompileTests/FailSafe_tests: make check rule complete (terminated normally) ******"
	@echo "***********************************************************************************************************"
else
conditional-check-local:
	@echo "Test for ROSE FailSafe Parser."
	@echo "***********************************************************************************************************"
	@echo "****** ROSE/tests/nonsmoke/functional/CompileTests/FailSafe_tests: make check rule for C/C++ skipped (terminated normally) ******"
	@echo "***********************************************************************************************************"
endif

EXTRA_DIST = $(ALL_TESTCODES) README

check-local: conditional-check-local

clean-local:
	rm -f *.o rose_*.* *.out
	rm -rf QMTest


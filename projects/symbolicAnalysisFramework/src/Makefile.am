include $(top_srcdir)/config/Makefile.for.ROSE.includes.and.libs

SUBDIRS =  \
         external \
         mpiAnal \
         ompAnal \
         unionFind \
         varBitVector \
         varLatticeVector \
         chkptRangeAnalysis \
	 taintAnalysis \
	 parallelCFG
#Liao, 2.24.2012
# removed duplicated subdirectories
#         common \
#         analysis \
#         arrIndexLabeler \
#         cfgUtils \
#         lattice \
#         rwAccessLabeler \
#         simpleAnalyses \
#         state \
#         variables 

# there are inter dependencies among sub directories, parallel build is a bad idea for now
#.NOTPARALLEL:
#	echo "parallel build is disabled in this directory to ease complex inter-directory dependencies"

# Saf stands for symbolic analysis framework
noinst_LTLIBRARIES = libSafall.la
libSafall_la_SOURCES = dummyCppFileForLibSafall.C

libSafall_la_LIBADD = \
          $(top_builddir)/projects/symbolicAnalysisFramework/src/external/libSafexternal.la \
          $(top_builddir)/projects/symbolicAnalysisFramework/src/ompAnal/libSafompAnal.la \
          $(top_builddir)/projects/symbolicAnalysisFramework/src/unionFind/libSafunionFind.la \
          $(top_builddir)/projects/symbolicAnalysisFramework/src/varBitVector/libSafvarBitVector.la 
#          $(top_builddir)/projects/symbolicAnalysisFramework/src/common/libSafcommon.la \
#          $(top_builddir)/projects/symbolicAnalysisFramework/src/analysis/libSafanalysis.la \
#          $(top_builddir)/projects/symbolicAnalysisFramework/src/arrIndexLabeler/libSafarrIndexLabeler.la \
#          $(top_builddir)/projects/symbolicAnalysisFramework/src/cfgUtils/libSafcfgUtils.la \
#          $(top_builddir)/projects/symbolicAnalysisFramework/src/lattice/libSaflattice.la \
#          $(top_builddir)/projects/symbolicAnalysisFramework/src/rwAccessLabeler/libSafrwAccessLabeler.la \
#          $(top_builddir)/projects/symbolicAnalysisFramework/src/simpleAnalyses/libSafsimpleAnalyses.la \
#          $(top_builddir)/projects/symbolicAnalysisFramework/src/state/libSafstate.la \
#          $(top_builddir)/projects/symbolicAnalysisFramework/src/variables/libSafvariables.la
# Liao, not yet compilable 	  
#          $(top_builddir)/projects/symbolicAnalysisFramework/src/varLatticeVector/libSafvarLatticeVector.la
#          $(top_builddir)/projects/symbolicAnalysisFramework/src/chkptRangeAnalysis/libSafchkptRangeAnalysis.la 
#          $(top_builddir)/projects/symbolicAnalysisFramework/src/mpiAnal/libSafmpiAnal.la

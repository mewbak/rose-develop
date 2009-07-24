#ifndef H_SATIRE_DATAFLOW_ANALYSIS
#define H_SATIRE_DATAFLOW_ANALYSIS

#include "satire.h"

#include <cstdarg>

namespace SATIrE {

class DataFlowAnalysis: public Analysis
{
public:
    virtual std::string identifier() const;
    virtual std::string description() const;

 // DataFlowAnalysis defines a common way to run data-flow analyzers by
 // providing implementations of these methods. The user need not override
 // these further.
    void run(Program *program);
    void processResults(Program *program);
 // The prefixFiles flag determines whether to prefix the generated files
 // (term, ICFG, etc.) with the analysis identifier.
    void processResultsWithPrefix(Program *program, bool prefixFiles = true);
    bool query(std::string query, ...);

 // The DataFlowAnalysis class provides a general framework for data-flow
 // analyzers generated by PAG; the details of a specific analysis are
 // encapsulated in an instance of this delegate class. This ensures that
 // the DataFlowAnalysis class is completely independent of function or
 // object names generated by PAG, and of the type of data flow information
 // computed by the analysis.
 // This class is abstract; it is made concrete by subclassing and defining
 // the methods and other details, for which SATIrE provides templates.
    class Implementation
    {
    public:
        virtual std::string identifier() const = 0;
        virtual std::string description() const = 0;

     // Methods for wrapping PAG's generated functions.
        virtual void analysisDoit(CFG *cfg) = 0;
        virtual void makePersistent() const = 0;

     // Methods for annotating the program and generating various annotated
     // representations. Note that these methods do not get the analysis
     // results as parameter; typically, this means that the subclass must
     // define a member to hold the analysis results computed by the
     // analysisDoit method. Note also that these are not const, since they
     // might have to pass their analysis info around.
        virtual void outputAnalysisVisualization(
                Program *program, bool prefixFiles = false) = 0;
        virtual void annotateProgram(Program *program) = 0;
        virtual void outputAnnotatedProgram(
                Program *program, bool prefixFiles = false) = 0;

     // Forwarded query method.
        virtual bool query(std::string query, va_list args) const = 0;

     // Methods for setting possibly prefixed PAG options.
        virtual void setDebugStat(int debugStat) const = 0;
        virtual void setGlobalRetfunc(int globalRetfunc) const = 0;
    };

 // A pointer to an instance of the Implementation must be passed in to
 // construct a DataFlowAnalysis; the Implementation object must live as
 // long as the DataFlowAnalysis instance lives, but its ownership is not
 // transferred to the DataFlowAnalysis object.
    DataFlowAnalysis(Implementation *implementation);
    ~DataFlowAnalysis();

private:
    Implementation *p_impl;

 // Copying of DataFlowAnalysis objects is explicitly prohibited.
    DataFlowAnalysis(const DataFlowAnalysis &);
    DataFlowAnalysis &operator=(const DataFlowAnalysis &);

#if HAVE_PAG
 // Helper methods for computing/printing call strings.
    void computeCallStrings(Program *program) const;
    void outputContextData(Program *program) const;
#endif
};

// Generated function that returns an instance of a provided data-flow
// analyzer.
DataFlowAnalysis *makeProvidedAnalyzer(const char *name);

}

#endif

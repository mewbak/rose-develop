#include <BinaryConcolic.h>

namespace Rose {
namespace BinaryAnalysis {
namespace Concolic {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LinuxExitStatus
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// class method
LinuxExitStatus::Ptr
LinuxExitStatus::create(const std::string databaseUrl, const boost::filesystem::path &executableName,
                        const std::vector<std::string> &arguments) {
    // Create the initial test case
    std::string name = executableName.filename().native();
    Specimen::Ptr specimen = Specimen::instance(executableName);
    TestCase::Ptr testCase0 = TestCase::instance(specimen);
    testCase0->name(name + " #0");

    // Create the database
    Database::Ptr db = Database::create("sqlite:" + name + ".db", name);
    db->id(testCase0);                                  // save the first test case, side effect of obtaining an ID

    return Ptr(new LinuxExitStatus(db));
}

void
LinuxExitStatus::run() {
    LinuxExecutor::Ptr concreteExecutor = LinuxExecutor::instance();
    SymbolicExecutor::Ptr symbolicExecutor = SymbolicExecutor::instance();

    while (!isFinished()) {
        // Run as many test cases concretely as possible.
        while (Database::TestCaseId testCaseId = pendingConcreteResult()) {
            TestCase::Ptr testCase = database()->object(testCaseId);
            std::unique_ptr<ConcreteExecutor::Result> concreteResult = concreteExecutor->execute(testCase);
            insertConcreteResults(testCase, *concreteResult);
        }

        // Now that all the test cases have run concretely, run a few of the "best" ones symbolically.  The "best" is defined
        // either by the ranks returned from the concrete executor, or by this class overriding pendingSymbolicResult (which we
        // haven't done).
        BOOST_FOREACH (Database::TestCaseId testCaseId, pendingSymbolicResults(10 /*arbitrary*/)) {
            TestCase::Ptr testCase = database()->object(testCaseId);
            std::vector<TestCase::Ptr> newTestCases = symbolicExecutor->execute(testCase);
            insertSymbolicResults(testCase, newTestCases);
        }
    }
}

} // namespace
} // namespace
} // namespace

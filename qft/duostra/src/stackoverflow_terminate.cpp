#include "stackoverflow_terminate.hpp"

#include <boost/stacktrace.hpp>
#define BOOST_STACKTRACE_USE_BACKTRACE

void on_terminate() {
    try {
        auto unknown = std::current_exception();
        if (unknown) {
            std::cout << boost::stacktrace::stacktrace() << "\n";
            abort();
        } else {
            std::cerr << "normal termination" << std::endl;
        }
    } catch (const std::exception& e) {  // for proper `std::` exceptions
        std::cerr << "unexpected exception: " << e.what() << std::endl;
    } catch (...) {  // last resort for things like `throw 1;`
        std::cerr << "unknown exception" << std::endl;
    }
}

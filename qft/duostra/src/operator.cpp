#include "operator.hpp"

std::string operator_get_name(Operator op) {
    switch (op) {
        case Operator::Swap:
            return "Swap";
        case Operator::CX:
            return "CX";
        case Operator::Single:
            return "Single";
    }
    return "Error";
}

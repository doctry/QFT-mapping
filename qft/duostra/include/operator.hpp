#pragma once

#include <iostream>
#include <string>

enum Operator { Swap, CX, Single };

std::string operator_get_name(Operator op);

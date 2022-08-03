#pragma once
#include <execinfo.h>
#include <signal.h>
#include <string.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

void on_terminate();

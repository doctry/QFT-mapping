#include "tqdm_wrapper.hpp"
#include "tqdm.h"

using namespace std;

// This file is used to hide all abstractions that requires tqdm.

TqdmWrapper::TqdmWrapper(size_t total)
    : counter_(0), total_(total), tqdm_(make_unique<tqdm>()) {}

TqdmWrapper::TqdmWrapper(int total) : TqdmWrapper(static_cast<size_t>(total)) {}

TqdmWrapper::~TqdmWrapper() {
    tqdm_->finish();
}

void TqdmWrapper::add() {
    tqdm_->progress(counter_++, total_);
}

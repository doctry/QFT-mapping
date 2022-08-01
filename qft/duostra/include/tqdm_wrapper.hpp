#pragma once

#include "externals/cpptqdm/tqdm.h"

class TqdmWrapper {
   public:
    TqdmWrapper(size_t total) : counter_(0), total_(total), tqdm_(tqdm()) {}
    TqdmWrapper(int total) : TqdmWrapper(static_cast<size_t>(total)) {}
    ~TqdmWrapper() { tqdm_.finish(); }

    size_t idx() const { return counter_; }
    bool done() const { return counter_ == total_; }
    void add() { tqdm_.progress(counter_++, total_); }
    TqdmWrapper& operator++() { return add(), *this; }

   private:
    size_t counter_;
    size_t total_;
    tqdm tqdm_;
};

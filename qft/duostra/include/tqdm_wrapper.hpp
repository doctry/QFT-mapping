#pragma once

#include <memory>

class tqdm;

class TqdmWrapper {
   public:
    TqdmWrapper(size_t total);
    TqdmWrapper(int total);
    ~TqdmWrapper();

    size_t idx() const { return counter_; }
    bool done() const { return counter_ == total_; }
    void add();
    TqdmWrapper& operator++() { return add(), *this; }

   private:
    size_t counter_;
    size_t total_;

    // Using a pointer so we don't need to know tqdm's size in advance.
    // This way, no need to #include it in the header
    // because although tqdm works well, it's not expertly written
    // which leads to a lot of warnings.
    std::unique_ptr<tqdm> tqdm_;
};

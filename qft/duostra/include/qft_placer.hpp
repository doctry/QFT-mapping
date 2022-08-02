#pragma once

#include <memory>
#include <random>
#include <vector>
#include "q_device.hpp"

namespace placer {
using namespace std;
using namespace device;

class Base {
   public:
    Base() {}
    Base(const Base& other) = delete;
    Base(Base&& other) = delete;
    virtual ~Base() {}

    void place_and_assign(Device& device) {
        auto assign = place(device);
        device.place(assign);
    }

   protected:
    virtual vector<size_t> place(Device& device) const = 0;
};

class Random : public Base {
   public:
    ~Random() override {}

   protected:
    vector<size_t> place(Device& device) const override;
};

class Static : public Base {
   public:
    ~Static() override {}

   protected:
    vector<size_t> place(Device& device) const override;
};

class DFS : public Base {
   public:
    ~DFS() override {}

   protected:
    vector<size_t> place(Device& device) const override;

   private:
    void dfs_device(size_t current,
                    Device& device,
                    vector<size_t>& assign,
                    vector<bool>& qubit_mark) const;
};

std::unique_ptr<Base> get(const string& typ);

}  // namespace placer

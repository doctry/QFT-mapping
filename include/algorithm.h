#include "qft_topo.h"
#include "device.h"

#pragma once

class QFTMapper {
public:
QFTMapper();
QFTMapper(const QFTMapper &other) = delete;
QFTMapper(QFTMapper &&other);

void init_place(topo::QFTTopology qft_topo, device::Device device);
private:

};
/*
 * Copyright (c) Shelly-HomeKit Contributors
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "shelly_pm_shunt.hpp"

#ifdef MGOS_HAVE_ADC

#include <cmath>

#include "mgos_adc.h"

namespace shelly {

ShuntPowerMeter::ShuntPowerMeter(int id, int adc_channel, float vin)
    : PowerMeter(id), adc_channel_(adc_channel), vin_(vin) {
}

Status ShuntPowerMeter::Init() {
  if (!mgos_adc_enable(adc_channel_)) {
    return mgos::Errorf(STATUS_UNAVAILABLE, "failed to enable ADC channel %d",
                        adc_channel_);
  }
  return Status::OK();
}

ShuntPowerMeter::~ShuntPowerMeter() {
}

StatusOr<float> ShuntPowerMeter::GetPowerW() {
  int raw_offset = 400;
  float raw_scaling = 0.035;  // amperes per dac unit

  int raw = mgos_adc_read(adc_channel_);

  float i = (raw - raw_offset) * raw_scaling;
  i = std::max(i, 0.0f);  // no negative values

  LOG(LL_DEBUG, ("ADC readings: %d, v %.3f, i %.3f", raw, vin_, i));
  return i * vin_;
}

StatusOr<float> ShuntPowerMeter::GetEnergyWH() {
  return mgos::Errorf(STATUS_UNIMPLEMENTED, "feature not implemented");
}

}  // namespace shelly

#endif  // MGOS_HAVE_ADC

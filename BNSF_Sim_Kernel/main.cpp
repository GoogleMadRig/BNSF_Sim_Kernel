//
//  main.cpp
//  BNSF_Sim_Kernel
//
//  Created by David Heiblum on 4/5/26.
//

#include <iostream>
#include <string>

// We use a 'struct' for simple data bundles.
// Think of this as the "Waybill" for the locomotive.
struct LocomotiveStatus {
    std::string unit_id;
    double current_speed;  // in MPH
    double track_limit;    // in MPH
    bool signal_clear;     // Green vs Red signal
};

int main() {
    // 1. Initializing our "Freight"
    LocomotiveStatus bnsf_7742 = {"BNSF-7742", 45.0, 50.0, true};

    // 2. The Safety Interlock Logic
    // This is the "Manual Management" you mentioned—C++ checking every car.
    if (bnsf_7742.current_speed > bnsf_7742.track_limit) {
        std::cout << "CRITICAL: " << bnsf_7742.unit_id << " EXCEEDING SPEED LIMIT!" << std::endl;
        // In a real BNSF system, this would trigger the air brakes automatically.
    } else if (!bnsf_7742.signal_clear) {
        std::cout << "SIGNAL RED: Initiating mandatory stop sequence for " << bnsf_7742.unit_id << "." << std::endl;
    } else {
        std::cout << bnsf_7742.unit_id << " proceeding on main line at " << bnsf_7742.current_speed << " MPH." << std::endl;
    }

    return 0;
}

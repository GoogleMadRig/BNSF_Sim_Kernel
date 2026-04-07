#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <cmath>

// --- The Fuel System Component (PaaS Logic) ---
class FuelSystem {
private:
    double tank_capacity; // Gallons
    double current_fuel;
    double efficiency_factor; // Based on throttle notch (0.0 to 1.0)

public:
    FuelSystem(double cap, double efficiency)
        : tank_capacity(cap), current_fuel(cap), efficiency_factor(efficiency) {}

    // Calculate burn based on Tractive Effort and Time
    void consume(double tractive_effort, double speed, double time_step) {
        // Simple Physics: Work = Force * Distance
        // Burn Rate simplified for simulation logic
        double burn_rate = (tractive_effort * speed) / (375.0 * efficiency_factor * 20.0);
        current_fuel -= (burn_rate * (time_step / 3600.0)); // Convert seconds to hours
        
        if (current_fuel < 0) current_fuel = 0;
    }

    double get_fuel_level() const { return current_fuel; }
};

// --- The Sovereign Locomotive Class ---
class Locomotive {
private:
    std::string unit_id;
    double mass; // Tons
    // Smart Pointer ownership: Locomotive "owns" the FuelSystem
    std::unique_ptr<FuelSystem> fuel_unit;

public:
    Locomotive(std::string id, double m, double fuel_cap)
        : unit_id(id), mass(m) {
        // Instantiate the fuel system using make_unique (C++14/17 standard)
        fuel_unit = std::make_unique<FuelSystem>(fuel_cap, 0.85);
    }

    void run_simulation_step(double grade, double speed, double seconds) {
        // Physics: Grade Resistance (approx 20 lbs per ton per 1% grade)
        double resistance = mass * 20.0 * grade;
        
        // Update fuel consumption
        fuel_unit->consume(resistance, speed, seconds);

        // Output to console (The Log)
        std::cout << "[" << unit_id << "] Speed: " << speed << " MPH | Fuel: "
                  << fuel_unit->get_fuel_level() << " Gal\n";
    }

    // Telemetry Export for Python Visualization
    void record_telemetry(std::ofstream& file, double time, double speed, double grade) {
        if (file.is_open()) {
            file << time << "," << speed << "," << fuel_unit->get_fuel_level() << "," << grade << "\n";
        }
    }
};

int main() {
    // Initialize the BNSF Unit
    auto bnsf_7742 = std::make_unique<Locomotive>("BNSF-7742", 200.0, 5000.0);
    
    // Prepare Telemetry File (The "Manifest")
    std::ofstream telemetry_file("bnsf_telemetry.csv");
    telemetry_file << "Time,Speed,Fuel,Grade\n"; // CSV Header

    // Simulation Loop (e.g., a 2-minute run)
    for (int t = 0; t <= 120; t += 10) {
        double current_grade = 1.5; // 1.5% incline
        double current_speed = 45.0; // Steady 45 MPH
        
        bnsf_7742->run_simulation_step(current_grade, current_speed, 10.0);
        bnsf_7742->record_telemetry(telemetry_file, t, current_speed, current_grade);
    }

    telemetry_file.close();
    std::cout << "\nSimulation Complete. Telemetry saved to bnsf_telemetry.csv\n";
    return 0;
}

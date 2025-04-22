// node_000001/scenarios/solar_wind_distortion.cpp
// Project SOLIS - Scenario 005: Solar Wind Distortion
// Simulation of charged particle interference on subsystem charge thresholds

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <string>

struct SolarWindLog {
    int cycle;
    double flux;                     // particles/cm²·s
    std::string subsystem_hit;
    double induced_charge;          // µC
    bool fallback_triggered;
    std::string response;
};

std::vector<SolarWindLog> logs;

// Subsystem charge thresholds
const double CPU_THRESHOLD      = 50.0;
const double RAM_THRESHOLD      = 30.0;
const double FLUX_PANEL_THRESH  = 80.0;
const double CAPACITOR_THRESH   = 100.0;

double shielding_efficiency = 0.25; // 25% absorption
bool fallback = false;
const int TOTAL_CYCLES = 6000;

// Subsystem list
std::vector<std::string> subsystems = {
    "CPU_LOGIC", "RAM_BUS", "FLUX_PANEL", "CAPACITOR_BANK"
};

// Returns random subsystem
std::string getRandomSubsystem() {
    return subsystems[rand() % subsystems.size()];
}

// Get threshold based on subsystem
double getThreshold(const std::string& name) {
    if (name == "CPU_LOGIC") return CPU_THRESHOLD;
    if (name == "RAM_BUS") return RAM_THRESHOLD;
    if (name == "FLUX_PANEL") return FLUX_PANEL_THRESH;
    if (name == "CAPACITOR_BANK") return CAPACITOR_THRESH;
    return 100.0;
}

// Simulate a single flare event
void simulateSolarWindDistortion(int cycle) {
    double flux = 800 + (rand() % 800);  // Solar wind flux: 800–1600 particles/cm²·s
    std::string subsystem = getRandomSubsystem();

    double base_charge = 0.08 * flux; // induced charge (simulated)
    double effective_charge = base_charge * (1.0 - shielding_efficiency);
    double threshold = getThreshold(subsystem);

    std::string response = "NORMAL";

    if (effective_charge > threshold) {
        fallback = true;
        response = "FALLBACK_TRIGGERED";
    } else if (effective_charge > 0.8 * threshold) {
        response = "WARNING_HIGH_CHARGE";
    }

    logs.push_back({
        cycle,
        flux,
        subsystem,
        effective_charge,
        fallback,
        response
    });
}

// Export to CSV
void exportLogsToCSV(const std::string& filename) {
    std::ofstream file(filename);
    file << "Cycle,Flux,Subsystem,Induced Charge (uC),Response,Fallback Triggered\n";

    for (const auto& log : logs) {
        file << log.cycle << ","
             << std::fixed << std::setprecision(2) << log.flux << ","
             << log.subsystem_hit << ","
             << log.induced_charge << ","
             << log.response << ","
             << (log.fallback_triggered ? "YES" : "NO") << "\n";
    }

    file.close();
}

// Main Simulation
int main() {
    srand(static_cast<unsigned int>(time(0)));
    std::cout << "[🌬️] Initiating Solar Wind Distortion Simulation...\n";

    for (int cycle = 1; cycle <= TOTAL_CYCLES; ++cycle) {
        if (fallback) break;

        // Simulate solar flare roughly every 1000 cycles
        if (cycle % 1000 == 0 || (rand() % 1200 == 0)) {
            simulateSolarWindDistortion(cycle);
        }
    }

    exportLogsToCSV("node_000001/logs/solar_wind_distortion.csv");

    if (fallback) {
        std::cout << "[⚠️] Fallback triggered due to solar wind overcharge.\n";
    } else {
        std::cout << "[✅] Simulation completed successfully.\n";
    }

    std::cout << "Total Flare Events Simulated: " << logs.size() << "\n";
    return 0;
}


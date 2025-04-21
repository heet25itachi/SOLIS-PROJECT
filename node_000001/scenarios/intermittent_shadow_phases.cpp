// node_000001/scenarios/intermittent_shadow_phase.cpp
// Project SOLIS -- Scenario 02: Orbital Eclipse / Shadow Phase Simulation
// 
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <iomanip>

struct ShadowLog {
    int cycle;
    double solar_flux;
    double internal_temp;
    bool eclipse_active;
};

std::vector<ShadowLog> logs;  // ✅ Fixed: lowercase

// Initial system state
double internal_temp = 24.0;
double total_energy = 0.0;
bool fallback_triggered = false;

// Simulates eclipse based on orbital cycle
bool isInEclipse(int cycle) {
    // Every 500 cycles → 100-cycle eclipse (from 400 to 499)
    return (cycle % 500 >= 400 && cycle % 500 <= 499);
}

void SimulateIntermittentShadowPhase(int total_cycles) {
    for (int i = 1; i <= total_cycles; ++i) {
        bool eclipse = isInEclipse(i);
        double flux = eclipse ? 0.0 : 1361.0;

        if (!eclipse) {
            total_energy += flux;
            internal_temp += 0.00012;   // heat gain in sunlight
        } else {
            internal_temp -= 0.0025;    // cooling during shadow
        }

        if (internal_temp < 4.0) {
            fallback_triggered = true;
            break;
        }

        logs.push_back({i, flux, internal_temp, eclipse});
    }
}

void exportLogsToCSV(const std::string& filename) {
    std::ofstream file(filename);
    file << "Cycle,Solar Flux,Internal Temp,Eclipse Active\n";

    for (const auto& log : logs) {
        file << log.cycle << ","
             << std::fixed << std::setprecision(2) << log.solar_flux << ","
             << std::fixed << std::setprecision(2) << log.internal_temp << ","
             << (log.eclipse_active ? "YES" : "NO") << "\n";
    }

    file.close();
}

int main() {
    std::cout << "[🌑] Starting Shadow Phase Simulation...\n";

    SimulateIntermittentShadowPhase(6000);
    exportLogsToCSV("node_000001/logs/intermittent_shadow_phase.csv");

    if (fallback_triggered) {
        std::cout << "[⚠️] Fallback triggered: Internal temp dropped too low.\n";
    } else {
        std::cout << "[✅] Simulation complete: No fallback required.\n";
    }

    std::cout << "Final Internal Temp: " << internal_temp << "°C\n";
    std::cout << "Total Energy Absorbed: " << total_energy << " W\n";

    return 0;
}


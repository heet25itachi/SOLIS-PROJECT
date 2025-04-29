// Project SOLIS — Scenario 10: Overload Relay Trip
// Author: Arjun (Heet Trivedi)

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<iomanip>
#include<cmath>
#include<cstdlib>
#include<ctime>

struct LoadLog {
    int cycle;
    double current_load;  // Amperes
    bool relay_tripped;
};

std::vector<LoadLog> logs;
bool fallback = false;

const int TOTAL_CYCLES = 6000;
const double NOMINAL_LOAD = 1.0;    // 1.0 Amps normal load
const double TRIP_THRESHOLD = 1.5;  // Trip if over 1.5A for too long
const double EMERGENCY_CUTOFF = 2.2; // Instant trip over 2.2A

void simulateOverloadRelay() {
    for (int cycle = 1; cycle <= TOTAL_CYCLES; ++cycle) {
        if (fallback) break;

        // Simulate random fluctuations
        double fluctuation = ((rand() % 200) - 100) / 1000.0;  // ±0.1 random
        double current_load = NOMINAL_LOAD + fluctuation;

        // Inject random high spikes
        if (cycle % 800 == 0 || (rand() % 1000 < 2)) {
            current_load += 0.8 + ((rand() % 400) / 1000.0);  // sudden surge
        }

        bool tripped = false;

        if (current_load >= EMERGENCY_CUTOFF) {
            tripped = true;
            fallback = true;
        }
        else if (current_load >= TRIP_THRESHOLD && rand() % 100 < 10) {
            // chance to trip if sustained moderate overload
            tripped = true;
            fallback = true;
        }

        logs.push_back({cycle, current_load, tripped});
    }
}

void exportLogsToCSV(const std::string& filename) {
    std::ofstream file(filename);
    file << "Cycle,Current_Load_Ampere,Relay_Tripped\n";
    for (const auto& log : logs) {
        file << log.cycle << ","
             << std::fixed << std::setprecision(3) << log.current_load << ","
             << (log.relay_tripped ? "YES" : "NO") << "\n";
    }
    file.close();
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    std::cout << "[🔌] Starting Overload Relay Trip Simulation...\n";

    simulateOverloadRelay();
    exportLogsToCSV("node_000001/logs/overload_relay_trip.csv");

    if (fallback) {
        std::cout << "[⚠️] Fallback triggered due to overload trip.\n";
    } else {
        std::cout << "[✅] Simulation completed successfully with no trip.\n";
    }

    std::cout << "Total Events Logged: " << logs.size() << "\n";
    return 0;
}


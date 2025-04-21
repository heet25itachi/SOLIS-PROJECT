
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>

struct ImpactLog {
    int cycle;
    std::string subsystem_hit;
    double dsi;
    std::string response;
    bool fallback_triggered;
};

std::vector<ImpactLog> logs;
bool fallback = false;
const int TOTAL_CYCLES = 6000;

std::string getRandomSubSystem() {
    const std::vector<std::string> subsystems = {
        "thermal_radiator",
        "flux_intake_panel",
        "capacitor_array",
        "core_memory",
        "CPU_control_cluster",
        "shield_layer"
    };

    return subsystems[rand() % subsystems.size()];
}

void simulateMicroMeteoriteImpact(int cycle) {
    std::string Subsystem = getRandomSubSystem();
    double dsi = 0.1 + static_cast<double>(rand()) / RAND_MAX * 1.5;
    std::string response;

    if (dsi < 0.5) {
        response = "LOGGED";
    } else if (dsi < 1.2) {
        response = "ISOLATED";
    } else {
        response = "FALLBACK";
        fallback = true;
    }

    logs.push_back({cycle, Subsystem, dsi, response, fallback});
}

void exportLogsToCSV(const std::string& filename) {
    std::ofstream file(filename);
    file << "Cycle,Subsystem Hit,DSI,Response,Fallback Triggered\n";

    for (const auto& log : logs) {
        file << log.cycle << ","
             << log.subsystem_hit << ","
             << std::fixed << std::setprecision(3) << log.dsi << ","
             << log.response << ","
             << (log.fallback_triggered ? "YES" : "NO") << "\n";
    }

    file.close();
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    std::cout << "[☄️] Initiating Micro-Meteorite Contact Simulation...\n";

    for (int cycle = 1; cycle <= TOTAL_CYCLES; ++cycle) {
        if (fallback) break;

        // Random chance of impact every 1000 cycles or randomly
        if (cycle % 1000 == 0 || (rand() % 1200 == 0)) {
            simulateMicroMeteoriteImpact(cycle);
        }
    }

    exportLogsToCSV("node_000001/logs/micro_meteorite_contact.csv");

    if (fallback) {
        std::cout << "[⚠️] Fallback triggered due to critical damage.\n";
    } else {
        std::cout << "[✅] Simulation complete. No fallback required.\n";
    }

    std::cout << "Total Impact Events Logged: " << logs.size() << "\n";
    return 0;
}

// node_000001/scenarios/high_intensity_solar_surge.cpp
// Scenario Simulation: High-Intensity Solar Surge 
// Part of the Project SOLIS - DysonNode Simulation Stack

#include<iostream>
#include<vector>
#include<cmath>
#include<fstream>
#include<iomanip>

struct CycleLog {
    int id; 
    double flux;
    double internal_temp;
    bool fallback_triggered;
};

std::vector<CycleLog> logs;

double total_energy = 0.0;
double internal_temp = 24.0;   // Initial Safe Temperature
bool Fallback = false;

void simulateHighIntensitySolarSurge(int Cycles) {
    for(int i = 1; i <= Cycles; ++i) {
        if(Fallback) break;

        double surge_flux = 1361.0 + std::sin(i) * 2.0;    // Simulated Solar Surge Pattern
        total_energy +=  surge_flux;    

        // Simulated internal thermal rise 
        internal_temp += 0.0015 + (i % 10) * 0.0001;

        // Trigger Fallback if system overheats 
        if(internal_temp > 78.5) {
            Fallback = true;
        }

        logs.push_back({i, surge_flux, internal_temp, Fallback});
    }
}

void exportLogsToCSV(const std::string& filename) {
    std::ofstream file(filename);
    file << "Cycle,Flux (W/m^2),Internal Temp (°C),Fallback Triggered\n";

    for(const auto& log : logs) {
        file << log.id << ","
             << std::fixed << std::setprecision(2) << log.flux << ","
             << std::fixed << std::setprecision(4) << log.internal_temp << ","
             << (log.fallback_triggered ? "YES" : "NO") << "\n";
    }

    file.close();
}

int main() {
    std::cout <<  "[🌞] Starting High-Intensity Solar Surge Simulation...\n";
    simulateHighIntensitySolarSurge(5970);

    exportLogsToCSV("node_000001/logs/high_intensity_surge.csv");

    if (Fallback) {
        std::cout << "[⚠️] Fallback Triggered! Internal temperature exceeded safe limit.\n";
    } else {
        std::cout << "[✅] Simulation completed without triggering fallback.\n";
    }

    std::cout << "Final Temperature: " << internal_temp << "°C\n";
    std::cout << "Total Energy Absorbed: " << total_energy << " W\n";

    return 0;
}


// node_000001/scenarios/rapid_temperture_drop
// Project SOLIS -- Scenario 04: Rapid Temperature Drop
// SImulation of sudden thermal failure in orbital node

#include<iostream>
#include<fstream>
#include<vector>
#include<cstdlib>
#include<iomanip>
#include<ctime>

// Log structure 
struct TempLog {
    int cycle;
    double internal_temp;
    double drop_rate;
    bool fallback_trigger;
};

// Global variable
std::vector<TempLog> logs;
double internal_temp = 24.0;
bool fallback = false;
const int TOTAL_CYCLES = 6000;
const double CRITICAL_TEMP = -5.0;

// Simulate thermal decay across all cycles 
void simulateRapidTempDrop(int total_cycles) {
    for(int i = 1; i <= total_cycles; ++i) {
        if(fallback) break;
    

    // Random drop rate between 0.1℃ and 0.6℃
    double drop_rate = 0.1 + static_cast<double>(rand()) / RAND_MAX * 0.5;
    internal_temp -= drop_rate;

    if(internal_temp < CRITICAL_TEMP) {
        fallback  = true;
    }

    logs.push_back({i, internal_temp, drop_rate, fallback});
    }
}

// Export To CSV
void exportLogsToCSV(const std::string& filename) {
     std::ofstream file(filename);
     file << "Cycle, Internal Temp(C), Drop Rate, Fallback Triggered\n";

     for(const auto& log : logs) {
        file << log.cycle<< ","
        << std::fixed<< std::setprecision(2)
        << log.internal_temp << "," << log.drop_rate << ","
        << (log.fallback_trigger ? "YES" : "NO")<< "\n";
    }

    file.close();
} 

// Entry point
int main() {
    srand(static_cast<unsigned int>(time(0)));

    std::cout<<"[🌡️] Starting Rapid Temperature Drop Simulation...\n";

    simulateRapidTempDrop(TOTAL_CYCLES);
    exportLogsToCSV("node_000001/logs/rapid_temp_drop.csv");

    if (fallback) {
        std::cout << "[⚠️] Fallback triggered: Internal temp dropped below critical.\n";
    } else {
        std::cout << "[✅] Simulation complete. No fallback required.\n";
    }

    std::cout << "Final Internal Temp: " << internal_temp << "°C\n";
    std::cout << "Total Cycles Processed: " << logs.size() << "\n";

    return 0;
}


#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<iomanip>
#include<cstdlib>
#include<ctime>

struct MemoryBlock {
    int cycle;
    double corruption_fraction;   // Percentage of corrupted memory
    bool fallback_triggered;
};

std::vector<MemoryBlock> logs;
bool fallback = false;
const int TOTAL_CYCLES =  6000;
const double LEAK_RATE = 0.00001;    // Leak probability per block per cycle
const double CRITICAL_THRESHOLD = 0.05;   // 5% corrruption triggers fallback
double corrupted_fraction = 0.0;


void simulateMemoryLeak() {
    for(int cycle = 1; cycle <= TOTAL_CYCLES; ++cycle) {
        if(fallback) break;

        double leak_this_cycle = LEAK_RATE * (1.0 - corrupted_fraction);
        corrupted_fraction += leak_this_cycle;

        if(corrupted_fraction >= CRITICAL_THRESHOLD) {
            fallback  = true;
        }

        logs.push_back({cycle, corrupted_fraction, fallback});
    }
}

void exportLogsToCSV(const std::string& filename) {
    std::ofstream file(filename);
    file << "Cycle, Corruption_Fraction, Fallback_Triggered\n";
    for(const auto& log : logs) {
        file << log.cycle << "," <<  std::setprecision(6) << log.corruption_fraction << "," << (log.fallback_triggered ? "YES" : "NO") << "\n";
    }
    file.close();
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    std::cout << "[💾] Starting Internal Memory Leak Simulation...\n";

    simulateMemoryLeak();
    exportLogsToCSV("node_000001/logs/internal_memory_leak.csv");

    if (fallback) {
        std::cout << "[⚠️] Fallback triggered due to critical memory corruption.\n";
    } else {
        std::cout << "[✅] Simulation complete without fallback.\n";
    }

    std::cout << "Final Corrupted Memory Fraction: " << corrupted_fraction * 100 << "%\n";
    return 0;
}

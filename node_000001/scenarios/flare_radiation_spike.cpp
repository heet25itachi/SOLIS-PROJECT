#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iomanip>

struct FlareLog {
    int cycle;
    double dose;  // in Krad
    std::string subsystem;
    std::string action;
    bool fallback_triggered;
};

std::vector<FlareLog> logs;
bool fallback = false;

const int TOTAL_CYCLES = 6000;
const std::vector<std::string> subsystems = {
    "CPU_Core", 
    "RAM_Array",
    "Comm_Module",
    "Thermal_Sensor", 
    "Power_Regulator",
    "Sensor_Hub"
};

double getRandomDose(bool isFlare) {
    if (isFlare) {
        return 20.0 + static_cast<double>(rand() % 1000) / 10.0; // 20 - 120 Krad
    }
    return 0.1 + static_cast<double>(rand() % 50) / 10.0; // 0.1 - 5 Krad
}

std::string evaluateSubSystem(double dose, const std::string& name) {
    if (name == "RAM_Array" && dose > 20.0) return "ECC_CORRECTED";
    if (name == "CPU_Core" && dose > 45.0) return "SHUTDOWN_INIT"; // fixed typo here
    if (name == "Comm_Module" && dose > 25.0) return "REBOOT_RADIO";
    if (name == "Thermal_Sensor" && dose > 15.0) return "TEMP_SENSOR_FAIL";
    if (name == "Power_Regulator" && dose > 30.0) return "VOLTAGE_CLAMP";
    if (name == "Sensor_Hub" && dose > 35.0) return "ISOLATED_HUB";

    return "NORMAL_OPERATION";
}

void simulateRadiationSpikes() {
    for (int cycle = 1; cycle <= TOTAL_CYCLES; ++cycle) {
        if (fallback) break;

        bool isFlare = (cycle % 600 == 0 || rand() % 1000 < 5);
        double dose = getRandomDose(isFlare);
        
        // FIXED: Don't shadow the global 'subsystems'
        std::string subsystem = subsystems[rand() % subsystems.size()];
        
        std::string response = evaluateSubSystem(dose, subsystem); // fixed function name

        if (response == "SHUTDOWN_INIT" || dose > 100.0) {
            fallback = true;
        }

        logs.push_back({cycle, dose, subsystem, response, fallback});
    }
}

void exportLogsToCSV(const std::string& filename) {
    std::ofstream file(filename);
    file << "Cycle,Dose(Krad),Subsystem,Action,Fallback Triggered\n";

    for (const auto& log : logs) {
        file << log.cycle << ","
             << std::fixed << std::setprecision(2) << log.dose << ","
             << log.subsystem << ","
             << log.action << ","
             << (log.fallback_triggered ? "YES" : "NO") << "\n";
    }

    file.close();
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    std::cout << "[☢️] Initiating Radiation Spike Simulation (Scenario 007)...\n";
    simulateRadiationSpikes();
    exportLogsToCSV("node_000001/logs/radiation_spikes.csv");

    if (fallback) {
        std::cout << "[⚠️] Fallback triggered due to critical radiation exposure.\n";
    } else {
        std::cout << "[✅] Simulation completed successfully with no fallback.\n";
    }

    std::cout << "Total Events Logged: " << logs.size() << "\n";
    return 0;
}


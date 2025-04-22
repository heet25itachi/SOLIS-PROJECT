// node_000001/scenarios/geo_alignment_drift.cpp
// Project SOLIS – Scenario 006: Geo-Alignment Drift
// Simulates orbital drift, deviation angle, and fallback correction

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

struct DriftLog {
    int cycle;
    double drift_angle_deg;
    double correction_torque;
    bool fallback_triggered;
    std::string status;
};

std::vector<DriftLog> logs;

// System constants
const double DEG_TO_RAD = 3.14159265 / 180.0;
const double WARNING_ANGLE = 0.8;   // in degrees
const double CRITICAL_ANGLE = 3.0;  // fallback threshold
const int TOTAL_CYCLES = 6000;

bool fallback = false;
double current_drift_angle = 0.0;

// Simulate external factors causing orientation drift
double generateDriftDelta() {
    return ((rand() % 100) / 100.0 - 0.5) * 0.15;  // random ±0.075 deg
}

// Correction torque model
double computeCorrectionTorque(double angle) {
    // Simulated torque needed (in mNm) proportional to drift angle
    return std::abs(angle) * 0.9;
}

// Main simulation function
void simulateGeoAlignmentDrift() {
    for (int i = 1; i <= TOTAL_CYCLES; ++i) {
        if (fallback) break;

        // Simulate drift injection
        current_drift_angle += generateDriftDelta();

        std::string status = "STABLE";
        double correction = 0.0;

        if (std::abs(current_drift_angle) > CRITICAL_ANGLE) {
            fallback = true;
            status = "FALLBACK_TRIGGERED";
            correction = computeCorrectionTorque(current_drift_angle);
        } else if (std::abs(current_drift_angle) > WARNING_ANGLE) {
            status = "CORRECTING";
            correction = computeCorrectionTorque(current_drift_angle);
            current_drift_angle -= correction * 0.05;  // simulated correction
        }

        logs.push_back({i, current_drift_angle, correction, fallback, status});
    }
}

// Export logs
void exportLogsToCSV(const std::string& filename) {
    std::ofstream file(filename);
    file << "Cycle,Drift Angle (deg),Correction Torque (mNm),Status,Fallback Triggered\n";

    for (const auto& log : logs) {
        file << log.cycle << ","
             << std::fixed << std::setprecision(4) << log.drift_angle_deg << ","
             << log.correction_torque << ","
             << log.status << ","
             << (log.fallback_triggered ? "YES" : "NO") << "\n";
    }

    file.close();
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    std::cout << "[📡] Starting Geo-Alignment Drift Simulation...\n";
    simulateGeoAlignmentDrift();
    exportLogsToCSV("node_000001/logs/geo_alignment_drift.csv");

    if (fallback) {
        std::cout << "[⚠️] Fallback triggered: Drift angle exceeded safe bounds.\n";
    } else {
        std::cout << "[✅] Simulation completed: No fallback required.\n";
    }

    std::cout << "Final Drift Angle: " << current_drift_angle << "°\n";
    std::cout << "Cycles Processed: " << logs.size() << "\n";

    return 0;
}

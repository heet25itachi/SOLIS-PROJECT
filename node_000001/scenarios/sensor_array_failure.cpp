#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <map>
#include <numeric>
#include <cmath>

struct SensorLog {
    int cycle;
    std::string sensor;
    std::string status;
    double confidence;
    std::string action;
};

std::vector<SensorLog> logs;
bool fallback = false;
const int TOTAL_CYCLES = 6000;
const double CONFIDENCE_THRESHOLD = 0.6;

std::vector<std::string> sensor_names = {  // FIXED spelling
    "TEMP", "RAD", "MAG",
    "ACC", "GYRO", "SOLR",
    "IR", "UV", "VISL"
};

// For each sensor, store last 10 readings
std::map<std::string, std::vector<double>> sensor_history;

// Generate fake sensor data with small noise or drift
double generateSensorReading(const std::string& sensor, int cycle) {
    double base = 1.0 + (rand() % 100) / 100.0; // Base 1.0 - 2.0
    if (sensor == "RAD" && (cycle % 700 == 0)) return base + 2.5;
    if (sensor == "GYRO" && (rand() % 1000) < 2) return base + 5.0;
    if (sensor == "SOLR" && cycle % 1200 < 40) return 0.0;
    return base + ((rand() % 20) - 10) / 100.0; // ±0.1 noise
}

double calculateVariance(const std::vector<double>& values) {
    if (values.size() < 2) return 0.0;
    double mean = std::accumulate(values.begin(), values.end(), 0.0) / values.size();
    double var = 0.0;
    for (double v : values) var += (v - mean) * (v - mean);
    return var / values.size();
}

double computeConfidence(const std::string& sensor) {
    const auto& history = sensor_history[sensor];
    if (history.size() < 2) return 1.0;
    double variance = calculateVariance(history);
    double confidence = 1.0 / (variance + 0.01); // avoid div by zero
    if (confidence > 1.0) confidence = 1.0;
    return confidence;
}

void updateSensorHistory(const std::string& sensor, double value) {
    sensor_history[sensor].push_back(value);
    if (sensor_history[sensor].size() > 10) {
        sensor_history[sensor].erase(sensor_history[sensor].begin());
    }
}

void simulateSensorArrayFailure() {
    int failed_sensors = 0;

    for (int cycle = 1; cycle <= TOTAL_CYCLES; ++cycle) {
        if (fallback) break;

        for (const auto& sensor : sensor_names) {
            double value = generateSensorReading(sensor, cycle);
            updateSensorHistory(sensor, value);

            double confidence = computeConfidence(sensor);
            std::string status = "OK";
            std::string action = "POLLING_CONTINUES";

            if (confidence < CONFIDENCE_THRESHOLD) {
                status = "UNRELIABLE";
                action = "ISOLATED";
                failed_sensors++;
            }

            if (confidence < 0.3) {
                status = "FAIL";
                action = "BACKUP_ACTIVATED";  // FIXED typo
                failed_sensors += 2;
            }

            if (failed_sensors >= 6) {
                fallback = true;
            }

            logs.push_back({cycle, sensor, status, confidence, action});
        }
    }
}

void exportLogsToCSV(const std::string& filename) {
    std::ofstream file(filename);
    file << "Cycle,Sensor,Status,Confidence,Action\n";

    for (const auto& log : logs) {
        file << log.cycle << ","
             << log.sensor << ","
             << log.status << ","
             << std::fixed << std::setprecision(2) << log.confidence << ","
             << log.action << "\n";
    }

    file.close();
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    std::cout << "[🧠] Starting Sensor Array Failure Simulation...\n";

    simulateSensorArrayFailure();
    exportLogsToCSV("node_000001/logs/sensor_array_failure.csv");

    if (fallback) {
        std::cout << "[⚠️] Fallback triggered due to multi-sensor failure.\n";
    } else {
        std::cout << "[✅] All sensors within operational trust limits.\n";
    }

    std::cout << "Total Events Logged: " << logs.size() << "\n";
    return 0;
}


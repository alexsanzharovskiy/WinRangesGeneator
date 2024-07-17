#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <unistd.h>
#include <limits.h>

class WinRangesGenerator {
public:
    static const std::string PAYOUT;
    static const std::string RANGES;
    static const std::string JSON_FILE_NAME;

private:
    float start;
    float end;
    std::map<int, float> rangeSteps;
    int exactStepLimitValue;
    float minStep;
    int maxWin;

    std::string savePath;

    std::vector<std::string> generatedRanges;

public:
    WinRangesGenerator(int maxWin, float minStep, int exactStepLimitValue, const std::map<int, float>& rangeSteps, const std::string& exePath)
        : start(0.00), end(0.00), maxWin(maxWin), minStep(minStep), exactStepLimitValue(exactStepLimitValue), rangeSteps(rangeSteps) {
            savePath = exePath + "/storage/" + JSON_FILE_NAME;
            createStorageDirectory(exePath + "/storage");
        }

    bool generateJsonFile() {
        if (generatedRanges.empty()) {
            generate();
        }

        std::ofstream file(savePath);
        if (!file.is_open()) {
            return false;
        }

        std::ostringstream jsonStream;
        jsonStream << "[";
        for (size_t i = 0; i < generatedRanges.size(); ++i) {
            jsonStream << generatedRanges[i];
            if (i < generatedRanges.size() - 1) {
                jsonStream << ",";
            }
        }
        jsonStream << "]";

        file << jsonStream.str();
        file.close();

        return true;
    }

    std::string getSavePath() const {
        return savePath;
    }

private:
    void generate() {
        generatedRanges.push_back("{\"payout\":0,\"ranges\":[0,0]}");

        std::vector<int> keys;
        for (const auto& pair : rangeSteps) {
            keys.push_back(pair.first);
        }

        int currentStepIndex = 0;
        while (end < maxWin) {
            if (end < exactStepLimitValue) {
                start += minStep;
                end += minStep;
            } else {
                if (currentStepIndex < keys.size() - 1 && end >= keys[currentStepIndex + 1]) {
                    currentStepIndex++;
                }
                start = end;
                end += rangeSteps[keys[currentStepIndex]];
            }

            std::ostringstream rangeStream;
            rangeStream << std::fixed << std::setprecision(2);
            std::string startStr = formatFloat(start);
            std::string endStr = formatFloat(end);
            std::string payoutStr = formatFloat(start < exactStepLimitValue ? start : start + (end - start) / 2);
            rangeStream << "{\"payout\":" << payoutStr
                        << ",\"ranges\":[" << startStr << "," << endStr << "]}";
            generatedRanges.push_back(rangeStream.str());
        }

        std::ostringstream finalRangeStream;
        finalRangeStream << std::fixed << std::setprecision(2);
        std::string finalMaxWinStr = formatFloat(maxWin);
        finalRangeStream << "{\"payout\":" << finalMaxWinStr << ",\"ranges\":[" << finalMaxWinStr << "," << finalMaxWinStr << "]}";
        generatedRanges.push_back(finalRangeStream.str());
    }

    std::string formatFloat(float value) {
        if (value == static_cast<int>(value)) {
            return std::to_string(static_cast<int>(value));
        } else {
            std::ostringstream out;
            out << std::fixed << std::setprecision(2) << value;
            return out.str();
        }
    }

    void createStorageDirectory(const std::string& path) {
        std::filesystem::create_directories(path);
    }
};

const std::string WinRangesGenerator::PAYOUT = "payout";
const std::string WinRangesGenerator::RANGES = "ranges";
const std::string WinRangesGenerator::JSON_FILE_NAME = "win_ranges.json";

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " maxWin minStep exactStepLimitValue rangeSteps..." << std::endl;
        return 1;
    }

    // Получаем путь к исполняемому файлу
    std::string exePath(argv[0]);
    exePath = exePath.substr(0, exePath.find_last_of("/"));

    int maxWin = std::stoi(argv[1]);
    float minStep = std::stof(argv[2]);
    int exactStepLimitValue = std::stoi(argv[3]);

    std::map<int, float> rangeSteps;
    std::string stepsStr = argv[4];
    std::istringstream stepsStream(stepsStr);
    std::string step;
    while (std::getline(stepsStream, step, ';')) {
        std::istringstream stepStream(step);
        std::string keyStr, valueStr;
        if (std::getline(stepStream, keyStr, ':') && std::getline(stepStream, valueStr, ':')) {
            int key = std::stoi(keyStr);
            float value = std::stof(valueStr);
            rangeSteps[key] = value;
        }
    }

    WinRangesGenerator generator(maxWin, minStep, exactStepLimitValue, rangeSteps, exePath);

    if (generator.generateJsonFile()) {
        std::cout << "JSON file successfully created: " << generator.getSavePath() << std::endl;
    } else {
        std::cerr << "Error creating JSON file" << std::endl;
        return 1;
    }

    return 0;
}

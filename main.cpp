#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

struct XorShift64Star {
    uint64_t state;

    explicit XorShift64Star(uint64_t seed) : state(seed == 0 ? 1469598103934665603ull : seed) {}

    uint64_t next() {
        state ^= state >> 12;
        state ^= state << 25;
        state ^= state >> 27;
        return state * 2685821657736338717ull;
    }

    double nextDouble() {
        return static_cast<double>(next() >> 11) * (1.0 / 9007199254740992.0);
    }
};

uint64_t countInsideCircle(uint64_t points, uint64_t seed) {
    XorShift64Star rng(seed);
    uint64_t inside = 0;

    for (uint64_t i = 0; i < points; ++i) {
        double x = rng.nextDouble();
        double y = rng.nextDouble();
        if (x * x + y * y <= 1.0) {
            ++inside;
        }
    }

    return inside;
}

double runParallel(uint64_t n, int threadsCount) {
    std::vector<std::thread> threads;
    std::vector<uint64_t> localInside(static_cast<size_t>(threadsCount), 0);

    uint64_t baseBatch = n / static_cast<uint64_t>(threadsCount);
    uint64_t remainder = n % static_cast<uint64_t>(threadsCount);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < threadsCount; ++i) {
        uint64_t points = baseBatch + (static_cast<uint64_t>(i) < remainder ? 1ull : 0ull);
        uint64_t seed = 123456789ull + 1000003ull * static_cast<uint64_t>(i + 1) + n;
        threads.emplace_back([&, i, points, seed]() {
            localInside[static_cast<size_t>(i)] = countInsideCircle(points, seed);
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto finish = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(finish - start).count();
}

std::vector<uint64_t> getNValues(bool fullMode) {
    if (fullMode) {
        return {
            1000000ull,
            10000000ull,
            100000000ull,
            1000000000ull,
            10000000000ull,
            100000000000ull
        };
    }

    return {100000ull, 500000ull, 1000000ull};
}

std::vector<int> getThreadValues(bool fullMode) {
    if (fullMode) {
        return {1, 2, 4, 8, 16, 32, 64, 128};
    }

    return {1, 2, 4};
}

int main(int argc, char* argv[]) {
    bool fullMode = false;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--full") {
            fullMode = true;
        }
    }

    auto nValues = getNValues(fullMode);
    auto threadValues = getThreadValues(fullMode);

    std::filesystem::create_directories("results");
    std::ofstream csv("results/pi_monte_carlo_parallel_results.csv");
    if (!csv) {
        std::cerr << "Cannot open results/pi_monte_carlo_parallel_results.csv\n";
        return 1;
    }

    csv << "N";
    for (int m : threadValues) {
        csv << ",M_" << m;
    }
    csv << '\n';

    csv << std::setprecision(10);
    std::cout << std::setprecision(10);

    for (uint64_t n : nValues) {
        csv << n;
        for (int m : threadValues) {
            double seconds = runParallel(n, m);
            csv << ',' << seconds;
            std::cout << "N=" << n << " M=" << m << " time=" << seconds << " sec\n";
        }
        csv << '\n';
    }

    return 0;
}

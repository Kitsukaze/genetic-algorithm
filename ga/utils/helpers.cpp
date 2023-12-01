#pragma once

#include <chrono>
#include <random>
#include <string>

template <typename... Args>
std::string format(const std::string &format, Args... args)
{
    // https://stackoverflow.com/a/26221725
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
    if (size_s <= 0)
    {
        throw std::runtime_error("Error during formatting.");
    }
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

double random(double l, double r)
{
    std::uniform_real_distribution<double> unif(l, r);
    return unif(rng);
}

#pragma once

#include <vector>
#include <set>
#include <stdexcept>

#include <pybind11/stl.h>

#include "helpers.cpp"
#include "maximum_flow.cpp"
#include "maximum_weighted_flow.cpp"

void check_constraints(
    unsigned size,
    std::vector<std::vector<double>> &demands,
    std::vector<std::vector<double>> &capacities,
    std::vector<std::set<unsigned>> &neighbors,
    std::vector<std::vector<double>> &flow_weights,
    unsigned source,
    unsigned sink)
{
    check_constraints(size, capacities, neighbors, flow_weights, source, sink);
    for (unsigned i = 0; i < size; i++)
    {
        for (unsigned j = 0; j < size; j++)
        {
            if (demands[i][j] > capacities[i][j])
            {
                throw std::invalid_argument(format(
                    "At flow from %d to %d: demand = %lf > capacity = %lf",
                    i, j, demands[i][j], capacities[i][j]));
            }
        }
    }
}

std::optional<std::pair<double, std::vector<std::vector<double>>>> weighted_flows_with_demands(
    unsigned size,
    std::vector<std::vector<double>> &demands,
    std::vector<std::vector<double>> &capacities,
    std::vector<std::set<unsigned>> &neighbors,
    std::vector<std::vector<double>> &flow_weights,
    unsigned source,
    unsigned sink)
{
    check_constraints(size, demands, capacities, neighbors, flow_weights, source, sink);

    std::vector<double> demands_in(size), demands_out(size);
    for (unsigned i = 0; i < size; i++)
    {
        for (auto j : neighbors[i])
        {
            demands_in[j] += demands[i][j];
            demands_out[i] += demands[i][j];
        }
    }

    std::vector<std::vector<double>> new_capacities(size + 2, std::vector<double>(size + 2));
    std::vector<std::set<unsigned>> new_neighbors(size + 2);
    for (unsigned i = 0; i < size; i++)
    {
        new_capacities[size][i] = demands_in[i];
        new_neighbors[size].insert(i);

        new_capacities[i][size + 1] = demands_out[i];
        new_neighbors[i].insert(size + 1);

        for (auto j : neighbors[i])
        {
            new_capacities[i][j] = capacities[i][j] - demands[i][j];
            new_neighbors[i].insert(j);
        }
    }

    double capacity_sum = 0.0;
    for (unsigned i = 0; i < size; i++)
    {
        for (auto j : neighbors[i])
        {
            capacity_sum += capacities[i][j];
        }
    }

    new_capacities[sink][source] = capacity_sum + 1;
    new_neighbors[sink].insert(source);

    auto satisfy_demands = maximum_flow_no_checking(size + 2, new_capacities, new_neighbors, size, size + 1);
    for (auto i : new_neighbors[size])
    {
        if (satisfy_demands.second[size][i] < new_capacities[size][i])
        {
            return std::optional<std::pair<double, std::vector<std::vector<double>>>>();
        }
    }

    for (unsigned i = 0; i < size; i++)
    {
        for (auto j : neighbors[i])
        {
            satisfy_demands.second[i][j] += demands[i][j]; // add flows from s'-v and u-t' back to u-v
            capacities[i][j] -= satisfy_demands.second[i][j];
        }
    }

    auto extend = maximum_weighted_flow_no_checking(size, capacities, neighbors, flow_weights, source, sink);

    double result = 0.0;
    std::vector<std::vector<double>> flow(size, std::vector<double>(size));
    for (unsigned i = 0; i < size; i++)
    {
        for (auto j : neighbors[i])
        {
            flow[i][j] = satisfy_demands.second[i][j] + extend.second[i][j];
            result += flow[i][j] * flow_weights[i][j];
        }
    }

    return std::pair<double, std::vector<std::vector<double>>>{result, flow};
}

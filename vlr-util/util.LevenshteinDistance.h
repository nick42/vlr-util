#pragma once

// Adapted from: https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C++

#include <algorithm>
#include <functional>
#include <vector>

#include "config.h"

namespace vlr {

template <typename T, typename TCostType = size_t, typename TCharType = typename T::value_type>
TCostType GeneralizedLevenshteinDistanceCustomCost(
    const T& source,
    const T& target,
    TCostType insert_cost,
    TCostType delete_cost,
    const std::function<TCostType(TCharType cSource, TCharType cTarget)>& getDeltaCost)
{
    if (source.size() > target.size()) {
        return GeneralizedLevenshteinDistanceCustomCost(target, source, delete_cost, insert_cost, getDeltaCost);
    }

    using TSizeType = typename T::size_type;
    const TSizeType min_size = source.size(), max_size = target.size();
    std::vector<TCostType> lev_dist(min_size + 1);

    lev_dist[0] = 0;
    for (TSizeType i = 1; i <= min_size; ++i) {
        lev_dist[i] = lev_dist[i - 1] + delete_cost;
    }

    for (TSizeType j = 1; j <= max_size; ++j) {
        TCostType previous_diagonal = lev_dist[0], previous_diagonal_save;
        lev_dist[0] += insert_cost;

        for (TSizeType i = 1; i <= min_size; ++i) {
            previous_diagonal_save = lev_dist[i];
            auto delta_cost = getDeltaCost(source[i - 1], target[j - 1]);
            if (delta_cost == 0) {
                lev_dist[i] = previous_diagonal;
            }
            else {
                lev_dist[i] = std::min(std::min(lev_dist[i - 1] + delete_cost, lev_dist[i] + insert_cost), previous_diagonal + delta_cost);
            }
            previous_diagonal = previous_diagonal_save;
        }
    }

    return lev_dist[min_size];
}

template <typename T, typename TCostType = size_t, typename TCharType = typename T::value_type>
TCostType GeneralizedLevenshteinDistance(
    const T& source,
    const T& target,
    TCostType insert_cost = 1,
    TCostType delete_cost = 1,
    TCostType replace_cost = 1)
{
    auto fGetDeltaCost = [=](TCharType cSource, TCharType cTarget)
    {
        return (cSource == cTarget) ? 0 : replace_cost;
    };
    return GeneralizedLevenshteinDistanceCustomCost<T, TCostType, TCharType>(
        source,
        target, 
        insert_cost, 
        delete_cost, 
        fGetDeltaCost);
}

} // namespace vlr

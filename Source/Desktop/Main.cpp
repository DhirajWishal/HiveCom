#include <iostream>

#include "Simulator/NetworkGrid.hpp"

int main()
{
    auto grid = HiveCom::NetworkGrid({
        HiveCom::Connection("A", "BDNO"),
        HiveCom::Connection("B", "ACFO"),
        HiveCom::Connection("C", "B"),
        HiveCom::Connection("D", "AE"),
        HiveCom::Connection("E", "DFP"),
        HiveCom::Connection("F", "BFG"),
        HiveCom::Connection("G", "FHI"),
        HiveCom::Connection("H", "GI"),
        HiveCom::Connection("I", "GHJKL"),
        HiveCom::Connection("J", "IK"),
        HiveCom::Connection("K", "JLM"),
        HiveCom::Connection("L", "IK"),
        HiveCom::Connection("M", "K"),
        HiveCom::Connection("N", "AO"),
        HiveCom::Connection("O", "ABN"),
        HiveCom::Connection("P", "E"),
    });

    const auto message = HiveCom::Message("N", "M", "Hello world");

    grid.sendMessage(message);
}

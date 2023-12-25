#pragma once

#include "Node.hpp"

#include <random>

namespace HiveCom
{
    /// @brief Randomize router node class.
    /// This routes the incoming packet to a random child node if the current node
    /// is not the intended recipient or if the destination node is not in the child
    /// list.
    class RandomizedRouterNode final : public Node
    {
      public:
        using Node::Node;

      private:
        /// @brief Route method.
        /// This method will send the incoming message to a random connection.
        /// @param message The message to route.
        void route(const MessagePtr &message) override;

      private:
        std::default_random_engine m_engine;
    };
} // namespace HiveCom

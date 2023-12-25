#include "NetworkGrid.hpp"

#include <cassert>
#include <string_view>

namespace HiveCom {
Connection::Connection(const std::string &source, std::string_view connections)
    : m_source(source) {
  for (const auto character : connections) {
    std::string string;
    string.push_back(character);
    m_connections.emplace_back(std::move(string));
  }
}

void NetworkGrid::sendMessage(const MessagePtr &message, std::string_view hop) {
  assert(!m_nodeMap.empty());
  m_nodeMap[hop.data()]->sendMessage(message);
}
} // namespace HiveCom

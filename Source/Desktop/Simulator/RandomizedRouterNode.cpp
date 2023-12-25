#include "RandomizedRouterNode.hpp"
#include "NetworkGrid.hpp"

namespace HiveCom {
void RandomizedRouterNode::route(const MessagePtr &message) {
  const auto index = m_engine() % m_connections.size();
  m_pNetworkGrid->sendMessage(message, m_connections[index]);
}
} // namespace HiveCom

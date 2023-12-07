#pragma once

#include <string>

/// @brief Macro to quickly convert serial number literals to IDs.
#define GENERATE_ID(serial_number)	#serial_number

namespace HiveCom
{
	/// @brief Message structure.
	/// This contains information about the source, destination and the information that is being
	/// communicated.
	struct Message final
	{
		std::string m_source;
		std::string m_destination;
		std::string m_message;
	};

	/// @brief Node class.
	/// This class contains information about a single node in a communications network.
	class Node final
	{
	public:
		/// @brief Explicit constructor.
		/// @param identifier The node identifier.
		explicit Node(std::string_view identifier) : m_identifier(identifier.data()) {}

		/// @brief Get the node identifier.
		/// @return The identifier string view.
		[[nodiscard]] std::string_view getIdentifier() const { return m_identifier; }

	private:
		std::string m_identifier;
	};
}

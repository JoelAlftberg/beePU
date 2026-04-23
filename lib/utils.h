#pragma once

#include <functional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace utils
{

/**
 * @brief Splits a string into tokens on whitespace
 * 
 * @param[in] line The line to split into tokens
 * 
 * @return A vector<string> containing the tokens
 * 
 * @note strips trailing commas from tokens
 */
inline std::vector<std::string> split(const std::string& line)
{
	std::vector<std::string> tokens{};
	std::istringstream ss(line);
	std::string token;
	while (ss >> token)
	{
		if (!token.empty() && token.back() == ',') { token.pop_back(); }
		tokens.push_back(token);
	}
	return tokens;
}

template<typename T>
using DispatchTable = std::unordered_map<std::string, std::function<T>>;

} // namespace utils
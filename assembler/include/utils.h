#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> split(const std::string& line)
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

#pragma once
#include "PrecompiledHeader.h"

namespace StringUtilities
{
	std::string Trim(const std::string& value);
	std::vector<std::string> Split(const std::string& value, const char* symbols);
	bool ParseBool(const std::string& value, const char* valueTrue, const char* valueFalse);
	std::string ReadFileName(const std::string& filepath);
	std::string ReadFileRecursiveInclude(const std::string& filepath, bool isRecursive);
	std::string ExtractTokens(const char* token, std::string& source, bool includeToken);
	size_t FirstIndexOf(const char* str, char c);
	size_t LastIndexOf(const char* str, char c);
};


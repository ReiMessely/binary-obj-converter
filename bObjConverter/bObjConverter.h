#pragma once
#include <string>

class bObjConverter final
{
public:
	bObjConverter() = delete;

	static bool ToObj(const std::string& filePath);
	static bool ToBinaryObj(const std::string& filePath);

private:
	struct Double3
	{
		double x{ 0.f };
		double y{ 0.f };
		double z{ 0.f };
	};
	struct UInt3
	{
		unsigned int x{0};
		unsigned int y{0};
		unsigned int z{0};
	};
};


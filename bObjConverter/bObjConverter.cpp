#include "bObjConverter.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include <sstream>

bool bObjConverter::ToObj(const std::string& filePath)
{
	uint16_t inputNr{};
	if (std::ifstream input{ filePath, std::ios::binary }; input.is_open())
	{
		input.read(reinterpret_cast<char *>(&inputNr), sizeof(uint16_t));
		//output.write((const char*)skeleton.name.c_str(), skeleton.name.size());
		//output.put(0xFF);
		//output.write((const char*)&skeleton.x, sizeof(float) * 3);
		input.close();
	}

	return filePath.empty();
}

bool bObjConverter::ToBinaryObj(const std::string& filePath)
{
	std::ifstream input{ filePath,std::ios::binary };
	if (!input.is_open())
	{
		return false;
	}
	std::vector<Double3> vertices{};
	std::vector<UInt3> faces{};
	std::vector<std::string> comments{};
	std::string line;
	char delimiter{ '\n' };


	while (std::getline(input, line, delimiter))
	{
		switch (*line.begin())
		{
		case 'v':
		{
			size_t currentPos{ 0 };
			std::vector<std::string> elements;
			while (currentPos < line.size())
			{
				elements.emplace_back(line.substr(currentPos));
				currentPos = line.find(' ', currentPos + 1);
			}
			Double3 v{ std::stod(elements.at(1)),std::stod(elements.at(2)),std::stod(elements.at(3)) };
			vertices.emplace_back(v);
		}
		break;
		case 'f':
		{
			size_t currentPos{ 0 };
			std::vector<std::string> elements;
			while (currentPos < line.size())
			{
				elements.emplace_back(line.substr(currentPos + 1));
				currentPos = line.find(' ', currentPos + 1);
			}
			UInt3 f{ std::stoul(elements.at(1)),std::stoul(elements.at(2)),std::stoul(elements.at(3)) };
			faces.emplace_back(f);
		}
		break;
		case '#':
		{
			comments.emplace_back(line.substr(line.find(' ')));
		}
		break;
		default:
			std::cout << "Unsuported token: " << *line.begin() << '\n';
			break;
		}
	}

	/*
		# --> Comment find null term
		v --> vertices incoming, next 2 bytes will declare amount
		f --> faceincoming, next 2 bytes will declare amount
	*/
	uint16_t verticesSize = static_cast<uint16_t>(vertices.size());
	if (std::ofstream output{ "output.bObj", std::ios::binary }; output.is_open()) 
	{
		output.write(reinterpret_cast<const char*>(&verticesSize),sizeof(uint16_t));
		//output.write((const char*)skeleton.name.c_str(), skeleton.name.size());
		//output.put(0xFF);
		//output.write((const char*)&skeleton.x, sizeof(float) * 3);
		output.close();
	}


	[[maybe_unused]] int i{ 0 };
	return true;
}

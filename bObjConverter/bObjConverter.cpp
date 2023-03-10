#include "bObjConverter.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include <sstream>

bool bObjConverter::ToObj(const std::string& filePath)
{
	/*
	# --> Comment find null term
	v --> vertices incoming, next 2 bytes will declare amount
	f --> faceincoming, next 2 bytes will declare amount
	*/

	uint16_t verticesAmount{};
	uint16_t facesAmount{};
	/*char delimiter{'\n'}; */
	std::vector<std::string> comments{};
	std::vector<Double3> vertices{};
	std::vector<UInt3> faces{};
	char nullTerminator{'\0'};
	char str{};
	if (std::ifstream input{ filePath, std::ios::binary }; input.is_open())
	{
		while (input.read(&str, sizeof(char)))
		{
			switch (str)
			{
			case '#':
			{
				std::stringstream comment{};
				while (str != nullTerminator)
				{
					input.read(&str, sizeof(char));
					if (str != nullTerminator)
					{
						comment << str;
					}
				}
				comments.emplace_back(comment.str());
			}
			break;
			case 'v':
			{
				Double3 vertex{};
				input.read(reinterpret_cast<char*>(&verticesAmount), sizeof(uint16_t));
				for (uint16_t i{ 0 }; i < verticesAmount; ++i)
				{
					input.read(reinterpret_cast<char*>(&vertex), sizeof(Double3));
					vertices.emplace_back(vertex);
				}
			}
			break;
			case 'f':
			{
				UInt3 face{};
				input.read(reinterpret_cast<char*>(&facesAmount), sizeof(uint16_t));
				for (uint16_t i{ 0 }; i < facesAmount; ++i)
				{
					input.read(reinterpret_cast<char*>(&face), sizeof(UInt3));
					faces.emplace_back(face);
				}
			}
			break;
			default:
				std::cout << "Unsuported token: " << str << '\n';
				break;
			}
		}
		input.close();
	}
	else return false;

	if (std::ofstream output{ "output.obj", std::ios::binary }; output.is_open())
	{
		for (const auto& comment : comments)
		{
			output << "# " << comment;
		}
		for (const auto& vertex : vertices)
		{
			output << "v " << vertex.x << ' ' << vertex.y << ' ' << vertex.z << '\n';
		}
		for (const auto& face : faces)
		{
			output << "f " << face.x << ' ' << face.y << ' ' << face.z << '\n';
		}
	}

	return true;
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
	char whitespace{ ' ' };


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
				currentPos = line.find(whitespace, currentPos + 1);
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
				currentPos = line.find(whitespace, currentPos + 1);
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
	uint16_t verticesAmount = static_cast<uint16_t>(vertices.size());
	uint16_t facesAmount = static_cast<uint16_t>(faces.size());
	if (std::ofstream output{ "output.bObj", std::ios::binary }; output.is_open()) 
	{
		if (comments.size() > 0)
		{
			std::string commentSymbol{ '#' };
			std::string nullTerminator{ '\0' };
			for (const auto& comment : comments)
			{
				output.write(commentSymbol.c_str(), sizeof(char));
				output.write(comment.c_str(), comment.size());
				output.write(nullTerminator.c_str(), sizeof(char));
			}
		}
		char vertexSymbol{ 'v' };
		char faceSymbol{ 'f' };
		output.write(&vertexSymbol, sizeof(char));
		output.write(reinterpret_cast<const char*>(&verticesAmount),sizeof(uint16_t));
		for (const auto& vertex : vertices)
		{
			output.write(reinterpret_cast<const char*>(&vertex), sizeof(Double3));
		}
		output.write(&faceSymbol, sizeof(char));
		output.write(reinterpret_cast<const char*>(&facesAmount), sizeof(uint16_t));
		for (const auto& face : faces)
		{
			output.write(reinterpret_cast<const char*>(&face), sizeof(UInt3));
		}
		output.close();
	}


	[[maybe_unused]] int i{ 0 };
	return true;
}

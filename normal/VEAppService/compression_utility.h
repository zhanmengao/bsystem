#ifndef __COMPRESSION_UTILITY_H__
#define __COMPRESSION_UTILITY_H__


#include<initializer_list>

#include <boost\filesystem.hpp>
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\json_parser.hpp>

#include "protocal_struct.h"
/*
#include <zip.h>
#pragma comment(lib, "zip.lib")
#pragma comment(lib, "zlib.lib")
#pragma comment(lib, "zlibstatic.lib")
*/



class compression_utility final
{
public:
	compression_utility() = default;
	compression_utility(const compression_utility&) = delete;
	compression_utility& operator=(const compression_utility&) = delete;

	static inline std::basic_string<char> compose_zip_file_name(const std::initializer_list<std::basic_string<char>> files)
	{
		std::hash<std::basic_string<char>> hash{};
		std::basic_string<char> file_names{};

		for (const auto& file : files)
		{
			file_names += file;
		}

		std::size_t hash_value{ hash(file_names) };
		std::basic_string<char> zip_file_name{ std::to_string(hash_value) };

		zip_file_name += ".zip";
		return zip_file_name;
	}

	static protocal_message compress_files(const std::initializer_list<std::basic_string<char>>& files, const std::basic_string<char>& zip_file_name);

	static protocal_message temp_zip_file_path()noexcept;
};


#endif //__COMPRESSION_UTILITY_H__
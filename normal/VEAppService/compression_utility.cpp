

#include <fstream>

#include <boost\system\error_code.hpp>


#include "StdAfx.h"
#include "compression_utility.h"

#include <ShlObj.h>



protocal_message compression_utility::compress_files(const std::initializer_list<std::basic_string<char>>& files, const std::basic_string<char>& zip_file_name)
{

	/**

	if (files.size() == 0 || zip_file_name.empty())
	{
		return protocal_message{
			static_cast<unsigned int>(protocal_error_code::COMPRESS_FILES_FAILED),
			error_to_str[protocal_error_code::COMPRESS_FILES_FAILED],
			{ "" }
		};
	}

	std::vector<char*> file_buffer_ptrs{};

	for (const auto& file : files)
	{
		if (!boost::filesystem::exists(file))
		{
			return protocal_message{
				static_cast<unsigned int>(protocal_error_code::COMPRESS_FILES_FAILED),
				error_to_str[protocal_error_code::COMPRESS_FILES_FAILED],
				{ "" }
			};
		}
	}


	protocal_message zip_temp_dir_msg = compression_utility::temp_zip_file_path();

	if (zip_temp_dir_msg.code_)
	{
		return protocal_message{
			static_cast<unsigned int>(protocal_error_code::COMPRESS_FILES_FAILED),
			error_to_str[protocal_error_code::COMPRESS_FILES_FAILED],
			{ "" }
		};
	}

	boost::filesystem::path zip_path{ zip_temp_dir_msg.data_ };
	zip_path /= zip_file_name;

	std::cout << zip_path.string() << std::endl;

	if (boost::filesystem::exists(zip_path))
	{
		boost::system::error_code code{};
		boost::filesystem::remove(zip_path);
	}


	// start compressing files.
	int zip_error{};
	std::basic_string<char> zip_path_str{ zip_path.string() };

	zip_t* zip_handle{ zip_open(zip_path_str.c_str(), ZIP_CREATE, &zip_error) };

	if (!zip_handle)
	{
		zip_close(zip_handle);
		return protocal_message{
			static_cast<unsigned int>(protocal_error_code::COMPRESS_FILES_FAILED),
			error_to_str[protocal_error_code::COMPRESS_FILES_FAILED],
			{ "" }
		};
	}

	for (const auto& file : files)
	{
		std::basic_ifstream<char> file_stream{ file, std::ios_base::binary | std::ios_base::in };

		if (!file_stream.is_open())
		{
			zip_close(zip_handle);
			return protocal_message{
				static_cast<unsigned int>(protocal_error_code::COMPRESS_FILES_FAILED),
				error_to_str[protocal_error_code::COMPRESS_FILES_FAILED],
				{ "" }
			};
		}

		file_stream.seekg(0, std::ios_base::end);
		std::size_t bytes_of_file{ file_stream.tellg()};

		std::cout << "bytes of file:    " << bytes_of_file << std::endl;

		char* file_buffer{ new char[bytes_of_file] {} };
		file_buffer_ptrs.push_back(file_buffer);

		file_stream.seekg(0, std::ios_base::beg);
		file_stream.read(file_buffer, bytes_of_file);

		if (!file_stream)
		{

			for (auto buffer_ptr : file_buffer_ptrs)
			{
				delete[] buffer_ptr;
			}

			return protocal_message{
				static_cast<unsigned int>(protocal_error_code::COMPRESS_FILES_FAILED),
				error_to_str[protocal_error_code::COMPRESS_FILES_FAILED],
				{ "" }
			};
		}

		zip_source_t* source{ zip_source_buffer(zip_handle, file_buffer, bytes_of_file, 0) };

		if (!source)
		{
			zip_source_free(source);
			zip_close(zip_handle);

			for (auto buffer_ptr : file_buffer_ptrs)
			{
				delete[] buffer_ptr;
			}

			return protocal_message{
				static_cast<unsigned int>(protocal_error_code::COMPRESS_FILES_FAILED),
				error_to_str[protocal_error_code::COMPRESS_FILES_FAILED],
				{ "" }
			};
		}


		boost::filesystem::path file_path_for_compressing{ file };
		std::basic_string<char> file_name{ file_path_for_compressing.filename().string() };
		std::int64_t added_index{ zip_file_add(zip_handle, file_name.c_str(), source, ZIP_FL_ENC_GUESS) };

		if (added_index < 0)
		{
			zip_source_free(source);
			zip_close(zip_handle);

			for (auto buffer_ptr : file_buffer_ptrs)
			{
				delete[] buffer_ptr;
			}

			return protocal_message{
				static_cast<unsigned int>(protocal_error_code::COMPRESS_FILES_FAILED),
				error_to_str[protocal_error_code::COMPRESS_FILES_FAILED],
				{ "" }
			};
		}

		source = nullptr;
	}

	int result{ zip_close(zip_handle) };

	for (auto buffer_ptr : file_buffer_ptrs)
	{
		delete[] buffer_ptr;
	}

	if (result < 0)
	{
		return protocal_message{
			static_cast<unsigned int>(protocal_error_code::COMPRESS_FILES_FAILED),
			error_to_str[protocal_error_code::COMPRESS_FILES_FAILED],
			{ "" }
		};
	}

	return protocal_message{
		0,
		"success",
		zip_path.string()
	};

	**/

	return protocal_message{
		0,
		"error",
		"-1"
	};

}






protocal_message compression_utility::temp_zip_file_path()noexcept
{
	/**
	WCHAR user_home_path[MAX_PATH]{};

	if(!SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, user_home_path)))
	{
		return protocal_message{
			static_cast<unsigned int>(protocal_error_code::GET_TEMP_ZIP_FILES_DIR),
			error_to_str[protocal_error_code::GET_TEMP_ZIP_FILES_DIR],
			{ "" }
		};
	}

	std::unique_ptr<std::basic_string<char>> home_path_str{ unicode_to_multibytes(user_home_path) };

	if (!home_path_str)
	{
		return protocal_message{
			static_cast<unsigned int>(protocal_error_code::GET_TEMP_ZIP_FILES_DIR),
			error_to_str[protocal_error_code::GET_TEMP_ZIP_FILES_DIR],
			{ "" }
		};
	}

	boost::filesystem::path temp_files_path{ *home_path_str };
	temp_files_path /= ".shared_manager_temp";

	if(!boost::filesystem::exists(temp_files_path))
	{
		boost::system::error_code code{};
		boost::filesystem::create_directories(temp_files_path, code);

		if (code)
		{
			return protocal_message
			{
				static_cast<unsigned int>(protocal_error_code::GET_TEMP_ZIP_FILES_DIR),
				error_to_str[protocal_error_code::GET_TEMP_ZIP_FILES_DIR],
				{ "" }
			};
		}
	}

	return protocal_message
	{
		0,
		"success",
		temp_files_path.string()
	};

	**/

	return protocal_message
	{
		0,
		"error",
		"-1"
	};
}
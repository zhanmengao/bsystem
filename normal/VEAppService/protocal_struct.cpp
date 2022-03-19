
#include <vector>
#include <cwchar>
#include<functional>

#include "Stdafx.h"
#include "GCDirFile.h"
#include "protocal_struct.h"


#include <LM.h>
#include <AclAPI.h>
#include <LMShare.h>
#include <Shlobj.h>

#include "../../../lib/easyloggingpp/easylogging++.h"



const char* const DATA{ "data" };
const char* const MESSAGE{ "msg" };
const char* const CODE{ "code" };
const char* const FILE_PATH{ "folderPath" };
const char* const USER_NAME{ "userName" };



std::map<protocal_error_code, std::basic_string<char>> error_to_str
{
	{protocal_error_code::PROTOCAL_STRUCT_TO_JSON_FAILED, "A error occured, during object to json string." },
	{protocal_error_code::PARSE_JSON_FAILED, "Failed to parse json."},
	{protocal_error_code::JSON_BUFFER_IS_EMPTY, "You are parsing a empty json string."},
	{protocal_error_code::NO_USERNAME_OR_PWD_DURING_CRETING_USER, "Can not get user name or password during creating user."},
	{protocal_error_code::MULTI_BYTES_TO_UNICODE_FAILED, "Failed to change multi bytes string to unicode string."},
	{protocal_error_code::UNOCODE_TO_MULTI_BYTES_FAILED, "Failed to change unicode string to multi bytes string."},
	{protocal_error_code::CREATE_USER_FAILED, "Failed to create user."},
	{protocal_error_code::USER_HOME_DIR_EXISTED, "User home existed."},
	{protocal_error_code::USER_NAME_IS_EMPTY, "You are using an empty string as user name."},
	{protocal_error_code::USER_ASSOCIATED_FOLDER_FAILED, "Failed to use user to associate folder."},
	{protocal_error_code::ADD_USER_TO_GROUP_FAILED, "Failed to add user to group."},
	{protocal_error_code::GET_EXE_PATH_FAILED, "Failed to get current exe path."},
	{protocal_error_code::CREATE_DISK_CONFIG_FAILED, "Failed to create disk config."},
	{protocal_error_code::READ_DISK_CONFIG_FAILED, "Failed to read disk config."},
	{protocal_error_code::DO_NOT_EXIST_THIS_NODE_IN_DISK_CONFIG, "Try to read a specify node in disk config which do not exist."},
	{protocal_error_code::PATH_IS_EMPTY, "Using a empty string as a path."},
	{protocal_error_code::DELETE_FILE_OR_DIR_FAILED, "Failed to delete a dir or file."},
	{protocal_error_code::UPLOAD_FILE_FAILED, "Failed to upload file!"},
	{protocal_error_code::GET_TEMP_ZIP_FILES_DIR, "Failed to create a dir for store temp zip files!"},
	{protocal_error_code::COMPRESS_FILES_FAILED, "Failed to compress files!"}
};


std::unique_ptr<protocal_message> parse_buff(const std::basic_string<char>& buffer)
{
	if (buffer.empty())
	{
		return std::make_unique<protocal_message>(protocal_message{
			static_cast<unsigned int>(protocal_error_code::JSON_BUFFER_IS_EMPTY),
			error_to_str[protocal_error_code::JSON_BUFFER_IS_EMPTY],
			{ "" }
		});
	}

	try {

	Json::Reader json_reader{};
	Json::Value root{};

	bool parse_flag{ json_reader.parse(buffer, root, false) };

	if (!parse_flag)
	{
		return std::make_unique<protocal_message>(protocal_message{
			static_cast<unsigned int>(protocal_error_code::PARSE_JSON_FAILED),
			error_to_str[protocal_error_code::PARSE_JSON_FAILED],
			{ "" }
		});
	}


	Json::Value code_value{ root.get(CODE, Json::Value{}) };

	// maybe, sometimes can not parse CODE, so will get a empty value.
	if (code_value.empty())
	{
		return std::make_unique<protocal_message>(protocal_message{
			static_cast<unsigned int>(protocal_error_code::PARSE_JSON_FAILED),
			error_to_str[protocal_error_code::PARSE_JSON_FAILED],
			{ "" }
		});
	}

	unsigned int code{ root[CODE].asUInt() };
	std::basic_string<char> message{ root[MESSAGE].asString() };
	std::basic_string<char> data{ root[DATA].asString() };

	if (data.empty() || code < 0)
	{
		return std::make_unique<protocal_message>(protocal_message{
			static_cast<unsigned int>(protocal_error_code::PARSE_JSON_FAILED),
			error_to_str[protocal_error_code::PARSE_JSON_FAILED],
			{ "" }
		});
	}

	return std::make_unique<protocal_message>(protocal_message{ code, message, data });

	}
	catch (const Json::Exception& json_exception) // a error occured during parse json.
	{
		LOG(ERROR) << json_exception.what();

		return std::make_unique<protocal_message>(protocal_message{
			static_cast<unsigned int>(protocal_error_code::PARSE_JSON_FAILED),
			error_to_str[protocal_error_code::PARSE_JSON_FAILED],
			{ "" }
		});
	}
}


std::unique_ptr<std::basic_string<char>> struct_to_json(const protocal_message& the_message)
{

	std::function <std::basic_string<char>()> produce_default_json
	{
	[]()
	{
		Json::Value json_object{Json::objectValue};
		json_object[CODE] = Json::Value{ static_cast<unsigned int>(protocal_error_code::PROTOCAL_STRUCT_TO_JSON_FAILED) };
		json_object[MESSAGE] = Json::Value{ error_to_str[protocal_error_code::PROTOCAL_STRUCT_TO_JSON_FAILED] };
		json_object[DATA] = Json::Value{ "" };

		Json::FastWriter fast_writer{};
		std::basic_string<char> json_str{fast_writer.write(json_object)};

		return json_str;
	}
	};

	try {

		Json::Value json_object{};
		json_object[CODE] = Json::Value{the_message.code_};
		json_object[MESSAGE] = Json::Value{the_message.message_};
		json_object[DATA] = Json::Value{the_message.data_};

		Json::FastWriter fast_writer{};
		std::basic_string<char> json_str{ fast_writer.write(json_object) };

		if (json_str.empty())
		{
			return std::make_unique<std::basic_string<char>>(produce_default_json());
		}

		return std::make_unique<std::basic_string<char>>(json_str);
	}
	catch (Json::Exception& json_exception)
	{
		LOG(ERROR) << json_exception.what();
		return std::make_unique<std::basic_string<char>>(produce_default_json());
	}
}


std::unique_ptr<std::basic_string<wchar_t>> multibytes_to_unicode(const std::basic_string<char>& multibytes_str)
{
//	std::mbstate_t state{};
//	const char* c_str{ multibytes_str.c_str() };
//	std::size_t size_of_wchars{ std::mbsrtowcs(nullptr, &c_str, 0, &state)};
//	std::basic_string<wchar_t> unicode_str{L"\0", size_of_wchars + 1};
//
//	std::size_t result{ std::mbsrtowcs(&unicode_str[0], &c_str, size_of_wchars, &state) };
//
//#ifdef _DEBUG
//	std::cout << "change result: " << result << std::endl; // result = size_of_wchars;
//	std::cout << "needed chars: " << size_of_wchars << std::endl;
//#endif //_DEBUG
//
//	if (result != size_of_wchars)
//	{
//		return {nullptr};
//	}
//
//	return std::make_unique<std::basic_string<wchar_t>>(unicode_str);

	int size_of_wchars{ MultiByteToWideChar(CP_ACP, 0, multibytes_str.c_str(), multibytes_str.size(), nullptr, 0) };
	wchar_t* unicode_str{ new wchar_t[size_of_wchars + 1]{} };
	int size_of_changed_chars{ MultiByteToWideChar(CP_ACP, 0, multibytes_str.c_str(), multibytes_str.size(), &unicode_str[0], size_of_wchars) };

	if (size_of_changed_chars != size_of_wchars)
	{
		delete[] unicode_str;
		return{ nullptr };
	}

	std::unique_ptr<std::basic_string<wchar_t>> unicode_str_ptr{ std::make_unique<std::basic_string<wchar_t>>(std::basic_string<wchar_t>{unicode_str}) };
	delete[] unicode_str;

	return unicode_str_ptr;
}

std::unique_ptr<std::basic_string<char>> unicode_to_multibytes(const std::basic_string<wchar_t>& unicode_str)
{
//	std::mbstate_t state{};
//	const wchar_t* c_str{ unicode_str.c_str() };
//
//	std::size_t size_of_chars{ std::wcsrtombs(nullptr, &c_str, 0, &state) };
//	std::basic_string<char> multi_bytes_str{"\0", size_of_chars + 1};
//
//	std::size_t result{ std::wcsrtombs(&multi_bytes_str[0], &c_str, multi_bytes_str.size(), &state) };
//
//#ifdef _DEBUG
//	std::cout << "change result: " << result << std::endl; // result = size_of_wchars - 1;
//	std::cout << "needed chars: " << size_of_chars << std::endl;
//#endif //_DEBUG
//
//	if (result != size_of_chars)
//	{
//		return{ nullptr };
//	}
//
//	return std::make_unique<std::basic_string<char>>(multi_bytes_str);


	std::basic_string<char> multi_bytes_str{};
	int size_of_chars{ WideCharToMultiByte(CP_ACP, 0, unicode_str.c_str(), unicode_str.size(), nullptr, 0, nullptr, nullptr) };
	char* buffer = new char[size_of_chars + 1];
	buffer[size_of_chars] = '\0';
	int size_of_changed_chars{ WideCharToMultiByte(CP_ACP, 0, unicode_str.c_str(), unicode_str.size(), buffer, size_of_chars, nullptr, nullptr) };

	if (size_of_chars != size_of_changed_chars)
	{
		delete[]buffer;
		return{ nullptr };
	}

	multi_bytes_str.append(buffer);
	delete[]buffer;

	return std::make_unique<std::basic_string<char>>(multi_bytes_str);

}


std::unique_ptr<protocal_message> delete_directory(const std::basic_string<char>& dir_str)
{
	if (dir_str.empty())
	{
		return std::make_unique<protocal_message>(protocal_message{
			static_cast<unsigned int>(protocal_error_code::PATH_IS_EMPTY),
			error_to_str[protocal_error_code::PATH_IS_EMPTY],
			{ "" }
		});
	}

	CFileFind finder;
	char next_path_str[MAX_PATH]{};
	CString unicode_file_path(dir_str.c_str());
	BOOL whether_found_files{ finder.FindFile(unicode_file_path) };

	if (!whether_found_files)
	{
		return std::make_unique<protocal_message>(protocal_message{
			static_cast<unsigned int>(protocal_error_code::DELETE_FILE_OR_DIR_FAILED),
			error_to_str[protocal_error_code::DELETE_FILE_OR_DIR_FAILED],
			{ "" }
		});
	}


	USES_CONVERSION;
	while (whether_found_files)
	{
		whether_found_files = finder.FindNextFile();

		if (finder.IsDots())
		{
			continue;
		} else if (finder.IsDirectory()) {

			CString current_dir_path_str{ finder.GetFilePath() };
			char * str_of_current_dir_path{ T2A(current_dir_path_str) };

			sprintf(next_path_str, "%s\\%s", str_of_current_dir_path, "*.*");

			std::unique_ptr<protocal_message> result_ptr{ delete_directory(next_path_str) };

			BOOL flag_of_del_dir{ RemoveDirectory(current_dir_path_str) };

			std::wcout << L"current_dir_path_str:   " << current_dir_path_str << std::endl;

			if (result_ptr->code_ > 0)
			{
				finder.Close();
				return result_ptr;
			}

		} else {
			CString file_path_str{ finder.GetFilePath() };
			DeleteFile(file_path_str);
		}

	}

	finder.Close();

	return std::make_unique<protocal_message>(protocal_message{
		0,
		{"success"},
		{ "" }
	});
}


std::unique_ptr<protocal_message> create_directory(const std::basic_string<char>& dir_str)
{
	if (dir_str.empty())
	{
		return std::make_unique<protocal_message>(protocal_message{
			static_cast<unsigned int>(protocal_error_code::PATH_IS_EMPTY),
			error_to_str[protocal_error_code::PATH_IS_EMPTY],
			{ "" }
		});
	}

	std::basic_string<char> dir_str_backup{ dir_str };

	std::basic_string<char>::iterator beg_itr{ dir_str_backup.begin() };
	std::basic_string<char>::iterator end_itr{ dir_str_backup.end() };

	for (; beg_itr != end_itr; ++beg_itr)
	{
		if (*beg_itr == '/')
		{
			*beg_itr = '\\';
		}
	}

	std::unique_ptr<std::basic_string<wchar_t>> unicode_dir_str{ multibytes_to_unicode(dir_str_backup) };

	if (!unicode_dir_str)
	{
		return std::make_unique<protocal_message>(protocal_message{
			static_cast<unsigned int>(protocal_error_code::MULTI_BYTES_TO_UNICODE_FAILED),
			error_to_str[protocal_error_code::MULTI_BYTES_TO_UNICODE_FAILED],
			{ "" }
		});
	}

	bool is_exist{ path_file_exists(*unicode_dir_str) };
	std::cout << __FILE__ << " " << __LINE__ << "  Path exists: " << is_exist << std::endl;

	if (is_exist)
	{
		return std::make_unique<protocal_message>(protocal_message{
			0,
			{ "success" },
			{ "" }
		});
	}

	BOOL flag_of_creating{TRUE};

	std::unique_ptr<std::basic_string<wchar_t>> unicode_dir_str_backup{ multibytes_to_unicode(dir_str_backup) };

	if (!unicode_dir_str_backup)
	{
		flag_of_creating = FALSE;
	}


	if (SHCreateDirectory(NULL, unicode_dir_str_backup->c_str()) != ERROR_SUCCESS)
	{
		flag_of_creating = FALSE;
	}


	if (!flag_of_creating)
	{
			return std::make_unique<protocal_message>(protocal_message{
				static_cast<unsigned int>(protocal_error_code::DELETE_FILE_OR_DIR_FAILED),
				error_to_str[protocal_error_code::DELETE_FILE_OR_DIR_FAILED],
				{ "" }
			});
	}

	return std::make_unique<protocal_message>(protocal_message{
		0,
		{ "success" },
		{ "" }
	});
}


template<>
bool path_file_exists<wchar_t>(const std::basic_string<wchar_t>& path_or_file_str)
{
	if (path_or_file_str.empty())
	{
		return true;
	}

	BOOL is_exist{ ::PathFileExists(path_or_file_str.c_str()) };
	return static_cast<bool>(is_exist);
}



template<>
bool path_file_exists<char>(const std::basic_string<char>& path_or_file_str)
{
	if (path_or_file_str.empty())
	{
		return true;
	}

	std::unique_ptr<std::basic_string<wchar_t>> unicode_path_or_file_str{
		multibytes_to_unicode(path_or_file_str)
	};

	if (!unicode_path_or_file_str)
	{
		return true;
	}

	BOOL is_exist{ ::PathFileExists(unicode_path_or_file_str->c_str()) };

	return static_cast<bool>(is_exist);
}




// add temporarily. this function will be deleted in develop branch.
std::list<std::basic_string<char>> get_all_users()
{
	LPUSER_INFO_0 user_info_0_ptr{ nullptr };
	LPUSER_INFO_0 temp_user_info_0_ptr{ nullptr };
	DWORD dw_level{ 0 };
	DWORD dw_perf_max_len{ MAX_PREFERRED_LENGTH };
	DWORD dw_entries_read{ 0 };
	DWORD dw_total_entries{ 0 };
	DWORD dw_resume_handle{ 0 };
	DWORD dw_total_count{ 0 };
	NET_API_STATUS status{};

	std::list<std::basic_string<char>> user_name_list{};

	do // begin do
	{
		status = NetUserEnum(nullptr,
			dw_level,
			FILTER_NORMAL_ACCOUNT, // global users
			(LPBYTE*)&user_info_0_ptr,
			dw_perf_max_len,
			&dw_entries_read,
			&dw_total_entries,
			&dw_resume_handle);
		//
		// If the call succeeds,
		//
		if ((status == NERR_Success) || (status == ERROR_MORE_DATA))
		{
			if ((temp_user_info_0_ptr = user_info_0_ptr) != nullptr)
			{
				std::size_t index{ 0 };
				for (; index < dw_entries_read; ++index)
				{

					if (temp_user_info_0_ptr == nullptr)
					{
						fprintf(stderr, "An access violation has occurred\n");
						break;
					}

					std::unique_ptr<std::basic_string<char>> multibytes_user_name{ unicode_to_multibytes(temp_user_info_0_ptr->usri0_name) };


					// if failed to change multi bytes string to wide char string. 
					if (!multibytes_user_name)
					{
						if (user_info_0_ptr != nullptr)
						{
							NetApiBufferFree(user_info_0_ptr);
							user_info_0_ptr = nullptr;
						}

						return user_name_list;
					}

					std::cout << __FILE__ << "  " << __LINE__ << " user name: " << *multibytes_user_name << std::endl;
					user_name_list.push_back(*multibytes_user_name);

					++temp_user_info_0_ptr;
					++dw_total_count;
				}
			}

		}
		else {
			return user_name_list;
		}

		//
		// Free the allocated buffer.
		//
		if (user_info_0_ptr != nullptr)
		{
			NetApiBufferFree(user_info_0_ptr);
			user_info_0_ptr = nullptr;
		}

	} while (status == ERROR_MORE_DATA);

	return user_name_list;
}


bool add_user_to_folder_permission(const std::basic_string<char>& path, const std::basic_string<char>& user_name)
{
	BOOL bSuccess = TRUE;
	EXPLICIT_ACCESS ea;
	PACL pNewDacl = NULL;
	PACL pOldDacl = NULL;

	auto unicode_path_ptr{ multibytes_to_unicode(path) };
	auto unicode_user_name_ptr{ multibytes_to_unicode(user_name) };

	if (!(unicode_path_ptr && unicode_user_name_ptr))
	{
		return false;
	}

	do
	{
		// 获取文件(夹)安全对象的DACL列表

		if (ERROR_SUCCESS != GetNamedSecurityInfo(unicode_path_ptr->c_str(), SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &pOldDacl, NULL, NULL))
		{
			bSuccess = FALSE;
			break;

		}

		// 此处不可直接用AddAccessAllowedAce函数,因为已有的DACL长度是固定,必须重新创建一个DACL对象

		// 生成指定用户帐户的访问控制信息(这里指定赋予全部的访问权限)

		::BuildExplicitAccessWithName(&ea, const_cast<wchar_t*>(unicode_user_name_ptr->data()), GENERIC_ALL, GRANT_ACCESS, SUB_CONTAINERS_AND_OBJECTS_INHERIT);

		// 创建新的ACL对象(合并已有的ACL对象和刚生成的用户帐户访问控制信息)

		if (ERROR_SUCCESS != ::SetEntriesInAcl(1, &ea, pOldDacl, &pNewDacl))
		{
			bSuccess = FALSE;
			break;
		}

		// 设置文件(夹)安全对象的DACL列表
		if (ERROR_SUCCESS != ::SetNamedSecurityInfo(const_cast<wchar_t*>(unicode_path_ptr->data()), SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, pNewDacl, NULL))
		{
			bSuccess = FALSE;
		}
	} while (FALSE);

	if (NULL != pNewDacl)
	{
		::LocalFree(pNewDacl);
	}

	return bSuccess;
}


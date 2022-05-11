#ifndef __PROTOCAL_STRUCT_H__
#define __PROTOCAL_STRUCT_H__


#ifdef _WIN64

#include <json_x64\json.h>
#include <json_x64\forwards.h>

#ifdef _DEBUG

#pragma comment(lib,"lib_json_d.lib")
#else
#pragma comment(lib,"lib_json_r.lib")

#endif //_DEBUG

#else

#include <json\json.h>
#include <json\forwards.h>

#ifdef _DEBUG
#else
#endif // DEBUG


#endif //_WIN64



#include <map>
#include <list>
#include <string>
#include <memory>
#include <type_traits>


enum class protocal_error_code : unsigned int
{
	PROTOCAL_STRUCT_TO_JSON_FAILED = 999,
	PARSE_JSON_FAILED,
	JSON_BUFFER_IS_EMPTY,
	NO_USERNAME_OR_PWD_DURING_CRETING_USER,
	MULTI_BYTES_TO_UNICODE_FAILED,
	UNOCODE_TO_MULTI_BYTES_FAILED,
	CREATE_USER_FAILED,
	USER_HOME_DIR_EXISTED,
	USER_NAME_IS_EMPTY,
	USER_ASSOCIATED_FOLDER_FAILED,
	ADD_USER_TO_GROUP_FAILED,
	GET_EXE_PATH_FAILED,
	CREATE_DISK_CONFIG_FAILED,
	READ_DISK_CONFIG_FAILED,
	DO_NOT_EXIST_THIS_NODE_IN_DISK_CONFIG,
	PATH_IS_EMPTY,
	DELETE_FILE_OR_DIR_FAILED,
	UPLOAD_FILE_FAILED,
	GET_TEMP_ZIP_FILES_DIR,
	COMPRESS_FILES_FAILED
};

extern std::map<protocal_error_code, std::basic_string<char>> error_to_str;

extern const char* const DATA;
extern const char* const MESSAGE;
extern const char* const CODE;
extern const char* const FILE_PATH;
extern const char* const USER_NAME;


struct protocal_message
{
	unsigned int code_; // It is error code or operation code.
	std::basic_string<char> message_;
	std::basic_string<char> data_;
};



extern std::unique_ptr<protocal_message> parse_buff(const std::basic_string<char>& buffer);
extern std::unique_ptr<std::basic_string<char>> struct_to_json(const protocal_message& the_message);
extern std::unique_ptr<std::basic_string<wchar_t>> multibytes_to_unicode(const std::basic_string<char>& multibytes_str);
extern std::unique_ptr<std::basic_string<char>> unicode_to_multibytes(const std::basic_string<wchar_t>& unicode_str);

extern std::unique_ptr<protocal_message> delete_directory(const std::basic_string<char>& dir_str);
extern std::unique_ptr<protocal_message> create_directory(const std::basic_string<char>& dir_str);

extern std::list<std::basic_string<char>> get_all_users();
extern bool add_user_to_folder_permission(const std::basic_string<char>& path, const std::basic_string<char>& user_name);


template<typename Ty, typename = typename std::enable_if<std::is_same<wchar_t, Ty>::value || 
														 std::is_same<char, Ty>::value>::type>
bool path_file_exists(const std::basic_string<Ty>& path_or_file_str)
{
	return false;
}

template<>
bool path_file_exists<wchar_t>(const std::basic_string<wchar_t>& path_or_file_str);

template<>
bool path_file_exists<char>(const std::basic_string<char>& path_or_file_str);



#endif //__PROTOCAL_STRUCT_H__

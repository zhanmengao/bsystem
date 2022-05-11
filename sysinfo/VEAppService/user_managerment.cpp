

#include "disk_config.h"
#include "user_managerment.h"
#include "easyloggingpp\easylogging++.h"


#include <LM.h> // for creating users.
#include <shlwapi.h>


constexpr static const char* const PASSWORD{ "password" };
constexpr static const char* const USERNAME{ "username" };



std::unique_ptr<protocal_message> user_managerment::create_users(const protocal_message& message)
{
	std::deque<user_managerment::user_info> infos_of_users{ this->parse_data(message.data_) };

	return{ nullptr };
}


std::deque<user_managerment::user_info> user_managerment::parse_data(const std::basic_string<char>& data)
{
	std::deque<user_managerment::user_info> user_infos{};

	if (data.empty())
	{
		return std::deque<user_managerment::user_info>{}; // if there no data in data parameter.
	}

	try {
		Json::Reader json_reader{};
		Json::Value json_root{};

		bool parse_result{ json_reader.parse(data, json_root, false) };

#ifdef _DEBUG
		std::cout << std::boolalpha << "parse result: " << parse_result << std::endl;
		std::cout << "is arrary: " << json_root.isArray() << std::endl;
#endif //_DEBUG

		if (!(parse_result && json_root.isArray()))
		{
			return std::deque<user_managerment::user_info>{};
		}

		Json::Value::iterator beg{ json_root.begin() };

		for (; beg != json_root.end(); ++beg)
		{
			user_managerment::user_info u_info{};
			Json::Value array_item{ *beg };

			u_info.user_name_ =  array_item[USERNAME].asString();
			u_info.user_password_ = array_item[PASSWORD].asString();

#ifdef _DEBUG
			std::cout << "username: " << u_info.user_name_ << std::endl;
			std::cout << "password: " << u_info.user_password_ << std::endl;
#endif //_DEBUG

			if (u_info.user_name_.empty() || u_info.user_password_.empty())
			{
				break;
			}

			user_infos.push_back(u_info);
		}

		user_infos.clear();
		return user_infos;
	}
	catch (Json::Exception& json_exception)
	{
		LOG(ERROR) << "A exception occured during parse username and password.";
		user_infos.clear();
		return user_infos;
	}
}



std::unique_ptr<protocal_message> user_managerment::create_user_p(const user_managerment::user_info& info)
{
	if (info.user_name_.empty() || info.user_password_.empty())
	{
		return std::make_unique<protocal_message>(protocal_message{
			static_cast<unsigned int>(protocal_error_code::NO_USERNAME_OR_PWD_DURING_CRETING_USER),
			error_to_str[protocal_error_code::NO_USERNAME_OR_PWD_DURING_CRETING_USER],
			{""}
		});
	}

	std::unique_ptr<std::basic_string<wchar_t>> unicode_name{ multibytes_to_unicode(info.user_name_) };
	std::unique_ptr<std::basic_string<wchar_t>> unicode_password{ multibytes_to_unicode(info.user_password_) };

	if (!(unicode_name && unicode_password))
	{
		return std::make_unique<protocal_message>(protocal_message{
			static_cast<unsigned int>(protocal_error_code::MULTI_BYTES_TO_UNICODE_FAILED),
			error_to_str[protocal_error_code::MULTI_BYTES_TO_UNICODE_FAILED],
			{ "" }
		});
	}


	USER_INFO_1 user_info_1{};
	DWORD dw_level{ 1 };
	DWORD dw_error{ 0 };
	NET_API_STATUS status{};

	user_info_1.usri1_priv = USER_PRIV_USER;
	user_info_1.usri1_home_dir = nullptr;
	user_info_1.usri1_comment = nullptr;
	user_info_1.usri1_flags = UF_SCRIPT | UF_DONT_EXPIRE_PASSWD;
	user_info_1.usri1_script_path = nullptr;

	status = ::NetUserAdd(nullptr, dw_level, reinterpret_cast<LPBYTE>(&user_info_1), &dw_error);

	bool flag{ true };

	switch (status)
	{
	case ERROR_ACCESS_DENIED:
	{
		flag = false;
		LOG(INFO) << "creat user error: ERROR_ACCESS_DENIED";
		break;
	}
	case NERR_InvalidComputer:
	{
		flag = false;
		LOG(INFO) << "creat user error: NERR_InvalidComputer";
		break;
	}
	case NERR_NotPrimary:
	{
		flag = false;
		LOG(INFO) << "creat user error: NERR_NotPrimary";
		break;
	}
	case NERR_GroupExists:
	{
		flag = false;
		LOG(INFO) << "creat user error: NERR_GroupExists";
		break;
	}
	case NERR_UserExists:
	{
		flag = false;
		LOG(INFO) << "creat user error: NERR_UserExists";
		break;
	}
	case NERR_PasswordTooShort:
	{
		flag = false;
		LOG(INFO) << "creat user error: NERR_PasswordTooShort";
		break;
	}
	default:
		break;
	}

	if (!flag)
	{
		return std::make_unique<protocal_message>(protocal_message{
			static_cast<unsigned int>(protocal_error_code::CREATE_USER_FAILED),
			error_to_str[protocal_error_code::CREATE_USER_FAILED],
			{ "" }
		});
	}

	return{ nullptr };
}



std::unique_ptr<protocal_message> user_managerment::add_user_to_groups(const std::basic_string<char>& user_name, std::vector<std::basic_string<char>> groups_name)
{
	if (user_name.empty() || groups_name.empty())
	{
		return{ nullptr };
	}

	std::unique_ptr<std::basic_string<wchar_t>> unicode_user_name{ multibytes_to_unicode(user_name) };

	if (!unicode_user_name)
	{
		return std::make_unique<protocal_message>(protocal_message{
			static_cast<unsigned int>(protocal_error_code::CREATE_USER_FAILED),
			error_to_str[protocal_error_code::CREATE_USER_FAILED],
			{ "" }
		});
	}

}



std::unique_ptr<protocal_message> add_user_to_group_p(const std::basic_string<wchar_t>& unicode_user_name, const std::basic_string<wchar_t>& unicode_group_name)
{
	DWORD level{ 3 };
	LOCALGROUP_MEMBERS_INFO_3 account{};
	DWORD  totalentries{ 1 };

	account.lgrmi3_domainandname = const_cast<wchar_t*>(unicode_user_name.data());


	NET_API_STATUS status{ NetLocalGroupAddMembers(NULL, unicode_group_name.data(), level, reinterpret_cast<LPBYTE>(&account), totalentries) };

	bool flag{ true };

	switch (status)
	{
	case NERR_GroupNotFound:
	{
		flag = false;
		LOG(INFO) << "creat user error: NERR_GroupNotFound";
		break;
	}
	case ERROR_ACCESS_DENIED:
	{
		flag = false;
		LOG(INFO) << "creat user error: ERROR_ACCESS_DENIED";
		break;
	}
	case ERROR_NO_SUCH_MEMBER:
	{
		flag = false;
		LOG(INFO) << "creat user error: ERROR_NO_SUCH_MEMBER";
		break;
	}
	case ERROR_MEMBER_IN_ALIAS:
	{
		flag = false;
		LOG(INFO) << "creat user error: ERROR_MEMBER_IN_ALIAS";
		break;
	}
	case ERROR_INVALID_MEMBER:
	{
		flag = false;
		LOG(INFO) << "creat user error: ERROR_INVALID_MEMBER";
		break;
	}
	default:
		break;
	}

	if (!flag)
	{
		return std::make_unique<protocal_message>(protocal_message{
			static_cast<unsigned int>(protocal_error_code::CREATE_USER_FAILED),
			error_to_str[protocal_error_code::CREATE_USER_FAILED],
			{ "" }
		});
	}

	return{ nullptr };

}


std::unique_ptr<protocal_message> user_managerment::create_user_folder(const std::basic_string<char>& user_name)
{
	if (user_name.empty())
	{
		return std::make_unique<protocal_message>(protocal_message{
			static_cast<unsigned int>(protocal_error_code::USER_NAME_IS_EMPTY),
			error_to_str[protocal_error_code::USER_NAME_IS_EMPTY],
			{ "" }
		});
	}

	disk_config xml_config{};
	std::unique_ptr<protocal_message> driver_in_xml{ xml_config[disk_config::DISK] };

	if (driver_in_xml->code_)
	{
		return driver_in_xml;
	}

	std::basic_string<char> user_folder_str{ driver_in_xml->message_ + "\\" + user_name };
	std::unique_ptr<std::basic_string<wchar_t>> unicode_folder_str{ multibytes_to_unicode(user_folder_str) };

	if (!unicode_folder_str)
	{
		return std::make_unique<protocal_message>(protocal_message{
			static_cast<unsigned int>(protocal_error_code::MULTI_BYTES_TO_UNICODE_FAILED),
			error_to_str[protocal_error_code::MULTI_BYTES_TO_UNICODE_FAILED],
			{ "" }
		});
	}

	bool is_exist{ path_file_exists(*unicode_folder_str) };

	if (is_exist)
	{

	}


}

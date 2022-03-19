#ifndef __USER_MANAGERMENT_H__
#define __USER_MANAGERMENT_H__

#include <deque>
#include <vector>

#include "protocal_struct.h"


class user_managerment
{
public:
	static inline user_managerment* instance() noexcept
	{
		static user_managerment* the_instance{ new user_managerment };
		return the_instance;
	}

	// notice that, this function will invoke add_user_groups and shared_user_dir.
	std::unique_ptr<protocal_message> create_users(const protocal_message& message);


	std::unique_ptr<protocal_message> add_user_to_groups(const std::basic_string<char>& user_name,
		std::vector<std::basic_string<char>> groups_name = {"Remote Desktop Users", "Users"});
	void shared_user_dir();

	std::unique_ptr<protocal_message> create_user_folder(const std::basic_string<char>& user_name);

private:
	struct user_info
	{
		std::basic_string<char> user_name_;
		std::basic_string<char> user_password_;
	};


	 std::deque<user_info> parse_data(const std::basic_string<char>& data);
	 std::unique_ptr<protocal_message> create_user_p(const user_managerment::user_info& info);
	 std::unique_ptr<protocal_message> add_user_to_group_p(const std::basic_string<wchar_t>& unicode_user_name, const std::basic_string<wchar_t>& unicode_group_name);
};





#endif __USER_MANAGER_H__
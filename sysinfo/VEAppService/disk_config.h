#ifndef __DISK_CONFIG_H__
#define __DISK_CONFIG_H__


#include <string>
#include <memory>

#include "protocal_struct.h"



class disk_config final
{
public:

	constexpr static const char* const DISK{ "disk" };
	constexpr static const char* const PERSONAL{ "personal" };
	constexpr static const char* const PUBLIC{ "public" };
	constexpr static const char* const DOWNLOAD{ "download" };


	// the returned value will not be nullptr. if succeededed, code_ will be equal to 0.
	std::unique_ptr<protocal_message> operator[](const std::basic_string<char>& node_name);

	// the returned value will not be nullptr. if succeededed, code_ will be equal to 0.
	inline std::unique_ptr<protocal_message> value(const std::basic_string<char>& node_name)
	{
		return this->operator[](node_name);
	}


private:
	std::unique_ptr<protocal_message> create_default_config(const std::basic_string<char>& xml_path);
	std::unique_ptr<protocal_message> read_disk_config_p(const std::basic_string<char>& node_name);
};




#endif // __DISK_CONFIG_H__

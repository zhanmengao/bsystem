

#include <iostream>
#include <functional>

#include <shlwapi.h>



#include "disk_config.h"
#include "../../../lib/tinyxml/tinyxml.h"


constexpr static const char* const CONFIG_NAME{ "disk_config.xml" };
constexpr static const char* const DEFAULT_DRIVER{ "e:\\" };


std::unique_ptr<protocal_message> disk_config::operator[](const std::basic_string<char>& node_name)
{
	if (node_name.empty())
	{
		return std::make_unique<protocal_message>(protocal_message{
			static_cast<unsigned int>(protocal_error_code::DO_NOT_EXIST_THIS_NODE_IN_DISK_CONFIG),
			error_to_str[protocal_error_code::DO_NOT_EXIST_THIS_NODE_IN_DISK_CONFIG],
			{ "" }
		});
	}

	std::unique_ptr<protocal_message> reading_result{ this->read_disk_config_p(node_name) };
	return reading_result;
}




std::unique_ptr<protocal_message> disk_config::read_disk_config_p(const std::basic_string<char>& node_name)
{
	std::function<std::basic_string<char>(TiXmlElement* const root_node, const std::basic_string<char>& node_name)> get_value_in_node
	{
		[](TiXmlElement* const root_node, const std::basic_string<char>& node_name) ->std::basic_string<char> {
		   TiXmlElement* element_node{ root_node->FirstChildElement(node_name.c_str()) };

		   if (!element_node)
		   {
			   return{};
		   }

		   std::basic_string<char> value{ element_node->GetText() };
		   return value;
		}
	};

	wchar_t exe_full_path[MAX_PATH]{};
	DWORD path_longth_in_array{ GetModuleFileName(nullptr, exe_full_path, MAX_PATH) };


	if (path_longth_in_array == 0)
	{
		return std::make_unique<protocal_message>(protocal_message{
			static_cast<unsigned int>(protocal_error_code::GET_EXE_PATH_FAILED),
			error_to_str[protocal_error_code::GET_EXE_PATH_FAILED],
			{ "" }
		});
	}

	std::basic_string<wchar_t> path_str{ exe_full_path, exe_full_path + path_longth_in_array };
	std::basic_string<wchar_t>::size_type pos{ path_str.find_last_of(L'\\') };
	path_str.erase(pos);
	path_str.push_back(L'\\');
	path_str += L"disk_config.xml";


	std::wcout << __FILE__ << L"  " << __LINE__ << L"  " << L"path of exe:  " << path_str << std::endl;

	bool is_exist{ path_file_exists(path_str) };
	std::unique_ptr<std::basic_string<char>> multi_bytes_path_str{ unicode_to_multibytes(path_str) };


	if (!multi_bytes_path_str)
	{
		return std::make_unique<protocal_message>(protocal_message{
			static_cast<unsigned int>(protocal_error_code::UNOCODE_TO_MULTI_BYTES_FAILED),
			error_to_str[protocal_error_code::UNOCODE_TO_MULTI_BYTES_FAILED],
			{ "" }
		});
	}

#ifdef _DEBUG
	std::cout << std::boolalpha << "whether exist disk config: " << is_exist << std::endl;
#endif //_DEBUG

	if (!is_exist)
	{
		std::unique_ptr<protocal_message> result_of_creating_conf{ this->create_default_config(*multi_bytes_path_str) };

		if (result_of_creating_conf)
		{
			return result_of_creating_conf;
		}
	}

	if (node_name == disk_config::DISK || node_name == disk_config::PERSONAL || 
		node_name == disk_config::PUBLIC || node_name == disk_config::DOWNLOAD)
	{
		TiXmlDocument doc{};
		bool load_result{ doc.LoadFile(multi_bytes_path_str->c_str(), TIXML_ENCODING_UTF8) };
		TiXmlElement* root{ doc.FirstChildElement("disk_config") };

		if (!root)
		{
			return std::make_unique<protocal_message>(protocal_message{
				static_cast<unsigned int>(protocal_error_code::READ_DISK_CONFIG_FAILED),
				error_to_str[protocal_error_code::READ_DISK_CONFIG_FAILED],
				{ "" }
			});
		}

		std::basic_string<char> value{ get_value_in_node(root, node_name) };

		if (value.empty())
		{
			return std::make_unique<protocal_message>(protocal_message{
				static_cast<unsigned int>(protocal_error_code::READ_DISK_CONFIG_FAILED),
				error_to_str[protocal_error_code::READ_DISK_CONFIG_FAILED],
				{ "" }
			});
		}

		return std::make_unique<protocal_message>(protocal_message{
			0,
			"success",
			value
		});
	}

	return{ nullptr };
}

std::unique_ptr<protocal_message> disk_config::create_default_config(const std::basic_string<char>& xml_path) // path include file name.
{
	std::function<void(TiXmlElement* root_item,const std::basic_string<char>& item_name, const std::basic_string<char>& item_value)> add_item{
		[](TiXmlElement* root_item, const std::basic_string<char>& item_name, const std::basic_string<char>& item_value)
	{
		TiXmlElement* p_ele{ new TiXmlElement{item_name.c_str()}};
		p_ele->LinkEndChild(new TiXmlText{ item_value.c_str() });
		root_item->LinkEndChild(p_ele);
	}
	};

	TiXmlDocument doc(xml_path.c_str());
	TiXmlDeclaration* decl{ new TiXmlDeclaration{"1.0", "utf-8", ""} };
	doc.LinkEndChild(decl);
	TiXmlElement* root_ele{ new TiXmlElement{"disk_config"} };
	doc.LinkEndChild(root_ele);

	add_item(root_ele, disk_config::DISK, DEFAULT_DRIVER);
	add_item(root_ele, disk_config::PUBLIC, disk_config::PUBLIC);
	add_item(root_ele, disk_config::PERSONAL, disk_config::PERSONAL);
	add_item(root_ele, disk_config::DOWNLOAD, disk_config::DOWNLOAD);

	bool save_result{ doc.SaveFile(xml_path.c_str()) };

	if (!save_result)
	{
		return std::make_unique<protocal_message>(protocal_message{
			static_cast<unsigned int>(protocal_error_code::CREATE_DISK_CONFIG_FAILED),
			error_to_str[protocal_error_code::CREATE_DISK_CONFIG_FAILED],
			{ "" }
		});
	}

	return{ nullptr };
}
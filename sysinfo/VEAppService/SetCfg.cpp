#include "stdafx.h"
#include "SetCfg.h"
#include "../../../lib/tinyxml/tinyxml.h"

string GetApplicationDir()
{
	char file_path[260];
	GetModuleFileNameA(NULL, file_path, sizeof(file_path));
	PathRemoveFileSpecA(file_path);
	return file_path;
}

static bool add_item_helper(TiXmlElement*p_root_item, const char* p_item_name, const char* p_item_value)
{
	TiXmlElement* p_ele = new TiXmlElement(p_item_name);
	p_ele->LinkEndChild(new TiXmlText(p_item_value));
	p_root_item->LinkEndChild(p_ele);
	return true;
}

static void get_item_value_helper(TiXmlElement*p_root_item, const char* p_item_name, string& out_vaule)
{
	TiXmlElement* p_ele = p_root_item->FirstChildElement(p_item_name);
	if (p_ele)
	{
		const char* p_result = p_ele->GetText();
		if (p_result) out_vaule = p_result;
	}
}

SetCfg::SetCfg() :
	disk("e:\\"),
	shareFolder("public"),
	userDownload("download"),
	userPersonal("personal")
{
}

bool SetCfg::save()
{
	string xml_path = GetApplicationDir() + "\\disk_config.xml";

	TiXmlDocument doc(xml_path.c_str()); // 建立一个XML文件
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "utf-8", ""); // 声明XML的属性
	doc.LinkEndChild(decl);
	TiXmlElement* p_root_ele = new TiXmlElement("disk_config");
	doc.LinkEndChild(p_root_ele);

	char sz_num[64];
	add_item_helper(p_root_ele, "disk", disk.c_str());
	add_item_helper(p_root_ele, "public", shareFolder.c_str());
	add_item_helper(p_root_ele, "download", userDownload.c_str());
	add_item_helper(p_root_ele, "personal", userPersonal.c_str());
	doc.SaveFile(xml_path.c_str());
	return true;
}

bool SetCfg::load()
{
	string xml_path = GetApplicationDir() + "\\disk_config.xml";

	TiXmlDocument doc;
	if (doc.LoadFile(xml_path.c_str(), TIXML_ENCODING_UTF8))
	{
		TiXmlElement* p_root_ele = doc.FirstChildElement("disk_config");
		if (p_root_ele == NULL) return true;

		get_item_value_helper(p_root_ele, "disk", disk);
		get_item_value_helper(p_root_ele, "public", shareFolder);
		get_item_value_helper(p_root_ele, "download", userDownload);
		get_item_value_helper(p_root_ele, "personal", userPersonal);

		return true;
	}
	return false;
}

SetCfg* get_config()
{
	static SetCfg s_cfg;
	static bool s_b_initialed = false;
	if (!s_b_initialed)
	{
		s_b_initialed = true;
		s_cfg.load();
	}
	return &s_cfg;
}

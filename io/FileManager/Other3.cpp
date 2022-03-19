#include<iostream>  
#include<vector>  
#include <string>  
#include <io.h>  
#include <list>    
using namespace std;


vector<string> getFiles(const string &folder, const bool all /* = true */)
{
	vector<string> files;
	list<string> subfolders;
	subfolders.push_back(folder);

	while (!subfolders.empty())
	{
		string current_folder(subfolders.back());

		if (*(current_folder.end() - 1) != '/ ')
		{    
			//ȷ����ַ�� �� ��./*�� ��β��  
			current_folder.append("//*");
		}
		else
		{
			current_folder.append("*");
		}

		subfolders.pop_back();

		struct _finddata_t file_info;
		long file_handler = _findfirst(current_folder.c_str(), &file_info);

		while (file_handler != -1)
		{
			if (all &&
				(!strcmp(file_info.name, ".") || !strcmp(file_info.name, "..")))
			{
				if (_findnext(file_handler, &file_info) != 0) break;
				continue;
			}
#if 0
			б�� / �� ��б��\ ����������Ͼ�����Щ�ˣ�����������һ�����·���;���·����
				. / SRC / ����д��ʾ����ǰĿ¼�е�SRC�ļ��У�
				.. / SRC / ����д��ʾ����ǰĿ¼����һ��Ŀ¼��SRC�ļ��У�
				/ SRC / ����д��ʾ����Ŀ��Ŀ¼������ֻ���̸�Ŀ¼��Ҳ����ָ��Ŀ��Ŀ¼���������ʵ�����������
#endif

				if (file_info.attrib & _A_SUBDIR)
				{
					// it��s a sub folder
					if (all)
					{
						// will search sub folder
						string folder(current_folder);
						folder.pop_back(); 
						folder.append(file_info.name);

						subfolders.push_back(folder.c_str());
					}
				}
				else
				{
					// it��s a file
					string file_path;
					// current_folder.pop_back();
					file_path.assign(current_folder.c_str()).pop_back();
					file_path.append(file_info.name);

					files.push_back(file_path);
				}

			if (_findnext(file_handler, &file_info) != 0) break;
		}  // while
		_findclose(file_handler);
	}

	return files;
}


void main3()
{
	string path = "F: / test";      //ע�������ַ��  /                        \\
								   vector<string> out;

	vector<string> out = getFiles(path, true);

	for (int i = 0; i < out.size(); i++)
	{
		cout << out[i] << endl;
	}
}
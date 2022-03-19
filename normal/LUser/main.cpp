#include <iostream>
#include"UserManager.h"
#include<netdb.h>
using namespace std;

int main()
{
    auto vec = UserManager::GetUserList();
    cout << "Hello World!" << endl;
    auto gvec = UserManager::GetGroupList();
    printf("login user = %s \n",getlogin());
    return 0;
}

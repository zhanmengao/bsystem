#include <iostream>

using namespace std;

int main(int argc,char* argv[])
{
    cout << "Hello World!" << endl;
    for(int i = 0;i<argc;i++)
    {
        printf("[%d] = %s \n",i,argv[i]);
    }
    return 0;
}

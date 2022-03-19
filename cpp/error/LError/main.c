#include"Error.h"
int main()
{
#ifdef  _WIN32
    printf(ErrorMessage(5));
	system("pause");
#else
	PError(ErrorMessage(5));
#endif //  _WIN32
    return 0;
}

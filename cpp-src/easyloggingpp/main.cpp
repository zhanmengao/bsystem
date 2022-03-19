#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argv, char* argc[])
{
   LOG(INFO) << "My first info log using default logger";
   return 0;
}

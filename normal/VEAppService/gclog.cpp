#include "gclog.h"
#include <ctime>

void WriteLog(char *fmt, ...)
{
	FILE *fp;
	va_list args;

	time_t now = time(0);
	char* dt = ctime(&now);

	tm *ltm = localtime(&now);
	
	char logfilename[50] = { 0 };
	sprintf(logfilename,"VEAppService-%d%d%d.log", ltm->tm_year+1900, 1 + ltm->tm_mon, ltm->tm_mday );

	if ((fp = fopen( logfilename, "a")) != NULL)
	{		
		va_start(args, fmt);				
		
		vprintf(fmt, args);		
		fprintf(fp, dt);
		vfprintf(fp, fmt, args );
		
		fclose(fp);

		va_end(args);
	}
}
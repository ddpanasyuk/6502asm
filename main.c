#include "main.h"

int main(char argc, char** argv)
{
	if(argc > 1)
	{
		if(!open_file_to_tokenize(argv[1]))
		{
			printf("opened file %s\n", argv[1]);

			int err = begin();
			
			tok_cleanup();
			
			if(err)
			{
				printf("6502asm: fatal error, assembly aborted\n");
			}
		}
	}
	return 0;
}


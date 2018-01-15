#include "main.h"

int main(char argc, char** argv)
{
	if(argc > 2)
	{
		if(!open_file_to_tokenize(argv[1]))
		{
			printf("opened file %s\n", argv[1]);

			create_new_program();
			
			int err = begin();
			
			if(err)
			{
				printf("6502asm: fatal error, assembly aborted\n");
			}
			else
			{
				save_program_to_file(argv[2]);
			}
			
			tok_cleanup();
			cleanup_program();
		}
	}
	return 0;
}


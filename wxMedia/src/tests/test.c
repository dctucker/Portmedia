#include <iostream>

int main(int argc, char **argv)
{
	for( int i=1; i < argc; i++)
	{
		char *c = argv[i]; //"AmpR";
		int v = 0;
		v = *((int *)c);

		int g = c[0] | c[1] << 8 | c[2] << 16 | c[3] << 24;
		std::cout << "\t\tcase 0x" << std::hex << std::uppercase << g << ":  // " << c << "\n";
	}
	return 0;
}

#ifndef MAIN_H
#define MAIN_H

#define MIDIPORT			0
#define SAMPLE_RATE			96000
#define FRAMES_PER_BUFFER	768
#define DESIRED_LATENCY     0.008
#define TRYMIDI( XX ) try { XX; } catch( RtMidiError &error ){ error.printMessage(); exit( EXIT_FAILURE ); }

typedef unsigned int uint;
int prog_main();

#ifdef MSVC
	#define and   &&
	#define or    ||
	#define not   !
	#define compl ~
	#define asm(X) /*X*/
#endif

typedef float fl;

#endif

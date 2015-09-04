#define MIDIPORT			0
#define SAMPLE_RATE			96000
#define FRAMES_PER_BUFFER	768
#define DESIRED_LATENCY     0.004
#define TRYMIDI( XX ) try { XX; } catch( RtError &error ){ error.printMessage(); exit( EXIT_FAILURE ); }

typedef unsigned int uint;
int prog_main();

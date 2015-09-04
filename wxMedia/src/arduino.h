/* == Taken from: == 
 *
 * Arduino-serial
 * --------------
 * 
 * A simple command-line example program showing how a computer can
 * communicate with an Arduino board. Works on any POSIX system (Mac/Unix/PC) 
 *
 * Created 5 December 2006
 * Copyleft (c) 2006, Tod E. Kurt, tod@todbot.com
 * http://todbot.com/blog/
 */

//#include <stdio.h>    /* Standard input/output definitions */
//#include <stdlib.h> 
//#include <stdint.h>   /* Standard types */
//#include <string.h>   /* String function definitions */
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/ioctl.h>


class Arduino
{
private:
	int fd;
public:
	bool initialized;

	Arduino()
		: initialized(false)
	{
	}

	int WriteByte( uint8_t b)
	{
		if( !initialized ) return -1;
		int n = write(fd,&b,1);
		if( n!=1)
			return -1;
		return 0;
	}

	int Write( const char* str)
	{
		if( !initialized ) return -1;
		int len = strlen(str);
		int n = write(fd, str, len);
		if( n!=len ) 
			return -1;
		return 0;
	}

	int ReadUntil( char* buf, char until)
	{
		if( !initialized ) return -1;
		char b[1];
		int i=0;
		do { 
			int n = read(fd, b, 1);  // read a char at a time
			if( n==-1) return -1;    // couldn't read
			if( n==0 ) {
				usleep( 10 * 1000 ); // wait 10 msec try again
				continue;
			}
			buf[i] = b[0]; i++;
		} while( b[0] != until );

		buf[i] = 0;  // null terminate the string
		return 0;
	}

	// takes the string name of the serial port (e.g. "/dev/tty.usbserial","COM1")
	// and a baud rate (bps) and connects to that port at that speed and 8N1.
	// opens the port in fully raw mode so you can send binary data.
	// returns valid fd, or -1 on error
	void Init(const char* serialport, int baud)
	{
		struct termios toptions;
		fd = -1;

		fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
		if (fd == -1)  {
			perror("init_serialport: Unable to open port ");
			return;
		}
		
		if (tcgetattr(fd, &toptions) < 0) {
			perror("init_serialport: Couldn't get term attributes");
			fd = -1;
			return;
		}
		speed_t brate = B115200;
		cfsetispeed(&toptions, brate);
		cfsetospeed(&toptions, brate);

		// 8N1
		toptions.c_cflag &= ~PARENB;
		toptions.c_cflag &= ~CSTOPB;
		toptions.c_cflag &= ~CSIZE;
		toptions.c_cflag |= CS8;
		// no flow control
		toptions.c_cflag &= ~CRTSCTS;

		toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
		toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl
		toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
		toptions.c_oflag &= ~OPOST; // make raw

		// see: http://unixwiz.net/techtips/termios-vmin-vtime.html
		toptions.c_cc[VMIN]  = 0;
		toptions.c_cc[VTIME] = 20;
		
		if( tcsetattr(fd, TCSANOW, &toptions) < 0) {
			perror("init_serialport: Couldn't set term attributes");
			fd = -1;
			return;
		}
		initialized = true;
	}
};

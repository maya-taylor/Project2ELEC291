/////////////////////////////////////////////////////////////////////////////
//
//  efm8p.c:  Serial flash loader for the EFM8LB1 family of microcontrollers.
//            for Linux and macOS.  There is also a version of this program for
//            Windows called efm8_prog.exe.
// 
// Copyright (C) 2013-2021  Jesus Calvino-Fraga, jesusc (at) ece.ubc.ca
// 
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option) any
// later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
/////////////////////////////////////////////////////////////////////////////
//
// To compile:
// gcc efm8p.c -o efm8p

#include <limits.h>
#ifdef __APPLE__
	#include <termios.h>
#else
	#include <termio.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define strnicmp strncasecmp 
#define TRUE true
#define FALSE false
#define EQ(X,Y)  (stricmp(X, Y)==0)
#define NEQ(X,Y) (stricmp(X, Y)!=0)
#define MAX_PATH PATH_MAX
#define DWORD unsigned long int
#define stricmp strcasecmp
#define BOOL bool

clock_t startm, stopm;
#define START if ( (startm = clock()) == -1) {printf("Error calling clock");}
#define STOP if ( (stopm = clock()) == -1) {printf("Error calling clock");}
#define PRINTTIME printf( "%.3f seconds.", ((double)stopm-startm)/CLOCKS_PER_SEC);

#define EFM8LB_64k_MEMSIZE (0xFA00)
#define EFM8LB_32k_MEMSIZE (0x8000)
#define EFM8LB_16k_MEMSIZE (0x4000)

struct _id {
   unsigned char device_id;
   unsigned char derivative_id;
   char * description; // The Flash size is embedded in the description string: F16, F32, or F64
};

struct _id id[]=
{
	{0x34, 0x41, "EFM8LB12F64E_QFN32"},
	{0x34, 0x42, "EFM8LB12F64E_QFP32"},
	{0x34, 0x43, "EFM8LB12F64E_QSOP24"},
	{0x34, 0x44, "EFM8LB12F64E_QFN24"},
	{0x34, 0x45, "EFM8LB12F32E_QFN32"},
	{0x34, 0x46, "EFM8LB12F32E_QFP32"},
	{0x34, 0x47, "EFM8LB12F32E_QSOP24"},
	{0x34, 0x48, "EFM8LB12F32E_QFN24"},
	{0x34, 0x49, "EFM8LB11F32E_QFN32"},
	{0x34, 0x4A, "EFM8LB11F32E_QFP32"},
	{0x34, 0x4B, "EFM8LB11F32E_QSOP24"},
	{0x34, 0x4C, "EFM8LB11F32E_QFN24"},
	{0x34, 0x4D, "EFM8LB11F16E_QFN32"},
	{0x34, 0x4E, "EFM8LB11F16E_QFP32"},
	{0x34, 0x4F, "EFM8LB11F16E_QSOP24"},
	{0x34, 0x50, "EFM8LB11F16E_QFN24"},
	{0x34, 0x51, "EFM8LB10F16E_QFN32"},
	{0x34, 0x52, "EFM8LB10F16E_QFP32"},
	{0x34, 0x53, "EFM8LB10F16E_QSOP24"},
	{0x34, 0x54, "EFM8LB10F16E_QFN24"},
	{0x34, 0x61, "EFM8LB12F64ES0_QFN32"},
	{0x34, 0x64, "EFM8LB12F64ES0_QFN24"},
	{0x34, 0x65, "EFM8LB12F32ES0_QFN32"},
	{0x34, 0x68, "EFM8LB12F32ES0_QFN24"},
	{0x34, 0x69, "EFM8LB11F32ES0_QFN32"},
	{0x34, 0x6C, "EFM8LB11F32ES0_QFN24"},
	{0x34, 0x6D, "EFM8LB11F16ES0_QFN32"},
	{0x34, 0x70, "EFM8LB11F16ES0_QFN24"},
	{0x34, 0x71, "EFM8LB10F16ES0_QFN32"},
	{0x34, 0x74, "EFM8LB10F16ES0_QFN24"},
	{0x00, 0x00, ""}
};

int fd;
char SerialPort[MAX_PATH]="/dev/ttyUSB0";
struct termios comio;

unsigned char FlashBuff[EFM8LB_64k_MEMSIZE];
char m_FileName[MAX_PATH]="";
char m_Serial[0x100]="";
int m_memsize=0;
int m_timeout=15;
int m_reset=0;
int Selected_Device=-1;

int Select_Baud (int Baud_Rate)
{
	switch (Baud_Rate)
	{
		case 115200: return B115200;
		case 57600:  return B57600;
		case 38400:  return B38400;
		case 19200:  return B19200;
		case 9600:   return B9600;
		case 4800:   return B4800;
		case 2400:   return B2400;
		case 1800:   return B1800;
		case 1200:   return B1200;
		default:     return B4800;
	}
}

//CSIZE  Character size mask.  Values are CS5, CS6, CS7, or CS8.
//CSTOPB Set two stop bits, rather than one.
//PARENB Enable parity generation on output and parity checking for input.
//PARODD If set, then parity for input and output is odd; otherwise even parity is used.
#define ONESTOPBIT 0 
#define TWOSTOPBITS CSTOPB 
#define NOPARITY 0

int OpenSerialPort(char * devicename, int baud, int parity, int numbits, int numstop)
{
    struct termios options;
	speed_t BAUD;
	
	BAUD=Select_Baud(baud);
	
	//open the device(com port) to be non-blocking (read will return immediately)
	fd = open(devicename, O_RDWR | O_NOCTTY | O_NDELAY );
	if (fd < 0)
	{
		perror(devicename);
		return(1);
	}
	
	/*Reading data from a port is a little trickier. When you operate the port
	in raw data mode, each read system call will return however many characters
	are actually available in the serial input buffers. If no characters are
	available, the call will block (wait) until characters come in, an interval
	timer expires, or an error occurs. The read function can be made to return
	immediately by doing the following:*/	

	/* The FNDELAY option causes the read function to return 0 if no characters
	are available on the port. To restore normal (blocking) behavior, call
	fcntl() without the FNDELAY option:*/
	
 	//fcntl(fd, F_SETFL, FNDELAY);
    fcntl(fd, F_SETFL, 0);
	
	/*This is also used after opening a serial port with the O_NDELAY option.*/

	// Make the file descriptor asynchronous 
	
	tcgetattr(fd, &comio);
	
	//newtio.sg_ispeed = newtio.sg_ospeed = BAUD;
	cfsetospeed(&comio, (speed_t)BAUD);
	cfsetispeed(&comio, (speed_t)BAUD);
	
	comio.c_cflag = BAUD | CS8 | CSTOPB | CLOCAL | CREAD;
	comio.c_cflag &= ~(CRTSCTS); /* No hardware flow control */
	comio.c_iflag = IGNPAR;
	comio.c_iflag &= ~(IXON | IXOFF | IXANY); /* No software flow control */
	comio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); /* Raw input mode*/
	comio.c_oflag &= ~OPOST; /* Raw ouput mode */
	comio.c_cc[VMIN]=0;
	comio.c_cc[VTIME]=1;
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &comio);

	return(0);
}

void CloseSerialPort(void)
{
	close(fd);
}

void Sleep (int msec)
{
	struct timespec req;

	req.tv_sec=0;
	req.tv_nsec=1000000L*msec;
    nanosleep (&req, NULL);
}

void Delay (int usec)
{
	struct timespec req;

	req.tv_sec=0;
	req.tv_nsec=1000L*usec;
    nanosleep (&req, NULL);
}

// Functions to read write hex/bin files
int hex2dec (char hex_digit)
{
   int j;
   j=toupper(hex_digit)-'0';
   if (j>9) j -= 7;
   return j;
}

unsigned char GetByte(char * buffer)
{
	return hex2dec(buffer[0])*0x10+hex2dec(buffer[1]);
}

unsigned short GetWord(char * buffer)
{
	return	hex2dec(buffer[0])*0x1000+
			hex2dec(buffer[1])*0x100+
			hex2dec(buffer[2])*0x10+
			hex2dec(buffer[3]);
}

long filesize(FILE *stream)
{
   long curpos, length;

   curpos = ftell(stream);
   fseek(stream, 0L, SEEK_END);
   length = ftell(stream);
   fseek(stream, curpos, SEEK_SET);
   return length;
}

int ReadHexFile(char * filename)
{
	char buffer[1024];
	FILE * filein;
	int j, numbytes;
	unsigned char linesize, recordtype, rchksum, value;
	unsigned short address;
	int MaxAddress=0;
	int chksum;
	int line_counter=0;
	int numread=0;
	int LoadedBytes=0;

	//Set the buffer to its default value
	for(j=0; j<EFM8LB_64k_MEMSIZE; j++) FlashBuff[j]=0xff;

    if ( (filein=fopen(filename, "r")) == NULL )
    {
       printf("Error: Can't open file `%s`.\r\n", filename);
       fflush(stdout);
       return -1;
    }
    
	numbytes=filesize(filein);

    while(fgets(buffer, sizeof(buffer), filein)!=NULL)
    {
    	numread+=(strlen(buffer)+1);

    	line_counter++;
    	if(buffer[0]==':')
    	{
			linesize = GetByte(&buffer[1]);
			address = GetWord(&buffer[3]);
			recordtype = GetByte(&buffer[7]);
			rchksum = GetByte(&buffer[9]+(linesize*2));
			chksum=linesize+(address/0x100)+(address%0x100)+recordtype+rchksum;

			if (recordtype==1) break; /*End of record*/

			for(j=0; j<linesize; j++)
			{
				value=GetByte(&buffer[9]+(j*2));
				chksum+=value;
				if((address+j)<EFM8LB_64k_MEMSIZE)
				{
					FlashBuff[address+j]=value;
					LoadedBytes++;
				}
			}
			if(MaxAddress<(address+linesize-1)) MaxAddress=(address+linesize-1);

			if((chksum%0x100)!=0)
			{
				printf("ERROR: Bad checksum in file '%s' at line %d\r\n", filename, line_counter);
				fflush(stdout);
				return -1;
			}
		}
    }
    fclose(filein);
	
	printf("%s: loaded %d bytes\n", filename, LoadedBytes); 

    return MaxAddress;
}

BOOL PortWrRd(char x, DWORD delay)
{
	DWORD j;
    char s=0;
    
    j=write(fd, &x, 1);
    Sleep(delay+1);
	j = read(fd, &s, 1);

	return (s==x)?TRUE:FALSE;
}

BOOL Identify (void)
{
	DWORD j;
    int i;
	unsigned char x[100];
    unsigned char c;
	    
	// Autobaud first
	x[0]=0xff;
    j=write(fd, x, 1);  // For Unix/Apple
	Sleep(5);
	
	for(i=0; id[i].device_id!=0; i++)
	{
		x[0]='$';
		x[1]=0x03;
		x[2]=0x30;
		x[3]=id[i].device_id;
		x[4]=id[i].derivative_id;

    	j=write(fd, x, 5);
		Sleep(1);
		
		c=0;
		j = read(fd, &c, 1); // For Unix/Apple

	    if(j==1)
		{
			if(c==0x40)
			{
				printf("Found %s.  Id: 0x%02x%02x\n", id[i].description, id[i].device_id, id[i].derivative_id);
				fflush(stdout);
				     if (strstr(id[i].description, "F64")!=NULL) m_memsize=EFM8LB_64k_MEMSIZE;
				else if (strstr(id[i].description, "F32")!=NULL) m_memsize=EFM8LB_32k_MEMSIZE;
				else if (strstr(id[i].description, "F16")!=NULL) m_memsize=EFM8LB_16k_MEMSIZE;
				else m_memsize=0;
				return TRUE;
			}
			else
			{
				printf(".");
				fflush(stdout);
			}
		}
		else
		{
			printf("x");
			fflush(stdout);
		}
	}
	if(c!=0x40)
	{
		printf("\nCould not get an ID to work.\n");
		fflush(stdout);
	}
	return FALSE;
}

unsigned char Send_EFM8LB_Record(unsigned char * x, int sleeptime)
{
	DWORD j;
    unsigned char c=0;

    j=write(fd, x, x[1]+2);
	if (j!=x[1]+2)
	{
		printf("Failed in call to write()\n");
		fflush(stdout);
		return 0;
	}
	Sleep(sleeptime);
	j = read(fd, &c, 1);
	if (j!=1)
	{
		printf("Failed in call to read()\n");
		fflush(stdout);
		return 0;
	}

	return c;
}

unsigned char Write_EFM8LB_Flash(int address, int len, int * hashcnt) 
{
	int j;
	unsigned char buff[0x100];
	unsigned char empty=1;

	buff[0]='$';
	buff[1]=len+3;
	buff[2]=0x33;
	buff[3]=address/0x100;
	buff[4]=address%0x100;

	for (j=0; j<len; j++)
	{
		buff[j+5]=FlashBuff[address+j];
		if (FlashBuff[address+j]!=0xff) empty=0; // Don't send empty lines
	}

	if(empty==0)
	{
		printf("#"); fflush(stdout);
		(*hashcnt)++;
		if(*hashcnt==60)
		{
			*hashcnt=0;
			printf("\n"); fflush(stdout);
		}
	}
	
	return empty?0x40:Send_EFM8LB_Record(buff, 0);
}

unsigned char Erase_EFM8LB_Flash(void) 
{
	unsigned int address;
	unsigned char buff[0x10];
	unsigned char toret=0, hashcnt;

	for(address=0, hashcnt=0; address<m_memsize; address+=0x200)
	{
    	printf("#"); fflush(stdout);
    	hashcnt++;
    	if(hashcnt==60)
    	{
    		hashcnt=0;
    		printf("\n"); fflush(stdout);
    	}
		buff[0]='$';
		buff[1]=0x03;
		buff[2]=0x32;
		buff[3]=address/0x100;
		buff[4]=address%0x100;
		toret=Send_EFM8LB_Record(buff, 0);
		if(toret!=0x40) return toret;
	}
	return toret;
}

unsigned char Setup_EFM8LB(void)
{
	unsigned char buff[0x10];
	
	// 24 04 31 A5 F1 00 
	buff[0]='$';
	buff[1]=0x04;
	buff[2]=0x31;
	buff[3]=0xA5;
	buff[4]=0xF1;
	buff[5]=0x00;
	return Send_EFM8LB_Record(buff, 0);
}

// CRC-CCITT (XModem) Polynomial: x^16 + x^12 + x^5 + 1 (0x1021). Initial crc must be 0x0000.
unsigned int crc16 (unsigned int crc, unsigned char val)
{
	unsigned char i;
	crc = crc ^ ((unsigned int)val << 8);
	for (i=0; i<8; i++)
	{
		if (crc & 0x8000)
		{
			crc = (crc << 1) ^ 0x1021;
		}
		else
		{
			crc <<= 1;
		}
	}
	return crc;
}

unsigned char Check_EFM8LB_Flash(void) 
{
	unsigned int j;
	unsigned char buff[0x10];
	unsigned int crc=0;
	unsigned char c;

	printf("Verifying... ");
	
	for(j=0; j<m_memsize; j++)
	{
		crc=crc16(crc, FlashBuff[j]);
	}

	buff[0]='$';
	buff[1]=0x07;
	buff[2]=0x34;
	buff[3]=0x00;
	buff[4]=0x00;
	buff[5]=(m_memsize-1)/0x100;
	buff[6]=(m_memsize-1)%0x100;
	buff[7]=crc/0x100;
	buff[8]=crc%0x100;
	c=Send_EFM8LB_Record(buff, 0);
	if(c==0x43)
	{
		printf(" ERROR!  The memory was not loaded correctly.\n");
	}
	else
	{
		printf(" Done.\n");
	}
	fflush(stdout);
	return c;
}

void Reset_EFM8LB(void) 
{
	unsigned char buff[0x10];

	printf("Running program... ");
	
	buff[0]='$';
	buff[1]=0x01;
	buff[2]=0x36;
	Send_EFM8LB_Record(buff, 0);
	printf(" Done.\n");
	fflush(stdout);
}

void GetOut (int val)
{
	exit(val);
}

void Flash(void) 
{
    int portn;
	char buff[0x100];
	int j, hashcnt;

	if(ReadHexFile(m_FileName)<0)
    {
        printf("\nERROR: Could not load hex file '%s'.\n", m_FileName);
        fflush(stdout);
        return;
    }
    
    if(OpenSerialPort(m_Serial, 115200, NOPARITY, 8, ONESTOPBIT)!=0)
    {
        printf("ERROR: Could not open serial port '%s'.\n", m_Serial);
        fflush(stdout);
        return;
    }

	START; // Measure the time it takes to program the microcontroller
	
	//Identify the microcontroller
	if(Identify()==FALSE)
	{
		printf("ERROR: Part identification command failed.\n");
		fflush(stdout);
		goto The_end;
	}
		
	printf("Sending 'setup' command... "); fflush(stdout);
	if(Setup_EFM8LB()!=0x40)
	{
		printf("\nERROR: Setup command failed.\n");
		fflush(stdout);
		goto The_end;
	}

	printf("Done.\nErasing flash memory...\n"); fflush(stdout);
	if(Erase_EFM8LB_Flash()!=0x40)
	{
		printf("\nERROR: Flash erase command failed.\n");
		fflush(stdout);
		goto The_end;
	}

	printf(" Done.\nWriting flash memory...\n"); fflush(stdout);
	for(j=0, hashcnt=0; j<m_memsize; j+=128)
    {
		if(Write_EFM8LB_Flash(j, 128, &hashcnt)!=0x40)
		{
            printf("ERROR: Flash write command failed.\n");
            fflush(stdout);
            goto The_end;
		}
	}
	
    printf(" Done.\n");
	Check_EFM8LB_Flash();
	
	if (m_reset==1) Reset_EFM8LB();
	
    printf("Actions completed in ");
    STOP;
	PRINTTIME;
	printf("\n");
	fflush(stdout);
	    
The_end:
    CloseSerialPort();
}

void print_help (char * prn)
{
	printf("Usage example:\n"
	       "%s -p/dev/cu.usbserial-DN05FVT8 somefile.hex (load flash memory with file 'somefile.hex' via /dev/cu.usbserial-DN05FVT8)\n"
	       "Options available:\n"
	       "   -p[portname] (use this serial port)\n"
	       "   -txx (boot loader activation timeout is xx seconds)\n"
	       "   -r  (reset processor after loading flash)\n"
	       "   -h (this help)\n"
		   "To check what serial ports are available you can use this command on a terminal:\n"
		   "ls -l /dev | grep \"usb\"\n"
	       , prn);
	fflush(stdout);
}

int main(int argc, char **argv)
{
	int j;
	
	unsigned char buff[32];
	BOOL use_ft230=FALSE;

    printf("Serial flash programmer for the EFM8LB1. (C) Jesus Calvino-Fraga (2012-2021)\n");
    fflush(stdout);
	
    for(j=1; j<argc; j++)
    {
    		 if(EQ("-h", argv[j])) {print_help(argv[0]); return 0;}
		else if(strnicmp("-p", argv[j], 2)==0) sprintf(m_Serial, "%s", &argv[j][2]);
		else if(strnicmp("-t", argv[j], 2)==0) m_timeout=atoi(&argv[j][2]);
		else if(EQ("-r", argv[j])) m_reset=1;
		else if(strnicmp("-d", argv[j], 2)==0) Selected_Device=atoi(&argv[j][2]);
    	else strcpy(m_FileName, argv[j]);
    }

    if(strlen(m_FileName)==0) {print_help(argv[0]); return 1;}
    if(strlen(m_Serial)==0) {print_help(argv[0]); return 1;}

    Flash();
    return 0;
}

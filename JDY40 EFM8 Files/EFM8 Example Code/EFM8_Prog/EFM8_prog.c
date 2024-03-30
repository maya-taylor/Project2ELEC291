/////////////////////////////////////////////////////////////////////////////
//
// EFM8_prog.c:  Serial flash loader for the EFM8LB1 family of microcontrollers.
// 
// Copyright (C) 2013-2022  Jesus Calvino-Fraga, jesusc (at) ece.ubc.ca
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
// To compile on Linux and macOS:
//
// gcc EFM8_prog.c -o EFM8_prog
//
// To compile on Windows with Visual C:
//
// cl EFM8_prog.c
//
/////////////////////////////////////////////////////////////////////////////

#ifdef __APPLE__
	#define __unix__ 1
#endif

#ifdef __unix__
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
	
#else
	#include <stdio.h>
	#include <stdlib.h>
	#include <conio.h>
	#include <windows.h>
	#include <time.h>
	#pragma comment(lib, "FTD2XX.lib")
	#define FTD2XX_STATIC
	#include "ftd2xx.h"
	#define EQ(X,Y)  (_stricmp(X, Y)==0)
	#define NEQ(X,Y) (_stricmp(X, Y)!=0)
#endif

time_t start_t, stop_t;
#define START time(&start_t);
#define STOP time(&stop_t);
#define PRINTTIME printf( "%d seconds.", (int)(stop_t-start_t));

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

#ifdef __unix__
	#define MAX_PATH PATH_MAX
	#define DWORD unsigned long int
	#define stricmp strcasecmp
	#define BOOL bool
	int fd;
	char SerialPort[MAX_PATH]="/dev/ttyUSB0";
	struct termios comio;
#else
	HANDLE hComm=INVALID_HANDLE_VALUE;
	int SerialPort=1;
	BOOL b_default_CBUS=FALSE;
	int Selected_Device=-1;
	FT_HANDLE handle;
#endif

unsigned char FlashBuff[EFM8LB_64k_MEMSIZE];
char m_FileName[MAX_PATH]="";
char m_Serial[0x100]="";
int m_memsize=0;
int m_timeout=15;
int m_reset=0;

#ifdef __unix__

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
	
	//fcntl(fd, F_SETFL, FNDELAY);

	/* The FNDELAY option causes the read function to return 0 if no characters
	are available on the port. To restore normal (blocking) behavior, call
	fcntl() without the FNDELAY option:*/
	
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

#else

int OpenSerialPort (char * Port, DWORD baud, BYTE parity, BYTE bits, BYTE stop)
{
	char sPort[16];
	DCB dcb;
	COMMTIMEOUTS Timeouts;

	sprintf(sPort, "\\\\.\\%s", Port);
	
	hComm = CreateFile(sPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hComm == INVALID_HANDLE_VALUE)
	{
		printf("Failed to open up the comunication port %s\n", Port);
		return -1;
	}
	
	if (!GetCommState(hComm, &dcb))
	{
		printf("Failed in call to GetCommState()\n");
		return -1;
	}

	dcb.fAbortOnError=FALSE;
	dcb.BaudRate = baud; 
	dcb.Parity = parity;
	dcb.ByteSize = bits;
	dcb.StopBits = stop;
	dcb.fDsrSensitivity = FALSE;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;

	//Now that we have all the settings in place, make the changes
	if (!SetCommState(hComm, &dcb))
	{
		printf("Failed in call to SetCommState()\n");
		return -1;
	}	

	ZeroMemory(&Timeouts, sizeof(COMMTIMEOUTS));
	Timeouts.ReadIntervalTimeout = 250;
	Timeouts.ReadTotalTimeoutMultiplier = 10;
	Timeouts.ReadTotalTimeoutConstant = 250;
	Timeouts.WriteTotalTimeoutMultiplier = 10;
	Timeouts.WriteTotalTimeoutConstant = 250;
	if (!SetCommTimeouts(hComm, &Timeouts))
	{
		printf("Failed in call to SetCommTimeouts\n");
		return -1;
	}

	return 0;
}

int CloseSerialPort (void)
{
	BOOL bSuccess;
	bSuccess = CloseHandle(hComm);
    hComm = INVALID_HANDLE_VALUE;
    if (!bSuccess)
    {
		printf("Failed to close serial port, GetLastError:%d\n", GetLastError());
		return -1;
	}
	return 0;
}

#endif

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
    
#ifdef __unix__
    j=write(fd, &x, 1);  // For Unix/macOS
#else
	FlushFileBuffers(hComm); // Not in the original program!
    WriteFile(hComm, &x, 1, &j, NULL);
#endif

    Sleep(delay+1);
    
#ifdef __unix__
	j = read(fd, &s, 1); // For Unix/macOS
#else
    ReadFile(hComm, &s, 1, &j, NULL);
#endif

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
#ifdef __unix__
    j=write(fd, x, 1);  // For Unix/macOS
#else
	FlushFileBuffers(hComm);
	WriteFile(hComm, x, 1, &j, NULL);
#endif
	Sleep(5);
	
	for(i=0; id[i].device_id!=0; i++)
	{
		x[0]='$';
		x[1]=0x03;
		x[2]=0x30;
		x[3]=id[i].device_id;
		x[4]=id[i].derivative_id;
#ifdef __unix__
    	j=write(fd, x, 5);
#else
		FlushFileBuffers(hComm);
		WriteFile(hComm, x, 5, &j, NULL);
#endif
		Sleep(1);
		
		c=0;
#ifdef __unix__
		j = read(fd, &c, 1); // For Unix/macOS
#else
    	ReadFile(hComm, &c, 1, &j, NULL);
#endif
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

#ifdef __unix__
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
#else
	FlushFileBuffers(hComm);
	if (!WriteFile(hComm, x, x[1]+2, &j, NULL))
	{
		printf("Failed in call to WriteFile()\n");
		fflush(stdout);
		return 0;
	}
	Sleep(sleeptime);
	if (!ReadFile(hComm, &c, 1, &j, NULL))
	{
		printf("Failed in call to ReadFile()\n");
		fflush(stdout);
		return 0;
	}
#endif
	
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
#ifndef __unix__
	if(handle!=NULL)
	{
		FT_SetBitMode(handle, 0x00, FT_BITMODE_CBUS_BITBANG); // Set CBUS3 and CBUS0 as input
		FT_SetBitMode(handle, 0x0, FT_BITMODE_RESET); // Back to serial port mode
		FT_Close(handle);
	}
#endif
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

#ifndef __unix__
int List_FTDI_Devices (void)
{
	FT_STATUS ftStatus;
	FT_HANDLE ftHandleTemp;
	DWORD numDevs;
	DWORD Flags;
	DWORD ID;
	DWORD Type;
	DWORD LocId;
	char SerialNumber[16];
	char Description[64];
	int j, toreturn=0;
	LONG PortNumber;
	
	if (Selected_Device>=0) return Selected_Device;
	
	// create the device information list
	ftStatus = FT_CreateDeviceInfoList(&numDevs);
	if (ftStatus == FT_OK)
	{
		//printf("Number of devices is %d\n",numDevs);
	}
	
	if (numDevs > 1)
	{
		printf("More than one device detected.  Use option -d to select device to use:\n");
		for(j=0; j<numDevs; j++)
		{
			ftStatus = FT_GetDeviceInfoDetail(j, &Flags, &Type, &ID, &LocId, SerialNumber, Description, &ftHandleTemp);
			if (ftStatus == FT_OK)
			{
				printf("-d%d: ", j);
				//printf("Flags=0x%x ",Flags);
				//printf("Type=0x%x ",Type);
				printf("ID=0x%x ",ID);
				//printf("LocId=0x%x ",LocId);
				printf("Serial=%s ",SerialNumber);
				printf("Description='%s' ",Description);
				//printf(" ftHandle=0x%x",ftHandleTemp);
				FT_Open(j, &handle);
				FT_GetComPortNumber(handle, &PortNumber);				
				FT_Close(handle);
				printf("Port=COM%d\n", PortNumber); fflush(stdout);
			}
		}
		fflush(stdout);
		exit(-1);
	}
	
	return toreturn;
}

// CBUS3 and CBUs0 are used as RESET and C2D respectevely, to activate bootload mode.
// They must be configured for GPIO first.
void FTDI_Set_CBUS_Mode (int pinmode)
{
	FT_STATUS status;
	char Manufacturer[64];
	char ManufacturerId[64];
	char Description[64];
	char SerialNumber[64];
	int j;
	
	FT_EEPROM_HEADER ft_eeprom_header;
	FT_EEPROM_X_SERIES ft_eeprom_x_series;
	
	ft_eeprom_header.deviceType = FT_DEVICE_X_SERIES; // FTxxxx device type to be accessed
	ft_eeprom_x_series.common = ft_eeprom_header;
	ft_eeprom_x_series.common.deviceType = FT_DEVICE_X_SERIES;
	
	status = FT_EEPROM_Read(handle, &ft_eeprom_x_series, sizeof(ft_eeprom_x_series),
							Manufacturer, ManufacturerId, Description, SerialNumber);
	// FT_X_SERIES_CBUS_IOMODE configure pin to this mode for bit bang mode
	// FT_X_SERIES_CBUS_SLEEP Factory default setting for CBUS3
	// FT_X_SERIES_CBUS_TXLED Factory default setting for CBUS2
	// FT_X_SERIES_CBUS_RXLED Factory default setting for CBUS1
	// FT_X_SERIES_CBUS_TXDEN Factory default setting for CBUS0
	if (status == FT_OK)
	{
		if(pinmode==1)
		{
			if ((ft_eeprom_x_series.Cbus3!=FT_X_SERIES_CBUS_IOMODE) ||  (ft_eeprom_x_series.Cbus0!=FT_X_SERIES_CBUS_IOMODE))
			{
				ft_eeprom_x_series.Cbus3=FT_X_SERIES_CBUS_IOMODE;
				ft_eeprom_x_series.Cbus0=FT_X_SERIES_CBUS_IOMODE;
				status = FT_EEPROM_Program(handle, &ft_eeprom_x_series, sizeof(ft_eeprom_x_series),
										Manufacturer, ManufacturerId, Description, SerialNumber);
				if (status == FT_OK)
				{
					FT_CyclePort(handle); // It takes about 5 seconds to cycle port
					FT_Close(handle);
					printf("Pins CBUS3 and CBUS0 configured as 'IO Mode for CBUS bit-bang'.\n");					
					fflush(stdout);
					
					for(j=0; j<20; j++)
					{
						if(FT_Open(List_FTDI_Devices(), &handle) != FT_OK)
						{
							Sleep(1000);
							printf(".");
							fflush(stdout);
						}
						else
						{
							printf("\n");
							fflush(stdout);
							break;
						}
					}
					if(j==20)
					{
				        printf("Can not open FTDI adapter.\n");
						fflush(stdout);
						exit(1);
					}    
				}
			}
		}
		else
		{
			if ((ft_eeprom_x_series.Cbus3!=FT_X_SERIES_CBUS_SLEEP) || (ft_eeprom_x_series.Cbus0!=FT_X_SERIES_CBUS_TXDEN))
			{
				ft_eeprom_x_series.Cbus3=FT_X_SERIES_CBUS_SLEEP;
				ft_eeprom_x_series.Cbus0=FT_X_SERIES_CBUS_TXDEN;
				status = FT_EEPROM_Program(handle, &ft_eeprom_x_series, sizeof(ft_eeprom_x_series),
										Manufacturer, ManufacturerId, Description, SerialNumber);
				if (status == FT_OK)
				{
					FT_ResetDevice(handle);
					Sleep(100);
					printf("WARNING: Pins CBUS3 and CBUS0 have been configured as factory default.\n");
					printf("Please unplug/plug the BO230XS board for the changes to take effect\n"
					       "and try again.\n");
					fflush(stdout);
					GetOut(0);
				}
			}
		}
	}
}

void Activate_Bootloader (void)
{
	// Bits 7 down to 4 set CBUSx as either input or ouput.  Bits 3 down to 0 is the actual output.
	// For example 0x90 configures CBUS3 and CBUS0 as outputs and set their values to zero.
	unsigned char Seq[3]={0x90, 0x98, 0x99};
	unsigned char c;

    FT_SetBaudRate(handle, 115200);
	
	FTDI_Set_CBUS_Mode(1); // Make sure both CBUS3 and CBUS0 are in "CBUS IOMODE"
	
	for(c=0; c<3; c++)
	{
		FT_SetBitMode(handle, Seq[c], FT_BITMODE_CBUS_BITBANG); // RESET=0, TEST=0
		Sleep(10); // Sleep mili seconds
	}
	
	FT_Purge(handle, FT_PURGE_RX | FT_PURGE_TX);
	FT_SetBitMode(handle, 0x00, FT_BITMODE_CBUS_BITBANG); // Set CBUS3 and CBUS0 as input
	FT_SetBitMode(handle, 0x0, FT_BITMODE_RESET); // Back to serial port mode

	Sleep(10);
}
#endif

void print_help (char * prn)
{
	printf("Usage example:\n"
#ifdef __unix__
	#ifdef __APPLE__
	       "%s -p/dev/cu.usbserial-DN05FVT8 somefile.hex\n"
	#else
	       "%s -p/dev/ttyUSB0 somefile.hex\n"
	#endif
#else
	       "%s -pCOM3 somefile.hex\n"
#endif
	       "Options available:\n"
	       "   -p[portname] (use this serial port)\n"
	       "   -txx (boot loader activation timeout is xx seconds)\n"
	       "   -r  (reset processor after loading flash)\n"
#ifndef __unix__
	       "   -f or -ft230 (activate bootloader using FT230 pins CBUS0 and CBUS3)\n"
	       "   -cbus  (restore default configuration for CBUS0 and CBUS3)\n"
	       "   -dx (select FT230XS device x, for example for device 1: -d1)\n"
#endif
	       "   -h (this help)\n"
#ifdef __unix__
	#ifdef __APPLE__
		   "To check what serial ports are available on a Mac terminal type:\n"
		   "ls -l /dev | grep \"usb\"\n"
	#else
		   "To check what serial ports are available on a Linux terminal type:\n"
		   "ls -l /dev | grep \"USB\"\n"
		   "On linux, 'user' must be a member of group 'dialout' to use serial ports.\n"
	#endif
#endif
	       , prn);
	fflush(stdout);
}

int main(int argc, char **argv)
{
	int j;
	
	unsigned char buff[32];
	BOOL use_ft230=FALSE;

    printf("Serial flash programmer for the EFM8LB1. (C) Jesus Calvino-Fraga (2012-2022)\n");
    fflush(stdout);
	
    for(j=1; j<argc; j++)
    {
    		 if(EQ("-h", argv[j])) {print_help(argv[0]); return 0;}
		else if(strnicmp("-p", argv[j], 2)==0) sprintf(m_Serial, "%s", &argv[j][2]);
		else if(strnicmp("-t", argv[j], 2)==0) m_timeout=atoi(&argv[j][2]);
		else if(EQ("-r", argv[j])) m_reset=1;
#ifndef __unix__
		else if(strnicmp("-d", argv[j], 2)==0) Selected_Device=atoi(&argv[j][2]);
		else if(stricmp("-cbus", argv[j])==0) b_default_CBUS=TRUE;
		else if(stricmp("-ft230", argv[j])==0) use_ft230=TRUE;
		else if(stricmp("-f", argv[j])==0) use_ft230=TRUE;
#endif
    	else strcpy(m_FileName, argv[j]);
    }

#ifndef __unix__
	// This works only on Windows, compiled with MS Visual C
	if(use_ft230)
	{
		LONG lComPortNumber;

	    if(FT_Open(List_FTDI_Devices(), &handle) != FT_OK)
	    {
	        puts("Can not open FTDI adapter.\n");
	        return 3;
	    }
	    
	    if(b_default_CBUS)
	    {
			FTDI_Set_CBUS_Mode(0); // Restore default operation of CBUS0 and CBUS3
			GetOut(0);
	    }
	    
	    if (FT_GetComPortNumber(handle, &lComPortNumber) == FT_OK)
	    { 
	    	if (lComPortNumber != -1)
	    	{
	    		sprintf(m_Serial, "COM%d", lComPortNumber);
	    		printf("Connected to COM%d\n", lComPortNumber); fflush(stdout);
	    		sprintf(buff,"echo COM%d>COMPORT.inc", lComPortNumber);
	    		system(buff);
	    	}
	    }
	    
	    Activate_Bootloader();
		FT_Close(handle); // The port will be re-open as an standard serial port
	}
#endif

    if(strlen(m_FileName)==0) {print_help(argv[0]); return 1;}
    if(strlen(m_Serial)==0) {print_help(argv[0]); return 1;}

    Flash();
    return 0;
}

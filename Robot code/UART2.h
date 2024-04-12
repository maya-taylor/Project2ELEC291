#define NEWLINE 0x0d
#define LINEFEED 0x0a

void initUART2(int BaudRate);
int ReadCom2(int Max,unsigned char *Buffer);
int WriteCom2(int Count,unsigned char *Buffer);
int eputs2(char *String);
int egets2(char *String, int size);
int egets_echo2(char *String, int size);
void eputc2(char c);
char egetc2(void);
char egetc_echo2(void);
int ReceivedBytes2 (void);

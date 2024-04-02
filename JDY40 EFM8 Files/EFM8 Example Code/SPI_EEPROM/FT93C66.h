void FT93C66_Write_Enable(void);
void FT93C66_Write_Disable(void);
unsigned char FT93C66_Read(unsigned int add);
void FT93C66_Erase(unsigned int add);
void FT93C66_Erase_All(void);
void FT93C66_Write(unsigned int add, unsigned char val);
void FT93C66_Write_All(unsigned char val);
void FT93C66_Init(void);

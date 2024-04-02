;--------------------------------------------------------
; File Created by C51
; Version 1.0.0 #1170 (Feb 16 2022) (MSVC)
; This file was generated Tue Apr 02 10:26:35 2024
;--------------------------------------------------------
$name EFM8integration
$optc51 --model-small
$printf_float
	R_DSEG    segment data
	R_CSEG    segment code
	R_BSEG    segment bit
	R_XSEG    segment xdata
	R_PSEG    segment xdata
	R_ISEG    segment idata
	R_OSEG    segment data overlay
	BIT_BANK  segment data overlay
	R_HOME    segment code
	R_GSINIT  segment code
	R_IXSEG   segment xdata
	R_CONST   segment code
	R_XINIT   segment code
	R_DINIT   segment code

;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	public _matchRange_PARM_2
	public _InitPinADC_PARM_2
	public _main
	public _matchRange
	public _loadTimer2
	public _SendATCommand
	public _waitms_or_RI1
	public _RXU1
	public _getstr1
	public _getchar1_with_timeout
	public _getchar1
	public _sendstr1
	public _putchar1
	public _UART1_Init
	public _GetPosition2
	public _map2
	public _LCDprint
	public _LCD_4BIT
	public _WriteCommand
	public _WriteData
	public _LCD_byte
	public _LCD_pulse
	public _Volts_at_Pin
	public _Get_ADC
	public _ADC_at_Pin
	public _TIMER0_Init
	public _InitPinADC
	public _waitms
	public _Timer3us
	public _InitADC
	public _Timer2_ISR
	public __c51_external_startup
	public _LCDprint_PARM_3
	public _buff
	public _GetPosition2_PARM_3
	public _GetPosition2_PARM_2
	public _map2_PARM_5
	public _map2_PARM_4
	public _map2_PARM_3
	public _map2_PARM_2
	public _LCDprint_PARM_2
	public _overflow_count
;--------------------------------------------------------
; Special Function Registers
;--------------------------------------------------------
_ACC            DATA 0xe0
_ADC0ASAH       DATA 0xb6
_ADC0ASAL       DATA 0xb5
_ADC0ASCF       DATA 0xa1
_ADC0ASCT       DATA 0xc7
_ADC0CF0        DATA 0xbc
_ADC0CF1        DATA 0xb9
_ADC0CF2        DATA 0xdf
_ADC0CN0        DATA 0xe8
_ADC0CN1        DATA 0xb2
_ADC0CN2        DATA 0xb3
_ADC0GTH        DATA 0xc4
_ADC0GTL        DATA 0xc3
_ADC0H          DATA 0xbe
_ADC0L          DATA 0xbd
_ADC0LTH        DATA 0xc6
_ADC0LTL        DATA 0xc5
_ADC0MX         DATA 0xbb
_B              DATA 0xf0
_CKCON0         DATA 0x8e
_CKCON1         DATA 0xa6
_CLEN0          DATA 0xc6
_CLIE0          DATA 0xc7
_CLIF0          DATA 0xe8
_CLKSEL         DATA 0xa9
_CLOUT0         DATA 0xd1
_CLU0CF         DATA 0xb1
_CLU0FN         DATA 0xaf
_CLU0MX         DATA 0x84
_CLU1CF         DATA 0xb3
_CLU1FN         DATA 0xb2
_CLU1MX         DATA 0x85
_CLU2CF         DATA 0xb6
_CLU2FN         DATA 0xb5
_CLU2MX         DATA 0x91
_CLU3CF         DATA 0xbf
_CLU3FN         DATA 0xbe
_CLU3MX         DATA 0xae
_CMP0CN0        DATA 0x9b
_CMP0CN1        DATA 0x99
_CMP0MD         DATA 0x9d
_CMP0MX         DATA 0x9f
_CMP1CN0        DATA 0xbf
_CMP1CN1        DATA 0xac
_CMP1MD         DATA 0xab
_CMP1MX         DATA 0xaa
_CRC0CN0        DATA 0xce
_CRC0CN1        DATA 0x86
_CRC0CNT        DATA 0xd3
_CRC0DAT        DATA 0xcb
_CRC0FLIP       DATA 0xcf
_CRC0IN         DATA 0xca
_CRC0ST         DATA 0xd2
_DAC0CF0        DATA 0x91
_DAC0CF1        DATA 0x92
_DAC0H          DATA 0x85
_DAC0L          DATA 0x84
_DAC1CF0        DATA 0x93
_DAC1CF1        DATA 0x94
_DAC1H          DATA 0x8a
_DAC1L          DATA 0x89
_DAC2CF0        DATA 0x95
_DAC2CF1        DATA 0x96
_DAC2H          DATA 0x8c
_DAC2L          DATA 0x8b
_DAC3CF0        DATA 0x9a
_DAC3CF1        DATA 0x9c
_DAC3H          DATA 0x8e
_DAC3L          DATA 0x8d
_DACGCF0        DATA 0x88
_DACGCF1        DATA 0x98
_DACGCF2        DATA 0xa2
_DERIVID        DATA 0xad
_DEVICEID       DATA 0xb5
_DPH            DATA 0x83
_DPL            DATA 0x82
_EIE1           DATA 0xe6
_EIE2           DATA 0xf3
_EIP1           DATA 0xbb
_EIP1H          DATA 0xee
_EIP2           DATA 0xed
_EIP2H          DATA 0xf6
_EMI0CN         DATA 0xe7
_FLKEY          DATA 0xb7
_HFO0CAL        DATA 0xc7
_HFO1CAL        DATA 0xd6
_HFOCN          DATA 0xef
_I2C0ADM        DATA 0xff
_I2C0CN0        DATA 0xba
_I2C0DIN        DATA 0xbc
_I2C0DOUT       DATA 0xbb
_I2C0FCN0       DATA 0xad
_I2C0FCN1       DATA 0xab
_I2C0FCT        DATA 0xf5
_I2C0SLAD       DATA 0xbd
_I2C0STAT       DATA 0xb9
_IE             DATA 0xa8
_IP             DATA 0xb8
_IPH            DATA 0xf2
_IT01CF         DATA 0xe4
_LFO0CN         DATA 0xb1
_P0             DATA 0x80
_P0MASK         DATA 0xfe
_P0MAT          DATA 0xfd
_P0MDIN         DATA 0xf1
_P0MDOUT        DATA 0xa4
_P0SKIP         DATA 0xd4
_P1             DATA 0x90
_P1MASK         DATA 0xee
_P1MAT          DATA 0xed
_P1MDIN         DATA 0xf2
_P1MDOUT        DATA 0xa5
_P1SKIP         DATA 0xd5
_P2             DATA 0xa0
_P2MASK         DATA 0xfc
_P2MAT          DATA 0xfb
_P2MDIN         DATA 0xf3
_P2MDOUT        DATA 0xa6
_P2SKIP         DATA 0xcc
_P3             DATA 0xb0
_P3MDIN         DATA 0xf4
_P3MDOUT        DATA 0x9c
_PCA0CENT       DATA 0x9e
_PCA0CLR        DATA 0x9c
_PCA0CN0        DATA 0xd8
_PCA0CPH0       DATA 0xfc
_PCA0CPH1       DATA 0xea
_PCA0CPH2       DATA 0xec
_PCA0CPH3       DATA 0xf5
_PCA0CPH4       DATA 0x85
_PCA0CPH5       DATA 0xde
_PCA0CPL0       DATA 0xfb
_PCA0CPL1       DATA 0xe9
_PCA0CPL2       DATA 0xeb
_PCA0CPL3       DATA 0xf4
_PCA0CPL4       DATA 0x84
_PCA0CPL5       DATA 0xdd
_PCA0CPM0       DATA 0xda
_PCA0CPM1       DATA 0xdb
_PCA0CPM2       DATA 0xdc
_PCA0CPM3       DATA 0xae
_PCA0CPM4       DATA 0xaf
_PCA0CPM5       DATA 0xcc
_PCA0H          DATA 0xfa
_PCA0L          DATA 0xf9
_PCA0MD         DATA 0xd9
_PCA0POL        DATA 0x96
_PCA0PWM        DATA 0xf7
_PCON0          DATA 0x87
_PCON1          DATA 0xcd
_PFE0CN         DATA 0xc1
_PRTDRV         DATA 0xf6
_PSCTL          DATA 0x8f
_PSTAT0         DATA 0xaa
_PSW            DATA 0xd0
_REF0CN         DATA 0xd1
_REG0CN         DATA 0xc9
_REVID          DATA 0xb6
_RSTSRC         DATA 0xef
_SBCON1         DATA 0x94
_SBRLH1         DATA 0x96
_SBRLL1         DATA 0x95
_SBUF           DATA 0x99
_SBUF0          DATA 0x99
_SBUF1          DATA 0x92
_SCON           DATA 0x98
_SCON0          DATA 0x98
_SCON1          DATA 0xc8
_SFRPAGE        DATA 0xa7
_SFRPGCN        DATA 0xbc
_SFRSTACK       DATA 0xd7
_SMB0ADM        DATA 0xd6
_SMB0ADR        DATA 0xd7
_SMB0CF         DATA 0xc1
_SMB0CN0        DATA 0xc0
_SMB0DAT        DATA 0xc2
_SMB0FCN0       DATA 0xc3
_SMB0FCN1       DATA 0xc4
_SMB0FCT        DATA 0xef
_SMB0RXLN       DATA 0xc5
_SMB0TC         DATA 0xac
_SMOD1          DATA 0x93
_SP             DATA 0x81
_SPI0CFG        DATA 0xa1
_SPI0CKR        DATA 0xa2
_SPI0CN0        DATA 0xf8
_SPI0DAT        DATA 0xa3
_SPI0FCN0       DATA 0x9a
_SPI0FCN1       DATA 0x9b
_SPI0FCT        DATA 0xf7
_SPI0PCF        DATA 0xdf
_TCON           DATA 0x88
_TH0            DATA 0x8c
_TH1            DATA 0x8d
_TL0            DATA 0x8a
_TL1            DATA 0x8b
_TMOD           DATA 0x89
_TMR2CN0        DATA 0xc8
_TMR2CN1        DATA 0xfd
_TMR2H          DATA 0xcf
_TMR2L          DATA 0xce
_TMR2RLH        DATA 0xcb
_TMR2RLL        DATA 0xca
_TMR3CN0        DATA 0x91
_TMR3CN1        DATA 0xfe
_TMR3H          DATA 0x95
_TMR3L          DATA 0x94
_TMR3RLH        DATA 0x93
_TMR3RLL        DATA 0x92
_TMR4CN0        DATA 0x98
_TMR4CN1        DATA 0xff
_TMR4H          DATA 0xa5
_TMR4L          DATA 0xa4
_TMR4RLH        DATA 0xa3
_TMR4RLL        DATA 0xa2
_TMR5CN0        DATA 0xc0
_TMR5CN1        DATA 0xf1
_TMR5H          DATA 0xd5
_TMR5L          DATA 0xd4
_TMR5RLH        DATA 0xd3
_TMR5RLL        DATA 0xd2
_UART0PCF       DATA 0xd9
_UART1FCN0      DATA 0x9d
_UART1FCN1      DATA 0xd8
_UART1FCT       DATA 0xfa
_UART1LIN       DATA 0x9e
_UART1PCF       DATA 0xda
_VDM0CN         DATA 0xff
_WDTCN          DATA 0x97
_XBR0           DATA 0xe1
_XBR1           DATA 0xe2
_XBR2           DATA 0xe3
_XOSC0CN        DATA 0x86
_DPTR           DATA 0x8382
_TMR2RL         DATA 0xcbca
_TMR3RL         DATA 0x9392
_TMR4RL         DATA 0xa3a2
_TMR5RL         DATA 0xd3d2
_TMR0           DATA 0x8c8a
_TMR1           DATA 0x8d8b
_TMR2           DATA 0xcfce
_TMR3           DATA 0x9594
_TMR4           DATA 0xa5a4
_TMR5           DATA 0xd5d4
_SBRL1          DATA 0x9695
_PCA0           DATA 0xfaf9
_PCA0CP0        DATA 0xfcfb
_PCA0CP1        DATA 0xeae9
_PCA0CP2        DATA 0xeceb
_PCA0CP3        DATA 0xf5f4
_PCA0CP4        DATA 0x8584
_PCA0CP5        DATA 0xdedd
_ADC0ASA        DATA 0xb6b5
_ADC0GT         DATA 0xc4c3
_ADC0           DATA 0xbebd
_ADC0LT         DATA 0xc6c5
_DAC0           DATA 0x8584
_DAC1           DATA 0x8a89
_DAC2           DATA 0x8c8b
_DAC3           DATA 0x8e8d
;--------------------------------------------------------
; special function bits
;--------------------------------------------------------
_ACC_0          BIT 0xe0
_ACC_1          BIT 0xe1
_ACC_2          BIT 0xe2
_ACC_3          BIT 0xe3
_ACC_4          BIT 0xe4
_ACC_5          BIT 0xe5
_ACC_6          BIT 0xe6
_ACC_7          BIT 0xe7
_TEMPE          BIT 0xe8
_ADGN0          BIT 0xe9
_ADGN1          BIT 0xea
_ADWINT         BIT 0xeb
_ADBUSY         BIT 0xec
_ADINT          BIT 0xed
_IPOEN          BIT 0xee
_ADEN           BIT 0xef
_B_0            BIT 0xf0
_B_1            BIT 0xf1
_B_2            BIT 0xf2
_B_3            BIT 0xf3
_B_4            BIT 0xf4
_B_5            BIT 0xf5
_B_6            BIT 0xf6
_B_7            BIT 0xf7
_C0FIF          BIT 0xe8
_C0RIF          BIT 0xe9
_C1FIF          BIT 0xea
_C1RIF          BIT 0xeb
_C2FIF          BIT 0xec
_C2RIF          BIT 0xed
_C3FIF          BIT 0xee
_C3RIF          BIT 0xef
_D1SRC0         BIT 0x88
_D1SRC1         BIT 0x89
_D1AMEN         BIT 0x8a
_D01REFSL       BIT 0x8b
_D3SRC0         BIT 0x8c
_D3SRC1         BIT 0x8d
_D3AMEN         BIT 0x8e
_D23REFSL       BIT 0x8f
_D0UDIS         BIT 0x98
_D1UDIS         BIT 0x99
_D2UDIS         BIT 0x9a
_D3UDIS         BIT 0x9b
_EX0            BIT 0xa8
_ET0            BIT 0xa9
_EX1            BIT 0xaa
_ET1            BIT 0xab
_ES0            BIT 0xac
_ET2            BIT 0xad
_ESPI0          BIT 0xae
_EA             BIT 0xaf
_PX0            BIT 0xb8
_PT0            BIT 0xb9
_PX1            BIT 0xba
_PT1            BIT 0xbb
_PS0            BIT 0xbc
_PT2            BIT 0xbd
_PSPI0          BIT 0xbe
_P0_0           BIT 0x80
_P0_1           BIT 0x81
_P0_2           BIT 0x82
_P0_3           BIT 0x83
_P0_4           BIT 0x84
_P0_5           BIT 0x85
_P0_6           BIT 0x86
_P0_7           BIT 0x87
_P1_0           BIT 0x90
_P1_1           BIT 0x91
_P1_2           BIT 0x92
_P1_3           BIT 0x93
_P1_4           BIT 0x94
_P1_5           BIT 0x95
_P1_6           BIT 0x96
_P1_7           BIT 0x97
_P2_0           BIT 0xa0
_P2_1           BIT 0xa1
_P2_2           BIT 0xa2
_P2_3           BIT 0xa3
_P2_4           BIT 0xa4
_P2_5           BIT 0xa5
_P2_6           BIT 0xa6
_P3_0           BIT 0xb0
_P3_1           BIT 0xb1
_P3_2           BIT 0xb2
_P3_3           BIT 0xb3
_P3_4           BIT 0xb4
_P3_7           BIT 0xb7
_CCF0           BIT 0xd8
_CCF1           BIT 0xd9
_CCF2           BIT 0xda
_CCF3           BIT 0xdb
_CCF4           BIT 0xdc
_CCF5           BIT 0xdd
_CR             BIT 0xde
_CF             BIT 0xdf
_PARITY         BIT 0xd0
_F1             BIT 0xd1
_OV             BIT 0xd2
_RS0            BIT 0xd3
_RS1            BIT 0xd4
_F0             BIT 0xd5
_AC             BIT 0xd6
_CY             BIT 0xd7
_RI             BIT 0x98
_TI             BIT 0x99
_RB8            BIT 0x9a
_TB8            BIT 0x9b
_REN            BIT 0x9c
_CE             BIT 0x9d
_SMODE          BIT 0x9e
_RI1            BIT 0xc8
_TI1            BIT 0xc9
_RBX1           BIT 0xca
_TBX1           BIT 0xcb
_REN1           BIT 0xcc
_PERR1          BIT 0xcd
_OVR1           BIT 0xce
_SI             BIT 0xc0
_ACK            BIT 0xc1
_ARBLOST        BIT 0xc2
_ACKRQ          BIT 0xc3
_STO            BIT 0xc4
_STA            BIT 0xc5
_TXMODE         BIT 0xc6
_MASTER         BIT 0xc7
_SPIEN          BIT 0xf8
_TXNF           BIT 0xf9
_NSSMD0         BIT 0xfa
_NSSMD1         BIT 0xfb
_RXOVRN         BIT 0xfc
_MODF           BIT 0xfd
_WCOL           BIT 0xfe
_SPIF           BIT 0xff
_IT0            BIT 0x88
_IE0            BIT 0x89
_IT1            BIT 0x8a
_IE1            BIT 0x8b
_TR0            BIT 0x8c
_TF0            BIT 0x8d
_TR1            BIT 0x8e
_TF1            BIT 0x8f
_T2XCLK0        BIT 0xc8
_T2XCLK1        BIT 0xc9
_TR2            BIT 0xca
_T2SPLIT        BIT 0xcb
_TF2CEN         BIT 0xcc
_TF2LEN         BIT 0xcd
_TF2L           BIT 0xce
_TF2H           BIT 0xcf
_T4XCLK0        BIT 0x98
_T4XCLK1        BIT 0x99
_TR4            BIT 0x9a
_T4SPLIT        BIT 0x9b
_TF4CEN         BIT 0x9c
_TF4LEN         BIT 0x9d
_TF4L           BIT 0x9e
_TF4H           BIT 0x9f
_T5XCLK0        BIT 0xc0
_T5XCLK1        BIT 0xc1
_TR5            BIT 0xc2
_T5SPLIT        BIT 0xc3
_TF5CEN         BIT 0xc4
_TF5LEN         BIT 0xc5
_TF5L           BIT 0xc6
_TF5H           BIT 0xc7
_RIE            BIT 0xd8
_RXTO0          BIT 0xd9
_RXTO1          BIT 0xda
_RFRQ           BIT 0xdb
_TIE            BIT 0xdc
_TXHOLD         BIT 0xdd
_TXNF1          BIT 0xde
_TFRQ           BIT 0xdf
;--------------------------------------------------------
; overlayable register banks
;--------------------------------------------------------
	rbank0 segment data overlay
;--------------------------------------------------------
; internal ram data
;--------------------------------------------------------
	rseg R_DSEG
_overflow_count:
	ds 1
_LCDprint_PARM_2:
	ds 1
_map2_PARM_2:
	ds 4
_map2_PARM_3:
	ds 4
_map2_PARM_4:
	ds 4
_map2_PARM_5:
	ds 4
_GetPosition2_PARM_2:
	ds 3
_GetPosition2_PARM_3:
	ds 3
_GetPosition2_volts_1_119:
	ds 3
_GetPosition2_mid_x_1_120:
	ds 4
_GetPosition2_mid_y_1_120:
	ds 4
_GetPosition2_vx_1_120:
	ds 4
_GetPosition2_vy_1_120:
	ds 4
_main_v_1_161:
	ds 8
_main_xy_pos_1_161:
	ds 8
_main_mid_1_161:
	ds 8
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
	rseg	R_OSEG
	rseg	R_OSEG
_InitPinADC_PARM_2:
	ds 1
	rseg	R_OSEG
	rseg	R_OSEG
	rseg	R_OSEG
	rseg	R_OSEG
_matchRange_PARM_2:
	ds 2
;--------------------------------------------------------
; indirectly addressable internal ram data
;--------------------------------------------------------
	rseg R_ISEG
_buff:
	ds 20
;--------------------------------------------------------
; absolute internal ram data
;--------------------------------------------------------
	DSEG
;--------------------------------------------------------
; bit data
;--------------------------------------------------------
	rseg R_BSEG
_LCDprint_PARM_3:
	DBIT	1
;--------------------------------------------------------
; paged external ram data
;--------------------------------------------------------
	rseg R_PSEG
;--------------------------------------------------------
; external ram data
;--------------------------------------------------------
	rseg R_XSEG
_main_buff_x_1_161:
	ds 17
_main_buff_y_1_161:
	ds 17
_main_temp_buff_1_161:
	ds 4
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	XSEG
;--------------------------------------------------------
; external initialized ram data
;--------------------------------------------------------
	rseg R_IXSEG
	rseg R_HOME
	rseg R_GSINIT
	rseg R_CSEG
;--------------------------------------------------------
; Reset entry point and interrupt vectors
;--------------------------------------------------------
	CSEG at 0x0000
	ljmp	_crt0
	CSEG at 0x002b
	ljmp	_Timer2_ISR
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	rseg R_HOME
	rseg R_GSINIT
	rseg R_GSINIT
;--------------------------------------------------------
; data variables initialization
;--------------------------------------------------------
	rseg R_DINIT
	; The linker places a 'ret' at the end of segment R_DINIT.
;--------------------------------------------------------
; code
;--------------------------------------------------------
	rseg R_CSEG
;------------------------------------------------------------
;Allocation info for local variables in function '_c51_external_startup'
;------------------------------------------------------------
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:50: char _c51_external_startup (void)
;	-----------------------------------------
;	 function _c51_external_startup
;	-----------------------------------------
__c51_external_startup:
	using	0
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:53: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:54: WDTCN = 0xDE; //First key
	mov	_WDTCN,#0xDE
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:55: WDTCN = 0xAD; //Second key
	mov	_WDTCN,#0xAD
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:57: VDM0CN=0x80;       // enable VDD monitor
	mov	_VDM0CN,#0x80
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:58: RSTSRC=0x02|0x04;  // Enable reset on missing clock detector and VDD
	mov	_RSTSRC,#0x06
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:65: SFRPAGE = 0x10;
	mov	_SFRPAGE,#0x10
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:66: PFE0CN  = 0x20; // SYSCLK < 75 MHz.
	mov	_PFE0CN,#0x20
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:67: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:88: CLKSEL = 0x00;
	mov	_CLKSEL,#0x00
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:89: CLKSEL = 0x00;
	mov	_CLKSEL,#0x00
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:90: while ((CLKSEL & 0x80) == 0);
L002001?:
	mov	a,_CLKSEL
	jnb	acc.7,L002001?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:91: CLKSEL = 0x03;
	mov	_CLKSEL,#0x03
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:92: CLKSEL = 0x03;
	mov	_CLKSEL,#0x03
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:93: while ((CLKSEL & 0x80) == 0);
L002004?:
	mov	a,_CLKSEL
	jnb	acc.7,L002004?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:98: P0MDOUT |= 0x11; // Enable UART0 TX (P0.4) and UART1 TX (P0.0) as push-pull outputs
	orl	_P0MDOUT,#0x11
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:99: P2MDOUT |= 0x01; // P2.0 in push-pull mode
	orl	_P2MDOUT,#0x01
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:100: XBR0     = 0x01; // Enable UART0 on P0.4(TX) and P0.5(RX)                     
	mov	_XBR0,#0x01
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:101: XBR1     = 0X00;
	mov	_XBR1,#0x00
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:102: XBR2     = 0x41; // Enable crossbar and uart 1
	mov	_XBR2,#0x41
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:108: SCON0 = 0x10;
	mov	_SCON0,#0x10
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:109: TH1 = 0x100-((SYSCLK/BAUDRATE)/(2L*12L));
	mov	_TH1,#0xE6
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:110: TL1 = TH1;      // Init Timer1
	mov	_TL1,_TH1
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:111: TMOD &= ~0xf0;  // TMOD: timer 1 in 8-bit auto-reload
	anl	_TMOD,#0x0F
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:112: TMOD |=  0x20;                       
	orl	_TMOD,#0x20
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:113: TR1 = 1; // START Timer1
	setb	_TR1
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:114: TI = 1;  // Indicate TX0 ready
	setb	_TI
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:118: TMR2CN0=0x00;   // Stop Timer2; Clear TF2;
	mov	_TMR2CN0,#0x00
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:119: CKCON0|=0b_0001_0000;
	orl	_CKCON0,#0x10
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:120: TMR2RL=(-(SYSCLK/(2*DEFAULT_F))); // Initialize reload value
	mov	_TMR2RL,#0xEE
	mov	(_TMR2RL >> 8),#0xF6
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:121: TMR2=0xffff;   // Set to reload immediately
	mov	_TMR2,#0xFF
	mov	(_TMR2 >> 8),#0xFF
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:122: ET2=1;         // Enable Timer2 interrupts
	setb	_ET2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:123: TR2=1;         // Start Timer2
	setb	_TR2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:124: EA=1; // Global interrupt enable
	setb	_EA
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:126: return 0;
	mov	dpl,#0x00
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'Timer2_ISR'
;------------------------------------------------------------
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:129: void Timer2_ISR (void) interrupt INTERRUPT_TIMER2
;	-----------------------------------------
;	 function Timer2_ISR
;	-----------------------------------------
_Timer2_ISR:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:131: TF2H = 0; // Clear Timer2 interrupt flag
	clr	_TF2H
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:132: BUZZER_OUT=!BUZZER_OUT; // complements the value of BUZZER_OUT to generate a square wave
	cpl	_P0_2
	reti
;	eliminated unneeded push/pop psw
;	eliminated unneeded push/pop dpl
;	eliminated unneeded push/pop dph
;	eliminated unneeded push/pop b
;	eliminated unneeded push/pop acc
;------------------------------------------------------------
;Allocation info for local variables in function 'InitADC'
;------------------------------------------------------------
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:136: void InitADC (void)
;	-----------------------------------------
;	 function InitADC
;	-----------------------------------------
_InitADC:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:138: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:139: ADEN=0; // Disable ADC
	clr	_ADEN
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:144: (0x0 << 0) ; // Accumulate n conversions: 0x0: 1, 0x1:4, 0x2:8, 0x3:16, 0x4:32
	mov	_ADC0CN1,#0x80
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:148: (0x0 << 2); // 0:SYSCLK ADCCLK = SYSCLK. 1:HFOSC0 ADCCLK = HFOSC0.
	mov	_ADC0CF0,#0x20
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:152: (0x1E << 0); // Conversion Tracking Time. Tadtk = ADTK / (Fsarclk)
	mov	_ADC0CF1,#0x1E
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:161: (0x0 << 0) ; // TEMPE. 0: Disable the Temperature Sensor. 1: Enable the Temperature Sensor.
	mov	_ADC0CN0,#0x00
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:166: (0x1F << 0); // ADPWR. Power Up Delay Time. Tpwrtime = ((4 * (ADPWR + 1)) + 2) / (Fadcclk)
	mov	_ADC0CF2,#0x3F
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:170: (0x0 << 0) ; // ADCM. 0x0: ADBUSY, 0x1: TIMER0, 0x2: TIMER2, 0x3: TIMER3, 0x4: CNVSTR, 0x5: CEX5, 0x6: TIMER4, 0x7: TIMER5, 0x8: CLU0, 0x9: CLU1, 0xA: CLU2, 0xB: CLU3
	mov	_ADC0CN2,#0x00
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:172: ADEN=1; // Enable ADC
	setb	_ADEN
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'Timer3us'
;------------------------------------------------------------
;us                        Allocated to registers r2 
;i                         Allocated to registers r3 
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:176: void Timer3us(unsigned char us)
;	-----------------------------------------
;	 function Timer3us
;	-----------------------------------------
_Timer3us:
	mov	r2,dpl
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:181: CKCON0|=0b_0100_0000;
	orl	_CKCON0,#0x40
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:183: TMR3RL = (-(SYSCLK)/1000000L); // Set Timer3 to overflow in 1us.
	mov	_TMR3RL,#0xB8
	mov	(_TMR3RL >> 8),#0xFF
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:184: TMR3 = TMR3RL;                 // Initialize Timer3 for first overflow
	mov	_TMR3,_TMR3RL
	mov	(_TMR3 >> 8),(_TMR3RL >> 8)
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:186: TMR3CN0 = 0x04;                 // Sart Timer3 and clear overflow flag
	mov	_TMR3CN0,#0x04
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:187: for (i = 0; i < us; i++)       // Count <us> overflows
	mov	r3,#0x00
L005004?:
	clr	c
	mov	a,r3
	subb	a,r2
	jnc	L005007?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:189: while (!(TMR3CN0 & 0x80));  // Wait for overflow
L005001?:
	mov	a,_TMR3CN0
	jnb	acc.7,L005001?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:190: TMR3CN0 &= ~(0x80);         // Clear overflow indicator
	anl	_TMR3CN0,#0x7F
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:187: for (i = 0; i < us; i++)       // Count <us> overflows
	inc	r3
	sjmp	L005004?
L005007?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:192: TMR3CN0 = 0 ;                   // Stop Timer3 and clear overflow flag
	mov	_TMR3CN0,#0x00
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'waitms'
;------------------------------------------------------------
;ms                        Allocated to registers r2 r3 
;j                         Allocated to registers r4 r5 
;k                         Allocated to registers r6 
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:195: void waitms (unsigned int ms)
;	-----------------------------------------
;	 function waitms
;	-----------------------------------------
_waitms:
	mov	r2,dpl
	mov	r3,dph
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:199: for(j=0; j<ms; j++)
	mov	r4,#0x00
	mov	r5,#0x00
L006005?:
	clr	c
	mov	a,r4
	subb	a,r2
	mov	a,r5
	subb	a,r3
	jnc	L006009?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:200: for (k=0; k<4; k++) Timer3us(250);
	mov	r6,#0x00
L006001?:
	cjne	r6,#0x04,L006018?
L006018?:
	jnc	L006007?
	mov	dpl,#0xFA
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	push	ar6
	lcall	_Timer3us
	pop	ar6
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
	inc	r6
	sjmp	L006001?
L006007?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:199: for(j=0; j<ms; j++)
	inc	r4
	cjne	r4,#0x00,L006005?
	inc	r5
	sjmp	L006005?
L006009?:
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'InitPinADC'
;------------------------------------------------------------
;pinno                     Allocated with name '_InitPinADC_PARM_2'
;portno                    Allocated to registers r2 
;mask                      Allocated to registers r3 
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:205: void InitPinADC (unsigned char portno, unsigned char pinno)
;	-----------------------------------------
;	 function InitPinADC
;	-----------------------------------------
_InitPinADC:
	mov	r2,dpl
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:209: mask=1<<pinno;
	mov	b,_InitPinADC_PARM_2
	inc	b
	mov	a,#0x01
	sjmp	L007013?
L007011?:
	add	a,acc
L007013?:
	djnz	b,L007011?
	mov	r3,a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:211: SFRPAGE = 0x20;
	mov	_SFRPAGE,#0x20
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:212: switch (portno)
	cjne	r2,#0x00,L007014?
	sjmp	L007001?
L007014?:
	cjne	r2,#0x01,L007015?
	sjmp	L007002?
L007015?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:214: case 0:
	cjne	r2,#0x02,L007005?
	sjmp	L007003?
L007001?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:215: P0MDIN &= (~mask); // Set pin as analog input
	mov	a,r3
	cpl	a
	mov	r2,a
	anl	_P0MDIN,a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:216: P0SKIP |= mask; // Skip Crossbar decoding for this pin
	mov	a,r3
	orl	_P0SKIP,a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:217: break;
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:218: case 1:
	sjmp	L007005?
L007002?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:219: P1MDIN &= (~mask); // Set pin as analog input
	mov	a,r3
	cpl	a
	mov	r2,a
	anl	_P1MDIN,a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:220: P1SKIP |= mask; // Skip Crossbar decoding for this pin
	mov	a,r3
	orl	_P1SKIP,a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:221: break;
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:222: case 2:
	sjmp	L007005?
L007003?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:223: P2MDIN &= (~mask); // Set pin as analog input
	mov	a,r3
	cpl	a
	mov	r2,a
	anl	_P2MDIN,a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:224: P2SKIP |= mask; // Skip Crossbar decoding for this pin
	mov	a,r3
	orl	_P2SKIP,a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:228: }
L007005?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:229: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'TIMER0_Init'
;------------------------------------------------------------
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:232: void TIMER0_Init(void)
;	-----------------------------------------
;	 function TIMER0_Init
;	-----------------------------------------
_TIMER0_Init:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:234: TMOD&=0b_1111_0000; // Set the bits of Timer/Counter 0 to zero
	anl	_TMOD,#0xF0
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:235: TMOD|=0b_0000_0001; // Timer/Counter 0 used as a 16-bit timer
	orl	_TMOD,#0x01
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:236: TR0=0; // Stop Timer/Counter 0
	clr	_TR0
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'ADC_at_Pin'
;------------------------------------------------------------
;pin                       Allocated to registers 
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:239: unsigned int ADC_at_Pin(unsigned char pin)
;	-----------------------------------------
;	 function ADC_at_Pin
;	-----------------------------------------
_ADC_at_Pin:
	mov	_ADC0MX,dpl
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:242: ADINT = 0;
	clr	_ADINT
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:243: ADBUSY = 1;     // Convert voltage at the pin
	setb	_ADBUSY
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:244: while (!ADINT); // Wait for conversion to complete
L009001?:
	jnb	_ADINT,L009001?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:245: return (ADC0);
	mov	dpl,_ADC0
	mov	dph,(_ADC0 >> 8)
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'Get_ADC'
;------------------------------------------------------------
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:248: unsigned int Get_ADC (void)
;	-----------------------------------------
;	 function Get_ADC
;	-----------------------------------------
_Get_ADC:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:250: ADINT = 0;
	clr	_ADINT
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:251: ADBUSY = 1;
	setb	_ADBUSY
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:252: while (!ADINT); // Wait for conversion to complete
L010001?:
	jnb	_ADINT,L010001?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:253: return (ADC0);
	mov	dpl,_ADC0
	mov	dph,(_ADC0 >> 8)
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'Volts_at_Pin'
;------------------------------------------------------------
;pin                       Allocated to registers r2 
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:257: float Volts_at_Pin(unsigned char pin)
;	-----------------------------------------
;	 function Volts_at_Pin
;	-----------------------------------------
_Volts_at_Pin:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:259: return ((ADC_at_Pin(pin)*VDD)/0b_0011_1111_1111_1111);
	lcall	_ADC_at_Pin
	lcall	___uint2fs
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	mov	dptr,#0x6C8B
	mov	b,#0x53
	mov	a,#0x40
	lcall	___fsmul
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	mov	a,sp
	add	a,#0xfc
	mov	sp,a
	clr	a
	push	acc
	mov	a,#0xFC
	push	acc
	mov	a,#0x7F
	push	acc
	mov	a,#0x46
	push	acc
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	lcall	___fsdiv
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	mov	a,sp
	add	a,#0xfc
	mov	sp,a
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'LCD_pulse'
;------------------------------------------------------------
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:263: void LCD_pulse (void)
;	-----------------------------------------
;	 function LCD_pulse
;	-----------------------------------------
_LCD_pulse:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:265: LCD_E=1;
	setb	_P1_6
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:266: Timer3us(40);
	mov	dpl,#0x28
	lcall	_Timer3us
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:267: LCD_E=0;
	clr	_P1_6
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'LCD_byte'
;------------------------------------------------------------
;x                         Allocated to registers r2 
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:270: void LCD_byte (unsigned char x)
;	-----------------------------------------
;	 function LCD_byte
;	-----------------------------------------
_LCD_byte:
	mov	r2,dpl
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:273: ACC=x; //Send high nible
	mov	_ACC,r2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:274: LCD_D7=ACC_7;
	mov	c,_ACC_7
	mov	_P1_0,c
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:275: LCD_D6=ACC_6;
	mov	c,_ACC_6
	mov	_P1_1,c
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:276: LCD_D5=ACC_5;
	mov	c,_ACC_5
	mov	_P1_2,c
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:277: LCD_D4=ACC_4;
	mov	c,_ACC_4
	mov	_P1_3,c
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:278: LCD_pulse();
	push	ar2
	lcall	_LCD_pulse
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:279: Timer3us(40);
	mov	dpl,#0x28
	lcall	_Timer3us
	pop	ar2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:280: ACC=x; //Send low nible
	mov	_ACC,r2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:281: LCD_D7=ACC_3;
	mov	c,_ACC_3
	mov	_P1_0,c
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:282: LCD_D6=ACC_2;
	mov	c,_ACC_2
	mov	_P1_1,c
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:283: LCD_D5=ACC_1;
	mov	c,_ACC_1
	mov	_P1_2,c
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:284: LCD_D4=ACC_0;
	mov	c,_ACC_0
	mov	_P1_3,c
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:285: LCD_pulse();
	ljmp	_LCD_pulse
;------------------------------------------------------------
;Allocation info for local variables in function 'WriteData'
;------------------------------------------------------------
;x                         Allocated to registers r2 
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:289: void WriteData (unsigned char x)
;	-----------------------------------------
;	 function WriteData
;	-----------------------------------------
_WriteData:
	mov	r2,dpl
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:291: LCD_RS=1;
	setb	_P1_7
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:292: LCD_byte(x);
	mov	dpl,r2
	lcall	_LCD_byte
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:293: waitms(2);
	mov	dptr,#0x0002
	ljmp	_waitms
;------------------------------------------------------------
;Allocation info for local variables in function 'WriteCommand'
;------------------------------------------------------------
;x                         Allocated to registers r2 
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:296: void WriteCommand (unsigned char x)
;	-----------------------------------------
;	 function WriteCommand
;	-----------------------------------------
_WriteCommand:
	mov	r2,dpl
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:298: LCD_RS=0;
	clr	_P1_7
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:299: LCD_byte(x);
	mov	dpl,r2
	lcall	_LCD_byte
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:300: waitms(5);
	mov	dptr,#0x0005
	ljmp	_waitms
;------------------------------------------------------------
;Allocation info for local variables in function 'LCD_4BIT'
;------------------------------------------------------------
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:303: void LCD_4BIT (void)
;	-----------------------------------------
;	 function LCD_4BIT
;	-----------------------------------------
_LCD_4BIT:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:305: LCD_E=0; // Resting state of LCD's enable is zero
	clr	_P1_6
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:307: waitms(20);
	mov	dptr,#0x0014
	lcall	_waitms
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:309: WriteCommand(0x33);
	mov	dpl,#0x33
	lcall	_WriteCommand
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:310: WriteCommand(0x33);
	mov	dpl,#0x33
	lcall	_WriteCommand
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:311: WriteCommand(0x32); // Change to 4-bit mode
	mov	dpl,#0x32
	lcall	_WriteCommand
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:313: WriteCommand(0x28);
	mov	dpl,#0x28
	lcall	_WriteCommand
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:314: WriteCommand(0x0c);
	mov	dpl,#0x0C
	lcall	_WriteCommand
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:315: WriteCommand(0x01); // Clear screen command (takes some time)
	mov	dpl,#0x01
	lcall	_WriteCommand
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:316: waitms(20); // Wait for clear screen command to finsih.
	mov	dptr,#0x0014
	ljmp	_waitms
;------------------------------------------------------------
;Allocation info for local variables in function 'LCDprint'
;------------------------------------------------------------
;line                      Allocated with name '_LCDprint_PARM_2'
;string                    Allocated to registers r2 r3 r4 
;j                         Allocated to registers r5 r6 
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:319: void LCDprint(char * string, unsigned char line, bit clear)
;	-----------------------------------------
;	 function LCDprint
;	-----------------------------------------
_LCDprint:
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:322: WriteCommand(line==2?0xc0:0x80);
	mov	a,#0x02
	cjne	a,_LCDprint_PARM_2,L017013?
	mov	r5,#0xC0
	sjmp	L017014?
L017013?:
	mov	r5,#0x80
L017014?:
	mov	dpl,r5
	push	ar2
	push	ar3
	push	ar4
	lcall	_WriteCommand
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:323: waitms(5);
	mov	dptr,#0x0005
	lcall	_waitms
	pop	ar4
	pop	ar3
	pop	ar2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:324: for(j=0; string[j]!=0; j++) WriteData(string[j]);// Write the message
	mov	r5,#0x00
	mov	r6,#0x00
L017003?:
	mov	a,r5
	add	a,r2
	mov	r7,a
	mov	a,r6
	addc	a,r3
	mov	r0,a
	mov	ar1,r4
	mov	dpl,r7
	mov	dph,r0
	mov	b,r1
	lcall	__gptrget
	mov	r7,a
	jz	L017006?
	mov	dpl,r7
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	push	ar6
	lcall	_WriteData
	pop	ar6
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
	inc	r5
	cjne	r5,#0x00,L017003?
	inc	r6
	sjmp	L017003?
L017006?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:325: if(clear) for(; j<CHARS_PER_LINE; j++) WriteData(' '); // Clear the rest ofthe line
	jnb	_LCDprint_PARM_3,L017011?
	mov	ar2,r5
	mov	ar3,r6
L017007?:
	clr	c
	mov	a,r2
	subb	a,#0x10
	mov	a,r3
	xrl	a,#0x80
	subb	a,#0x80
	jnc	L017011?
	mov	dpl,#0x20
	push	ar2
	push	ar3
	lcall	_WriteData
	pop	ar3
	pop	ar2
	inc	r2
	cjne	r2,#0x00,L017007?
	inc	r3
	sjmp	L017007?
L017011?:
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'map2'
;------------------------------------------------------------
;in_min                    Allocated with name '_map2_PARM_2'
;in_max                    Allocated with name '_map2_PARM_3'
;out_min                   Allocated with name '_map2_PARM_4'
;out_max                   Allocated with name '_map2_PARM_5'
;x                         Allocated to registers r2 r3 r4 r5 
;value                     Allocated to registers r2 r3 r4 r5 
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:328: float map2(float x, float in_min, float in_max, float out_min, float out_max)
;	-----------------------------------------
;	 function map2
;	-----------------------------------------
_map2:
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:330: float value =  (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	push	_map2_PARM_2
	push	(_map2_PARM_2 + 1)
	push	(_map2_PARM_2 + 2)
	push	(_map2_PARM_2 + 3)
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	lcall	___fssub
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	mov	a,sp
	add	a,#0xfc
	mov	sp,a
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	push	_map2_PARM_4
	push	(_map2_PARM_4 + 1)
	push	(_map2_PARM_4 + 2)
	push	(_map2_PARM_4 + 3)
	mov	dpl,_map2_PARM_5
	mov	dph,(_map2_PARM_5 + 1)
	mov	b,(_map2_PARM_5 + 2)
	mov	a,(_map2_PARM_5 + 3)
	lcall	___fssub
	mov	r6,dpl
	mov	r7,dph
	mov	r0,b
	mov	r1,a
	mov	a,sp
	add	a,#0xfc
	mov	sp,a
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
	push	ar6
	push	ar7
	push	ar0
	push	ar1
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	lcall	___fsmul
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	mov	a,sp
	add	a,#0xfc
	mov	sp,a
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	push	_map2_PARM_2
	push	(_map2_PARM_2 + 1)
	push	(_map2_PARM_2 + 2)
	push	(_map2_PARM_2 + 3)
	mov	dpl,_map2_PARM_3
	mov	dph,(_map2_PARM_3 + 1)
	mov	b,(_map2_PARM_3 + 2)
	mov	a,(_map2_PARM_3 + 3)
	lcall	___fssub
	mov	r6,dpl
	mov	r7,dph
	mov	r0,b
	mov	r1,a
	mov	a,sp
	add	a,#0xfc
	mov	sp,a
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
	push	ar6
	push	ar7
	push	ar0
	push	ar1
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	lcall	___fsdiv
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	mov	a,sp
	add	a,#0xfc
	mov	sp,a
	push	_map2_PARM_4
	push	(_map2_PARM_4 + 1)
	push	(_map2_PARM_4 + 2)
	push	(_map2_PARM_4 + 3)
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	lcall	___fsadd
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	mov	a,sp
	add	a,#0xfc
	mov	sp,a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:331: return value;
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'GetPosition2'
;------------------------------------------------------------
;pos                       Allocated with name '_GetPosition2_PARM_2'
;mid                       Allocated with name '_GetPosition2_PARM_3'
;volts                     Allocated with name '_GetPosition2_volts_1_119'
;mid_x                     Allocated with name '_GetPosition2_mid_x_1_120'
;mid_y                     Allocated with name '_GetPosition2_mid_y_1_120'
;vx                        Allocated with name '_GetPosition2_vx_1_120'
;vy                        Allocated with name '_GetPosition2_vy_1_120'
;max                       Allocated to registers 
;min                       Allocated to registers 
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:334: void GetPosition2 (float volts[2], float pos[2], float mid[2]) {
;	-----------------------------------------
;	 function GetPosition2
;	-----------------------------------------
_GetPosition2:
	mov	_GetPosition2_volts_1_119,dpl
	mov	(_GetPosition2_volts_1_119 + 1),dph
	mov	(_GetPosition2_volts_1_119 + 2),b
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:336: float mid_x = mid[0];
	mov	r5,_GetPosition2_PARM_3
	mov	r6,(_GetPosition2_PARM_3 + 1)
	mov	r7,(_GetPosition2_PARM_3 + 2)
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	_GetPosition2_mid_x_1_120,a
	inc	dptr
	lcall	__gptrget
	mov	(_GetPosition2_mid_x_1_120 + 1),a
	inc	dptr
	lcall	__gptrget
	mov	(_GetPosition2_mid_x_1_120 + 2),a
	inc	dptr
	lcall	__gptrget
	mov	(_GetPosition2_mid_x_1_120 + 3),a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:337: float mid_y = mid[1];
	mov	a,#0x04
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	_GetPosition2_mid_y_1_120,a
	inc	dptr
	lcall	__gptrget
	mov	(_GetPosition2_mid_y_1_120 + 1),a
	inc	dptr
	lcall	__gptrget
	mov	(_GetPosition2_mid_y_1_120 + 2),a
	inc	dptr
	lcall	__gptrget
	mov	(_GetPosition2_mid_y_1_120 + 3),a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:339: float vx = volts[0];
	mov	dpl,_GetPosition2_volts_1_119
	mov	dph,(_GetPosition2_volts_1_119 + 1)
	mov	b,(_GetPosition2_volts_1_119 + 2)
	lcall	__gptrget
	mov	_GetPosition2_vx_1_120,a
	inc	dptr
	lcall	__gptrget
	mov	(_GetPosition2_vx_1_120 + 1),a
	inc	dptr
	lcall	__gptrget
	mov	(_GetPosition2_vx_1_120 + 2),a
	inc	dptr
	lcall	__gptrget
	mov	(_GetPosition2_vx_1_120 + 3),a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:340: float vy = volts[1];
	mov	a,#0x04
	add	a,_GetPosition2_volts_1_119
	mov	r4,a
	clr	a
	addc	a,(_GetPosition2_volts_1_119 + 1)
	mov	r5,a
	mov	r6,(_GetPosition2_volts_1_119 + 2)
	mov	dpl,r4
	mov	dph,r5
	mov	b,r6
	lcall	__gptrget
	mov	_GetPosition2_vy_1_120,a
	inc	dptr
	lcall	__gptrget
	mov	(_GetPosition2_vy_1_120 + 1),a
	inc	dptr
	lcall	__gptrget
	mov	(_GetPosition2_vy_1_120 + 2),a
	inc	dptr
	lcall	__gptrget
	mov	(_GetPosition2_vy_1_120 + 3),a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:345: if (pos[0] < mid_x) {
	mov	r4,_GetPosition2_PARM_2
	mov	r5,(_GetPosition2_PARM_2 + 1)
	mov	r6,(_GetPosition2_PARM_2 + 2)
	mov	dpl,r4
	mov	dph,r5
	mov	b,r6
	lcall	__gptrget
	mov	r7,a
	inc	dptr
	lcall	__gptrget
	mov	r2,a
	inc	dptr
	lcall	__gptrget
	mov	r3,a
	inc	dptr
	lcall	__gptrget
	mov	r0,a
	push	ar4
	push	ar5
	push	ar6
	push	_GetPosition2_mid_x_1_120
	push	(_GetPosition2_mid_x_1_120 + 1)
	push	(_GetPosition2_mid_x_1_120 + 2)
	push	(_GetPosition2_mid_x_1_120 + 3)
	mov	dpl,r7
	mov	dph,r2
	mov	b,r3
	mov	a,r0
	lcall	___fslt
	mov	r2,dpl
	mov	a,sp
	add	a,#0xfc
	mov	sp,a
	pop	ar6
	pop	ar5
	pop	ar4
	mov	a,r2
	jz	L019002?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:346: pos[0] = map2(vx, min, mid_x, -50.0, 0);
	mov	_map2_PARM_2,#0x00
	mov	(_map2_PARM_2 + 1),#0x00
	mov	(_map2_PARM_2 + 2),#0x00
	mov	(_map2_PARM_2 + 3),#0x00
	mov	_map2_PARM_3,_GetPosition2_mid_x_1_120
	mov	(_map2_PARM_3 + 1),(_GetPosition2_mid_x_1_120 + 1)
	mov	(_map2_PARM_3 + 2),(_GetPosition2_mid_x_1_120 + 2)
	mov	(_map2_PARM_3 + 3),(_GetPosition2_mid_x_1_120 + 3)
	mov	_map2_PARM_4,#0x00
	mov	(_map2_PARM_4 + 1),#0x00
	mov	(_map2_PARM_4 + 2),#0x48
	mov	(_map2_PARM_4 + 3),#0xC2
	mov	_map2_PARM_5,#0x00
	mov	(_map2_PARM_5 + 1),#0x00
	mov	(_map2_PARM_5 + 2),#0x00
	mov	(_map2_PARM_5 + 3),#0x00
	mov	dpl,_GetPosition2_vx_1_120
	mov	dph,(_GetPosition2_vx_1_120 + 1)
	mov	b,(_GetPosition2_vx_1_120 + 2)
	mov	a,(_GetPosition2_vx_1_120 + 3)
	push	ar4
	push	ar5
	push	ar6
	lcall	_map2
	mov	r2,dpl
	mov	r3,dph
	mov	r7,b
	mov	r0,a
	pop	ar6
	pop	ar5
	pop	ar4
	mov	dpl,r4
	mov	dph,r5
	mov	b,r6
	mov	a,r2
	lcall	__gptrput
	inc	dptr
	mov	a,r3
	lcall	__gptrput
	inc	dptr
	mov	a,r7
	lcall	__gptrput
	inc	dptr
	mov	a,r0
	lcall	__gptrput
	sjmp	L019003?
L019002?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:349: pos[0] = map2(vx, mid_x, max, 0.0, 50.0);
	mov	_map2_PARM_2,_GetPosition2_mid_x_1_120
	mov	(_map2_PARM_2 + 1),(_GetPosition2_mid_x_1_120 + 1)
	mov	(_map2_PARM_2 + 2),(_GetPosition2_mid_x_1_120 + 2)
	mov	(_map2_PARM_2 + 3),(_GetPosition2_mid_x_1_120 + 3)
	mov	_map2_PARM_3,#0x33
	mov	(_map2_PARM_3 + 1),#0x33
	mov	(_map2_PARM_3 + 2),#0x53
	mov	(_map2_PARM_3 + 3),#0x40
	mov	_map2_PARM_4,#0x00
	mov	(_map2_PARM_4 + 1),#0x00
	mov	(_map2_PARM_4 + 2),#0x00
	mov	(_map2_PARM_4 + 3),#0x00
	mov	_map2_PARM_5,#0x00
	mov	(_map2_PARM_5 + 1),#0x00
	mov	(_map2_PARM_5 + 2),#0x48
	mov	(_map2_PARM_5 + 3),#0x42
	mov	dpl,_GetPosition2_vx_1_120
	mov	dph,(_GetPosition2_vx_1_120 + 1)
	mov	b,(_GetPosition2_vx_1_120 + 2)
	mov	a,(_GetPosition2_vx_1_120 + 3)
	push	ar4
	push	ar5
	push	ar6
	lcall	_map2
	mov	r2,dpl
	mov	r3,dph
	mov	r7,b
	mov	r0,a
	pop	ar6
	pop	ar5
	pop	ar4
	mov	dpl,r4
	mov	dph,r5
	mov	b,r6
	mov	a,r2
	lcall	__gptrput
	inc	dptr
	mov	a,r3
	lcall	__gptrput
	inc	dptr
	mov	a,r7
	lcall	__gptrput
	inc	dptr
	mov	a,r0
	lcall	__gptrput
L019003?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:352: if (pos[1] < mid_y) {
	mov	a,#0x04
	add	a,r4
	mov	r4,a
	clr	a
	addc	a,r5
	mov	r3,a
	mov	ar2,r6
	mov	dpl,r4
	mov	dph,r3
	mov	b,r2
	lcall	__gptrget
	mov	r5,a
	inc	dptr
	lcall	__gptrget
	mov	r6,a
	inc	dptr
	lcall	__gptrget
	mov	r7,a
	inc	dptr
	lcall	__gptrget
	mov	r0,a
	push	ar2
	push	ar3
	push	ar4
	push	_GetPosition2_mid_y_1_120
	push	(_GetPosition2_mid_y_1_120 + 1)
	push	(_GetPosition2_mid_y_1_120 + 2)
	push	(_GetPosition2_mid_y_1_120 + 3)
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	mov	a,r0
	lcall	___fslt
	mov	r5,dpl
	mov	a,sp
	add	a,#0xfc
	mov	sp,a
	pop	ar4
	pop	ar3
	pop	ar2
	mov	a,r5
	jz	L019005?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:353: pos[1] = map2(vy, min, mid_y, -50.0, 0.0);
	mov	_map2_PARM_2,#0x00
	mov	(_map2_PARM_2 + 1),#0x00
	mov	(_map2_PARM_2 + 2),#0x00
	mov	(_map2_PARM_2 + 3),#0x00
	mov	_map2_PARM_3,_GetPosition2_mid_y_1_120
	mov	(_map2_PARM_3 + 1),(_GetPosition2_mid_y_1_120 + 1)
	mov	(_map2_PARM_3 + 2),(_GetPosition2_mid_y_1_120 + 2)
	mov	(_map2_PARM_3 + 3),(_GetPosition2_mid_y_1_120 + 3)
	mov	_map2_PARM_4,#0x00
	mov	(_map2_PARM_4 + 1),#0x00
	mov	(_map2_PARM_4 + 2),#0x48
	mov	(_map2_PARM_4 + 3),#0xC2
	mov	_map2_PARM_5,#0x00
	mov	(_map2_PARM_5 + 1),#0x00
	mov	(_map2_PARM_5 + 2),#0x00
	mov	(_map2_PARM_5 + 3),#0x00
	mov	dpl,_GetPosition2_vy_1_120
	mov	dph,(_GetPosition2_vy_1_120 + 1)
	mov	b,(_GetPosition2_vy_1_120 + 2)
	mov	a,(_GetPosition2_vy_1_120 + 3)
	push	ar2
	push	ar3
	push	ar4
	lcall	_map2
	mov	r5,dpl
	mov	r6,dph
	mov	r7,b
	mov	r0,a
	pop	ar4
	pop	ar3
	pop	ar2
	mov	dpl,r4
	mov	dph,r3
	mov	b,r2
	mov	a,r5
	lcall	__gptrput
	inc	dptr
	mov	a,r6
	lcall	__gptrput
	inc	dptr
	mov	a,r7
	lcall	__gptrput
	inc	dptr
	mov	a,r0
	ljmp	__gptrput
L019005?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:356: pos[1] = map2(vy, mid_y, max, 0.0, 50.0);
	mov	_map2_PARM_2,_GetPosition2_mid_y_1_120
	mov	(_map2_PARM_2 + 1),(_GetPosition2_mid_y_1_120 + 1)
	mov	(_map2_PARM_2 + 2),(_GetPosition2_mid_y_1_120 + 2)
	mov	(_map2_PARM_2 + 3),(_GetPosition2_mid_y_1_120 + 3)
	mov	_map2_PARM_3,#0x33
	mov	(_map2_PARM_3 + 1),#0x33
	mov	(_map2_PARM_3 + 2),#0x53
	mov	(_map2_PARM_3 + 3),#0x40
	mov	_map2_PARM_4,#0x00
	mov	(_map2_PARM_4 + 1),#0x00
	mov	(_map2_PARM_4 + 2),#0x00
	mov	(_map2_PARM_4 + 3),#0x00
	mov	_map2_PARM_5,#0x00
	mov	(_map2_PARM_5 + 1),#0x00
	mov	(_map2_PARM_5 + 2),#0x48
	mov	(_map2_PARM_5 + 3),#0x42
	mov	dpl,_GetPosition2_vy_1_120
	mov	dph,(_GetPosition2_vy_1_120 + 1)
	mov	b,(_GetPosition2_vy_1_120 + 2)
	mov	a,(_GetPosition2_vy_1_120 + 3)
	push	ar2
	push	ar3
	push	ar4
	lcall	_map2
	mov	r5,dpl
	mov	r6,dph
	mov	r7,b
	mov	r0,a
	pop	ar4
	pop	ar3
	pop	ar2
	mov	dpl,r4
	mov	dph,r3
	mov	b,r2
	mov	a,r5
	lcall	__gptrput
	inc	dptr
	mov	a,r6
	lcall	__gptrput
	inc	dptr
	mov	a,r7
	lcall	__gptrput
	inc	dptr
	mov	a,r0
	ljmp	__gptrput
;------------------------------------------------------------
;Allocation info for local variables in function 'UART1_Init'
;------------------------------------------------------------
;baudrate                  Allocated to registers r2 r3 r4 r5 
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:360: void UART1_Init (unsigned long baudrate)
;	-----------------------------------------
;	 function UART1_Init
;	-----------------------------------------
_UART1_Init:
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:362: SFRPAGE = 0x20;
	mov	_SFRPAGE,#0x20
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:363: SMOD1 = 0x0C; // no parity, 8 data bits, 1 stop bit
	mov	_SMOD1,#0x0C
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:364: SCON1 = 0x10;
	mov	_SCON1,#0x10
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:365: SBCON1 =0x00;   // disable baud rate generator
	mov	_SBCON1,#0x00
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:366: SBRL1 = 0x10000L-((SYSCLK/baudrate)/(12L*2L));
	mov	__divulong_PARM_2,r2
	mov	(__divulong_PARM_2 + 1),r3
	mov	(__divulong_PARM_2 + 2),r4
	mov	(__divulong_PARM_2 + 3),r5
	mov	dptr,#0xA200
	mov	b,#0x4A
	mov	a,#0x04
	lcall	__divulong
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	mov	__divulong_PARM_2,#0x18
	clr	a
	mov	(__divulong_PARM_2 + 1),a
	mov	(__divulong_PARM_2 + 2),a
	mov	(__divulong_PARM_2 + 3),a
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	lcall	__divulong
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	clr	a
	clr	c
	subb	a,r2
	mov	r2,a
	clr	a
	subb	a,r3
	mov	r3,a
	mov	a,#0x01
	subb	a,r4
	clr	a
	subb	a,r5
	mov	_SBRL1,r2
	mov	(_SBRL1 >> 8),r3
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:367: TI1 = 1; // indicate ready for TX
	setb	_TI1
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:368: SBCON1 |= 0x40;   // enable baud rate generator
	orl	_SBCON1,#0x40
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:369: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'putchar1'
;------------------------------------------------------------
;c                         Allocated to registers r2 
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:372: void putchar1 (char c) 
;	-----------------------------------------
;	 function putchar1
;	-----------------------------------------
_putchar1:
	mov	r2,dpl
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:374: SFRPAGE = 0x20;
	mov	_SFRPAGE,#0x20
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:375: while (!TI1);
L021001?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:376: TI1=0;
	jbc	_TI1,L021008?
	sjmp	L021001?
L021008?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:377: SBUF1 = c;
	mov	_SBUF1,r2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:378: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'sendstr1'
;------------------------------------------------------------
;s                         Allocated to registers r2 r3 r4 
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:381: void sendstr1 (char * s)
;	-----------------------------------------
;	 function sendstr1
;	-----------------------------------------
_sendstr1:
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:383: while(*s)
L022001?:
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
	mov	r5,a
	jz	L022004?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:385: putchar1(*s);
	mov	dpl,r5
	push	ar2
	push	ar3
	push	ar4
	lcall	_putchar1
	pop	ar4
	pop	ar3
	pop	ar2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:386: s++;	
	inc	r2
	cjne	r2,#0x00,L022001?
	inc	r3
	sjmp	L022001?
L022004?:
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'getchar1'
;------------------------------------------------------------
;c                         Allocated to registers 
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:390: char getchar1 (void)
;	-----------------------------------------
;	 function getchar1
;	-----------------------------------------
_getchar1:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:393: SFRPAGE = 0x20;
	mov	_SFRPAGE,#0x20
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:394: while (!RI1);
L023001?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:395: RI1=0;
	jbc	_RI1,L023008?
	sjmp	L023001?
L023008?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:397: SCON1&=0b_0011_1111;
	anl	_SCON1,#0x3F
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:398: c = SBUF1;
	mov	dpl,_SBUF1
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:399: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:400: return (c);
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'getchar1_with_timeout'
;------------------------------------------------------------
;c                         Allocated to registers 
;timeout                   Allocated to registers r2 r3 
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:403: char getchar1_with_timeout (void)
;	-----------------------------------------
;	 function getchar1_with_timeout
;	-----------------------------------------
_getchar1_with_timeout:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:407: SFRPAGE = 0x20;
	mov	_SFRPAGE,#0x20
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:409: while (!RI1)
	mov	r2,#0x00
	mov	r3,#0x00
L024003?:
	jb	_RI1,L024005?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:411: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:412: Timer3us(20);
	mov	dpl,#0x14
	push	ar2
	push	ar3
	lcall	_Timer3us
	pop	ar3
	pop	ar2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:413: SFRPAGE = 0x20;
	mov	_SFRPAGE,#0x20
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:414: timeout++;
	inc	r2
	cjne	r2,#0x00,L024012?
	inc	r3
L024012?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:415: if(timeout==25000)
	cjne	r2,#0xA8,L024003?
	cjne	r3,#0x61,L024003?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:417: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:418: return ('\n'); // Timeout after half second
	mov	dpl,#0x0A
	ret
L024005?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:421: RI1=0;
	clr	_RI1
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:423: SCON1&=0b_0011_1111;
	anl	_SCON1,#0x3F
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:424: c = SBUF1;
	mov	dpl,_SBUF1
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:425: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:426: return (c);
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'getstr1'
;------------------------------------------------------------
;s                         Allocated to registers r2 r3 r4 
;c                         Allocated to registers r5 
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:430: void getstr1 (char * s)
;	-----------------------------------------
;	 function getstr1
;	-----------------------------------------
_getstr1:
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:434: while(1)
L025004?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:436: c=getchar1_with_timeout();
	push	ar2
	push	ar3
	push	ar4
	lcall	_getchar1_with_timeout
	mov	r5,dpl
	pop	ar4
	pop	ar3
	pop	ar2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:437: if(c=='\n')
	cjne	r5,#0x0A,L025002?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:439: *s=0;
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	clr	a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:440: return;
	ljmp	__gptrput
L025002?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:442: *s=c;
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	lcall	__gptrput
	inc	dptr
	mov	r2,dpl
	mov	r3,dph
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:443: s++;
	sjmp	L025004?
;------------------------------------------------------------
;Allocation info for local variables in function 'RXU1'
;------------------------------------------------------------
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:448: bit RXU1 (void)
;	-----------------------------------------
;	 function RXU1
;	-----------------------------------------
_RXU1:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:451: SFRPAGE = 0x20;
	mov	_SFRPAGE,#0x20
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:452: mybit=RI1;
	mov	c,_RI1
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:453: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:454: return mybit;
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'waitms_or_RI1'
;------------------------------------------------------------
;ms                        Allocated to registers r2 r3 
;j                         Allocated to registers r4 r5 
;k                         Allocated to registers r6 
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:457: void waitms_or_RI1 (unsigned int ms)
;	-----------------------------------------
;	 function waitms_or_RI1
;	-----------------------------------------
_waitms_or_RI1:
	mov	r2,dpl
	mov	r3,dph
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:461: for(j=0; j<ms; j++)
	mov	r4,#0x00
	mov	r5,#0x00
L027007?:
	clr	c
	mov	a,r4
	subb	a,r2
	mov	a,r5
	subb	a,r3
	jnc	L027011?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:463: for (k=0; k<4; k++)
	mov	r6,#0x00
L027003?:
	cjne	r6,#0x04,L027019?
L027019?:
	jnc	L027009?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:465: if(RXU1()) return;
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	push	ar6
	lcall	_RXU1
	clr	a
	rlc	a
	pop	ar6
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
	jz	L027002?
	ret
L027002?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:466: Timer3us(250);
	mov	dpl,#0xFA
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	push	ar6
	lcall	_Timer3us
	pop	ar6
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:463: for (k=0; k<4; k++)
	inc	r6
	sjmp	L027003?
L027009?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:461: for(j=0; j<ms; j++)
	inc	r4
	cjne	r4,#0x00,L027007?
	inc	r5
	sjmp	L027007?
L027011?:
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'SendATCommand'
;------------------------------------------------------------
;s                         Allocated to registers r2 r3 r4 
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:471: void SendATCommand (char * s)
;	-----------------------------------------
;	 function SendATCommand
;	-----------------------------------------
_SendATCommand:
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:473: printf("Command: %s", s);
	push	ar2
	push	ar3
	push	ar4
	push	ar2
	push	ar3
	push	ar4
	mov	a,#__str_0
	push	acc
	mov	a,#(__str_0 >> 8)
	push	acc
	mov	a,#0x80
	push	acc
	lcall	_printf
	mov	a,sp
	add	a,#0xfa
	mov	sp,a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:474: P2_0=0; // 'set' pin to 0 is 'AT' mode.
	clr	_P2_0
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:475: waitms(5);
	mov	dptr,#0x0005
	lcall	_waitms
	pop	ar4
	pop	ar3
	pop	ar2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:476: sendstr1(s);
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	_sendstr1
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:477: getstr1(buff);
	mov	dptr,#_buff
	mov	b,#0x40
	lcall	_getstr1
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:478: waitms(10);
	mov	dptr,#0x000A
	lcall	_waitms
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:479: P2_0=1; // 'set' pin to 1 is normal operation mode.
	setb	_P2_0
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:480: printf("Response: %s\r\n", buff);
	mov	a,#_buff
	push	acc
	mov	a,#(_buff >> 8)
	push	acc
	mov	a,#0x40
	push	acc
	mov	a,#__str_1
	push	acc
	mov	a,#(__str_1 >> 8)
	push	acc
	mov	a,#0x80
	push	acc
	lcall	_printf
	mov	a,sp
	add	a,#0xfa
	mov	sp,a
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'loadTimer2'
;------------------------------------------------------------
;freq                      Allocated to registers r2 r3 r4 r5 
;x                         Allocated to registers r2 r3 r4 r5 
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:485: void loadTimer2(unsigned long int freq) {
;	-----------------------------------------
;	 function loadTimer2
;	-----------------------------------------
_loadTimer2:
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:486: unsigned long int x=(SYSCLK/(2L*freq));
	mov	a,r2
	add	a,r2
	mov	__divulong_PARM_2,a
	mov	a,r3
	rlc	a
	mov	(__divulong_PARM_2 + 1),a
	mov	a,r4
	rlc	a
	mov	(__divulong_PARM_2 + 2),a
	mov	a,r5
	rlc	a
	mov	(__divulong_PARM_2 + 3),a
	mov	dptr,#0xA200
	mov	b,#0x4A
	mov	a,#0x04
	lcall	__divulong
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:488: TR2=0; // Stop timer 2
	clr	_TR2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:489: TMR2RL=0x10000L-x; // Change reload value for new frequency
	clr	a
	clr	c
	subb	a,r2
	mov	r2,a
	clr	a
	subb	a,r3
	mov	r3,a
	mov	a,#0x01
	subb	a,r4
	clr	a
	subb	a,r5
	mov	_TMR2RL,r2
	mov	(_TMR2RL >> 8),r3
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:490: TR2=1; // Start timer 2
	setb	_TR2
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'matchRange'
;------------------------------------------------------------
;y                         Allocated with name '_matchRange_PARM_2'
;x                         Allocated to registers r2 r3 
;result                    Allocated to registers r4 
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:507: char matchRange(int x, int y) {
;	-----------------------------------------
;	 function matchRange
;	-----------------------------------------
_matchRange:
	mov	r2,dpl
	mov	r3,dph
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:511: if (x >= -5 && x <= 5) {
	clr	c
	mov	a,r2
	subb	a,#0xFB
	mov	a,r3
	xrl	a,#0x80
	subb	a,#0x7f
	jnc	L030121?
	ljmp	L030081?
L030121?:
	clr	c
	mov	a,#0x05
	subb	a,r2
	clr	a
	xrl	a,#0x80
	mov	b,r3
	xrl	b,#0x80
	subb	a,b
	jnc	L030122?
	ljmp	L030081?
L030122?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:512: if (y >= 45)
	clr	c
	mov	a,_matchRange_PARM_2
	subb	a,#0x2D
	mov	a,(_matchRange_PARM_2 + 1)
	xrl	a,#0x80
	subb	a,#0x80
	jc	L030021?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:513: result = 'A';
	mov	r4,#0x41
	ljmp	L030082?
L030021?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:514: else if (y >= 30 && y <= 45)
	clr	c
	mov	a,_matchRange_PARM_2
	subb	a,#0x1E
	mov	a,(_matchRange_PARM_2 + 1)
	xrl	a,#0x80
	subb	a,#0x80
	jc	L030017?
	mov	a,#0x2D
	subb	a,_matchRange_PARM_2
	clr	a
	xrl	a,#0x80
	mov	b,(_matchRange_PARM_2 + 1)
	xrl	b,#0x80
	subb	a,b
	jc	L030017?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:515: result = 'B';
	mov	r4,#0x42
	ljmp	L030082?
L030017?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:516: else if (y >= 5 && y <= 30)
	clr	c
	mov	a,_matchRange_PARM_2
	subb	a,#0x05
	mov	a,(_matchRange_PARM_2 + 1)
	xrl	a,#0x80
	subb	a,#0x80
	jc	L030013?
	mov	a,#0x1E
	subb	a,_matchRange_PARM_2
	clr	a
	xrl	a,#0x80
	mov	b,(_matchRange_PARM_2 + 1)
	xrl	b,#0x80
	subb	a,b
	jc	L030013?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:517: result = 'C';
	mov	r4,#0x43
	ljmp	L030082?
L030013?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:518: else if (y <= -45)
	clr	c
	mov	a,#0xD3
	subb	a,_matchRange_PARM_2
	mov	a,#(0xFF ^ 0x80)
	mov	b,(_matchRange_PARM_2 + 1)
	xrl	b,#0x80
	subb	a,b
	jc	L030010?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:519: result = 'D';
	mov	r4,#0x44
	ljmp	L030082?
L030010?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:520: else if (y >= -45 && y <= -30)
	clr	c
	mov	a,_matchRange_PARM_2
	subb	a,#0xD3
	mov	a,(_matchRange_PARM_2 + 1)
	xrl	a,#0x80
	subb	a,#0x7f
	jc	L030006?
	mov	a,#0xE2
	subb	a,_matchRange_PARM_2
	mov	a,#(0xFF ^ 0x80)
	mov	b,(_matchRange_PARM_2 + 1)
	xrl	b,#0x80
	subb	a,b
	jc	L030006?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:521: result = 'E';
	mov	r4,#0x45
	ljmp	L030082?
L030006?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:522: else if (y >= -30 && y <= -5)
	clr	c
	mov	a,_matchRange_PARM_2
	subb	a,#0xE2
	mov	a,(_matchRange_PARM_2 + 1)
	xrl	a,#0x80
	subb	a,#0x7f
	jc	L030002?
	mov	a,#0xFB
	subb	a,_matchRange_PARM_2
	mov	a,#(0xFF ^ 0x80)
	mov	b,(_matchRange_PARM_2 + 1)
	xrl	b,#0x80
	subb	a,b
	jc	L030002?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:523: result = 'F';
	mov	r4,#0x46
	ljmp	L030082?
L030002?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:525: result = 'Z'; // Default case if Y doesn't match any range
	mov	r4,#0x5A
	ljmp	L030082?
L030081?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:527: else if (x >= 40) {
	clr	c
	mov	a,r2
	subb	a,#0x28
	mov	a,r3
	xrl	a,#0x80
	subb	a,#0x80
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:529: if (y >= 5) 
	jc	L030078?
	mov	a,_matchRange_PARM_2
	subb	a,#0x05
	mov	a,(_matchRange_PARM_2 + 1)
	xrl	a,#0x80
	subb	a,#0x80
	jc	L030031?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:530: result = 'K';
	mov	r4,#0x4B
	ljmp	L030082?
L030031?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:531: else if (y <= -5)
	clr	c
	mov	a,#0xFB
	subb	a,_matchRange_PARM_2
	mov	a,#(0xFF ^ 0x80)
	mov	b,(_matchRange_PARM_2 + 1)
	xrl	b,#0x80
	subb	a,b
	jc	L030028?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:532: result = 'M';
	mov	r4,#0x4D
	ljmp	L030082?
L030028?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:533: else if (y >= -5 && y <= 5)
	clr	c
	mov	a,_matchRange_PARM_2
	subb	a,#0xFB
	mov	a,(_matchRange_PARM_2 + 1)
	xrl	a,#0x80
	subb	a,#0x7f
	jc	L030024?
	mov	a,#0x05
	subb	a,_matchRange_PARM_2
	clr	a
	xrl	a,#0x80
	mov	b,(_matchRange_PARM_2 + 1)
	xrl	b,#0x80
	subb	a,b
	jc	L030024?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:534: result = 'G';
	mov	r4,#0x47
	ljmp	L030082?
L030024?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:536: result = 'Z'; // Default case if Y doesn't match any range
	mov	r4,#0x5A
	ljmp	L030082?
L030078?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:538: else if (x <= -40) {
	clr	c
	mov	a,#0xD8
	subb	a,r2
	mov	a,#(0xFF ^ 0x80)
	mov	b,r3
	xrl	b,#0x80
	subb	a,b
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:540: if (y <= -5)
	jc	L030075?
	mov	a,#0xFB
	subb	a,_matchRange_PARM_2
	mov	a,#(0xFF ^ 0x80)
	mov	b,(_matchRange_PARM_2 + 1)
	xrl	b,#0x80
	subb	a,b
	clr	a
	rlc	a
	mov	r5,a
	jnz	L030044?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:541: result = 'N';
	mov	r4,#0x4E
	ljmp	L030082?
L030044?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:542: else if (y >= 5)
	clr	c
	mov	a,_matchRange_PARM_2
	subb	a,#0x05
	mov	a,(_matchRange_PARM_2 + 1)
	xrl	a,#0x80
	subb	a,#0x80
	jc	L030041?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:543: result = 'L';
	mov	r4,#0x4C
	ljmp	L030082?
L030041?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:544: else if (y <= -5)
	mov	a,r5
	jnz	L030038?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:545: result = 'N';
	mov	r4,#0x4E
	ljmp	L030082?
L030038?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:546: else if (y >= -5 && y <= 5)
	clr	c
	mov	a,_matchRange_PARM_2
	subb	a,#0xFB
	mov	a,(_matchRange_PARM_2 + 1)
	xrl	a,#0x80
	subb	a,#0x7f
	jc	L030034?
	mov	a,#0x05
	subb	a,_matchRange_PARM_2
	clr	a
	xrl	a,#0x80
	mov	b,(_matchRange_PARM_2 + 1)
	xrl	b,#0x80
	subb	a,b
	jc	L030034?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:547: result = 'I';
	mov	r4,#0x49
	ljmp	L030082?
L030034?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:549: result = 'Z'; // Default case if Y doesn't match any range
	mov	r4,#0x5A
	ljmp	L030082?
L030075?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:551: else if (x >= 5 && x <= 40) {
	clr	c
	mov	a,r2
	subb	a,#0x05
	mov	a,r3
	xrl	a,#0x80
	subb	a,#0x80
	jc	L030071?
	mov	a,#0x28
	subb	a,r2
	clr	a
	xrl	a,#0x80
	mov	b,r3
	xrl	b,#0x80
	subb	a,b
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:553: if (y >= -5 && y <= 5)
	jc	L030071?
	mov	a,_matchRange_PARM_2
	subb	a,#0xFB
	mov	a,(_matchRange_PARM_2 + 1)
	xrl	a,#0x80
	subb	a,#0x7f
	jc	L030053?
	mov	a,#0x05
	subb	a,_matchRange_PARM_2
	clr	a
	xrl	a,#0x80
	mov	b,(_matchRange_PARM_2 + 1)
	xrl	b,#0x80
	subb	a,b
	jc	L030053?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:554: result = 'H';
	mov	r4,#0x48
	ljmp	L030082?
L030053?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:555: else if (y >= 5)
	clr	c
	mov	a,_matchRange_PARM_2
	subb	a,#0x05
	mov	a,(_matchRange_PARM_2 + 1)
	xrl	a,#0x80
	subb	a,#0x80
	jc	L030050?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:556: result = 'K';
	mov	r4,#0x4B
	ljmp	L030082?
L030050?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:557: else if (y <= -5)
	clr	c
	mov	a,#0xFB
	subb	a,_matchRange_PARM_2
	mov	a,#(0xFF ^ 0x80)
	mov	b,(_matchRange_PARM_2 + 1)
	xrl	b,#0x80
	subb	a,b
	jc	L030047?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:558: result = 'M';
	mov	r4,#0x4D
	ljmp	L030082?
L030047?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:560: result = 'Z'; // Default case if Y doesn't match any range
	mov	r4,#0x5A
	ljmp	L030082?
L030071?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:562: else if (x >= -40 && x <= -5) {
	clr	c
	mov	a,r2
	subb	a,#0xD8
	mov	a,r3
	xrl	a,#0x80
	subb	a,#0x7f
	jc	L030067?
	mov	a,#0xFB
	subb	a,r2
	mov	a,#(0xFF ^ 0x80)
	mov	b,r3
	xrl	b,#0x80
	subb	a,b
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:564: if (y >= -5 && y <= 5)
	jc	L030067?
	mov	a,_matchRange_PARM_2
	subb	a,#0xFB
	mov	a,(_matchRange_PARM_2 + 1)
	xrl	a,#0x80
	subb	a,#0x7f
	jc	L030063?
	mov	a,#0x05
	subb	a,_matchRange_PARM_2
	clr	a
	xrl	a,#0x80
	mov	b,(_matchRange_PARM_2 + 1)
	xrl	b,#0x80
	subb	a,b
	jc	L030063?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:565: result = 'J';
	mov	r4,#0x4A
	sjmp	L030082?
L030063?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:566: else if (y >= 5)
	clr	c
	mov	a,_matchRange_PARM_2
	subb	a,#0x05
	mov	a,(_matchRange_PARM_2 + 1)
	xrl	a,#0x80
	subb	a,#0x80
	jc	L030060?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:567: result = 'L';
	mov	r4,#0x4C
	sjmp	L030082?
L030060?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:568: else if (y <= -5)
	clr	c
	mov	a,#0xFB
	subb	a,_matchRange_PARM_2
	mov	a,#(0xFF ^ 0x80)
	mov	b,(_matchRange_PARM_2 + 1)
	xrl	b,#0x80
	subb	a,b
	jc	L030057?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:569: result = 'N';
	mov	r4,#0x4E
	sjmp	L030082?
L030057?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:571: result = 'Z'; // Default case if Y doesn't match any range
	mov	r4,#0x5A
	sjmp	L030082?
L030067?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:574: result = 'Z'; // Default case if X doesn't match any range
	mov	r4,#0x5A
L030082?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:577: return result;
	mov	dpl,r4
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'main'
;------------------------------------------------------------
;timeout_cnt               Allocated to registers r4 r5 
;metal_lev                 Allocated to registers r2 r3 
;v                         Allocated with name '_main_v_1_161'
;xy_pos                    Allocated with name '_main_xy_pos_1_161'
;mid                       Allocated with name '_main_mid_1_161'
;mapped_dir                Allocated to registers r4 
;buff_x                    Allocated with name '_main_buff_x_1_161'
;buff_y                    Allocated with name '_main_buff_y_1_161'
;temp_buff                 Allocated with name '_main_temp_buff_1_161'
;------------------------------------------------------------
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:581: void main(void)
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:584: int metal_lev = 0;
	mov	r2,#0x00
	mov	r3,#0x00
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:595: LCD_4BIT();
	push	ar2
	push	ar3
	lcall	_LCD_4BIT
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:597: waitms(500);
	mov	dptr,#0x01F4
	lcall	_waitms
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:598: printf("\x1b[2J"); // Clear screen using ANSI escape sequence.
	mov	a,#__str_2
	push	acc
	mov	a,#(__str_2 >> 8)
	push	acc
	mov	a,#0x80
	push	acc
	lcall	_printf
	dec	sp
	dec	sp
	dec	sp
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:603: __FILE__, __DATE__, __TIME__);
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:602: "Compiled: %s, %s\n\n",
	mov	a,#__str_6
	push	acc
	mov	a,#(__str_6 >> 8)
	push	acc
	mov	a,#0x80
	push	acc
	mov	a,#__str_5
	push	acc
	mov	a,#(__str_5 >> 8)
	push	acc
	mov	a,#0x80
	push	acc
	mov	a,#__str_4
	push	acc
	mov	a,#(__str_4 >> 8)
	push	acc
	mov	a,#0x80
	push	acc
	mov	a,#__str_3
	push	acc
	mov	a,#(__str_3 >> 8)
	push	acc
	mov	a,#0x80
	push	acc
	lcall	_printf
	mov	a,sp
	add	a,#0xf4
	mov	sp,a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:604: UART1_Init(9600);
	mov	dptr,#0x2580
	clr	a
	mov	b,a
	lcall	_UART1_Init
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:606: InitPinADC(1, 4); // Configure P1.4 as analog input
	mov	_InitPinADC_PARM_2,#0x04
	mov	dpl,#0x01
	lcall	_InitPinADC
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:607: InitPinADC(1, 5); // Configure P1.5 as analog input
	mov	_InitPinADC_PARM_2,#0x05
	mov	dpl,#0x01
	lcall	_InitPinADC
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:608: InitADC();
	lcall	_InitADC
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:626: SendATCommand("AT+DVIDA0A0\r\n");  
	mov	dptr,#__str_7
	mov	b,#0x80
	lcall	_SendATCommand
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:629: SendATCommand("AT+VER\r\n");
	mov	dptr,#__str_8
	mov	b,#0x80
	lcall	_SendATCommand
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:630: SendATCommand("AT+BAUD\r\n");
	mov	dptr,#__str_9
	mov	b,#0x80
	lcall	_SendATCommand
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:631: SendATCommand("AT+RFID\r\n");
	mov	dptr,#__str_10
	mov	b,#0x80
	lcall	_SendATCommand
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:632: SendATCommand("AT+DVID\r\n");
	mov	dptr,#__str_11
	mov	b,#0x80
	lcall	_SendATCommand
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:633: SendATCommand("AT+RFC\r\n");
	mov	dptr,#__str_12
	mov	b,#0x80
	lcall	_SendATCommand
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:634: SendATCommand("AT+POWE\r\n");
	mov	dptr,#__str_13
	mov	b,#0x80
	lcall	_SendATCommand
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:635: SendATCommand("AT+CLSS\r\n");
	mov	dptr,#__str_14
	mov	b,#0x80
	lcall	_SendATCommand
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:637: mid[0] = Volts_at_Pin(XPOS_PIN);;
	mov	dpl,#0x0A
	lcall	_Volts_at_Pin
	mov	r4,dpl
	mov	r5,dph
	mov	r6,b
	mov	r7,a
	mov	_main_mid_1_161,r4
	mov	(_main_mid_1_161 + 1),r5
	mov	(_main_mid_1_161 + 2),r6
	mov	(_main_mid_1_161 + 3),r7
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:638: mid[1] = Volts_at_Pin(YPOS_PIN);;
	mov	dpl,#0x0B
	lcall	_Volts_at_Pin
	mov	r4,dpl
	mov	r5,dph
	mov	r6,b
	mov	r7,a
	mov	(_main_mid_1_161 + 0x0004),r4
	mov	((_main_mid_1_161 + 0x0004) + 1),r5
	mov	((_main_mid_1_161 + 0x0004) + 2),r6
	mov	((_main_mid_1_161 + 0x0004) + 3),r7
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:640: printf("LED voltage ref: 1.7854, midXvolts: %f, midYvolts: %f \r\n", mid[1], mid[0]);
	push	_main_mid_1_161
	push	(_main_mid_1_161 + 1)
	push	(_main_mid_1_161 + 2)
	push	(_main_mid_1_161 + 3)
	push	ar4
	push	ar5
	push	ar6
	push	ar7
	mov	a,#__str_15
	push	acc
	mov	a,#(__str_15 >> 8)
	push	acc
	mov	a,#0x80
	push	acc
	lcall	_printf
	mov	a,sp
	add	a,#0xf5
	mov	sp,a
	pop	ar3
	pop	ar2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:642: while(1)
L031037?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:644: waitms(50);
	mov	dptr,#0x0032
	push	ar2
	push	ar3
	lcall	_waitms
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:646: v[0] = Volts_at_Pin(XPOS_PIN) ;
	mov	dpl,#0x0A
	lcall	_Volts_at_Pin
	mov	r4,dpl
	mov	r5,dph
	mov	r6,b
	mov	r7,a
	mov	_main_v_1_161,r4
	mov	(_main_v_1_161 + 1),r5
	mov	(_main_v_1_161 + 2),r6
	mov	(_main_v_1_161 + 3),r7
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:647: v[1] = Volts_at_Pin(YPOS_PIN) ;
	mov	dpl,#0x0B
	lcall	_Volts_at_Pin
	mov	r4,dpl
	mov	r5,dph
	mov	r6,b
	mov	r7,a
	mov	(_main_v_1_161 + 0x0004),r4
	mov	((_main_v_1_161 + 0x0004) + 1),r5
	mov	((_main_v_1_161 + 0x0004) + 2),r6
	mov	((_main_v_1_161 + 0x0004) + 3),r7
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:649: GetPosition2(v, xy_pos, mid);
	mov	_GetPosition2_PARM_2,#_main_xy_pos_1_161
	mov	(_GetPosition2_PARM_2 + 1),#0x00
	mov	(_GetPosition2_PARM_2 + 2),#0x40
	mov	_GetPosition2_PARM_3,#_main_mid_1_161
	mov	(_GetPosition2_PARM_3 + 1),#0x00
	mov	(_GetPosition2_PARM_3 + 2),#0x40
	mov	dptr,#_main_v_1_161
	mov	b,#0x40
	lcall	_GetPosition2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:651: mapped_dir = matchRange((int) xy_pos[0], (int) xy_pos[1]);
	mov	dpl,_main_xy_pos_1_161
	mov	dph,(_main_xy_pos_1_161 + 1)
	mov	b,(_main_xy_pos_1_161 + 2)
	mov	a,(_main_xy_pos_1_161 + 3)
	lcall	___fs2sint
	mov	r4,dpl
	mov	r5,dph
	mov	dpl,(_main_xy_pos_1_161 + 0x0004)
	mov	dph,((_main_xy_pos_1_161 + 0x0004) + 1)
	mov	b,((_main_xy_pos_1_161 + 0x0004) + 2)
	mov	a,((_main_xy_pos_1_161 + 0x0004) + 3)
	push	ar4
	push	ar5
	lcall	___fs2sint
	mov	_matchRange_PARM_2,dpl
	mov	(_matchRange_PARM_2 + 1),dph
	pop	ar5
	pop	ar4
	mov	dpl,r4
	mov	dph,r5
	lcall	_matchRange
	mov	r4,dpl
	pop	ar3
	pop	ar2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:662: sprintf(buff_x, "x=%.2f, Mlev:%d", xy_pos[0], metal_lev);
	push	ar2
	push	ar3
	push	ar4
	push	ar2
	push	ar3
	push	_main_xy_pos_1_161
	push	(_main_xy_pos_1_161 + 1)
	push	(_main_xy_pos_1_161 + 2)
	push	(_main_xy_pos_1_161 + 3)
	mov	a,#__str_16
	push	acc
	mov	a,#(__str_16 >> 8)
	push	acc
	mov	a,#0x80
	push	acc
	mov	a,#_main_buff_x_1_161
	push	acc
	mov	a,#(_main_buff_x_1_161 >> 8)
	push	acc
	clr	a
	push	acc
	lcall	_sprintf
	mov	a,sp
	add	a,#0xf4
	mov	sp,a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:663: LCDprint(buff_x, 1, 1);
	mov	_LCDprint_PARM_2,#0x01
	setb	_LCDprint_PARM_3
	mov	dptr,#_main_buff_x_1_161
	mov	b,#0x00
	lcall	_LCDprint
	pop	ar4
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:664: sprintf(buff_y, "y=%.2f,pos=%c", xy_pos[1], mapped_dir);
	mov	a,r4
	rlc	a
	subb	a,acc
	mov	r5,a
	push	ar4
	push	ar5
	push	ar4
	push	ar5
	push	(_main_xy_pos_1_161 + 0x0004)
	push	((_main_xy_pos_1_161 + 0x0004) + 1)
	push	((_main_xy_pos_1_161 + 0x0004) + 2)
	push	((_main_xy_pos_1_161 + 0x0004) + 3)
	mov	a,#__str_17
	push	acc
	mov	a,#(__str_17 >> 8)
	push	acc
	mov	a,#0x80
	push	acc
	mov	a,#_main_buff_y_1_161
	push	acc
	mov	a,#(_main_buff_y_1_161 >> 8)
	push	acc
	clr	a
	push	acc
	lcall	_sprintf
	mov	a,sp
	add	a,#0xf4
	mov	sp,a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:665: LCDprint(buff_y, 2, 1);
	mov	_LCDprint_PARM_2,#0x02
	setb	_LCDprint_PARM_3
	mov	dptr,#_main_buff_y_1_161
	mov	b,#0x00
	lcall	_LCDprint
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:667: sprintf(temp_buff, "%c\r\n", mapped_dir);
	mov	a,#__str_18
	push	acc
	mov	a,#(__str_18 >> 8)
	push	acc
	mov	a,#0x80
	push	acc
	mov	a,#_main_temp_buff_1_161
	push	acc
	mov	a,#(_main_temp_buff_1_161 >> 8)
	push	acc
	clr	a
	push	acc
	lcall	_sprintf
	mov	a,sp
	add	a,#0xf8
	mov	sp,a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:668: sendstr1(temp_buff);
	mov	dptr,#_main_temp_buff_1_161
	mov	b,#0x00
	lcall	_sendstr1
	pop	ar3
	pop	ar2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:671: if(P2_1 == 0)
	jb	_P2_1,L031007?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:674: waitms(10);
	mov	dptr,#0x000A
	push	ar2
	push	ar3
	lcall	_waitms
	pop	ar3
	pop	ar2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:675: if(P2_1 == 0)
	jb	_P2_1,L031007?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:677: while(P2_1 == 0);
L031001?:
	jnb	_P2_1,L031001?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:678: printf("--EMERGENCY--\r\n");
	push	ar2
	push	ar3
	mov	a,#__str_19
	push	acc
	mov	a,#(__str_19 >> 8)
	push	acc
	mov	a,#0x80
	push	acc
	lcall	_printf
	dec	sp
	dec	sp
	dec	sp
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:679: sendstr1("€€\r\n");
	mov	dptr,#__str_20
	mov	b,#0x80
	lcall	_sendstr1
	pop	ar3
	pop	ar2
L031007?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:686: waitms(10);
	mov	dptr,#0x000A
	push	ar2
	push	ar3
	lcall	_waitms
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:688: sendstr1("m\r\n");
	mov	dptr,#__str_21
	mov	b,#0x80
	lcall	_sendstr1
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:690: putchar('.');
	mov	dpl,#0x2E
	lcall	_putchar
	pop	ar3
	pop	ar2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:693: while (1) 
	mov	r4,#0x00
	mov	r5,#0x00
L031013?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:695: if (RXU1()) break;
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	lcall	_RXU1
	clr	a
	rlc	a
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
	jnz	L031014?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:696: waitms_or_RI1(1);
	mov	dptr,#0x0001
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	lcall	_waitms_or_RI1
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:697: timeout_cnt++;
	inc	r4
	cjne	r4,#0x00,L031060?
	inc	r5
L031060?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:698: if (timeout_cnt>=500){
	clr	c
	mov	a,r4
	subb	a,#0xF4
	mov	a,r5
	xrl	a,#0x80
	subb	a,#0x81
	jc	L031013?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:699: printf("timeout \r\n");
	push	ar2
	push	ar3
	mov	a,#__str_22
	push	acc
	mov	a,#(__str_22 >> 8)
	push	acc
	mov	a,#0x80
	push	acc
	lcall	_printf
	dec	sp
	dec	sp
	dec	sp
	pop	ar3
	pop	ar2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:700: break;
L031014?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:703: if (RXU1()) 
	push	ar2
	push	ar3
	lcall	_RXU1
	pop	ar3
	pop	ar2
	jc	L031062?
	ljmp	L031037?
L031062?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:705: getstr1(buff);
	mov	dptr,#_buff
	mov	b,#0x40
	push	ar2
	push	ar3
	lcall	_getstr1
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:706: printf("%s\r\n", buff);
	mov	a,#_buff
	push	acc
	mov	a,#(_buff >> 8)
	push	acc
	mov	a,#0x40
	push	acc
	mov	a,#__str_23
	push	acc
	mov	a,#(__str_23 >> 8)
	push	acc
	mov	a,#0x80
	push	acc
	lcall	_printf
	mov	a,sp
	add	a,#0xfa
	mov	sp,a
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:707: if(strlen(buff) == 2)
	mov	dptr,#_buff
	mov	b,#0x40
	lcall	_strlen
	mov	r4,dpl
	mov	r5,dph
	pop	ar3
	pop	ar2
	cjne	r4,#0x02,L031063?
	cjne	r5,#0x00,L031063?
	sjmp	L031064?
L031063?:
	ljmp	L031037?
L031064?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:709: if(buff[0] != 'z')
	mov	r0,#_buff
	mov	ar4,@r0
	cjne	r4,#0x7A,L031065?
	sjmp	L031030?
L031065?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:711: if(buff[0] == 'a') {
	cjne	r4,#0x61,L031027?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:712: loadTimer2(metalLevel_1);
	mov	dptr,#0x012C
	clr	a
	mov	b,a
	lcall	_loadTimer2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:713: metal_lev = 1;
	mov	r2,#0x01
	mov	r3,#0x00
	ljmp	L031037?
L031027?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:715: else if(buff[0] == 'b') {
	cjne	r4,#0x62,L031024?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:716: loadTimer2(metalLevel_2);
	mov	dptr,#0x0190
	clr	a
	mov	b,a
	lcall	_loadTimer2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:717: metal_lev = 2;
	mov	r2,#0x02
	mov	r3,#0x00
	ljmp	L031037?
L031024?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:719: else if(buff[0] == 'c') {
	cjne	r4,#0x63,L031021?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:720: loadTimer2(metalLevel_3);
	mov	dptr,#0x01F4
	clr	a
	mov	b,a
	lcall	_loadTimer2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:721: metal_lev = 3;
	mov	r2,#0x03
	mov	r3,#0x00
	ljmp	L031037?
L031021?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:723: else if(buff[0] == 'd') {
	cjne	r4,#0x64,L031018?
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:724: loadTimer2(metalLevel_4);
	mov	dptr,#0x0258
	clr	a
	mov	b,a
	lcall	_loadTimer2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:725: metal_lev = 4;
	mov	r2,#0x04
	mov	r3,#0x00
	ljmp	L031037?
L031018?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:727: else if(buff[0] == 'e') {
	cjne	r4,#0x65,L031074?
	sjmp	L031075?
L031074?:
	ljmp	L031037?
L031075?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:728: loadTimer2(metalLevel_5);
	mov	dptr,#0x02BC
	clr	a
	mov	b,a
	lcall	_loadTimer2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:729: metal_lev = 5;
	mov	r2,#0x05
	mov	r3,#0x00
	ljmp	L031037?
L031030?:
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:734: TR2 = 0; 		// Stop timer 2
	clr	_TR2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:735: BUZZER_OUT = 0;
	clr	_P0_2
;	C:\Users\keybo\Documents\GitHub\Project2ELEC291\integrated_files\EFM8integration.c:736: metal_lev = 0;
	mov	r2,#0x00
	mov	r3,#0x00
	ljmp	L031037?
	rseg R_CSEG

	rseg R_XINIT

	rseg R_CONST
__str_0:
	db 'Command: %s'
	db 0x00
__str_1:
	db 'Response: %s'
	db 0x0D
	db 0x0A
	db 0x00
__str_2:
	db 0x1B
	db '[2J'
	db 0x00
__str_3:
	db 'Metal Detecting Robot Controller'
	db 0x0A
	db 'File: %s'
	db 0x0A
	db 'Compiled: %s, %s'
	db 0x0A
	db 0x0A
	db 0x00
__str_4:
	db 'C:'
	db 0x5C
	db 'Users'
	db 0x5C
	db 'keybo'
	db 0x5C
	db 'Documents'
	db 0x5C
	db 'GitHub'
	db 0x5C
	db 'Project2ELEC291'
	db 0x5C
	db 'integrated_f'
	db 'iles'
	db 0x5C
	db 'EFM8integration.c'
	db 0x00
__str_5:
	db 'Apr  2 2024'
	db 0x00
__str_6:
	db '10:26:35'
	db 0x00
__str_7:
	db 'AT+DVIDA0A0'
	db 0x0D
	db 0x0A
	db 0x00
__str_8:
	db 'AT+VER'
	db 0x0D
	db 0x0A
	db 0x00
__str_9:
	db 'AT+BAUD'
	db 0x0D
	db 0x0A
	db 0x00
__str_10:
	db 'AT+RFID'
	db 0x0D
	db 0x0A
	db 0x00
__str_11:
	db 'AT+DVID'
	db 0x0D
	db 0x0A
	db 0x00
__str_12:
	db 'AT+RFC'
	db 0x0D
	db 0x0A
	db 0x00
__str_13:
	db 'AT+POWE'
	db 0x0D
	db 0x0A
	db 0x00
__str_14:
	db 'AT+CLSS'
	db 0x0D
	db 0x0A
	db 0x00
__str_15:
	db 'LED voltage ref: 1.7854, midXvolts: %f, midYvolts: %f '
	db 0x0D
	db 0x0A
	db 0x00
__str_16:
	db 'x=%.2f, Mlev:%d'
	db 0x00
__str_17:
	db 'y=%.2f,pos=%c'
	db 0x00
__str_18:
	db '%c'
	db 0x0D
	db 0x0A
	db 0x00
__str_19:
	db '--EMERGENCY--'
	db 0x0D
	db 0x0A
	db 0x00
__str_20:
	db 0xE2
	db 0x82
	db 0xAC
	db 0xE2
	db 0x82
	db 0xAC
	db 0x0D
	db 0x0A
	db 0x00
__str_21:
	db 'm'
	db 0x0D
	db 0x0A
	db 0x00
__str_22:
	db 'timeout '
	db 0x0D
	db 0x0A
	db 0x00
__str_23:
	db '%s'
	db 0x0D
	db 0x0A
	db 0x00

	CSEG

end

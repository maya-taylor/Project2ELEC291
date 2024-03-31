;--------------------------------------------------------
; File Created by C51
; Version 1.0.0 #1170 (Feb 16 2022) (MSVC)
; This file was generated Sun Mar 31 12:14:52 2024
;--------------------------------------------------------
$name All_timers
$optc51 --model-small
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
	public _main
	public _PCA_ISR
	public _Timer5_ISR
	public _Timer4_ISR
	public _Timer3_ISR
	public _Timer2_ISR
	public _Timer1_ISR
	public _Timer0_ISR
	public __c51_external_startup
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
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
	rseg	R_OSEG
;--------------------------------------------------------
; indirectly addressable internal ram data
;--------------------------------------------------------
	rseg R_ISEG
;--------------------------------------------------------
; absolute internal ram data
;--------------------------------------------------------
	DSEG
;--------------------------------------------------------
; bit data
;--------------------------------------------------------
	rseg R_BSEG
;--------------------------------------------------------
; paged external ram data
;--------------------------------------------------------
	rseg R_PSEG
;--------------------------------------------------------
; external ram data
;--------------------------------------------------------
	rseg R_XSEG
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
	CSEG at 0x000b
	ljmp	_Timer0_ISR
	CSEG at 0x001b
	ljmp	_Timer1_ISR
	CSEG at 0x002b
	ljmp	_Timer2_ISR
	CSEG at 0x005b
	ljmp	_PCA_ISR
	CSEG at 0x0073
	ljmp	_Timer3_ISR
	CSEG at 0x008b
	ljmp	_Timer4_ISR
	CSEG at 0x0093
	ljmp	_Timer5_ISR
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
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:33: char _c51_external_startup (void)
;	-----------------------------------------
;	 function _c51_external_startup
;	-----------------------------------------
__c51_external_startup:
	using	0
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:36: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:37: WDTCN = 0xDE; //First key
	mov	_WDTCN,#0xDE
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:38: WDTCN = 0xAD; //Second key
	mov	_WDTCN,#0xAD
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:40: VDM0CN=0x80;       // enable VDD monitor
	mov	_VDM0CN,#0x80
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:41: RSTSRC=0x02|0x04;  // Enable reset on missing clock detector and VDD
	mov	_RSTSRC,#0x06
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:48: SFRPAGE = 0x10;
	mov	_SFRPAGE,#0x10
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:49: PFE0CN  = 0x20; // SYSCLK < 75 MHz.
	mov	_PFE0CN,#0x20
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:50: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:71: CLKSEL = 0x00;
	mov	_CLKSEL,#0x00
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:72: CLKSEL = 0x00;
	mov	_CLKSEL,#0x00
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:73: while ((CLKSEL & 0x80) == 0);
L002001?:
	mov	a,_CLKSEL
	jnb	acc.7,L002001?
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:74: CLKSEL = 0x03;
	mov	_CLKSEL,#0x03
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:75: CLKSEL = 0x03;
	mov	_CLKSEL,#0x03
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:76: while ((CLKSEL & 0x80) == 0);
L002004?:
	mov	a,_CLKSEL
	jnb	acc.7,L002004?
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:82: P0MDOUT|=0b_1100_0010;
	orl	_P0MDOUT,#0xC2
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:83: P1MDOUT|=0b_1111_1111;
	mov	a,_P1MDOUT
	mov	_P1MDOUT,#0xFF
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:84: P2MDOUT|=0b_0000_0001;
	orl	_P2MDOUT,#0x01
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:86: XBR0     = 0x00;                     
	mov	_XBR0,#0x00
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:87: XBR1     = 0X00;
	mov	_XBR1,#0x00
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:88: XBR2     = 0x40; // Enable crossbar and weak pull-ups
	mov	_XBR2,#0x40
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:91: TR0=0;
	clr	_TR0
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:92: TF0=0;
	clr	_TF0
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:93: CKCON0|=0b_0000_0100; // Timer 0 uses the system clock
	orl	_CKCON0,#0x04
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:94: TMOD&=0xf0;
	anl	_TMOD,#0xF0
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:95: TMOD|=0x01; // Timer 0 in mode 1: 16-bit timer
	orl	_TMOD,#0x01
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:97: TMR0=65536L-(SYSCLK/(2*TIMER_0_FREQ));
	mov	_TMR0,#0x60
	mov	(_TMR0 >> 8),#0x73
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:98: ET0=1;     // Enable Timer0 interrupts
	setb	_ET0
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:99: TR0=1;     // Start Timer0
	setb	_TR0
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:102: TR1=0;
	clr	_TR1
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:103: TF1=0;
	clr	_TF1
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:104: CKCON0|=0b_0000_1000; // Timer 1 uses the system clock
	orl	_CKCON0,#0x08
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:105: TMOD&=0x0f;
	anl	_TMOD,#0x0F
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:106: TMOD|=0x10; // Timer 1 in mode 1: 16-bit timer
	orl	_TMOD,#0x10
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:108: TMR1=65536L-(SYSCLK/(2*TIMER_1_FREQ));
	mov	_TMR1,#0xB0
	mov	(_TMR1 >> 8),#0xB9
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:109: ET1=1;     // Enable Timer1 interrupts
	setb	_ET1
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:110: TR1=1;     // Start Timer1
	setb	_TR1
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:113: TMR2CN0=0x00;   // Stop Timer2; Clear TF2;
	mov	_TMR2CN0,#0x00
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:114: CKCON0|=0b_0001_0000; // Timer 2 uses the system clock
	orl	_CKCON0,#0x10
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:115: TMR2RL=(0x10000L-(SYSCLK/(2*TIMER_2_FREQ))); // Initialize reload value
	mov	_TMR2RL,#0x20
	mov	(_TMR2RL >> 8),#0xD1
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:116: TMR2=0xffff;   // Set to reload immediately
	mov	_TMR2,#0xFF
	mov	(_TMR2 >> 8),#0xFF
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:117: ET2=1;         // Enable Timer2 interrupts
	setb	_ET2
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:118: TR2=1;         // Start Timer2 (TMR2CN is bit addressable)
	setb	_TR2
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:121: TMR3CN0=0x00;   // Stop Timer3; Clear TF3;
	mov	_TMR3CN0,#0x00
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:122: CKCON0|=0b_0100_0000; // Timer 3 uses the system clock
	orl	_CKCON0,#0x40
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:123: TMR3RL=(0x10000L-(SYSCLK/(2*TIMER_3_FREQ))); // Initialize reload value
	mov	_TMR3RL,#0xD8
	mov	(_TMR3RL >> 8),#0xDC
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:124: TMR3=0xffff;   // Set to reload immediately
	mov	_TMR3,#0xFF
	mov	(_TMR3 >> 8),#0xFF
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:125: EIE1|=0b_1000_0000;     // Enable Timer3 interrupts
	orl	_EIE1,#0x80
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:126: TMR3CN0|=0b_0000_0100;  // Start Timer3 (TMR3CN0 is not bit addressable)
	orl	_TMR3CN0,#0x04
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:129: SFRPAGE=0x10;
	mov	_SFRPAGE,#0x10
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:130: TMR4CN0=0x00;   // Stop Timer4; Clear TF4; WARNING: lives in SFR page 0x10
	mov	_TMR4CN0,#0x00
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:131: CKCON1|=0b_0000_0001; // Timer 4 uses the system clock
	orl	_CKCON1,#0x01
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:132: TMR4RL=(0x10000L-(SYSCLK/(2*TIMER_4_FREQ))); // Initialize reload value
	mov	_TMR4RL,#0xE0
	mov	(_TMR4RL >> 8),#0xE3
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:133: TMR4=0xffff;   // Set to reload immediately
	mov	_TMR4,#0xFF
	mov	(_TMR4 >> 8),#0xFF
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:134: EIE2|=0b_0000_0100;     // Enable Timer4 interrupts
	orl	_EIE2,#0x04
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:135: TR4=1;
	setb	_TR4
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:138: SFRPAGE=0x10;
	mov	_SFRPAGE,#0x10
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:139: TMR5CN0=0x00;   // Stop Timer5; Clear TF5; WARNING: lives in SFR page 0x10
	mov	_TMR5CN0,#0x00
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:140: CKCON1|=0b_0000_0100; // Timer 5 uses the system clock
	orl	_CKCON1,#0x04
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:141: TMR5RL=(0x10000L-(SYSCLK/(2*TIMER_5_FREQ))); // Initialize reload value
	mov	_TMR5RL,#0x6C
	mov	(_TMR5RL >> 8),#0xEE
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:142: TMR5=0xffff;   // Set to reload immediately
	mov	_TMR5,#0xFF
	mov	(_TMR5 >> 8),#0xFF
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:143: EIE2|=0b_0000_1000; // Enable Timer5 interrupts
	orl	_EIE2,#0x08
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:144: TR5=1;         // Start Timer5 (TMR5CN0 is bit addressable)
	setb	_TR5
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:147: SFRPAGE=0x0;
	mov	_SFRPAGE,#0x00
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:148: PCA0MD=0x00; // Disable and clear everything in the PCA
	mov	_PCA0MD,#0x00
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:149: PCA0L=0; // Initialize the PCA counter to zero
	mov	_PCA0L,#0x00
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:150: PCA0H=0;
	mov	_PCA0H,#0x00
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:151: PCA0MD=0b_0000_1000; // Configure PCA.  System CLK is the frequency input for the PCA
	mov	_PCA0MD,#0x08
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:153: PCA0CPM0=PCA0CPM1=PCA0CPM2=PCA0CPM3=PCA0CPM4=0b_0100_1001; // ECOM|MAT|ECCF;
	mov	_PCA0CPM4,#0x49
	mov	_PCA0CPM3,#0x49
	mov	_PCA0CPM2,#0x49
	mov	_PCA0CPM1,#0x49
	mov	_PCA0CPM0,#0x49
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:155: PCA0CPL0=(SYSCLK/(2*PCA_0_FREQ))%0x100; //Always write low byte first!
	mov	_PCA0CPL0,#0x16
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:156: PCA0CPH0=(SYSCLK/(2*PCA_0_FREQ))/0x100;
	mov	_PCA0CPH0,#0x14
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:158: PCA0CPL1=(SYSCLK/(2*PCA_1_FREQ))%0x100; //Always write low byte first!
	mov	_PCA0CPL1,#0x94
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:159: PCA0CPH1=(SYSCLK/(2*PCA_1_FREQ))/0x100;
	mov	_PCA0CPH1,#0x11
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:161: PCA0CPL2=(SYSCLK/(2*PCA_2_FREQ))%0x100; //Always write low byte first!
	mov	_PCA0CPL2,#0xA0
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:162: PCA0CPH2=(SYSCLK/(2*PCA_2_FREQ))/0x100;
	mov	_PCA0CPH2,#0x0F
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:164: PCA0CPL3=(SYSCLK/(2*PCA_3_FREQ))%0x100; //Always write low byte first!
	mov	_PCA0CPL3,#0x10
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:165: PCA0CPH3=(SYSCLK/(2*PCA_3_FREQ))/0x100;
	mov	_PCA0CPH3,#0x0E
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:167: PCA0CPL4=(SYSCLK/(2*PCA_4_FREQ))%0x100; //Always write low byte first!
	mov	_PCA0CPL4,#0xC8
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:168: PCA0CPH4=(SYSCLK/(2*PCA_4_FREQ))/0x100;
	mov	_PCA0CPH4,#0x0C
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:169: CR=1; // Enable PCA counter
	setb	_CR
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:170: EIE1|=0b_0001_0000; // Enable PCA interrupts
	orl	_EIE1,#0x10
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:172: EA=1; // Enable interrupts
	setb	_EA
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:174: return 0;
	mov	dpl,#0x00
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'Timer0_ISR'
;------------------------------------------------------------
;------------------------------------------------------------
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:177: void Timer0_ISR (void) interrupt INTERRUPT_TIMER0
;	-----------------------------------------
;	 function Timer0_ISR
;	-----------------------------------------
_Timer0_ISR:
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:179: SFRPAGE=0x0;
	mov	_SFRPAGE,#0x00
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:181: TMR0=0x10000L-(SYSCLK/(2*TIMER_0_FREQ));
	mov	_TMR0,#0x60
	mov	(_TMR0 >> 8),#0x73
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:182: TIMER_OUT_0=!TIMER_OUT_0;
	cpl	_P2_0
	reti
;	eliminated unneeded push/pop psw
;	eliminated unneeded push/pop dpl
;	eliminated unneeded push/pop dph
;	eliminated unneeded push/pop b
;	eliminated unneeded push/pop acc
;------------------------------------------------------------
;Allocation info for local variables in function 'Timer1_ISR'
;------------------------------------------------------------
;------------------------------------------------------------
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:185: void Timer1_ISR (void) interrupt INTERRUPT_TIMER1
;	-----------------------------------------
;	 function Timer1_ISR
;	-----------------------------------------
_Timer1_ISR:
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:187: SFRPAGE=0x0;
	mov	_SFRPAGE,#0x00
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:189: TMR1=0x10000L-(SYSCLK/(2*TIMER_1_FREQ));
	mov	_TMR1,#0xB0
	mov	(_TMR1 >> 8),#0xB9
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:190: TIMER_OUT_1=!TIMER_OUT_1;
	cpl	_P1_7
	reti
;	eliminated unneeded push/pop psw
;	eliminated unneeded push/pop dpl
;	eliminated unneeded push/pop dph
;	eliminated unneeded push/pop b
;	eliminated unneeded push/pop acc
;------------------------------------------------------------
;Allocation info for local variables in function 'Timer2_ISR'
;------------------------------------------------------------
;------------------------------------------------------------
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:193: void Timer2_ISR (void) interrupt INTERRUPT_TIMER2
;	-----------------------------------------
;	 function Timer2_ISR
;	-----------------------------------------
_Timer2_ISR:
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:195: SFRPAGE=0x0;
	mov	_SFRPAGE,#0x00
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:196: TF2H = 0; // Clear Timer2 interrupt flag
	clr	_TF2H
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:197: TIMER_OUT_2=!TIMER_OUT_2;
	cpl	_P1_6
	reti
;	eliminated unneeded push/pop psw
;	eliminated unneeded push/pop dpl
;	eliminated unneeded push/pop dph
;	eliminated unneeded push/pop b
;	eliminated unneeded push/pop acc
;------------------------------------------------------------
;Allocation info for local variables in function 'Timer3_ISR'
;------------------------------------------------------------
;------------------------------------------------------------
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:200: void Timer3_ISR (void) interrupt INTERRUPT_TIMER3
;	-----------------------------------------
;	 function Timer3_ISR
;	-----------------------------------------
_Timer3_ISR:
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:202: SFRPAGE=0x0;
	mov	_SFRPAGE,#0x00
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:203: TMR3CN0&=0b_0011_1111; // Clear Timer3 interrupt flags
	anl	_TMR3CN0,#0x3F
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:204: TIMER_OUT_3=!TIMER_OUT_3;
	cpl	_P1_5
	reti
;	eliminated unneeded push/pop psw
;	eliminated unneeded push/pop dpl
;	eliminated unneeded push/pop dph
;	eliminated unneeded push/pop b
;	eliminated unneeded push/pop acc
;------------------------------------------------------------
;Allocation info for local variables in function 'Timer4_ISR'
;------------------------------------------------------------
;------------------------------------------------------------
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:207: void Timer4_ISR (void) interrupt INTERRUPT_TIMER4
;	-----------------------------------------
;	 function Timer4_ISR
;	-----------------------------------------
_Timer4_ISR:
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:209: SFRPAGE=0x10;
	mov	_SFRPAGE,#0x10
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:210: TF4H = 0; // Clear Timer4 interrupt flag
	clr	_TF4H
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:211: TIMER_OUT_4=!TIMER_OUT_4;
	cpl	_P1_4
	reti
;	eliminated unneeded push/pop psw
;	eliminated unneeded push/pop dpl
;	eliminated unneeded push/pop dph
;	eliminated unneeded push/pop b
;	eliminated unneeded push/pop acc
;------------------------------------------------------------
;Allocation info for local variables in function 'Timer5_ISR'
;------------------------------------------------------------
;------------------------------------------------------------
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:214: void Timer5_ISR (void) interrupt INTERRUPT_TIMER5
;	-----------------------------------------
;	 function Timer5_ISR
;	-----------------------------------------
_Timer5_ISR:
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:216: SFRPAGE=0x10;
	mov	_SFRPAGE,#0x10
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:217: TF5H = 0; // Clear Timer5 interrupt flag
	clr	_TF5H
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:218: TIMER_OUT_5=!TIMER_OUT_5;
	cpl	_P0_2
	reti
;	eliminated unneeded push/pop psw
;	eliminated unneeded push/pop dpl
;	eliminated unneeded push/pop dph
;	eliminated unneeded push/pop b
;	eliminated unneeded push/pop acc
;------------------------------------------------------------
;Allocation info for local variables in function 'PCA_ISR'
;------------------------------------------------------------
;j                         Allocated to registers r2 r3 
;------------------------------------------------------------
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:221: void PCA_ISR (void) interrupt INTERRUPT_PCA0
;	-----------------------------------------
;	 function PCA_ISR
;	-----------------------------------------
_PCA_ISR:
	push	acc
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	push	ar6
	push	ar7
	push	psw
	mov	psw,#0x00
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:225: SFRPAGE=0x0;
	mov	_SFRPAGE,#0x00
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:227: if (CCF0)
	jnb	_CCF0,L009002?
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:229: j=(PCA0CPH0*0x100+PCA0CPL0)+(SYSCLK/(2L*PCA_0_FREQ));
	mov	r3,_PCA0CPH0
	mov	r2,#0x00
	mov	r4,_PCA0CPL0
	mov	r5,#0x00
	mov	a,r4
	add	a,r2
	mov	r2,a
	mov	a,r5
	addc	a,r3
	mov	r3,a
	rlc	a
	subb	a,acc
	mov	r4,a
	mov	r5,a
	mov	a,#0x16
	add	a,r2
	mov	r2,a
	mov	a,#0x14
	addc	a,r3
	mov	r3,a
	clr	a
	addc	a,r4
	clr	a
	addc	a,r5
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:230: PCA0CPL0=j%0x100; //Always write low byte first!
	mov	ar4,r2
	mov	_PCA0CPL0,r4
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:231: PCA0CPH0=j/0x100;
	mov	ar4,r3
	mov	r5,#0x00
	mov	_PCA0CPH0,r4
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:232: CCF0=0;
	clr	_CCF0
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:233: PCA_OUT_0=!PCA_OUT_0;
	cpl	_P1_2
L009002?:
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:235: if (CCF1)
	jnb	_CCF1,L009004?
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:237: j=(PCA0CPH1*0x100+PCA0CPL1)+(SYSCLK/(2L*PCA_1_FREQ));
	mov	r5,_PCA0CPH1
	mov	r4,#0x00
	mov	r6,_PCA0CPL1
	mov	r7,#0x00
	mov	a,r6
	add	a,r4
	mov	r4,a
	mov	a,r7
	addc	a,r5
	mov	r5,a
	rlc	a
	subb	a,acc
	mov	r6,a
	mov	r7,a
	mov	a,#0x94
	add	a,r4
	mov	r4,a
	mov	a,#0x11
	addc	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	clr	a
	addc	a,r7
	mov	r7,a
	mov	ar2,r4
	mov	ar3,r5
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:238: PCA0CPL1=j%0x100; //Always write low byte first!
	mov	ar4,r2
	mov	_PCA0CPL1,r4
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:239: PCA0CPH1=j/0x100;
	mov	ar4,r3
	mov	r5,#0x00
	mov	_PCA0CPH1,r4
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:240: CCF1=0;
	clr	_CCF1
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:241: PCA_OUT_1=!PCA_OUT_1;
	cpl	_P1_1
L009004?:
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:243: if (CCF2)
	jnb	_CCF2,L009006?
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:245: j=(PCA0CPH2*0x100+PCA0CPL2)+(SYSCLK/(2L*PCA_2_FREQ));
	mov	r5,_PCA0CPH2
	mov	r4,#0x00
	mov	r6,_PCA0CPL2
	mov	r7,#0x00
	mov	a,r6
	add	a,r4
	mov	r4,a
	mov	a,r7
	addc	a,r5
	mov	r5,a
	rlc	a
	subb	a,acc
	mov	r6,a
	mov	r7,a
	mov	a,#0xA0
	add	a,r4
	mov	r4,a
	mov	a,#0x0F
	addc	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	clr	a
	addc	a,r7
	mov	r7,a
	mov	ar2,r4
	mov	ar3,r5
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:246: PCA0CPL2=j%0x100; //Always write low byte first!
	mov	ar4,r2
	mov	_PCA0CPL2,r4
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:247: PCA0CPH2=j/0x100;
	mov	ar4,r3
	mov	r5,#0x00
	mov	_PCA0CPH2,r4
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:248: CCF2=0;
	clr	_CCF2
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:249: PCA_OUT_2=!PCA_OUT_2;
	cpl	_P1_0
L009006?:
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:251: if (CCF3)
	jnb	_CCF3,L009008?
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:253: j=(PCA0CPH3*0x100+PCA0CPL3)+(SYSCLK/(2L*PCA_3_FREQ));
	mov	r5,_PCA0CPH3
	mov	r4,#0x00
	mov	r6,_PCA0CPL3
	mov	r7,#0x00
	mov	a,r6
	add	a,r4
	mov	r4,a
	mov	a,r7
	addc	a,r5
	mov	r5,a
	rlc	a
	subb	a,acc
	mov	r6,a
	mov	r7,a
	mov	a,#0x10
	add	a,r4
	mov	r4,a
	mov	a,#0x0E
	addc	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	clr	a
	addc	a,r7
	mov	r7,a
	mov	ar2,r4
	mov	ar3,r5
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:254: PCA0CPL3=j%0x100; //Always write low byte first!
	mov	ar4,r2
	mov	_PCA0CPL3,r4
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:255: PCA0CPH3=j/0x100;
	mov	ar4,r3
	mov	r5,#0x00
	mov	_PCA0CPH3,r4
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:256: CCF3=0;
	clr	_CCF3
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:257: PCA_OUT_3=!PCA_OUT_3;
	cpl	_P0_7
L009008?:
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:259: if (CCF4)
	jnb	_CCF4,L009010?
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:261: j=(PCA0CPH4*0x100+PCA0CPL4)+(SYSCLK/(2L*PCA_4_FREQ));
	mov	r5,_PCA0CPH4
	mov	r4,#0x00
	mov	r6,_PCA0CPL4
	mov	r7,#0x00
	mov	a,r6
	add	a,r4
	mov	r4,a
	mov	a,r7
	addc	a,r5
	mov	r5,a
	rlc	a
	subb	a,acc
	mov	r6,a
	mov	r7,a
	mov	a,#0xC8
	add	a,r4
	mov	r4,a
	mov	a,#0x0C
	addc	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	clr	a
	addc	a,r7
	mov	r7,a
	mov	ar2,r4
	mov	ar3,r5
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:262: PCA0CPL4=j%0x100; //Always write low byte first!
	mov	ar4,r2
	mov	r5,#0x00
	mov	_PCA0CPL4,r4
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:263: PCA0CPH4=j/0x100;
	mov	ar2,r3
	mov	r3,#0x00
	mov	_PCA0CPH4,r2
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:264: CCF4=0;
	clr	_CCF4
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:265: PCA_OUT_4=!PCA_OUT_4;
	cpl	_P0_6
L009010?:
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:268: CF=0;
	clr	_CF
	pop	psw
	pop	ar7
	pop	ar6
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
	pop	acc
	reti
;	eliminated unneeded push/pop dpl
;	eliminated unneeded push/pop dph
;	eliminated unneeded push/pop b
;------------------------------------------------------------
;Allocation info for local variables in function 'main'
;------------------------------------------------------------
;j                         Allocated to registers r2 r3 
;------------------------------------------------------------
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:271: void main (void)
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:274: while(1)
L010002?:
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:277: MAIN_OUT=!MAIN_OUT;
	cpl	_P0_1
;	C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c:278: for (j=0; j<1000; j++);
	mov	r2,#0xE8
	mov	r3,#0x03
L010006?:
	dec	r2
	cjne	r2,#0xff,L010013?
	dec	r3
L010013?:
	mov	a,r2
	orl	a,r3
	jnz	L010006?
	sjmp	L010002?
	rseg R_CSEG

	rseg R_XINIT

	rseg R_CONST

	CSEG

end

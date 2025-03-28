;--------------------------------------------------------
; File Created by C51
; Version 1.0.0 #1170 (Feb 16 2022) (MSVC)
; This file was generated Thu Mar 27 20:14:45 2025
;--------------------------------------------------------
$name EFM8_JDY40_test
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
	public _InitPinADC_PARM_2
	public _main
	public _InitPushButton
	public _Volts_at_Pin
	public _ADC_at_Pin
	public _InitPinADC
	public _ReceptionOff
	public _SendATCommand
	public _waitms_or_RI1
	public _RXU1
	public _getstr1
	public _getchar1_with_timeout
	public _getchar1
	public _sendstr1
	public _putchar1
	public _UART1_Init
	public _waitms
	public _Timer3us
	public _InitADC
	public __c51_external_startup
	public _buff
	public _getstr1_PARM_2
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
_getstr1_PARM_2:
	ds 1
_main_v_1_131:
	ds 16
_main_norm_x_1_131:
	ds 4
_main_norm_y_1_131:
	ds 4
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
	rseg	R_OSEG
	rseg	R_OSEG
	rseg	R_OSEG
	rseg	R_OSEG
_InitPinADC_PARM_2:
	ds 1
	rseg	R_OSEG
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
_main_button_1_state_1_131:
	DBIT	1
_main_button_2_state_1_131:
	DBIT	1
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
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:15: char _c51_external_startup (void)
;	-----------------------------------------
;	 function _c51_external_startup
;	-----------------------------------------
__c51_external_startup:
	using	0
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:18: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:19: WDTCN = 0xDE; //First key
	mov	_WDTCN,#0xDE
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:20: WDTCN = 0xAD; //Second key
	mov	_WDTCN,#0xAD
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:22: VDM0CN=0x80;       // enable VDD monitor
	mov	_VDM0CN,#0x80
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:23: RSTSRC=0x02|0x04;  // Enable reset on missing clock detector and VDD
	mov	_RSTSRC,#0x06
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:30: SFRPAGE = 0x10;
	mov	_SFRPAGE,#0x10
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:31: PFE0CN  = 0x20; // SYSCLK < 75 MHz.
	mov	_PFE0CN,#0x20
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:32: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:53: CLKSEL = 0x00;
	mov	_CLKSEL,#0x00
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:54: CLKSEL = 0x00;
	mov	_CLKSEL,#0x00
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:55: while ((CLKSEL & 0x80) == 0);
L002001?:
	mov	a,_CLKSEL
	jnb	acc.7,L002001?
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:56: CLKSEL = 0x03;
	mov	_CLKSEL,#0x03
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:57: CLKSEL = 0x03;
	mov	_CLKSEL,#0x03
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:58: while ((CLKSEL & 0x80) == 0);
L002004?:
	mov	a,_CLKSEL
	jnb	acc.7,L002004?
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:63: P0MDOUT |= 0x11; // Enable UART0 TX (P0.4) and UART1 TX (P0.0) as push-pull outputs
	orl	_P0MDOUT,#0x11
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:64: P2MDOUT |= 0x01; // P2.0 in push-pull mode
	orl	_P2MDOUT,#0x01
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:65: XBR0     = 0x01; // Enable UART0 on P0.4(TX) and P0.5(RX)                     
	mov	_XBR0,#0x01
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:66: XBR1     = 0X00;
	mov	_XBR1,#0x00
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:67: XBR2     = 0x41; // Enable crossbar and uart 1
	mov	_XBR2,#0x41
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:73: SCON0 = 0x10;
	mov	_SCON0,#0x10
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:74: TH1 = 0x100-((SYSCLK/BAUDRATE)/(2L*12L));
	mov	_TH1,#0xE6
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:75: TL1 = TH1;      // Init Timer1
	mov	_TL1,_TH1
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:76: TMOD &= ~0xf0;  // TMOD: timer 1 in 8-bit auto-reload
	anl	_TMOD,#0x0F
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:77: TMOD |=  0x20;                       
	orl	_TMOD,#0x20
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:78: TR1 = 1; // START Timer1
	setb	_TR1
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:79: TI = 1;  // Indicate TX0 ready
	setb	_TI
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:81: return 0;
	mov	dpl,#0x00
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'InitADC'
;------------------------------------------------------------
;------------------------------------------------------------
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:84: void InitADC (void)
;	-----------------------------------------
;	 function InitADC
;	-----------------------------------------
_InitADC:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:86: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:87: ADEN=0; // Disable ADC
	clr	_ADEN
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:92: (0x0 << 0) ; // Accumulate n conversions: 0x0: 1, 0x1:4, 0x2:8, 0x3:16, 0x4:32
	mov	_ADC0CN1,#0x80
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:96: (0x0 << 2); // 0:SYSCLK ADCCLK = SYSCLK. 1:HFOSC0 ADCCLK = HFOSC0.
	mov	_ADC0CF0,#0x20
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:100: (0x1E << 0); // Conversion Tracking Time. Tadtk = ADTK / (Fsarclk)
	mov	_ADC0CF1,#0x1E
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:109: (0x0 << 0) ; // TEMPE. 0: Disable the Temperature Sensor. 1: Enable the Temperature Sensor.
	mov	_ADC0CN0,#0x00
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:114: (0x1F << 0); // ADPWR. Power Up Delay Time. Tpwrtime = ((4 * (ADPWR + 1)) + 2) / (Fadcclk)
	mov	_ADC0CF2,#0x3F
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:118: (0x0 << 0) ; // ADCM. 0x0: ADBUSY, 0x1: TIMER0, 0x2: TIMER2, 0x3: TIMER3, 0x4: CNVSTR, 0x5: CEX5, 0x6: TIMER4, 0x7: TIMER5, 0x8: CLU0, 0x9: CLU1, 0xA: CLU2, 0xB: CLU3
	mov	_ADC0CN2,#0x00
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:120: ADEN=1; // Enable ADC
	setb	_ADEN
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'Timer3us'
;------------------------------------------------------------
;us                        Allocated to registers r2 
;i                         Allocated to registers r3 
;------------------------------------------------------------
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:124: void Timer3us(unsigned char us)
;	-----------------------------------------
;	 function Timer3us
;	-----------------------------------------
_Timer3us:
	mov	r2,dpl
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:129: CKCON0|=0b_0100_0000;
	orl	_CKCON0,#0x40
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:131: TMR3RL = (-(SYSCLK)/1000000L); // Set Timer3 to overflow in 1us.
	mov	_TMR3RL,#0xB8
	mov	(_TMR3RL >> 8),#0xFF
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:132: TMR3 = TMR3RL;                 // Initialize Timer3 for first overflow
	mov	_TMR3,_TMR3RL
	mov	(_TMR3 >> 8),(_TMR3RL >> 8)
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:134: TMR3CN0 = 0x04;                 // Sart Timer3 and clear overflow flag
	mov	_TMR3CN0,#0x04
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:135: for (i = 0; i < us; i++)       // Count <us> overflows
	mov	r3,#0x00
L004004?:
	clr	c
	mov	a,r3
	subb	a,r2
	jnc	L004007?
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:137: while (!(TMR3CN0 & 0x80));  // Wait for overflow
L004001?:
	mov	a,_TMR3CN0
	jnb	acc.7,L004001?
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:138: TMR3CN0 &= ~(0x80);         // Clear overflow indicator
	anl	_TMR3CN0,#0x7F
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:135: for (i = 0; i < us; i++)       // Count <us> overflows
	inc	r3
	sjmp	L004004?
L004007?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:140: TMR3CN0 = 0 ;                   // Stop Timer3 and clear overflow flag
	mov	_TMR3CN0,#0x00
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'waitms'
;------------------------------------------------------------
;ms                        Allocated to registers r2 r3 
;j                         Allocated to registers r4 r5 
;k                         Allocated to registers r6 
;------------------------------------------------------------
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:143: void waitms (unsigned int ms)
;	-----------------------------------------
;	 function waitms
;	-----------------------------------------
_waitms:
	mov	r2,dpl
	mov	r3,dph
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:147: for(j=0; j<ms; j++)
	mov	r4,#0x00
	mov	r5,#0x00
L005005?:
	clr	c
	mov	a,r4
	subb	a,r2
	mov	a,r5
	subb	a,r3
	jnc	L005009?
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:148: for (k=0; k<4; k++) Timer3us(250);
	mov	r6,#0x00
L005001?:
	cjne	r6,#0x04,L005018?
L005018?:
	jnc	L005007?
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
	sjmp	L005001?
L005007?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:147: for(j=0; j<ms; j++)
	inc	r4
	cjne	r4,#0x00,L005005?
	inc	r5
	sjmp	L005005?
L005009?:
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'UART1_Init'
;------------------------------------------------------------
;baudrate                  Allocated to registers r2 r3 r4 r5 
;------------------------------------------------------------
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:152: void UART1_Init (unsigned long baudrate)
;	-----------------------------------------
;	 function UART1_Init
;	-----------------------------------------
_UART1_Init:
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:154: SFRPAGE = 0x20;
	mov	_SFRPAGE,#0x20
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:155: SMOD1 = 0x0C; // no parity, 8 data bits, 1 stop bit
	mov	_SMOD1,#0x0C
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:156: SCON1 = 0x10;
	mov	_SCON1,#0x10
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:157: SBCON1 =0x00;   // disable baud rate generator
	mov	_SBCON1,#0x00
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:158: SBRL1 = 0x10000L-((SYSCLK/baudrate)/(12L*2L));
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
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:159: TI1 = 1; // indicate ready for TX
	setb	_TI1
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:160: SBCON1 |= 0x40;   // enable baud rate generator
	orl	_SBCON1,#0x40
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:161: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'putchar1'
;------------------------------------------------------------
;c                         Allocated to registers r2 
;------------------------------------------------------------
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:164: void putchar1 (char c) 
;	-----------------------------------------
;	 function putchar1
;	-----------------------------------------
_putchar1:
	mov	r2,dpl
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:166: SFRPAGE = 0x20;
	mov	_SFRPAGE,#0x20
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:167: while (!TI1);
L007001?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:168: TI1=0;
	jbc	_TI1,L007008?
	sjmp	L007001?
L007008?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:169: SBUF1 = c;
	mov	_SBUF1,r2
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:170: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'sendstr1'
;------------------------------------------------------------
;s                         Allocated to registers r2 r3 r4 
;------------------------------------------------------------
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:173: void sendstr1 (char * s)
;	-----------------------------------------
;	 function sendstr1
;	-----------------------------------------
_sendstr1:
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:175: while(*s)
L008001?:
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	__gptrget
	mov	r5,a
	jz	L008004?
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:177: putchar1(*s);
	mov	dpl,r5
	push	ar2
	push	ar3
	push	ar4
	lcall	_putchar1
	pop	ar4
	pop	ar3
	pop	ar2
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:178: s++;	
	inc	r2
	cjne	r2,#0x00,L008001?
	inc	r3
	sjmp	L008001?
L008004?:
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'getchar1'
;------------------------------------------------------------
;c                         Allocated to registers 
;------------------------------------------------------------
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:182: char getchar1 (void)
;	-----------------------------------------
;	 function getchar1
;	-----------------------------------------
_getchar1:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:185: SFRPAGE = 0x20;
	mov	_SFRPAGE,#0x20
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:186: while (!RI1);
L009001?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:187: RI1=0;
	jbc	_RI1,L009008?
	sjmp	L009001?
L009008?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:189: SCON1&=0b_0011_1111;
	anl	_SCON1,#0x3F
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:190: c = SBUF1;
	mov	dpl,_SBUF1
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:191: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:192: return (c);
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'getchar1_with_timeout'
;------------------------------------------------------------
;c                         Allocated to registers 
;timeout                   Allocated to registers r2 r3 
;------------------------------------------------------------
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:195: char getchar1_with_timeout (void)
;	-----------------------------------------
;	 function getchar1_with_timeout
;	-----------------------------------------
_getchar1_with_timeout:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:199: SFRPAGE = 0x20;
	mov	_SFRPAGE,#0x20
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:201: while (!RI1)
	mov	r2,#0x00
	mov	r3,#0x00
L010003?:
	jb	_RI1,L010005?
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:203: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:204: Timer3us(20);
	mov	dpl,#0x14
	push	ar2
	push	ar3
	lcall	_Timer3us
	pop	ar3
	pop	ar2
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:205: SFRPAGE = 0x20;
	mov	_SFRPAGE,#0x20
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:206: timeout++;
	inc	r2
	cjne	r2,#0x00,L010012?
	inc	r3
L010012?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:207: if(timeout==25000)
	cjne	r2,#0xA8,L010003?
	cjne	r3,#0x61,L010003?
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:209: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:210: return ('\n'); // Timeout after half second
	mov	dpl,#0x0A
	ret
L010005?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:213: RI1=0;
	clr	_RI1
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:215: SCON1&=0b_0011_1111;
	anl	_SCON1,#0x3F
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:216: c = SBUF1;
	mov	dpl,_SBUF1
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:217: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:218: return (c);
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'getstr1'
;------------------------------------------------------------
;n                         Allocated with name '_getstr1_PARM_2'
;s                         Allocated to registers r2 r3 r4 
;c                         Allocated to registers r1 
;cnt                       Allocated to registers r5 
;------------------------------------------------------------
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:221: void getstr1 (char * s, unsigned char n)
;	-----------------------------------------
;	 function getstr1
;	-----------------------------------------
_getstr1:
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:227: while(1)
	mov	r5,#0x00
	mov	ar6,r2
	mov	ar7,r3
	mov	ar0,r4
L011007?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:229: c=getchar1_with_timeout();
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	push	ar6
	push	ar7
	push	ar0
	lcall	_getchar1_with_timeout
	mov	r1,dpl
	pop	ar0
	pop	ar7
	pop	ar6
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:230: if(c=='\n')
	cjne	r1,#0x0A,L011002?
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:232: *s=0;
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	clr	a
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:233: return;
	ljmp	__gptrput
L011002?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:236: if (cnt<n)
	clr	c
	mov	a,r5
	subb	a,_getstr1_PARM_2
	jnc	L011004?
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:238: cnt++;
	inc	r5
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:239: *s=c;
	mov	dpl,r6
	mov	dph,r7
	mov	b,r0
	mov	a,r1
	lcall	__gptrput
	inc	dptr
	mov	r6,dpl
	mov	r7,dph
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:240: s++;
	mov	ar2,r6
	mov	ar3,r7
	mov	ar4,r0
	sjmp	L011007?
L011004?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:244: *s=0;
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	clr	a
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:245: return;
	ljmp	__gptrput
;------------------------------------------------------------
;Allocation info for local variables in function 'RXU1'
;------------------------------------------------------------
;------------------------------------------------------------
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:251: bit RXU1 (void)
;	-----------------------------------------
;	 function RXU1
;	-----------------------------------------
_RXU1:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:254: SFRPAGE = 0x20;
	mov	_SFRPAGE,#0x20
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:255: mybit=RI1;
	mov	c,_RI1
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:256: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:257: return mybit;
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'waitms_or_RI1'
;------------------------------------------------------------
;ms                        Allocated to registers r2 r3 
;j                         Allocated to registers r4 r5 
;k                         Allocated to registers r6 
;------------------------------------------------------------
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:260: void waitms_or_RI1 (unsigned int ms)
;	-----------------------------------------
;	 function waitms_or_RI1
;	-----------------------------------------
_waitms_or_RI1:
	mov	r2,dpl
	mov	r3,dph
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:264: for(j=0; j<ms; j++)
	mov	r4,#0x00
	mov	r5,#0x00
L013007?:
	clr	c
	mov	a,r4
	subb	a,r2
	mov	a,r5
	subb	a,r3
	jnc	L013011?
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:266: for (k=0; k<4; k++)
	mov	r6,#0x00
L013003?:
	cjne	r6,#0x04,L013019?
L013019?:
	jnc	L013009?
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:268: if(RXU1()) return;
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
	jz	L013002?
	ret
L013002?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:269: Timer3us(250);
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
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:266: for (k=0; k<4; k++)
	inc	r6
	sjmp	L013003?
L013009?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:264: for(j=0; j<ms; j++)
	inc	r4
	cjne	r4,#0x00,L013007?
	inc	r5
	sjmp	L013007?
L013011?:
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'SendATCommand'
;------------------------------------------------------------
;s                         Allocated to registers r2 r3 r4 
;------------------------------------------------------------
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:274: void SendATCommand (char * s)
;	-----------------------------------------
;	 function SendATCommand
;	-----------------------------------------
_SendATCommand:
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:276: printf("Command: %s", s);
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
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:277: P2_0=0; // 'set' pin to 0 is 'AT' mode.
	clr	_P2_0
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:278: waitms(5);
	mov	dptr,#0x0005
	lcall	_waitms
	pop	ar4
	pop	ar3
	pop	ar2
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:279: sendstr1(s);
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	lcall	_sendstr1
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:280: getstr1(buff, sizeof(buff)-1);
	mov	_getstr1_PARM_2,#0x13
	mov	dptr,#_buff
	mov	b,#0x40
	lcall	_getstr1
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:281: waitms(10);
	mov	dptr,#0x000A
	lcall	_waitms
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:282: P2_0=1; // 'set' pin to 1 is normal operation mode.
	setb	_P2_0
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:283: printf("Response: %s\r\n", buff);
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
;Allocation info for local variables in function 'ReceptionOff'
;------------------------------------------------------------
;------------------------------------------------------------
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:286: void ReceptionOff (void)
;	-----------------------------------------
;	 function ReceptionOff
;	-----------------------------------------
_ReceptionOff:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:288: P2_0=0; // 'set' pin to 0 is 'AT' mode.
	clr	_P2_0
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:289: waitms(10);
	mov	dptr,#0x000A
	lcall	_waitms
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:290: sendstr1("AT+DVID0000\r\n"); // Some unused id, so that we get nothing in RXD1.
	mov	dptr,#__str_2
	mov	b,#0x80
	lcall	_sendstr1
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:291: waitms(10);
	mov	dptr,#0x000A
	lcall	_waitms
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:293: SCON1&=0b_0011_1111;
	anl	_SCON1,#0x3F
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:294: P2_0=1; // 'set' pin to 1 is normal operation mode.
	setb	_P2_0
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'InitPinADC'
;------------------------------------------------------------
;pin_num                   Allocated with name '_InitPinADC_PARM_2'
;portno                    Allocated to registers r2 
;mask                      Allocated to registers r3 
;------------------------------------------------------------
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:299: void InitPinADC (unsigned char portno, unsigned char pin_num)
;	-----------------------------------------
;	 function InitPinADC
;	-----------------------------------------
_InitPinADC:
	mov	r2,dpl
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:303: mask=1<<pin_num;
	mov	b,_InitPinADC_PARM_2
	inc	b
	mov	a,#0x01
	sjmp	L016013?
L016011?:
	add	a,acc
L016013?:
	djnz	b,L016011?
	mov	r3,a
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:305: SFRPAGE = 0x20;
	mov	_SFRPAGE,#0x20
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:306: switch (portno)
	cjne	r2,#0x00,L016014?
	sjmp	L016001?
L016014?:
	cjne	r2,#0x01,L016015?
	sjmp	L016002?
L016015?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:308: case 0:
	cjne	r2,#0x02,L016005?
	sjmp	L016003?
L016001?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:309: P0MDIN &= (~mask); // Set pin as analog input
	mov	a,r3
	cpl	a
	mov	r2,a
	anl	_P0MDIN,a
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:310: P0SKIP |= mask; // Skip Crossbar decoding for this pin
	mov	a,r3
	orl	_P0SKIP,a
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:311: break;
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:312: case 1:
	sjmp	L016005?
L016002?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:313: P1MDIN &= (~mask); // Set pin as analog input
	mov	a,r3
	cpl	a
	mov	r2,a
	anl	_P1MDIN,a
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:314: P1SKIP |= mask; // Skip Crossbar decoding for this pin
	mov	a,r3
	orl	_P1SKIP,a
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:315: break;
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:316: case 2:
	sjmp	L016005?
L016003?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:317: P2MDIN &= (~mask); // Set pin as analog input
	mov	a,r3
	cpl	a
	mov	r2,a
	anl	_P2MDIN,a
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:318: P2SKIP |= mask; // Skip Crossbar decoding for this pin
	mov	a,r3
	orl	_P2SKIP,a
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:322: }
L016005?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:323: SFRPAGE = 0x00;
	mov	_SFRPAGE,#0x00
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'ADC_at_Pin'
;------------------------------------------------------------
;pin                       Allocated to registers 
;------------------------------------------------------------
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:326: unsigned int ADC_at_Pin(unsigned char pin)
;	-----------------------------------------
;	 function ADC_at_Pin
;	-----------------------------------------
_ADC_at_Pin:
	mov	_ADC0MX,dpl
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:329: ADINT = 0;
	clr	_ADINT
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:330: ADBUSY = 1;     // Convert voltage at the pin
	setb	_ADBUSY
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:331: while (!ADINT); // Wait for conversion to complete
L017001?:
	jnb	_ADINT,L017001?
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:332: return (ADC0);
	mov	dpl,_ADC0
	mov	dph,(_ADC0 >> 8)
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'Volts_at_Pin'
;------------------------------------------------------------
;pin                       Allocated to registers r2 
;------------------------------------------------------------
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:335: float Volts_at_Pin(unsigned char pin)
;	-----------------------------------------
;	 function Volts_at_Pin
;	-----------------------------------------
_Volts_at_Pin:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:337: return ((ADC_at_Pin(pin)*VDD)/16383.0);
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
	mov	dptr,#0x3333
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
;Allocation info for local variables in function 'InitPushButton'
;------------------------------------------------------------
;------------------------------------------------------------
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:340: void InitPushButton(void)
;	-----------------------------------------
;	 function InitPushButton
;	-----------------------------------------
_InitPushButton:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:342: SFRPAGE = 0x20;  // Switch to Port Configuration Page
	mov	_SFRPAGE,#0x20
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:343: P3MDOUT &= ~(1 << 2); // Set P3.2 as open-drain (input mode)
	anl	_P3MDOUT,#0xFB
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:344: P3 |= (1 << 2);  // Enable internal pull-up resistor
	orl	_P3,#0x04
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:345: P3 |= (1 << 0); // P3.0
	orl	_P3,#0x01
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:346: P3 |= (1 << 1); // P3.1
	orl	_P3,#0x02
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:347: SFRPAGE = 0x00;  // Restore SFRPAGE
	mov	_SFRPAGE,#0x00
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'main'
;------------------------------------------------------------
;timeout_cnt               Allocated to registers r2 r3 
;cont1                     Allocated to registers 
;cont2                     Allocated to registers 
;v                         Allocated with name '_main_v_1_131'
;norm_x                    Allocated with name '_main_norm_x_1_131'
;norm_y                    Allocated with name '_main_norm_y_1_131'
;mode                      Allocated to registers r6 r7 
;------------------------------------------------------------
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:351: void main (void)
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:365: waitms(500);
	mov	dptr,#0x01F4
	lcall	_waitms
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:366: printf("\x1b[2J"); // Clear screen using ANSI escape sequence.
	mov	a,#__str_3
	push	acc
	mov	a,#(__str_3 >> 8)
	push	acc
	mov	a,#0x80
	push	acc
	lcall	_printf
	dec	sp
	dec	sp
	dec	sp
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:368: UART1_Init(9600);
	mov	dptr,#0x2580
	clr	a
	mov	b,a
	lcall	_UART1_Init
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:382: ReceptionOff();
	lcall	_ReceptionOff
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:385: SendATCommand("AT+VER\r\n");
	mov	dptr,#__str_4
	mov	b,#0x80
	lcall	_SendATCommand
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:386: SendATCommand("AT+BAUD\r\n");
	mov	dptr,#__str_5
	mov	b,#0x80
	lcall	_SendATCommand
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:387: SendATCommand("AT+RFID\r\n");
	mov	dptr,#__str_6
	mov	b,#0x80
	lcall	_SendATCommand
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:388: SendATCommand("AT+DVID\r\n");
	mov	dptr,#__str_7
	mov	b,#0x80
	lcall	_SendATCommand
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:389: SendATCommand("AT+RFC\r\n");
	mov	dptr,#__str_8
	mov	b,#0x80
	lcall	_SendATCommand
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:390: SendATCommand("AT+POWE\r\n");
	mov	dptr,#__str_9
	mov	b,#0x80
	lcall	_SendATCommand
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:391: SendATCommand("AT+CLSS\r\n");
	mov	dptr,#__str_10
	mov	b,#0x80
	lcall	_SendATCommand
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:395: SendATCommand("AT+DVIDFDFD\r\n");
	mov	dptr,#__str_11
	mov	b,#0x80
	lcall	_SendATCommand
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:396: SendATCommand("AT+RFC113\r\n");
	mov	dptr,#__str_12
	mov	b,#0x80
	lcall	_SendATCommand
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:398: InitPinADC(2, 2); // Configure P2.2 as analog input
	mov	_InitPinADC_PARM_2,#0x02
	mov	dpl,#0x02
	lcall	_InitPinADC
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:399: InitPinADC(2, 3); // Configure P2.3 as analog input
	mov	_InitPinADC_PARM_2,#0x03
	mov	dpl,#0x02
	lcall	_InitPinADC
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:400: InitPinADC(2, 4); // Configure P2.4 as analog input
	mov	_InitPinADC_PARM_2,#0x04
	mov	dpl,#0x02
	lcall	_InitPinADC
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:401: InitPinADC(2, 5); // Configure P2.5 as analog input
	mov	_InitPinADC_PARM_2,#0x05
	mov	dpl,#0x02
	lcall	_InitPinADC
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:402: InitADC();
	lcall	_InitADC
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:403: InitPushButton();
	lcall	_InitPushButton
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:405: while(1)
L020034?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:409: putchar1('!'); // Send a message to the slave. First send the 'attention' character which is '!'
	mov	dpl,#0x21
	lcall	_putchar1
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:411: waitms(5); // This may need adjustment depending on how busy is the slave
	mov	dptr,#0x0005
	lcall	_waitms
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:419: putchar1('@'); // Request a message from the slave
	mov	dpl,#0x40
	lcall	_putchar1
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:424: while(1)
	mov	r2,#0x00
	mov	r3,#0x00
L020006?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:426: if(RXU1()) break; // Something has arrived
	push	ar2
	push	ar3
	lcall	_RXU1
	pop	ar3
	pop	ar2
	jc	L020007?
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:427: if(++timeout_cnt>250) break; // Wait up to 25ms for the repply
	inc	r2
	cjne	r2,#0x00,L020055?
	inc	r3
L020055?:
	clr	c
	mov	a,#0xFA
	subb	a,r2
	clr	a
	xrl	a,#0x80
	mov	b,r3
	xrl	b,#0x80
	subb	a,b
	jc	L020007?
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:428: Timer3us(100); // 100us*250=25ms
	mov	dpl,#0x64
	push	ar2
	push	ar3
	lcall	_Timer3us
	pop	ar3
	pop	ar2
	sjmp	L020006?
L020007?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:432: if(RXU1()) // Something has arrived from the slave
	lcall	_RXU1
	jc	L020057?
	ljmp	L020031?
L020057?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:436: v[0] = Volts_at_Pin(QFP32_MUX_P2_2);
	mov	dpl,#0x0F
	lcall	_Volts_at_Pin
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	mov	_main_v_1_131,r2
	mov	(_main_v_1_131 + 1),r3
	mov	(_main_v_1_131 + 2),r4
	mov	(_main_v_1_131 + 3),r5
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:437: v[1] = Volts_at_Pin(QFP32_MUX_P2_3);
	mov	dpl,#0x10
	lcall	_Volts_at_Pin
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	mov	(_main_v_1_131 + 0x0004),r2
	mov	((_main_v_1_131 + 0x0004) + 1),r3
	mov	((_main_v_1_131 + 0x0004) + 2),r4
	mov	((_main_v_1_131 + 0x0004) + 3),r5
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:438: v[2] = Volts_at_Pin(QFP32_MUX_P2_4);
	mov	dpl,#0x11
	lcall	_Volts_at_Pin
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	mov	(_main_v_1_131 + 0x0008),r2
	mov	((_main_v_1_131 + 0x0008) + 1),r3
	mov	((_main_v_1_131 + 0x0008) + 2),r4
	mov	((_main_v_1_131 + 0x0008) + 3),r5
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:439: v[3] = Volts_at_Pin(QFP32_MUX_P2_5);
	mov	dpl,#0x12
	lcall	_Volts_at_Pin
	mov	r2,dpl
	mov	r3,dph
	mov	r4,b
	mov	r5,a
	mov	(_main_v_1_131 + 0x000c),r2
	mov	((_main_v_1_131 + 0x000c) + 1),r3
	mov	((_main_v_1_131 + 0x000c) + 2),r4
	mov	((_main_v_1_131 + 0x000c) + 3),r5
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:441: norm_x = (v[1] / 3.294) * 2.0 - 1.0;  // Horizontal (P2.3)
	mov	a,#0xE5
	push	acc
	mov	a,#0xD0
	push	acc
	mov	a,#0x52
	push	acc
	mov	a,#0x40
	push	acc
	mov	dpl,(_main_v_1_131 + 0x0004)
	mov	dph,((_main_v_1_131 + 0x0004) + 1)
	mov	b,((_main_v_1_131 + 0x0004) + 2)
	mov	a,((_main_v_1_131 + 0x0004) + 3)
	lcall	___fsdiv
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
	mov	dptr,#(0x00&0x00ff)
	clr	a
	mov	b,a
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
	push	acc
	mov	a,#0x80
	push	acc
	mov	a,#0x3F
	push	acc
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	lcall	___fssub
	mov	_main_norm_x_1_131,dpl
	mov	(_main_norm_x_1_131 + 1),dph
	mov	(_main_norm_x_1_131 + 2),b
	mov	(_main_norm_x_1_131 + 3),a
	mov	a,sp
	add	a,#0xfc
	mov	sp,a
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:442: norm_y = (v[0] / 3.294) * 2.0 - 1.0;  // Vertical   (P2.2)
	mov	a,#0xE5
	push	acc
	mov	a,#0xD0
	push	acc
	mov	a,#0x52
	push	acc
	mov	a,#0x40
	push	acc
	mov	dpl,_main_v_1_131
	mov	dph,(_main_v_1_131 + 1)
	mov	b,(_main_v_1_131 + 2)
	mov	a,(_main_v_1_131 + 3)
	lcall	___fsdiv
	mov	r6,dpl
	mov	r7,dph
	mov	r2,b
	mov	r3,a
	mov	a,sp
	add	a,#0xfc
	mov	sp,a
	push	ar6
	push	ar7
	push	ar2
	push	ar3
	mov	dptr,#(0x00&0x00ff)
	clr	a
	mov	b,a
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
	push	acc
	mov	a,#0x80
	push	acc
	mov	a,#0x3F
	push	acc
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,r5
	lcall	___fssub
	mov	_main_norm_y_1_131,dpl
	mov	(_main_norm_y_1_131 + 1),dph
	mov	(_main_norm_y_1_131 + 2),b
	mov	(_main_norm_y_1_131 + 3),a
	mov	a,sp
	add	a,#0xfc
	mov	sp,a
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:444: button_state = (P3 & (1 << 2)) ? 0 : 1;
	mov	a,_P3
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:445: button_1_state = (P3 & (1 << 0)) ? 0 : 1; // If HIGH, button not pressed; If LOW, button pressed
	mov	a,_P3
	anl	a,#0x01
	mov	r6,a
	cjne	a,#0x01,L020058?
L020058?:
	mov	_main_button_1_state_1_131,c
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:446: button_2_state = (P3 & (1 << 1)) ? 0 : 1; 
	mov	a,_P3
	rr	a
	anl	a,#0x01
	mov	r6,a
	cjne	a,#0x01,L020059?
L020059?:
	mov	_main_button_2_state_1_131,c
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:448: if (button_1_state == 1) {
	jnb	_main_button_1_state_1_131,L020028?
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:449: printf("button 1 pressed. switch to automatic mode");
	mov	a,#__str_13
	push	acc
	mov	a,#(__str_13 >> 8)
	push	acc
	mov	a,#0x80
	push	acc
	lcall	_printf
	dec	sp
	dec	sp
	dec	sp
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:450: mode = 5;
	mov	r6,#0x05
	mov	r7,#0x00
	ljmp	L020029?
L020028?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:453: else if (button_2_state == 1) {
	jnb	_main_button_2_state_1_131,L020025?
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:454: printf("button 2 pressed. switch to manual mode");
	mov	a,#__str_14
	push	acc
	mov	a,#(__str_14 >> 8)
	push	acc
	mov	a,#0x80
	push	acc
	lcall	_printf
	dec	sp
	dec	sp
	dec	sp
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:455: mode = 6;
	mov	r6,#0x06
	mov	r7,#0x00
	ljmp	L020029?
L020025?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:468: else if (norm_x <= 1.5 && norm_x > 0.5) //right
	clr	a
	push	acc
	push	acc
	mov	a,#0xC0
	push	acc
	mov	a,#0x3F
	push	acc
	mov	dpl,_main_norm_x_1_131
	mov	dph,(_main_norm_x_1_131 + 1)
	mov	b,(_main_norm_x_1_131 + 2)
	mov	a,(_main_norm_x_1_131 + 3)
	lcall	___fsgt
	mov	r2,dpl
	mov	a,sp
	add	a,#0xfc
	mov	sp,a
	mov	a,r2
	jnz	L020021?
	push	acc
	push	acc
	push	acc
	mov	a,#0x3F
	push	acc
	mov	dpl,_main_norm_x_1_131
	mov	dph,(_main_norm_x_1_131 + 1)
	mov	b,(_main_norm_x_1_131 + 2)
	mov	a,(_main_norm_x_1_131 + 3)
	lcall	___fsgt
	mov	r2,dpl
	mov	a,sp
	add	a,#0xfc
	mov	sp,a
	mov	a,r2
	jz	L020021?
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:470: mode = 3;
	mov	r6,#0x03
	mov	r7,#0x00
	ljmp	L020029?
L020021?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:473: else if (norm_x <-0.5 && norm_x>= -1.5) //left
	clr	a
	push	acc
	push	acc
	push	acc
	mov	a,#0xBF
	push	acc
	mov	dpl,_main_norm_x_1_131
	mov	dph,(_main_norm_x_1_131 + 1)
	mov	b,(_main_norm_x_1_131 + 2)
	mov	a,(_main_norm_x_1_131 + 3)
	lcall	___fslt
	mov	r2,dpl
	mov	a,sp
	add	a,#0xfc
	mov	sp,a
	mov	a,r2
	jz	L020017?
	clr	a
	push	acc
	push	acc
	mov	a,#0xC0
	push	acc
	mov	a,#0xBF
	push	acc
	mov	dpl,_main_norm_x_1_131
	mov	dph,(_main_norm_x_1_131 + 1)
	mov	b,(_main_norm_x_1_131 + 2)
	mov	a,(_main_norm_x_1_131 + 3)
	lcall	___fslt
	mov	r2,dpl
	mov	a,sp
	add	a,#0xfc
	mov	sp,a
	mov	a,r2
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:475: mode = 4;
	jnz	L020017?
	mov	r6,#0x04
	mov	r7,a
	ljmp	L020029?
L020017?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:478: else if (norm_y <= 1.5 && norm_y > 0.5) // forward
	clr	a
	push	acc
	push	acc
	mov	a,#0xC0
	push	acc
	mov	a,#0x3F
	push	acc
	mov	dpl,_main_norm_y_1_131
	mov	dph,(_main_norm_y_1_131 + 1)
	mov	b,(_main_norm_y_1_131 + 2)
	mov	a,(_main_norm_y_1_131 + 3)
	lcall	___fsgt
	mov	r2,dpl
	mov	a,sp
	add	a,#0xfc
	mov	sp,a
	mov	a,r2
	jnz	L020013?
	push	acc
	push	acc
	push	acc
	mov	a,#0x3F
	push	acc
	mov	dpl,_main_norm_y_1_131
	mov	dph,(_main_norm_y_1_131 + 1)
	mov	b,(_main_norm_y_1_131 + 2)
	mov	a,(_main_norm_y_1_131 + 3)
	lcall	___fsgt
	mov	r2,dpl
	mov	a,sp
	add	a,#0xfc
	mov	sp,a
	mov	a,r2
	jz	L020013?
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:480: mode = 1;
	mov	r6,#0x01
	mov	r7,#0x00
	sjmp	L020029?
L020013?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:483: else if (norm_y <-0.5 && norm_y>= -1.5) //backwards
	clr	a
	push	acc
	push	acc
	push	acc
	mov	a,#0xBF
	push	acc
	mov	dpl,_main_norm_y_1_131
	mov	dph,(_main_norm_y_1_131 + 1)
	mov	b,(_main_norm_y_1_131 + 2)
	mov	a,(_main_norm_y_1_131 + 3)
	lcall	___fslt
	mov	r2,dpl
	mov	a,sp
	add	a,#0xfc
	mov	sp,a
	mov	a,r2
	jz	L020009?
	clr	a
	push	acc
	push	acc
	mov	a,#0xC0
	push	acc
	mov	a,#0xBF
	push	acc
	mov	dpl,_main_norm_y_1_131
	mov	dph,(_main_norm_y_1_131 + 1)
	mov	b,(_main_norm_y_1_131 + 2)
	mov	a,(_main_norm_y_1_131 + 3)
	lcall	___fslt
	mov	r2,dpl
	mov	a,sp
	add	a,#0xfc
	mov	sp,a
	mov	a,r2
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:485: mode = 2;
	jnz	L020009?
	mov	r6,#0x02
	mov	r7,a
	sjmp	L020029?
L020009?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:489: mode = 0;
	mov	r6,#0x00
	mov	r7,#0x00
L020029?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:492: sprintf(buff, "test %d\n", mode);
	push	ar6
	push	ar7
	mov	a,#__str_15
	push	acc
	mov	a,#(__str_15 >> 8)
	push	acc
	mov	a,#0x80
	push	acc
	mov	a,#_buff
	push	acc
	mov	a,#(_buff >> 8)
	push	acc
	mov	a,#0x40
	push	acc
	lcall	_sprintf
	mov	a,sp
	add	a,#0xf8
	mov	sp,a
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:495: sendstr1(buff);
	mov	dptr,#_buff
	mov	b,#0x40
	lcall	_sendstr1
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:498: waitms(100);
	mov	dptr,#0x0064
	lcall	_waitms
	sjmp	L020032?
L020031?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:516: printf("NO RESPONSE\r\n", buff);
	mov	a,#_buff
	push	acc
	mov	a,#(_buff >> 8)
	push	acc
	mov	a,#0x40
	push	acc
	mov	a,#__str_16
	push	acc
	mov	a,#(__str_16 >> 8)
	push	acc
	mov	a,#0x80
	push	acc
	lcall	_printf
	mov	a,sp
	add	a,#0xfa
	mov	sp,a
L020032?:
;	C:\Users\leigh\Elec291Project2\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c:519: waitms(50);  // Set the information interchange pace: communicate about every 50ms
	mov	dptr,#0x0032
	lcall	_waitms
	ljmp	L020034?
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
	db 'AT+DVID0000'
	db 0x0D
	db 0x0A
	db 0x00
__str_3:
	db 0x1B
	db '[2J'
	db 0x00
__str_4:
	db 'AT+VER'
	db 0x0D
	db 0x0A
	db 0x00
__str_5:
	db 'AT+BAUD'
	db 0x0D
	db 0x0A
	db 0x00
__str_6:
	db 'AT+RFID'
	db 0x0D
	db 0x0A
	db 0x00
__str_7:
	db 'AT+DVID'
	db 0x0D
	db 0x0A
	db 0x00
__str_8:
	db 'AT+RFC'
	db 0x0D
	db 0x0A
	db 0x00
__str_9:
	db 'AT+POWE'
	db 0x0D
	db 0x0A
	db 0x00
__str_10:
	db 'AT+CLSS'
	db 0x0D
	db 0x0A
	db 0x00
__str_11:
	db 'AT+DVIDFDFD'
	db 0x0D
	db 0x0A
	db 0x00
__str_12:
	db 'AT+RFC113'
	db 0x0D
	db 0x0A
	db 0x00
__str_13:
	db 'button 1 pressed. switch to automatic mode'
	db 0x00
__str_14:
	db 'button 2 pressed. switch to manual mode'
	db 0x00
__str_15:
	db 'test %d'
	db 0x0A
	db 0x00
__str_16:
	db 'NO RESPONSE'
	db 0x0D
	db 0x0A
	db 0x00

	CSEG

end

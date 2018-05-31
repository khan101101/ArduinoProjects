// *** Hardwarespecific functions ***
void UTFT::_hw_special_init()
{
    pinMode(PD4, OUTPUT); digitalWrite(PD4, HIGH); // Set the TFT_RD pin permanently HIGH as it is not supported by UTFT
}

void UTFT::LCD_Writ_Bus(char VH,char VL, byte mode)
{   
	switch (mode)
	{
	case 1:
		asm("nop");		// Mode is unsupported
		break;
	case 8:
		asm("nop");		// Mode is unsupported
		break;
	case 16:


/*		REG_PIOD_CODR=0x0000c703;
		REG_PIOE_CODR=0x0000ff80;

		REG_PIOD_SODR=( ((VL & 0x03) <<14) | ((VL & 0x0c) >> 2 ) | ((VH & 0xe0) << 3) );
		REG_PIOE_SODR=( ((VL & 0xf0) << 3) | ((VH & 0x1f) << 11) );
*/

		GPIOD->regs->BSRR = (0x0000c703 & ( ((VL & 0x03) <<14) | ((VL & 0x0c) >> 2 ) | ((VH & 0xe0) << 3) ) ) |
							(0xc7030000 & (~ ( ((VL & 0x03) <<14) | ((VL & 0x0c) >> 2 ) | ((VH & 0xe0) << 3) ) ) << 16 );
		GPIOE->regs->BSRR = (0x0000ff80 & ( ((VL & 0xf0) << 3) | ((VH & 0x1f) << 11) ) ) |
							(0xff800000 & ( ~( ((VL & 0xf0) << 3) | ((VH & 0x1f) << 11) ) ) << 16 );
		
		
		
		pulse_low(P_WR, B_WR); 
		break;
	case LATCHED_16:
		asm("nop");		// Mode is unsupported
		break;
	}
}

void UTFT::_set_direction_registers(byte mode)
{
	if (mode!=LATCHED_16)
	{
/*
		REG_PIOD_OER=0x0000c703;
		REG_PIOE_OER=0x0000ff80;
*/
		pinMode(PD14, OUTPUT);
		pinMode(PD15, OUTPUT);
		pinMode(PD0, OUTPUT);
		pinMode(PD1, OUTPUT);
		pinMode(PE7, OUTPUT);
		pinMode(PE8, OUTPUT);
		pinMode(PE9, OUTPUT);
		pinMode(PE10, OUTPUT);
		pinMode(PE11, OUTPUT);
		pinMode(PE12, OUTPUT);
		pinMode(PE13, OUTPUT);
		pinMode(PE14, OUTPUT);
		pinMode(PE15, OUTPUT);
		pinMode(PD8, OUTPUT);
		pinMode(PD9, OUTPUT);
		pinMode(PD10, OUTPUT);
	}
	else
	{
		asm("nop");		// Mode is unsupported
	}
}

void UTFT::_fast_fill_16(int ch, int cl, long pix)
{
	long blocks;

/*
	REG_PIOD_CODR=0x0000c703;
	REG_PIOE_CODR=0x0000ff80;

	REG_PIOD_SODR=( ((cl & 0x03) <<14) | ((cl & 0x0c) >> 2 ) | ((ch & 0xe0) << 3) );
	REG_PIOE_SODR=( ((cl & 0xf0) << 3) | ((ch & 0x1f) << 11) );
*/
		GPIOD->regs->BSRR = (0x0000c703 & ( ((cl & 0x03) <<14) | ((cl & 0x0c) >> 2 ) | ((ch & 0xe0) << 3) ) ) |
							(0xc7030000 & (~ ( ((cl & 0x03) <<14) | ((cl & 0x0c) >> 2 ) | ((ch & 0xe0) << 3) ) ) << 16 );
		GPIOE->regs->BSRR = (0x0000ff80 & ( ((cl & 0xf0) << 3) | ((ch & 0x1f) << 11) ) )|
							(0xff800000 & ( ~( ((cl & 0xf0) << 3) | ((ch & 0x1f) << 11) ) ) << 16 );
	blocks = pix/16;
	for (int i=0; i<blocks; i++)
	{
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
	}
	if ((pix % 16) != 0)
		for (int i=0; i<(pix % 16)+1; i++)
		{
			pulse_low(P_WR, B_WR);
		}
}

void UTFT::_fast_fill_8(int ch, long pix)
{
	long blocks;

/*
	REG_PIOD_CODR=0x0000c703;
	REG_PIOE_CODR=0x0000ff80;

	REG_PIOD_SODR=( ((ch & 0xe0) << 3) );
	REG_PIOE_SODR=( ((ch & 0x1f) << 11) );
*/
		GPIOD->regs->BSRR = (0x0000c703 & ( ((ch & 0x03) <<14)  ) ) |
							(0xc7030000 & (~ ( ((ch & 0x03) <<14)  ) ) << 16 );
		GPIOE->regs->BSRR = (0x0000ff80 & ( ((ch & 0x1f) << 11) ) )|
							(0xff800000 & ( ~( ((ch & 0x1f) << 11) ) ) << 16 );


	blocks = pix/16;
	for (int i=0; i<blocks; i++)
	{
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
	}
	if ((pix % 16) != 0)
		for (int i=0; i<(pix % 16)+1; i++)
		{
			pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		}
}

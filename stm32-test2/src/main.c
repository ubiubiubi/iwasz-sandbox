#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_spi.h>
#include <ff.h>

//#define DISPLAY_R_TYPE 1
#define DISPLAY_B_TYPE 1

/**
 * A simple time consuming function.
 * For a more real-world one,
 * we would use timers and interrupts.
 */
static void delay (__IO uint32_t ms)
{
        // @ 168MHz
        uint32_t nCount = 3360 * ms;

        while (nCount--);
                __asm("nop");
}

/**
 * Init all the peripherals I need.
 */
void initPperipheral ()
{
        RCC_APB2PeriphClockCmd (RCC_APB2Periph_SPI1, ENABLE);
        RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE);
        RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOE, ENABLE);

        // For SPI (NSS SOFT) for TFT.
        GPIO_InitTypeDef gpioInitStruct;
        gpioInitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
        gpioInitStruct.GPIO_Mode = GPIO_Mode_AF;
        gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        gpioInitStruct.GPIO_OType = GPIO_OType_PP;
        gpioInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init (GPIOA, &gpioInitStruct);

        GPIO_PinAFConfig (GPIOA, GPIO_PinSource5, GPIO_AF_SPI1); // SCK - to w datasheet można znaleźć.
        GPIO_PinAFConfig (GPIOA, GPIO_PinSource6, GPIO_AF_SPI1); // MISO
        GPIO_PinAFConfig (GPIOA, GPIO_PinSource7, GPIO_AF_SPI1); // MOSI

        gpioInitStruct.GPIO_Pin = GPIO_Pin_7 | // TFT Slave Select
                                  GPIO_Pin_9 | // RFT A0 (0 = command, 1 = data)
                                  GPIO_Pin_11; // SD card Slave Select
        gpioInitStruct.GPIO_Mode = GPIO_Mode_OUT;
        gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        gpioInitStruct.GPIO_OType = GPIO_OType_PP;
        gpioInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init (GPIOE, &gpioInitStruct);

        GPIOE->BSRRL |= GPIO_Pin_7 | GPIO_Pin_9; // Both slaves are not selected

        SPI_InitTypeDef spiInitStruct;
        spiInitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
        spiInitStruct.SPI_Mode = SPI_Mode_Master;
        spiInitStruct.SPI_DataSize = SPI_DataSize_8b;
        spiInitStruct.SPI_CPOL = SPI_CPOL_Low;
        spiInitStruct.SPI_CPHA = SPI_CPHA_1Edge;
        spiInitStruct.SPI_NSS = SPI_NSS_Soft;
        spiInitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
        spiInitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
        SPI_CalculateCRC (SPI1, DISABLE);
        SPI_Init (SPI1, &spiInitStruct);

        SPI_Cmd (SPI1, ENABLE);
}

/**
 * Send a byte over the SPI.
 */
void spiSend (SPI_TypeDef *SPIx, GPIO_TypeDef *softSS, uint16_t softSSPinMask, uint8_t data)
{
        GPIOE->BSRRH |= softSSPinMask;
        SPIx->DR = data; // write data to be transmitted to the SPI data register
        while (!(SPIx->SR & SPI_I2S_FLAG_TXE)) ; // wait until transmit complete
        while (!(SPIx->SR & SPI_I2S_FLAG_RXNE)) ; // wait until receive complete
        while (SPIx->SR & SPI_I2S_FLAG_BSY) ; // wait until SPI is not busy anymore
//        uint8_t ret = SPIx->DR; // return received data from SPI data register
        GPIOE->BSRRL |= softSSPinMask;
//        return ret;
}

void spiSendNoSS (SPI_TypeDef *SPIx, uint8_t data)
{
        SPIx->DR = data;
        while (!(SPIx->SR & SPI_I2S_FLAG_TXE)) ;
        while (!(SPIx->SR & SPI_I2S_FLAG_RXNE)) ;
        while (SPIx->SR & SPI_I2S_FLAG_BSY) ;
}

/**
 * Send a command to the TFT.
 */
void sendCommand (uint8_t cmd)
{
        // A0 = 0
        GPIOE->BSRRH |= GPIO_Pin_9;
        spiSend (SPI1, GPIOE, GPIO_Pin_7, cmd);
}

/**
 * Send a byte of data to the TFT.
 */
void sendData (uint8_t data)
{
        // A0 = 1
        GPIOE->BSRRL |= GPIO_Pin_9;
        spiSend (SPI1, GPIOE, GPIO_Pin_7, data);
}

//#define INITR_GREENTAB 0x0
//#define INITR_REDTAB   0x1
//#define INITR_BLACKTAB   0x2

#define ST7735_TFTWIDTH  128
#define ST7735_TFTHEIGHT 160

#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

// Color definitions
#define ST7735_BLACK   0x0000
#define ST7735_BLUE    0x001F
#define ST7735_RED     0xF800
#define ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF

#define DELAY 0x80

static uint8_t

  Rcmd1[] = {                 // Init for 7735R, part 1 (red or green tab)
    15,                       // 15 commands in list:
    ST7735_SWRESET,   DELAY,  //  1: Software reset, 0 args, w/delay
      150,                    //     150 ms delay
    ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, 0 args, w/delay
      255,                    //     500 ms delay
    ST7735_FRMCTR1, 3      ,  //  3: Frame rate ctrl - normal mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2, 3      ,  //  4: Frame rate control - idle mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3, 6      ,  //  5: Frame rate ctrl - partial mode, 6 args:
      0x01, 0x2C, 0x2D,       //     Dot inversion mode
      0x01, 0x2C, 0x2D,       //     Line inversion mode
    ST7735_INVCTR , 1      ,  //  6: Display inversion ctrl, 1 arg, no delay:
      0x07,                   //     No inversion
    ST7735_PWCTR1 , 3      ,  //  7: Power control, 3 args, no delay:
      0xA2,
      0x02,                   //     -4.6V
      0x84,                   //     AUTO mode
    ST7735_PWCTR2 , 1      ,  //  8: Power control, 1 arg, no delay:
      0xC5,                   //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    ST7735_PWCTR3 , 2      ,  //  9: Power control, 2 args, no delay:
      0x0A,                   //     Opamp current small
      0x00,                   //     Boost frequency
    ST7735_PWCTR4 , 2      ,  // 10: Power control, 2 args, no delay:
      0x8A,                   //     BCLK/2, Opamp current small & Medium low
      0x2A,
    ST7735_PWCTR5 , 2      ,  // 11: Power control, 2 args, no delay:
      0x8A, 0xEE,
    ST7735_VMCTR1 , 1      ,  // 12: Power control, 1 arg, no delay:
      0x0E,
    ST7735_INVOFF , 0      ,  // 13: Don't invert display, no args, no delay
    ST7735_MADCTL , 1      ,  // 14: Memory access control (directions), 1 arg:
      0xC8,                   //     row addr/col addr, bottom to top refresh
    ST7735_COLMOD , 1      ,  // 15: set color mode, 1 arg, no delay:
      0x05 },                 //     16-bit color

  Rcmd2red[] = {              // Init for 7735R, part 2 (red tab only)
    2,                        //  2 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F,             //     XEND = 127
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x9F },           //     XEND = 159

  Rcmd3[] = {                 // Init for 7735R, part 3 (red or green tab)
    4,                        //  4 commands in list:
    ST7735_GMCTRP1, 16      , //  1: Magical unicorn dust, 16 args, no delay:
      0x02, 0x1c, 0x07, 0x12,
      0x37, 0x32, 0x29, 0x2d,
      0x29, 0x25, 0x2B, 0x39,
      0x00, 0x01, 0x03, 0x10,
    ST7735_GMCTRN1, 16      , //  2: Sparkles and rainbows, 16 args, no delay:
      0x03, 0x1d, 0x07, 0x06,
      0x2E, 0x2C, 0x29, 0x2D,
      0x2E, 0x2E, 0x37, 0x3F,
      0x00, 0x00, 0x02, 0x10,
    ST7735_NORON  ,    DELAY, //  3: Normal display on, no args, w/delay
      10,                     //     10 ms delay
    ST7735_DISPON ,    DELAY, //  4: Main screen turn on, no args w/delay
      100 };                  //     100 ms delay

/**
 * Capable of running a command list defined above.
 */
void commandList (uint8_t *addr)
{
        uint8_t numCommands, numArgs;
        uint16_t ms;

        numCommands = *addr++; // Number of commands to follow

        while (numCommands--) {                 // For each command...
                sendCommand (*addr++);          // Read, issue command
                numArgs = *addr++;              // Number of args to follow
                ms = numArgs & DELAY;           // If hibit set, delay follows args
                numArgs &= ~DELAY;              // Mask out delay bit

                while (numArgs--) {             //   For each argument...
                        sendData (*addr++);     // Read, issue argument
                }

                if (ms) {
                        ms = *addr++;           // Read post-command delay time (ms)

                        if (ms == 255) {
                                ms = 500;       // If 255, delay for 500 ms
                        }

                        delay (ms);
                }
        }
}

/**
 * Issues series of commands to the TFT which hopefully will init the thing.
 */
void initTft ()
{
        // Reset
//        GPIOE->BSRRH |= GPIO_Pin_7;  // CS to low = slave select enabled.
//        GPIOE->BSRRL |= GPIO_Pin_11; // reset pin high for ~500ms
//        delay (500);
//        GPIOE->BSRRH |= GPIO_Pin_11;
//        delay (500);
//        GPIOE->BSRRL |= GPIO_Pin_11;
//        delay (500);
        GPIOE->BSRRH |= GPIO_Pin_11;
//        delay (500);

//        commandList (Bcmd);

        // BLACK TAB!
        commandList (Rcmd1);
        commandList(Rcmd2red);
        commandList(Rcmd3);

        sendCommand (ST7735_MADCTL);
        sendData (0xC0);

}


void setAddrWindow (uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{

        sendCommand (ST7735_CASET); // Column addr set
        sendData (0x00);
        sendData (x0); // XSTART
        sendData (0x00);
        sendData (x1); // XEND

        sendCommand (ST7735_RASET); // Row addr set
        sendData (0x00);
        sendData (y0); // YSTART
        sendData (0x00);
        sendData (y1); // YEND

        sendCommand (ST7735_RAMWR); // write to RAM
}


// fill a rectangle
void fillRect (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{

        // rudimentary clipping (drawChar w/big text requires this)
        if ((x >= ST7735_TFTWIDTH) || (y >= ST7735_TFTHEIGHT)) {
                return;
        }

        if ((x + w - 1) >= ST7735_TFTWIDTH) {
                w = ST7735_TFTWIDTH - x;
        }

        if ((y + h - 1) >= ST7735_TFTHEIGHT) {
                h = ST7735_TFTHEIGHT - y;
        }

        setAddrWindow (x, y, x + w - 1, y + h - 1);

        uint8_t hi = color >> 8, lo = color;
//TODO  tak zrobić
//        *rsport |= rspinmask;
//        *csport &= ~cspinmask;

        for (y = h; y > 0; y--) {
                for (x = w; x > 0; x--) {
                        // TODO za każdym razem ustawia CS i A0 zrobić to tylko raz na początku
                        sendData (hi);
                        sendData (lo);
                }
        }

//        *csport |= cspinmask;
}

void fillScreen (uint16_t color)
{
        setAddrWindow (0, 0, ST7735_TFTWIDTH - 1, ST7735_TFTHEIGHT - 1);

        GPIOE->BSRRH |= GPIO_Pin_7;
        GPIOE->BSRRL |= GPIO_Pin_9;

        for (uint8_t x = 0; x < ST7735_TFTWIDTH; ++x) {
                for (uint8_t y = 0; y < ST7735_TFTHEIGHT; ++y) {
                        spiSendNoSS (SPI1, color >> 8);
                        spiSendNoSS (SPI1, color);

                }
        }

        GPIOE->BSRRL |= GPIO_Pin_7;

}

void fillScreenSlow (uint16_t color)
{
        setAddrWindow (0, 0, ST7735_TFTWIDTH - 1, ST7735_TFTHEIGHT - 1);

        for (uint8_t x = 0; x < ST7735_TFTWIDTH; ++x) {
                for (uint8_t y = 0; y < ST7735_TFTHEIGHT; ++y) {
                        sendData (color >> 8);
                        sendData (color);

                }
        }
}

/****************************************************************************/

void testSdCard (void)
{
        FATFS Fatfs;            /* File system object */
        FIL Fil;                        /* File object */
//        BYTE Buff[128];         /* File read buffer */

        FRESULT rc; /* Result code */
//        DIR dir; /* Directory object */
//        FILINFO fno; /* File information object */
        UINT bw/*, br, i*/;

        f_mount (0, &Fatfs); /* Register volume work area (never fails) */

//        rc = f_open (&Fil, "MESSAGE.TXT", FA_READ);
//
//        if (rc) {
//                // TODO jakoś zgłaszać błędy
//                return;
//        }

//        for (;;) {
//                rc = f_read (&Fil, Buff, sizeof Buff, &br); /* Read a chunk of file */
//                if (rc || !br)
//                        break; /* Error or end of file */
//                for (i = 0; i < br; i++) /* Type the data */
//                        putchar (Buff[i]);
//        }
//        if (rc)
//                die (rc);
//
//        printf ("\nClose the file.\n");
//        rc = f_close (&Fil);
//        if (rc)
//                die (rc);

        rc = f_open (&Fil, "HELLO.TXT", FA_WRITE | FA_CREATE_ALWAYS);

        if (rc) {
                return;
        }

        rc = f_write (&Fil, "Hello world!\r\n", 14, &bw);

        if (rc) {
                return;
        }

        rc = f_close (&Fil);

        if (rc) {
                return;
        }
}

void initTimer6 (void)
{
        TIM_TimeBaseInitTypeDef timTimeBaseStructure;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

        timTimeBaseStructure.TIM_Prescaler = 0;
        timTimeBaseStructure.TIM_Period = 0xFF; // ARR
        // timTimeBaseStructure.TIM_ClockDivision; Nie jest brane pod uwagę
        // timTimeBaseStructure.TIM_CounterMode; Nie jest brane pod uwagę
        // timTimeBaseStructure.TIM_RepetitionCounter; Nie jest brane pod uwagę
        TIM_TimeBaseInit (TIM6, &timTimeBaseStructure);

        TIM_SelectOutputTrigger (TIM6, TIM_TRGOSource_Update);
        TIM_Cmd (TIM6, ENABLE);
}


/**
 *
 */
int main (void)
{
        initPperipheral ();
        delay (10);
        initTft ();

        fillScreen (ST7735_BLACK);

        testSdCard ();

        fillScreen (ST7735_RED);

//        uint16_t color = 0;
        while (1) {
//                fillScreen (++color);
        }
}


#ifdef USE_FULL_ASSERT
void assert_failed ( uint8_t * file , uint32_t line)
{
        /* Infinite loop */
        /* Use GDB to find out why we're here */
        while (1);
}
#endif

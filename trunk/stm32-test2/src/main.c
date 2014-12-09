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


/*--------------------------------------------------------------------------------------------------------------*/

#define SLAVE_ADDRESS 0x3D // the slave address (example)
void init_I2C1 (void)
{
        GPIO_InitTypeDef GPIO_InitStruct;
        I2C_InitTypeDef I2C_InitStruct;

        // enable APB1 peripheral clock for I2C1
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
        // enable clock for SCL and SDA pins
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

        /*
         * setup SCL and SDA pins
         * You can connect I2C1 to two different
         * pairs of pins:
         * 1. SCL on PB6 and SDA on PB7
         * 2. SCL on PB8 and SDA on PB9
         * To można sprawdzić w DATASHEET na stronie 60.
         *
         * We are going to use PB6 and PB7
         */
        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
        // Set pins to alternate function.
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
        // set GPIO speed
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        // Set output to open drain --> the line has to be only pulled low, not driven high.
        GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
        // Enable pull up resistors. Wyłączyć, kiedy są wlutowane rezystory.
        GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
        // Init GPIOB
        GPIO_Init(GPIOB, &GPIO_InitStruct);

        // Connect I2C1 pins to AF
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1); // SCL
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1); // SDA

        // Tu 100kHz. To może być dowolna wartość, mniejsza (równa?) niż 400kHz.
        I2C_InitStruct.I2C_ClockSpeed = 100000;
        // I2C_Mode_I2C | I2C_Mode_SMBusDevice | I2C_Mode_SMBusHost.
        I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
        /*
         * 50% duty cycle --> standard. Możliwe wartości : I2C_DutyCycle_16_9 i I2C_DutyCycle_2.
         * Ma znaczenie dla FAST MODE, czyli kiedy speed jest >= 100kHz AND speed < 400kHz.
         */
        I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
        // Own address, not relevant in master mode. 7 lub 10 bit.
        I2C_InitStruct.I2C_OwnAddress1 = 0x00;
        // Disable acknowledge when reading (can be changed later on). Możw być I2C_Ack_Enable
        I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;
        // Set address length to 7 bit addresses. Może być I2C_AcknowledgedAddress_10bit
        I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
        I2C_Init(I2C1, &I2C_InitStruct); // init I2C1

        // enable I2C1
        I2C_Cmd(I2C1, ENABLE);
}

/* This function issues a start condition and
 * transmits the slave address + R/W bit
 *
 * Parameters:
 *              I2Cx --> the I2C peripheral e.g. I2C1
 *              address --> the 7 bit slave address
 *              direction --> the tranmission direction can be:
 *                                              I2C_Direction_Tranmitter for Master transmitter mode
 *                                              I2C_Direction_Receiver for Master receiver
 */
void I2C_start (I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction)
{
        // wait until I2C1 is not busy anymore
        while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
                ;

        // Send I2C1 START condition
        I2C_GenerateSTART(I2Cx, ENABLE);

        /*
         * Wait for I2C1 EV5 --> Slave has acknowledged start condition. The SB bit is set
         * by hardware and an interrupt is generated if the ITEVFEN bit is set. Then the master
         * waits for a read of the SR1 register followed by a write in the DR register with
         * the Slave address
         */
        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
                ;

        /*
         * Send slave Address for write along with direction bit. Direction parameter can be
         * of value I2C_Direction_Transmitter or I2C_Direction_Receiver. After sending an address
         * the ADDR bit is set by hardware and an interrupt is generated if the ITEVFEN bitis set.
         * This is EV6, but in StdPeriph EV6 and EV8_1 seems to be mixed together as
         * I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED, which in turn is commented to be EV6.
         */
        I2C_Send7bitAddress (I2Cx, address, direction);

        /*
         * wait for I2C1 EV6, check if
         * either Slave has acknowledged Master transmitter or
         * Master receiver mode, depending on the transmission
         * direction
         */
        if (direction == I2C_Direction_Transmitter) {
                while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
                        ;

        }
        else if (direction == I2C_Direction_Receiver) {
                while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
                        ;
        }
}

/* This function transmits one byte to the slave device
 * Parameters:
 *              I2Cx --> the I2C peripheral e.g. I2C1
 *              data --> the data byte to be transmitted
 */
void I2C_write_slow (I2C_TypeDef* I2Cx, uint8_t data)
{
        I2C_SendData (I2Cx, data);

        /*
         * Wait for I2C1 EV8_2 --> byte has been transmitted. This one is unclear for me. According to reference
         * manual, EV8_2 is issued when the byte have just been transmitted, but the user hanen't put another byte
         * into data register. So in my opinion this code will work, but a little bit slower than my version (below).
         */
        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
                ;
}

void I2C_write (I2C_TypeDef* I2Cx, uint8_t data)
{
        /*
         * Wait for I2C1 EV8. In the reference manual we read : "A continuous transmit stream can
         * be maintained if the next data to be transmitted is put in DR once the transmission is started (TxE=1)"
         */
        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
                ;

        I2C_SendData (I2Cx, data);
}


/*
 * This function reads one byte from the slave device
 * and acknowledges the byte (requests another byte)
 */
uint8_t I2C_read_ack (I2C_TypeDef* I2Cx)
{
        // enable acknowledge of recieved data
        I2C_AcknowledgeConfig(I2Cx, ENABLE);

        // wait until one byte has been received (EV7)
        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
                ;

        // read data from I2C data register and return data byte
        uint8_t data = I2C_ReceiveData(I2Cx);
        return data;
}

/*
 * This function reads one byte from the slave device
 * and doesn't acknowledge the recieved data
 */
uint8_t I2C_read_nack (I2C_TypeDef* I2Cx)
{
        // disabe acknowledge of received data
        I2C_AcknowledgeConfig(I2Cx, DISABLE);

        // wait until one byte has been received (EV7)
        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
                ;

        // read data from I2C data register and return data byte
        uint8_t data = I2C_ReceiveData(I2Cx);
        return data;
}

/* This funtion issues a stop condition and therefore
 * releases the bus
 */
void I2C_stop (I2C_TypeDef* I2Cx)
{
        // Send I2C1 STOP Condition
        I2C_GenerateSTOP(I2Cx, ENABLE);
}

void I2C_stop (I2C_TypeDef* I2Cx)
{
        /*
         * According to RM : "Stop condition should be programmed during EV8_2 event, when either TxE or BTF is set."
         */
        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
                ;

        // Send I2C1 STOP Condition
        I2C_GenerateSTOP(I2Cx, ENABLE);
}


/*--------------------------------------------------------------------------------------------------------------*/

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

        /*
         * +----------------+
         * | Initialization |
         * +----------------+
         */

        accelgyro.initialize();
            setClockSource(MPU6050_CLOCK_PLL_XGYRO/*0x01*/);
                /*
                 * Excerpt from domcumentation : Upon power up, the MPU-60X0 clock source defaults to the internal oscillator.
                 * However, it is highly recommended that the device be configured to use one of the gyroscopes. Below is the code
                 * which does it:
                 */
                I2Cdev::writeBits(devAddr, MPU6050_RA_PWR_MGMT_1/*0x6B*/, MPU6050_PWR1_CLKSEL_BIT/*2*/, MPU6050_PWR1_CLKSEL_LENGTH/*3*/, source/*MPU6050_CLOCK_PLL_XGYRO         0x01*/);

            setFullScaleGyroRange(MPU6050_GYRO_FS_250/*0x00*/);
                /*
                 * 0x1b register is used to trigger gyroscope self-test and configure the gyroscopes’ full scale range. Below
                 * we set ful scale to be +/- 250 units (seconds?)
                 */
                I2Cdev::writeBits(devAddr, MPU6050_RA_GYRO_CONFIG/*0x1B*/, MPU6050_GCONFIG_FS_SEL_BIT/*4*/, MPU6050_GCONFIG_FS_SEL_LENGTH/*2*/, range/*0x00*/);

            setFullScaleAccelRange(MPU6050_ACCEL_FS_2/*0x00*/);
                /*
                 * Set accelerometer full scale to be +/- 2g.
                 */
                I2Cdev::writeBits(devAddr, MPU6050_RA_ACCEL_CONFIG/*0x1C*/, MPU6050_ACONFIG_AFS_SEL_BIT/*4*/, MPU6050_ACONFIG_AFS_SEL_LENGTH/*2*/, range/*0*/);

            setSleepEnabled(false); // thanks to Jack Elston for pointing this one out!
                /*
                 * By default MPU6050 is in sleep mode after powering up. Below we are waking it back on. This
                 * is done using the same register as in first line,
                 */
                I2Cdev::writeBit(devAddr, MPU6050_RA_PWR_MGMT_1/*0x6B*/, MPU6050_PWR1_SLEEP_BIT/*6*/, enabled/*false*/);

        accelgyro.testConnection()
                getDeviceID() == 0x34;
                        /*
                         * This register is used to verify the identity of the device. The contents of WHO_AM_I are
                         * the upper 6 bits of the MPU-60X0’s 7-bit I C address. The Power-On-Reset value of Bit6:Bit1 is 0b110100 == 0x34.
                         */
                        I2Cdev::readBits(devAddr, MPU6050_RA_WHO_AM_I/*0x75*/, MPU6050_WHO_AM_I_BIT/*6*/, MPU6050_WHO_AM_I_LENGTH/*6*/, buffer);
                        return buffer[0];

        /*
         * +----------------+
         * | Main loop      |
         * +----------------+
         */
        int16_t ax, ay, az;
        int16_t gx, gy, gz;

        accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

                /*
                 * In MPU-6000 and MPU-6050 Product Specification Rev 3.3 on pages 36 and 37 we read, that I²C reads and writes
                 * can be performed with single byte or multiple bytes. In single byte mode, we issue (after sending slave
                 * address ofcourse) a register address, and send or receive one byte of data. Multiple byte reads and writes, at the
                 * other hand consist of slave address, regiser address and multiple consecutive bytes od data. Slave puts or gets
                 * first byte from the register with the address we've just sent, and increases this addres by 1 after each byte.
                 *
                 * This is very useful in case of accelerometer and gyroscope because manufacturer has set up the apropriate registers
                 * cnsecutively, so one can read accel, internal temp and gyro data in one read command. Below is the code which does
                 * exactly this:
                 */
                I2Cdev::readBytes(devAddr, MPU6050_RA_ACCEL_XOUT_H/*0x3B*/, 14, buffer);
                *ax = (((int16_t)buffer[0]) << 8) | buffer[1];
                *ay = (((int16_t)buffer[2]) << 8) | buffer[3];
                *az = (((int16_t)buffer[4]) << 8) | buffer[5];
                *gx = (((int16_t)buffer[8]) << 8) | buffer[9];
                *gy = (((int16_t)buffer[10]) << 8) | buffer[11];
                *gz = (((int16_t)buffer[12]) << 8) | buffer[13];

        while(1){
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

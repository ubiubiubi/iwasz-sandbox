#include <stm32f4xx.h>
#include "mpu6050.h"
#include <stdio.h>

void initI2C1 (void)
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
        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
        // Set pins to alternate function.
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
        // set GPIO speed
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        // Set output to open drain --> the line has to be only pulled low, not driven high.
        GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
        // Enable pull up resistors. Wyłączyć, kiedy są wlutowane rezystory.
        GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
        // Init GPIOB
        GPIO_Init(GPIOB, &GPIO_InitStruct);

        // Connect I2C1 pins to AF
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1); // SCL
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1); // SDA

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
        I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
        // Set address length to 7 bit addresses. Może być I2C_AcknowledgedAddress_10bit
        I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
        I2C_Init(I2C1, &I2C_InitStruct); // init I2C1

        // enable I2C1
        I2C_Cmd(I2C1, ENABLE);
}

/**
 *
 */
void initUsart (void)
{
        RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1, ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

        GPIO_InitTypeDef gpioInitStruct;

        gpioInitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
        gpioInitStruct.GPIO_Mode = GPIO_Mode_AF;
        gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        gpioInitStruct.GPIO_OType = GPIO_OType_PP;
        gpioInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOB, &gpioInitStruct);
        GPIO_PinAFConfig (GPIOB, GPIO_PinSource6, GPIO_AF_USART1); // TX
        GPIO_PinAFConfig (GPIOB, GPIO_PinSource7, GPIO_AF_USART1); // RX

        USART_InitTypeDef usartInitStruct;
        usartInitStruct.USART_BaudRate = 9600;
        usartInitStruct.USART_WordLength = USART_WordLength_8b;
        usartInitStruct.USART_StopBits = USART_StopBits_1;
        usartInitStruct.USART_Parity = USART_Parity_No;
        usartInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        usartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_Init (USART1, &usartInitStruct);
        USART_Cmd (USART1, ENABLE);
}

void usartSendString (USART_TypeDef *USARTx, const char *data)
{
        char c;
        while ((c = *data++)) {
                USARTx->DR = c;

                while (!(USARTx->SR & USART_SR_TXE))
                        ;
        }
}

/*
 * This function issues a start condition and
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

/*
 * This function transmits one byte to the slave device
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

void I2C_stop (I2C_TypeDef* I2Cx)
{
        /*
         * According to RM : "Stop condition should be programmed during EV8_2 event, when either TxE or BTF is set."
         */
//        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED) &&
//               !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
//                ;

        // Send I2C1 STOP Condition
        I2C_GenerateSTOP(I2Cx, ENABLE);
}


int main (void)
{

        initI2C1 ();
        initUsart ();

#if 0

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

#endif

        // Configuration:
        I2C_start (I2C1, MPU6050_ADDRESS_AD0_LOW, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
        I2C_write_slow (I2C1, MPU6050_RA_PWR_MGMT_1); // Register address
        I2C_write (I2C1, MPU6050_CLOCK_PLL_XGYRO); // Register value = 0x01. Which means, that DEVICE_RESET, SLEEP, CYCLE and TEMP_DIS are all 0.
        I2C_stop (I2C1);

        I2C_start (I2C1, MPU6050_ADDRESS_AD0_LOW, I2C_Direction_Transmitter);
        I2C_write (I2C1, MPU6050_RA_GYRO_CONFIG);
        I2C_write (I2C1, MPU6050_GYRO_FS_250); // All bits set to zero.
        I2C_stop (I2C1);

        I2C_start (I2C1, MPU6050_ADDRESS_AD0_LOW, I2C_Direction_Transmitter);
        I2C_write (I2C1, MPU6050_RA_ACCEL_CONFIG);
        I2C_write (I2C1, MPU6050_ACCEL_FS_2); // All bits set to zero.
        I2C_stop (I2C1);

        // Simple test if communication is working

        I2C_start (I2C1, MPU6050_ADDRESS_AD0_LOW, I2C_Direction_Transmitter);
        I2C_write (I2C1, MPU6050_RA_WHO_AM_I);
        I2C_stop (I2C1);
        I2C_start (I2C1, MPU6050_ADDRESS_AD0_LOW, I2C_Direction_Receiver);
        uint8_t whoAmI = I2C_read_nack (I2C1); // read one byte and don't request another byte
        I2C_stop (I2C1);

        if (whoAmI == 0x34) {
                usartSendString (USART1, "Accelerometer has been found!\r\n");
        }
        else {
                usartSendString (USART1, "*NO* Accelerometer has been found!\r\n");
        }

        while (1) {
                I2C_start (I2C1, MPU6050_ADDRESS_AD0_LOW, I2C_Direction_Transmitter);
                I2C_write (I2C1, MPU6050_RA_ACCEL_XOUT_H);
                I2C_stop (I2C1);
                I2C_start (I2C1, MPU6050_ADDRESS_AD0_LOW, I2C_Direction_Receiver);
                uint16_t ax = ((uint16_t)I2C_read_ack (I2C1) << 8) | I2C_read_ack (I2C1);
                uint16_t ay = ((uint16_t)I2C_read_ack (I2C1) << 8) | I2C_read_ack (I2C1);
                uint16_t az = ((uint16_t)I2C_read_ack (I2C1) << 8) | I2C_read_ack (I2C1);
                uint16_t temp = ((uint16_t)I2C_read_ack (I2C1) << 8) | I2C_read_ack (I2C1);
                uint16_t gx = ((uint16_t)I2C_read_ack (I2C1) << 8) | I2C_read_ack (I2C1);
                uint16_t gy = ((uint16_t)I2C_read_ack (I2C1) << 8) | I2C_read_ack (I2C1);
                uint16_t gz = ((uint16_t)I2C_read_ack (I2C1) << 8) | I2C_read_nack (I2C1);
                I2C_stop (I2C1);

                printf ("Accel : (%d, %d, %d), temperature : %d, gyro : (%d, %d, %d)\r\n", ax, ay, az, temp, gx, gy, gz);
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

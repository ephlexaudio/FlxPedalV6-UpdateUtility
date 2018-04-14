/*
 * SharedMemoryInt.cpp
 *
 *  Created on: Mar 20, 2016
 *      Author: mike
 */

#include "config.h"
#include "SPI.h"
#include "Utilities.h"


#define OPEN_TRANSFER_CLOSE 0
//extern struct Request request;
using namespace std;

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static const char *device0 = "/dev/spidev0.0";
//static const char *device1 = "/dev/spidev0.1";
static uint8_t mode = 0;
static uint8_t bits = 8;
static uint32_t speed = 100000;

int spiFD;

SPI::SPI()
{
	this->status = 0;
	//int status = 0;
	char tempStr[5];
#if(!OPEN_TRANSFER_CLOSE)
	int ret;
	//cout << "opening device: " << device0 << endl;
	spiFD = open(device0, O_RDWR);
	if (spiFD < 0)
	{
		this->status = 1;
		pabort("can't open device");
	}

	/*
	 * spi mode
	 */


	if(this->status == 0)
	{
		//cout << "setting SPI_IOC_WR_MODE" << endl;
		ret = ioctl(spiFD, SPI_IOC_WR_MODE, &mode);
		if (ret == -1)
		{
			this->status = 1;
			pabort("can't set spi mode");
		}

	}

	if(this->status == 0)
	{
		//cout << "setting SPI_IOC_RD_MODE" << endl;
		ret = ioctl(spiFD, SPI_IOC_RD_MODE, &mode);
		if (ret == -1)
		{
			this->status = 1;
			pabort("can't get spi mode");
		}

	}

	/*
	 * bits per word
	 */

	if(this->status == 0)
	{
		//cout << "setting SPI_IOC_WR_BITS_PER_WORD" << endl;
		ret = ioctl(spiFD, SPI_IOC_WR_BITS_PER_WORD, &bits);
		if (ret == -1)
		{
			this->status = 1;
			pabort("can't set bits per word");
		}

	}

	if(this->status == 0)
	{
		//cout << "setting SPI_IOC_RD_BITS_PER_WORD" << endl;
		ret = ioctl(spiFD, SPI_IOC_RD_BITS_PER_WORD, &bits);
		if (ret == -1)
		{
			this->status = 1;
			pabort("can't get bits per word");
		}

	}

	/*
	 * max speed hz
	 */
	if(this->status == 0)
	{
		//cout << "setting SPI_IOC_WR_MAX_SPEED_HZ" << endl;
		ret = ioctl(spiFD, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
		if (ret == -1)
		{
			this->status = 1;
			pabort("can't set max speed hz");
		}
	}

	if(this->status == 0)
	{
		//cout << "setting SPI_IOC_RD_MAX_SPEED_HZ" << endl;
		ret = ioctl(spiFD, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
		if (ret == -1)
		{
			this->status = 1;
			pabort("can't get max speed hz");
		}
	}

	//printf("spi file descriptor: %d", spiFD );
	//printf("spi mode: %d\n", mode);
	//printf("bits per word: %d\n", bits);
	//printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
#endif

}

SPI::~SPI()
{
	int ret;

	this->status = 0;
	//printf("spi file descriptor: %d", spiFD );
#if(1)
	ret = close(spiFD);
	if(ret == -1)
	{
		this->status = 1;
		pabort("couldn't close SPI port");
	}
#endif


}


#define dbg 0
uint8_t SPI::sendData(char *data, unsigned int length)
{
#if(dbg >= 1)
	cout << "***** ENTERING: SPI::sendData" << endl;
#endif
	int ret;


	uint8_t status = 0;

	for(int i = 0; i < TX_BUFFER_SIZE; i++) this->txBuffer[i] = 0;

	for(int i = 0; i < RX_BUFFER_SIZE; i++) this->rxBuffer[i] = 0;

	strncpy(this->txBuffer, data, 22);
	//this->txBuffer[21] = 0;
	struct spi_ioc_transfer spi;
	//memset(&spi,0, sizeof(spi));

#if(dbg >= 1)
	cout << "file length: " << length << endl;
	cout << "sendData data: " << data << endl;
#endif


	spi.tx_buf        = (unsigned long)(this->txBuffer); // transmit from "txBuffer"
	spi.rx_buf        = (unsigned long)(this->rxBuffer); // receive into "rxBuffer"
	spi.len           = ARRAY_SIZE(this->txBuffer);//204;//sizeof(this->rxBuffer);
	spi.delay_usecs   = 50;
	spi.speed_hz      = speed;
	spi.bits_per_word = bits;
	spi.cs_change = 0;
	spi.tx_nbits = 0;
	spi.rx_nbits = 0;
	//spi.pad = 0;

	errno = 0;
	status = ioctl(spiFD, SPI_IOC_MESSAGE(1), &spi) ;
	//status = write(spiFD,this->txBuffer,length+2);

#if(dbg >= 1)
	cout << "spi transmit status: " << strerror(errno) << endl;
#endif


#if(dbg >= 2)
	puts((const char*)(this->txBuffer));
#endif


	char statusString[3];
	sprintf(statusString,"%d",status);

#if(dbg >= 1)
	cout << "***** EXITING: SPI::sendData: " << statusString << endl;
#endif
	return status;
}

#define dbg 0
uint8_t SPI::getData(char *data, uint16_t length)
{
#if(dbg >= 1)
	cout << "***** ENTERING: SPI::getData" << endl;
#endif

	uint8_t status = 0;
	//int retVal = 0;

#if(dbg >= 2)
	cout << "address: " << address << endl;
#endif


	struct spi_ioc_transfer spi;

	for(int i = 0; i < TX_BUFFER_SIZE; i++) this->txBuffer[i] = 0;

	for(int i = 0; i < RX_BUFFER_SIZE; i++) this->rxBuffer[i] = 0;


	spi.tx_buf        = (unsigned long)(this->txBuffer); // transmit from "txBuffer"
	spi.rx_buf        = (unsigned long)(this->rxBuffer); // receive into "rxBuffer"
	spi.len           = length;//sizeof(this->rxBuffer);
	spi.delay_usecs   = 30;
	spi.speed_hz      = speed;
	spi.bits_per_word = bits;
	spi.cs_change = 0;
	spi.tx_nbits = 0;
	spi.rx_nbits = 0;
	spi.pad = 0;


#if(dbg >= 2)
	puts((const char*)(data));
#endif


#if(dbg >= 1)
	cout << "***** EXITING: SPI::getData" << endl;
#endif

	return status;
}


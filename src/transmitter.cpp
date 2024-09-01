
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <chrono>
#include <thread>
#include <cmath>
#include <climits>

//define
//specific for BCM2835

#define PERIPHERALS_VIRT_BASE 	0x7E000000 		  //base address of peripherals (not used)
#define BCM2835_PERI_PHYS_BASE 	0x20000000 		  //base address of physical address

#define GPIO_BASE_OFFSET 		0x00200000 

//gpio base
#define GPCLK0_OFFSET 			0x00101070 		  //0x7e101070 general purpose clock 0 address

//gpclk GPIO4
#define CLK_MANAGER_PASSWD 		(0x5a << 24) 	  //clock password
#define GPCLK_CTL_MASH(x)     	((x & 0x03) << 9) // division mode
#define GPCLK_CTL_SRC_PLLD		0x06			  //enable PLLD as clk source
#define PLLD_FREQUENCY			500				  //PLLD frequency
#define GPCLK_CTL_ENAB			(0x01 << 4) 	  //clk enable 

#define GPCLK_DIV0_OFFSET		0x00101074		  //offset of gpclk0 divisor 
#define GPCLK_DIVI(x) 			(x)		  		  //value for gpclk divisor	

#define PERIPHERAL_SIZE  		0x01000000  	  // 16 MB



class PeripheralSetup{
private:
	int memFd;		   //fd for /dev/mem
	void* peripherals; //map for /dev/mem
	uint32_t *output;  //output config
	uint32_t *clk;	   //glclk clock 
	uint32_t *div;	   //gpclk divisor
	
	PeripheralSetup(){
		memFd=open("/dev/mem",O_RDWR | O_SYNC);
		//accessing memory
		if(memFd < 0){
			throw std::runtime_error("Error in opening /dev/mem");
		}

		//mapping peripherals with peripherals base offset
		peripherals=mmap(nullptr,PERIPHERAL_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memFd,BCM2835_PERI_PHYS_BASE);

		close(memFd);

		if(peripherals==MAP_FAILED){
			throw std::runtime_error("Error in mmap");
		}

		//output pin config for gpio 4
		output=(uint32_t*)((uintptr_t)peripherals+GPIO_BASE_OFFSET); //output pin 

		//setting gpio pin4 to ALT0
		*output=(*output  & 0xffff8fff) | (0x04 << 12); 

		//gpclk clock config
		clk=(uint32_t*)((uintptr_t)peripherals+GPCLK0_OFFSET);

		//divisor config
		div=(uint32_t*)((uintptr_t)peripherals+GPCLK_DIV0_OFFSET);

	}

};
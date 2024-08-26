#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>


#define PERIPHERALS_VIRT_BASE 	0x7E000000 		  //base address of peripherals
#define BCM2835_PERI_PHYS_BASE 	0x20000000 		  //base address of physical address

#define GPIO_BASE_OFFSET 		0x00200000 

#define GPCLK0_OFFSET 			0x00101070 		  //0x7e101070 general purpose clock 0 address

#define CLK_MANAGER_PASSWD 		(0x5a << 24) 	  //clock password
#define GPCLK_CTL_MASH(x)     	((x & 0x3) << 9)  // division mode
#define GPCLK_CTL_SRC_PLLA		0x04			  //enable PLLA as clk source

#define GPCLK_CTL_ENAB			(0x01 << 4) 	  //enable 

// #define GLCLK_CTL_BUSY		
#define GLCLK_DIV0_OFFSET		0x00101074		  //offset of gpclk0 divisor 
#define GPCLK_DIVI(x) 			(x << 12)		  //value for gpclk divisor	

#define PERIPHERAL_SIZE  		0x01000000  	  // 16 MB



int main(){
	int memFd = open("/dev/mem",O_RDWR | O_SYNC);
	if(memFd < 0){
		std::cerr << "Error in opening /dev/mem";
		return -1;
	}

	void* peripherals=mmap(nullptr,PERIPHERAL_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memFd,BCM2835_PERI_PHYS_BASE);
	close(memFd);

	if(peripherals==MAP_FAILED){
		std::cerr << "Error in mmap";
		return -1;
	}

	
	std::cout<<peripherals<<std::endl;
	std::cout<<(uint32_t*)((uint8_t*)peripherals+GPCLK0_OFFSET);

	munmap(peripherals,PERIPHERAL_SIZE);
}




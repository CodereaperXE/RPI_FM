#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <chrono>
#include <thread>


#define PERIPHERALS_VIRT_BASE 	0x7E000000 		  //base address of peripherals
#define BCM2835_PERI_PHYS_BASE 	0x20000000 		  //base address of physical address

#define GPIO_BASE_OFFSET 		0x00200000 

#define GPCLK0_OFFSET 			0x00101070 		  //0x7e101070 general purpose clock 0 address

#define CLK_MANAGER_PASSWD 		(0x5a << 24) 	  //clock password
#define GPCLK_CTL_MASH(x)     	((x & 0x03) << 9)  // division mode
#define GPCLK_CTL_SRC_PLLD		0x06			  //enable PLLD as clk source

#define GPCLK_CTL_ENAB			(0x01 << 4) 	  //enable 

// #define GPCLK_CTL_BUSY		
#define GPCLK_DIV0_OFFSET		0x00101074		  //offset of gpclk0 divisor 
#define GPCLK_DIVI(x) 			(x)		  //value for gpclk divisor	

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

	uint32_t *output=(uint32_t*)((uintptr_t)peripherals+GPIO_BASE_OFFSET); //output pin 
	*output=(*output  & 0xffff8fff) | (0x04 << 12); //setting gpio pin4 to output mode
	// *output = (*output & ~(7 << 12)) | (4 << 12); // setting GPIO pin 4 to ALT0

	uint32_t *gpclk=(uint32_t*)((uintptr_t)peripherals+GPCLK0_OFFSET);

	*gpclk = CLK_MANAGER_PASSWD | (1<<5); //disable clock
	std::this_thread::sleep_for(std::chrono::microseconds(1000));

	*gpclk = CLK_MANAGER_PASSWD | GPCLK_CTL_SRC_PLLD;
		std::this_thread::sleep_for(std::chrono::microseconds(1000));


	//set the divisor
	uint32_t *divisor=(uint32_t*)((uintptr_t)peripherals+GPCLK_DIV0_OFFSET);
	*divisor=CLK_MANAGER_PASSWD | 0x00005000; //configure clock properly only then it will work lesson learned after many mistakes
	//clock config at 100Mhz
	std::this_thread::sleep_for(std::chrono::microseconds(500));

	//enable clock
	*gpclk = CLK_MANAGER_PASSWD | GPCLK_CTL_SRC_PLLD | GPCLK_CTL_ENAB | GPCLK_CTL_MASH(0x1);  // Enable clock

	if((*gpclk & (1 << 5)) != 0) {
    std::cout << "kill mode still on" << std::endl;
	}

	std::cout<<"dfdfd"<<std::endl;
	munmap(peripherals,PERIPHERAL_SIZE);

	try{
	while(1){

	}
}catch(...){
	std::cout<<"hello";
}
}





// #include <iostream>
// #include <unistd.h>
// #include <fcntl.h>
// #include <sys/mman.h>
// #include <chrono>
// #include <thread>

// #define PERIPHERALS_VIRT_BASE 	0x7E000000 		  //base address of peripherals
// #define BCM2835_PERI_PHYS_BASE 	0x20000000 		  //base address of physical address

// #define GPIO_BASE_OFFSET 		0x00200000 

// #define GPCLK0_OFFSET 			0x00101070 		  //0x7e101070 general purpose clock 0 address

// #define CLK_MANAGER_PASSWD 		(0x5a << 24) 	  //clock password
// #define GPCLK_CTL_MASH(x)     	((x & 0x03) << 9)  // division mode
// #define GPCLK_CTL_SRC_PLLA		0x06			  //enable PLLA as clk source

// #define GPCLK_CTL_ENAB			(0x01 << 4) 	  //enable 

// // #define GPCLK_CTL_BUSY		
// #define GPCLK_DIV0_OFFSET		0x00101074		  //offset of gpclk0 divisor 
// #define GPCLK_DIVI(x) 			(x)		  //value for gpclk divisor	

// #define PERIPHERAL_SIZE  		0x01000000  	  // 16 MB



// int main(){
// 	int memFd = open("/dev/mem",O_RDWR | O_SYNC);
// 	if(memFd < 0){
// 		std::cerr << "Error in opening /dev/mem";
// 		return -1;
// 	}

// 	void* peripherals=mmap(nullptr,PERIPHERAL_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memFd,BCM2835_PERI_PHYS_BASE);
// 	close(memFd);

// 	if(peripherals==MAP_FAILED){
// 		std::cerr << "Error in mmap";
// 		return -1;
// 	}	

// 	// *(uint32_t*)((uintptr_t)peripherals+GPIO_BASE_OFFSET+0x08) =(*(uint32_t*)((uintptr_t)peripherals+GPIO_BASE_OFFSET+0x08) & 0xffffffc7) | (0x02 << 3);
// 	// // std::cout<<peripherals<<std::endl;
// 	// std::cout<< std::hex <<*(uint32_t*)((uintptr_t)peripherals+GPCLK0_OFFSET);
// 	// std::cout<<std::endl;

// 	// std::cout<<"busy " <<std::hex<< (*(uint32_t*)((uintptr_t)peripherals+GPCLK0_OFFSET) & 0x80) <<std::endl;
// 	// //setting up clock
// 	// uint32_t data =*(uint32_t*)((uintptr_t)peripherals+GPCLK0_OFFSET) | CLK_MANAGER_PASSWD | GPCLK_CTL_SRC_PLLA;	
	
// 	// std::cout<< std::hex <<data<<std::endl;

// 	// *(uint32_t*)((uintptr_t)peripherals+GPCLK0_OFFSET)=data;
// 	// std::this_thread::sleep_for(std::chrono::microseconds(100));

// 	// //setting up divisor
// 	// *(uint32_t*)((uintptr_t)peripherals+GLCLK_DIV0_OFFSET) |= CLK_MANAGER_PASSWD | GPCLK_DIVI(0x01);
	
// 	// //settup
// 	// *(uint32_t*)((uintptr_t)peripherals+GPCLK0_OFFSET) |= CLK_MANAGER_PASSWD | GPCLK_CTL_ENAB | GPCLK_CTL_MASH(0x1) | GPCLK_CTL_SRC_PLLA;

// 	// std::cout<< std::hex <<data<<std::endl;

// 	// std::cout<<"busy " <<std::hex<< (*(uint32_t*)((uintptr_t)peripherals+GPCLK0_OFFSET) & 0x80) <<std::endl;

// 	//iteration 2

// 	//start with setting output pinmode gpio
// 	// uint32_t *output=(uint32_t*)((uintptr_t)peripherals+GPIO_BASE_OFFSET); //output pin 
// 	// *output=(*output  & 0xffff8fff) | (0x04 << 12); //setting gpio pin4 to output mode

// 	// //setting glclk 
// 	// uint32_t *gpclk=(uint32_t*)((uintptr_t)peripherals+GPCLK0_OFFSET);

// 	// std::cout<<"program is running"<<std::endl;
// 	// *gpclk |= CLK_MANAGER_PASSWD | GPCLK_CTL_SRC_PLLA;
// 	// std::cout<<"busy "<<std::hex<<(*gpclk & 0x80);
// 	// std::this_thread::sleep_for(std::chrono::microseconds(100));

// 	// //setting up divisor

// 	// uint32_t *divisor=(uint32_t*)((uintptr_t)peripherals+GPCLK_DIV0_OFFSET);

// 	// *divisor|= CLK_MANAGER_PASSWD | 0x10;

// 	// *gpclk |= CLK_MANAGER_PASSWD | GPCLK_CTL_SRC_PLLA | GPCLK_CTL_ENAB| GPCLK_CTL_MASH(0x1);

	
// 	// std::cout<<"busy "<< std::hex<<(*gpclk & 0x80);

// 	// std::cout<<"dfd"<<std::endl;


// 	//iteration 3
// 	//disable the clock
// 	//set the divisor
// 	//set the clock source and mode mash mode
// 	//enable the clock

// 	uint32_t *output=(uint32_t*)((uintptr_t)peripherals+GPIO_BASE_OFFSET); //output pin 
// 	// *output=(*output  & 0xffff8fff) | (0x04 << 12); //setting gpio pin4 to output mode
// 	*output = (*output & ~(7 << 12)) | (4 << 12); // setting GPIO pin 4 to ALT0

// 	uint32_t *gpclk=(uint32_t*)((uintptr_t)peripherals+GPCLK0_OFFSET);

// 	*gpclk = CLK_MANAGER_PASSWD | (1<<5); //disable clock
// 	std::this_thread::sleep_for(std::chrono::microseconds(500));

// 	//set the divisor
// 	uint32_t *divisor=(uint32_t*)((uintptr_t)peripherals+GPCLK_DIV0_OFFSET);
// 	*divisor=CLK_MANAGER_PASSWD | 0x100;

// 	std::this_thread::sleep_for(std::chrono::microseconds(500));

// 	//enable clock
// 	*gpclk = CLK_MANAGER_PASSWD | GPCLK_CTL_SRC_PLLA | GPCLK_CTL_ENAB | GPCLK_CTL_MASH(0x1);  // Enable clock

// 	if((*gpclk & (1 << 5)) != 0) {
//     std::cout << "kill mode still on" << std::endl;
// 	}

// 	std::cout<<"dfdfd"<<std::endl;
// 	munmap(peripherals,PERIPHERAL_SIZE);

// 	while(1){

// 	}
// }

//working code
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <chrono>
#include <thread>

#define BCM2835_PERI_PHYS_BASE 0x20000000  // Physical base address of peripherals
#define GPIO_BASE_OFFSET 0x00200000        // Offset for GPIO
#define GPCLK0_OFFSET 0x00101070           // Offset for GPCLK0
#define GPCLK_DIV0_OFFSET 0x00101074       // Offset for GPCLK0 divisor

#define PERIPHERAL_SIZE 0x01000000         // 16 MB
#define CLK_MANAGER_PASSWD (0x5a << 24)    // Clock manager password

#define GPIO_PIN4_FUNC_SHIFT 12
#define GPIO_PIN4_FUNC_MASK (7 << GPIO_PIN4_FUNC_SHIFT)
#define GPIO_FUNC_ALT0 (4 << GPIO_PIN4_FUNC_SHIFT)

#define GPCLK_CTL_ENAB (0x01 << 4)          // Enable GPCLK0
#define GPCLK_CTL_SRC_PLLA 0x06             // Source from PLLA
#define GPCLK_CTL_MASH(x) ((x & 0x03) << 9) // MASH mode
#define GPCLK_DIVI(x) (x)                   // Divisor value

void setup_gpio_pin4(volatile uint32_t* gpio_base) {
    // Set GPIO pin 4 to ALT0 (GPCLK0) mode
    gpio_base[1] = (gpio_base[1] & ~GPIO_PIN4_FUNC_MASK) | GPIO_FUNC_ALT0;
}

void set_gpio_pin4_high(volatile uint32_t* gpio_base) {
    gpio_base[7] = (1 << 4); // Set GPIO pin 4 high
}

void set_gpio_pin4_low(volatile uint32_t* gpio_base) {
    gpio_base[10] = (1 << 4); // Set GPIO pin 4 low
}

void setup_gpclk0(volatile uint32_t* gpclk_base, volatile uint32_t* divisor_base) {
    // Disable GPCLK0
    gpclk_base[0] = CLK_MANAGER_PASSWD | (1 << 5);
    std::this_thread::sleep_for(std::chrono::microseconds(500));

    // Set divisor (e.g., divisor = 0x100)
    divisor_base[0] = CLK_MANAGER_PASSWD | GPCLK_DIVI(0x100);
    std::this_thread::sleep_for(std::chrono::microseconds(500));

    // Set up GPCLK0 (source PLLA, enable, and MASH mode)
    gpclk_base[0] = CLK_MANAGER_PASSWD | GPCLK_CTL_SRC_PLLA | GPCLK_CTL_ENAB | GPCLK_CTL_MASH(0x1);
}

int main() {
    int memFd = open("/dev/mem", O_RDWR | O_SYNC);
    if (memFd < 0) {
        std::cerr << "Error in opening /dev/mem" << std::endl;
        return -1;
    }

    void* peripherals = mmap(nullptr, PERIPHERAL_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memFd, BCM2835_PERI_PHYS_BASE);
    close(memFd);

    if (peripherals == MAP_FAILED) {
        std::cerr << "Error in mmap" << std::endl;
        return -1;
    }

    volatile uint32_t* gpio_base = (volatile uint32_t*)((uintptr_t)peripherals + GPIO_BASE_OFFSET);
    volatile uint32_t* gpclk_base = (volatile uint32_t*)((uintptr_t)peripherals + GPCLK0_OFFSET);
    volatile uint32_t* divisor_base = (volatile uint32_t*)((uintptr_t)peripherals + GPCLK_DIV0_OFFSET);

    setup_gpio_pin4(gpio_base);

    std::cout << "GPIO pin 4 is set to ALT0 (GPCLK0)" << std::endl;

    while (true) {
        set_gpio_pin4_high(gpio_base);
        std::cout << "GPIO pin 4 set high" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        set_gpio_pin4_low(gpio_base);
        std::cout << "GPIO pin 4 set low" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    munmap(peripherals, PERIPHERAL_SIZE);
    return 0;
}







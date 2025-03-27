#include <stdio.h>
#include <sys/mman.h> 
#include <fcntl.h> 
#include <unistd.h>
#define _BSD_SOURCE

#define GPIO_DIP_OFFSET 2
#define GPIO_LED_OFFSET 0 
#define DIPS_AND_LEDS_BASEADDR 	0x41200000
#define RGB_LEDS_BASEADDR 		0x41210000

/**
 * @brief Color Combination
 */
#define RGB_OFF	0x0
#define RGB_R	0x1
#define RGB_G	0x2
#define RGB_B	0x4
#define RGB_RG	0x3
#define RGB_RB	0x5
#define RGB_BG	0x6
#define RGB_ALL	0x7

static const int SW_RGB_map[8] = {0x0, 0x03, 0xC, 0xF, 0x30, 0x33, 0x3C, 0x3F};


// the below code uses a device called /dev/mem to get a pointer to a physical
// address.  We will use this pointer to read/write the custom peripheral
volatile unsigned int * get_a_pointer(unsigned int phys_addr)
{

	int mem_fd = open("/dev/mem", O_RDWR | O_SYNC); 
	void *map_base = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, phys_addr); 
	volatile unsigned int *radio_base = (volatile unsigned int *)map_base; 
	return (radio_base);
}

int main()
{
    printf("\r\n\r\n\r\nLab 9 - Switch Controlled RBG LEDs Program\n\r");
	printf("Author: Yuchen Zhou\n\r");

// first, get a pointer to the peripheral base address using /dev/mem and the function mmap
    volatile unsigned int *DIPandLEDs_ptr = get_a_pointer(DIPS_AND_LEDS_BASEADDR);	
	volatile unsigned int *RGBleds_ptr = get_a_pointer(RGB_LEDS_BASEADDR);	
	int switches_value = *(DIPandLEDs_ptr+GPIO_DIP_OFFSET);
    while (1)
    {
		*RGBleds_ptr	= SW_RGB_map[switches_value];
	    usleep(500000);
		switches_value 	= *(DIPandLEDs_ptr+GPIO_DIP_OFFSET);
    }
    return 0;
}

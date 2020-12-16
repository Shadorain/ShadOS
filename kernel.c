// kernel.c
#include "keyboard_map.h"

// 25 lines each of 80 columns; each element takes 2 bytes
#define LINES 25
#define COL_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE BYTES_FOR_EACH_ELEMENT * COL_IN_LINE * LINES

#define KB_DATA_PORT 0x60
#define KB_STATUS_PORT 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

#define ENTER_KEY_CODE 0x1C

extern unsigned char kb_map[128];
extern void kb_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);

struct IDT_entry {
	unsigned short int offset_lowbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_highbits;
};

struct IDT_entry IDT[IDT_SIZE];

char *vidptr = (char*)0xb8000; 	// Start of vid memory in Protected Mode
unsigned int curr_loc = 0; // cursor loc

void idt_init (void) {
	unsigned long kb_addr;
	unsigned long idt_addr;
	unsigned long idt_ptr[2];

	kb_addr = (unsigned long)kb_handler;
	IDT[0x21].offset_lowbits = kb_addr & 0xffff;
	IDT[0x21].selector = 0x08;
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = 0x8e;
	IDT[0x21].offset_highbits = (kb_addr & 0xffff0000) >> 16;

	/* -- Ports -- 
	*		  PIC1	PIC2
	* Command 0x20	0xA0
	* Data	  0x21	0xA1
	*/
	write_port(0x20, 0x11);
	write_port(0xA0, 0x11);

	write_port(0x21, 0x20);
	write_port(0xA1, 0x28);

	write_port(0x21, 0x00);
	write_port(0xA1, 0x00);

	write_port(0x21, 0x01);
	write_port(0xA1, 0x01);

	write_port(0x21, 0xff);
	write_port(0xA1, 0xff);

	idt_addr = (unsigned long)IDT;
	idt_ptr[0] = (sizeof(struct IDT_entry) * IDT_SIZE) + ((idt_addr & 0xffff) << 16);
	idt_ptr[1] = idt_addr >> 16;

	load_idt(idt_ptr);
}

void kb_init (void) {
	write_port(0x21, 0xFD);
}

void kprint (const char *str) {
	unsigned int i = 0;
	while (str[i] != '\0') {
		vidptr[curr_loc++] = str[i++];
		vidptr[curr_loc++] = 0x07;
	}
}

void kprint_nl (void) {
	unsigned int ln_size = BYTES_FOR_EACH_ELEMENT * COL_IN_LINE;
	curr_loc = curr_loc + (ln_size - curr_loc % (ln_size));
}

void clr_screen (void) {
	unsigned int i = 0;
	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = 0x07;
	}
}

void kb_handler_main (void) {
	unsigned char status;
	char keycode;

	write_port(0x20, 0x20);

	status = read_port(KB_STATUS_PORT);
	if (status & 0x01) {
		keycode = read_port(KB_DATA_PORT);
		if (keycode < 0) return;
		if (keycode == ENTER_KEY_CODE) {
			kprint_nl();
			return;
		}

		vidptr[curr_loc++] = kb_map[(unsigned char)keycode];
		vidptr[curr_loc++] = 0x07;
	}
}

void kmain(void)
{
	const char *str = "Hello, World!";
	/* clr_screen(); */
	kprint(str);
	kprint_nl();

	idt_init();
	kb_init();
	return;
}

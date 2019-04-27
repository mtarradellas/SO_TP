#include <stdint.h>
#include <defs.h>
#include <interruptions.h>
 
#pragma pack(push)  // push current alignment
#pragma pack (1)    // alignes structures to 1 byte

/* Interruption description */
typedef struct {
  uint16_t offset1;  // offset bits 0..15
  uint16_t selector;  // code segment selector
  uint8_t ist;        // bits 0..2 holds Interrupt Stack Table offset, rest bits zero
  uint8_t type_attr;  // type and attributes     
  uint16_t offset2;  // offset bits 16..31
  uint32_t offset3;  // offset bits 32..63
  uint32_t zero;      // reserved
} IDTDesc;

#pragma pack(pop)   // pops for previous alignment

IDTDesc * idt = (IDTDesc *) 0;  // IDT created at position 0

static void IDTEntrySetup (int index, uint64_t offset);
void loadIDT(); 



void loadIDT() {
  _cli();   // disables interruptions 

  IDTEntrySetup (0x00, (uint64_t)&_exception0Handler);  //0 division
  IDTEntrySetup (0x06, (uint64_t)&_exceptionInvalidOpcodeHandler);  //invalid opcode
  IDTEntrySetup (0x20, (uint64_t)&_irq00Handler);   //timer tick 
  IDTEntrySetup (0x21, (uint64_t)&_irq01Handler);   //keyboard 
  IDTEntrySetup (0x80, (uint64_t)&_syscall_handler);    //system calls
  
  picMasterMask(0xFC);    // enables only timer tick and keyboard interruptions
  picSlaveMask(0xFF);     // disables all slave pic interruptions
   

  _sti();   // reenables interruptions

}

static void IDTEntrySetup (int index, uint64_t offset) {
  idt[index].offset1 = offset & 0xFFFF;
  idt[index].selector = 0x08;
  idt[index].ist = 0;
  idt[index].type_attr = ACS_INT;
  idt[index].offset2 = (offset >> 16) & 0xFFFF;
  idt[index].offset3 = (offset >> 32) & 0xFFFFFFFF;
  idt[index].zero = (uint64_t) 0;
}
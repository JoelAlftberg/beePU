# Memory Map


## RAM
`0x0000 - 0x7FFF` - fixed RAM (programs, data) (32kB)

## Banked RAM window
`0x8000 - 0xDFFF` - banked window (24kB)  

Bank index `1` is reserved for the stack.  

## Hardware
`0xE000 - 0xE7FF`  - display (2kB)  
Display layout is 80 columns x 25 rows  

`0xE800 - 0xEFFF` - reserved I/O (2kB)

## ROM Region  
`0xF000 - 0xFFFF` - ROM (4kB)

### Interrupt vectors
The last 32 bytes are reserved for interrupt vectors.  
For more information read the [Interrupt Vectors](interrupt_vectors.md)  
`0xFFC0 - 0xFFFF` - Interrupt vectors

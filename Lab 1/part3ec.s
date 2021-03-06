AREA Myprog, CODE, READONLY
ENTRY
EXPORT __main

;don't change these addresses!
PCR22 	  EQU 0x4004A058 ;PORTB_PCR22  address
SCGC5 	  EQU 0x40048038 ;SIM_SCGC5    address
PDDR 	  EQU 0x400FF054 ;GPIOB_PDDR   address
PCOR 	  EQU 0x400FF048 ;GPIOB_PCOR   address
PSOR      EQU 0x400FF044 ;GPIOB_PSOR   address

ten		  EQU 0x00000400 ; 1 << 10
eight     EQU 0x00000100 ; 1 << 8
twentytwo EQU 0x00400000 ; 1 << 22

__main
; Your code goes here!

MOV   R3, #7
MOV   R7, #19
MOV   R1, #0xbeef
BL    LEDSETUP

MOV R0,#7
BL fib
BL NUMDIVIDE

BL DOTDELAY
BL DOTDELAY

MOV R0, #8
BL fib
BL NUMDIVIDE

BL DOTDELAY
BL DOTDELAY

MOV R0, #9
BL fib
BL NUMDIVIDE 

B forever
;BL fib
;BL NUMDIGIT
;BL DIGIT

;		MOV R0,#5
;		BL MorseDigit
;		BL DOTDELAY
;		BL DOTDELAY
;		BL MorseDigit


;BL MorseDigit






NUMDIVIDE
PUSH{LR}
PUSH {R0}
PUSH {R1}
PUSH{R2}
PUSH {R4}
MOV R4, #0
MOV R1, #10
numdividel1
UDIV R2,R0,R1
MUL R2, R2, R1
SUB R3, R0,R2 ; gives remainder
PUSH {R3}
ADD R4,R4, #1
UDIV R0,R0,R1
CMP R0,#0
BGT numdividel1
numdividel2
POP {R0}
BL MorseDigit
BL DOTDELAY
BL DOTDELAY
BL DOTDELAY
SUB R4, R4, #1
CMP R4, #0
BGT numdividel2
POP {R4}
POP {R2}
POP {R1}
POP {R0}
POP {LR}
BX LR





;		BL LEDDOT
;		BL DOTDELAY
;		BL LEDDASH

NUMDIGIT ; calculate number of digits
PUSH {LR}
PUSH {R2}
PUSH {R4}
PUSH {R3}
MOV R2, #0
MOV R1,#10
MOV R3,#10
ndigit
MOV R4,R0
SUB R4,R4,R1
ADD R2,R2,#1
MUL R1,R1,R3
CMP R4,#0
BGE ndigit
MOV R1,R2
POP {R3}
POP {R4}
POP {R2}
POP {LR}
BX LR





CALLDIGIT ; calls N calls of DIGIT


DIGIT ; finds digit of N place, calls morse.
PUSH{LR}
PUSH {R0}
PUSH {R1}
PUSH {R2}
PUSH {R3}
PUSH {R4}
PUSH {R5}
MOV R2,#10
digitl
MOV R3,#1
MOV R4,R1
MOV R5,#0
tenton
MUL R3,R3,R2 ; multiply by 10 each time
SUB R4,R4, #1
CMP R4,#0
BNE tenton
subloop
SUB R0,R0, R3
ADD R5, R5, #1
CMP R0,#0
BGE subloop
SUB R5, R5, #1
ADD R0,R0, R3
PUSH {R0}
MOV R0,R5
BL MorseDigit
;		PUSH {R1}
;		MOV R1,#3
;loopdig
;		BL DOTDELAY
;		SUB R1,R1,#1
;		CMP R1,#0
;		BNE loopdig
;		POP {R1}

POP {R0}
SUB R1,R1, #1
CMP R0,#0
BNE digitl
POP {R5}
POP {R4}
POP {R3}
POP {R2}
POP {R1}
POP {R0}
POP {LR}
BX LR








MorseDigit
PUSH {LR}
CMP R0,#0
BEQ zero
CMP R0, #1
BEQ one
CMP R0,#2
BEQ two
CMP R0, #3
BEQ three
CMP R0,#4
BEQ four
CMP R0, #5
BEQ five
CMP R0,#6
BEQ six
CMP R0, #7
BEQ seven
CMP R0,#8
BEQ eightl
CMP R0, #9
BEQ nine
POP {LR}
BX LR

zero
PUSH {R0}
MOV R0,#5
BL DASHLOOP
POP {R0}
POP{LR}
BX LR
one
PUSH {R0}
BL LEDDOT
BL DOTDELAY
MOV R0,#4
BL DASHLOOP
POP {R0}
POP{LR}
BX LR

two
PUSH {R0}
MOV R0,#2
BL DOTLOOP
MOV R0,#3
BL DASHLOOP
POP {R0}
POP{LR}
BX LR
three
PUSH {R0}
MOV R0,#3
BL DOTLOOP
MOV R0,#2
BL DASHLOOP
POP {R0}
POP{LR}
BX LR
four
PUSH {R0}
MOV R0,#4
BL DOTLOOP
MOV R0,#1
BL DASHLOOP
POP {R0}
POP{LR}
BX LR
five
PUSH {R0}
MOV R0,#5
BL DOTLOOP
POP {R0}
POP{LR}
BX LR
six
PUSH {R0}
BL LEDDASH
BL DOTDELAY
MOV R0,#4
BL DOTLOOP
POP {R0}
POP{LR}
BX LR
seven
PUSH {R0}
MOV R0,#2
BL DASHLOOP
MOV R0,#3
BL DOTLOOP
POP {R0}
POP{LR}
BX LR
eightl
PUSH {R0}
MOV R0,#3
BL DASHLOOP
MOV R0,#2
BL DOTLOOP
POP {R0}
POP{LR}
BX LR
nine
PUSH {R0}
MOV R0,#4
BL DASHLOOP
BL LEDDOT
POP {R0}
POP{LR}
BX LR



B     forever


DASHLOOP
PUSH {LR}
PUSH {R0}
dashloop1
BL LEDDASH
BL DOTDELAY
SUB R0,R0,#1
CMP R0,#0
BNE dashloop1
POP {R0}
POP{LR}
BX LR


DOTLOOP
PUSH {LR}
PUSH {R0}
dotloop1
BL LEDDOT
BL DOTDELAY
SUB R0,R0,#1
CMP R0,#0
BNE dotloop1
POP {R0}
POP {LR}
BX LR


LEDDASH

  PUSH {LR}
  PUSH {R0}
  BL LEDON
  MOV R1, #3
loopr
  BL DOTDELAY
  SUB R1,R1,#1
  CMP R1,#0
  BNE loopr
  BL LEDOFF
  POP {R0}
  POP {LR}
  BX LR

LEDDOT

  PUSH {LR}
  PUSH {R0}
  BL LEDON
  BL DOTDELAY
  BL LEDOFF
  POP {R0}
  POP {LR}
  BX LR

DOTDELAY
  PUSH {LR}
  PUSH {R1}
  MOV R1, #16
ldelay
  BL DELAY
  SUB R1, R1, #1
  CMP R1, #0
  BNE ldelay
  POP {R1}
  POP{LR}
  BX LR
DELAY
  PUSH {LR}
  PUSH {R1}
  MOV R1, 0xEEEE
loopy
  SUB R1,R1,#1
  CMP R1,#0
  BNE loopy
  POP {R1}
  POP {LR}
  BX LR

fib
PUSH {LR}
CMP R0,#0
BEQ returnzero
CMP R0,#1
BEQ returnone
PUSH {R1}
SUB R0,R0,#1
PUSH {R0}
BL fib
MOV R1,R0
POP {R0}
SUB R0,R0,#1
BL fib
ADD R0,R0,R1
POP {R1}
POP {LR}
BX LR
returnzero
MOV R0,#0
POP {LR}
BX LR
returnone
MOV R0,#1
POP {LR}
BX LR

; Call this function first to set up the LED
LEDSETUP
    PUSH  {R4, R5} ; To preserve R4 and R5
    LDR   R4, =ten ; Load the value 1 << 10
    LDR		R5, =SCGC5
    STR		R4, [R5]

    LDR   R4, =eight
    LDR   R5, =PCR22
    STR   R4, [R5]

    LDR   R4, =twentytwo
    LDR   R5, =PDDR
    STR   R4, [R5]
    POP   {R4, R5}
    BX    LR

; The functions below are for you to use freely
LEDON
    PUSH  {R4, R5}
    LDR   R4, =twentytwo
    LDR   R5, =PCOR
    STR   R4, [R5]
    POP   {R4, R5}
    BX    LR
LEDOFF
    PUSH  {R4, R5}
    LDR   R4, =twentytwo
    LDR   R5, =PSOR
    STR   R4, [R5]
    POP   {R4, R5}
    BX    LR

forever
  B		forever						; wait here forever
  END

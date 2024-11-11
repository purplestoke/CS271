// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// The algorithm is based on repetitive addition.

//// Replace this comment with your code.

// INITIALIZE R2 WITH 0
@2
M=0

// INITIALIZE i WITH 0
// ACTS AS A COUNTER
@i
M=0

(LOOP)
  // LOAD VALUE i INTO D REGISTER
  @i
  D=M

  // SUBTRACTS VALUE OF R0 (MULTIPLIER) FROM D
  @0
  D=D-M

  // CHECKS IF i HAS REACHED VALUE OF R0 (i >= R0)
  @END
  D;JGE

  // LOAD VALUE OF R1 INTO D
  @1
  D=M

  // ADDS R1 TO R2 -> R2
  @2
  M=D+M

  // INCREMENT i BY 1
  @i
  M=M+1

  // JUMP BACK TO LOOP LABEL
  @LOOP
  0;JMP
(END)

// END OF PROGRAM
@END
0;JMP

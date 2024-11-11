// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed,
// the screen should be cleared.

//// Replace this comment with your code.

// INITIALIZE state TO -1
// ENSURES PROGRAM ENTERS UPDATESCREEN ROUTINE AT STARTUP
@state
M=-1

// SET D REGISTER TO 0
D=0

// JUMP TO UPDATESCREEN LABEL
@UPDATESCREEN
0;JMP

(LOOP)
  // READ KEYBOARD INPUT AND SET TO D
  @KBD
  D=M

  // CHECK IF KEYBOARD VALUE IS 0 (NO KEY PRESSED)
  // IF 0 JUMP TO UPDATESCREEN
  @UPDATESCREEN
  D;JEQ

  // SET D TO -1 IF KEYBOARD INPUT IS NON-ZERO
  D=-1

(UPDATESCREEN)
  // STORE CURRENT VALUE OF D IN ARG VARIABLE
  @ARG
  M=D

  // CHECK IF STATE HAS CHANGED
  // CALCULATE DIFFERENCE BETWEEN D AND state
  @state
  D=D-M

  // IF D == state, JUMP BACK TO LOOP
  @LOOP
  D;JEQ

  // LOAD ARG INTO D TO UPDATE state
  @ARG
  D=M
  @state
  M=D

  // LOAD SCREEN BASE ADDRESS INTO D
  @SCREEN
  D=A

  // ADD OFFSET OF 8192 TO SCREEN ADDRESS
  @8192
  D=D+A

  // STORE SCREEN OFFSET IN i (i IS A POINTER)
  @i
  M=D

(SETLOOP)
  // DECREMENT i BY 1
  @i
  D=M-1
  M=D

  // CHECK IF i IS NEGATIVE
  // IF SO JUMPS TO LOOP
  @LOOP
  D;JLT

  // LOAD state INTO D (CURRENT VALUE TO BE WRITTEN TO SCREEN)
  @state
  D=M

  // WRITES state VALUE TO SCREEN MEMORY AT ADRESS POINTED BY i
  @i
  A=M
  M=D

  // JUMP BACK TO SETLOOP TO DECREMENT i AND WRITE TO SCREEN
  // UNTIL i IS NEGATIVE
  @SETLOOP
  0;JMP

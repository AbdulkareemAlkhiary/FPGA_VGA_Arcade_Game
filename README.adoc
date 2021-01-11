:Author: araasai
:Email: {AuthorEmail}
:Date: 09/12/2020
:Revision: version#
:License: Public Domain


= FPGA VGA Arcade Game

Developed a game in which the protagonist combats a large number of enemies by shooting at them while dodging their fire. 
The controlling player must rely primarily on reaction times to succeed. 
The player's goal is to shoot as quickly as possible at anything that moves or threatens them to reach the end of the level. 

== Step 1: Design Implementation

The hardware modules we incorporated were 2 PIO’s. 
These PIO’s were responsible for sending the scan code from the PS2 keyboard, and the code_ready signal sent by the PS2 driver. 
This allowed us to read the codes from the keyboard and turn them into useful keystrokes implemented into our software.

BOM: 
 1. PS2 Keyboard
 2. DE1-Board

== Step 2: Load the code

Upload the code contained in this sketch on to your board

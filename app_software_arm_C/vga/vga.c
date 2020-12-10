#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
//#include "altera_up_avalon_ps2.h"
//#include "altera_up_ps2_keyboard.h"

#define HW_REGS_BASE ( 0xFC000000 )
#define HW_OCRAM_BASE ( 0xC8000000 )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )
#define FPGA_CHAR_BASE      0xC9000000


#define PHYSMEM_32_reg(addr) (*((unsigned int *)(virtual_base_reg + (addr & HW_REGS_MASK))))
#define PHYSMEM_16_reg(addr) (*((unsigned short *)(virtual_base_reg + (addr & HW_REGS_MASK))))

#define PHYSMEM_32(addr) (*((unsigned int *)(virtual_base + (addr & HW_REGS_MASK))))
#define PHYSMEM_16(addr) (*((unsigned short *)(virtual_base + (addr & HW_REGS_MASK))))


 #define NUM_COLORS 256 /*number of colors in mode 0x13 */ 
 #define SPAWN_WIDTH_START 20 /*width in pixels of mode 0x13 */ 
 #define SPAWN_WIDTH_END 300 /*width in pixels of mode 0x13 */ 
 #define SPAWN_HEIGHT_START 50 /*height in pixels of mode 0x13 */
 #define SPAWN_HEIGHT_END 150 /*height in pixels of mode 0x13 */


/****************************************************************************************
 * Draw a filled rectangle on the VGA monitor 
****************************************************************************************/

struct Bullet
{
	int x1;
	int x2;
	int y1;
	int y2;
	
	short color;
	
	//length or width
	char size;
	
	//increment x and y value
	int speed;
};

struct Player
{
	int x1;
	int x2;
	int y1;
	int y2;
	
	//status
	int alive;
	
	int speed;
	
	short color;
	
	char weapon;

};

struct Enemy
{
	int x1;
	int x2;
	int y1;
	int y2;
	
	//status
	int alive;
	
	int direc;
	
	int shootTimer;
	
	int shoot;
	
	short color;
	
	int hasBullet;
	
	struct Bullet bullet;
};

enum direction
{
up, down, left, right
};

void VGA_box(int x1, int y1, int x2, int y2, short pixel_color, void *virtual_base)
{ 
	unsigned int pixel_ptr, row, col;

	/* assume that the box coordinates are valid */
	for (row = y1; row <= y2; row++)
		for (col = x1; col <= x2; ++col)
		{
			pixel_ptr = HW_OCRAM_BASE + (row << 10) + (col << 1);
			PHYSMEM_16(pixel_ptr) = pixel_color;		// set pixel color
		}
}

void VGA_text(int x, int y, char * text_ptr, void *virtual_base)
{
	int offset;
  	unsigned int  char_ptr;

	/* assume that the text string fits on one line */
	offset = (y << 7) + x;
	while ( *(text_ptr) )
	{
		char_ptr =  FPGA_CHAR_BASE + offset;
		
		PHYSMEM_32(char_ptr) = *(text_ptr);
		
		++text_ptr;
		++offset;
	}
}

//implement reading pixels from bmp or png or jpeg or something

void draw_enemy(int x1, int y1, int x2, int y2, short pixel_color, void *virtual_base)
{ 
	unsigned int pixel_ptr, row, col;
	int temp;
			if(y1 < 1)
			{
				temp = y2 - y1;
				y1 = 1;
				y2 = y1 + temp;
			}
			else if(x1 < 1)
			{
				temp = x2 - x1;
				x1 = 1;
				x2 = x1 + temp;
			}
			else if(y2 > 315)
			{
				temp = y1 - y2;
				y2 = 315;
				y1 = y2 + temp;
			}
			else if(x2 > 315)
			{
				temp = x1 - x2;
				x2 = 315;
				x1 = x2 + temp;
			}
			
	/* assume that the box coordinates are valid */
	for (row = y1; row <= y2; row++)
		for (col = x1; col <= x2; ++col)
		{
			
			pixel_ptr = HW_OCRAM_BASE + (row << 10) + (col << 1);
			PHYSMEM_16(pixel_ptr) = pixel_color;		// set pixel color
		}
}

void draw_player(int x1, int y1, int x2, int y2, short pixel_color, void *virtual_base)
{ 
	unsigned int pixel_ptr, row, col;

	/* assume that the box coordinates are valid */
	for (row = y1; row <= y2; row++)
		for (col = x1; col <= x2; ++col)
		{
			pixel_ptr = HW_OCRAM_BASE + (row << 10) + (col << 1);
			PHYSMEM_16(pixel_ptr) = pixel_color;		// set pixel color
		}
}

void VGA_line(int x1, int y1,int x2, int y2, short pixel_color, void *virtual_base)
{
	unsigned int pixel_ptr, col, row;
	
	int slope = (y2 - y1)/(x2 - x1);
	
	for(col = x1; col < x2; ++col)
	{
		row = col * slope;
		pixel_ptr = HW_OCRAM_BASE + (row << 10) + (col << 1);
		PHYSMEM_16(pixel_ptr) = pixel_color;
	}
	/*
 	for(col = x2; col >= x1; --col)
	{
		row = col * -slope;
		pixel_ptr = HW_OCRAM_BASE + (row << 10) + (col << 1);
		PHYSMEM_16(pixel_ptr) = 0x0000;
		for(secondTimer = 0; secondTimer < 2500000; ++secondTimer)
		{
		}
	} 
	*/


}

void VGA_text_clear(void *virtual_base)
{
	int x,y;
	int offset;
	char blank[1] = " ";
  	unsigned int  char_ptr;

	// Character coordinats are from 0,0 to 79,59 (x,y) position
	for(x = 0; x < 80; x++)
		for(y = 0; y < 60; y++)
		{
		/* assume that the text string fits on one line */
		offset = (y << 7) + x;
		char_ptr =  FPGA_CHAR_BASE + offset;
		PHYSMEM_32(char_ptr) = *blank;
		}
}









struct Bullet Create_Enemy_Bullet(struct Enemy enemy)
{
	struct Bullet temp;
	
	temp.x1 = enemy.x1 + 7;
	temp.x2 = enemy.x2 - 7;
	temp.y1 = enemy.y2;				
	temp.y2 = enemy.y2 + 5;
	
	temp.color = 0xFFFF;
	
	return temp;
}

struct Enemy Create_Enemy()
{	
	struct Enemy temp;
	temp.x1=rand() % (SPAWN_WIDTH_END + 1 - SPAWN_WIDTH_START) + SPAWN_WIDTH_START;
	temp.x2= temp.x1+15;
	temp.y1=rand() % (SPAWN_HEIGHT_END + 1 - SPAWN_HEIGHT_START) + SPAWN_HEIGHT_START;
	temp.y2= temp.y1+15;
	temp.color=rand()%NUM_COLORS;
	temp.alive = 1;
	temp.shootTimer = 15;
	temp.direc = rand() % 2;
	//temp.shoot = 0;
	
	return temp;
}

struct Enemy MoveEnemy(struct Enemy enemy)
{
	int updown = rand() % 2;
	
	if(enemy.direc == 0) //right
	{
		if(enemy.x2 <= 315)
		{
			enemy.x1++;
			enemy.x2++;
		}
		else if(updown == 0)
		{
			enemy.y1 += 2;
			enemy.y2 += 2;
			enemy.direc = 1;
		}
		else
		{
			enemy.y1 -= 2;
			enemy.y2 -= 2;
			enemy.direc = 1;
		}
	}
	else //left
	{
		if(enemy.x1 >= 5)
		{
		enemy.x1--;
		enemy.x2--;
		}
		else if(updown == 0)
		{
			enemy.y1 += 2;
			enemy.y2 += 2;
			enemy.direc = 0;
		}
		else
		{
			enemy.y1 -= 2;
			enemy.y2 -= 2;
			enemy.direc = 0;
		}
	}
	
	return enemy;
}

int Shoot()
{
	int temp = rand() % 2;
	return temp;
}














int main(int argc,char ** argv) {
	
    void *virtual_base;
    int fd;
	void *virtual_base_reg;
    int fd_reg;
	int i;
	int wincounter;
	
	int score = 0;

	int aliveEnemies = 0;
  	
   if( ( fd_reg = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
	    printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}
    
	virtual_base_reg = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd_reg, HW_REGS_BASE );
	if( virtual_base_reg == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd_reg );
		return( 1 );
	}
	
    // Set framebuffer addr to beginning of the SRAM
    PHYSMEM_32_reg(0xff203024) = 0xc8000000;
    PHYSMEM_32_reg(0xff203020) = 0xc8000000;
    
	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
	    printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}
	
    virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_OCRAM_BASE );
	if( virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd );
		return( 1 );
	}
	
	VGA_text_clear(virtual_base); // Clear the text
	VGA_box(0, 0, 500,500,0x0000, virtual_base); // Clear the screen
	
		
	/////////////////////////////////////////////////// BEGINNING SCREEN OF THE GAME //////////////////////////////////////////////////////////////////////////////
	VGA_text(6, 10, "Welcome to Game.EXE, a bullet hell game implemented on a DE1 board.", virtual_base); // Welcoming the Player
	VGA_text(4, 12, "Use the standard [W, S, A, D] to move and [period] to exit at any point.", virtual_base); // Welcoming the Player
	VGA_text(6, 14, "Enter number of enemies(1-50) on console and press enter to start....\n", virtual_base); // Welcoming the Player
	scanf("%d", &aliveEnemies); // Wait for number of enemies and enter to be pressed
	VGA_text_clear(virtual_base);
	
	struct Player Player1; 
	struct Enemy Enemy[50];
	
	wincounter = aliveEnemies;

	/////////////////////////////////////////////////// DECLARING THE PLAYERS STARTING POSITION //////////////////////////////////////////////////////////////////////////////
	
	Player1.x1 = 157;
	Player1.x2 = 162;
	Player1.y1 = 420;
	Player1.y2 = 430;
	
	struct Bullet bullet1;		
	bullet1.x1 = 0;
	bullet1.x2 = 0;
	bullet1.y1 = 0;				
	bullet1.y2 = 0;	

	int PlayerBullets = 0;
	
	if(aliveEnemies > 50)
	{
		aliveEnemies = 50;
	}
	
	for(i = 0; i < aliveEnemies; i++)
	{
		Enemy[i] = Create_Enemy();
	}

	char coderdy = PHYSMEM_16_reg(0xFF202f00);
	char scanCode = PHYSMEM_16_reg(0xFF201f00); 
	//char input;
	//use system call to make terminal send all keystrokes directly to stdin
	//system ("/bin/stty raw");
	int counterrr = 0;
	while(scanCode != 0xF)
	{
		usleep(2000);
		if((counterrr % 2) == 0)
		{
			coderdy = PHYSMEM_16_reg(0xFF202f00);
			scanCode = PHYSMEM_16_reg(0xFF201f00);
		}
		//printf("Code = %c\n", scanCode);
		
		for(i = 0; i < aliveEnemies; i++)
		{
			if(Enemy[i].alive){
				//printf("erasing enemy %d\n", i);
				draw_enemy(Enemy[i].x1, Enemy[i].y1, Enemy[i].x2, Enemy[i].y2, 0x0000, virtual_base);
			}
		}
		
		//putchar(input);
		draw_player(Player1.x1, Player1.y1, Player1.x2, Player1.y2, 0x0000, virtual_base);
		
		VGA_box(bullet1.x1, bullet1.y1, bullet1.x2, bullet1.y2, 0x0000, virtual_base);
		
		for(i = 0; i < aliveEnemies; i++)
		{
			if(Enemy[i].alive)
			Enemy[i] = MoveEnemy(Enemy[i]);
		}
		
		if(scanCode == 0x0B && coderdy == 1)
		{
			Player1.y1 -= 2;
			Player1.y2 -= 2;
			coderdy = 0;
		}
		else if(scanCode == 0x0A && coderdy == 1)
		{
			Player1.x1--;
			Player1.x2--;
			coderdy = 0;
		}
		else if(scanCode == 0x0C && coderdy == 1)
		{
			Player1.y1 += 2;
			Player1.y2 += 2;
			coderdy = 0;
		}
		else if(scanCode == 0x0D && coderdy == 1)
		{
			Player1.x1++;
			Player1.x2++;
			coderdy = 0;
		}
		else if(scanCode == 0x0E && coderdy == 1)
		{
			printf("alive enemies: %d\n", wincounter);
			if (PlayerBullets == 0)
			{	
				bullet1.x1 = Player1.x1 + 2;
				bullet1.x2 = Player1.x2 - 2;
				bullet1.y1 = Player1.y1 - 5 ;				
				bullet1.y2 = Player1.y1;
				
				PlayerBullets++;
			}
			else
			{
				bullet1.x1 = Player1.x1 + 2;
				bullet1.x2 = Player1.x2 - 2;
				bullet1.y1 = Player1.y1 - 5 ;				
				bullet1.y2 = Player1.y1;
			}
			coderdy = 0;		
		}
		
		if(PlayerBullets > 0)
		{
				bullet1.y1 -= 2;
				bullet1.y2 -= 2;
		}
		
		if (bullet1.y1 <= 0) 
		{
			PlayerBullets = 0;	
		}
		

		
		for(i = 0; i < aliveEnemies; i++){
			if(Enemy[i].alive){
				
				if(Enemy[i].hasBullet == 0){
				Enemy[i].shootTimer -= Shoot();
				if(Enemy[i].shootTimer <= 0)
				{
					Enemy[i].shoot = 1;
					Enemy[i].shootTimer = 50;
				}
				}
				
				if(bullet1.y1 >= Enemy[i].y1 && bullet1.y1 <= Enemy[i].y2 && bullet1.x2 >= Enemy[i].x1 && bullet1.x1 <= Enemy[i].x2)
				{
					printf("Bullet 1 hit Enemy %d\n", i);
					draw_enemy(Enemy[i].x1, Enemy[i].y1, Enemy[i].x2, Enemy[i].y2, 0x0000, virtual_base);
					Enemy[i].x1 = 0;
					Enemy[i].x2 = 0;
					Enemy[i].y1 = 0;
					Enemy[i].y2 = 0;
					Enemy[i].alive = 0;
					score += 100;
					wincounter--;
				}
					
			}
		}
		
		for(i = 0; i < aliveEnemies; i++)
		{
			if(Enemy[i].alive)
			{
				if(Enemy[i].shoot)
				{
					Enemy[i].shoot = 0;
					Enemy[i].bullet = Create_Enemy_Bullet(Enemy[i]);
					Enemy[i].hasBullet = 1;
				}
			}
		}
		
		for(i = 0; i < aliveEnemies; i++)
		{   usleep(1000);
			VGA_box(Enemy[i].bullet.x1, Enemy[i].bullet.y1, Enemy[i].bullet.x2, Enemy[i].bullet.y2, 0x0000, virtual_base);
			if(Enemy[i].hasBullet)
			{
				Enemy[i].bullet.y1 += 2;
				Enemy[i].bullet.y2 += 2;
				
			if(Enemy[i].bullet.y1 >= Player1.y1 && Enemy[i].bullet.y1 <= Player1.y2 && Enemy[i].bullet.x2 >= Player1.x1 && Enemy[i].bullet.x1 <= Player1.x2)
			{
				printf("Player died\n");
				VGA_text(36, 27, "GAME OVER", virtual_base);
				printf("Score: %d\n", score);
				draw_player(Player1.x1, Player1.y1, Player1.x2, Player1.y2, 0x0000, virtual_base);
				usleep(1000000);
				exit(1);
			}
			if(Enemy[i].hasBullet)
			{
				if(Enemy[i].bullet.y1 >= 480 || Enemy[i].bullet.y2 >= 480)
				{
					Enemy[i].hasBullet = 0;
					Enemy[i].bullet.y1 = 0;
					Enemy[i].bullet.x1 = 0;
					Enemy[i].bullet.y2 = 0;
					Enemy[i].bullet.x2 = 0;
				}
			}
			}
			VGA_box(Enemy[i].bullet.x1, Enemy[i].bullet.y1, Enemy[i].bullet.x2, Enemy[i].bullet.y2, 0xFFFF, virtual_base);
			usleep(500);
		}
			
		VGA_box(bullet1.x1, bullet1.y1, bullet1.x2, bullet1.y2, 0xFFFF, virtual_base);
		
		draw_player(Player1.x1, Player1.y1, Player1.x2, Player1.y2, 0xFFFF, virtual_base);
		
		for(i = 0; i < aliveEnemies; i++)
		{
			if(Enemy[i].alive){
				//printf("drawing enemy %d\n", i);
				draw_enemy(Enemy[i].x1, Enemy[i].y1, Enemy[i].x2, Enemy[i].y2, Enemy[i].color, virtual_base);
				usleep(1000);
			}
		}
		
		if(wincounter == 0)
		{
			VGA_text(36, 27, "VICTORY", virtual_base);
			printf("Score: %d\n", score);
			draw_player(Player1.x1, Player1.y1, Player1.x2, Player1.y2, 0x0000, virtual_base);
			usleep(1000000);
			exit(1);
		}
		counterrr++;
		if (counterrr >= 100){counterrr = 0;}
		
	}
	
	// system call to set terminal back to normal
	//system ("/bin/stty cooked");
	
	    // Unmap registers region, map onchip ram region
    if( munmap( virtual_base_reg, HW_REGS_SPAN ) != 0 ) {
		printf( "ERROR: munmap() failed...\n" );
		close( fd_reg );
		return( 1 );
	}
	
	if( munmap( virtual_base, HW_REGS_SPAN ) != 0 ) {
		printf( "ERROR: munmap() failed...\n" );
		close( fd );
		return( 1 );
	}

	close( fd );

	return( 0 );

}

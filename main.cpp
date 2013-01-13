#define NUM_ITEMS 5

#include <allegro.h>
#include <stdio.h>

void init();
void deinit();

BITMAP *items[NUM_ITEMS] = {NULL};
BITMAP *arrows[3] = {NULL};
BITMAP *buffer = NULL;
BITMAP *background = NULL;
BITMAP *truck = NULL;
BITMAP *gtruck = NULL;
BITMAP *machine = NULL;
BITMAP *brokenmachine = NULL;
BITMAP *cursor = NULL;
BITMAP *mcurs = NULL;
BITMAP *partimg[2] = {NULL};

MIDI *midi = NULL;

volatile long speed_counter = 0;

int machinelevel, machtarget;
int convey[4][13];
int broken;
int trucktrash;
int arrimg;
int trucky, truckx, gtruckx;
long score;
long tick;
int carried;
int chewing;

int level;

struct particle
{
       int type,x, y, life; struct particle *next, *prev;
};

particle *parts = NULL;

void increment_speed_counter() // A function to increment the speed counter
{
  speed_counter++; // This will just increment the speed counter by one. :)
}
END_OF_FUNCTION(increment_speed_counter); // Make sure you tell it that it's the end of the

void add_to_convey(int i, int item) // A function to increment the speed counter
{
     int j=0;
     while (convey[i][j] != 0 && j<13) j++;
     if (j < 13)
     convey[i][j] = item;
}
END_OF_FUNCTION(add_to_convey); // Make sure you tell it that it's the end of the

void chew(int i) // A function to increment the speed counter
{
     int j;

     if (convey[i][0] == chewing && !broken)
     {
         if (convey[i][0] == 1) broken = 1000;

         for (j = 0; j < 12; j++)
            convey[i][j] = convey[i][j+1];
         convey[i][12]=0;
     } else chewing = 0;
}
END_OF_FUNCTION(chew); // Make sure you tell it that it's the end of the

void get_from_convey(int i) // A function to increment the speed counter
{
     int j=12;
     while (convey[i][j] == 0 && j>=0) j--;
     if (j >= 0) {
           mcurs = items[convey[i][j]-1];
           carried = convey[i][j];
           convey[i][j] = 0;
     }
}
END_OF_FUNCTION(get_from_convey); // Make sure you tell it that it's the end of the

int main() {
	init();
	int i,j;
int mouse_clicked, last_button=0, current_button=0;
particle *tp, *tp2;


	while (!key[KEY_ESC]) {
      while(speed_counter > 0)
      {
   last_button = current_button;
   current_button = mouse_b & 1;
   mouse_clicked = (current_button && !last_button);
       if (mouse_clicked) {
          if (mouse_x < 272 && mouse_y < 143)
          {
              if (gtruckx == 0 && carried == 1)
              {
                  mcurs = cursor;
                  carried=0;
                  trucktrash++;
                  if (trucktrash > 2) gtruckx=-1;
              }
          } else if (mouse_x < 81 && mouse_y > 150 && mouse_y < 439) {
                 machtarget = 150+75*((int)(mouse_y - 150) / 75);
                 chewing = convey[(int)(mouse_y - 150) / 75][0];
          } else if (mouse_x > 80 && mouse_y > 141 && mouse_y < 439) {
                 if (carried == 0)
                   get_from_convey((int)(mouse_y-141)/75);
                 else
                 {
                   add_to_convey((int)(mouse_y-141)/75,carried);
                   carried=0;
                   mcurs=cursor;
                 }
          }
                   // CLICK!
       }
        rest(1);
        speed_counter --;
      } //This is the closing bracket for the (speed_counter > 0) test
      tick++;
      if (tick % 60000 == 0) level++;
      if (tick % (60*(20-level)) == 0) { trucky=480; truckx = 525; }
      if (trucky > 0) trucky--; if (trucky < 115 && truckx < 640) truckx++;
      if (gtruckx<0 && trucktrash > 0) gtruckx--;
      if (gtruckx<-1000) trucktrash=0;
      if (gtruckx<0 && trucktrash == 0) gtruckx++;

      if (broken > 0) broken--; else {
      if (machinelevel < machtarget) machinelevel++;
      if (machinelevel > machtarget) machinelevel--;
      if (machinelevel == machtarget) chew((int)(machinelevel - 150) / 75); }

      if (trucky == 150) add_to_convey(0,rand()%5+1);
      if (trucky == 225) add_to_convey(1,rand()%5+1);
      if (trucky == 300) add_to_convey(2,rand()%5+1);
      if (trucky == 375) add_to_convey(3,rand()%5+1);

      arrimg = (arrimg+1) % 180;

      if (!broken) {
      tp = (particle *)malloc(sizeof(particle));
      tp->next=parts;
      tp->prev = NULL;
      tp->type = 0;
      tp->y=machinelevel;
      tp->life = rand()%100;
      tp->x = 57;
      if (parts != NULL)
      parts->prev=tp;
      parts=tp;

      tp = (particle *)malloc(sizeof(particle));
      tp->next=parts;
      tp->prev = NULL;
      tp->type = 0;
      tp->y=machinelevel-6;
      tp->life = rand()%100;
      tp->x = 47;
      parts->prev=tp;
      parts=tp;
      } else {
             for (i=0; i<5; i++) {
      tp = (particle *)malloc(sizeof(particle));
      tp->next=parts;
      tp->prev = NULL;
      tp->type = rand() % 2;
      tp->y=machinelevel+44;
      tp->life = rand()%100;
      tp->x = 17+rand()%40;
      if (parts != NULL)
      parts->prev=tp;
      parts=tp; }
      }

    clear_bitmap(buffer);
    blit(background, buffer, 0,0,0,0,640,480);

    for (i=0; i<4; i++)
    for (j=0; j<13; j++)
    if (convey[i][j] != 0)
    draw_sprite(buffer,items[convey[i][j]-1],32*j+100,75*i+147);

    for (i=0; i<4; i++)
    draw_sprite(buffer,arrows[arrimg/60], 0,75*i+150);

    if (broken == 0) draw_sprite(buffer,machine, 0,machinelevel);
    else draw_sprite(buffer,brokenmachine, 0,machinelevel);

    draw_sprite(buffer, gtruck, gtruckx,0);
    for (i=0; i<trucktrash; i++)
        draw_sprite(buffer,items[0],32*i+240,0);

    draw_sprite(buffer, truck, truckx,trucky);

    while (tp != NULL)
    {
          tp2 = tp->next;
          tp->y--;
          tp->x += (rand()%3-1);
          draw_sprite(buffer, partimg[tp->type], tp->x,tp->y);

          tp->life--;
          if (tp->life <= 0)
          {
              if (tp->prev == NULL) parts = tp->next; else tp->prev->next = tp->next;
              if (tp->next != NULL) tp->next->prev = tp->prev;
              free(tp);
          }
          tp = tp2;
    }

    draw_sprite(buffer, mcurs, mouse_x,mouse_y);

    blit(buffer, screen, 0,0,0,0,640,480);

	}

	deinit();
	return 0;
}
END_OF_MAIN()

void reset_game() {
    int i;
	for (i=0; i<13; i++) {
	convey[0][i]=0;
	convey[1][i]=0;
	convey[2][i]=0;
	convey[3][i]=0;}

	level=0;
	score=0;
	broken=0;
	machinelevel=150;
	machtarget = 150;
	trucktrash=0;
	trucky=0;
	truckx=640;
	gtruckx=0;
	tick=0;
	carried = 0;
	chewing = 0;
	mcurs=cursor;
}

END_OF_FUNCTION(reset_game);

void init() {
	int depth, res, i;
	char path[80];

	allegro_init();
	install_timer();
	install_keyboard();
	install_mouse();
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);

    LOCK_VARIABLE(speed_counter); //Used to set the timer - which regulates the game's
    LOCK_FUNCTION(increment_speed_counter);//speed.

    install_int_ex(increment_speed_counter, BPS_TO_TIMER(60));//Set our BPS

	depth = desktop_color_depth();
	if (depth == 0) depth = 16;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}
    set_volume(255, 255); //digital and music to max levels

	/* add other initializations here */
	buffer = create_bitmap(640,480);
	background = load_bitmap("bg.bmp",NULL);
	truck = load_bitmap("truck.bmp",NULL);
	gtruck = load_bitmap("gtruck.bmp",NULL);
	machine = load_bitmap("machine.bmp",NULL);
	cursor = load_bitmap("cursor.bmp",NULL);
	brokenmachine = load_bitmap("broken.bmp",NULL);
	partimg[0] = load_bitmap("smoke.bmp",NULL);
	partimg[1] = load_bitmap("fire.bmp",NULL);

 parts = NULL;
	for (i=0; i<NUM_ITEMS; i++)
	{
       sprintf(path,"item%d.bmp",i);
	   items[i] = load_bitmap(path,NULL);
    }
	for (i=0; i<3; i++)
	{
       sprintf(path,"arrow%d.bmp",i);
	   arrows[i] = load_bitmap(path,NULL);
    }

    midi = load_midi("bg.mid");
    play_midi(midi, true);
	reset_game();
}

void deinit() {
     int i;
	clear_keybuf();
	/* add other deinitializations here */
	destroy_bitmap(buffer);
	destroy_bitmap(background);
	destroy_bitmap(truck);
	destroy_bitmap(gtruck);
	destroy_bitmap(machine);
	destroy_bitmap(brokenmachine);
	destroy_bitmap(cursor);
	destroy_bitmap(partimg[0]);
	destroy_bitmap(partimg[1]);
	for (i=0; i<NUM_ITEMS; i++)
 	    destroy_bitmap(items[i]);
	for (i=0; i<3; i++)
 	    destroy_bitmap(arrows[i]);
    destroy_midi(midi);
}

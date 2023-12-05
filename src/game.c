#include "game.h"
#include "keyboard.h"
#include "videocard.h"
#include "rtc.h"
#include "timer.h"
#include "mouse.c"

#include "imagens/normalCowboy.h"
#include "imagens/jumpingCowboy.h"
#include "imagens/nuvem.h"
#include "imagens/normalCowboyShooting.h"
#include "imagens/reverseCowboyShooting.h"
#include "imagens/Engine.h"
#include "imagens/Wheels.h"
#include "imagens/heart.h"
#include "imagens/arrow.h"
#include "imagens/reverseCowboy.h"
#include <minix/sysutil.h>
#include "imagens/cactus.h"
#include "imagens/menu.h"
#include "imagens/instructions.h"
#include <sys/types.h>
#include <unistd.h>
#include "imagens/player1wins.h"
#include "imagens/player2wins.h"
#include "imagens/clock1.h"
#include "imagens/clock2.h"
#include "imagens/clock3.h"
#include "imagens/clock4.h"
#include "imagens/clock5.h"
#include "imagens/clock6.h"
#include "imagens/clock7.h"
#include "imagens/clock8.h"
#include "imagens/clock9.h"
#include "imagens/clock10.h"
#include "imagens/clock11.h"
#include "imagens/clock12.h"

//loop flag
bool leave = false;

//mouse
struct packet pp;
uint8_t bytes[3];
extern uint8_t scancode_mouse;
bool first = true, second = false, third = false;


//vbe info
extern uint16_t x_res, y_res;
extern void* buffer_base;
extern uint8_t* video_mem;
static int cloudx = 0;


//counters for animations
static int shoot_image1_counter = 0;
static int shoot_image2_counter = 0;
bool shoot_image1;
bool shoot_image2;

//rtc
bool night = false;
int startHour;

//player creation positions
int initialx1 = 350;
int initialx2 = 720;
int initialy = 360;

struct Player p1;
struct Player p2;

struct Bullet b1;
struct Bullet b2;

struct Mouse mouse;

bool play = false;
bool instructions = false;






int create_menu(){
  int ipc_status, r;
  message msg;
  uint8_t bit_no_timer;
  uint8_t bit_no_mouse;

    mouse.x = 0;
    mouse.y = 0;



    int count = 0;

    int oldx=0;
    int oldy=0;
    int oldx2 = 0;
    int oldy2 = 0;


  vg_draw_xpm(Menu, 130, 0, video_mem);
  vg_draw_xpm(Menu, 130, 0, buffer_base);



  if(timer_subscribe_int(&bit_no_timer)!=0){
      printf("error subscribing");
      return 1;
  };
  if(mouse_subscribe_int(&bit_no_mouse)!=0){
      printf("error subscribing");
      return 1;
  };




  uint32_t timer_set = BIT(bit_no_timer);
  uint32_t mouse_set = BIT(bit_no_mouse);




  mouse_en_data_rep();


  while(!leave) { /* You may want to use a different condition */
        /* Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */

                    if ((msg.m_notify.interrupts & mouse_set)) {
                      mouse_ih();
                      if (first && (scancode_mouse & BIT(3))){
                        bytes[0] = scancode_mouse;
                        first = false;
                        second = true;
                      }
                      else if (second) {
                        bytes[1] = scancode_mouse;
                        third = true;
                        second = false;
                      }
                      else if (third) {
                        bytes[2] = scancode_mouse;
                        first = true;
                        third = false;
                        for(unsigned int i = 0; i < 3; i++)
                          pp.bytes[i] = bytes[i];


                        parse_packet(&pp);



                        oldx2 = oldx;
                        oldx = mouse.x;
                        mouse.x += pp.delta_x;


                        oldy2 = oldy;
                        oldy = mouse.y;
                        mouse.y -= pp.delta_y;

                        mouse.clicked = pp.lb;

                        mouseLimitVerifications();



                        uint8_t *ptr = buffer_base;

                        if(count%2==0){
                          for (uint8_t i = 0; i < 21; i++) {
                              memcpy(ptr + (oldy2 + i) * x_res + oldx2, video_mem + (oldy2 + i) * x_res + oldx2, 12);
                            }
                            vg_draw_xpm(arrow, mouse.x, mouse.y, buffer_base);

                            count = 0;
                        }
                        count++;



                      }
                    }

                  else if (msg.m_notify.interrupts & timer_set) {

                    menuButtons();

              }

                  else
                    continue;




                  break;
              default:
                break; /* no other notifications expected: do nothing */
          }
      }

        else {
        }
  }

  mouse_dis_data_rep();

  if(timer_unsubscribe_int()!=0){
    printf("error unsubscribing");
    return 1;
  }
  if(mouse_unsubscribe_int()!=0){
    printf("error unsubscribing\n");
    return 1;
  }

  if(play)
    start_game();




  return 0;
}

int start_game() {


  date_struct date = get_date();
  if(date.hours > 19 || date.hours < 6){
    night = true;
  } else {
    night = false;
  }

  startHour = treatHour(date.hours);

  setup();

  int ipc_status, r;
  message msg;
  uint8_t bit_no_kbc;
  uint8_t bit_no_timer;
  //uint8_t bit_no_mouse;
  if(kbc_subscribe_int(&bit_no_kbc)!=0){
      printf("error subscribing");
      return 1;
  };
  if(timer_subscribe_int(&bit_no_timer)!=0){
      printf("error subscribing");
      return 1;
  };



  uint32_t kbc_set = BIT(bit_no_kbc);
  uint32_t timer_set = BIT(bit_no_timer);

  while(scancode!=ESC_BC && p1.alive && p2.alive) { /* You may want to use a different condition */
        display();
        /* Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */
                    if (msg.m_notify.interrupts & kbc_set) { /* subscribed interrupt */

                        kbc_ih();
                        scancode_handler();
                        //player 1
                        if(scancode==D_MC) {
                          p1.right = true;
                        }
                        if(scancode==D_BC) {
                          p1.right = false;
                        }
                        if(scancode==A_MC) {
                          p1.left = true;
                        }
                        if(scancode==A_BC) {
                          p1.left = false;
                        }
                        if(scancode==W_MC) {
                          if(!p1.falling){
                            p1.jumping = true;
                          }
                        }
                        if(scancode==E_MC){
                          shoot_image1 = true;
                          p1.shooting = true;
                        }




                        //player 2
                        if(scancode==K_MC) {
                          p2.left = true;
                        }
                        if(scancode==K_BC) {
                          p2.left = false;
                        }
                        if(scancode==Ç_MC) {
                          p2.right = true;
                        }
                        if(scancode==Ç_BC) {
                          p2.right = false;
                        }
                        if(scancode==O_MC) {
                          if(!p2.falling){
                            p2.jumping = true;
                          }
                        }
                        if(scancode==I_MC){
                          shoot_image2 = true;
                          p2.shooting = true;
                        }

                    }


                    else if (msg.m_notify.interrupts & timer_set) {

                      //player 1
                      p1Verifications();
                      //player 2
                      p2Verifications();

                      //others
                        //clouds
                      cloudVerifications();

                      //shooting
                      shootingVerifications();


                    }

                    break;
                default:
                  break; /* no other notifications expected: do nothing */
            }
        }

        else {

        }
  }

  showWinner();


  if(kbc_unsubscribe_int()!=0){
      printf("error unsubscribing");
      return 1;

  }
  if(timer_unsubscribe_int()!=0){
    printf("error unsubscribing");
    return 1;
  }


  return 0;
}

void display(){

  if(night){
    vg_draw_rectangle_proj(295, 109, 1024-300, 290, 17, video_mem); //direita da cabine
    vg_draw_rectangle_proj(0, 0, 1024, 145, 17, video_mem); //parte das nuvens
  } else {
    vg_draw_rectangle_proj(295, 109, 1024-300, 290, 11, video_mem); //direita da cabine
    vg_draw_rectangle_proj(0, 0, 1024, 145, 11, video_mem); //parte das nuvens
  }

  vg_draw_rectangle_proj(0, 435, 1024, 300, 2, video_mem); //relva
  draw_hearts();
  if(b1.visible){
    vg_draw_rectangle_proj(b1.x, b1.y, 6, 3,4, video_mem);
  }
  if(b2.visible){
    vg_draw_rectangle_proj(b2.x, b2.y, 6, 3,4, video_mem);
  }




  if(shoot_image1){
    vg_draw_xpm(normalCowboyShooting, p1.x-3,p1.y, video_mem);
  }
  else{
    vg_draw_xpm(normalCowboy, p1.x,p1.y, video_mem);
  }
  if(shoot_image2){
    vg_draw_xpm(reverseCowboyShooting, p2.x-3, p2.y, video_mem);
  }
  else{
    vg_draw_xpm(reverseCowboy,p2.x, p2.y, video_mem);
  }



  vg_draw_xpm(nuvem, (200+cloudx)%1024, 100, video_mem);
  vg_draw_xpm(nuvem, (180+cloudx)%1024, 12, video_mem);
  vg_draw_xpm(nuvem, (380+cloudx)%1024, 75, video_mem);
  vg_draw_xpm(nuvem, (700+cloudx)%1024, 52, video_mem);
  vg_draw_xpm(nuvem, (500+cloudx)%1024, 04, video_mem);
  vg_draw_xpm(nuvem, (900+cloudx)%1024, 23, video_mem);
  vg_draw_xpm(nuvem, (40+cloudx)%1024, 57, video_mem);

  memcpy(buffer_base, video_mem, x_res*y_res);

}

void draw_hearts(){
  int initialx = 80;
  for(int i=0;i<p1.lives;i++){
    vg_draw_xpm(Heart, initialx + i*40, 500, video_mem);
  }
  for(int i=0;i<p2.lives;i++){
    vg_draw_xpm(Heart, 950-(initialx + i*40), 500, video_mem);
  }

}

void setup(){
  shoot_image1 = false;

  if(night){
    vg_draw_rectangle_proj(0, 109, 300, 330, 17, video_mem);
    vg_draw_rectangle_proj(300, 409, 1024-300, 728-390, 17, video_mem);
    vg_draw_rectangle_proj(835, 399, 180, 10, 17, video_mem); //direita da plataforma
  } else {
    vg_draw_rectangle_proj(0, 109, 300, 330, 11, video_mem);
    vg_draw_rectangle_proj(300, 409, 1024-300, 728-390, 11, video_mem);
    vg_draw_rectangle_proj(835, 399, 180, 10, 11, video_mem); //direita da plataforma
  }

  vg_draw_xpm(Engine, 0, 233, video_mem);
  vg_draw_xpm(Wheels, 300, 390, video_mem);
  vg_draw_xpm(Wheels, 700, 390, video_mem);
  vg_draw_rectangle_proj(0, 430, 1024, 5, 0, video_mem); //carril

  draw_clock();

  p1.lives = p2.lives = 3;
  p1.alive = p2.alive = true;
  p1.left = p2.left = false;
  p1.right = p2.right = false;
  p1.jumping = p2.jumping = false;
  p1.falling = p2.falling = false;
  p1.shooting = p2.shooting = false;
  p1.x = initialx1; p1.y = initialy;
  p2.x = initialx2; p2.y = initialy;


  b1.x = b1.y = b2.x = b2.y = -1;
  b1.visible = false;
  b2.visible = false;


}

void p1Verifications(){
  if(p1.right && p1.x<x_res-225){
    p1.x+=5;
  }
  if(p1.left && p1.x>300){
    p1.x-=5;
  }
  if(p1.jumping){
    if(p1.y <= (initialy - 95)){
      p1.jumping = false;
      p1.falling = true;
    }
    else{
      p1.y-=7;
    }
  }
  if(p1.falling){
    if(p1.y == initialy){
      p1.falling = false;
    }
    else{
      p1.y+=7;
    }
  }
  if(p1.shooting){
    if(!b1.visible){
      b1.x = p1.x +3;
      b1.y = p1.y +25;
      b1.visible = true;
    }
    p1.shooting = false;
  }
}

void p2Verifications(){
  if(p2.right && p2.x<x_res-225 ){
    p2.x+=5;
  }
  if(p2.left && p2.x>300){
    p2.x-=5;
  }

  if(p2.jumping){
    if(p2.y <= (initialy - 95)){
      p2.jumping = false;
      p2.falling = true;
    }
    else{
      p2.y-=7;
    }
  }
  if(p2.falling){
    if(p2.y == initialy){
      p2.falling = false;
    }
    else{
      p2.y+=7;
    }
  }
  if(p2.shooting){
    if(!b2.visible){
      b2.x = p2.x +3;
      b2.y = p2.y +25;
      b2.visible = true;
    }
    p2.shooting = false;
  }
}

void shootingVerifications(){
  if(shoot_image1_counter == 50){
    shoot_image1_counter = 0;
    shoot_image1 = false;
  }
  else{
    shoot_image1_counter++;
  }
  if(shoot_image2_counter == 50){
    shoot_image2_counter = 0;
    shoot_image2 = false;
  }
  else{
    shoot_image2_counter++;
  }

    //bullet
  if(b1.x < x_res && b1.visible){
    b1.x+=9;
  }
  if(b2.x < x_res && b2.visible){
    b2.x-=9;
  }
  if(b1.x>=x_res-200){
    b1.visible = false;
  }
  if(b2.x<=300){
    b2.visible = false;
  }
  if(b1.x>=p2.x && b1.x<=p2.x+31 && b1.visible){
    if(b1.y >= p2.y && b1.y<=p2.y+39){
      b1.visible = false;
      p2.lives--;
    }
  }
  if(b2.x>=p1.x && b2.x<=p1.x+31 && b2.visible){
    if(b2.y >= p1.y && b2.y<=p1.y+39){
      b2.visible = false;
      p1.lives--;
    }
  }
  if(p1.lives == 0){
    p1.alive = false;
  }
  if(p2.lives ==0){
    p2.alive = false;
  }
}

void cloudVerifications(){
  if(cloudx>1024){
    cloudx=0;
  }
  else{
    cloudx+=4;
  }
}

void draw_clock(){

  switch(startHour){
    case 1:
      vg_draw_xpm(clock1, 150, 320, video_mem);
      break;
    case 2:
      vg_draw_xpm(clock2, 150, 320, video_mem);
      break;
    case 3:
      vg_draw_xpm(clock3, 150, 320, video_mem);
      break;
    case 4:
      vg_draw_xpm(clock4, 150, 320, video_mem);
      break;
    case 5:
      vg_draw_xpm(clock5, 150, 320, video_mem);
      break;
    case 6:
      vg_draw_xpm(clock6, 150, 320, video_mem);
      break;
    case 7:
      vg_draw_xpm(clock7, 150, 320, video_mem);
      break;
    case 8:
      vg_draw_xpm(clock8, 150, 320, video_mem);
      break;
    case 9:
      vg_draw_xpm(clock9, 150, 320, video_mem);
      break;
    case 10:
      vg_draw_xpm(clock10, 150, 320, video_mem);
      break;
    case 11:
      vg_draw_xpm(clock11, 150, 320, video_mem);
      break;
    case 12:
      vg_draw_xpm(clock12, 150, 320, video_mem);
      break;
    default:
      break;
  }
}

void showWinner(){
  if(!p1.alive){
    vg_draw_rectangle_proj(0,0,1024,768,0,buffer_base);
    vg_draw_xpm(player2wins, 213, 270, buffer_base);

    sleep(15);

  }
  else if(!p2.alive){
    vg_draw_rectangle_proj(0,0,1024,768,0,buffer_base);
    vg_draw_xpm(player1wins, 213, 270, buffer_base);
    sleep(5);
  }
}

void menuButtons(){
  if(mouse.clicked && mouse.y>365 && mouse.x>360 && mouse.y<422 && mouse.x<615 && !instructions){
    //play
    leave = true;
    mouse.clicked = 0;
    vg_draw_rectangle_proj(0,0,1024,768,0,video_mem);
    play = true;
  }

  if(mouse.clicked && mouse.y>442 && mouse.x>360 && mouse.y<498 && mouse.x<615 && !instructions){
    //instructions
    instructions = true;
    mouse.clicked = 0;
    vg_draw_rectangle_proj(0,0,1024,768,0,video_mem);
    vg_draw_xpm(Instructions, 130, 0, video_mem);
    vg_draw_xpm(Instructions, 130, 0, buffer_base);


  }
  if(mouse.clicked && mouse.y>518 && mouse.x>360 && mouse.y<574 && mouse.x<615 && !instructions){
    //exit
    leave = true;
  }
  if(mouse.clicked && mouse.y>510 && mouse.x>360 && mouse.y<556 && mouse.x<615 && instructions){

    instructions = false;
    vg_draw_rectangle_proj(0,0,1024,768,0,buffer_base);
    vg_draw_xpm(Menu, 130, 0, video_mem);
    vg_draw_xpm(Menu, 130, 0, buffer_base);

  }
}

void mouseLimitVerifications(){
  if(mouse.x>=1024){
    mouse.x = 1024;
  }
  if(mouse.x<=0){
    mouse.x = 0;
  }
  if(mouse.y<=0){
    mouse.y = 0;
  }
  if(mouse.y>=599){
    mouse.y=599;
  }
}

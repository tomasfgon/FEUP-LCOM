#ifndef _LCOM_GAME_H_
#define _LCOM_GAME_H_
#include <lcom/lcf.h>
#include <stdint.h>

#include "i8042.h"
#include "i8254.h"

/** @defgroup Game Game.h
 * @{ Game.h
 */



/**
    * @brief Responsable for the functioning and management of the game. Handles the interruptions for the kbc and timer and calls other appropriate functions.
    * @return Returns 0 on success and 1 otherwise
    */
int start_game();



/**
    * @brief Holds player information.
    */
struct Player{
  int x; /**< x Horizontal Position */
  int y; /**< y Vertical Position */
  int lives; /**< number of players lives. */
  bool alive; /**< flag that indicates if player is still alive. */
  bool left; /**< flag that indicates if player is moving left. */
  bool right; /**< flag that indicates if player is moving right. */
  bool jumping; /**< flag that indicates if player is jumping. */
  bool falling; /**< flag that indicates if player is falling. */
  bool shooting; /**< flag that indicates if player is shooting. */
};

/**
    * @brief Holds bullet information.
    */
struct Bullet{
  bool visible; /**< flag that indicates if bullet is still visible. */
  int x; /**< indicates bullet Horizontal position. */
  int y; /**< indicates bullet Vertical position. */
};

/**
    * @brief Holds mouse information.
    */
struct Mouse{
  bool clicked; /**< flag that indicates if mouse lb was pressed. */
  int x; /**< holds mouse pointer Horizontal position. */
  int y; /**< holds mouse pointer Vertical position. */
};

/**
    * @brief sets up initial values.
    */
void setup();


/**
    * @brief Prints and manages menu content as well as timer and mouse interruptions
    */
int create_menu();

/**
    * @brief Prints xpm elements during the game.
    * @Returns 0 on success and 1 otherwise
    */
void display();
/**
    * @brief Prints number of player lives
    */
void draw_hearts();
/**
    * @brief updates player1 coordinates.
    */
void p1Verifications();

/**
    * @brief updates player2 coordinates.
    */
void p2Verifications();
/**
    * @brief updates bullet coordinates.
    */
void shootingVerifications();
/**
    * @brief updates clouds coordinates.
    */
void cloudVerifications();
/**
    * @brief draws clock with RTC.
    */
void draw_clock();
/**
    * @brief shows game winner.
    */
void showWinner();
/**
    * @brief verifies menu button selection.
    */
void menuButtons();
/**
    * @brief verifies that mouse coordinates doesn't exit screen.
    */
void mouseLimitVerifications();


/** @} end of Game */

#endif /*_LCOM_GAME_H_*/

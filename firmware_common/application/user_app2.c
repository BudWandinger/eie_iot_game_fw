/**********************************************************************************************************************
File: user_app2.c                                                                

Description:
This is a user_app2.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp2Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp2RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp2"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp2Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp2_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp2_StateMachine;            /* The state machine function pointer */
//static u32 UserApp2_u32Timeout;                      /* Timeout counter used across states */

u8 UserApp2_NumPlayers = 1;
u8 UserApp2_Scores[4] = {0, 0, 0, 0};

PixelAddressType UserApp2_HeaderLocation;
PixelAddressType UserApp2_P1Location;
PixelAddressType UserApp2_P2Location;
PixelAddressType UserApp2_P3Location;
PixelAddressType UserApp2_P4Location;

u8 UserApp2_Score_string[] = "Scores";
u8 UserApp2_P1Score_string[] = "P1 0 ";
u8 UserApp2_P2Score_string[] = "P2 0 ";
u8 UserApp2_P3Score_string[] = "P3 0 ";
u8 UserApp2_P4Score_string[] = "P4 0 ";


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

// Takes in a player number as 1, 2, 3, 4 (this can be changed to fit the rest
// of the codebase
void SetScore(int player, int score) {
  UserApp2_Scores[player - 1] = score;
}

void SetNumberOfPlayers(u8 players) {
  UserApp2_NumPlayers = players;
}

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp2Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp2Initialize(void)
{
  u16 left_column, right_column;
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    // Place the Score text on the right hand side of the screen
    left_column = LCD_CENTER_COLUMN + (LCD_SMALL_FONT_COLUMNS + LCD_SMALL_FONT_SPACE);
    right_column = left_column + (LCD_SMALL_FONT_COLUMNS + LCD_SMALL_FONT_SPACE) * 4;
    
    UserApp2_HeaderLocation.u16PixelColumnAddress = left_column;
    UserApp2_P1Location.u16PixelColumnAddress = left_column;
    UserApp2_P2Location.u16PixelColumnAddress = right_column;
    UserApp2_P3Location.u16PixelColumnAddress = left_column;
    UserApp2_P4Location.u16PixelColumnAddress = right_column;
    
    UserApp2_HeaderLocation.u16PixelRowAddress = LCD_SMALL_FONT_LINE1;
    UserApp2_P1Location.u16PixelRowAddress = LCD_SMALL_FONT_LINE3;
    UserApp2_P2Location.u16PixelRowAddress = LCD_SMALL_FONT_LINE4;
    UserApp2_P3Location.u16PixelRowAddress = LCD_SMALL_FONT_LINE5;
    UserApp2_P4Location.u16PixelRowAddress = LCD_SMALL_FONT_LINE6;
    
    UserApp2_StateMachine = UserApp2SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp2_StateMachine = UserApp2SM_FailedInit;
  }

} /* end UserApp2Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp2RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp2RunActiveState(void)
{
  UserApp2_StateMachine();

} /* end UserApp2RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
void SetScoreString(u8 player) {
  u8 *scoreString;
  u8 score;
  switch (player) {
    case 1:
      scoreString = UserApp2_P1Score_string;
      break;
    case 2:
      scoreString = UserApp2_P2Score_string;
      break;
    case 3:
      scoreString = UserApp2_P3Score_string;
      break;
    case 4:
      scoreString = UserApp2_P4Score_string;
      break;
  }
  
  score = UserApp2_Scores[player - 1];
  if (score > 9) {
    scoreString[4] = (score / 10) + 48;
    scoreString[3] = (score % 10) + 48;
  }
  else {
    scoreString[3] = score + 48;
  }
}

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp2SM_Idle(void)
{
  SetScoreString(1);
  SetScoreString(2);
  SetScoreString(3);
  SetScoreString(4);
  
  LcdLoadString(UserApp2_Score_string, LCD_FONT_SMALL, &UserApp2_HeaderLocation);
  
  // One player, I imagine this will be like a waiting screen
  if (UserApp2_NumPlayers > 0)
    LcdLoadString(UserApp2_P1Score_string, LCD_FONT_SMALL, &UserApp2_P1Location);
  if (UserApp2_NumPlayers > 1)
    LcdLoadString(UserApp2_P2Score_string, LCD_FONT_SMALL, &UserApp2_P2Location);
  if (UserApp2_NumPlayers > 2)
    LcdLoadString(UserApp2_P3Score_string, LCD_FONT_SMALL, &UserApp2_P3Location);
  if (UserApp2_NumPlayers > 3)
    LcdLoadString(UserApp2_P4Score_string, LCD_FONT_SMALL, &UserApp2_P4Location);
} /* end UserApp2SM_Idle() */
     
#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp2SM_Error(void)          
{
  
} /* end UserApp2SM_Error() */
#endif


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserApp2SM_FailedInit(void)          
{
    
} /* end UserApp2SM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

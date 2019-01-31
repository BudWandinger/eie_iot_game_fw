/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static CapTouchMovementState horizontalMoveState;
static CapTouchMovementState verticalMoveState;
static GameboardErrorType err;
static u32 u32Timer;
static CursorType cursor;
static UserPlayState userPlayState;
static Gameboard gameboard;

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/
GameboardErrorType setDot(GameboardCoordinateType* coordinate_)
{
  return drawDot(coordinate_, DRAW_SET);
}

GameboardErrorType clearDot(GameboardCoordinateType* coordinate_)
{
  return drawDot(coordinate_, DRAW_CLEAR);
}

GameboardErrorType setVerticalLine(GameboardCoordinateType* coordinate_)
{
  return drawVerticalLine(coordinate_, DRAW_SET);
}

GameboardErrorType clearVerticalLine(GameboardCoordinateType* coordinate_)
{
  return drawVerticalLine(coordinate_, DRAW_CLEAR);
}

GameboardErrorType setHorizontalLine(GameboardCoordinateType* coordinate_)
{
  return drawHorizontalLine(coordinate_, DRAW_SET);
}

GameboardErrorType clearHorizontalLine(GameboardCoordinateType* coordinate_)
{
  return drawHorizontalLine(coordinate_, DRAW_CLEAR);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
 
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
    
    /* Init Gameboard */
    LcdClearScreen();
    drawEmptyGameboard();
    
    /* Init captouch */
    CapTouchOn();
    
    /* Init cursor */
    cursor.cursorLocation.u8RowCoordinate = 0;
    cursor.cursorLocation.u8ColumnCoordinate = 0;
    cursor.cursorState = CURSOR_OFF;
    
    /* Init cursor state machine */
    horizontalMoveState = MOVEMENT_NOT_STARTED;
    verticalMoveState = MOVEMENT_NOT_STARTED;
    
    /* Init user play state machine. */
    userPlayState = SELECTED_NOTHING;
    
    /* Init gameboard */
    gameboard.pu8HorizontalLines = (u8*)malloc(sizeof(u8) * (GAMEBOARD_SIZE - 1));
    gameboard.pu8VerticalLines = (u8*)malloc(sizeof(u8) * (GAMEBOARD_SIZE - 1));
    for(uint8_t i = 0; i < GAMEBOARD_SIZE - 1; i++)
    {
      *(gameboard.pu8HorizontalLines + i) = 0;
      *(gameboard.pu8VerticalLines + i) = 0;
    }
    
    /* Init app timer */
    u32Timer = 0;
    
    // Test Scoring
    SetNumberOfPlayers(3);
    SetScore(2, 43);
    SetScore(3, 1);
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
static GameboardErrorType drawDot(GameboardCoordinateType* coordinate_, DrawType drawType_)
{
  if(coordinate_->u8ColumnCoordinate >= GAMEBOARD_SIZE)
  {
    return BOUNDARY_ERROR_DOT_X;
  }
  else if(coordinate_->u8RowCoordinate >= GAMEBOARD_SIZE)
  {
    return BOUNDARY_ERROR_DOT_Y;
  }
  
  u8 i, j;
  for(i = 0; i < GAMEBOARD_DOT_WIDTH; i++)
  {
    for(j = 0; j < GAMEBOARD_DOT_WIDTH; j++)
    {
      PixelAddressType pixel =
      {
        .u16PixelRowAddress = GAMEBOARD_BORDER_DOT_OFFSET + (coordinate_->u8RowCoordinate * GAMEBOARD_DOT_OFFSET) + i,
        .u16PixelColumnAddress = GAMEBOARD_BORDER_DOT_OFFSET + (coordinate_->u8ColumnCoordinate * GAMEBOARD_DOT_OFFSET) + j
      };
      
      if(drawType_ == DRAW_SET)
      {
        LcdSetPixel(&pixel);
      }
      else if(drawType_ == DRAW_CLEAR)
      {
        LcdClearPixel(&pixel);
      }
    }
  }
  
  PixelBlockType updateArea =
  {
    .u16RowStart = GAMEBOARD_BORDER_DOT_OFFSET + (coordinate_->u8RowCoordinate * GAMEBOARD_DOT_OFFSET),
    .u16ColumnStart = GAMEBOARD_BORDER_DOT_OFFSET + (coordinate_->u8ColumnCoordinate * GAMEBOARD_DOT_OFFSET),
    .u16RowSize = GAMEBOARD_DOT_WIDTH,
    .u16ColumnSize = GAMEBOARD_DOT_WIDTH
  };
  
  LcdUpdateScreenRefreshArea(&updateArea);
  
  return GAMEBOARD_SUCCESS;
}

static GameboardErrorType drawVerticalLine(GameboardCoordinateType* coordinate_, DrawType drawType_)
{
  if(coordinate_->u8ColumnCoordinate >= GAMEBOARD_SIZE)
  {
    return BOUNDARY_ERROR_VLINE_X;
  }
  else if(coordinate_->u8RowCoordinate >= GAMEBOARD_SIZE - 1)
  {
    return BOUNDARY_ERROR_VLINE_Y;
  }
  
  u8 i, j;
  for(i = 0; i < GAMEBOARD_LINE_LENGTH; i++)
  {
    for(j = 0; j < GAMEBOARD_LINE_WIDTH; j++)
    {
      PixelAddressType pixel =
      {
        .u16PixelRowAddress = GAMEBOARD_BORDER_LINE_LENGTH_OFFSET + (coordinate_->u8RowCoordinate * GAMEBOARD_LINE_OFFSET) + i,
        .u16PixelColumnAddress = GAMEBOARD_BORDER_LINE_WIDTH_OFFSET + (coordinate_->u8ColumnCoordinate * GAMEBOARD_LINE_OFFSET) + j
      };
      
      if(drawType_ == DRAW_SET)
      {
        LcdSetPixel(&pixel);
      }
      else if(drawType_ == DRAW_CLEAR)
      {
        LcdClearPixel(&pixel);
      }
    }
  }
  
  PixelBlockType updateArea =
  {
    .u16RowStart = GAMEBOARD_BORDER_LINE_LENGTH_OFFSET + (coordinate_->u8RowCoordinate * GAMEBOARD_LINE_OFFSET),
    .u16ColumnStart = GAMEBOARD_BORDER_LINE_WIDTH_OFFSET + (coordinate_->u8ColumnCoordinate * GAMEBOARD_LINE_OFFSET),
    .u16RowSize = GAMEBOARD_LINE_LENGTH,
    .u16ColumnSize = GAMEBOARD_LINE_WIDTH
  };
  
  LcdUpdateScreenRefreshArea(&updateArea);
  
  return GAMEBOARD_SUCCESS;
}

static GameboardErrorType drawHorizontalLine(GameboardCoordinateType* coordinate_, DrawType drawType_)
{
  if(coordinate_->u8ColumnCoordinate >= GAMEBOARD_SIZE - 1)
  {
    return BOUNDARY_ERROR_HLINE_X;
  }
  else if(coordinate_->u8RowCoordinate >= GAMEBOARD_SIZE)
  {
    return BOUNDARY_ERROR_HLINE_Y;
  }
  
  u8 i, j;
  for(i = 0; i < GAMEBOARD_LINE_WIDTH; i++)
  {
    for(j = 0; j < GAMEBOARD_LINE_LENGTH; j++)
    {
      PixelAddressType pixel =
      {
        .u16PixelRowAddress = GAMEBOARD_BORDER_LINE_WIDTH_OFFSET + (coordinate_->u8RowCoordinate * GAMEBOARD_LINE_OFFSET) + i,
        .u16PixelColumnAddress = GAMEBOARD_BORDER_LINE_LENGTH_OFFSET + (coordinate_->u8ColumnCoordinate * GAMEBOARD_LINE_OFFSET) + j
      };
      
      if(drawType_ == DRAW_SET)
      {
        LcdSetPixel(&pixel);
      }
      else if(drawType_ == DRAW_CLEAR)
      {
        LcdClearPixel(&pixel);
      }
    }
  }
  
  PixelBlockType updateArea =
  {
    .u16RowStart = GAMEBOARD_BORDER_LINE_WIDTH_OFFSET + (coordinate_->u8RowCoordinate * GAMEBOARD_LINE_OFFSET),
    .u16ColumnStart = GAMEBOARD_BORDER_LINE_LENGTH_OFFSET + (coordinate_->u8ColumnCoordinate * GAMEBOARD_LINE_OFFSET),
    .u16RowSize = GAMEBOARD_LINE_WIDTH,
    .u16ColumnSize = GAMEBOARD_LINE_LENGTH
  };
  
  LcdUpdateScreenRefreshArea(&updateArea);
  
  return GAMEBOARD_SUCCESS;
}

static GameboardErrorType drawEmptyGameboard(void)
{
  u8 i, j;
  for(i = 0; i < GAMEBOARD_SIZE; i++)
  {
    for(j = 0; j < GAMEBOARD_SIZE; j++)
    {
      GameboardCoordinateType coordinate =
      {
        .u8RowCoordinate = i,
        .u8ColumnCoordinate = j
      };
      setDot(&coordinate);
    }
  }
  
  return GAMEBOARD_SUCCESS;
}

static GameboardErrorType testGameboardDrawingFunctions(void)
{
    static u8 i = 0;
    static u8 j = 0;
    static DrawingTestState state = SETTING_DOTS;
    GameboardErrorType err;
    GameboardCoordinateType coord;
    
    switch(state)
    {
      case SETTING_DOTS:
        coord.u8RowCoordinate = i;
        coord.u8ColumnCoordinate = j;
        err = setDot(&coord);
        if(err != GAMEBOARD_SUCCESS)
        {
          return err;
        }
        if(j < 7)
        {
          j++;
        }
        else if(i < 7)
        {
          i++;
          j = 0;
        }
        else
        {
          i = 0;
          j = 0;
          state = SETTING_VLINES;
        }
        break;
        
      case SETTING_VLINES:
        coord.u8RowCoordinate = i;
        coord.u8ColumnCoordinate = j;
        err = setVerticalLine(&coord);
        if(err != GAMEBOARD_SUCCESS)
        {
          return err;
        }
        if(j < 7)
        {
          j++;
        }
        else if(i < 6)
        {
          i++;
          j = 0;
        }
        else
        {
          i = 0; 
          j = 0; 
          state = SETTING_HLINES;
        }
        break;
        
      case SETTING_HLINES:
        coord.u8RowCoordinate = i;
        coord.u8ColumnCoordinate = j;
        err = setHorizontalLine(&coord);
        if(err != GAMEBOARD_SUCCESS)
        {
          return err;
        }
        if(j < 6)
        {
          j++;
        }
        else if( i < 7)
        {
          i++;
          j = 0;
        }
        else
        {
          state = CLEARING_HLINES;
        }
        break;
        
    case CLEARING_HLINES:
      coord.u8RowCoordinate = i;
      coord.u8ColumnCoordinate = j;
      err = clearHorizontalLine(&coord);
      if(err != GAMEBOARD_SUCCESS)
      {
        return err;
      }
      if(j != 0)
      {
        j--;
      }
      else if(i != 0)
      {
        i--;
        j = 6;
      }
      else
      {
        i = 6;
        j = 7;
        state = CLEARING_VLINES;
      }
      break;
      
    case CLEARING_VLINES:
      coord.u8RowCoordinate = i;
      coord.u8ColumnCoordinate = j;
      err = clearVerticalLine(&coord);
      if(err != GAMEBOARD_SUCCESS)
      {
        return err;
      }
      if(j != 0)
      {
        j--;
      }
      else if(i != 0)
      {
        i--;
        j = 7;
      }
      else
      {
        i = 7;
        j = 7;
        state = CLEARING_DOTS;
      }
      break;
      
    case CLEARING_DOTS:
      coord.u8RowCoordinate = i;
      coord.u8ColumnCoordinate = j;
      err = clearDot(&coord);
      if(err != GAMEBOARD_SUCCESS)
      {
        return err;
      }
      if(j != 0)
      {
        j--;
      }
      else if(i != 0)
      {
        i--;
        j = 7;
      }
      else
      {
        state = DRAWING_COMPLETE;
      }
    }

    return GAMEBOARD_SUCCESS;
}

static GameboardErrorType blinkCursor(void)
{
  if(u32Timer % 250 == 0)
  {
    switch(cursor.cursorState)
    {
      case CURSOR_OFF:
        err = setDot(&cursor.cursorLocation);
        if(err != GAMEBOARD_SUCCESS)
        {
          return err;
        }
        cursor.cursorState = CURSOR_ON;
        break;
        
      case CURSOR_ON:
        err = clearDot(&cursor.cursorLocation);
        if(err != GAMEBOARD_SUCCESS)
        {
          return err;
        }
        cursor.cursorState = CURSOR_OFF;
        break;
    }
  }
  
  return GAMEBOARD_SUCCESS;
}

static GameboardErrorType moveCursor(void)
{
  u8 horizontalSliderPos = CaptouchCurrentHSlidePosition();
  u8 verticalSliderPos = CaptouchCurrentVSlidePosition();
  
  switch(horizontalMoveState)
  {
    case MOVEMENT_NOT_STARTED:
      if(horizontalSliderPos >= CAPTOUCH_HORIZONTAL_LEFT_THRESHOLD &&
         horizontalSliderPos < CAPTOUCH_HORIZONTAL_RIGHT_THRESHOLD)
      {
        horizontalMoveState = MOVEMENT_STARTED;
      }
      break;
    case MOVEMENT_STARTED:
      if(horizontalSliderPos < CAPTOUCH_HORIZONTAL_LEFT_THRESHOLD)
      {
        /* Check if the cursor is already at the left border of the screen. */
        if(cursor.cursorLocation.u8ColumnCoordinate == 0)
        {
          break;
        }
        /* If the cursor is currently off, turn it on before moving it. */
        if(cursor.cursorState == CURSOR_OFF)
        {
          err = setDot(&cursor.cursorLocation);
          if(err != GAMEBOARD_SUCCESS)
          {
            return err;
          }
          cursor.cursorState = CURSOR_ON;
        }
        
        /* Update the cursor location and move the cursor state machine. */
        cursor.cursorLocation.u8ColumnCoordinate--;
        horizontalMoveState = MOVEMENT_NOT_STARTED;
      }
      else if(horizontalSliderPos >= CAPTOUCH_HORIZONTAL_RIGHT_THRESHOLD)
      {
        /* Check if the cursor is already at the right border of the screen. */
        if(cursor.cursorLocation.u8ColumnCoordinate == GAMEBOARD_SIZE - 1)
        {
          break;
        }
        
        /* If the cursor is currently off, turn it on before moving it. */
        if(cursor.cursorState == CURSOR_OFF)
        {
          err = setDot(&cursor.cursorLocation);
          if(err != GAMEBOARD_SUCCESS)
          {
            return err;
          }
          cursor.cursorState = CURSOR_ON;
        }
        
        /* Update the cursor location and move the state machine. */
        cursor.cursorLocation.u8ColumnCoordinate++;
        horizontalMoveState = MOVEMENT_NOT_STARTED;
      }
      break;
  }
  
  switch(verticalMoveState)
  {
    case MOVEMENT_NOT_STARTED:
      if(verticalSliderPos >= CAPTOUCH_VERTICAL_DOWN_THRESHOLD &&
         verticalSliderPos < CAPTOUCH_VERTICAL_UP_THRESHOLD)
      {
        verticalMoveState = MOVEMENT_STARTED;
      }
      break;
    case MOVEMENT_STARTED:
      if(verticalSliderPos < CAPTOUCH_VERTICAL_DOWN_THRESHOLD)
      {
        /* Check if the cursor is already at the bottom border of the screen. */
        if(cursor.cursorLocation.u8RowCoordinate == 0)
        {
          break;
        }
        /* If the cursor is currently off, turn it on before moving it. */
        if(cursor.cursorState == CURSOR_OFF)
        {
          err = setDot(&cursor.cursorLocation);
          if(err != GAMEBOARD_SUCCESS)
          {
            return err;
          }
          cursor.cursorState = CURSOR_ON;
        }
        
        /* Update the cursor location and move the cursor state machine. */
        cursor.cursorLocation.u8RowCoordinate--;
        verticalMoveState = MOVEMENT_NOT_STARTED;
      }
      else if(verticalSliderPos >= CAPTOUCH_VERTICAL_UP_THRESHOLD)
      {
        /* Check if the cursor is already at the upper border of the screen. */
        if(cursor.cursorLocation.u8RowCoordinate == GAMEBOARD_SIZE - 1)
        {
          break;
        }
        
        /* If the cursor is currently off, turn it on before moving it. */
        if(cursor.cursorState == CURSOR_OFF)
        {
          err = setDot(&cursor.cursorLocation);
          if(err != GAMEBOARD_SUCCESS)
          {
            return err;
          }
          cursor.cursorState = CURSOR_ON;
        }
        
        /* Update the cursor location and move the state machine. */
        cursor.cursorLocation.u8RowCoordinate++;
        verticalMoveState = MOVEMENT_NOT_STARTED;
      }
      break;
  }
  
  return GAMEBOARD_SUCCESS;
}
/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  switch(userPlayState)
  {
    /* Here the user has not selected any dots to start their turn. Functionality should allow the user to move around
     * the gameboard by blinking the cursor, and moving the cursor based on input from the captouch sensors. The next
     * state (SELECTED_FIRST_DOT) is achieved after the user has selected a dot with BUTTON1 */
    case SELECTED_NOTHING:
      err = blinkCursor();
      if(err != GAMEBOARD_SUCCESS)
      {
        //????
      }
      err = moveCursor();
      if(err != GAMEBOARD_SUCCESS)
      {
        //????
      }
      break;
    
    /* Here the user has selected one dot and must now select a second. Functionality should allow the user to select
     * between possible second dots by blinking a possible line that would connect two dots and switch between possible
     * line options by taking input from the captouch sensors. From here, the user can go back to the previous state
     * (SELECT_NOTHING) by pressing BUTTON0 or go forward to the next state (SELECTED_SECOND_DOT) by pressing BUTTON1
     * when they've reached their desired second dot option. */
    case SELECTED_FIRST_DOT:
      break;
    
     /* Here the user has selected both dots and their turn is over. No more functionality must be provided to the user
      * until the user play state machine is started again. The players move on the gameboard should be updated in the
      * global gameboard variable */
    case SELECTED_SECOND_DOT:
      break;
  }
  
  /* Don't ever remove this! Other functions are depending on it!! */
  u32Timer++;
  
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

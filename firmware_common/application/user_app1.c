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
    
    LcdClearScreen();
    /*drawEmptyGameboard();
    
    u8 i , j;
    for(i = 0; i < GAMEBOARD_SIZE; i++)
    {
      for(j = 0; j < GAMEBOARD_SIZE - 1; j++)
      {
        GameboardCoordinateType lineLocation =
        {
          .u8RowCoordinate = i,
          .u8ColumnCoordinate = j
        };
        drawHorizontalLine(&lineLocation);
      }
    }*/
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

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
    static uint32_t timer = 0;
    
    if(timer % 100 == 0)
    {
      testGameboardDrawingFunctions();
    }
    timer++;
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

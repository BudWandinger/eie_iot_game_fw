/**********************************************************************************************************************
File: user_app1.h                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
1. Follow the instructions at the top of user_app1.c
2. Use ctrl-h to find and replace all instances of "user_app1" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.h
6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

Description:
Header file for user_app1.c

**********************************************************************************************************************/

#ifndef __USER_APP1_H
#define __USER_APP1_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/
typedef enum
{
  GAMEBOARD_SUCCESS,
  BOUNDARY_ERROR_DOT_X,
  BOUNDARY_ERROR_DOT_Y,
  BOUNDARY_ERROR_VLINE_X,
  BOUNDARY_ERROR_VLINE_Y,
  BOUNDARY_ERROR_HLINE_X,
  BOUNDARY_ERROR_HLINE_Y
} GameboardErrorType;

typedef enum
{
  SETTING_DOTS,
  SETTING_VLINES,
  SETTING_HLINES,
  CLEARING_HLINES,
  CLEARING_VLINES,
  CLEARING_DOTS,
  DRAWING_COMPLETE
}DrawingTestState;

typedef enum
{
  DRAW_SET,
  DRAW_CLEAR
} DrawType;
  
typedef struct
{
  u8 u8RowCoordinate;
  u8 u8ColumnCoordinate;
} GameboardCoordinateType;

/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
#define GAMEBOARD_SIZE          (u8)8
#define GAMEBOARD_DOT_WIDTH     (u8)3
#define GAMEBOARD_LINE_WIDTH    (u8)1
#define GAMEBOARD_LINE_LENGTH   (u8)5

#define GAMEBOARD_BORDER_DOT_OFFSET             (u8)2
#define GAMEBOARD_DOT_OFFSET                    (u8)8
#define GAMEBOARD_BORDER_LINE_WIDTH_OFFSET      (u8)3
#define GAMEBOARD_BORDER_LINE_LENGTH_OFFSET     (u8)5
#define GAMEBOARD_LINE_OFFSET                   (u8)8
  
/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/
GameboardErrorType setDot(GameboardCoordinateType* coordinate_);
GameboardErrorType clearDot(GameboardCoordinateType* coordinate_);
GameboardErrorType setVerticalLine(GameboardCoordinateType* coordinate_);
GameboardErrorType clearVerticalLine(GameboardCoordinateType* coordinate_);
GameboardErrorType setHorizontalLine(GameboardCoordinateType* coordinate_);
GameboardErrorType clearHorizontalLine(GameboardCoordinateType* coordinate_);

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void UserApp1Initialize(void);
void UserApp1RunActiveState(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
static GameboardErrorType drawDot(GameboardCoordinateType* coordinate_, DrawType drawType_);
static GameboardErrorType drawVerticalLine(GameboardCoordinateType* coordinate_, DrawType drawType_);
static GameboardErrorType drawHorizontalLine(GameboardCoordinateType* coordinate_, DrawType drawType_);
static GameboardErrorType drawEmptyGameboard(void);
static GameboardErrorType testGameboardDrawingFunctions(void);

/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserApp1SM_Idle(void);    

static void UserApp1SM_Error(void);         


#endif /* __USER_APP1_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

/*
Description: Onitama game implementation for CCPROG2 Machine Project. Onitama is a fast-paced, abstract strategy board game for two players, 
             combining elements of chess and martial arts in a minimalist yet deeply strategic format.
Programmed by: John Benedict G. Santos S11
Last modified: 23 July 2025
Version: 1.0
Acknowledgements : Ansi escape code reference: https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
                   conio.h uses from https://stackoverflow.com/questions/59812014/why-use-conio-h
                   srand and rand uses https://stackoverflow.com/questions/52801380/srandtimenull-function
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define MAX_MOVES 200
#define MAX_MENU_SELECTION 4
#define MAX_FILENAME_LEN 100
#define MAX_NAME_LEN 31
#define MAX_CARDS_PER_PLAYER 2
#define MAX_LEAD_LINES 20
#define MAX_CARDS 16
#define MAX_CARD_PER_GAME 5
#define BOARD_MAX_LENGTH 50
#define TEXT_FILE_ROWS 6
#define VISUAL_CARD_CENTER 2
#define ROWS 5
#define COLS 5
#define NUM_OF_STUD 5


#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER 13

typedef char str30[31];
typedef char str100[100];
typedef char oniBrd[ROWS][COLS][BOARD_MAX_LENGTH];   

struct leadTag
{
    char name[MAX_NAME_LEN];
    int turns;
};

typedef struct leadTag lead;

struct posTag 
{
    int x, y;
};

typedef struct posTag Position;

struct dataCrdsTag
{
    str30 rCardDisp[TEXT_FILE_ROWS];
    str30 lCardDisp[TEXT_FILE_ROWS];
    str30 rCardName;
    str30 lCardName;
};

typedef struct dataCrdsTag dataCrdsIG;

struct splayerCard 
{
    str30 playercard[MAX_CARDS_PER_PLAYER];
    str30 visualCard[MAX_CARDS_PER_PLAYER][TEXT_FILE_ROWS];
};

typedef struct splayerCard playercard;

struct pieceTag
{
    char sensei[BOARD_MAX_LENGTH];
    char student[NUM_OF_STUD][BOARD_MAX_LENGTH];
    char selectedPiece[BOARD_MAX_LENGTH];
};

typedef struct pieceTag piece;

struct playerTag
{
    piece pPiece;
    playercard card;
    int turns;
    str30 name;
    str30 pSelectedCard;
};

typedef struct playerTag player;

struct MoveTag 
{
    int player;           
    Position from;
    Position to;
    char cardName[MAX_NAME_LEN];  
};

typedef struct MoveTag MoveRecord;

/*
Resets the board to empty state

Precondition:
    - Board array must be properly allocated
    - ROWS and COLS must match board dimensions

@param board The game board to reset
@return Returns no data type (void)
*/
void resetBoard(oniBrd board) 
{
    int i, j;
    
    for (i = 0; i < ROWS; i++) 
    {
        for (j = 0; j < COLS; j++) 
        {
            strcpy(board[i][j], "| _ |");
        }
    }
}

/*
Records a move into the replay system

Precondition:
    - replayMoves array must have space for MAX_MOVES
    - moveCount must point to valid integer < MAX_MOVES
    - cardUsed must be null-terminated string < MAX_NAME_LEN

@param replayMoves Array to store moves
@param moveCount Pointer to current move count
@param player Player number (1 or 2)
@param from Starting position
@param to Destination position
@param cardUsed Name of card used for move
@return Returns no data type (void)
*/
void recordMove(MoveRecord* replayMoves, int* moveCount, int player,
                Position from, Position to, char* cardUsed) 
{
    if (*moveCount < MAX_MOVES) 
    {
        replayMoves[*moveCount].player = player;
        replayMoves[*moveCount].from = from;
        replayMoves[*moveCount].to = to;

        strcpy(replayMoves[*moveCount].cardName, cardUsed);
        (*moveCount)++;
    }
}

/*
Applies a single move to the board during replay

Precondition:
    - Board must be properly initialized
    - move.from and move.to must be valid positions
    - BOARD_MAX_LENGTH must be sufficient for piece strings

@param board The game board to modify
@param move The move record to apply
@return Returns no data type (void)
*/
void applyMove(oniBrd board, MoveRecord move) 
{
    char piece[BOARD_MAX_LENGTH];
    
    strcpy(piece, board[move.from.y][move.from.x]);
    strcpy(board[move.to.y][move.to.x], piece);
    strcpy(board[move.from.y][move.from.x], "| _ |");
}

/*
Displays the current board state during replay

Precondition:
    - Board must be properly initialized

@param board The game board to display
@return Returns no data type (void)
*/
void displayReplayBoard(oniBrd board)
{
    int i, j;

    system("CLS");
    printf("\n\t\t\t\t\t\t" "\x1b[36m" " O N I T A M A   R E P L A Y " "\x1b[0m" "\n\n");

    printf("\t\t\t\t ");
    for (i = 0; i < COLS; i++)
    {
        printf("+-------");
    }
    printf("+\n");

    for (i = 0; i < ROWS; i++)
    {
        printf("\t\t\t\t ");
        for (j = 0; j < COLS; j++)
        {
            printf("| %s ", board[i][j]);
        }
        printf("|\n\t\t\t\t ");

        for (j = 0; j < COLS; j++)
        {
            printf("+-------");
        }
        printf("+\n");
    }
    printf("\n");
}
/*
Plays back all recorded moves in sequence to recreate the game

Precondition:
    - replayMoves must contain moveCount valid moves
    - moveCount must be <= MAX_MOVES
    - Board display functions must be initialized

@param replayMoves Array containing all recorded moves
@param moveCount Number of moves in replayMoves array
@return Returns no data type (void)
*/
void replayMovesSequence(MoveRecord* replayMoves, int moveCount)
{
    oniBrd board;
    int i;

    resetBoard(board);
    // Set up initial board state
    // Player 2 (RED) pieces at top
    strcpy(board[0][0], "\x1b[31m" "| s |" "\x1b[0m");
    strcpy(board[0][1], "\x1b[31m" "| s |" "\x1b[0m");
    strcpy(board[0][2], "\x1b[31m" "| M |" "\x1b[0m");
    strcpy(board[0][3], "\x1b[31m" "| s |" "\x1b[0m");
    strcpy(board[0][4], "\x1b[31m" "| s |" "\x1b[0m");

    // Player 1 (BLUE) pieces at bottom
    strcpy(board[4][0], "\x1b[34m" "| s |" "\x1b[0m");
    strcpy(board[4][1], "\x1b[34m" "| s |" "\x1b[0m");
    strcpy(board[4][2], "\x1b[34m" "| M |" "\x1b[0m");
    strcpy(board[4][3], "\x1b[34m" "| s |" "\x1b[0m");
    strcpy(board[4][4], "\x1b[34m" "| s |" "\x1b[0m");

    displayReplayBoard(board);
    printf("Press any key to start replay...");
    getch();

    for (i = 0; i < moveCount; i++)
    {
        displayReplayBoard(board);
        printf("Move %d/%d: Player %d used %s\n", i + 1, moveCount,
               replayMoves[i].player, replayMoves[i].cardName);
        printf("From: (%d, %d) To: (%d, %d)\n",
               replayMoves[i].from.x, replayMoves[i].from.y,
               replayMoves[i].to.x, replayMoves[i].to.y);

        applyMove(board, replayMoves[i]);
        Sleep(1000);
    }
    displayReplayBoard(board);
}
/*
Reads HallOfFame data from file and store into str100 storeHoF[]

Precondition:
    - HallOfFame.txt must exist
    - storeHoF array must be allocated

@param storeHoF Array to store HallOfFame entries
@return Returns no data type (void)
*/
void readHoF(str100 storeHoF[])
{
    FILE *fp = fopen("HallOfFame.txt", "r");
    int i;

    if (fp != NULL) // file must Exist
    {
        for(i=0; i < MAX_LEAD_LINES; i++)
            fscanf(fp, "%s", storeHoF[i]); 

        fclose(fp);
    }
}

/*
Creates and returns a Position structure with specified coordinates

Precondition:
    - x and y must be valid board coordinates (0 <= x < COLS, 0 <= y < ROWS)

@param x The column coordinate (0-based)
@param y The row coordinate (0-based)
@return Returns a Position struct with the specified coordinates
*/
Position makePosition(int x, int y) 
{
    Position p;
    
    p.x = x;
    p.y = y;

    return p;
}

/*
Reads move cards file and store data into str30 moveDeck[]

Precondition:
    - movecards.txt must exist and be properly formatted
    - moveDeck array must be allocated
    - cardCount pointer must be valid

@param moveDeck Array to store loaded card names
@param cardCount Pointer to store number of cards loaded
@return Returns no data type (void)
*/
int readMoveCards(str30 moveDeck[], int *cardCount) 
{
    int i;
    FILE *fp;
    int status = 0;
    
    fp = fopen("movecards.txt", "r");

    if (fp != NULL) 
    {
        if(fscanf(fp, "%d", cardCount)==1)
        {
            status = 1;
            for (i = 0; i < *cardCount; i++) 
            {
                fscanf(fp, "%s", moveDeck[i]);
            }
        }
    
    fclose(fp);
    }
    else
        printf("Error: Cannot open file %s\n", "movecards.txt");
    
    return status;
}

/*
Shuffles all the cards before the Game starts and before the player draws

Precondition: moveDeck must be initialized with cardCount valid strings, 
              and cardCount must be a positive integer less than or equal 
              to the size of moveDeck. int cardCount is > 0

@param moveDeck: 1D array of strings that stores the name of the cards, Its needed in the function because the
       functions purpose is to shuffle all the cards before the player draws

@param cardCount: the number of cards in moveDeck that should be shuffled 
@return Returns no data type (void)
*/
void shuffleCards(str30 moveDeck[], int cardCount) 
{
    int i, j, k;
    str30 temp;

    for(i = 0; i < cardCount; i++) // Loops n of cardCount times
    {
        j = rand() % cardCount; // Two variables that generates two random numbers
        k = rand() % cardCount;

        strcpy(temp, moveDeck[k]);
        strcpy(moveDeck[k], moveDeck[j]);
        strcpy(moveDeck[j], temp);
    }
} 

/*
Distributes the first 5 index of moveDeck cards to players and sets up the initial game cards

Precondition: 
    - moveDeck must contain at least 5 initialized card strings
    - p1 and p2 must be valid player pointers with properly allocated card arrays
    - crdsIG must be a valid dataCrdsIG pointer
    - moveDeck must have been previously shuffled

@param str30 moveDeck[] Array containing the shuffled deck of cards to distribute
@param *p1 Pointer to player 1's data structure to receive their cards
@param *p2 Pointer to player 2's data structure to receive their cards
@param *crdsIG Pointer to game card data structure for initial setup
@return Returns no data type (void)
*/
void assignMoveCards(str30 moveDeck[], player *p1, player *p2, dataCrdsIG *crdsIG) 
{
    int i, j;
    char filename[MAX_FILENAME_LEN];
    FILE *fp;

    // Assigns the first 4 cards to player 1 and 2 and the fifth card in the index of movedeck to the rightcard or middle card
    strcpy(p1->card.playercard[0], moveDeck[0]);
    strcpy(p1->card.playercard[1], moveDeck[1]);

    strcpy(p2->card.playercard[0], moveDeck[2]);
    strcpy(p2->card.playercard[1], moveDeck[3]);

    strcpy(crdsIG->lCardName, "None"); // Initializes the left card
    strcpy(crdsIG->rCardName, moveDeck[4]);

    // loops through the max cards a player can hold in this instance for player 1
    for (i = 0; i < MAX_CARDS_PER_PLAYER; i++)
    {
        strcpy(filename, p1->card.playercard[i]); // assigns the name of the move card to a variable filename
        strcat(filename, ".txt"); 
        fp = fopen(filename, "r");
        
        if (fp != NULL) 
        {
            for (j = 0; j < TEXT_FILE_ROWS; j++) // loops 6 times since the txt file is 6X5 row 1 being the color of the card
                fscanf(fp, "%s", p1->card.visualCard[i][j]);

            fclose(fp); 
        }
        else
        {
            printf("Error: Cannot open file %s\n", filename);
            Sleep(1000);
        }
    }

    // loops through the max cards a player can hold in this instance for player 2
    for (i = 0; i < MAX_CARDS_PER_PLAYER; i++)
    {
        strcpy(filename, p2->card.playercard[i]); // assigns the name of the move card to a variable filename
        strcat(filename, ".txt"); 
        fp = fopen(filename, "r");

        if (fp != NULL) 
        {
            for (j = 0; j < TEXT_FILE_ROWS; j++) // loops 6 times since the txt file is 6X5 row 1 being the color of the card
                fscanf(fp, "%s", p2->card.visualCard[i][j]);

            fclose(fp);
        }
        else
        {
            printf("Error: Cannot open file %s\n", filename);
            Sleep(1000); 
        }
    }

    // assigns the name of the move card to a variable filename for this instance the middle card or rightcard
    strcpy(filename, crdsIG->rCardName); // assigns the name of the move card to a variable filename
    strcat(filename, ".txt"); 
    fp = fopen(filename, "r");
    
    if (fp != NULL) 
    {
        for (i = 0; i < TEXT_FILE_ROWS; i++) // loops 6 times since the txt file is 6X5 row 1 being the color of the card
            fscanf(fp, "%s", crdsIG->rCardDisp[i]); 

        fclose(fp);
    }   
    else
    {
        printf("Error: Cannot open file %s\n", filename);
        Sleep(1000); 
    }

    for(i=0;i<TEXT_FILE_ROWS;i++) // initializes the left card for displaying otherwise it would display null chars
        strcpy(crdsIG->lCardDisp[i], ".....");
}

/*
Checks the win condition by verifying if either player's Sensei piece has reached
the opponent's temple or if a Sensei has been captured

Precondition: 
    - Board must be properly initialized with valid positions
    - Player structures must contain valid Sensei piece identifiers
    - ROWS and COLS must match board dimensions

@param board The game board array to check for win conditions
@param *p1 Pointer to player 1's data structure
@param *p2 Pointer to player 2's data structure
@return Returns 1 if player 1 wins, 2 if player 2 wins, 0 if no winner yet
*/
int checkWinCondition(oniBrd board, player *p1, player *p2) 
{
    int i, j;
    int result = 0;
    int p1SenseiFound = 0;
    int p2SenseiFound = 0;
    
    //checks if the player 1 sensei is on the temples arch, vice versa
    if (strcmp(board[0][COLS/2], p1->pPiece.sensei) == 0) 
        result = 1;

    else if (strcmp(board[ROWS-1][COLS/2], p2->pPiece.sensei) == 0) 
        result = 2;
    
    //loops through the whole board to check if a sensei exists
    if (result == 0) 
    {
        for (i = 0; i < ROWS; i++) 
        {
            for (j = 0; j < COLS; j++) 
            {
                if (strcmp(board[i][j], p1->pPiece.sensei) == 0) 
                    p1SenseiFound = 1;
        
                if (strcmp(board[i][j], p2->pPiece.sensei) == 0) 
                    p2SenseiFound = 1;
            }
        }
        
        //if the p1 sensei is not found in any of the board and is still 0 means player 2 won, 
        // otherwise if p2 sensei is not found p1 won, otherwise result is 0 and the game will continue
        if (!p1SenseiFound) 
            result = 2;

        else if (!p2SenseiFound) 
            result = 1;
    }
    return result;
}

/*
Checks if a character is alphabetic

Precondition: 
    - Input must be a single character

@param name The character to check
@return Returns 1 if character is alphabetic, 0 otherwise
*/
int isAlpha (char name)
{
    return (name >= 'A' && name <= 'Z') || (name >= 'a' && name <= 'z');
}

/*
Validates if the selected piece belongs to the current player

Precondition: 
    - Board coordinates must be within valid range
    - Player turn must be either 0 or 1
    - Board must be properly initialized

@param *currentPlayer Pointer to the current player's data
@param board The game board array
@param fromRow Source row coordinate
@param fromCol Source column coordinate
@param playerTurn Current player's turn indicator (0 or 1)
@return Returns 1 if piece is valid for current player, 0 otherwise
*/
int isValidpiece(player *currentPlayer, oniBrd board, int fromRow, int fromCol, int playerTurn)
{
    int result = 0;

    if(playerTurn == 0)
    {
        if (strcmp(board[fromRow][fromCol], "\x1b[34m" "| M |" "\x1b[0m") == 0 || //checks if the selected piece is itself from the current player
            strcmp(board[fromRow][fromCol], "\x1b[34m" "| s |" "\x1b[0m") == 0)
            result = 1;
    }
    else
    {
        if (strcmp(board[fromRow][fromCol], "\x1b[31m" "| M |" "\x1b[0m") == 0||
            strcmp(board[fromRow][fromCol], "\x1b[31m" "| s |" "\x1b[0m") == 0)
            result = 1;
    }
    return result;
}

/*
Validates if a desired move follows the game rules and card pattern

Precondition: 
    - Selected card must be valid (0 or 1)
    - Board and player structures must be properly initialized

@param *currentPlayer Pointer to the current player's data
@param board The game board array
@param fromRow Source row coordinate
@param fromCol Source column coordinate
@param toRow Destination row coordinate
@param toCol Destination column coordinate
@param playerTurn Current player's turn indicator (0 or 1)
@param selectedCard Which of the player's cards is being used (0 or 1)
@return Returns 1 if move is valid, 0 otherwise
*/
int isValidMove(player *currentPlayer, oniBrd board, int fromRow, int fromCol,
                int toRow, int toCol, int playerTurn, int selectedCard)
{
    int isValidMovement = 0;
    int isSamePosition = 0;
    int isOccupiedByOwnPiece = 0;
    int rowDisplacement = toRow - fromRow;
    int columnDisplacement = toCol - fromCol;
    int cardPatternRow = 0;
    int cardPatternColumn = 0;
    int isPatternMatch = 0;
    int outofBounds = 0;

    isSamePosition = (fromRow == toRow) && (fromCol == toCol);

    if (playerTurn == 1)
        rowDisplacement *= -1;

    cardPatternRow = VISUAL_CARD_CENTER + rowDisplacement + 1;
    cardPatternColumn = VISUAL_CARD_CENTER + columnDisplacement;

    if (cardPatternRow < 0 || cardPatternRow >= TEXT_FILE_ROWS ||
        cardPatternColumn < 0 || cardPatternColumn >= COLS)
        outofBounds = 1;

    if (playerTurn == 0)
        isOccupiedByOwnPiece = (strcmp(board[toRow][toCol], "\x1b[34m" "| s |" "\x1b[0m") == 0 || strcmp(board[toRow][toCol], "\x1b[34m" "| M |" "\x1b[0m") == 0);
    else
        isOccupiedByOwnPiece = (strcmp(board[toRow][toCol], "\x1b[31m" "| s |" "\x1b[0m") == 0 || strcmp(board[toRow][toCol], "\x1b[31m" "| M |" "\x1b[0m") == 0);

    if (!outofBounds)
        isPatternMatch = currentPlayer->card.visualCard[selectedCard][cardPatternRow][cardPatternColumn] == 'x';

    if (!isSamePosition && !isOccupiedByOwnPiece && isPatternMatch)
        isValidMovement = 1;

    return isValidMovement;
}
/*
Checks if the current player has any valid moves available using the selected card. Iterates through all possible cards, 
board positions, and potential moves to determine if there exists at least one valid move for the current player's pieces.
Precondition: 

- The board must be properly initialized
- currentPlayer must point to a valid player structure
- playerTurn must be either 1 (RED) or 2 (BLUE)
@param currentPlayer Pointer to the current player's data (including their cards)
@param board The game board represented as a 2D array
@param playerTurn Indicates which player's turn it is (1 for RED, 2 for BLUE)
@param selectedCard Which of the player's cards is being used (0 or 1)
@return 1 if at least one valid move exists for the current player, 0 otherwise
 */
int hasAnyValidMove(player *currentPlayer, oniBrd board, int playerTurn)
{
    int novalidMove = 0;
    int toCol;
    int toRow;
    int fromRow;
    int fromCol;
    int nCard;

    for(nCard = 0; nCard < MAX_CARD_PER_GAME; nCard++)
    {
        // loops all the possible positions in the array
        for(fromRow = 0; fromRow < ROWS; fromRow++)
        {
            for(fromCol = 0; fromCol < COLS; fromCol++)
            {
                if(playerTurn == 1)
                {
                    if(strcmp(board[fromRow][fromCol], "\x1b[31m" "| s |" "\x1b[0m") == 0 ||
                       strcmp(board[fromRow][fromCol], "\x1b[31m" "| M |" "\x1b[0m") == 0)
                    {
                        for(toRow = 0; toRow < ROWS; toRow++)
                        {
                            for(toCol = 0; toCol < COLS; toCol++)
                            {
                                if(isValidMove(currentPlayer, board, fromRow, fromCol, toRow, toCol, playerTurn, nCard))
                                    novalidMove = 1;
                            }
                        }
                    }
                }
                else
                {
                    if(strcmp(board[fromRow][fromCol], "\x1b[34m" "| s |" "\x1b[0m") == 0 ||
                       strcmp(board[fromRow][fromCol], "\x1b[34m" "| M |" "\x1b[0m") == 0)
                    {
                        for(toRow = 0; toRow < ROWS; toRow++)
                        {
                            for(toCol = 0; toCol < COLS; toCol++)
                            {
                                if(isValidMove(currentPlayer, board, fromRow, fromCol, toRow, toCol, playerTurn, nCard))
                                    novalidMove = 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return novalidMove;
}

/*
Checks if a specific piece has any valid moves with the currently selected card
This prevents players from selecting pieces that can't move with their chosen card
Precondition:
    - currentPlayer must be valid and initialized
    - board must be properly populated
    - fromRow/fromCol must be valid board coordinates
    - playerTurn must be 0 (BLUE) or 1 (RED)
    - selectedCard must be 0 or 1 (index of player's cards)

@param currentPlayer Pointer to current player's data
@param board The game board state
@param fromRow Row of piece to check
@param fromCol Column of piece to check  
@param playerTurn Current player's turn indicator
@param selectedCard Which card is being used (0 or 1)
@return 1 if valid moves exist, 0 otherwise
*/
int hasValidMoveForPiece(player *currentPlayer, oniBrd board, int fromRow, int fromCol, 
                        int playerTurn, int selectedCard) 
{
    int toRow;
    int toCol;
    int moveValidExist = 0;

    for(toRow = 0; toRow < ROWS; toRow++) 
    {
        for(toCol = 0; toCol < COLS; toCol++) 
        {
            if(isValidMove(currentPlayer, board, fromRow, fromCol, toRow, toCol, playerTurn, selectedCard)) 
                moveValidExist = 1;
        }
    }
    return moveValidExist;
}

/*
Determines which player gets the first turn based on the color of the right game card

Precondition: 
    - crdsIG must be properly initialized with valid card displays
    - crdsIG.rCardDisp[0] must contain either "red" or another valid color string
    - The right card's display data must be loaded from its corresponding text file

@param crdsIG The game cards data structure containing card display information
@return Returns 1 if the first player (RED) should start, 0 if the second player (BLUE) should start otherwise -1
*/
int getFirstTurn(dataCrdsIG crdsIG) 
{
    int result = -1;

    if (strcmp(crdsIG.rCardDisp[0], "red") == 0)
        result = 1;
    else if(strcmp(crdsIG.rCardDisp[0], "blue") == 0)
        result = 0;

    return result;  
}

/*
Updates the HallOfFame with a winner's information

Precondition:
    - winner must contain valid name and turns
    - HallOfFame.txt exists

@param winner The winning player's data
@return Returns no data type (void)
*/
void updateHoF(player winner) 
{
    lead leaders[MAX_LEAD_LINES/2 + 1] = {0};  
    lead temp;
    int count = 0;
    int i = 0;
    int j = 0;
    int min_idx = 0;
    FILE *fp;
    
    fp = fopen("HallOfFame.txt", "r");

    if (fp != NULL) 
    {
    while (count < MAX_LEAD_LINES / 2 &&
           fscanf(fp, "%s", leaders[count].name) == 1 &&
           fscanf(fp, "%d", &leaders[count].turns) == 1)
        {
            count++;
        }
        fclose(fp);
    }

    /* 2. Add new winner */
    if (count < MAX_LEAD_LINES/2 + 1) 
    {
        strcpy(leaders[count].name, winner.name);
        leaders[count].turns = winner.turns;
        count++;
    }

    // Sort by turns 
    for (i = 0; i < count - 1; i++) 
    {
        min_idx = i;
        for (j = i + 1; j < count; j++) 
        {
            if (leaders[j].turns < leaders[min_idx].turns) 
                min_idx = j;
        }
        
        if (min_idx != i)
        { 
            /* Swap elements */
            strcpy(temp.name, leaders[i].name);
            temp.turns = leaders[i].turns;

            strcpy(leaders[i].name, leaders[min_idx].name);
            leaders[i].turns = leaders[min_idx].turns;
            
            strcpy(leaders[min_idx].name, temp.name);
            leaders[min_idx].turns = temp.turns;
        }
    }

    if (count > MAX_LEAD_LINES/2) //Keep only top MAX_LEAD_LINES/2 entries 
        count = MAX_LEAD_LINES/2;

   
    // Write back to file 
    fp = fopen("HallOfFame.txt", "w");

    for (i = 0; i < count; i++) 
        fprintf(fp, "%s\n%d\n", leaders[i].name, leaders[i].turns);
        
    fclose(fp);

}

/*
Displays the hall of fame

Precondition:
    - HallOfFame.txt must exist in working directory
@return Returns no data type (void)
*/
void displayHoF()
{
    str100 hallofFame[MAX_LEAD_LINES] = {0};
    int i;

    readHoF(hallofFame);

    system("CLS");
    printf("\n");
    printf("\t\t" "\x1b[36m" "|" "\x1b[0m" "\x1b[1m" "           --HALL OF FAME--           " "\x1b[0m" "\x1b[36m" "|" "\x1b[0m" "\n");

    if(hallofFame[0][0] == '\0')
        printf("\x1b[33m" "No Hall of Fame entries yet. Be the first to make history!\n" "\x1b[0m");

    else
    {
        for(i = 0; i < MAX_LEAD_LINES; i+=2)
        {
            if(hallofFame[i][0] == '\0')
                i = MAX_LEAD_LINES;

            else
            {
                printf("\x1b[36m" "Top %d:\n" "\x1b[0m", (i / 2) + 1);
                printf("\x1b[35m" "Name:" "\x1b[0m" "%s\n", hallofFame[i]);
                printf("\x1b[35m" "Turns It Took To Win: " "\x1b[0m" "%s", hallofFame[i+1]);
            }
            printf("\n");
        }
    }

    printf("\n\n\n\t\t" "\x1b[36m" "|" "\x1b[0m" "   Press " "\x1b[32m" "ENTER" "\x1b[0m" " to return to Main Menu   " "\x1b[36m" "|" "\x1b[0m" "\n");
    getchar();
}

/*
Displays game instructions and controls

Precondition: None
@return Returns no data type (void)
*/
void howToPlay()
{
    system("CLS");
    printf("\n");
    printf("\t\t" "\x1b[36m" "|" "\x1b[0m" "\x1b[1m" "           HOW TO PLAY ONITAMA!           " "\x1b[0m" "\x1b[36m" "|" "\x1b[0m" "\n");

    printf("\x1b[33m" " Overview:" "\x1b[0m" "\n");
    printf(" ------------------------------------------\n");
    printf(" Onitama is a two-player turn-based strategy game played on a 5x5 board.\n");
    printf(" Each player has five pieces: one " "\x1b[1m" "SENSEI" "\x1b[0m" " (Master) and four " "\x1b[1m" "STUDENTS" "\x1b[0m" ".\n\n");

    printf("\x1b[33m" " Objective:" "\x1b[0m" "\n");
    printf(" ------------------------------------------\n");
    printf(" To win, you must accomplish one of these:\n");
    printf(" " "\x1b[32m" "1." "\x1b[0m" " Capture your opponent's " "\x1b[1m" "SENSEI" "\x1b[0m" "\n");
    printf(" " "\x1b[32m" "2." "\x1b[0m" " Move your " "\x1b[1m" "SENSEI" "\x1b[0m" " to your opponent's starting temple (center of their back row)\n\n");

    printf("\x1b[33m" " Game Setup:" "\x1b[0m" "\n");
    printf(" ------------------------------------------\n");
    printf(" - Players start with their pieces arranged on opposite sides\n");
    printf(" - Each player begins with 2 unique movement cards\n");
    printf(" - There's 1 neutral card that isn't assigned to either player\n\n");

    printf("\x1b[33m" " Movement Rules:" "\x1b[0m" "\n");
    printf(" ------------------------------------------\n");
    printf(" - On your turn, you must perform " "\x1b[1m" "ONE" "\x1b[0m" " action:\n");
    printf("   " "\x1b[32m" "1." "\x1b[0m" " Move a piece according to one of your movement cards\n");
    printf("   " "\x1b[32m" "2." "\x1b[0m" " Exchange one of your cards with the neutral card\n");
    printf(" - After moving, the used card becomes the neutral card\n");
    printf(" - Movement patterns are shown on the cards (arrows indicate possible moves)\n\n");

    printf("\x1b[33m" " Special Rules:" "\x1b[0m" "\n");
    printf(" ------------------------------------------\n");
    printf(" - " "\x1b[1m" "SENSEI" "\x1b[0m" " can be captured like any other piece\n");
    printf(" - " "\x1b[1m" "STUDENTS" "\x1b[0m" " can capture opponent's pieces by landing on them\n");
    printf(" - If a " "\x1b[1m" "STUDENT" "\x1b[0m" " reaches the opponent's temple row, it remains there\n\n");

    printf("\t\t" "\x1b[36m" "|" "\x1b[0m" "   Press " "\x1b[32m" "ENTER" "\x1b[0m" " to return to Main Menu   " "\x1b[36m" "|" "\x1b[0m" "\n");

    getchar();
}

/*
Initializes player piece names with proper colors

Precondition:
    - Player structures must be allocated
    - Color macros must be properly defined

@param p1 Pointer to Player 1's data
@param p2 Pointer to Player 2's data
@return Returns no data type (void)
*/
void populatePlayerPiece(player *p1, player *p2)
{
    int i;

    for(i = 0; i < NUM_OF_STUD; i++)
    {
        if(i==NUM_OF_STUD/2)
        strcpy(p1->pPiece.sensei, "\x1b[34m" "| M |" "\x1b[0m");
        else
        strcpy(p1->pPiece.student[i], "\x1b[34m" "| s |" "\x1b[0m");
    }


    for(i = 0; i < NUM_OF_STUD; i++)
    {
        if(i==NUM_OF_STUD/2)
        strcpy(p2->pPiece.sensei, "\x1b[31m" "| M |" "\x1b[0m");
        else
        strcpy(p2->pPiece.student[i], "\x1b[31m" "| s |" "\x1b[0m");
    }
}

/*
Initializes the game board with starting piece positions

Precondition:
    - Board must be properly allocated
    - Player structures must have initialized piece names

@param board The game board to populate
@param p1 Pointer to Player 1's data
@param p2 Pointer to Player 2's data
@return Returns no data type (void)
*/
void populateBoard(oniBrd board, player *p1, player *p2) 
{
    int i, j;

    for(i = 0; i < ROWS; i++)
    {
        for(j = 0; j < COLS; j++)
        {   
            strcpy(board[i][j], "| _ |");
        }
    }

    for(j = 0; j < COLS; j++)
    {
        if(j == COLS/2)
            strcpy(board[ROWS-1][j], p1->pPiece.sensei);
        else if(j < NUM_OF_STUD)
            strcpy(board[ROWS-1][j], p1->pPiece.student[j]);
    }

    for(j = 0; j < COLS; j++)
    {
        if(j == COLS/2)
            strcpy(board[0][j], p2->pPiece.sensei);
        else if(j < NUM_OF_STUD)  
            strcpy(board[0][j], p2->pPiece.student[j]);
    }

}

/*
Displays the current game board with all pieces and cards

Precondition:
    - Board must be properly populated
    - Player structures must be initialized
    - crdsIG must contain current card displays
    - selRow and selCol must be valid coordinates or -1
    - nFlagPiece must be 0 or 1

@param board The game board
@param p1 Pointer to Player 1's data
@param p2 Pointer to Player 2's data
@param crdsIG Pointer to game cards display data
@param selRow Currently selected row (-1 for none)
@param selCol Currently selected column (-1 for none)
@param nFlagPiece Selection highlight flag
@return Returns no data type (void)
*/
void displayBoard(oniBrd board, player *p1, player *p2, dataCrdsIG *crdsIG, int selRow, int selCol, int nFlagPiece)
{
    int i, j;

    system("CLS");
    printf("\n\t\t\t\t\t\t" "\x1b[36m" " O N I T A M A " "\x1b[0m" "\t\t\t\t\t\t\n");


    printf(" " "\x1b[31m" "[Player 2] " "\x1b[0m" "%s's Cards:\n", p2->name);

    for (i = TEXT_FILE_ROWS - 1; i >= 0; i--)
    {
        printf(" \t\t\t\t\t" "\x1b[31m" "| " "\x1b[0m" "%6s" "\x1b[31m" " |\t\t| " "\x1b[0m", p2->card.visualCard[0][i]);
        printf("%6s" "\x1b[31m" " |\n" "\x1b[0m", p2->card.visualCard[1][i]);
    }

    printf(" \t\t\t\t\t  " "\x1b[31m" "%6s" "\x1b[0m" "\t\t  " "\x1b[31m" "%6s\n" "\x1b[0m", p2->card.playercard[0], p2->card.playercard[1]);

    printf("\n\t\t" "\x1b[33m" "+-------------------+" "\x1b[0m" "\t\t\t\t\t\t\t" "\x1b[33m" "+-------------------+" "\x1b[0m");
    printf("\n\t\t" "\x1b[33m" "| Card: %6s |" "\x1b[0m" "\t\t\t\t\t\t\t" "\x1b[33m" "| Card: %6s |\n" "\x1b[0m",
           crdsIG->lCardName, crdsIG->rCardName);
    printf("\t\t" "\x1b[33m" "+-------------------+" "\x1b[0m" "\t\t\t\t\t\t\t" "\x1b[33m" "+-------------------+" "\x1b[0m");
    printf("\n\n\t\t\t\t ");

    for (i = 0; i < COLS+1; i++)
    {
        printf("+-------");
    }
    printf("+\n");

    for (i = 0; i < ROWS; i++)
    {
        printf("\t\t%s\t\t", crdsIG->lCardDisp[i]);
        for (j = 0; j < COLS; j++)
        {
            printf("| ");
            if (i == selRow && j == selCol && nFlagPiece == 1) //if nFlagPiece is 1 prints the arrow for user to
                                                               //see where the current position is he currently selecting
            {
                // Changed to Magenta for the arrows
                printf("\x1b[35m" ">" "\x1b[0m"); // Magenta '>' and reset color immediately
                printf("%s", board[i][j]);         // Print board content (e.g., "| M |" or "| s |" which have their own colors)
                printf("\x1b[35m" "<" "\x1b[0m"); // Magenta '<' and reset color immediately
            }
            else
                printf(" %s ", board[i][j]);
            printf(" ");
        }

        printf("|\t\t");
        if (i < TEXT_FILE_ROWS)
        {
            printf("%s", crdsIG->rCardDisp[i]);
        }

        printf("\n\t\t\t\t ");

        for (j = 0; j < COLS+1; j++)
        {
            printf("+-------");
        }
        printf("+\n");
    }

    printf("\t\t%s", crdsIG->lCardDisp[TEXT_FILE_ROWS-1]);
    printf("\t\t\t\t\t\t\t\t\t\t%s\n\n", crdsIG->rCardDisp[TEXT_FILE_ROWS-1]);

    // Player 1 (bottom) cards with better labels
    printf(" " "\x1b[34m" "[Player 1] " "\x1b[0m" "%s's Cards:\n", p1->name);
    for (i = 0; i < TEXT_FILE_ROWS; i++)
    {
        printf(" \t\t\t\t\t" "\x1b[34m" "| " "\x1b[0m" "%6s" "\x1b[34m" " |\t\t| " "\x1b[0m", p1->card.visualCard[0][i]);
        printf("%6s" "\x1b[34m" " |\n" "\x1b[0m", p1->card.visualCard[1][i]);
    }
    printf(" \t\t\t\t\t  " "\x1b[34m" "%6s" "\x1b[0m" "\t\t  " "\x1b[34m" "%6s\n\n" "\x1b[0m", p1->card.playercard[0], p1->card.playercard[1]);
}
/*
Swaps a player's card with a game card according to game rules

Precondition:
    - currentPlayer must have valid cards
    - crdsIG must contain valid game cards
    - selectedCard must be 0 or 1
    - playerTurn must be 0 or 1
    - round must be >= 1

@param currentPlayer Pointer to current player's data
@param crdsIG Pointer to game cards data
@param selectedCard Which player card to swap (0 or 1)
@param playerTurn Current player indicator (0 or 1)
@param round Current round number
@return Returns no data type (void)
*/
void switchCard(player *currentPlayer, dataCrdsIG *crdsIG, int selectedCard, int playerTurn, int round)
{
    str30 usedCard;
    str30 visualUsedCard[TEXT_FILE_ROWS];
    int i;

    strcpy(usedCard, currentPlayer->card.playercard[selectedCard]);

    if (playerTurn == 0) // Blue's turn
    {
        // Blue takes the right card, puts used card to the left
        strcpy(currentPlayer->card.playercard[selectedCard], crdsIG->rCardName);
        strcpy(crdsIG->lCardName, usedCard);
        strcpy(crdsIG->rCardName, "None");

        for(i = 0; i < TEXT_FILE_ROWS; i++)
        {
            strcpy(visualUsedCard[i], currentPlayer->card.visualCard[selectedCard][i]);
            strcpy(currentPlayer->card.visualCard[selectedCard][i], crdsIG->rCardDisp[i]);
            strcpy(crdsIG->lCardDisp[i], visualUsedCard[i]);
            strcpy(crdsIG->rCardDisp[i], ".....");
        }
    }
    else if (playerTurn == 1 && round == 1) // Red's first move
    {
        // Red takes the right card, puts used card back to right
        strcpy(currentPlayer->card.playercard[selectedCard], crdsIG->rCardName);
        strcpy(crdsIG->rCardName, usedCard);

        for(i = 0; i < TEXT_FILE_ROWS; i++)
        {
            strcpy(visualUsedCard[i], currentPlayer->card.visualCard[selectedCard][i]);
            strcpy(currentPlayer->card.visualCard[selectedCard][i], crdsIG->rCardDisp[i]);
            strcpy(crdsIG->rCardDisp[i], visualUsedCard[i]);
        }
    }
    else if (playerTurn == 1) // Red's later turns
    {
        // Red takes the left card, puts used card to the right
        strcpy(currentPlayer->card.playercard[selectedCard], crdsIG->lCardName);
        strcpy(crdsIG->rCardName, usedCard);
        strcpy(crdsIG->lCardName, "None");
        for(i = 0; i < TEXT_FILE_ROWS; i++)
        {
            strcpy(visualUsedCard[i], currentPlayer->card.visualCard[selectedCard][i]);
            strcpy(currentPlayer->card.visualCard[selectedCard][i], crdsIG->lCardDisp[i]);
            strcpy(crdsIG->rCardDisp[i], visualUsedCard[i]);
            strcpy(crdsIG->lCardDisp[i], ".....");
        }
    }
}

/*
Manages a single player's turn including piece selection and movement

Precondition:
    - All player structures must be initialized
    - Board must be properly populated
    - crdsIG must contain current game cards
    - playerTurn must be 0 or 1
    - round must be >= 1

@param p1 Pointer to Player 1's data
@param p2 Pointer to Player 2's data
@param board The game board
@param crdsIG Pointer to game cards data
@param playerTurn Pointer to current player indicator
@param round Pointer to current round number
@return Returns no data type (void)
*/
void gameTurn(player *p1, player *p2, oniBrd board, dataCrdsIG *crdsIG, int *playerTurn, int *round,
              MoveRecord* replayMoves, int* moveCount)
{
    int nArrkey;
    int nFlagPiece = 0;
    int moveValid = 0;
    int fromRow = 0, fromCol = 0;
    int toRow = 0 , toCol = 0;
    int selectedCard = 0;
    int noValidMoveFlag = 0;
    player *currentPlayer;


    if (*playerTurn == 0)
        currentPlayer = p1;
       else
         currentPlayer = p2;

    if (!hasAnyValidMove(currentPlayer, board, *playerTurn)) //Check if the player has any valid moves at all
    {
        noValidMoveFlag = 1;
        do //card selection phase to swap
        {
            displayBoard(board, p1, p2, crdsIG, 0, 0, 0);
            printf("\x1b[31m" "\nPlayer %s has no valid moves this turn.\n" "\x1b[0m", currentPlayer->name);
            printf("You must still place one of your move cards on the side and pick up the one from the center.\n");
            printf("Choose card to place (LEFT/RIGHT), then press ENTER.\n");
            printf("Selected Card: %s\n", currentPlayer->card.playercard[selectedCard]);

            nArrkey = getch();
            if (nArrkey == 0 || nArrkey == 224)
            {
                nArrkey = getch();
                if (nArrkey == KEY_LEFT)
                    selectedCard = 0;
                else if (nArrkey == KEY_RIGHT)
                    selectedCard = 1;
            }
        } while (nArrkey != KEY_ENTER);

        switchCard(currentPlayer, crdsIG, selectedCard, *playerTurn, *round);

        if (*playerTurn == 0)
            *playerTurn = 1;
        else
            *playerTurn = 0;

        *round = *round + 1;
        currentPlayer->turns++;
    }

    if(!noValidMoveFlag) // card selection phase if has a valid move
    {
        selectedCard = 0;
        do
        {
            displayBoard(board, p1, p2, crdsIG, fromRow, fromCol, nFlagPiece);
            printf("\n%s's turn\n", currentPlayer->name);
            printf("Choose your move card using LEFT/RIGHT arrow. Press ENTER to confirm.\n");
            printf("Selected Card: %s\n", currentPlayer->card.playercard[selectedCard]);

            nArrkey = getch();
            if (nArrkey == 0 || nArrkey == 224)
            {
                nArrkey = getch();
                if (nArrkey == KEY_LEFT)
                    selectedCard = 0;
                else if (nArrkey == KEY_RIGHT)
                    selectedCard = 1;

            }
        } while (nArrkey != KEY_ENTER);

        strcpy(currentPlayer->pSelectedCard, currentPlayer->card.playercard[selectedCard]);

        if(!noValidMoveFlag)
        {
            do //position selection piece
            {
                nFlagPiece = 1;
                displayBoard(board, p1, p2, crdsIG, fromRow, fromCol, nFlagPiece);
                printf("\n%s's turn\n", currentPlayer->name);
                printf("Select your piece (arrow keys). Press ENTER to confirm.\n");
                printf("Current Card: %s\n", currentPlayer->pSelectedCard);
                if(!isValidpiece(currentPlayer, board, fromRow, fromCol, *playerTurn))
                {
                    printf("\x1b[31m" "Not a valid Piece!\n" "\x1b[0m");
                }

                nArrkey = getch();
                if (nArrkey == 0 || nArrkey == 224)
                {
                        nArrkey = getch();
                    if (nArrkey == KEY_UP)
                        fromRow = (fromRow - 1 + ROWS) % ROWS;
                    else if (nArrkey == KEY_DOWN)
                        fromRow = (fromRow + 1) % ROWS;
                    else if (nArrkey == KEY_LEFT)
                        fromCol = (fromCol - 1 + COLS) % COLS;
                    else if (nArrkey == KEY_RIGHT)
                        fromCol = (fromCol + 1) % COLS;
                }
            } while (nArrkey != KEY_ENTER || !isValidpiece(currentPlayer, board, fromRow, fromCol, *playerTurn));

            strcpy(currentPlayer->pPiece.selectedPiece, board[fromRow][fromCol]);
            toRow = fromRow;
            toCol = fromCol;

            if (!hasValidMoveForPiece(currentPlayer, board, fromRow, fromCol, *playerTurn, selectedCard)) //Check if the player has any valid moves for the selected piece
            {
                printf("\x1b[31m" "\nPlayer %s has no valid moves for the selected card.\n" "\x1b[0m", currentPlayer->name);
                printf("Selected Card: %s\n", currentPlayer->card.playercard[selectedCard]);
                printf("Press any key to Continue...\n");
                getch();
                selectedCard = !selectedCard;
                strcpy(currentPlayer->pSelectedCard, currentPlayer->card.playercard[selectedCard]);

                if (!hasValidMoveForPiece(currentPlayer, board, fromRow, fromCol, *playerTurn, selectedCard)) //Check if the player has any valid moves for the selected piece
                {
                    noValidMoveFlag = 1;
                    do
                    {
                        displayBoard(board, p1, p2, crdsIG, 0, 0, 0);
                        printf("\x1b[31m" "\nPlayer %s has no valid moves for the current selected piece.\n" "\x1b[0m", currentPlayer->name);
                        printf("You must still place one of your move cards on the side and pick up the one from the center.\n");
                        printf("Choose card to place (LEFT/RIGHT), then press ENTER.\n");
                        printf("Selected Card: %s\n", currentPlayer->card.playercard[selectedCard]);

                        nArrkey = getch();
                        if (nArrkey == 0 || nArrkey == 224)
                        {
                            nArrkey = getch();
                            if (nArrkey == KEY_LEFT)
                                selectedCard = 0;
                            else if (nArrkey == KEY_RIGHT)
                                selectedCard = 1;
                        }
                    } while (nArrkey != KEY_ENTER);

                    switchCard(currentPlayer, crdsIG, selectedCard, *playerTurn, *round);

                    if (*playerTurn == 0)
                        *playerTurn = 1;
                    else
                        *playerTurn = 0;

                    *round = *round + 1;
                    currentPlayer->turns++;
                }
            }
            if(!noValidMoveFlag)
            {
                do
                {
                    displayBoard(board, p1, p2, crdsIG, toRow, toCol, nFlagPiece);
                    printf("\n%s's turn\n", currentPlayer->name);
                    printf("Select destination (arrow keys). Press ENTER to confirm.\n");
                    printf("Moving with: %s\n", currentPlayer->pSelectedCard);
                    printf("Selected Piece: %s\n", currentPlayer->pPiece.selectedPiece);
                    if(!isValidMove(currentPlayer, board, fromRow, fromCol, toRow, toCol, *playerTurn, selectedCard))
                        printf("Not a valid Move!\n");

                    else if(!isValidMove(currentPlayer, board, fromRow, fromCol, toRow, toCol, *playerTurn, selectedCard))
                        printf("\x1b[33m" "No Valid Move for Player %s\n" "\x1b[0m", currentPlayer->name);

                    nArrkey = getch();
                    if (nArrkey == 0 || nArrkey == 224)
                    {
                        nArrkey = getch();
                        if (nArrkey == KEY_UP)
                            toRow = (toRow - 1 + ROWS) % ROWS;
                        else if (nArrkey == KEY_DOWN)
                            toRow = (toRow + 1) % ROWS;
                        else if (nArrkey == KEY_LEFT)
                            toCol = (toCol - 1 + COLS) % COLS;
                        else if (nArrkey == KEY_RIGHT)
                            toCol = (toCol + 1) % COLS;
                    }
                } while (nArrkey != KEY_ENTER || !isValidMove(currentPlayer, board, fromRow, fromCol, toRow, toCol, *playerTurn, selectedCard));

                moveValid = isValidMove(currentPlayer, board, fromRow, fromCol, toRow, toCol, *playerTurn, selectedCard);

                if (moveValid)
                {
                    strcpy(currentPlayer->pPiece.selectedPiece, board[fromRow][fromCol]); //Update board
                    strcpy(board[toRow][toCol], currentPlayer->pPiece.selectedPiece);
                    strcpy(board[fromRow][fromCol], "| _ |");

                    switchCard(currentPlayer, crdsIG, selectedCard, *playerTurn, *round); // Switch Card position
                    // Record the move
                    recordMove(replayMoves, moveCount, *playerTurn + 1, makePosition(fromCol, fromRow), makePosition(toCol, toRow), currentPlayer->pSelectedCard);

                    if (*playerTurn == 0) //Swap Turns
                        *playerTurn = 1;
                    else
                        *playerTurn = 0;

                    (*round)++;
                    currentPlayer->turns++;

                }
            }
            else
                printf("\x1b[31m" "Invalid move! Try again.\n" "\x1b[0m"); //Invalid Move
        }
    }
}

/*
Sets up the game by shuffling cards, assigning them to players, and reading player names

Precondition:
    - moveDeck must contain at least MAX_CARD_PER_GAME valid cards
    - Player structures must be uninitialized
    - crdsIG must be uninitialized

@param moveDeck Array of card names to use
@param cardCount Number of cards in moveDeck
@param p1 Pointer to Player 1's data
@param p2 Pointer to Player 2's data
@param crdsIG Pointer to game cards data
@return Returns no data type (void)
*/
void initializeGame(str30 moveDeck[], int cardCount, player *p1, player *p2, dataCrdsIG *crdsIG)
{
    int i, isValid;

    system("CLS");

    printf("\x1b[33m" "These are the card before shuffling!\n\n" "\x1b[0m");
    for (i = 0; i < cardCount; i++)
        printf("\x1b[36m" "%s\n\n" "\x1b[0m", moveDeck[i]);
    printf("Press any key to continue!\n");
    getch();

    system("CLS");
    shuffleCards(moveDeck, cardCount); // Shuffle Cards
    assignMoveCards(moveDeck, p1, p2, crdsIG); // Assign After Shuffling

    printf("\x1b[33m" "These Are The Shuffled Cards!\n\n" "\x1b[0m");
    for (i = 0; i < cardCount; i++)
        printf("\x1b[36m" "%s\n\n" "\x1b[0m", moveDeck[i]);
    printf("Press any key to continue!\n");
    getch();

    p1->turns = 0;
    p2->turns = 0;

    system("CLS");
    printf("\x1b[36m" "--- Onitama ---\n\n" "\x1b[0m");

    /* Get player names */
    do
    {
        isValid = 1;
        printf("\x1b[33m" "Enter" "\x1b[0m" "\x1b[34m" " Player 1 " "\x1b[0m" "\x1b[33m" "Name: " "\x1b[0m");
        scanf("%s", p1->name);
        for (i = 0; i < strlen(p1->name); i++)
        {
            if (!isAlpha(p1->name[i]))
            {
                printf("\x1b[31m" "Invalid Name!\n" "\x1b[0m");
                isValid = 0;
            }
        }
    } while (!isValid);

    do
    {
        isValid = 1;
        printf("\x1b[33m" "Enter" "\x1b[0m"  "\x1b[31m" " Player 2 " "\x1b[0m" "\x1b[33m" "Name: " "\x1b[0m");
        scanf("%s", p2->name);
        for (i = 0; i < strlen(p2->name); i++)
        {
            if (!isAlpha(p2->name[i]))
            {
                printf("\x1b[31m" "Invalid Name!\n" "\x1b[0m");
                isValid = 0;
            }
        }
    } while (!isValid);
}
/*
Initializes and runs the main game sequence

Precondition:
    - Board must be properly allocated
    - moveDeck must contain valid card names
    - cardCount must be > 0 and <= MAX_CARDS

@param board The game board array
@param moveDeck Array containing all available move cards
@param cardCount Number of cards in moveDeck
@return Returns no data type (void)
*/
void playGame(oniBrd board, str30 moveDeck[], int cardCount) 
{
    // All variables declared at top
    player p1, p2;
    dataCrdsIG crdsIG;
    int playerTurn = 0;
    int gameOver = 0;
    int round = 1;
    MoveRecord replayMoves[MAX_MOVES];
    int moveCount = 0;
    char choice;
    
    initializeGame(moveDeck, cardCount, &p1, &p2, &crdsIG);
    populatePlayerPiece(&p1, &p2);
    populateBoard(board, &p1, &p2);
    playerTurn = getFirstTurn(crdsIG);

    while (!gameOver) 
    {
        gameTurn(&p1, &p2, board, &crdsIG, &playerTurn, &round, replayMoves, &moveCount);
        gameOver = checkWinCondition(board, &p1, &p2);
    } 

    if (gameOver == 1) 
    {
        Sleep(1000);
        printf("\nCongratulations %s! You won!\n", p1.name);
        updateHoF(p1);
        Sleep(1000);
    }
    else 
    {
        Sleep(1000);
        printf("\nCongratulations %s! You won!\n", p2.name);
        updateHoF(p2);
        Sleep(1000);
    }

    do 
    {
        printf("Would you like to replay the game? (y/n): ");
        scanf(" %c", &choice);
    } while (!(choice == 'y' || choice == 'Y' ||choice == 'n' || choice == 'N'));

    if(choice == 'y' || choice == 'Y')
        replayMovesSequence(replayMoves, moveCount);

    printf("Press any key to return to main menu...");
    getch();    
}

/*
Main game menu system for Onitama

Precondition: 
    - All necessary header files are included
    - Game constants are properly defined
    - Data structures are correctly initialized
    - Move cards file exists and is properly formatted

@return Returns 0 on successful program execution
*/
int main()
{

    srand(time(NULL)); // Seed Random Gen else if this didnt exist it will still
                       // generate random number but once exiting the code the random num generated would still be the same
    str30 moveDeck[MAX_CARDS]; // Move deck cards
    int cardCount = 0; // count of cards
    oniBrd board;
    int nArrKeys;
    int nSelected = 0;
    int i;
    int ok;
    str30 sMenu[MAX_MENU_SELECTION] =
    {
        "Start Game", "Hall of Fame", "How to Play", "Quit"
    };

    ok = readMoveCards(moveDeck, &cardCount);

    if(ok)
    {
        do
        {
            system("CLS");
            printf("\n\t\t+===========================+\n");
            printf("\t\t|       " "\x1b[36m" "ONITAMA GAME" "\x1b[0m" "        |\n");
            printf("\t\t+===========================+\n\n");
            printf("\t\t   Use " "\x1b[32m" "ARROWKEYS" "\x1b[0m" " to move, " "\x1b[32m" "Enter" "\x1b[0m" " to select.\n\n");

            for (i = 0; i < MAX_MENU_SELECTION; i++)
            {
                if (i == nSelected)
                    printf("\t\t " "\x1b[35m" "-> %s <-" "\x1b[0m" "\n", sMenu[i]);
                else
                    printf("\t\t    %s\n", sMenu[i]);
            }

            nArrKeys = getch();
            if (nArrKeys == 0 || nArrKeys == 224)
            {
                nArrKeys = getch();
                if (nArrKeys == KEY_UP)
                    nSelected = (nSelected - 1 + MAX_MENU_SELECTION) % MAX_MENU_SELECTION;
                else if (nArrKeys == KEY_DOWN)
                    nSelected = (nSelected + 1) % MAX_MENU_SELECTION;
            }
            else if (nArrKeys == KEY_ENTER)
            {
                if(nSelected == 0)
                    playGame(board, moveDeck, cardCount);
                else if(nSelected == 1)
                    displayHoF();
                else if(nSelected == 2)
                    howToPlay();
            }
        } while (nArrKeys != KEY_ENTER || nSelected != 3);
    }
    printf("\n");

    return 0;
}

/* This is to certify that this project is my/our own work, based on my/our personal efforts in studying and applying
the concepts learned. We have constructed the functions and their respective algorithms and corresponding codes
either individually or with my group mate. The program was run, tested, and debugged by my/our own efforts.
I/We further certify that I/we have not copied in part or whole or otherwise plagiarized the work of other
students/groups and/or persons.

John Benedict G. Santos
DLSU ID 12486566 */
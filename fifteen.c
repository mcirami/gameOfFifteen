/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep for animation's sake
        usleep(500000);
    }

    // that's all folks
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init(void)
{
    int k = d*d - 1;
    int x = k;
    
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++, k--)
        {
            if(k == 0)
            {
                board[i][j] = (char) 32;
            }
            else
            {
                board[i][j] = k;
            }
        }
    }
    if(x % 2)
    {
        int temp = board[d - 1][d - 2];
        board[d - 1][d - 2] = board[d - 1][d - 3];
        board[d - 1][d - 3] = temp;
    }
    
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if(board[i][j] == (char) 32)
            {
                printf("%c", (char) 32);
                printf("  ");
            }
            else
            {
                if(board[i][j] < 10)
                    printf("  ");
                else
                    printf(" ");
                    
                printf("%d", board[i][j]);
            }
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    bool legal;
    bool blank;
    int x;
    int y;
    
    // search for blank tile
    for(x = 0; x < d; x++)
    {
        for(y = 0; y < d; y++)
        {
            if(board[x][y] == (char) 32)
            {
                blank = true;
                break;
            }
        }
        
        if(blank)
        {
            break;
        }
    }
    
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if(board[i][j] == tile)
            {
                // check if there's a space next to tile to be move
                if(board[i][j + 1] == (char) 32 || board[i][j - 1] == (char) 32 || board[i + 1][j] == (char) 32 || board[i - 1][j] == (char) 32)
                {
                    int temp = board[i][j];
                    board[i][j] = board[x][y];
                    board[x][y] = temp;
                    legal = true;
                    break;
                }
                else
                {
                    legal = false;
                    i = j = d;
                    break;
                }
            }
        }
        if(legal)
        {
            break;
        }
    }
    
    return legal;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    bool win;
    int x = d*d - 1;
    int k = x;
    int counter = 0;
    
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++, k++)
        {
            if(board[i][j] == (k % x) + 1)
            {
                counter++;
                
                if(counter == x)
                {
                    win = true;
                    i = j = d;
                    break;
                } 
            }
            else
            {
                win = false;
                i = j = d;
                break;
            }
        }   
    }
    
    return win;
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}

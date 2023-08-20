// name: Alexander he
// unikey: alhe6638
// SID: 520432093


// Note: I had a few "magic numbers" persay, but I commented them

#include <stdio.h> 
#include <string.h> 
#include <ctype.h>

#define maxMoves 361
#define boardSize 19

enum Player{
    P1, P2
};

void endMsg(char player);

int checkGame(int gameRound);

int min(int a, int b);

int power(int base, int exp);

int getPos(char arr[], int arrLen, char end, int start);

int trailingSpaceCheck(char arr[], int spacePos, int nullTermPos);

int checkInput(char inputPart[], int inputPartLen, char* accepted[], int acceptedLen);

int stringCompare(char a[], int aLen, char b[], int bLen);

void fill(char a[], int aLen, char c);

int countSpaces(char a[], int aLen);

int getLen(char a[], int aMax);

int extractNumber(char param[], int paramSize, int coordLength);

int charToInt(char c);

char getPlayer(int p);

int checkCoords(int row, int col);

int linearSearch(int mode, char board[boardSize][boardSize], int start[2], char symbol);

void areaSearch(int * surroundings, int move[2], int player, char playerSymbol, char board[boardSize][boardSize]); 

int sum5(int * sur);

int checkWin(char board[boardSize][boardSize], int player, char playerSymbol,int move[2]);

void relocateMist(int *currentCoords, int stoneCoords[]);

void addToHistory(char hist[maxMoves][4], char toAdd[], int turn);

void printHistory(char hist[][4], int histSize); 

int view(char board[boardSize][boardSize], int mistCoords[]);

int clearBuff(char initialInput[], int initialInputLength);

// * Start
int main(int argc, char const *argv[])
{
    // Control variables 
    int gameRound = 0; // Starting round 0
    int mistCoords[] = {10,10}; // Starting coordinates
    enum Player currentPlayer = P1; 

    // history array: 
    char hist[maxMoves][4]; // 4 is used as that is essentially the largest string possible
    // String used to record the last entered coordinate
    //enum acceptedCommands userInput;

    // Board
    char board[boardSize][boardSize]; // Input is column, row, array is indexed row, column 
    for (int col = 0; col < boardSize; col++)
    {
        fill(board[col], boardSize, '.');
    }
    
    // Inputs 
    
    // Set buffer size to 15 for extra security
    const int initialInputLen = 15;

    // Array of acceptable "command strings"
    const int commandNum = 6; // 6 acceptable commands 
    char * acceptedCommands[] = {"who", "term", "resign", "view", "place", "history"}; 
    
    while (checkGame(gameRound)){

        char initial[15]; // Corresponds to the initialInputLen
        
        fill(initial, initialInputLen, '\0');

        // ! EDITED AFTER LECTURE
        if(! feof(stdin)){
            if (fgets(initial, initialInputLen, stdin) == NULL){
                return 1;
            }
        }
        
        int inputLen = getLen(initial, initialInputLen); 

        // String with potentially two parts: <command> <Optional: Coords> 
        
        // Command part: History command is 8 chars long (9 if counting null term, and 10 if counting newline). 
        // Set at 10 to be safe 
        const int part1Size = 10;
        char part1[10]; 
        
        // Coords: The longest acceptable coordinate is "SboardSize\0"
        // Exclude the newline from this consideration. 
        const int part2Size = 4;
        char part2[4];
        fill(part1, part1Size, '\0');
        fill(part2, part2Size, '\0');
        
        int spaceDetected = getPos(initial, initialInputLen, ' ', 0); 
        //! -1 if no spaces detected, any other number means that is the position of the space
        int termDetected = getPos(initial, initialInputLen, '\0', 0);
        // ! -1 if no nullTerm, any other number means that is the position of the newline 
        int newLinePos = getPos(initial, initialInputLen, '\n', 0);
        // ! -1 if no newline, any other position indicates the position of the newline
        
        int i;
        
        
        for (i = 0; i < initialInputLen; i++)
        {
            // Break up inputs into command and parameter 
            if(i == spaceDetected || i == termDetected || i == newLinePos || i >= part1Size-1){
                part1[i] = '\0';
                break;
            }
            part1[i] = initial[i];
            
        }
        // Return the detected message as an enumeration, -1 if unknown message
        int msg = checkInput(part1, part1Size, acceptedCommands, commandNum);
        
        // Error checking (whitespaces, captitalisation, etc)
        
        // If the message is unknown or has whitespaces (that aren't place)
        //int flag = 0;
        // ? Combine the if and elif later

        if((msg != 4 && spaceDetected >= 0) || msg < 0){
            //printf("Invalid 1: %d!\n", msg);
            clearBuff(initial, initialInputLen);
            printf("Invalid!\n");
        }

        else if (msg == 4 && (spaceDetected != 5 || countSpaces(initial, inputLen) != 1)){
            //printf("Invalid 2!\n");
            clearBuff(initial, initialInputLen);
            printf("Invalid!\n");
        }
        
        else{
            // ? Command has passed error checking for spaces. Can be everything but place: 
            switch (msg)
            {
            case 0: // who 
                printf("%c\n", getPlayer(currentPlayer));
                break;

            case 1: // term 
                return 1; 
                break; 

            case 2: // ! resign
                endMsg(getPlayer((currentPlayer + 1) % 2)); // Rotate player
                printHistory(hist, gameRound); 
                printf("Thank you for playing!\n");
                return 0;
                break;

            case 3: // ! view 
                //printBoard(board);
                view(board, mistCoords);
                break; 

            case 4: // ! place 
                i++; // skip the space 
                int validCoordinate = 1; // Boolean representation
                // Get "coordinate parameter"

                // The 7th index of a place command should ALWAYS be a number
                if (isalpha(initial[7]) || !isdigit(initial[7])){
                    if(initial[8] == '\0'){ 
                        // Due to the fact that the initial array was filled with '\0' 
                        // This can be searched for to determine invalidity.
                        clearBuff(initial, initialInputLen);
                        if(initial[7] == '\0' && initial[8] =='\0'){
                            // An invalid term will have these properties at position 7 and 8
                            // Always defined as they were filled with '\0' initially 
                            printf("Invalid!\n");
                        }
                        else{
                            printf("Invalid coordinate\n");
                        }
                        break;
                    }
                    validCoordinate = 0; 
                }

                else{
                    for(int j = 0; j < initialInputLen; j++){
                        //printf("J: %d\n", j);
                        if(initial[i] == '\0' || initial[i] == '\n' || j == part2Size-1){
                            //printf("i: %c, j: %c\n", initial[i], j);
                            if(j <= 1 || !(initial[i] == '\n' || initial[i] == '\0')){
                                validCoordinate = 0;
                            }
                            break; 
                        }
                        else{
                            part2[j] = initial[i++];
                        }
                    }
                }

                // ! Check if Letter is in the right range, number is in the right range
                int num = 0; 
                
                int lenOfCoord = getLen(part2, part2Size) - 2; // Remove null term  + letter

                num = extractNumber(part2, part2Size, lenOfCoord);
                
                if(part2[0] < 'A' || part2[0] > 'S' || num <= 0 || num  > boardSize || !validCoordinate || part2[1] == '0'){
                    if (clearBuff(initial, initialInputLen) >= 1){
                        printf("Invalid!\n");
                    }
                    else{
                        printf("Invalid coordinate\n");
                    }
                    // = 1;
                    break;
                }

                // ! Testing by modifying game board

                int currentMove[] = {num - 1, charToInt(part2[0] - 1)};

                if (board[num - 1][charToInt(part2[0]) - 1] != '.'){
                    clearBuff(initial, initialInputLen);
                    printf("Occupied coordinate\n"); 
                    break;
                }   
                
                char playerSymbol = '#'; 

                if(getPlayer(currentPlayer) == 'B'){
                    playerSymbol = '#';
                    board[num - 1][charToInt(part2[0]) - 1] = playerSymbol;
                }
                else if (getPlayer(currentPlayer) == 'W'){
                    playerSymbol = 'o';
                    board[num- 1][charToInt(part2[0]) - 1] = playerSymbol;
                }
                addToHistory(hist, part2, gameRound++);

                if(checkWin(board, currentPlayer, playerSymbol, currentMove)){
                    endMsg(getPlayer(currentPlayer));
                    printHistory(hist, gameRound); 
                    printf("Thank you for playing!\n");
                    return 0; 
                }

                currentPlayer = (currentPlayer + 1) % 2;
                // Rescale back to "alpha coordinate system" 
                currentMove[0] = currentMove[0] + 1;
                currentMove[1] = currentMove[1] + 1; 
                relocateMist(&mistCoords[0], currentMove);
                break;   

            case 5: // ! history 
                printHistory(hist, gameRound);
                break;  
            }
            //printf("Flushing 1\n");
            fflush(stdin);
            
        }
        // Prevents carryover from long entries like "whowhowhowhowhowhowhowho"
    }
    // Tie ending condition (While loop actually exits)
    printHistory(hist, gameRound); 
    printf("Thank you for playing!\n");
    return 0;
}

int checkInput(char inputPart[], int inputPartLen, char* accepted[], int acceptedLen){
    // Checks whether the input from the user is one of the accepted commands. 
    // Returns an enumeration of the command if so. Else, -1 is returned
    for (int i = 0; i < acceptedLen; i++)
    {
        //printf("Expected: %s, Got: %s, Equiv: %d\n", *(accepted + i), 
        //inputPart, stringCompare(inputPart, inputPartLen, *(accepted + i), strlen(*(accepted + i))));
        if(stringCompare(inputPart, inputPartLen, *(accepted + i), strlen(*(accepted + i)))){
            return i;
        }
    }
    return -1; 
}

int checkGame(int gameRound){
    // Check if a player has won
    if(gameRound >= maxMoves){
        
        endMsg('T');
        return 0;
    }
    return 1; 
}

void endMsg(char player){
    /*
    'B' -> black wins 
    'W' -> White wins 
    'T' -> Tie

    */
    switch (player)
    {
    case 'B':
        printf("Black wins!\n"); 
        break;
    case 'W':
        printf("White wins!\n"); 
        break; 
    case 'T':
        printf("Wow, a tie!\n");     
    default:
        break;
    }
}

int min(int a, int b){
    // Get the minimum value (math.h doesn't work for some reason)
    if(a > b){
        return b;
    }
    else{
        return a;
    }
}

int power(int base, int exp){
    for (int i = 1; i < exp; i++)
    {
        base = base * base; 
    }
    if(exp == 0){
        return 1;
    }
    return base; 
}

int getPos(char arr[], int arrLen, char end, int start){
    // Returns first position of the character instance 'end' 
    // Use to find the first insance of a whitespace, null term, or newline
    int counter = 0;  
    int i = start;
    while(arr[i++] != end){
        counter++; 
        if(i >= arrLen){
            return -1; // Character is not in the array
        }
    }
    return counter; 
}

int trailingSpaceCheck(char arr[], int spacePos, int nullTermPos){
    // Checks if a trailing space exists. No trailing spaces are allowed
    if(arr[nullTermPos - 1] == ' '){
        return 1;
    }
    if(spacePos == -1){
        return 0;
    }
    if(spacePos + 1 == nullTermPos){
        return 1; 
    }
    return 0; 
    
}

int stringCompare(char a[], int aLen, char b[], int bLen){
    // Compares two strings to see if they are identical characterwise 
    for (int i = 0; i < min(aLen, bLen)+1; i++){
        if((a[i] == '\0' && b[i] != '\0') || (a[i] != '\0' && b[i] == '\0')){
            //printf("Here\n");
            return 0;
        }

        if (a[i] != b[i])
        {
            return 0; 
        }
    }
    return 1;
}

void fill(char a[], int aLen, char c){
    // Fills an array up with specific chars for security
    for (int i = 0; i < aLen; i++)
    {
        a[i] = c;
    }
}

int countSpaces(char a[], int aLen){
    // Counts the number of whitspaces in a character array
    int counter = 0; 
    for (int i = 0; i < aLen; i++)
    {
        if(a[i] == ' '){
            counter ++; 
        }
    }
    return counter; 
}

int getLen(char a[], int aMax){
    // Gets the length of a string in a buffer up to 1 + the first null terminator
    int len = 0; 
    for (int i = 0; i < aMax; i++)
    {
        if(a[i] == '\0'){
            return len + 1; 
        }
        len++; 
    }
    return -1;
}

int charToInt(char c){
    return c - 'A' + 1;
}

int extractNumber(char param[], int paramSize, int coordLength){
    // Extract Number
    int num = 0; 
    for (int i = 1; i < 4; i++){
        if(param[i] != '\0' && i == 3){
            return -1;
        }
        else if (param[i] == '\0'){
            break; 
        }
        num += (param[i] - '0') * power(10, coordLength - i);
    }
    return num;
}

void relocateMist(int *currentCoords, int stoneCoords[]){
    int newX = 1 + (5 * stoneCoords[1] * stoneCoords[1] + 3 * stoneCoords[1] + 4) % 19;
    int newY = 1 + (4 * stoneCoords[0] * stoneCoords[0] + 2 * stoneCoords[0] - 4) % 19;

    *(currentCoords) = newX;
    *(currentCoords + 1) = newY; 
}

char getPlayer(int p){
    switch (p)
    {
    case P1:
        return 'B';
    case P2: 
        return 'W';
    }
    return '\0';
}

int checkCoords(int row, int col){
    if(row < 0 || row > 18 || col < 0 || col > 18){
        return 0; 
    }
    return 1;
}

int linearSearch(int mode, char board[boardSize][boardSize], int start[2], char symbol){
    // Repeats 3 times to check for a 5 in a row 
    int consecSymbols = 0; 
    const int count = 5; 
    switch (mode)
    {
    case 0:
        for (int i = 2; i < count; i++) // Checks through each coordinate (hence 2)
        {
            if(checkCoords(start[0] - i, start[1] - i)){
                char x = board[start[0]-i][start[1] - i];
                if(x == symbol){
                    consecSymbols++; 
                }
                else{
                    break;
                }
            }
            else{
                break; 
            }
        }
        
        break;

    case 1:
        for (int i = 2; i < count; i++)
        {
            if(checkCoords(start[0] - i, start[1])){
                char x = board[start[0]-i][start[1]];
                if(x == symbol){
                    consecSymbols++; 
                }
                else{
                    break;
                }
            }
            else{
                break; 
            }
        }
        
        break;

    case 2:
        for (int i = 2; i < count; i++)
        {
            if(checkCoords(start[0] - i, start[1] + i)){
                char x = board[start[0] - i][start[1] + i];
                if(x == symbol){
                    consecSymbols++; 
                }
                else{
                    break;
                }
            }
            else{
                break; 
            }
        }
        break;   

    case 3:
        for (int i = 2; i < count; i++)
        {
            if(checkCoords(start[0], start[1] - i)){
                char x = board[start[0]][start[1] - i];
                if(x == symbol){
                    consecSymbols++; 
                }
                else{
                    break;
                }
            }
            else{
                break; 
            }
        }
        break;

    case 4:
        return 1;
        break;

    case 5:
        for (int i = 2; i < count; i++)
        {
            if(checkCoords(start[0], start[1] + i)){
                char x = board[start[0]][start[1] + i];
                if(x == symbol){
                    consecSymbols++; 
                }
                else{
                    break;
                }
            }
            else{
                break; 
            }
        }
        break;

    case 6:
        for (int i = 2; i < count; i++)
        {
            if(checkCoords(start[0] + i, start[1] - i)){
                char x = board[start[0]+i][start[1] - i];
                if(x == symbol){
                    consecSymbols++; 
                }
                else{
                    break;
                }
            }
            else{
                break; 
            }
        }
        break;

    case 7:
        for (int i = 2; i < count; i++)
        {
            if(checkCoords(start[0] + i, start[1])){
                char x = board[start[0]+i][start[1]];
                if(x == symbol){
                    consecSymbols++; 
                }
                else{
                    break;
                }
            }
            else{
                break; 
            }
        }
        break;

    case 8:
        for (int i = 2; i < count; i++)
        {
            if(checkCoords(start[0] + i, start[1] + i)){
                char x = board[start[0]+i][start[1] + i];
                if(x == symbol){
                    consecSymbols++; 
                }
                else{
                    break;
                }
            }
            else{
                break; 
            }
        }
        break;
    }
    return consecSymbols; 
}

void areaSearch(int * surroundings, int move[2], int player, char playerSymbol, char board[boardSize][boardSize]){
    //functions based off the assumption that:
    /*
    Iterating through a "2d representation" of the board with i and j
    -1 0   1
    -1 Pos 1
    -1 0   1
    */
    int counter = 0; 
    
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if(i == 0 && j == 0){
                counter++;
                continue; 
            }
            if(checkCoords(move[0] + i, move[1] + j)){
                if(playerSymbol == board[move[0] + i][move[1] + j]){
                    *(surroundings + counter) = 1;
                }
                else{
                    *(surroundings + counter) = 0;
                }
            }
            else{
                *(surroundings + counter) = 0; 
            }
            counter++; 
        }
    }
}

int sum5(int * sur){
    // Checks summations in different directions 
    int r2 = 0, c2 = 0,  d1 = 0, d2 = 0;
    r2 = sur[3] + sur[4] + sur[5];
    c2 = sur[1] + sur[4] + sur[7];
    d1 = sur[0] + sur[4] + sur[8]; 
    d2 = sur[2] + sur[4] + sur[6]; 
    return (
        r2 >= 5 || c2 >= 5 || d1 >= 5 || d2 >= 5
    );
    
}

int checkWin(char board[boardSize][boardSize], int player, char playerSymbol,int move[2]){
    int surroundings[] = {
        0, 0, 0,
        0, 1, 0,
        0, 0, 0}; // 9 squares

    // Fill array with 0's 
    areaSearch((int*) surroundings, move, player, playerSymbol, board); // !Possible problem??!?

    for (int i = 0; i < 9; i++)
    {
        if(i == 4){
            continue;
        }
        if(surroundings[i]){
            int save = surroundings[i];
            int val = linearSearch(i, board, move, playerSymbol);
            if(val == 0){
                continue; 
            }
            surroundings[i] = val + save; 
        }
    }
    if(sum5((int * ) surroundings)){
        return 1;
    }

    return 0;
}

void addToHistory(char hist[maxMoves][4], char toAdd[], int turn){
    int pos = turn; 
    int i = 0; 
    while (toAdd[i] != '\0'){
        *(*(hist + pos) + i) = toAdd[i]; 
        i++; 
    }
    *(*(hist + pos) + i) = '\0';
}

void printHistory(char hist[][4], int histSize){
    for (int i = 0; i < histSize; i++)
    {
        printf("%s", hist[i]);
    }
    printf("\n");
}

int view(char board[boardSize][boardSize], int mistCoords[]){
    printf("%c%d,", mistCoords[0] + 'A' - 1, mistCoords[1]);
    // mistCoords[0] is the letter (Col), mistCoords[1] is the number (row)
    int startRow = mistCoords[1] + 3 - 1;
    int startCol = mistCoords[0] - 3 - 1;
    //printf("start spaces (%d, %d)", startRow, startCol);
    // 7 * 7 loops as there are 49 spaces to print
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            if(startRow - i < 0 || startRow - i > 18 || startCol + j < 0 || startCol + j > 18){
                printf("x");
            }
            else{
                printf("%c", board[startRow - i][startCol + j]);
            }
        }
    }
    printf("\n");
    return 0;
}

int clearBuff(char initialInput[], int initialInputLength){
    int spaceCounter = 0;
    int c = initialInput[initialInputLength - 2];
    // The 'initial' string will always end with a null terminator
    // Hence, the - 2. 
    if(c != '\0' && c != '\n'){
        c = getchar();
        while (c != EOF && c != '\n'){
            if(c == ' '){
                spaceCounter++;
            }
            c = getchar();
            //printf("Here c: %c\n", c);
        }
    }

    fflush(stdin);  // Initial approach
    return spaceCounter;
}
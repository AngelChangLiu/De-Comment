#include <stdio.h>
#include <stdlib.h>

/* All state types from dfa */
enum Statetype {NORMAL, BACKSLASH, INCOMMENT, STAR, STRING, 
ESCAPESTRING, CHAR, ESCAPECHAR}; 

/* This code removes comments from a text while preserving the text
that isn't part of a code through the use of a dfa that handles corner 
cases and switches states. If the comment starts but doesn't end, 
an error message is pushed */


/* This function handles the NORMAL state (start) and takes in the 
parameter c and returns the next state of the dfa from the choices 
BACKSLASH, STRING, CHAR, or NORMAL depending on the current parameter 
c */
enum Statetype 
handleNormalState(int c) {   
    enum Statetype state;   
    if (c == '/') {            
        state = BACKSLASH;   
    } 
    else if (c == '\"') {      
        putchar(c);      
        state = STRING;   
    }   
    else if (c == '\'') {
        putchar(c);
        state = CHAR;
    }
    else {
        putchar(c);
        state = NORMAL;
    }

    return state; 
}

/* This function handles the STRING state and takes in the 
parameter c and returns the next state of the dfa from the choices 
ESCAPESTRING, NORMAL, or STRING depending on the current parameter c */
enum Statetype 
handleStringState(int c) {
    enum Statetype state;   
    if (c == '\\') {      
        putchar(c);      
        state = ESCAPESTRING;   
    }
    else if (c == '\"') {      
        putchar(c);      
        state = NORMAL;   
    }   
    else {
        putchar(c);
        state = STRING;
    }
    return state; 
}

/* This function handles the ESCAPESTRING state and takes in the 
parameter c and returns the next state of the dfa which is STRING after
printing out the current c */
enum Statetype 
handleEscapeStringState(int c) {
    enum Statetype state;
    putchar(c);   
    state = STRING;
    return state; 
}

/* This function handles the CHAR state and takes in the 
parameter c and returns the next state of the dfa from the choices 
ESCAPECHAR, NORMAL, or CHAR depending on the current parameter c */
enum Statetype 
handleCharState(int c) {
    enum Statetype state;   
    if (c == '\\') {      
        putchar(c);      
        state = ESCAPECHAR;   
    } 
    else if (c == '\'') {      
        putchar(c);      
        state = NORMAL;   
    } 
    else {
        putchar(c);
        state = CHAR;
    }
    return state; 
}

/* This function handles the ESCAPECHAR state and takes in the 
parameter c and returns the next state of the dfa which is CHAR after
printing out the current c */
enum Statetype 
handleCharEscapeState(int c) {
    enum Statetype state;
    putchar(c);   
    state = CHAR;
    return state; 
}

/* This function handles the BACKSLASH state and takes in the 
parameter c and returns the next state of the dfa from the choices 
ESCAPECHAR, CHAR, STRING, INCOMMENT, OR NORMAL depending on the current 
parameter c */
enum Statetype 
handleBackslashState(int c) {
    enum Statetype state;   
    if (c == '/') {
        putchar('/'); 
        state = BACKSLASH;   
    } 
    else if (c == '\'') {  
        putchar('/');    
        putchar(c);      
        state = CHAR;   
    }   
    else if (c == '\"') {
        putchar('/');
        putchar(c);
        state = STRING;
    }
    else if (c == '*') {
        putchar(' ');
        state = INCOMMENT;
    }
    else {
        putchar('/');
        putchar(c);
        state = NORMAL;
    }

    return state; 
}

/* This function handles the INCOMMENT state and takes in the 
parameter c and returns the next state of the dfa from the choices 
STAR, or INCOMMENT depending on the current parameter c */
enum Statetype 
handleInCommentState(int c) {
    enum Statetype state;   
    if (c == '*') {
        state = STAR;
    }
    else if (c == '\n') {
        putchar(c);
        state = INCOMMENT;
    }
    else {
        state = INCOMMENT;
    }

    return state; 
}

/* This function handles the STAR state and takes in the 
parameter c and returns the next state of the dfa from the choices 
STAR, NORMAL, or INCOMMENT depending on the current parameter c */
enum Statetype 
handleStarState(int c) {
    enum Statetype state;   
    if (c == '*') {
        state = STAR;
    }
    else if (c == '/') {
        state = NORMAL;
    }
    else if (c == '\n') {
        putchar(c);
        state = INCOMMENT;
    }
    else {
        state = INCOMMENT;
    }
    return state; 
}

/* The main function calls the functions above if needed to go through 
the dfa while traversing through each character. If the comment
doesn't end, an error message with the line of the start of the comment
is printed out */
int main(void) {   
    int c; /* Each character */
    int line = 1; /* Line counter */
    int startError = 1;  /* Counter for when a failed comment starts */
    enum Statetype state = NORMAL; /* Initialize to start state */    
    /* Traverses through all chars */
    while ((c = getchar()) != EOF) {      
        switch (state) {         
            case NORMAL:             
                state = handleNormalState(c);            
                break;         
            case BACKSLASH:            
                state = handleBackslashState(c);
                /* Find the start of failed comment */
                if (state == INCOMMENT) {
                    startError = line;
                }           
                break; 
            case INCOMMENT:             
                state = handleInCommentState(c);            
                break;         
            case STAR:            
                state = handleStarState(c);            
                break;
            case STRING:             
                state = handleStringState(c);            
                break;         
            case ESCAPESTRING:            
                state = handleEscapeStringState(c);            
                break;
            case CHAR:             
                state = handleCharState(c);            
                break;         
            case ESCAPECHAR:            
                state = handleCharEscapeState(c);            
                break;                    
            }  
            if (c == '\n') {
                line++;
            }
        
        }
        /* Code for error line using startError */
        if (state == INCOMMENT || state == STAR) {
            fprintf(stderr, "Error: line %d: unterminated comment\n",  
            startError);
            exit(EXIT_FAILURE);
        }
        if (state == BACKSLASH) {
            putchar('/');
        }

    return 0;
}
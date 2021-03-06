// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*--------------------------------------------------------------------------------------------------------
|   KEYBOARD_DRIVER.C    |                                                                               |
|----------------------                                                                                  |
| This library handles the reading of the keyboard, considering its layout and special key combinations. |
--------------------------------------------------------------------------------------------------------*/

#include <interrupts.h>
#include <keyboard_driver.h>
#include <pipe_manager.h>

#define NULL (void *)0;

pipe stdinPipe = NULL;


/* ------------------------------------------------------------------------------------------------------
                                            KEYBOARD DEFINITIONS
------------------------------------------------------------------------------------------------------ */                                            

int read();

#define BUFF_SIZE 1024

// Escape: 1, Backspace: 14, Tab: 15, Enter: 28, Ctrl: 29, LShift: 42/170, RShift: 54/182, Alt: 56, Space: 57,
// F1-F10: 59-68, Cmd: 91
// Keys represented with a 0 are not ASCII representable

char keyboard_map[64] = {
    0,27,'1','2','3','4','5','6','7','8','9','0','-','=',8,9,
    'q','w','e','r','t','y','u','i','o','p','[',']',13,0,'a',
    's','d','f','g','h','j','k','l',';','\'','`',0,'\\','z','x',
    'c','v','b','n','m',',','.','/',0,0,0,' '
};

char shifted_keyboard_map[64] = {
    0,27,'!','@','#','$','%','^','&','*','(',')','_','+',8,9,
    'q','w','e','r','t','y','u','i','o','p','[',']',13,0,'a',
    's','d','f','g','h','j','k','l',':','"','~',0,'|','z','x',
    'c','v','b','n','m','<','>','?',0,0,0,' '
};

int shift=0;
int caps=0;
int ctrl=0;

/* ------------------------------------------------------------------------------------------------------
                                            KEYBOARD METHODS
------------------------------------------------------------------------------------------------------ */

char keyCodeToChar(int keyCode){

    // Shift in
    if(keyCode == 42 || keyCode == 54){
        shift = 1;
        return 15;
    }

    // Shift out
    if(keyCode == 170 || keyCode == 182){
        shift = 0;
        return 14;
    }

    // Caps lock
    if(keyCode == 58){
        caps = caps ? 0 : 1;
        return 14;
    }

    // ESC
    if(keyCode == 1)
        return 4;

    // Unmapped key
    if(keyCode > 63)
        return 0;
    
    // If it is a letter check for shift/caps
    if((keyCode>=16 && keyCode<=25) || (keyCode>=30 && keyCode<=38) || (keyCode>=44 && keyCode<=50)){      
        int cap = (shift || caps) && !(shift && caps);
        return cap ? keyboard_map[keyCode]-'a'+'A' : keyboard_map[keyCode];   
    }else{
        return shift ? shifted_keyboard_map[keyCode] : keyboard_map[keyCode];
    }

}



void handleKeyStroke() {
   
    char aux = keyCodeToChar(read());
    
    if(aux != 0)
        pipe_write_nofd(stdinPipe, &aux, 1);


}


void assignKeyboardPipe(pipe new) {

    stdinPipe = new;

}
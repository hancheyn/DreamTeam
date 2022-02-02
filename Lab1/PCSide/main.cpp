#include <iostream>
#include<windows.h>
#include <stdio.h>
#include <fstream>

#include <time.h>
#include <stdio.h>

#include <string>
#include <stdlib.h>
#include <conio.h>
using namespace std;

#include "COMPORT.h"

/*****************************************************************
* Authors: Nathan Hanchey, Getrude Muthiani, Brenden Beauchamp
* Date: 1/13/2022
*
* References: EGR 436 Materials
* COM PORTS: https://www.pololu.com/docs/0J73/15.6
*
******************************************************************/
void delay(int n);

int main() {

    //!Find Correct Comm Port
    char char_continue;
    do {
        comport_search();
        cout << "Enter 'R' to update list or 'C' to continue: ";
        scanf(" %c", &char_continue);
        cout << endl;
    } while(char_continue == 'R');

    //Enter the com port id
    do {
        printf("Enter the Com Port (e.g. COM1): ");
        scanf("%s", pszPortName);
        if(comport_open(pszPortName)) {
            cout << "Connected!" << endl << endl;
            char_continue = 'C';
        }
        else {
            cout << "Not Connected!" << endl << endl;
            char_continue = 'R';
        }
    } while(char_continue == 'R');

    //3 Identify if connected

    cout << "Enter Command for Beats Per Minute" << endl;
    cout << "\t'r' = reset to 60 bpm" << endl;
    cout << "\t'u' = increase by 2 bpm" << endl;
    cout << "\t'd' = decrease by 2 bpm" << endl;
    cout << "\t'N' = send bpm number" << endl;
    cout << "\t'X' = exit" << endl;

    //!Identify Action to take
    char c, send[15];
    string mess;


    while(c != 'X') {
        if(_kbhit()) {
            c = getch();
            printf("You entered: %c\n", c);
        }

        if(c == 'r') {

            /************************************/
            //Writing data to Serial Port
            comport_send(&c);
            mess = comport_recieve();
            cout << "Tempo BPM: " << mess << endl << endl;
            /************************************/
            c = '0';
        }
        else if(c == 'u') {
            /************************************/
            comport_send(&c);
            mess = comport_recieve();
            cout << "Tempo BPM: " << mess << endl << endl;
            /************************************/
            c = '0';
        }
        else if(c == 'd') {
            /************************************/
            comport_send(&c);
            mess = comport_recieve();
            cout << "Tempo BPM: " << mess << endl << endl;
            /************************************/
            c = '0';
        }
        else if(c == 'N') {
            cout << "Enter a BPM number and press enter: ";
            cin >> send;
            comport_send(send);
            mess = comport_recieve();
            cout << "Tempo BPM: " << mess << endl << endl;
            c = '0';
        }

        if(c == '0') {
            //clock_t start = clock();
            //while(clock() - start < 1000);
            c = 'c'; //close
        }


    }
    CloseHandle(hComm);
    clock();

    return 0;
}



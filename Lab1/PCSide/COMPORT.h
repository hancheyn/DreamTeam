/*****************************************************************
* Authors: Nathan Hanchey, Getrude Muthiani, Brenden Beauchamp
* Date: 1/13/2022
*
* References: EGR 436 Materials
* COM PORTS SOURCE: https://www.pololu.com/docs/0J73/15.6
* DOCUMENTATION: AN197: Serial Communications Guide for the CP210x
******************************************************************/

#ifndef COMPORT_H_INCLUDED
#define COMPORT_H_INCLUDED


#define _countof(a) (sizeof(a)/sizeof(*(a)))
#define wchar_t const char

//!COM PORT
HANDLE hComm;  // Handle to the Serial port
BOOL   Status; // Status
DCB dcbSerialParams;  // Initializing DCB structure
COMMTIMEOUTS timeouts;  //Initializing timeouts structure
  //  char SerialBuffer[64] = { 0 }; //Buffer to send and receive data
DWORD BytesWritten;          // No of bytes written to the port
DWORD dwEventMask;     // Event mask to trigger
    //char  ReadData;        //temperory Character
DWORD NoBytesRead;     // Bytes read by ReadFile()
char pszPortName[10]; //com port id
char PortNo[20];

void comport_search();
int comport_open(char PszPortName[]);
void comport_send(char serialbuffer[]);
string comport_recieve();


/// ****| comport_search | ****************//*
/// * Brief: Searches and Prints out available COM Ports
/// * param:
/// *      N/A
/// * return:
/// *      N/A
/// ****************************************************/
void comport_search() {
    //1 Find Comm ports Available
    system("Powershell [System.IO.Ports.SerialPort]::getportnames() > comm.txt");
    //2 Read Comm File & Send Messages to identify which is serial comm with MSP
    printf("Available COM Ports: \n");
    string line;
    ifstream file("comm.txt");
    if(file.is_open()) {
        while(getline(file,line))
        {
            if((line.length()))
                cout << line << endl;
        }
        cout << "End of Available Ports" << endl;
        file.close();
    }
}

/// ****| comport_open | ****************//*
/// * Brief: Opens a COM port
/// * param:
/// *      char array - COM Name for opening port
/// * return:
/// *      int - 0 = false
/// ****************************************************/
int comport_open(char pszPortName[]) {

    //Enter the com port id
    sprintf(PortNo, "\\\\.\\%s", pszPortName);
    //Open the serial com port
    hComm = CreateFile(PortNo, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);                             // Null for Comm Devices
    if (hComm == INVALID_HANDLE_VALUE) {
        return 0;
    }
    //Setting the Parameters for the SerialPort
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    Status = GetCommState(hComm, &dcbSerialParams); //retrieves  the current settings
    if (Status == FALSE) {
        return 0;
    }
    dcbSerialParams.BaudRate = CBR_9600;      //BaudRate = 9600
    dcbSerialParams.ByteSize = 8;             //ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT;    //StopBits = 1
    dcbSerialParams.Parity = NOPARITY;        //Parity = None
    Status = SetCommState(hComm, &dcbSerialParams);
    if (Status == FALSE) {
        return 0;
    }
    //Setting Timeouts
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if (SetCommTimeouts(hComm, &timeouts) == FALSE){
        return 0;
    }

    return 1;
}

/// ****| comport_send | ****************//*
/// * Brief: Send char data through UART
/// * param:
/// *      char array - message to send
/// * return:
/// *      void
/// ****************************************************/
void comport_send(char SBuffer[]) {

    //Writing data to Serial Port
    unsigned int buffer_size = sizeof(SBuffer);
    Status = WriteFile(hComm, SBuffer, buffer_size, &BytesWritten, NULL);
    if (Status == FALSE) {
        printf("\nFailed to be Written");
    }

}

/// ****| comport_recieve | ****************//*
/// * Brief: void
/// * param:
/// *      void
/// * return:
/// *      string - recieved string returned
/// ****************************************************/
string comport_recieve() {

    bool success = false;
    char SerialBuffer[64] = { '\0' };
    char ReadData;
    DWORD wait_time = 1000;
    unsigned char loop = 0;

    OVERLAPPED o = {0};

    do {
        o.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        if (!ReadFile(hComm, &ReadData, sizeof(ReadData), &NoBytesRead, &o)) {
                if (GetLastError() == ERROR_IO_PENDING)
                    if (WaitForSingleObject(o.hEvent, wait_time) == WAIT_OBJECT_0) //Time is in milliseconds
                        success = true;
                GetOverlappedResult(hComm, &o, &NoBytesRead, FALSE);
        } else
            success = true;

        SerialBuffer[loop] = ReadData;
        loop++;
    } while(NoBytesRead > 0);
    loop--; //Get Actual length of received data

    CloseHandle(o.hEvent);

    string str;
    int index = 0;
    for (index = 0; index < loop; ++index) {
        str += SerialBuffer[index];
        SerialBuffer[index] = '\0';
    }

    return str;
}


#endif // COMPORT_H_INCLUDED

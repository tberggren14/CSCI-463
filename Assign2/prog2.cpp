//***************************************************************************
//
//  Trevor Berggren
//  Z1906236
//  CSCI 463 
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************
#include <iostream>
#include <iomanip>
#include <stdio.h>

using std::endl;
using std::cout;
using std::cin;
using std::hex;
using std::dec;
using std::setfill;
using std::setw;
using std::showpos;

/**
 * Main function used to call printBinFloat to decode a hex number to IEEE 754 number
 *
 
 * @return int
 *
 ********************************************************************************/
int main() 
{
    void printBinFloat(uint32_t x);

    uint32_t x;

    while(cin >> hex >> x)
        printBinFloat(x);

    return 0;
}


/**
 * Reads zero or more 32-bit hex values and then print a detailed description of the 
 * floating point value that they represent by extracting and showing the sign, exponent, and significand from the 32-bit value
 *
 * @param x a 32 bit unsigned hex number used to print the IEEE 754 number
 * 
 * @return does not return / void
 *
 * @note Loops through the unsigned hex number and prints the following :
 * the binary version
 * the exponent
 * the significand
 * the IEEE 754 number
 ********************************************************************************/
void printBinFloat(uint32_t x) 
{
    uint32_t temp = 0x80000000;
    int lineCount = 0;
    
    cout << setfill('0');                           
    cout << "0x" << hex << setw(8) << x << " = ";           // prints the hex number

    for (int i = 0; i < 32; i++)                            // loops through the 32 bits and prints the number in binary
    {
        if(lineCount == 4)
        {
            cout << " ";
            lineCount = 0;
        }
        
        cout << (x & temp ? '1':'0');
        temp >>= 1;                                             // shifts temp to the right by 1
    
        lineCount += 1;
    }
    
    cout << "\nsign: " << (x & 0x80000000 ? '1':'0') << endl;

    temp = 0xff000000;                                      // sets the mask to get exponent
    temp >>=1;
    temp = x&temp;
    temp >>= 23;
    temp -= 127;
    int32_t tempExp = temp;
    
    cout << setfill('0');
    cout << " exp: 0x" << hex << setw(8) << tempExp << dec <<" (" << tempExp << ")" << endl;        // prints the exponent in hex and decimal

    uint32_t sig = x&0x007FFFFF;                                                                    //  sets the mask to get significand                                        
    cout << " sig: 0x" << hex << setw(8) << sig << endl;
    temp = 0x00800000;                                                                              // sets the mask for prints the significand           
    cout << (x & 0x80000000 ? '-':'+');

    if(tempExp == -127 )                // if the exponent is -127
    {
        cout << "0" << endl;
        return;

    }

    else if(tempExp == 128 )            // if the exponent is all 1s 
    {
        cout << "inf" << endl;
        return;
    } 

    if(tempExp < 0)                     // if the exponent is negitive leading 0. 
    {
        cout << "0.";

        for(int x = 0; x < std::abs(tempExp)-1; x++ )   // loops through the absolute value of the exponent
            cout << "0";

        cout << "1";

        for(int i = 0; i < 23; i++)     // loops through the 23 bits of the significand
        {
            temp >>=1;
            cout << (sig & temp ? '1':'0'); 
        }

        cout << endl;

    }
    else if(tempExp == 0)               // if the exponent is negitive leading 1.
    {
        //1.
        cout << "1.";

        for(int i = 0; i < 23; i++)     // loops through the 23 bits of the significand
        {
            temp >>=1;
             cout << (sig & temp ? '1':'0');
             
             
        }

        cout << endl;

    }
    else if(tempExp > 0)                    // if the exponent is greater than 1 prints the significand then .0
    {
        cout << "1";

        for(int i = 0; i < 23; i++)         // loops through the 23 bits of the significand
        {
            temp >>=1;
            cout << (sig & temp ? '1':'0'); 
        }

    
        for(int x = 0; x < tempExp-23; x++ )    // loops through the exponent minus the 23 bits of the significand
            cout << "0";

        cout << ".0";

        cout << endl;

    }
    



}
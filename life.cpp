//*************************************************************************
//
//	DESCRIPTION:   Life Sim
//	AUTHOR:        kcrissey
//	An implementation of the life sim program.
//
//*************************************************************************


#include <iostream>
using namespace std;

//#include <conio.h>        // for _kbhit () used below - DOS only
#include <limits.h>         // used for min/max values of data types
#include <memory.h>         // used for memset () used below
#include <stdlib.h>         // used for system () below
#include <time.h>           // used for time () - number of seconds 1/1/70
#include <sys/select.h>     // Linux alternative to conio.h

int main () {

  int   kbhit  (void);

  const long   NumRows  (35);
  const long   NumCols  (85);
  const time_t WaitTime (1);

  bool   CurrGen [NumRows + 2] [NumCols + 2];
  time_t CurrTime;
  long   Col;
  long   Generation (0);
  long   Population (0);
  long   Births     (0);
  long   Deaths     (0);
  long   i;
  bool   NextGen [NumRows + 2] [NumCols + 2];
  long   NumNeighbors;
  long   NumStartingCells;
  long   Row;
  time_t StartTime;

  memset (CurrGen, false, (NumRows + 2) * (NumCols + 2) * sizeof (bool));

  cout << endl;
  cout << "Enter starting cell coordinates between Rows 0 to "
       << NumRows << " and Columns 0 to " << NumCols << "." << endl;
  cout << "(Enter '0 0' when done.)" << endl;

  do {
    cout << "Enter cell position (Row Col): ";
    cin  >> Row >> Col;
    if ((Row >= 1) && (Row <= NumRows) && (Col >= 1) && (Col <= NumCols)) {
      CurrGen [Row] [Col] = true;
      Population++;
    }
    else
      if ((Row != 0) && (Col != 0)) {
        cout << "Invalid CurrGen position" << endl;
        i--;
      }
  } while ((Row != 0) && (Col != 0));

  // Display the CurrGen on the screen
  do {		     // loop until Enter pressed
    Generation++;
    system ("clear");

    cout << "Generation " << Generation << "\t\tBirths " << Births << "\tDeaths " << Deaths
         << "\tPopulation " << Population << endl;
    for (Row = 1; Row <= NumRows; Row++) {
      for (Col = 1; Col <= NumCols; Col++)
      cout << ((CurrGen [Row] [Col]) ? '*' : '.');
      cout << endl;
    }
      cout << "Displaying Generations until you press <Enter>" << endl;

    // Scan array & count neighbors to each cell
    Births = 0;
    Deaths = 0;
    for (Row = 1; Row <= NumRows; Row++) {
      for (Col = 1; Col <= NumCols; Col++) {
        NumNeighbors = 0;
        if (CurrGen [Row - 1] [Col - 1])
          NumNeighbors++;
        if (CurrGen [Row - 1] [Col])
          NumNeighbors++;
        if (CurrGen [Row - 1] [Col + 1])
          NumNeighbors++;
        if (CurrGen [Row] [Col - 1])
          NumNeighbors++;
        if (CurrGen [Row] [Col + 1])
          NumNeighbors++;
        if (CurrGen [Row + 1] [Col - 1])
          NumNeighbors++;
        if (CurrGen [Row + 1] [Col])
          NumNeighbors++;
        if (CurrGen [Row + 1] [Col + 1])
          NumNeighbors++;

        // Apply cell life/death rules
        if ((NumNeighbors >= 4) || (NumNeighbors <= 1)) {
          NextGen [Row] [Col] = false;
          if (CurrGen [Row] [Col])
            Deaths++;
          }
        else {
          if (NumNeighbors == 3) {
            NextGen [Row] [Col] = true;
            if (CurrGen [Row] [Col] == false)
              Births++;
            }
          else
            NextGen [Row] [Col] = CurrGen [Row] [Col];
        }
      }
    }

    // Copy NextGen array to CurGen for re-display
    Population = 0;
    for (Row = 1; Row <= NumRows; Row++) {
      for (Col = 1; Col <= NumCols; Col++) {
        CurrGen [Row] [Col] = NextGen [Row] [Col];
        if (CurrGen [Row] [Col])
          Population++;
      }
    }


    StartTime = time (0);   // loop to wait for a while
    do {
      CurrTime = time (0);
      } while ((CurrTime - StartTime) < WaitTime);

  } while (!kbhit ());     //  end of loop until <Enter> pressed
}


//Replacement function for kbhit for Linux

int kbhit(void)
{
struct timeval tv;
fd_set read_fd;

/* Do not wait at all, not even a microsecond */
tv.tv_sec=0;
tv.tv_usec=0;

/* Must be done first to initialize read_fd */
FD_ZERO(&read_fd);

/* Makes select() ask if input is ready:
* 0 is the file descriptor for stdin */
FD_SET(0,&read_fd);

/* The first parameter is the number of the
* largest file descriptor to check + 1. */
if(select(1, &read_fd,NULL, /*No writes*/NULL, /*No exceptions*/&tv) == -1)
return 0; /* An error occured */

/* read_fd now holds a bit map of files that are
* readable. We test the entry for the standard
* input (file 0). */

if(FD_ISSET(0,&read_fd))
/* Character pending on stdin */
return 1;

/* no characters were pending */
return 0;
}

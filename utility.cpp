
#include "utility.h"

#include <iostream>
using namespace std;


void parseymd(long int ymd,int *year,int *month,int *day)
{
  *year = ymd/10000;
  int itemp = ymd-*year*10000;
  *month = itemp/100.0;
  itemp = itemp-*month*100;
  *day = itemp;
}


void parsehms(double hms,int *hour,int *min,double *sec)
{
  *hour = hms/10000;
  double dtemp = hms-*hour*10000;
  *min = dtemp/100.0;
  dtemp = dtemp-*min*100.0;
  *sec=dtemp;
}


void sec2hms(double sec,int *h,int *m,double *s)
{

  double newsec = sec;
  int dow = (int)(newsec/86400.0);
  newsec -= dow*86400;
  int hr = (int)(newsec/3600.0);
  newsec -= hr*3600;
  int min = (int)(newsec/60.0);
  newsec -= min*60.0;
  *h = hr;
  *m = min;
  *s = newsec;
}


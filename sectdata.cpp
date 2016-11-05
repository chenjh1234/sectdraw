// sectData.cpp: implementation of the sectData class.
//
//////////////////////////////////////////////////////////////////////

#include "sectdata.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

sectData::sectData() : drawData()
{
   init();

}

sectData::~sectData()
{

}

void sectData::init()
{
   m_iDotOffsetX = 0;
   m_iDotOffsetY = 0;
   m_iIndexColorTBBak = 0;
   sam = 0;
   si = 0;
   trs = 0;
   headLen = 0;
   fmax = 0;
   fmin = 0;
   m_RMS = 0;
   m_xCom = 0;
   m_yCom = 0;
   m_iDrawType = 0;
   //m_iMaxDisplayHeader = 0;


   m_fGain = 2;
   head = NULL;
   buf = NULL;
   obuf = NULL;
   displayHeader = NULL;
   m_bGainGlobal = true;
   m_rms = 5.;
   m_compareMode = COMPARE_MODE_V;
   m_pickMode = PICK_NO;
   m_pickTo = PICK_NOW;
   m_pickData = new pickData();
   m_iAGCLen = 0; //ms
   m_f1 = 0;
   m_f2 = 0;
   m_f3 = 0;
   m_f4 = 0;

}
qreal sectData::labMethodX(qreal x)
{
   int i;
   if (displayHeader == NULL)
   {
      return x;
   }
   i = x;
   return displayHeader[i];
}
qreal sectData::labMethodY(qreal x)
{
   //return  si*x;
   return  x; // careless of si
}
double sectData::getDataRms()
{
   int i, j;
   float *ff;
   double rms;
   rms = 0;


   for (i = 0; i < trs; i++)
   {
      ff = buf + (int)i * sam;

      for (j = 0; j < sam; j++)
      {
         rms += ff[j] * ff[j];
      }
   }
   rms = rms / (trs * sam);
   rms = sqrt(rms);
   m_RMS = rms;
   return rms;
}
double sectData::getRMS()
{
   return m_rms;
}
void sectData::setRMS(float m)
{
   m_rms = m;//times of the RMS as the max value.
   fmax = m * m_RMS;//data rms.
   fmin = 0;
}
void sectData::getMaxMin()
{
   double r;
   getDataRms();
   setRMS(m_rms);
   return;
#if 0
   int i, j;
   float ffmax, ffmin;
   float *ff;

   ffmax = 0;
   ffmin = 300000;

   //double ffm, ffp;

   for (i = 0; i < trs; i++)
   {

      ff = buf + (int)i * sam;

      for (j = 0; j < sam; j++)
      {
         if (abs(ff[j]) > ffmax)
         {
            ffmax = abs(ff[j]);

         }
         if (abs(ff[j]) < ffmin) ffmin = abs(ff[j]);
         //ff[j] =0;
      }
   }
   fmax = ffmax;
   fmin = ffmin;
   #endif // 0
}
void sectData::linearInterpolation(int numSamplesIn, float const *xin, float const *yin,
                                   int numSamplesOut, float sampleIntOut, float *yout)
{
   if (numSamplesIn <= 0) return;
   float x0 = xin[0];
   float y0 = yin[0];
   float xN = xin[numSamplesIn - 1];
   float yN = yin[numSamplesIn - 1];

   int sample0 = (int)(x0 / sampleIntOut);
   int sampleN = (int)(xN / sampleIntOut) + 1;
   if (sampleN > numSamplesOut) sampleN = numSamplesOut;
   //for( int isamp = 0; isamp <= sample0; isamp++ ) // core down if not added;
   for (int isamp = 0; isamp <= sample0 && isamp < numSamplesOut; isamp++)
   {
      yout[isamp] = y0;
   }
   for (int isamp = sampleN; isamp < numSamplesOut; isamp++)
   {
      yout[isamp] = yN;
   }
   //qDebug() << "in int =" << numSamplesIn << numSamplesOut<< sample0 << sampleN;
   if (numSamplesIn == 1) return;


   float x1  = x0;
   float x2  = numSamplesIn > 1 ? xin[1] : x0;
   float y1  = y0;
   float y2  = numSamplesIn > 1 ? yin[1] : y0;
   float dx = x2 - x1;
   float dy = y2 - y1;
   float dydx = dx > 0.0f ? dy / dx : 0.0f;
   int counter = 0;
   for (int isamp = sample0 + 1; isamp < sampleN; isamp++)
   {
      float x = (float)isamp * sampleIntOut;
      while (x > x2)
      {
         x1 = x2;
         y1 = y2;
         counter += 1;
         if (counter == numSamplesIn)
         {
            x2 = x; // Avoids entering this loop again
            break;
         }
         x2 = xin[counter];
         y2 = yin[counter];
         dx = x2 - x1;
         dy = y2 - y1;
         dydx = dx > 0.0f ? dy / dx : 0.0f;
      }
      yout[isamp] = y1 + (x - x1) * dydx;
   }
}
float sectData::adjustPick(int idx, float time0)
{
   int i, j, p_flag, imax;
   float max, maxa[100];
   int maxa_idx[100], max_idx, max_c;
   int pk_position, tm_len, tm, tm1, tm2;
   float *trace;
   float retTime;

   pk_position = (int)m_pickTo;
   if (m_pickTo == PICK_NOW) return time0;
   i = idx;
   tm_len = 200;
   //qDebug() << "sam = " << sam<< i;
   trace = buf + i * sam;

   tm = time0;
   tm1 = tm - tm_len;
   tm2 = tm + tm_len;
//make it sample:
   if (si == 0) return 0;
//qDebug() << "ttm1,ttm2= " << tm1 << tm2<<time0;
   tm1 = tm1 / (int)si;
   tm2 = tm2 / (int)si;
   tm = tm / (int)si;

// adjust if the value crazy:
   if (tm1 < 0) tm1 = 0;
   if (tm2 < 0) tm2 = 0;
   if (tm < 0) tm = 0;
   if (tm1 >= sam) tm1 = sam - 1;
   if (tm2 >= sam) tm2 = sam - 1;
   if (tm >= sam) tm = sam;
   // qDebug() << "tm1,tm2= " << tm1 << tm2;

// init the
   p_flag = 1; // first change to positive
   max = 0;   //max value;
   max_idx = 0; //max index;
   max_c = -1;  // counter of the max
   switch (pk_position)
   {
   case PICK_TO_PEAK:

      for (j = tm1; j < tm2 + 1; j++)
      {
         // qDebug()<< "time = " <<j<<j * si << trace[j];
         if (trace[j] >= 0)
         {
            if (p_flag == 0) //last loop gs->trace is negitive or first loop
            {
               p_flag = 1;
               max = trace[j];
               max_idx = j;
            }
            else
            {
               if (trace[j] > max)
               {
                  max = trace[j];
                  max_idx = j;
               }
            }

         }
         else
         { // turn to negitive: save the last :positive max value iand index;
            if (p_flag == 1)  // first change to negitive
            {
               max_c++;
               maxa[max_c] = max;
               maxa_idx[max_c] = max_idx;
               max = 0; //make next phase
               p_flag = 0;
               // qDebug() << "max_c" << max_c <<max_idx<< trace[max_idx];
            }

         }  // endof if---else
      } //end of loop tm1 tm2
        // if last loop is positive:it is max ,but we cannot sure it is a peak.
      /*
      if(maxa[max_c] != max || maxa_idx[max_c] != max_idx)
      {
          max_c ++;
          maxa[max_c] = max;
          maxa_idx[max_c] = max_idx;
      }
      */
      //adjust here max is minimal ,max_idx is the index of minimal
      // to find the nearest peak;
      imax = 10000;
      for (j = 0; j < max_c + 1; j++)
      {
         if (imax > abs(tm - maxa_idx[j]))
         {
            imax =  abs(tm - maxa_idx[j]);
            max_idx = j;
         }
      }
      // we cannot find peak:
      if (max_c  == -1)
      {
         retTime = tm * (int)si;
         break;
      }
      // if the last is minmal
      // we find peak:
      retTime = maxa_idx[max_idx] * (int)si; //+m_iSI;
      if (retTime  == 0) retTime = tm * (int)si;
      break;
      //=====================================================================
      // end of PEAK

   case PICK_TO_VALLEY:

      for (j = tm1; j < tm2 + 1; j++)
      {
         if (trace[j] <= 0)
         {
            if (p_flag == 0) //last loop gs->trace is positive or first loop
            {
               p_flag = 1;
               max = trace[j];
               max_idx = j;
            }
            else
            {
               if (trace[j] < max)
               {
                  max = trace[j];
                  max_idx = j;
               }
            }

         }
         else
         {
            if (p_flag == 1)  // first change to positive
            {
               max_c++;
               maxa[max_c] = max;
               maxa_idx[max_c] = max_idx;
               max = 0;
               p_flag = 0;
            }

         } // end of if ---else
      } //end of loop tm1 tm2
        // if last loop is negitive

      if (maxa[max_c] != max || maxa_idx[max_c] != max_idx)
      {
         max_c++;
         maxa[max_c] = max;
         maxa_idx[max_c] = max_idx;
      }
      //adjust here max is mainimal ,max_idx is the index of minimal
      imax = 10000;
      for (j = 0; j < max_c + 1; j++)
      {
         if (abs(tm - maxa_idx[j]) < imax)
         {
            imax =  abs(tm - maxa_idx[j]);
            max_idx = j;
         }
      }
      // if the last is minmal
      if (max_c  == -1)
      {
         retTime = tm * (int)si;
         break;
      }
      retTime = maxa_idx[max_idx] * (int)si; // + m_iSI;
      if (retTime == 0) retTime = tm * (int)si;
      break;
      //=========================================================================
      // end of VALLEY
   case PICK_TO_CROSS:
      for (j = tm1; j < tm2 + 1; j++)
      {
         if (trace[j] <= 0)
         {
            if (p_flag == 0) //last loop gs->trace is positive or first loop
            {
               p_flag = 1;
            }
         }
         else
         {
            if (p_flag == 1)  // first turn to positive
            {
               max_c++;
               //axa[max_c] = max;
               maxa_idx[max_c] = j;
               //max=0;
               p_flag = 0;
            }

         } // end of if ---else
      } //end of loop tm1 tm2
        //adjust here max is mainimal ,max_idx is the index of minimal
      imax = 10000;
      for (j = 0; j < max_c + 1; j++)
      {
         if (abs(tm - maxa_idx[j]) < imax)
         {
            imax =  abs(tm - maxa_idx[j]);
            max_idx = j;
         }
      }
      // if the last is minmal
      if (max_c  == -1)
      {
         retTime = tm * (int)si;
         break;
      }
      //cjh ???
      retTime = maxa_idx[max_idx] * (int)si; //+ m_iSI;
      if (retTime == 0) retTime = tm * (int)si;
      break;
      // end of ZERO
   } // end of switch
   return retTime;
}
void sectData::doAGC(float *data, int nt, int iwagc)
{
   //      int first = TRUE;
   float *agcdata;
   register int i;
   register float val;
   register float sum;
   register int nwin;
   register float rms;
   int FSIZE = 4;
   /* allocate room for agc'd data */

   agcdata = new float[nt];

   /* compute initial window for first datum */
   sum = 0.0;
   for (i = 0; i <= iwagc; ++i)
   {
      val = data[i];
      sum += val * val;
   }
   nwin = iwagc + 1;
   rms = sum / nwin;
   agcdata[0] = (!rms) ? 0.0 : data[0] / sqrt(rms);

   /* ramping on */
   for (i = 1; i <= iwagc; ++i)
   {
      val = data[i + iwagc];
      sum += val * val;
      ++nwin;
      rms = sum / nwin;
      agcdata[i] = (!rms) ? 0.0 : data[i] / sqrt(rms);
   }

   /* middle range -- full rms window */
   data[nt] = 0.0;
   for (i = iwagc + 1; i <= nt - iwagc; ++i)
   {
      val = data[i + iwagc];
      sum += val * val;
      val = data[i - iwagc - 1];
      sum -= val * val; /* rounding could make sum negative! */
      rms = sum / nwin;
      agcdata[i] = (rms <= 0.0) ? 0.0 : data[i] / sqrt(rms);

   }

   for (i = nt - iwagc + 1; i < nt; ++i)
   {
      val = data[i - iwagc - 1];
      sum -= val * val; /* rounding could make sum negative! */
      --nwin;
      rms = sum / nwin;
      agcdata[i] = (rms <= 0.0) ? 0.0 : data[i] / sqrt(rms);

   }
   memcpy(data,  agcdata, nt * FSIZE);
   delete[] agcdata;
   return;
}
int  sectData::doFilter(float *data, int len, float si, float f11, float f22, float f33, float f44)
{
   float a1, a2, a3, a4;
   float f1, f2, f3, f4;

   int npoles;
   float f3db, nyq;
//	float *data1;
//float si;
   float dt;
   //si = 4;

   f1 = f11;
   f2 = f22;
   f3 = f33;
   f4 = f44;

   dt = (double)si / 1000000.0;
   nyq = 0.5 / dt;

   f1 = f1 / 100 * nyq;
   f2 = f2 / 100 * nyq;
   f3 = f3 / 100 * nyq;
   f4 = f4 / 100 * nyq;

   f1 *= dt;
   f2 *= dt;
   f3 *= dt;
   f4 *= dt;


//	data1 = new float[len +10];
//	high pass:
   a1 = 0.05;
   a2 = 0.95;
   if (f1 !=0 && f2 !=0)
   {
       bfdesign(f2, a2, f1, a1, &npoles, &f3db);
       bfhighpass(npoles, f3db, len, data, data);
   }


//	low pass:

   a3 = 0.95;
   a4 = 0.05;
   if (f3 !=0 && f4 !=0)
   {
       bfdesign(f3, a3, f4, a4, &npoles, &f3db);
       bflowpass(npoles, f3db, len, data, data);
   }

//	delete data1;
   return 0;
}


void
sectData::bfdesign(float fpass, float apass, float fstop, float astop,
                   int *npoles, float *f3db)
/*****************************************************************************
Butterworth filter:  compute number of poles and -3 db frequency
for a low-pass or high-pass filter, given a frequency response
constrained at two frequencies.
******************************************************************************
Input:
fpass		frequency in pass band at which amplitude is >= apass
apass		amplitude in pass band corresponding to frequency fpass
fstop 		frequency in stop band at which amplitude is <= astop
astop		amplitude in stop band corresponding to frequency fstop

Output:
npoles		number of poles
f3db		frequency at which amplitude is sqrt(0.5) (-3 db)
******************************************************************************
Notes:
(1) Nyquist frequency equals 0.5

(2) The following conditions must be true:
    (0.0<fpass && fpass<0.5) &&
    (0.0<fstop && fstop<0.5) &&
    (fpass!=fstop) &&
    (0.0<astop && astop<apass && apass<1.0)

(3) if (fpass<fstop)
        a low-pass filter is assumed
    else
        a high-pass filter is assumed
******************************************************************************
Author:  Dave Hale, Colorado School of Mines, 06/02/89
*****************************************************************************/
{
   float wpass, wstop, fnpoles, w3db;

   /* warp frequencies according to bilinear transform */
   wpass = 2.0 * tan(PI * fpass);
   wstop = 2.0 * tan(PI * fstop);

   /* if lowpass filter, then */
   if (fstop > fpass)
   {
      fnpoles = log((1.0 / (apass * apass) - 1.0) / (1.0 / (astop * astop) - 1.0))
         / log(pow(wpass / wstop, 2.0));
      w3db = wpass / pow((1.0 / (apass * apass) - 1.0), 0.5 / fnpoles);

      /* else, if highpass filter, then */
   }
   else
   {
      fnpoles = log((1.0 / (apass * apass) - 1.0) / (1.0 / (astop * astop) - 1.0))
         / log(pow(wstop / wpass, 2.0));
      w3db = wpass * pow((1.0 / (apass * apass) - 1.0), 0.5 / fnpoles);
   }

   /* determine integer number of poles */
   *npoles = 1 + (int)fnpoles;

   /* determine (unwarped) -3 db frequency */
   *f3db = atan(0.5 * w3db) / PI;
}

void
sectData::bfhighpass(int npoles, float f3db, int n, float p[], float q[])
/*****************************************************************************
Butterworth filter:  high-pass
******************************************************************************
Input:
npoles		number of poles (and zeros); npoles>=0 is required
f3db		3 db frequency; nyquist = 0.5; 0.0<=f3db<=0.5 is required
n		length of p and q
p		array[n] to be filtered

Output:
q		filtered array[n] (may be equivalent to p)
******************************************************************************
Author:  Dave Hale, Colorado School of Mines, 06/02/89
*****************************************************************************/
{
   int jpair, j;
   float r, scale, theta, a, b1, b2, pj, pjm1, pjm2, qjm1, qjm2;

   r = 2.0 * tan(PI * fabs(f3db));
   if (npoles % 2 != 0)
   {
      scale = r + 2.0;
      a = 2.0 / scale;
      b1 = (r - 2.0) / scale;
      pj = 0.0;
      qjm1 = 0.0;
      for (j = 0; j < n; j++)
      {
         pjm1 = pj;
         pj = p[j];
         q[j] = a * (pj - pjm1) - b1 * qjm1;
         qjm1 = q[j];
      }
   }
   else
   {
      for (j = 0; j < n; j++) q[j] = p[j];
   }
   for (jpair = 0; jpair < npoles / 2; jpair++)
   {
      theta = PI * (2 * jpair + 1) / (2 * npoles);
      scale = 4.0 + 4.0 * r * sin(theta) + r * r;
      a = 4.0 / scale;
      b1 = (2.0 * r * r - 8.0) / scale;
      b2 = (4.0 - 4.0 * r * sin(theta) + r * r) / scale;
      pjm1 = 0.0;
      pj = 0.0;
      qjm2 = 0.0;
      qjm1 = 0.0;
      for (j = 0; j < n; j++)
      {
         pjm2 = pjm1;
         pjm1 = pj;
         pj = q[j];
         q[j] = a * (pj - 2.0 * pjm1 + pjm2) - b1 * qjm1 - b2 * qjm2;
         qjm2 = qjm1;
         qjm1 = q[j];
      }
   }
}

void
sectData::bflowpass(int npoles, float f3db, int n, float p[], float q[])
/*****************************************************************************
Butterworth filter:  low-pass
******************************************************************************
Input:
npoles		number of poles (and zeros); npoles>=0 is required
f3db		3 db frequency; nyquist = 0.5; 0.0<=f3db<=0.5 is required
n		length of p and q
p		array[n] to be filtered

Output:
q		filtered array[n] (may be equivalent to p)
******************************************************************************
Author:  Dave Hale, Colorado School of Mines, 06/02/89
*****************************************************************************/
{
   int jpair, j;
   float r, scale, theta, a, b1, b2, pj, pjm1, pjm2, qjm1, qjm2;

   r = 2.0 * tan(PI * fabs(f3db));
   if (npoles % 2 != 0)
   {
      scale = r + 2.0;
      a = r / scale;
      b1 = (r - 2.0) / scale;
      pj = 0.0;
      qjm1 = 0.0;
      for (j = 0; j < n; j++)
      {
         pjm1 = pj;
         pj = p[j];
         q[j] = a * (pj + pjm1) - b1 * qjm1;
         qjm1 = q[j];
      }
   }
   else
   {
      for (j = 0; j < n; j++) q[j] = p[j];
   }
   for (jpair = 0; jpair < npoles / 2; jpair++)
   {
      theta = PI * (2 * jpair + 1) / (2 * npoles);
      scale = 4.0 + 4.0 * r * sin(theta) + r * r;
      a = r * r / scale;
      b1 = (2.0 * r * r - 8.0) / scale;
      b2 = (4.0 - 4.0 * r * sin(theta) + r * r) / scale;
      pjm1 = 0.0;
      pj = 0.0;
      qjm2 = 0.0;
      qjm1 = 0.0;
      for (j = 0; j < n; j++)
      {
         pjm2 = pjm1;
         pjm1 = pj;
         pj = q[j];
         q[j] = a * (pj + 2.0 * pjm1 + pjm2) - b1 * qjm1 - b2 * qjm2;
         qjm2 = qjm1;
         qjm1 = q[j];
      }
   }
}



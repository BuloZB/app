#ifndef __JPCNTX_H__
#define __JPCNTX_H__

#define NUM_OF_CATEGORY 6

#include "types.h" 

#define ENOUGH_REL_THRESHOLD  100
#define MAX_REL_THRESHOLD     1000

//hiragana frequency category table
extern char jp2CharContext[83][83];

class JapaneseContextAnalysis
{
public:
  JapaneseContextAnalysis() {Reset();};

  void HandleData(const char* aBuf, PRUint32 aLen);

  void HandleOneChar(const char* aStr, PRUint32 aCharLen)
  {
    PRInt32 order;

    //if we received enough data, stop here   
    if (mTotalRel > MAX_REL_THRESHOLD)   mDone = PR_TRUE;
    if (mDone)       return;
     
    //Only 2-bytes characters are of our interest
    order = (aCharLen == 2) ? GetOrder(aStr) : -1;
    if (order != -1 && mLastCharOrder != -1)
    {
      mTotalRel++;
      //count this sequence to its category counter
      mRelSample[jp2CharContext[mLastCharOrder][order]]++;
    }
    mLastCharOrder = order;
  };

  float GetConfidence();
  void      Reset();
  void      SetOpion(){};
  PRBool GotEnoughData() {return mTotalRel > ENOUGH_REL_THRESHOLD;};

protected:
  virtual PRInt32 GetOrder(const char* str, PRUint32 *charLen) = 0;
  virtual PRInt32 GetOrder(const char* str) = 0;

  //category counters, each interger counts sequence in its category
  PRUint32 mRelSample[NUM_OF_CATEGORY];

  //total sequence received
  PRUint32 mTotalRel;
  
  //The order of previous char
  PRInt32  mLastCharOrder;

  //if last byte in current buffer is not the last byte of a character, we
  //need to know how many byte to skip in next buffer.
  PRUint32 mNeedToSkipCharNum;

  //If this flag is set to PR_TRUE, detection is done and conclusion has been made
  PRBool   mDone;
};


class SJISContextAnalysis : public JapaneseContextAnalysis
{
  //SJISContextAnalysis(){};
protected:
  PRInt32 GetOrder(const char* str, PRUint32 *charLen);

  PRInt32 GetOrder(const char* str)
  {
    //We only interested in Hiragana, so first byte is '\202'
    if (*str == '\202' && 
          (uchar)*(str+1) >= (uchar)0x9f && 
          (uchar)*(str+1) <= (uchar)0xf1)
      return (uchar)*(str+1) - (uchar)0x9f;
    return -1;
  };
};

class EUCJPContextAnalysis : public JapaneseContextAnalysis
{
protected:
  PRInt32 GetOrder(const char* str, PRUint32 *charLen);
  PRInt32 GetOrder(const char* str)
    //We only interested in Hiragana, so first byte is '\244'
  {
    if (*str == '\244' &&
          (uchar)*(str+1) >= (uchar)0xa1 &&
          (uchar)*(str+1) <= (uchar)0xf3)
      return (uchar)*(str+1) - (uchar)0xa1;
    return -1;
  };
};

#endif /* __JPCNTX_H__ */


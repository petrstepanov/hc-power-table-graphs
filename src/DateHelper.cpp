/*
 * DateHelper.cpp
 *
 *  Created on: Nov 27, 2025
 *      Author: petrstepanov
 */

#include<DateHelper.h>
#include<TTimeStamp.h>

namespace DateHelper {

    TString formatTimeStamp(UInt_t timestamp){
        TTimeStamp* myTTimeStamp = new TTimeStamp();
        myTTimeStamp->Set(timestamp, kFALSE, 0, kFALSE);
        TString myTTimeStampString = myTTimeStamp->AsString("s");
        Int_t space = myTTimeStampString.First(' ');
        Int_t len = myTTimeStampString.Length();
        myTTimeStampString.Remove(space,len-space);
        return myTTimeStampString;
    }

}



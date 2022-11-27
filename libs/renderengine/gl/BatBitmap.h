#ifndef _BATBITMAP_H_
#define _BATBITMAP_H_

#include <string>
#include <stdint.h>

class BatBitmap
{
public:
    bool loadBitmap(const char* filename);

    std::string toString();
    uint8_t* getByte() {    return mData;   }
    int getDatalen() {  return mDatalen;    }
    uint32_t getWidth() {   return mInfoHead.hiWidth;   }
    uint32_t getHeight() {   return abs(mInfoHead.biHeight);   }
private:
    struct tagBITMAPFILEHEADER 
    {  
        uint16_t bfType;    
        uint32_t bfSize; 
        uint16_t bfReserved1; 
        uint16_t bfReserved2; 
        uint32_t bfOffBits;
    };

    struct tagBITMAPINFOHEADER
    {
        uint32_t biSize;
        uint32_t hiWidth;
        int32_t biHeight;
        uint16_t biPlanes;
        uint16_t biBitCount;
        uint32_t biCommpression;
        uint32_t biSizeImage;
        uint32_t biXPelsPerMeter;
        uint32_t biYPelsPerMeter;
        uint32_t biClrUsed;
        uint32_t biClrImpartant;
    };

    tagBITMAPFILEHEADER mHead;
    tagBITMAPINFOHEADER mInfoHead;

    uint8_t* mData = nullptr;
    int mDatalen = 0;

    static int transByte(const uint8_t* buff, int offset, uint32_t& value);
    static int transByte(const uint8_t* buff, int offset, int32_t& value);
    static int transByte(const uint8_t* buff, int offset, uint16_t& value);
};




#endif
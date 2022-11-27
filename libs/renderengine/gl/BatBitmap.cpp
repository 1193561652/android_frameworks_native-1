#include "BatBitmap.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <log/log.h>

bool BatBitmap::loadBitmap(const char* filename)
{
    int file = open(filename, O_RDWR);
    uint8_t buff[50] = {0};
    if (file > 0)
    {
        read(file, buff, 14);
        
        int offset = 0;
        offset = BatBitmap::transByte(buff, offset, mHead.bfType);
        offset = BatBitmap::transByte(buff, offset, mHead.bfSize);
        offset = BatBitmap::transByte(buff, offset, mHead.bfReserved1);
        offset = BatBitmap::transByte(buff, offset, mHead.bfReserved2);
        offset = BatBitmap::transByte(buff, offset, mHead.bfOffBits);

        read(file, buff, 40);
        offset = 0;
        offset = BatBitmap::transByte(buff, offset, mInfoHead.biSize);
        offset = BatBitmap::transByte(buff, offset, mInfoHead.hiWidth);
        offset = BatBitmap::transByte(buff, offset, mInfoHead.biHeight);
        offset = BatBitmap::transByte(buff, offset, mInfoHead.biPlanes);
        offset = BatBitmap::transByte(buff, offset, mInfoHead.biBitCount);
        offset = BatBitmap::transByte(buff, offset, mInfoHead.biCommpression);
        offset = BatBitmap::transByte(buff, offset, mInfoHead.biSizeImage);
        offset = BatBitmap::transByte(buff, offset, mInfoHead.biXPelsPerMeter);
        offset = BatBitmap::transByte(buff, offset, mInfoHead.biYPelsPerMeter);
        offset = BatBitmap::transByte(buff, offset, mInfoHead.biClrUsed);
        offset = BatBitmap::transByte(buff, offset, mInfoHead.biClrImpartant);

        int bytecnt = mInfoHead.biBitCount / 8;
        mDatalen = mInfoHead.hiWidth * abs(mInfoHead.biHeight) * bytecnt;
        if (mData) delete[] mData;
        mData = new uint8_t[mDatalen];

        int skip = (4 - (bytecnt * mInfoHead.hiWidth) % 4) % 4;
        uint8_t* p = mData;
        if (mInfoHead.biHeight > 0)
            p = &(mData[mDatalen-mInfoHead.hiWidth*bytecnt]);
        for (int i=0; i < abs(mInfoHead.biHeight); i++)
        {
            if (mInfoHead.biHeight > 0)
            {
                read(file, p, mInfoHead.hiWidth * bytecnt);
                p -= (mInfoHead.hiWidth * bytecnt);
            }
            else
            {
                read(file, p, mInfoHead.hiWidth * bytecnt);
                p += (mInfoHead.hiWidth * bytecnt);
                if (skip > 0)
                    read(file, buff, skip);
            }
        }
        close(file);
    } else {
        ALOGE("BAT open file %s fail %d", filename, errno);
        return false;
    }
    return true;
}

int BatBitmap::transByte(const uint8_t* buff, int offset, uint32_t& value)
{
    value = ((buff[offset+3] << 24) | (buff[offset+2] << 16) | (buff[offset+1] << 8) | buff[offset]);
    return offset + 4;
}
int BatBitmap::transByte(const uint8_t* buff, int offset, uint16_t& value)
{
    value = ((buff[offset+1] << 8) | buff[offset]);
    return offset + 2;
}
int BatBitmap::transByte(const uint8_t* buff, int offset, int32_t& value)
{
    uint32_t a = ((buff[offset+3] << 24) | (buff[offset+2] << 16) | (buff[offset+1] << 8) | buff[offset]);
    memcpy(&value, &a, 4);
    return offset + 4;
}

std::string BatBitmap::toString()
{
    printf("bfType:%d\n", mHead.bfType);
    printf("bfSize:%d\n", mHead.bfSize);
    printf("bfReserved1:%d\n", mHead.bfReserved1);
    printf("bfReserved2:%d\n", mHead.bfReserved2);
    printf("bfOffBits:%d\n", mHead.bfOffBits);

    printf("biSize:%d\n", mInfoHead.biSize);
    printf("hiWidth:%d\n", mInfoHead.hiWidth);
    printf("biHeight:%d\n", mInfoHead.biHeight);
    printf("biPlanes:%d\n", mInfoHead.biPlanes);
    printf("biBitCount:%d\n", mInfoHead.biBitCount);
    printf("biCommpression:%d\n", mInfoHead.biCommpression);
    printf("biSizeImage:%d\n", mInfoHead.biSizeImage);
    printf("biXPelsPerMeter:%d\n", mInfoHead.biXPelsPerMeter);
    printf("biYPelsPerMeter:%d\n", mInfoHead.biYPelsPerMeter);
    printf("biClrUsed:%d\n", mInfoHead.biClrUsed);
    printf("biClrImpartant:%d\n", mInfoHead.biClrImpartant);
    return std::string("");
}

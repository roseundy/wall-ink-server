#ifndef IMAGE_h
#define IMAGE_h

#include <string>
#include <vector>

#define X_RES0 800
#define Y_RES0 480

#define X_RES1 400
#define Y_RES1 300

#define X_RES2 800
#define Y_RES2 480

#define X_RES3 800
#define Y_RES3 480

#define X_RES4 400
#define Y_RES4 300

struct reservation {
    public:
        std::string title;
        int startBlock;
        int endBlock;
};

#include "layouts.h"

void setPixel(uint32_t x, uint32_t y, unsigned char color);
void drawRect(int x, int y, int width, int height, unsigned char color);
void initializeImage();
void drawCharacter(int x, int y, char c);
void drawString(int x, int y, std::string str);
void drawFancyString(std::string str, int16_t x, int16_t y);
uint16_t getTextWidth(std::string str); 
bool drawCenteredString(std::string str, int16_t y);
uint16_t getQrCodeSize(std::string str);
uint16_t putQrCode(uint16_t x, uint16_t y, std::string str, uint16_t scale);
unsigned char reverseByte(unsigned char x);
void invert();
void mirror();
void flip();
std::string reservationBlockToTime(int block);
std::string militaryTimeToNormalPersonTime(std::string military);
std::string fancyDateFromYYYY_MM_DD(std::string YYYY_MM_DD);
std::vector<reservation> parseReservations(std::string* reservations, bool only_events);
void checkBattery(int xOffset, int yOffset, float voltage);
uint32_t setSleepTime(uint32_t increment, int daylightSavingsActive); //increment is the target number of seconds between refreshes

#endif

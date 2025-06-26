#include "image.h"
#include "layouts.h"
#include "fonts.h"
#include <sstream>

extern GFXcanvas1* canvas;
extern uint16_t x_res;
extern uint16_t y_res;
extern uint16_t rotation;

void drawRoomDate(std::string roomName, std::string date, std::string displayUrl, std::string qrCodeString, bool drawQR, bool drawURL) { 
    canvas->setFont(&FreeSansBold18pt7b);
    uint16_t room_width = getTextWidth(roomName);
    bool big_name = true;

    if ((room_width + 5) > x_res) {
        big_name = false;
        canvas->setFont(&FreeSansBold12pt7b);
        room_width = getTextWidth(roomName);
    }

    bool emphasis = (room_width + 15) < x_res;

    //draw room name
    drawFancyString(roomName, emphasis ? 15 : 5, 45);

    //draw date
    if (big_name) {
    	canvas->setFont(&FreeSansBold12pt7b);
    } else {
    	canvas->setFont(&FreeSansBold9pt7b);
    }
    drawFancyString(fancyDateFromYYYY_MM_DD(date), emphasis ? 15 : 5, 80);

    //draw rectangle to emphasize title block
    if (emphasis)
       drawRect(0,0,9,98,3);

    if (drawQR) {
       int s = getQrCodeSize(qrCodeString);
       if ((room_width + 15) < (x_res-19-2*2))
           putQrCode(x_res-10-2*s,44-s,qrCodeString, 2);
       else
           putQrCode(x_res-10-2*s,115,qrCodeString, 2);
    }

    if (drawURL) {
       //draw line with URL under date
       canvas->setFont(&FreeSansBold9pt7b);
       drawRect(9,95,x_res-9,3,1);
       drawRect(x_res-36-getTextWidth(displayUrl)-12,95,getTextWidth(displayUrl)+12,3,0);

       //draw base scheduling url
       drawFancyString(displayUrl,x_res-43 - getTextWidth(displayUrl),100);
    } else {
       //draw line under date
       drawRect(9,95,x_res-9,3,1);
    }
}

void drawBottomMessage(std::string time, bool show_usage, int ypos) {
    std::string updated_msg = militaryTimeToNormalPersonTime(time);
    canvas->setFont(&FreeSansBold9pt7b);
    if (show_usage)
    	drawFancyString("Reserved Times Today:", 5, ypos);
    drawFancyString(updated_msg, x_res - getTextWidth(updated_msg) - 10, ypos);
}

void drawTimeBlocks(std::string* reservations, int currentBlock, bool drawArrow) {

    int block_w = x_res / 32;
    int hour_w = block_w * 2;

    //draw times
    canvas->setFont(&FreeSansBold9pt7b);
    for (int hour = 7; hour < 22; hour++) {
        std::stringstream hourString;
        if (hour < 13) {
            hourString << hour;
            hourString << ((hour == 12) ? "p" : "a");
        } else {
            hourString << hour-12;
            hourString << "p";
        }
        if (hourString.str().length() == 1)
            //drawFancyString(hourString.str(), (hour-6)*hour_w - 5 , y_res-6);
            drawFancyString(hourString.str(), (hour-6)*hour_w - 5 , y_res-6);
        else
            //drawFancyString(hourString.str(), (hour-6)*hour_w - 11, y_res-6);
            drawFancyString(hourString.str(), (hour-6)*hour_w - 11, y_res-6);
    }

    //draw blocks
    for (int block = 0; block < 32; block++) {
        drawRect(block*block_w, y_res-47, block_w, 22, 1);
        if (reservations[block].compare("Available") == 0) {
            //eliminate vertical lines
            drawRect(block*block_w, y_res-45, block_w, 18, 0);
        }
    }

    //put rounded corners on ends
    drawRect(0, y_res-45, 2, 18, 1);
    drawRect(0, y_res-47, 1, 2, 0);
    drawRect(1, y_res-47, 1, 1, 0);
    drawRect(2, y_res-45, 1, 1, 1);
    drawRect(0, y_res-27, 1, 2, 0);
    drawRect(1, y_res-26, 1, 1, 0);
    drawRect(2, y_res-28, 1, 1, 1);

    drawRect(x_res-2, y_res-45, 2, 18, 1);
    drawRect(x_res-1, y_res-47, 1, 2, 0);
    drawRect(x_res-2, y_res-47, 1, 1, 0);
    drawRect(x_res-3, y_res-45, 1, 1, 1);
    drawRect(x_res-1, y_res-27, 1, 2, 0);
    drawRect(x_res-2, y_res-26, 1, 1, 0);
    drawRect(x_res-3, y_res-28, 1, 1, 1);

    //round corners if edge case
    for (int block = 0; block < 32; block++) {
        if (reservations[block].compare("Available") == 0) {
            if (block > 0) {
                if (reservations[block-1].compare("Available") != 0) {
                    drawRect(block*block_w - 4, y_res-47, 5, 2, 0);
                    drawRect(block*block_w,     y_res-46, 1, 1, 1);
                    drawRect(block*block_w - 4, y_res-46, 1, 1, 1);
                    drawRect(block*block_w - 4, y_res-27, 5, 2, 0);
                    drawRect(block*block_w,     y_res-27, 1, 1, 1);
                    drawRect(block*block_w - 4, y_res-27, 1, 1, 1);
                    drawRect(block*block_w - 2, y_res-47, 1, 22, 0);
                    drawRect(block*block_w,     y_res-45, 1, 18, 1);
                    drawRect(block*block_w + 1, y_res-45, 1, 1, 1);
                    drawRect(block*block_w + 1, y_res-28, 1, 1, 1);
                }
            }
            if (block < 31) {
                if (reservations[block+1].compare("Available") != 0) {
                    drawRect(block*block_w + block_w-1, y_res-47, 5, 2, 0);
                    drawRect(block*block_w + block_w-1, y_res-46, 1, 1, 1);
                    drawRect(block*block_w + block_w+3, y_res-46, 1, 1, 1);
                    drawRect(block*block_w + block_w-1, y_res-27, 5, 2, 0);
                    drawRect(block*block_w + block_w-1, y_res-27, 1, 1, 1);
                    drawRect(block*block_w + block_w+3, y_res-27, 1, 1, 1);
                    drawRect(block*block_w + block_w+1, y_res-47, 1, 22, 0);
                    drawRect(block*block_w + block_w-1, y_res-45, 1, 18, 1);
                    drawRect(block*block_w + block_w-2, y_res-45, 1, 1, 1);
                    drawRect(block*block_w + block_w-2, y_res-28, 1, 1, 1);
                }
            }
        } else {
            if (block > 0) {
                if (reservations[block-1].compare(reservations[block]) != 0 && reservations[block-1].compare("Available") != 0) {
                    drawRect(block*block_w - 2, y_res-47, 5, 2, 0);
                    drawRect(block*block_w + 2, y_res-46, 1, 1, 1);
                    drawRect(block*block_w - 2, y_res-46, 1, 1, 1);
                    drawRect(block*block_w - 2, y_res-27, 5, 2, 0);
                    drawRect(block*block_w + 2, y_res-27, 1, 1, 1);
                    drawRect(block*block_w - 2, y_res-27, 1, 1, 1);
                    drawRect(block*block_w,     y_res-47, 1, 22, 0);
                }
            }
        }
    }

    //draw arrow
    if (drawArrow) {
       drawRect(currentBlock*block_w, y_res-49, 2, 1, 1);
       drawRect(currentBlock*block_w - 1, y_res-50, 4, 1, 1);
       drawRect(currentBlock*block_w - 2, y_res-51, 6, 1, 1);
       drawRect(currentBlock*block_w - 3, y_res-53, 8, 2, 1);

       //draw time above the arrow
       //canvas->setFont(&FreeSansBold9pt7b);
       //drawFancyString(militaryTimeToNormalPersonTime(reservationBlockToTime(currentBlock-currentBlock%2)), (currentBlock-currentBlock%2)*block_w - 30, 326);
    }
}

int getCurrentBlock (std::string time) {
    int currentBlock;

    currentBlock = (atoi(time.substr(0,2).c_str()) - 6) * 2;
    currentBlock += atoi(time.substr(3,2).c_str()) / 30;
    if (currentBlock < 0)
        currentBlock = 0;
    if (currentBlock > 31)
        currentBlock = 31;

    return currentBlock;
}

int getCurrentEvent (std::vector<reservation> reservs, int currentBlock, bool only_events) {
    int currentEventIndex = -1;
    for (int i = 0; i < reservs.size(); i++) {
        if ((currentBlock >= reservs.at(i).startBlock || only_events) && currentBlock <= reservs.at(i).endBlock) {
            currentEventIndex = i;
	    break;
	}
    }
    return currentEventIndex;
}

void drawCurrentEvent(std::vector<reservation> reservs, int currentBlock, int currentEventIndex, int ypos, bool big, bool only_events) {
    if (big)
    	canvas->setFont(&FreeSansBold18pt7b);
    else
    	canvas->setFont(&FreeSansBold12pt7b);

    if (currentEventIndex == -1) {
        drawFancyString("Room Available", 15, ypos);
	return;
    }

    int startBlock = reservs.at(currentEventIndex).startBlock;
    if (currentBlock > startBlock)
        startBlock = currentBlock;
    std::string currentEventTime = militaryTimeToNormalPersonTime(reservationBlockToTime(startBlock)) + " - " + militaryTimeToNormalPersonTime(reservationBlockToTime(reservs.at(currentEventIndex).endBlock));

    drawFancyString(currentEventTime, 15, ypos);
    ypos += big ? 36 : 30;
    if (big)
        canvas->setFont(&FreeSans18pt7b);
    else
    	canvas->setFont(&FreeSans12pt7b);

   drawFancyString(reservs.at(currentEventIndex).title, 15, ypos);
}

void drawNextEvent(std::vector<reservation> reservs, int currentBlock, int currentEventIndex, reservation tomorrow, int ypos, bool big, bool only_events) {
    std::string nextEventTime;
    std::string nextEventTitle;
    if (big)
    	canvas->setFont(&FreeSans12pt7b);
    else
    	canvas->setFont(&FreeSans9pt7b);
    if (currentEventIndex > 0 && reservs.size() > currentEventIndex+1) {
        nextEventTime = militaryTimeToNormalPersonTime(reservationBlockToTime(reservs.at(currentEventIndex+1).startBlock)) + " - " + militaryTimeToNormalPersonTime(reservationBlockToTime(reservs.at(currentEventIndex+1).endBlock));
        nextEventTitle = reservs.at(currentEventIndex+1).title;
	drawFancyString("Later Today:", 15, ypos);
    } else {
        nextEventTime = militaryTimeToNormalPersonTime(reservationBlockToTime(tomorrow.startBlock)) + " - " + militaryTimeToNormalPersonTime(reservationBlockToTime(tomorrow.endBlock));
        nextEventTitle = tomorrow.title;
	drawFancyString("Tomorrow:", 15, ypos);
    }
    ypos += big ? 35 : 25;
    if (big)
    	canvas->setFont(&FreeSansBold18pt7b);
    else
    	canvas->setFont(&FreeSansBold12pt7b);
    drawFancyString(nextEventTime, 15, ypos);
    ypos += big ? 36 : 30;
    if (big)
    	canvas->setFont(&FreeSans18pt7b);
    else
    	canvas->setFont(&FreeSans12pt7b);
    drawFancyString(nextEventTitle, 15, ypos);
}


//portrait 7", 30 min refresh
void drawImage0(std::string roomName, std::string date, std::string time, std::string* reservations, float voltage, std::string displayUrl, std::string qrCodeString, int daylightSavingsActive, reservation tomorrow, bool no_events, bool only_events) {
    //set sleepTime
    setSleepTime(1800, daylightSavingsActive);

    uint16_t xres = (rotation == 0 ? x_res : y_res);
    uint16_t yres = (rotation == 0 ? y_res : x_res);

    //Draw room name
    canvas->setFont(&FreeSansBold18pt7b);
    canvas->setTextColor(1);
    canvas->setTextWrap(false);
    if (!drawCenteredString(roomName + " Reservations", 32)) {
        if (!drawCenteredString(roomName, 32)) {
            canvas->setFont(&FreeSansBold12pt7b);
            if (!drawCenteredString(roomName + " Reservations", 29)) {
                if (!drawCenteredString(roomName, 29)) {
                    canvas->setFont(&FreeSansBold9pt7b);
                    if (!drawCenteredString(roomName + " Reservations", 28)) {
                        if (!drawCenteredString(roomName, 28)) {
                           canvas->setFont(&FreeSans9pt7b);
                           drawCenteredString(roomName, 28);
                        }
                    }
                }
            }
        }
    }

    canvas->setFont(&FreeSans9pt7b);

    //Draw Date
    drawCenteredString(fancyDateFromYYYY_MM_DD(date), 54);

    //Draw Line
    drawRect(5,60,xres-10,3,1);

    //Time
    drawFancyString("Updated " + militaryTimeToNormalPersonTime(time), 32, yres-10);
    //drawCenteredString(displayUrl, yres-13);

    int currentBlock = getCurrentBlock(time);

    int box_height = 20;
    int x_pos = 20;

    uint16_t boxCoordinates[32][2];
    for (int i=0; i<32; i++) {
           boxCoordinates[i][0] = x_pos; // x
           boxCoordinates[i][1] = 78 + box_height * i; //y
    }

    canvas->setFont(&FreeSans9pt7b);
    //canvas->setFont(&FreeMono9pt7b);
    canvas->setTextColor(1);

    //For each time of day
    for (int i = 0; i < 32; i++) {

        //generate and display time string
        std::ostringstream time;
        std::string ampm = "am";
        int hour = i + 12;
        hour /= 2;
        if (hour > 11)
            ampm = "pm";
        if (hour > 12)
            hour -= 12;
        if (hour < 10)
            time << " ";
        time << hour;
        time << ":";
        time << (i % 2) * 30;
        if (i % 2 == 0)
            time << "0";
        time << ampm;

        //draw box around current time
        if (i == currentBlock) {
            drawRect(boxCoordinates[i][0]-12, boxCoordinates[i][1], 75, box_height+1, 1);
            drawRect(boxCoordinates[i][0]-10, boxCoordinates[i][1]+2, 71, box_height-3, 0);
        }

        drawFancyString(time.str(), boxCoordinates[i][0] - 11, boxCoordinates[i][1]+box_height-5); 

        //draw black boxes
        drawRect(boxCoordinates[i][0]+70, boxCoordinates[i][1], 51, box_height, 1);

        //draw white boxes for open time slots
        if (reservations[i].compare("Available") == 0)
            drawRect(boxCoordinates[i][0]+72, boxCoordinates[i][1]+1, 47, box_height - 2, 0);
    }
    //finish top and bottom of the boxes we just drew
    drawRect(70 + x_pos, 77, 51, 1, 1);
    drawRect(70 + x_pos, 77 + box_height * 32, 51, 2, 1);

    //draw boxes and text for bookings
    std::vector<reservation> reservs = parseReservations(reservations, only_events);
    canvas->setTextColor(0);
    canvas->setFont(&FreeSans9pt7b);
    for (int i = 0; i < reservs.size(); i++) {
	if (reservs.at(i).title.compare("Available") != 0) {
            int h = (reservs.at(i).endBlock - reservs.at(i).startBlock) * box_height;
            int x = boxCoordinates[reservs.at(i).startBlock][0];
            int y = boxCoordinates[reservs.at(i).startBlock][1];
            drawRect(x+123, y-1, 350-x, h+2, 1);
            if (((h / box_height) < 1) || (getTextWidth(reservs.at(i).title) + 127 < xres)) {
               drawFancyString(reservs.at(i).title, x+127, y+h/2+5);
            } else {
	       // find a space after middle of title to split
               int tl = reservs.at(i).title.length();
               int sp = reservs.at(i).title.find(" ", tl/2);
               if (sp != std::string::npos) {
                   std::string part1 = reservs.at(i).title.substr(0, sp+1);
                   std::string part2 = reservs.at(i).title.substr(sp);
                   drawFancyString(part1, x+127, y+h/2-5);
                   drawFancyString(part2, x+127, y+h/2+15);
               } else {
                   // well, we tried
                   drawFancyString(reservs.at(i).title, x+127, y+h/2+5);
               }
            }
        }
    }
    canvas->setTextColor(1);
    canvas->setFont(&FreeSans9pt7b);

    //key
    drawRect(32,yres-59,51,28,1);
    drawRect(34,yres-57,47,24,0);
    drawFancyString("Available",88,yres-39);
    drawRect(191,yres-59,51,28,1);
    drawFancyString("Reserved",244,yres-39);

    //QR
    int s = getQrCodeSize(qrCodeString);
    putQrCode(xres-39-s,yres-39-s,qrCodeString, 2);

    checkBattery(xres-100, yres-100, voltage);
    //mirror();
}

//landscape 4", shows 2 appointments
void drawImage1(std::string roomName, std::string date, std::string time, std::string* reservations, float voltage, std::string displayUrl, std::string qrCodeString, int daylightSavingsActive, reservation tomorrow, bool no_events, bool only_events) {
    //set sleepTime
    setSleepTime(1800, daylightSavingsActive);

    //Draw room name and date
    canvas->setFont(&FreeSansBold12pt7b);
    canvas->setTextColor(1);
    canvas->setTextWrap(false);
    drawCenteredString(roomName, 26);
    drawCenteredString(fancyDateFromYYYY_MM_DD(date), 53);

    //Draw dividing line
    drawRect(0,62,x_res,2,1);

    //Get current block
    int currentBlock = getCurrentBlock(time);

    //Get current event
    std::string currentTitle = reservations[currentBlock];
    std::string currentStart = reservationBlockToTime(currentBlock);
    int blockNextStart = currentBlock;
    while (currentTitle.compare(reservations[++blockNextStart]) == 0 && blockNextStart < 32) {}
    std::string currentEnd = reservationBlockToTime(blockNextStart);
    
    //Draw current event
    std::string currentEventTime = militaryTimeToNormalPersonTime(currentStart) + " - " + militaryTimeToNormalPersonTime(currentEnd);
    canvas->setFont(&FreeSansBold18pt7b);
    drawFancyString(currentEventTime, 8, 100);
    canvas->setFont(&FreeSans18pt7b);
    drawFancyString(currentTitle, 8, 140);

    //Get next event
    if (blockNextStart < 31) {
        std::string nextTitle = reservations[blockNextStart];
        std::string nextStart = reservationBlockToTime(blockNextStart);
		int blockNextEnd = blockNextStart;
		while (nextTitle.compare(reservations[++blockNextEnd]) == 0 && blockNextEnd < 32) {}
        std::string nextEnd = reservationBlockToTime(blockNextEnd);

		//Draw next event
        std::string nextEventTime = militaryTimeToNormalPersonTime(nextStart) + " - " + militaryTimeToNormalPersonTime(nextEnd);
		canvas->setFont(&FreeSansBold12pt7b);
		drawFancyString(nextEventTime, 9, 230);
		canvas->setFont(&FreeSans12pt7b);
		drawFancyString(nextTitle, 8, 260);
    }

    checkBattery(x_res-64, y_res-44, voltage);
}

//7" landscape, shows 2 appointments plus blocks & a QR code, 15 min refresh
void drawImage2(std::string roomName, std::string date, std::string time, std::string* reservations, float voltage, std::string displayUrl, std::string qrCodeString, int daylightSavingsActive, reservation tomorrow, bool no_events, bool only_events) {
    //set sleepTime
    setSleepTime(900, daylightSavingsActive);

    canvas->setTextColor(1);
    canvas->setTextWrap(false);

    drawRoomDate(roomName, date, displayUrl, qrCodeString, true, false);

    //Get current block
    int currentBlock = getCurrentBlock(time);

    //parse reservations
    std::vector<reservation> reservs = parseReservations(reservations, only_events);

    //Get current event
    int currentEventIndex = getCurrentEvent(reservs, currentBlock, only_events);
    
    if (!no_events) {
        //Draw current event
        drawCurrentEvent(reservs, currentBlock, currentEventIndex, 142, true, only_events);

        //Draw next event
        drawNextEvent(reservs, currentBlock, currentEventIndex, tomorrow, 225, true, only_events);
    }

    drawBottomMessage(time, true, y_res-60);

    drawTimeBlocks(reservations, currentBlock, true);
    checkBattery(x_res-100, y_res-100, voltage);
}

//7" landscape, shows 3 appointments QR code plus blocks, 15 min update
void drawImage3(std::string roomName, std::string date, std::string time, std::string* reservations, float voltage, std::string displayUrl, std::string qrCodeString, int daylightSavingsActive, reservation tomorrow, bool no_events, bool only_events) {
    //set sleepTime
    setSleepTime(900, daylightSavingsActive);

    canvas->setTextColor(1);
    canvas->setTextWrap(false);

    //parse reservations
    std::vector<reservation> reservs = parseReservations(reservations, only_events);

    drawRoomDate(roomName, date, displayUrl, qrCodeString, true, false);

    //Get current block
    int currentBlock = getCurrentBlock(time);

    //Get current event
    int currentEventIndex = getCurrentEvent(reservs, currentBlock, only_events);
    
    if (!no_events) {
        //Draw current event
        drawCurrentEvent(reservs, currentBlock, currentEventIndex, 142, true, only_events);

        //Draw next event
        drawNextEvent(reservs, currentBlock, currentEventIndex, tomorrow, 225, true, only_events);

        //Draw next next event
        if (currentEventIndex > 0 && reservs.size() > currentEventIndex+1) {
            drawNextEvent(reservs, currentBlock, currentEventIndex+1, tomorrow, 330, false, only_events);
        }
    }

    drawBottomMessage(time, true, y_res-60);
    drawTimeBlocks(reservations, currentBlock, true);
    checkBattery(x_res-100, y_res-100, voltage);
}

//landscape 4", shows 2 appointments
void drawImage4(std::string roomName, std::string date, std::string time, std::string* reservations, float voltage, std::string displayUrl, std::string qrCodeString, int daylightSavingsActive, reservation tomorrow, bool no_events, bool only_events) {
    //set sleepTime
    setSleepTime(1800, daylightSavingsActive);

    //Draw room name and date
    canvas->setFont(&FreeSansBold12pt7b);
    canvas->setTextColor(1);
    canvas->setTextWrap(false);
    drawCenteredString(roomName, 26);
    drawCenteredString(fancyDateFromYYYY_MM_DD(date), 53);

    //Draw dividing line
    drawRect(0,63,x_res,2,1);

    //Get current block
    int currentBlock = getCurrentBlock(time);

    //Get current event
    std::string currentTitle = reservations[currentBlock];
    std::string currentStart = reservationBlockToTime(currentBlock);
    int blockNextStart = currentBlock;
    while (currentTitle.compare(reservations[++blockNextStart]) == 0 && blockNextStart < 32) {}
    std::string currentEnd = reservationBlockToTime(blockNextStart);
    
    //Draw current event
    std::string currentEventTime = militaryTimeToNormalPersonTime(currentStart) + " - " + militaryTimeToNormalPersonTime(currentEnd);
    canvas->setFont(&FreeSansBold18pt7b);
    drawFancyString(currentEventTime, 8, 101);
    canvas->setFont(&FreeSans18pt7b);
    canvas->setTextWrap(false);
    drawFancyString(currentTitle, 8, 141);
    //Get next event
    if (blockNextStart < 31) {
        std::string nextTitle = reservations[blockNextStart];
        std::string nextStart = reservationBlockToTime(blockNextStart);
		int blockNextEnd = blockNextStart;
		while (nextTitle.compare(reservations[++blockNextEnd]) == 0 && blockNextEnd < 32) {}
        std::string nextEnd = reservationBlockToTime(blockNextEnd);

		//Draw next event
        std::string nextEventTime = militaryTimeToNormalPersonTime(nextStart) + " - " + militaryTimeToNormalPersonTime(nextEnd);
		canvas->setFont(&FreeSansBold12pt7b);
		drawFancyString(nextEventTime, 9, 191);
		canvas->setFont(&FreeSans12pt7b);
		drawFancyString(nextTitle, 8, 221);
    }

    checkBattery(x_res-64, y_res-44, voltage);
    
    //draw times
    canvas->setFont(&FreeSansBold9pt7b);
    for (int hour = 7; hour < 22; hour++) {
        if (hour % 2 == 0) {
            std::stringstream hourString;
            if (hour < 13)
                hourString << hour;
            else
                hourString << hour-12;
            if (hourString.str().length() == 1)
                drawFancyString(hourString.str(), hour*24 - 143, 294);
            else
                drawFancyString(hourString.str(), hour*24 - 147, 294);
        }
    }

    //draw blocks
    for (int block = 0; block < 32; block++) {
        drawRect(block*12+8, 259, 12, 16, 1);
        if (reservations[block].compare("Available") == 0) {
            
            //eliminate vertical lines
            drawRect(block*12+8, 261, 12, 12, 0);

            //put rounded corners on ends
            drawRect(8, 261, 2, 12, 1);
            drawRect(8, 259, 1, 2, 0);
            drawRect(9, 259, 1, 1, 0);
            drawRect(10, 261, 1, 1, 1);
            drawRect(8, 273, 1, 2, 0);
            drawRect(9, 274, 1, 1, 0);
            drawRect(10, 272, 1, 1, 1);

            drawRect(390, 261, 2, 12, 1);
            drawRect(391, 259, 1, 2, 0);
            drawRect(390, 259, 1, 1, 0);
            drawRect(389, 261, 1, 1, 1);
            drawRect(391, 273, 1, 2, 0);
            drawRect(390, 274, 1, 1, 0);
            drawRect(389, 272, 1, 1, 1);
        }
    }

    //round corners if edge case
    for (int block = 0; block < 32; block++) {
        if (reservations[block].compare("Available") == 0) {
            if (block > 0) {
                if (reservations[block-1].compare("Available") != 0) {
                    drawRect(block*12 + 5, 259, 5, 2, 0);
                    drawRect(block*12 + 9, 260, 1, 1, 1);
                    drawRect(block*12 + 5, 260, 1, 1, 1);
                    drawRect(block*12 + 5, 273, 5, 2, 0);
                    drawRect(block*12 + 9, 273, 1, 1, 1);
                    drawRect(block*12 + 5, 273, 1, 1, 1);
                    drawRect(block*12 + 7, 259, 1, 16, 0);
                    drawRect(block*12 + 8, 261, 2, 12, 1);
                    drawRect(block*12 + 10, 261, 1, 1, 1);
                    drawRect(block*12 + 10, 272, 1, 1, 1);
                }
            }
            if (block < 31) {
                if (reservations[block+1].compare("Available") != 0) {
                    drawRect(block*12 + 18, 259, 5, 2, 0);
                    drawRect(block*12 + 18, 260, 1, 1, 1);
                    drawRect(block*12 + 22, 260, 1, 1, 1);
                    drawRect(block*12 + 18, 273, 5, 2, 0);
                    drawRect(block*12 + 18, 273, 1, 1, 1);
                    drawRect(block*12 + 22, 273, 1, 1, 1);
                    drawRect(block*12 + 20, 259, 1, 16, 0);
                    drawRect(block*12 + 18, 261, 2, 12, 1);
                    drawRect(block*12 + 17, 261, 1, 1, 1);
                    drawRect(block*12 + 17, 272, 1, 1, 1);
                }
            }
        } else {
            if (block > 0) {
                if (reservations[block-1].compare(reservations[block]) != 0 && reservations[block-1].compare("Available") != 0) {
                    drawRect(block*12 + 6, 259, 5, 2, 0);
                    drawRect(block*12 + 10, 260, 1, 1, 1);
                    drawRect(block*12 + 6, 260, 1, 1, 1);
                    drawRect(block*12 + 6, 273, 5, 2, 0);
                    drawRect(block*12 + 10, 273, 1, 1, 1);
                    drawRect(block*12 + 6, 273, 1, 1, 1);
                    drawRect(block*12 + 8, 259, 1, 16, 0);
                }
            }
        }
    }
    
    //draw arrow
    drawRect((currentBlock-currentBlock%2)*12 + 8, 257, 2, 1, 1);
    drawRect((currentBlock-currentBlock%2)*12 + 7, 256, 4, 1, 1);
    drawRect((currentBlock-currentBlock%2)*12 + 6, 255, 6, 1, 1);
    drawRect((currentBlock-currentBlock%2)*12 + 5, 253, 8, 2, 1);
}

//layout 5 was for static 7" images, though it is no longer used this way

//landscape 4", shows 2 appointments and has QR code
void drawImage6(std::string roomName, std::string date, std::string time, std::string* reservations, float voltage, std::string displayUrl, std::string qrCodeString, int daylightSavingsActive, reservation tomorrow, bool no_events, bool only_events) {
    //set sleepTime
    setSleepTime(1800, daylightSavingsActive);

    //Draw room name and date
    canvas->setFont(&FreeSansBold12pt7b);
    canvas->setTextColor(1);
    canvas->setTextWrap(false);
    drawFancyString(roomName,9,30);
    canvas->setFont(&FreeSansBold12pt7b);
    drawFancyString(fancyDateFromYYYY_MM_DD(date),9,61);

    //Draw rectangle for title emphasis
    drawRect(0,0,6,76,3);

    //Draw dividing line
    drawRect(6,74,x_res-6,2,1);

    //Get current block
    int currentBlock = getCurrentBlock(time);

    //Get current event
    std::string currentTitle = reservations[currentBlock];
    std::string currentStart = reservationBlockToTime(currentBlock);
    int blockNextStart = currentBlock;
    while (currentTitle.compare(reservations[++blockNextStart]) == 0 && blockNextStart < 32) {}
    std::string currentEnd = reservationBlockToTime(blockNextStart);
    
    //Draw current event
    std::string currentEventTime = militaryTimeToNormalPersonTime(currentStart) + " - " + militaryTimeToNormalPersonTime(currentEnd);
    canvas->setFont(&FreeSansBold18pt7b);
    drawFancyString(currentEventTime, 8, 114);
    canvas->setFont(&FreeSans18pt7b);
    canvas->setTextWrap(false);
    drawFancyString(currentTitle, 8, 154);
    //Get next event
    if (blockNextStart < 31) {
        std::string nextTitle = reservations[blockNextStart];
        std::string nextStart = reservationBlockToTime(blockNextStart);
		int blockNextEnd = blockNextStart;
		while (nextTitle.compare(reservations[++blockNextEnd]) == 0 && blockNextEnd < 32) {}
        std::string nextEnd = reservationBlockToTime(blockNextEnd);

		//Draw next event
        std::string nextEventTime = militaryTimeToNormalPersonTime(nextStart) + " - " + militaryTimeToNormalPersonTime(nextEnd);
		canvas->setFont(&FreeSansBold12pt7b);
		drawFancyString(nextEventTime, 9, 199);
		canvas->setFont(&FreeSans12pt7b);
		drawFancyString(nextTitle, 8, 229);
    }

    
    //draw times
    canvas->setFont(&FreeSansBold9pt7b);
    for (int hour = 7; hour < 22; hour++) {
        if (hour % 2 == 0) {
            std::stringstream hourString;
            if (hour < 13)
                hourString << hour;
            else
                hourString << hour-12;
            if (hourString.str().length() == 1)
                drawFancyString(hourString.str(), hour*24 - 143, 299);
            else
                drawFancyString(hourString.str(), hour*24 - 147, 299);
        }
    }

    //draw blocks
    for (int block = 0; block < 32; block++) {
        drawRect(block*12+8, 259, 12, 16, 1);
        if (reservations[block].compare("Available") == 0) {
            
            //eliminate vertical lines
            drawRect(block*12+8, 261, 12, 12, 0);

            //put rounded corners on ends
            drawRect(8, 261, 2, 12, 1);
            drawRect(8, 259, 1, 2, 0);
            drawRect(9, 259, 1, 1, 0);
            drawRect(10, 261, 1, 1, 1);
            drawRect(8, 273, 1, 2, 0);
            drawRect(9, 274, 1, 1, 0);
            drawRect(10, 272, 1, 1, 1);

            drawRect(390, 261, 2, 12, 1);
            drawRect(391, 259, 1, 2, 0);
            drawRect(390, 259, 1, 1, 0);
            drawRect(389, 261, 1, 1, 1);
            drawRect(391, 273, 1, 2, 0);
            drawRect(390, 274, 1, 1, 0);
            drawRect(389, 272, 1, 1, 1);
        }
    }

    //round corners if edge case
    for (int block = 0; block < 32; block++) {
        if (reservations[block].compare("Available") == 0) {
            if (block > 0) {
                if (reservations[block-1].compare("Available") != 0) {
                    drawRect(block*12 + 5, 259, 5, 2, 0);
                    drawRect(block*12 + 9, 260, 1, 1, 1);
                    drawRect(block*12 + 5, 260, 1, 1, 1);
                    drawRect(block*12 + 5, 273, 5, 2, 0);
                    drawRect(block*12 + 9, 273, 1, 1, 1);
                    drawRect(block*12 + 5, 273, 1, 1, 1);
                    drawRect(block*12 + 7, 259, 1, 16, 0);
                    drawRect(block*12 + 8, 261, 2, 12, 1);
                    drawRect(block*12 + 10, 261, 1, 1, 1);
                    drawRect(block*12 + 10, 272, 1, 1, 1);
                }
            }
            if (block < 31) {
                if (reservations[block+1].compare("Available") != 0) {
                    drawRect(block*12 + 18, 259, 5, 2, 0);
                    drawRect(block*12 + 18, 260, 1, 1, 1);
                    drawRect(block*12 + 22, 260, 1, 1, 1);
                    drawRect(block*12 + 18, 273, 5, 2, 0);
                    drawRect(block*12 + 18, 273, 1, 1, 1);
                    drawRect(block*12 + 22, 273, 1, 1, 1);
                    drawRect(block*12 + 20, 259, 1, 16, 0);
                    drawRect(block*12 + 18, 261, 2, 12, 1);
                    drawRect(block*12 + 17, 261, 1, 1, 1);
                    drawRect(block*12 + 17, 272, 1, 1, 1);
                }
            }
        } else {
            if (block > 0) {
                if (reservations[block-1].compare(reservations[block]) != 0 && reservations[block-1].compare("Available") != 0) {
                    drawRect(block*12 + 6, 259, 5, 2, 0);
                    drawRect(block*12 + 10, 260, 1, 1, 1);
                    drawRect(block*12 + 6, 260, 1, 1, 1);
                    drawRect(block*12 + 6, 273, 5, 2, 0);
                    drawRect(block*12 + 10, 273, 1, 1, 1);
                    drawRect(block*12 + 6, 273, 1, 1, 1);
                    drawRect(block*12 + 8, 259, 1, 16, 0);
                }
            }
        }
    }
    
    //draw arrow
    drawRect((currentBlock-currentBlock%2)*12 + 8, 257, 2, 1, 1);
    drawRect((currentBlock-currentBlock%2)*12 + 7, 256, 4, 1, 1);
    drawRect((currentBlock-currentBlock%2)*12 + 6, 255, 6, 1, 1);
    drawRect((currentBlock-currentBlock%2)*12 + 5, 253, 8, 2, 1);

    putQrCode(333,10,qrCodeString, 2);

    checkBattery(x_res-64, y_res-44, voltage);
}

//7" landscape, shows 2 appointments plus blocks & a QR code, 30 min refresh
void drawImage7(std::string roomName, std::string date, std::string time, std::string* reservations, float voltage, std::string displayUrl, std::string qrCodeString, int daylightSavingsActive, reservation tomorrow, bool no_events, bool only_events) { 
    //set sleepTime
    setSleepTime(1800, daylightSavingsActive);

    canvas->setTextColor(1);
    canvas->setTextWrap(false);

    //parse reservations
    std::vector<reservation> reservs = parseReservations(reservations, only_events);

    drawRoomDate(roomName, date, displayUrl, qrCodeString, true, false);

    //Get current block
    int currentBlock = getCurrentBlock(time);

    //Get current event
    int currentEventIndex = getCurrentEvent(reservs, currentBlock, only_events);
    
    if (!no_events) {
        //Draw current event
        drawCurrentEvent(reservs, currentBlock, currentEventIndex, 142, true, only_events);

        //Draw next event
        drawNextEvent(reservs, currentBlock, currentEventIndex, tomorrow, 225, true, only_events);
    }

    drawBottomMessage(time, true, y_res-60);
    drawTimeBlocks(reservations, currentBlock, true);
    checkBattery(x_res-100, y_res-100, voltage);
}

//layout 8 was for static 4" images, though it is no longer used this way

//landscape 4", shows 2 appointments and has QR code. More battery efficient than layout 6.
void drawImage9(std::string roomName, std::string date, std::string time, std::string* reservations, float voltage, std::string displayUrl, std::string qrCodeString, int daylightSavingsActive, reservation tomorrow, bool no_events, bool only_events) {
    //set sleepTime
    setSleepTime(1800, daylightSavingsActive);

    //parse reservations
    std::vector<reservation> reservs = parseReservations(reservations, only_events);

    //Draw room name and date
    canvas->setFont(&FreeSansBold12pt7b);
    canvas->setTextColor(1);
    canvas->setTextWrap(false);
    drawFancyString(roomName,9,30);
    canvas->setFont(&FreeSansBold12pt7b);
    drawFancyString(fancyDateFromYYYY_MM_DD(date),9,61);

    //Draw rectangle for title emphasis
    drawRect(0,0,6,76,3);

    //Draw dividing line
    drawRect(6,74,x_res-6,2,1);

    //Get current block
    int currentBlock = getCurrentBlock(time);
    
    //Get current event
    int currentEventIndex;
    for (int i = 0; i < reservs.size(); i++) {
        if (currentBlock >= reservs.at(i).startBlock && currentBlock <= reservs.at(i).endBlock)
            currentEventIndex = i;
    }

    //Get current event
    std::string currentTitle = reservations[currentBlock];
    std::string currentStart = reservationBlockToTime(currentBlock);
    int blockNextStart = currentBlock;
    while (currentTitle.compare(reservations[++blockNextStart]) == 0 && blockNextStart < 32) {}
    std::string currentEnd = reservationBlockToTime(blockNextStart);
    
    //Draw current event
    std::string currentEventTime = militaryTimeToNormalPersonTime(reservationBlockToTime(reservs.at(currentEventIndex).startBlock)) + " - " + militaryTimeToNormalPersonTime(currentEnd);
    canvas->setFont(&FreeSansBold18pt7b);
    drawFancyString(currentEventTime, 8, 114);
    canvas->setFont(&FreeSans18pt7b);
    canvas->setTextWrap(false);
    drawFancyString(currentTitle, 8, 154);
    //Get next event
    if (blockNextStart < 31) {
        std::string nextTitle = reservations[blockNextStart];
        std::string nextStart = reservationBlockToTime(blockNextStart);
		int blockNextEnd = blockNextStart;
		while (nextTitle.compare(reservations[++blockNextEnd]) == 0 && blockNextEnd < 32) {}
        std::string nextEnd = reservationBlockToTime(blockNextEnd);

		//Draw next event
        std::string nextEventTime = militaryTimeToNormalPersonTime(nextStart) + " - " + militaryTimeToNormalPersonTime(nextEnd);
		canvas->setFont(&FreeSansBold12pt7b);
		drawFancyString(nextEventTime, 9, 199);
		canvas->setFont(&FreeSans12pt7b);
		drawFancyString(nextTitle, 8, 229);
    }

    
    //draw times
    canvas->setFont(&FreeSansBold9pt7b);
    for (int hour = 7; hour < 22; hour++) {
        if (hour % 2 == 0) {
            std::stringstream hourString;
            if (hour < 13)
                hourString << hour;
            else
                hourString << hour-12;
            if (hourString.str().length() == 1)
                drawFancyString(hourString.str(), hour*24 - 143, 299);
            else
                drawFancyString(hourString.str(), hour*24 - 147, 299);
        }
    }

    //draw blocks
    for (int block = 0; block < 32; block++) {
        drawRect(block*12+8, 259, 12, 16, 1);
        if (reservations[block].compare("Available") == 0) {
            
            //eliminate vertical lines
            drawRect(block*12+8, 261, 12, 12, 0);

            //put rounded corners on ends
            drawRect(8, 261, 2, 12, 1);
            drawRect(8, 259, 1, 2, 0);
            drawRect(9, 259, 1, 1, 0);
            drawRect(10, 261, 1, 1, 1);
            drawRect(8, 273, 1, 2, 0);
            drawRect(9, 274, 1, 1, 0);
            drawRect(10, 272, 1, 1, 1);

            drawRect(390, 261, 2, 12, 1);
            drawRect(391, 259, 1, 2, 0);
            drawRect(390, 259, 1, 1, 0);
            drawRect(389, 261, 1, 1, 1);
            drawRect(391, 273, 1, 2, 0);
            drawRect(390, 274, 1, 1, 0);
            drawRect(389, 272, 1, 1, 1);
        }
    }

    //round corners if edge case
    for (int block = 0; block < 32; block++) {
        if (reservations[block].compare("Available") == 0) {
            if (block > 0) {
                if (reservations[block-1].compare("Available") != 0) {
                    drawRect(block*12 + 5, 259, 5, 2, 0);
                    drawRect(block*12 + 9, 260, 1, 1, 1);
                    drawRect(block*12 + 5, 260, 1, 1, 1);
                    drawRect(block*12 + 5, 273, 5, 2, 0);
                    drawRect(block*12 + 9, 273, 1, 1, 1);
                    drawRect(block*12 + 5, 273, 1, 1, 1);
                    drawRect(block*12 + 7, 259, 1, 16, 0);
                    drawRect(block*12 + 8, 261, 2, 12, 1);
                    drawRect(block*12 + 10, 261, 1, 1, 1);
                    drawRect(block*12 + 10, 272, 1, 1, 1);
                }
            }
            if (block < 31) {
                if (reservations[block+1].compare("Available") != 0) {
                    drawRect(block*12 + 18, 259, 5, 2, 0);
                    drawRect(block*12 + 18, 260, 1, 1, 1);
                    drawRect(block*12 + 22, 260, 1, 1, 1);
                    drawRect(block*12 + 18, 273, 5, 2, 0);
                    drawRect(block*12 + 18, 273, 1, 1, 1);
                    drawRect(block*12 + 22, 273, 1, 1, 1);
                    drawRect(block*12 + 20, 259, 1, 16, 0);
                    drawRect(block*12 + 18, 261, 2, 12, 1);
                    drawRect(block*12 + 17, 261, 1, 1, 1);
                    drawRect(block*12 + 17, 272, 1, 1, 1);
                }
            }
        } else {
            if (block > 0) {
                if (reservations[block-1].compare(reservations[block]) != 0 && reservations[block-1].compare("Available") != 0) {
                    drawRect(block*12 + 6, 259, 5, 2, 0);
                    drawRect(block*12 + 10, 260, 1, 1, 1);
                    drawRect(block*12 + 6, 260, 1, 1, 1);
                    drawRect(block*12 + 6, 273, 5, 2, 0);
                    drawRect(block*12 + 10, 273, 1, 1, 1);
                    drawRect(block*12 + 6, 273, 1, 1, 1);
                    drawRect(block*12 + 8, 259, 1, 16, 0);
                }
            }
        }
    }
    
    //draw arrow
    //drawRect((currentBlock-currentBlock%2)*12 + 8, 257, 2, 1, 1);
    //drawRect((currentBlock-currentBlock%2)*12 + 7, 256, 4, 1, 1);
    //drawRect((currentBlock-currentBlock%2)*12 + 6, 255, 6, 1, 1);
    //drawRect((currentBlock-currentBlock%2)*12 + 5, 253, 8, 2, 1);

    putQrCode(333,10,qrCodeString, 2);

    checkBattery(x_res-64, y_res-44, voltage);
}

//7" landscape, shows 2 appointments plus blocks & a QR code, no time
void drawImage10(std::string roomName, std::string date, std::string time, std::string* reservations, float voltage, std::string displayUrl, std::string qrCodeString, int daylightSavingsActive, reservation tomorrow, bool no_events, bool only_events) {
    //set sleepTime
    setSleepTime(1800, daylightSavingsActive);

    canvas->setTextColor(1);
    canvas->setTextWrap(false);

    drawRoomDate(roomName, date, displayUrl, qrCodeString, true, false);

    //Get current block
    int currentBlock = getCurrentBlock(time);

    //parse reservations
    std::vector<reservation> reservs = parseReservations(reservations, only_events);

    //Get current event
    int currentEventIndex = getCurrentEvent(reservs, currentBlock, only_events);
    
    if (!no_events) {
        //Draw current event
        drawCurrentEvent(reservs, currentBlock, currentEventIndex, 142, true, only_events);

        //Draw next event
        drawNextEvent(reservs, currentBlock, currentEventIndex, tomorrow, 225, true, only_events);
    }

    drawTimeBlocks(reservations, currentBlock, false);
    checkBattery(x_res-100, y_res-100, voltage);
}

//7" landscape, shows 2 appointments plus blocks & a QR code. For events.
void drawImage11(std::string roomName, std::string date, std::string time, std::string* reservations, float voltage, std::string displayUrl, std::string qrCodeString, int daylightSavingsActive, reservation tomorrow, bool no_events, bool only_events) { 
    //set sleepTime
    setSleepTime(1800, daylightSavingsActive);

    canvas->setFont(&FreeSansBold18pt7b);
    canvas->setTextColor(1);
    canvas->setTextWrap(false);

    //parse reservations
    std::vector<reservation> reservs = parseReservations(reservations, only_events);

    drawRoomDate(roomName, date, displayUrl, qrCodeString, true, false);

    //Get current block
    int currentBlock = getCurrentBlock(time);

    //Get current event
    int currentEventIndex;
    for (int i = 0; i < reservs.size(); i++) {
        if (currentBlock >= reservs.at(i).startBlock && currentBlock <= reservs.at(i).endBlock)
            currentEventIndex = i;
    }
    
    if (!no_events) {
        //Draw current event
        std::string currentEventTime = militaryTimeToNormalPersonTime(reservationBlockToTime(currentBlock)) + " - " + militaryTimeToNormalPersonTime(reservationBlockToTime(reservs.at(currentEventIndex).endBlock));
        canvas->setFont(&FreeSansBold18pt7b);
        drawFancyString(currentEventTime, 15, 142);
        canvas->setFont(&FreeSans18pt7b);
        canvas->setTextWrap(false);
        if (reservs.at(currentEventIndex).title.compare("Available") == 0) {
            drawFancyString("No Events", 15, 178);
        } else {
            drawFancyString(reservs.at(currentEventIndex).title, 15, 178);
        }
        canvas->setTextWrap(false);

        //Draw next event
        if (reservs.size() > currentEventIndex+1) {
            std::string nextEventTime = militaryTimeToNormalPersonTime(reservationBlockToTime(reservs.at(currentEventIndex+1).startBlock)) + " - " + militaryTimeToNormalPersonTime(reservationBlockToTime(reservs.at(currentEventIndex+1).endBlock));
		    canvas->setFont(&FreeSansBold12pt7b);
		    drawFancyString(nextEventTime, 15, 221);
		    canvas->setFont(&FreeSans12pt7b);
            canvas->setTextWrap(false);
            if (reservs.at(currentEventIndex+1).title.compare("Available") == 0) {
                drawFancyString("No Events", 15, 251);
            } else {
                drawFancyString(reservs.at(currentEventIndex+1).title, 15, 251);
            }
            canvas->setTextWrap(false);
        }
    }

    drawBottomMessage(time, true, y_res-60);
    drawTimeBlocks(reservations, currentBlock, true);

    checkBattery(x_res-100, y_res-100, voltage);
}

//7" landscape, shows 2 appointments plus blocks & a QR code, 15 min refresh, no time block
void drawImage12(std::string roomName, std::string date, std::string time, std::string* reservations, float voltage, std::string displayUrl, std::string qrCodeString, int daylightSavingsActive, reservation tomorrow, bool no_events, bool only_events) {
    //set sleepTime
    setSleepTime(900, daylightSavingsActive);

    canvas->setTextColor(1);
    canvas->setTextWrap(false);

    drawRoomDate(roomName, date, displayUrl, qrCodeString, true, false);

    //Get current block
    int currentBlock = getCurrentBlock(time);

    //parse reservations
    std::vector<reservation> reservs = parseReservations(reservations, only_events);

    //Get current event
    int currentEventIndex = getCurrentEvent(reservs, currentBlock, only_events);
    
    if (!no_events) {
        //Draw current event
        drawCurrentEvent(reservs, currentBlock, currentEventIndex, 142, true, only_events);

        //Draw next event
        drawNextEvent(reservs, currentBlock, currentEventIndex, tomorrow, 225, true, only_events);
    }

    drawBottomMessage(time, false, y_res-10);
    checkBattery(x_res-100, y_res-100, voltage);
}

//7" landscape, shows 3 appointments QR code plus blocks, 30 min update
void drawImage13(std::string roomName, std::string date, std::string time, std::string* reservations, float voltage, std::string displayUrl, std::string qrCodeString, int daylightSavingsActive, reservation tomorrow, bool no_events, bool only_events) {
    //set sleepTime
    setSleepTime(1800, daylightSavingsActive);

    canvas->setTextColor(1);
    canvas->setTextWrap(false);

    //parse reservations
    std::vector<reservation> reservs = parseReservations(reservations, only_events);

    drawRoomDate(roomName, date, displayUrl, qrCodeString, true, false);

    //Get current block
    int currentBlock = getCurrentBlock(time);

    //Get current event
    int currentEventIndex = getCurrentEvent(reservs, currentBlock, only_events);
    
    if (!no_events) {
        //Draw current event
        drawCurrentEvent(reservs, currentBlock, currentEventIndex, 142, true, only_events);

        //Draw next event
        drawNextEvent(reservs, currentBlock, currentEventIndex, tomorrow, 225, true, only_events);

        //Draw next next event
        if (reservs.size() > currentEventIndex+1) {
            drawNextEvent(reservs, currentBlock, currentEventIndex+1, tomorrow, 330, false, only_events);
        }
    }

    drawBottomMessage(time, true, y_res-60);
    drawTimeBlocks(reservations, currentBlock, true);
    checkBattery(x_res-100, y_res-100, voltage);
}



# Summary
The door display server houses several important functions:

* It generates new images for the displays
* It serves images to the displays
* It notifies the database when the displays are updated
* It hosts a website for the configuration of the displays

The following diagram roughly illustrates the information passed between the parts of the system:

![Door display sequence diagram](https://i.imgur.com/YZ32F0h.png)

# Build
These instructions assume that your web root is hosted in a ```www``` folder adjancent to the folder the repo resides in.
To build and deploy to the test server (hosted at ```../www/test```), go to ```/wall-ink-server``` and use the ```make``` command. You will need ```gcc``` and GNU ```make```. To build and deploy to the live server (hosted at ```../www```), use the command ```make deploy``` instead.

# Device Manager
The Device Manager website is hosted at the web root. It acts as an aid in keeping track of your devices.
## Important Files
The files for the configuration website can be found at ```door-display/image_gen/web/device_manager```.
#### dbconfig.php
Contains information for connecting to the database
#### view_devices.php
This is the homepage; it shows a list of devices
#### edit_device
This file displays an HTML form to the user to allow them to edit the database entry for the device
#### handle_edit_device
A handler for the HTML form generated by ```edit_device.php```
#### handle_delete_device
A handler for the "Delete" button on ```edit_device.php```
#### handle_upload
A handler for the "Upload" button on ```edit_device.php```
#### .htaccess
Makes sure only those with proper credentials can access the site

# Image Generation and Hosting
The image generation code is located at ```door-display/image_gen```. GCC 8.1.0 was used to build it.
## Important Files
#### makefile
Builds the code and deploys it to the test server with ```make```; builds the code and deploys it to the live server with ```make deploy```. Builds the code with debug flags but does not deploy it with ```make debug```.
#### image.cpp
Contains most of the code used to generate the images from reservation data. This file is bloated, and should be split up.
#### compressImage.cpp
Contains the code used to convert an array of bytes into a compressed image for the use of a display
#### fonts.h
Contains include statements for lots of Adafruit fonts so they don't need to be in image.cpp or image.h
#### letters.h
Contains a font that was found on stackoverflow; might be good to get rid of this.
#### sha1.c, sha1.h
Contains a library for sha1 hashing.
#### pbmToCompressed.cpp
Source code to convert a raw, binary .pbm file to a compressed file for the use of a display. Will not work with ASCII .pbm files!
#### qr_code_generator/
Contains a C++ library used by image.cpp to generate QR codes from strings
#### Adafruit-GFX-Library/
Contains a library used main for fonts in image.cpp
#### get_image.php
Takes in a MAC address and a voltage. If the MAC address corresponds to a device which needs a static image (device_type 5), it updates the ```last_checked_in``` and (if relevant) ```batteries_replaced_date``` fields on the database. It then and serves up the static image at ```../www/image_data/$mac_address.static```. Otherwise, it passes the MAC address and voltage to ```get_image.sh```.
#### get_image.sh
Takes in a MAC address and voltage, queries the database for information about appointments, and calls ```genimg```. It also updates the ```last_checked_in``` and (if relevant) ```batteries_replaced_date``` fields on the database.
#### genimg
Binary Linux executable which takes in a file containing information about a screen & its associated room and spits out a compressed image for use on the displays. It uses statically linked libraries, so it should run on most Linux systems.
#### pbmToCompressed
Binary Linux executable which takes in a raw, binary .pbm image and outputs a compressed file for use on the displays. Usage is:
  ```./pbmToCompressed image.pbm outputImage.static```
Must supply an image with the precisely correct resolution for the target display!

# Compression Algorithm
The images are compressed with a very simple run-length encoding algorithm. The first 4 bytes contain the current Unix time. The next 4 bytes contain the Unix time when the device should next wake up and contact the server. The next 20 bytes contain a sha1 hash of the image. The next byte contains the value of the first pixel in the image. The rest of the bytes each contain the number of pixels before a differing pixel is encountered. If the value is greater than 255, more than one byte will be used (for example, 255 255 16 for 526 identical pixels in a row). If the value is an exact multiple of 255, a zero will be appended (for example, 255 255 0 for 510 identical pixels in a row).  For example, this 16x16 image: 
![Picture of the number 5](https://i.imgur.com/71pE4rY.png)
would be encoded as this:
```
2c00 185b 3407 185b 9318 7c12 5b9f c496
ba16 663c d789 e0b6 a346 4269 0011 0e02
0406 0402 0401 0902 0401 0902 0404 0602
0401 0302 0402 0901 0402 0901 0402 0901
0402 0401 0401 0402 0401 0301 0502 0503
0602 0e02 0e11 0a                      
```

On a 7" screen, this reduces the image size from 30 kilobytes to about 5500 kilobytes. Although there are other algorithms which can achieve better compression, this custom algorithm was used because it was relatively easy to implement and we weren't able to find any pre-made compression/decompression software which worked with the ESP8266.

# Database
The ```devices``` table in the door-display database was generated with the following code, and runs on MariaDB:
```sql
CREATE TABLE `devices` (
  `device_id` int(11) NOT NULL AUTO_INCREMENT,
  `resource_id` int(11) NOT NULL,
  `device_type` int(11) NOT NULL,
  `mac_address` char(12) NOT NULL,
  `voltage` float NOT NULL DEFAULT '0',
  `orientation` int(11) NOT NULL DEFAULT '0',
  `last_checked_in` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `batteries_replaced_date` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `firmware_version` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`device_id`),
  UNIQUE KEY `mac_address_UNIQUE` (`mac_address`)
) ENGINE=InnoDB AUTO_INCREMENT=56 DEFAULT CHARSET=latin1
```

The scheduling information was taken from the Booked database, ```collegeresv```. In the near future, we want to set up a system for plugins to be added to interface with other scheduling systems, such as Exchange.

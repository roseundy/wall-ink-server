CXXFLAGS += -static -O1 -g -std=c++11
LIBSRC = BitBuffer QrCode QrSegment compressImage sha1 layouts Adafruit_GFX
objects = image.o pbmToCompressed.o compressImage.o BitBuffer.o QrCode.o QrSegment.o sha1.o layouts.o Adafruit_GFX.o
VPATH = qr_code_generator:web:Adafruit-GFX-Library
CXX=g++
CC=gcc
CFLAGS += -static
SHELL := /bin/bash

#This makefile has recursive test and deploy functions.
#This is to make up for the limitations of make, which can't source bash scripts
$(info MAKELEVEL=$(MAKELEVEL))
ifeq ($(MAKELEVEL), 0)
test:
	bash -c "source ./web/config/settings.cfg; \
		for var in \$$(compgen -v); do export \$$var; done; \
		$(MAKE) $@"
else
test: genimg pbmToCompressed genconfig gcal
	rm -rf $(webDirectory)/test/
	mkdir $(webDirectory)/test
	mkdir $(webDirectory)/test/image_data
	mkdir $(webDirectory)/test/voltage_monitor
	mkdir $(webDirectory)/test/voltage_monitor/data
	cp -r web/google $(webDirectory)/test/google
	cp -r web/config $(webDirectory)/test/config
	cp -r web/device_manager $(webDirectory)/test/device_manager
	cp web/.htaccess $(webDirectory)/test/
	cp web/index.html $(webDirectory)/test/
	cp web/genimg $(webDirectory)/test/
	cp web/pbmToCompressed $(webDirectory)/test/
	cp web/get_image.php $(webDirectory)/test/
	cp web/get_image.sh $(webDirectory)/test/
	cp web/booked.sh $(webDirectory)/test/
	cp web/get_png.php $(webDirectory)/test/
	cp web/rawToPng.sh $(webDirectory)/test/
	cp web/unix_time.php $(webDirectory)/test/
	cp web/r.php $(webDirectory)/test/
	chmod -R g+rw $(webDirectory)/test
endif

ifeq ($(MAKELEVEL), 0)
deploy:
	bash -c "source ./web/config/settings.cfg; \
		for var in \$$(compgen -v); do export \$$var; done; \
		$(MAKE) $@"
else
deploy: genimg pbmToCompressed genconfig gcal
	source ./web/config/settings.cfg
	rm -rf $(webDirectory)/google
	rm -rf $(webDirectory)/config
	rm -rf $(webDirectory)/device_manager
	rm -rf $(webDirectory)/image_data
	rm -rf $(webDirectory)/voltage_monitor
	mkdir $(webDirectory)/image_data
	mkdir $(webDirectory)/voltage_monitor
	mkdir $(webDirectory)/voltage_monitor/data
	cp -r web/google $(webDirectory)/google
	cp -r web/config $(webDirectory)/config
	cp -r web/device_manager $(webDirectory)/device_manager
	cp web/.htaccess $(webDirectory)/
	cp web/index.html $(webDirectory)/
	cp web/genimg $(webDirectory)/
	cp web/pbmToCompressed $(webDirectory)/
	cp web/get_image.php $(webDirectory)/
	cp web/get_image.sh $(webDirectory)/
	cp web/booked.sh $(webDirectory)/
	cp web/get_png.php $(webDirectory)/
	cp web/rawToPng.sh $(webDirectory)/
	cp web/unix_time.php $(webDirectory)/
	cp web/r.php $(webDirectory)/
	chmod -Rf g+rw $(webDirectory)
endif

gcal : 
	go build -o web/google/gcal web/google/gcal.go 

genconfig :
	echo "#!/bin/bash" > web/config/database.sh
	echo "#Do not edit this file! It is autogenerated by the makefile, and will be overwritten!" >> web/config/database.sh
	echo "#Instead, edit settings.cfg" >> web/config/database.sh
	cat web/config/settings.cfg >> web/config/database.sh
	cp web/config/settings.cfg web/config/temp
	sed -i 's/^\(.*\)$$/\$$\1;/' web/config/temp
	echo "<?php" > web/config/dbconfig.php
	echo "#Do not edit this file! It is autogenerated by the makefile, and will be overwritten!" >> web/config/dbconfig.php
	echo "#Instead, edit settings.cfg" >> web/config/database.sh
	cat web/config/temp >> web/config/dbconfig.php
	echo "?>" >> web/config/dbconfig.php
	rm web/config/temp

genimg : image.o compressImage.o BitBuffer.o QrCode.o QrSegment.o layouts.o Adafruit_GFX.o
	$(CXX) image.o $(LIBSRC:=.o) $(CXXFLAGS) -o web/genimg

pbmToCompressed : pbmToCompressed.o compressImage.o
	$(CXX) pbmToCompressed.o compressImage.o sha1.o $(CXXFLAGS) -o web/pbmToCompressed

image.o : image.h
pbmToCompressed.o : pbmToCompressed.cpp compressImage.cpp compressImage.h
compressImage.o : compressImage.h sha1.o
BitBuffer.o : BitBuffer.hpp
QrCode.o : QrCode.hpp
QrSegment.o : QrSegment.hpp
sha1.o : sha1.h
layouts.o : layouts.h
Adafruit_GFX.o : Adafruit_GFX.h

debug:
	$(CXX) image.cpp $(LIBSRC:=.cpp) $(CXXFLAGS) -g -o web/genimg
	$(CXX) pbmToCompressed.cpp compressImage.cpp $(CXXFLAGS) -g -o web/pbmToCompressed

clean : 
	rm $(objects)

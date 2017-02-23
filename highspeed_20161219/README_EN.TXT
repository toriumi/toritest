[Start of Document]
==============================================================================
* highspeed camera shooting program  "highspeed" simple manual

                                                  R0  2016/12/19  Yo Namikaze

==============================================================================

* Preparation before making "highspeed"

  Install the SSP library(libssp) in advance.

* Make method

Make sure that highspeed is extracted in the directory where libssp
is installed as follows.

For example, When you installed ssplib on /home/pi/libssp/1.01/,
Please extract highspeed to /home/pi/libssp/1.01/highspeed/

  You will find following files in the directory.
    README_EN.TXT  This file
    README_JP.TXT  README(Japanese edition)
    Makefile       make file
    Highspeed.c    High-speed shooting program
    Preview.c      RAW preview program

If ssplib and highspeed are installed correctly, you can type "make" on
LXTerminal to make highspeed.

---------------------------------------------------------------------------
$ make
---------------------------------------------------------------------------

After a while, the following executable files should be generated.

  highspeed
  preview


* High-speed shooting program execution method

On LXTerminal, please type as follows.

---------------------------------------------------------------------------
$ cd /home/pi/libssp/1.01/highspeed/
$ ./highspeed IMX219_640x480_187fps_10bit_2lane.xml 230 2000 50
---------------------------------------------------------------------------

  IMX219_640x480_187fps_10bit_2lane.xml : xml file
  230                                   : Analog Gain
  2000                                  : DigitalGain
  50                                    : ExposureTime

  These specified values are examples.

* RAW preview program execution method

On LXTerminal, please type as follows, so, you can preview all frames.
---------------------------------------------------------------------------
$ cd /home/pi/libssp/1.01/highspeed/
$ ./preview 640 480 raw/frame*.raw
---------------------------------------------------------------------------

* Commands for real-time preview mode

Q   Termination (common for highspeed / preview)
C   Perform simple white balance gain adjustment
    (common for highspeed / preview)

R   Recording (only for highspeed)


* Command for preview mode (common for highspeed / preview)

Q       Quit
R       Go to recording mode
V       Go to real-time preview mode
W       Movie file output (uncompressed)
W       Movie file output (Motion JPEG format)
A       Movie file output (MP4 format)
P       Pause
S       Jump to start frame
E       Jump to end frame
Z       Export current frame
X       Export entire frames
T       Toggle show / hide message
0 to 5  Adjust preview speed
9       Play with skiping 5 frames
8       Play without skipping
[Space] Show next frame
M       Show previous frame
F       Forward 100 frames
B       Back 100 frames
F       Forward 200 frames
B       Back 200 frames

* Support

If you have any questions and troubles, please do not hesitate to visit our web site and please post your comment at our community.

web site : https://www.visionproc.org/

[End of Document]

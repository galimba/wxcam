wxcam
=====

New optimised filters added to wxcam using Streaming SIMD Extensions (SSE)

======  FIXES ======

++++++++++++

Dependencies

Netbeans 7.2.1 (install CVS plug-in)

SUDO APT-GET INSTALL libwxgtk2.8 libwxgtk2.8-dev libxvidcore4 libxvidcore-dev libv4l-dev cimg-dev libmjpegtools-dev libasound2-dev libgtk2.0-0 libgtk2.0-dev libglade2-dev

++++++++++++

Workarounds

-Issues with: "mjpegtools/jpegutils.h"
sudo apt-get install libmjpegtools-dev

-Issues with: "wx-conf"
sudo apt-get install libwxgtk2.8

-Issues with: "ALSA"
sudo apt-get install libasound2-dev

-Issues with: "CImg"
sudo apt-get install cimg-dev

-Issues with: "XvidEncoder"
sudo apt-get install libxvidcore-dev

-To fix: "unable to locate theme engine in module_path"
sudo apt-get install gtk2-engines-pixbuf

-To fix: "video device not found"
su su
rmmod uvcvideo
modprobe uvcvideo
chmod 777 /dev/video0

-To fix issues with the menu
sudo chmod 777 /dev/video0

wxcam
=====

New optimised filters added to wxcam, using  Streaming SIMD Extensions (SSE)

======  FIXES ======

++++++++++++
DEPENDENCIAS

Netbeans 7.2.1, instalar plug-in de CVS

SUDO APT-GET INSTALL libwxgtk2.8 libwxgtk2.8-dev libxvidcore4 libxvidcore-dev libv4l-dev cimg-dev libmjpegtools-dev libasound2-dev libgtk2.0-0 libgtk2.0-dev libglade2-dev

++++++++++++

-Para solucionar problemas con [mjpegtools/jpegutils.h]
sudo apt-get install libmjpegtools-dev

-Para solucionar problemas con [wx-conf]
sudo apt-get install libwxgtk2.8

-Para solucionar problemas con [ALSA]
sudo apt-get install libasound2-dev

-Para solucionar problemas con [CImg]
sudo apt-get install cimg-dev

-Para solucionar problemas con [XvidEncoder]
sudo apt-get install libxvidcore-dev

-Para solucionar unable to locate theme engine in module_path
sudo apt-get install gtk2-engines-pixbuf

-Para solucionar video device not found
su su
rmmod uvcvideo
modprobe uvcvideo
chmod 777 /dev/video0

======================================
-Para solucionar problemas con el menú
sudo chmod 777 /dev/video0
======================================

-Lo podríamos solucionar nosotros =>
avilib.c: In function ‘AVI_open_input_file’:
avilib.c:646:8: warning: variable ‘auds_strf_seen’ set but not used [-Wunused-but-set-variable]

-Lo podríamos solucionar nosotros =>
ccvt.c: In function ‘ccvt_yuyv’:
ccvt.c:314:20: warning: variable ‘linewidth’ set but not used [-Wunused-but-set-variable]

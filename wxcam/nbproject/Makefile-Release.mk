#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=nasm

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/blueonly.o \
	${OBJECTDIR}/src/negative.o \
	${OBJECTDIR}/src/v4l1.o \
	${OBJECTDIR}/src/contrast.o \
	${OBJECTDIR}/src/uncompressed.o \
	${OBJECTDIR}/src/wxcam.o \
	${OBJECTDIR}/ccvt/ccvt.o \
	${OBJECTDIR}/src/motion.o \
	${OBJECTDIR}/src/filters.o \
	${OBJECTDIR}/src/mddialog.o \
	${OBJECTDIR}/src/recording.o \
	${OBJECTDIR}/src/bona.o \
	${OBJECTDIR}/src/pixelate.o \
	${OBJECTDIR}/src/brightness.o \
	${OBJECTDIR}/src/edge.o \
	${OBJECTDIR}/src/audio.o \
	${OBJECTDIR}/src/picture.o \
	${OBJECTDIR}/src/setting.o \
	${OBJECTDIR}/src/mono.o \
	${OBJECTDIR}/src/configure.o \
	${OBJECTDIR}/src/xvidcodec.o \
	${OBJECTDIR}/src/blur.o \
	${OBJECTDIR}/src/progressdlg.o \
	${OBJECTDIR}/src/blushing.o \
	${OBJECTDIR}/src/XvidEncoder.o \
	${OBJECTDIR}/src/stretch.o \
	${OBJECTDIR}/src/avilib.o \
	${OBJECTDIR}/src/median.o \
	${OBJECTDIR}/src/device.o \
	${OBJECTDIR}/src/vidiostd.o \
	${OBJECTDIR}/src/verticalmirror.o \
	${OBJECTDIR}/src/cfile.o \
	${OBJECTDIR}/src/BaseEncoder.o \
	${OBJECTDIR}/src/resolution.o \
	${OBJECTDIR}/src/revelcore.o \
	${OBJECTDIR}/src/v4l2.o \
	${OBJECTDIR}/src/frame.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-ggdb `wx-config --cxxflags`  
CXXFLAGS=-ggdb `wx-config --cxxflags`  

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-g -f elf64

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/local/lib -lxvidcore -llavjpeg -lmjpegutils -lX11 -lasound `wx-config --libs`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk bin/wxcam

bin/wxcam: ${OBJECTFILES}
	${MKDIR} -p bin
	${LINK.cc} -o bin/wxcam ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/src/blueonly.o: nbproject/Makefile-${CND_CONF}.mk src/blueonly.asm 
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/src/blueonly.o src/blueonly.asm

${OBJECTDIR}/src/negative.o: nbproject/Makefile-${CND_CONF}.mk src/negative.asm 
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/src/negative.o src/negative.asm

${OBJECTDIR}/src/v4l1.o: nbproject/Makefile-${CND_CONF}.mk src/v4l1.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/v4l1.o src/v4l1.cpp

${OBJECTDIR}/src/contrast.o: nbproject/Makefile-${CND_CONF}.mk src/contrast.asm 
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/src/contrast.o src/contrast.asm

${OBJECTDIR}/src/uncompressed.o: nbproject/Makefile-${CND_CONF}.mk src/uncompressed.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/uncompressed.o src/uncompressed.cpp

${OBJECTDIR}/src/wxcam.o: nbproject/Makefile-${CND_CONF}.mk src/wxcam.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/wxcam.o src/wxcam.cpp

${OBJECTDIR}/ccvt/ccvt.o: nbproject/Makefile-${CND_CONF}.mk ccvt/ccvt.c 
	${MKDIR} -p ${OBJECTDIR}/ccvt
	${RM} $@.d
	$(COMPILE.c) -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/ccvt/ccvt.o ccvt/ccvt.c

${OBJECTDIR}/src/motion.o: nbproject/Makefile-${CND_CONF}.mk src/motion.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/motion.o src/motion.cpp

${OBJECTDIR}/src/filters.o: nbproject/Makefile-${CND_CONF}.mk src/filters.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/filters.o src/filters.cpp

${OBJECTDIR}/src/mddialog.o: nbproject/Makefile-${CND_CONF}.mk src/mddialog.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/mddialog.o src/mddialog.cpp

${OBJECTDIR}/src/recording.o: nbproject/Makefile-${CND_CONF}.mk src/recording.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/recording.o src/recording.cpp

${OBJECTDIR}/src/bona.o: nbproject/Makefile-${CND_CONF}.mk src/bona.asm 
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/src/bona.o src/bona.asm

${OBJECTDIR}/src/pixelate.o: nbproject/Makefile-${CND_CONF}.mk src/pixelate.asm 
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/src/pixelate.o src/pixelate.asm

${OBJECTDIR}/src/brightness.o: nbproject/Makefile-${CND_CONF}.mk src/brightness.asm 
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/src/brightness.o src/brightness.asm

${OBJECTDIR}/src/edge.o: nbproject/Makefile-${CND_CONF}.mk src/edge.asm 
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/src/edge.o src/edge.asm

${OBJECTDIR}/src/audio.o: nbproject/Makefile-${CND_CONF}.mk src/audio.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/audio.o src/audio.cpp

${OBJECTDIR}/src/picture.o: nbproject/Makefile-${CND_CONF}.mk src/picture.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/picture.o src/picture.cpp

${OBJECTDIR}/src/setting.o: nbproject/Makefile-${CND_CONF}.mk src/setting.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/setting.o src/setting.cpp

${OBJECTDIR}/src/mono.o: nbproject/Makefile-${CND_CONF}.mk src/mono.asm 
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/src/mono.o src/mono.asm

${OBJECTDIR}/src/configure.o: nbproject/Makefile-${CND_CONF}.mk src/configure.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/configure.o src/configure.cpp

${OBJECTDIR}/src/xvidcodec.o: nbproject/Makefile-${CND_CONF}.mk src/xvidcodec.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/xvidcodec.o src/xvidcodec.cpp

${OBJECTDIR}/src/blur.o: nbproject/Makefile-${CND_CONF}.mk src/blur.asm 
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/src/blur.o src/blur.asm

${OBJECTDIR}/src/progressdlg.o: nbproject/Makefile-${CND_CONF}.mk src/progressdlg.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/progressdlg.o src/progressdlg.cpp

${OBJECTDIR}/src/blushing.o: nbproject/Makefile-${CND_CONF}.mk src/blushing.asm 
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/src/blushing.o src/blushing.asm

${OBJECTDIR}/src/XvidEncoder.o: nbproject/Makefile-${CND_CONF}.mk src/XvidEncoder.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/XvidEncoder.o src/XvidEncoder.cpp

${OBJECTDIR}/src/stretch.o: nbproject/Makefile-${CND_CONF}.mk src/stretch.asm 
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/src/stretch.o src/stretch.asm

${OBJECTDIR}/src/avilib.o: nbproject/Makefile-${CND_CONF}.mk src/avilib.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/avilib.o src/avilib.c

${OBJECTDIR}/src/median.o: nbproject/Makefile-${CND_CONF}.mk src/median.asm 
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/src/median.o src/median.asm

${OBJECTDIR}/src/device.o: nbproject/Makefile-${CND_CONF}.mk src/device.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/device.o src/device.cpp

${OBJECTDIR}/src/vidiostd.o: nbproject/Makefile-${CND_CONF}.mk src/vidiostd.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/vidiostd.o src/vidiostd.cpp

${OBJECTDIR}/src/verticalmirror.o: nbproject/Makefile-${CND_CONF}.mk src/verticalmirror.asm 
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -o ${OBJECTDIR}/src/verticalmirror.o src/verticalmirror.asm

${OBJECTDIR}/src/cfile.o: nbproject/Makefile-${CND_CONF}.mk src/cfile.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/cfile.o src/cfile.cpp

${OBJECTDIR}/src/BaseEncoder.o: nbproject/Makefile-${CND_CONF}.mk src/BaseEncoder.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/BaseEncoder.o src/BaseEncoder.cpp

${OBJECTDIR}/src/resolution.o: nbproject/Makefile-${CND_CONF}.mk src/resolution.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/resolution.o src/resolution.cpp

${OBJECTDIR}/src/revelcore.o: nbproject/Makefile-${CND_CONF}.mk src/revelcore.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/revelcore.o src/revelcore.cpp

${OBJECTDIR}/src/v4l2.o: nbproject/Makefile-${CND_CONF}.mk src/v4l2.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/v4l2.o src/v4l2.cpp

${OBJECTDIR}/src/frame.o: nbproject/Makefile-${CND_CONF}.mk src/frame.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -Iccvt -Iimages -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/frame.o src/frame.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} bin/wxcam

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc

#!/bin/sh

DIR='./'
SHA1=`git log --pretty=format:%h -n1`
projectversion=`git describe --tags --long`
cleanversion=${projectversion%%-*}

VER_MAJ=`echo $cleanversion | sed -n 's/.*v\([0-9]\+\).*/\1/p'`
VER_MIN=`echo $cleanversion | sed -n 's/.*v[0-9]\+\.\([0-9]\+\).*/\1/p'`
VER_REV=`echo $cleanversion | sed -n 's/.*v[0-9]\+\.[0-9]\+\.\([0-9]\+\).*/\1/p'`

DEF_SHA1="#define SHA1 0x$SHA1"
DEF_VER_MAJ="#define VER_SW_MAJOR		$VER_MAJ"
DEF_VER_MIN="#define VER_SW_MINOR		$VER_MIN"
DEF_VER_REV="#define VER_SW_REVISION	$VER_REV"

if [ ! "$SHA1" = "" ]; then 
	echo $DEF_SHA1 >> "${DIR}/version.h"
fi

if [ ! "$VER_MAJ" = "" ]; then 
	echo $DEF_VER_MAJ >> "${DIR}/version.h"
fi

if [ ! "$VER_MIN" = "" ]; then 
	echo $DEF_VER_MIN >> "${DIR}/version.h"
fi

if [ ! "$VER_REV" = "" ]; then 
	echo $DEF_VER_REV >> "${DIR}/version.h"
fi

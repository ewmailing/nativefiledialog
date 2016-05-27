#!/bin/sh

(cd cmake_ndk_build
	for d in */ ; do
		(cd "$d"
			make -j2
		)
	done
)


cp -f ../AndroidCMake/Android.mk dist
mkdir -p dist/include
rsync -avz ../BlurrrCore.h dist/include/


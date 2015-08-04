#!/bin/bash

# usage: make.sh [cbrvdp]
# c-> Clean
# b-> Build
# b->r-> Rebuild
# b->v-> Verbose
# b->d-> Debug
# p-> Push

ARGS=$1
CMDLINE="NDK_PROJECT_PATH=. \
APP_BUILD_SCRIPT=Android.mk \
NDK_APPLICATION_MK=Application.mk \
NDK_OUT=build/android/out/obj \
NDK_LIBS_OUT=build/android/out/libs"

mkdir -p build/android/out/{obj,libs}

if [[ $ARGS == *"c"* ]]; then
  ndk-build clean $CMDLINE
fi

if [[ $ARGS == *"b"* ]]; then
  if [[ $ARGS == *"r"* ]]; then
    CMDLINE="${CMDLINE} \
    -B"
  fi
  if [[ $ARGS == *"v"* ]]; then
    CMDLINE="${CMDLINE} \
    V=1"
  fi
  if [[ $ARGS == *"d"* ]]; then
    CMDLINE="${CMDLINE} \
    NDK_DEBUG=1"
  fi
  ndk-build $CMDLINE
fi

if [[ $ARGS == *"p"* ]]; then
  adb devices
  adb push build/android/out/libs/arm*/cbump /data/tmp
  adb shell "su -c chmod 755 /data/tmp/cbump"
fi

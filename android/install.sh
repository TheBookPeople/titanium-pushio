#!/bin/bash
ant clean dist
cd ./dist
unzip uk.co.tbp.pushio-android-1.3.0.zip
cp -a modules/android/uk.co.tbp.pushio ~/Library/Application\ Support/Titanium/modules/android

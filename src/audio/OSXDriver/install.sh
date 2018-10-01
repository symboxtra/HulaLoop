#!/bin/bash

echo
echo "======== Post-build HAL Plug-In Install ========"
echo
echo "Copying build output to HAL Plug-In folder..."
echo "IMPORTANT: This requires root permissions."
echo "           If xcodebuild hangs, it's waiting for root password."
echo "           Interrupt it and run it with sudo, or copy the files yourself using:"
echo "    sudo cp -rf ${BUILT_PRODUCTS_DIR}/${EXECUTABLE_NAME}.driver ${LOCAL_LIBRARY_DIR}/Audio/Plug-Ins/HAL"
echo

# Copy the driver to the HAL folder
$(set -x && sudo cp -rf ${BUILT_PRODUCTS_DIR}/${EXECUTABLE_NAME}.driver ${LOCAL_LIBRARY_DIR}/Audio/Plug-Ins/HAL)

echo "Restarting CoreAudio service..."

# Restart CoreAudio after install
sudo kill `ps -ax | grep 'coreaudio[d]' | grep 'sbin' | awk '{print $1}'`

echo
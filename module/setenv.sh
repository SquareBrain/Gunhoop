#!/bin/sh

# duye library root path
export DUYE_ROOT=`pwd`/..
export DUYE_LIB=$DUYE_ROOT/duye/lib
export DUYE_BIN=$DUYE_ROOT/duye/bin

echo "DUYE_ROOT=$DUYE_ROOT"
echo "DUYE_LIB=$DUYE_LIB"
echo "DUYE_BIN=$DUYE_BIN"

# duye library install path
echo "DUYE_INSTALL_PATH=$install_path"
export DUYE_INSTALL_DIR=$install_path
echo "DUYE_INSTALL_PATH=$DUYE_INSTALL_PATH"

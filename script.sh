#!/bin/bash
# reset with pins first
sudo dfu-programmer atmega16u2 erase
sudo dfu-programmer atmega16u2 flash "$1"
sudo dfu-programmer atmega16u2 reset

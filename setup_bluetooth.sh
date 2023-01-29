#!/bin/bash
#script for setting up bluetooth

sudo service bluetooth stop
sudo bluetoothd --compat &
sudo hciconfig hci0 up

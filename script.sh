#!/bin/sh
df | cut -d % -f2 | sed 's/\ \ \ \ //' | grep / > devices.txt

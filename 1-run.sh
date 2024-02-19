#!/bin/bash


docker run -i -e DISPLAY=:0.0 -v /tmp/.X11-unix/:/tmp/.X11-unix/  -v ./dragon/src:/root/dragon/src -t emulates/dragon32 /bin/bash


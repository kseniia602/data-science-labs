#!/bin/bash
# Примусове програмне малювання для стабільності у VMware
export LIBGL_ALWAYS_SOFTWARE=1
export MESA_LOADER_DRIVER_OVERRIDE=llvmpipe
./build/lab7
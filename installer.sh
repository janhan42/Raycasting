#!/bin/bash

git clone https://github.com/janhan42/Raycasting.git
cd Raycasting
bash -c "$(curl -fsSL https://raw.githubusercontent.com/janhan42/SFML-installer/master/installer.sh)"
mkdir .OBJ
make
./Raycasting

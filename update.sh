#!/bin/bash

# Cloning into teamfcm/futils



if [ ! -d ".dl_futils" ]; then
    echo " - Downloading futils..."
    git clone -b master --single-branch git@github.com:ganzf/futils.git .dl_futils --depth=1
    echo " - Done."
fi

cd .dl_futils
echo "Fetching update..."
git pull origin master
cd ..

if [ ! -d "./futils" ]; then
   mkdir ./futils
fi

ls ./futils > oldUtils
cp .dl_futils/utils/* ./futils/
ls ./futils > newUtils
diff oldUtils newUtils
rm -f oldUtils
rm -f newUtils
cp .dl_futils/projects/fender/release/libfender.a ./lib
cp .dl_futils/projects/fender/include/*.hpp ./include

echo "Update complete."

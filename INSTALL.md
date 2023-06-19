## Install hul_common_lib
### Build example

```
mkdir hul_common_lib
cd  hul_common_lib
mkdir build install

git clone

cmake \
   -S . \
   -B ./build \
   -DCMAKE_INSTALL_PREFIX=./install
cd build
make
make install
```

Do not build and install under the source directory.
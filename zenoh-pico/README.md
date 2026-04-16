# zenoh-pico example

* Build the zenoh-pico first

```bash
mkdir -p zenoh-pico-build zenoh-pico-install
cd zenoh-pico-build
cmake ../zenoh-pico \
      -DCMAKE_INSTALL_PREFIX=../zenoh-pico-install \
      -DCMAKE_BUILD_TYPE=Release \
      -DZ_FEATURE_UNSTABLE_API=1 \
      -DZ_FEATURE_ADVANCED_PUBLICATION=1
make
make install
cd ..
```

`Z_FEATURE_ADVANCED_PUBLICATION` needs `Z_FEATURE_UNSTABLE_API=1`. The other required features for this example are already enabled by default in `zenoh-pico`.

* Build the example

```bash
mkdir -p build
cd build
cmake .. \
      -DCMAKE_PREFIX_PATH="$(pwd)/../zenoh-pico-install" \
      -DCMAKE_BUILD_TYPE=Release
make
cd ..
```

If your environment still does not resolve `zenohpico` through `CMAKE_PREFIX_PATH`, use the package config directory directly instead:

```bash
mkdir -p build
cd build
cmake .. \
      -DCMAKE_PREFIX_PATH="$(pwd)/../zenoh-pico-install/lib/cmake/zenohpico" \
      -DCMAKE_BUILD_TYPE=Release
make
cd ..
```

This builds:

* `zp_simple_pub`
* `zp_simple_advanced_pub`

* Run the examples

```bash
./build/zp_simple_pub
./build/zp_simple_advanced_pub
```

* Clean the environment

```bash
rm -rf build zenoh-pico-build zenoh-pico-install
```

# FEUP-EDAA

In Ubuntu, you better install SFML from apt.

```sh
git submodule update --init --recursive
```

```sh
mkdir build
cd build
cmake ..
cmake --build .
./cal_proj view 1 255
```

XML file was obtained from: https://overpass-api.de/api/map?bbox=-8.7863,40.9980,-8.4718,41.3722

Save the file as `map.xml` in path `map/original/map.xml`

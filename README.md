# Advanced Data Structures and Algorithms 🚕

In Ubuntu, you better install SFML from apt.

```sh
sudo apt install libsfml-dev
```

```sh
git submodule update --init --recursive
```

```sh
mkdir build
cd build
cmake ..
cmake --build .
cd ..
build/cal_proj view 1 255
```

XML file was obtained from: https://overpass-api.de/api/map?bbox=-8.7863,40.9980,-8.4718,41.3722

Save the file as `map.xml` in path `map/original/map.xml`

## Voronoi notes


- https://www.youtube.com/watch?v=pUwEp1hs8MM
- http://blog.ivank.net/fortunes-algorithm-and-implementation.html

- Using `valid` flag for *events* is not very effective and practical, however it's necessary, since priority queue (at least C++ implementation) doesn't support removal of an element that isn't the top one.

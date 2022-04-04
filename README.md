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
./da_proj view
```

XML file was obtained from: https://overpass-api.de/api/map?bbox=-8.7863,40.9980,-8.4718,41.3722

Save the file as `map.xml` in path `map/original/map.xml`

## Voronoi notes

- Using `valid` flag for *events* is not very effective and practical, however it's necessary, since priority queue (at least C++ implementation) doesn't support removal of an element that isn't the top one.

- [Voronoi Diagram - Philipp Kindermann](https://www.youtube.com/watch?v=pUwEp1hs8MM)
- [Fortune's Algorithm - Desmos](https://www.desmos.com/calculator/ejatebvup4)
- [Voronoi Generation - Paul Reed](http://paul-reed.co.uk/fortune.htm#findycoord)
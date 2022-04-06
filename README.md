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
- [Fortune's Algorithm - Desmos](https://www.desmos.com/calculator/y1ybdnkvvz)
- [Voronoi Generation - Paul Reed](http://paul-reed.co.uk/fortune.htm#findycoord)

## Strange segfault

When I was running Kosaraju, I kept getting SEGFAULT. I ran valgrind and apparently it said that no more stack could be reserved  (apparently because Linux sets a stack limit of around 8192KB, and my algorithm was recursing pretty deep). To solve this, I removed the stack limit by running

```sh
ulimit -s unlimited
```

This change is not permanent, it only applies to the current shell. To make it permanent, add it to the end of your `~/.bashrc` file, or just run this command which does exactly the same thing:

```sh
sudo echo -e "\nulimit -s unlimited\n" >> ~/.bashrc
```

# Advanced Data Structures and Algorithms 🚕

## Taxi Trajectory Analysis

Our project consists in analysing a set of taxi trips and their trajectories in the city of Porto.

It was divided into two parts, the first one consisting in developing a map matching solution.

## Group

- Diogo Rodrigues (diogo.rodrigues@fe.up.pt)
- Eduardo Correia (up201806613@fe.up.pt)
- João António Sousa (up201806613@fe.up.pt)

## Installing

If you are cloning from GitHub, you should pull all submodules with:

```sh
git submodule update --init --recursive
```

In Ubuntu, you better install SFML from `apt`.

```sh
sudo apt install libsfml-dev
```

## Build

Retrieve `map.xml` and process it:

```sh
cd res
make
```

Create build directory:

```sh
mdkir build
```

Build project using CMAKE:

```sh
cd build 
cmake ..
cmake --build .
```

## Usage

```sh
da_proj <view|view-trips|match-trip>
```
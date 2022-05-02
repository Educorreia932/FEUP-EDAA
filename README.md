# Advanced Data Structures and Algorithms 🚕

## Group

- [Diogo Rodrigues](https://github.com/dmfrodrigues) ([diogo.rodrigues@fe.up.pt](mailto:diogo.rodrigues@fe.up.pt))
- [Eduardo Correia](https://github.com/Educorreia932) ([up201806613@fe.up.pt](mailto:up201806613@fe.up.pt))
- [João António Sousa](https://github.com/JoaoASousa) ([up201806613@fe.up.pt](mailto:up201806613@fe.up.pt))

## Taxi Trajectory Analysis

Our project consists in analysing a set of taxi trips and their trajectories in the city of Porto.

It was divided into two parts, the first one consisting in developing a map matching solution.

## Installing

If you are cloning from GitHub, you should pull all submodules with:

```sh
git submodule update --init --recursive
```

In Ubuntu, you can install SFML from `apt` to accelerate compilation.

```sh
sudo apt install libsfml-dev
```

## Build

Retrieve and process all data:

```sh
cd res
make
```

To compile the project:

```sh
mdkir build
cd build 
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j8
```

## Usage

```sh
da_proj <view|view-trips|match-trip>
```

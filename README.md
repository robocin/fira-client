
# fira-client
> Client base code #vss #LARC #ssl-vision #FiraSim 

## Description
This repository was created for the purpose of helping beginner teams.
This project is capable of:
- receive field and robot information from FiraSim
- Send commands to robots on grSim

Almost every material present here can be found at:

 [RoboCup/ssl-vision](https://github.com/RoboCup-SSL/ssl-vision)

 [Robocin/FIRASim](https://github.com/robocin/FIRASim)


Feel free to ask and contribute too :)

## Dependencies
- Qt Creator
- [protobuf](https://github.com/google/protobuf)

*Tested with: Qt 5.8.0, protoc 3.x, Ubuntu 20.04 LTS.*

## First Steps
1. If you have protobuf installed, check which version you use with the command:
```sh
protoc --version
```

1. If you do not , follow the instructions in [C++ Installation - Unix](https://github.com/google/protobuf/tree/master/src).

1. According to its version download the referent in [protobuf/tags](https://github.com/google/protobuf/tags).

1. Extract the downloaded file and go to `/protobuf-3.6.1/src/` *(example version)*.

1. Copy the google folder and paste it into `/ssl-Client/net`

1. Go to folder `/ssl-Client/net/pb/proto/` and run:
```sh
sh compile.sh
```

*Obs: when building the program, if something referring to `google/protobuf/stubs/common.h` has any errors, it will probably be an incompatibility version problem with protobuf (`net/google`)*.

## Usage example
1. Open the FiraSim.
![](prints/FiraSim.png)

1. Get the Vision multicast adress, Vision multicast port and Command listen port on grSim.
![](prints/ips.png)

1. In Qt Creator IDE open the project thru the `ssl-Clienty.pro` file.

1. Go to `/ssl-Client/main.cpp` and paste the Vision Multicast adress and the Vision Multicast port on `visionCliente` initialization.
![](prints/clientH.png)

1. Go to `/ssl-Client/main.cpp` and paste the Vision Multicast adress and the Command listen port on `commandCliente` initialization.
![](prints/myudpCPP.png)

1. compile running the script compile.sh and run the bin main





#### Contributors: 
- [Renato Sousa](https://github.com/renatoosousa) 
- [Tiago Barros](https://github.com/tsb4)


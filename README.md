# railroad
Railroad - Train Signalling System

train_simu is application to control a simulation of a train traffic and signaling system. 
Features:
The system includes the following components:
Track segments of arbitrary length
1. Connections between segments
2. Signals which control traffic flow and can have two states GREEN or RED
3. Trains which initially placed at specific segment and can move both directions
4. The system automatically find shortest route between start and destination points
for each train
5. User Interface is implemented using XML files which can be modified by
user. XML files serve as blueprints for the system builds

Prerequisites
1. Install TinyXML library for Ubuntu
2. Install cmake 

Compilation: 
git clone https://github.com/satortanya/railroad.git 
cd railroad
mkdir build
cd build/
cmake ..
make

Usage: 
./train_simu // uses hard coded file name - ../resources/railroadlayout1.xml
./train_simu -f ../resources/railroadlayout2.xml // uses provided file name 






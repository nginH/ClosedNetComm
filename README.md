<div style="text-align: center;">

[//]: # (\TODO ADD IMAGE HERE )

# ClosedNetComm

</div>

---

<div style="color:#EDBB89;">
<h3>Local Communication Exploration (Hypothetical Scenario)</h3>

Disclaimer: This scenario is for educational purposes only and does not represent recommended practices.

Imagine a hypothetical situation where two devices on a physically isolated network need to exchange data. This project could explore techniques for establishing communication within such a controlled environment, highlighting the potential security considerations and limitations.

Key Points:

* Communication within a closed network (not connected to the internet).
* Focus on understanding message transmission mechanisms.
* Importance of responsible use and potential security risks.
</div>

# <div id="System-Requirements">System Requirements:</div>
Linux or Unix (MacOs) based operating system.<br>
(and on Windows system you might find difficult you have to import winsock2.h , ws2tcpip.h and windows.h blah blah blah...)<br>

# <div id ="How-to-use">How to use:</div>
Clone this repository<br>
Open terminal

```terminal  
 cd closedNetComm/'Source File'
 ./script.sh
````
> This script will copy the code inside to the 'Source File' to the directory where the C++ header file  is present 


code for server 
```c++
#include<iostreamS.H>
int main(){
      char ip="127.0.0.1";
      int port =6969;
      // server( ip_address, portNumber, logging ( 1 for enable/ 0 for disable)
      server(ip, port, 1);  
}
```

code for client

```c++
#include <iostreamC.h>
int main(){
      char ip="127.0.0.1";
      int port =6969;
      client(ip, port);
}
```

### compile your code
```Terminal
g++ yourFileName.cpp -o outpuFileName -std=c++20 -pthread && ./outputFileName
```

<video autoplay loop playsinline>
 <source src="https://github.com/Harsh69k/ClosedNetComm/assets/80211435/27bc13bf-a944-4d1f-a309-286ec1d4c96e
" type="video/mp4">
</video>


[//]: # (\TODO video of the serer and client running)

>> here's breakdown of above terminal command : <br>
> g++ :- telling terminal to compile for c++ file  the <yourFileName.cpp> this the directory of the cpp  file < -o yourFileName > and output it in a yourFileName file and  < -std=c++20 > we're stating compiler to use c++20 (you can use 11 -upper) and -pthread to include this while compiling cause this file is multithreaded  and  execute ./yourFileName 



```
Networked Keylogger and it's client with network traffic encryption
change ip and port as needed (default is 6969) in keylogger and its client.
the keylogger is a cpp based windows malware that runs in the background and sends the keystrokes to the client.
this can be obfuscatedb or packed to be a stealthy malware with defensive evasion.
 _   _      _                      _          
| \ | | ___| |___      _____  _ __| | __      
|  \| |/ _ \ __\ \ /\ / / _ \| '__| |/ /      
| |\  |  __/ |_ \ V  V / (_) | |  |   <       
|_| \_|\___|\__|_\_/\_/ \___/|_|  |_|\_\      
| |/ /___ _   _| | ___   __ _  __ _  ___ _ __ 
| ' // _ \ | | | |/ _ \ / _` |/ _` |/ _ \ '__|
| . \  __/ |_| | | (_) | (_| | (_| |  __/ |   
|_|\_\___|\__, |_|\___/ \__, |\__, |\___|_|   
          |___/         |___/ |___/           
Compilations
Actual windows Keylogger Malware:
g++ .\keylogger.cpp -o .\keylogger.exe -lws2_32 -lcrypto -lssl

windows client<currently need fixes>
g++ -o .\sneakykeyhooklistener.exe .\sneakykeyhooklistener.cpp -lcrypto -lws2_32


Linux client:
g++ -o sneakykeyhooklistenerlinux sneakykeyhooklistenerlinux.cpp -lssl -lcrypto


[+] Client should be run first before the krylogger for it to connect to the client.
[+] The keylogger timing is in default 15 seconds but can be edited on crono::seconds
[+] usage of it in non-etical purposes does not hold me responsible for any damage or harm caused by it.
[+] This is for educational purposes only

@Author
Azr43lKn1ght a.k.a Nithin Chenthur Prabhu

@contributors
Anuush MB
```

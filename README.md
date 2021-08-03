

```
                                                  _:_
                                                 '-.-'
                                        ()      __.'.__
                                     .-:--:-.  |_______|                _     __     ___    _____    _____ 
                              ()      \____/    \=====/                /_\   / _\   / __\   \_   \   \_   \
                              /\      {====}     )___(                //_\\  \ \   / /       / /\/    / /\/
                   (\=,      //\\      )__(     /_____\              /  _  \ _\ \ / /___  /\/ /_   /\/ /_
   __    |'-'-'|  //  .\    (    )    /____\     |   |               \_/ \_/ \__/ \____/  \____/   \____/  
  /  \   |_____| (( \_  \    )__(      |  |      |   |
  \__/    |===|   ))  `\_)  /____\     |  |      |   |            ___              __   __   __   
 /____\   |   |  (/     \    |  |      |  |      |   |           / __\   /\  /\   /__\ / _\ / _\   
  |  |    |   |   | _.-'|    |  |      |  |      |   |          / /     / /_/ /  /_\   \ \  \ \     
  |__|    )___(    )___(    /____\    /____\    /_____\        / /___  / __  /  //__   _\ \ _\ \     
 (====)  (=====)  (=====)  (======)  (======)  (=======)       \____/  \/ /_/   \__/   \__/ \__/    
 }===={  }====={  }====={  }======{  }======{  }======={
(______)(_______)(_______)(________)(________)(_________)

```


basic ascii chess board from  
https://github.com/Parigyan/ASCII-Chess  
Credit goes to 

@Parigyan   
@sharat20   
@asheeshr  

---  
### This is 2 player chess game.  
  
Player create their chess room and other players join the game room.  
 

When clients make their game room, the server create processes for each.  
Every process has its own port number. Players in game send their moves with socket.
<br><br><br><br>




#### Prerequisites  
***
```Mysql```  ```Ubuntu Posix libraries```  ```GCC```
<br><br><br>

   
#### Compile option
***
client  
```gcc client.c chat_clnt.c chess_clnt.c client.h  -o client -lpthread -std=c99 -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient -D_REENTRANT```  
<br>
server  
```gcc server.c chess_serv.c server.h -o server -lpthread -std=c99 -D_REENTRANT```
<br><br><br>
#### usage
***
You can run server and client with compiled 'client' and 'server' file.  <br>
<br>
run ```./server portnumber``` (ex: $./server 3000)  
run ```./client ip portnumber``` (ex: $./client 127.0.0.1 3000)  <-local IP



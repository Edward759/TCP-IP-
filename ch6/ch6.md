1. - TCP比UDP慢的原因：（1）收发数据前后进行的连接设置及清楚过程；（2）收发数据过程中位保证可靠性而添加的流控制。
   - TCP传输可靠的原因：TCP有流控制机制，而UDP没有
2. c, e
3. IP的作用是让数据包准确传递到主机，UDP的作用是让数据包传递到特定端口。
4. 收发数据量小但需要频繁连接时，UDP更高效。
5. 首次调用sendto()函数时给套接字分配IP和端口。
6. 若与一台主机长时间通信，使用connect()函数可缩短注册IP和端口号的时间，提高整体性能。
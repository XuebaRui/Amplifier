主机到从机控制命令
包头   CF       KK       AA     JJJ      AGC    RES 
5A     21bit   6BIT    5BIT    2BIT   1BIT    5BIT    A5
主机到从机查询命令  
包头   
5B    0  0 0  0 0 0 0 0 b5

从机机到主机返回
  CF       KK       AA     JJJ      AGC    fa  RES
21bit   6BIT    5BIT    2BIT   1BIT     2BIT   3bit  0x00

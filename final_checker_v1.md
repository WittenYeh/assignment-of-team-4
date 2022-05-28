# 第三阶段：v1 （cr. 2022.5.28)

## 重构代码

- 对主体代码增添system类和room类，并调整一些变量所处的空间，将大部分全局变量封装至system类和room类中，提高代码的重用性，体现面向对象编程思想。
- `system`类：
- `room`类：代表不同房间，其封装的变量有

    - `historyRoom`
    - `mode`(1，2，3代表单人,AI,联机)
    - `networkMode` ( 0代表客户端 )
    - `userCnt`表示当前游戏的允许加入人数，有2，3，6三个选项;
    - `roomNumber`表示当前的房间号
    - `countdown`代表计数器
    - `board`代表棋盘
    - `curChosenCoord`当前选择棋子
    - `INIT_GAME,USER_HAS_BEEN_INIT,START_GAME`为游戏初始化变量
    - `curUser`代表当前玩家
    - `coords`代表坐标
    - `users[7]`为玩家数组
    - `userCntInRoom`房间内玩家数量
    - `playerList[7]`为玩家列表
    - `winnerList[7]`胜利者列表
    - 以下为一些棋盘处理函数：
        - `initCoords()`
        - `initUsers()`
        - `next()`
        - `search()`
        - `cancelChoose()`
        - `cancelFeasibility()`
        - `choose(Coord& chosenCoord)`


## 功能增加

- `system`绑定`server`和`client`  （5.29)
  - 变量
    - `system`包含房间`room[10]`,`*curRoom`
    - networkServer
    - networkSocket
  - send
  - receive
  - receiveData
基于Qt的贪吃蛇游戏设计
随便写的，目前存在诸多不足
Design of Greedy Snake Game based on Qt
Written casually, there are currently many shortcomings

+ [x] 窗口构建与UI控件设计 Window Construction and UI Design
+ [x] 控件逻辑 Control logic
+ [x] 游戏逻辑 Game Logic
+ [x] 操作读取 Operation reading
+ [ ] 障碍 Obstacle
+ [ ] 其他敌人 SnakeEnemy

```
width 660 = 33*20
height 580 = 29*20
```

问题bugs：全部解决all fixed

左出界，仅蛇头出界，正常结束(正常)

+ [x] (1)右未出界，距离右边界3，提前正常结束，且蛇头restart后未重绘(界外)
+ [x] (2)上出界，蛇头刚接触边界，渲染在界内，crash
+ [x] (3)下出界，蛇头刚接触边界，渲染在界内，crash
+ [x] (4)不确定原因的转向遇到食物crash

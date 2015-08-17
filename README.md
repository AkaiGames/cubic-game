![](http://imgur.com/download/rQZsfbp) 
##  Cubic Memory Game

Get ready to boost your memory with this awesome game! :)

### 1. Introduction

Cubic is a cross platform game where your goal is to train your memory by memorising patterns of touchable tiles. Currently (0.8.5-alpha) only "Lights up" mode is available, so most instructions here apply to this mode.

### 2. How to play

##### Scoring:

You earn points in each game mode by repeating/finding the pattern as fast as you can. 

##### Time constraint:

The longer you take, the less points you'll get if you touch a correct tile and more points you'll loose if you touch a wrong tile. So hurry up! :) 

##### Leveling:

To level up you need to find/repeat 10 correct sets of tiles in a row.

##### "Lives":

You start the game with 5 lives (which are actually gray squares '-'). You loose a square by clicking a wrong tile. You get a square each time you complete a set of correct tiles, as long as you don't have the max number of squares (which is 5). If you run out of squares/lives, the game is over for you.

##### Formulas:

* Touched correct tile: 

```score += ((totalCorrectSoFar * level)*3) / (secondsElapsed ? secondsElapsed : 1)```

* Touched wrong tile:

```score -= ((totalTouchedSoFar - totalCorrectSoFar) * level) * ((secondsElapsed/3) ? (secondsElapsed/3) : 1)```

### 3. Screenshots:

![](http://i.imgur.com/6j1Oc3Vm.png)
![](http://i.imgur.com/TX5OsVZm.png)
![](http://i.imgur.com/NL1jgMbm.png)

### 4. Download:

Click [here](https://www.dropbox.com/s/rmhkgpc1os5klu7/Cubic-v0.8.5-alpha-release-signed.apk?dl=0) to download the release apk for version 0.8.5-alpha.


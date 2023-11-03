# 参考手册

乐谱由元数据和若干小节构成， `|` 记号用来分隔小节。
小节内可包含：
- 音符
- 和弦
- 宏
- 设置

## 元数据
元数据包含标题、作曲家、乐器三部分，以下为例子：
```
title: Dance of the Yao Tribe
composer: Peng Xiuwen
instrument: Zhongruan
```
注意：元数据必须填写在乐谱开头，且必须包含`title`、`composer`、`instrument`三个字段，暂不支持中文，暂不支持自定义元数据

## 音符

### 音符的表示

音符使用`[>vft]*[nb\#]?[0-7](,+|'+)?`正则表达式进行匹配，一个音符分为四部分：

1. 演奏法：
   1. 普通演奏法：无前缀
   2. 重音：`>`
   3. 顿音：`v`
   4. 轮指（Tremolo）：`t`
   5. 延长（Fermata）：`f`
2. 升（`#`）降(`b`)还原(`n`)号
3. 唱名(`0-7`，其中`0`为休止符)
4. 音符的附加音高：高八度为`'`，低八度为`,`，高八度的高八度为`''`，低八度的低八度为`,,`，以此类推。

合法的音符：

- `#1''`：高两个八度的升do。
- `n2,`：低一个八度的还原2。
- `>v3`：既有顿音又有重音的mi。

非法的音符：

- `bb1,,`：简谱记法里无需支持重升、重降号
- `1'',,`：音符不可既高八度又低八度

### 时值

在音符后加入：

  - 延音线：`-`
  - 下划线：`/`
  - 浮点：`.`

来表示时值，如表示附点八分（十六分）音符，下划线需在浮点之前。什么都不加的音符默认为四分音符，内核最短支持到十六分音符。

## 和弦

和弦使用小括号（`()`）扩起，中间为不含时值的音符，例如`(1,3,n5,)`，内核会自动将其转换为和弦。

和弦的演奏技法在括号前加入，例如`>(1,3,n5,)`，这种情况下，和弦的演奏技法为重音。

和弦的时值在括号后使用下划线（`/`）表示，例如`(1,3,n5,)/`，这种情况下，和弦的时值为八分音符。


## 宏

宏用于表示一些可显示的内容，如强弱等，用中括号表示，大致分为如下几类：

### tempo
例如 `[tempo=123]` 代表当前速度节奏为123。

### 换行
`[newline]` 用于换行。

### 小节线
`[lrep]` `[rrep]` 用于表示左右反复的小节线。
`[end]` 用于表示结束的小节线。

### Volta括号（跳房子）

使用`[volta1begin]` `[volta1end]`来标记一房子的开始和结束，使用`[volta2]`来标记二房子的开始。

### 一般宏

一般宏有如下几种，根据不同特征会显示在谱子的上方、内部或下方：

| 宏            | 名称           | 宏              | 名称            | 宏             | 名称       |
| ------------- | -------------- | --------------- | --------------- | -------------- | ---------- |
| `[f]`         | forte          | `[mf]`          | mezzo forte     | `[ff]`         | fortissimo |
| `[p]`         | piano          | `[mp]`          | mezzo piano     | `[pp]`         | pianissimo |
| `[sfz]`       | sforzando      | `[sfp]`         | sforzando piano | `[sf]`         | sforzato   |
| `[fp]`        | forzando piano | `[cresc.]`      | crescendo       |                |            |
| `[rit.]`      | ritardando     | `[accel.]`      | accelerando     |
| `[div.]`      | divisi         | `[unis.]`       | unisono         | `[solo]`       | solo       |
| `[tutti]`     | tutti          |                 |                 |                |            |
| `[Largo]`     | Largo          | `[Larghetto]`   | Larghetto       | `[Adagio]`     | Adagio     |
| `[Adagietto]` | Adagietto      | `[Andante]`     | Andante         | `[Andantino]`  | Andantino  |
| `[Moderato]`  | Moderato       | `[Allegro]`     | Allegro         | `[Vivace]`     | Vivace     |
| `[Presto]`    | Presto         | `[Prestissimo]` | Prestissimo     | `[Largamente]` | Largamente |
| `[4/4]`       | 4/4拍          | `[3/4]`         | 3/4拍           | `[2/4]`        | 2/4拍      |
| `[1=C]`       | C调            | `[1=G]`         | G调             | `[1=D]`        | D调        |
| `[1=A]`       | A调            | `[1=E]`         | E调             | `[1=B]`        | B调        |
| `[1=F#]`      | F#调           | `[1=C#]`        | C#调            | `[1=F]`        | F调        |
| `[1=Bb]`      | Bb调           | `[1=Eb]`        | Eb调            | `[1=Ab]`       | Ab调       |
| `[1=Db]`      | Db调           | `[1=Gb]`        | Gb调            | `[1=Cb]`       | Cb调       |
| `[arpeggio]`  | 琶音           |                 |                 |                |            |

## 设置

设置用于设置乐谱的一些属性，如调号、拍号等，用大括号表示，现支持的设置有：

- `{typekey:key}`：设置输入使用的调式。`key`为调式的音名，例如`C`、`G`、`bB`等，区分大小写。
- `{displaykey:key}`：设置显示使用的调式。`key`为调式的音名，例如`C`、`G`、`bB`等，区分大小写。
- `{display_sharp}`：设置显示时，当遇到等音（如`#1`和`b2`），更倾向于显示升号。
- `{display_flat}`：同理。
- `{sharp:note}`：设置接下来所有的`note`均自带升号。`note`为唱名，例如`1`、`3`、`5`等。需注意，此设置会影响到所有的音符，直到遇到`{flat:note}`或`{natural:note}`。
- `{flat:note}`：同理。
- `{natural:note}`：同理。

据此，该软件推荐使用两种打谱模式：

- 当翻译五线谱到简谱时：使用`{typekey:C}`，然后再使用`{sharp:note}`或`{flat:note}`来设置升降号，再直接输入五线谱上的音高即可。例如五线谱为`D`调，那么使用`{typekey:C}`，然后再使用`{sharp:1}{sharp:4}`来设置升号。
- 当翻译简谱到其它调时：使用`{typekey:key}`，然后再使用`{displaykey:key}`来设置显示调式，再直接输入简谱上的音高即可。例如简谱为`bB`调，要翻译成`C`调，那么使用`{typekey:bB}{displaykey:C}`，然后再输入即可。
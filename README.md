# genshin_relics
genshin relics tools for evaluating and optimizing

![image](https://github.com/756yang/genshin_relics/blob/main/genshin_relics.png)

this code use qt compile, you can use msvc++ with qt tools for building.
the code only has main program, without picture recognition.
you can change the picture recognition engine, whit cmd "ocr.exe xxx.png > xxx.txt" running.
for release, use paddleocr to recognize the text from picture.

圣遗物特调是一款原神圣遗物工具，它集评分、优化、计算于一体。
[评分]，可以对单个圣遗物进行两种类型的评分，绝对评分使优先属性具有绝对权，相对评分使优先属性相对优先，绝对优先给所有属性评分，而相对优先除了给优先属性评分外只给输出属性评分。绝对评分不收敛，相对评分收敛于最优属性，因此圣遗物点数分配采用相对评分体系，而绝对优先更适合评分体系。相对优先评分说明了各个属性对输出提升，这比绝对优先更准确。
[优化]，即点数分配，一般地，一套圣遗物副属性点数最多450点，而毕业装输出点数才400+，加上武器一般450点左右，这是我们可以分配的点数。优化有两个方向，攻击路线适合前中期输出点数不到350，双暴路线（起始50/100双暴）适合后期点数达400+，主C推荐双暴路线，而辅C或辅助推荐攻击路线，后期辅C转型双暴路线。
[计算]，能够计算一套圣遗物提供的副属性点数以及输出评分。武器和人物自带等属性也可以算到圣遗物里面，输出评分直接表明伤害，优先属性能够提升输出评分，一般输出评分高则圣遗物套装更强。
不仅如此，还支持图片识别，从游戏界面直接截图（Alt+PrtSc）然后粘贴（Ctrl+V）到其中，可自动设置单个圣遗物的属性。

打开图片：打开单个圣遗物的属性截图，并识别。
保存图片：保存打开或粘贴的图片，如果有评分，则打印评分到保存的图片中。
关于：帮助说明
粘贴：打开剪切板中的图片数据（不是文件）并截取一部分作单个圣遗物的属性截图，并识别。
剪贴'：功能同粘贴，只是裁剪方式不一样。
角色预设下拉选框：选择相应的预设，会影响定义的十二个分级。

顶部五个按钮，对应圣遗物总属性，点按会固定属性（点数分配时不改变其属性）。
其下十个按钮，对应十项副属性，与副属性四个按钮配合，以改变副属性项。
副属性四个按钮，点按会出现"[]"以表示选中，然后按下十项副属性按钮，以填入副属性项。
期望总属性下有六个按钮，攻击力%和双暴暴击按钮按下将其期望属性与圣遗物总属性同步，元素爆发率和元素反应率按钮按下分别将圣遗物总元素充能和元素精通用于评分（单个圣遗物评分默认使用固定的元素充能和精通），生命%和防御%按钮也是用作圣遗物总属性固定（其输入框是共用的）。
更改期望/更改圣遗物按钮，下拉选框有十二个用于评分的分级，更改这个选框时影响期望总属性/圣遗物总属性。
期望总属性右边选框，更改优先属性。
圣遗物点数按钮，点按进行点数分配，其右输入框输入0或负值以计算圣遗物副属性点数（固定属性转换为百分比属性计算，会损失一定点数）。
纯输出选框：选中以进行只考虑攻击力和双暴的计算。
生命值lef  防御力def  攻击力atk  暴击率ctr  暴击伤害ctd
元素充能ene  元素精通cx  附加伤害add  加p表示百分比，如patk

可以调整小参，于genshin_relics.ini中已调整完毕，适用于当前游戏角色。
可以自己更改软件的背景图片，分辨率为1024* 768，软件目录下名为background.png的图片。
可以自己调整图片识别参数，于config.txt中已调整完毕，适用于游戏窗口截图分辨率16:9下。
如果游戏分辨率不是16:9，请自己更改genshin_relics.ini中的裁剪参数。

作者：杨玉军
于2021年4月19日完成  5月21日更新

Relic special mixologist is an original relics tool, which integrates scoring, optimization and calculation.
[scoring], there are two types of scoring for a single relic. Absolute scoring gives the priority attribute absolute weight, relative scoring gives the priority attribute relative priority, absolute priority gives all attributes ratings, and relative priority only gives the output attribute ratings except the priority attribute ratings.
The absolute score does not converge, and the relative score converges to the optimal attribute, so the relic points allocation adopts the relative score system, and the absolute priority is more suitable for the score system. Relative priority score shows that each attribute improves the output, which is more accurate than absolute priority.
[optimization], that is, point allocation. Generally, the maximum number of points for a set of relic's secondary attributes is 450, while the number of output points for the graduation suit is only 400 +. In addition, weapons generally have about 450 points. This is the number of points that we can allocate.
There are two directions for optimization. The attack route is suitable for less than 350 output points in the early and middle stages, and the double storm route (50 / 100 double storm at the beginning) is suitable for 400 + in the later stage. The main C recommends the double storm route, while the auxiliary C or auxiliary C recommends the attack route, and the latter auxiliary C transforms into the double storm route.
[calculation], can calculate the secondary attribute points provided by a set of relic and output score. The attributes such as weapons and characters can also be included in the relic. The output score directly indicates the damage. The priority attribute can improve the output score. Generally, if the output score is higher, the relic suit will be stronger.
Not only that, it also supports image recognition. You can take a screenshot from the game interface (ALT+PrtSC) and paste it (Ctrl+V) to it. You can automatically set the attributes of a single relic.

Open Picture: open the attribute screenshot of a single relic and identify it.
Save Picture: save the opened or pasted picture. If there is a score, the score will be printed into the saved picture.
About: help notes
Paste: open the image data (not the file) in the clipboard and take part of it as the attribute screenshot of a single relic, and identify it.
Paste': the function is the same as Paste, but the cutting method is different.
Character preset drop-down box: select the corresponding preset, which will affect the 12 levels defined.

The top five buttons correspond to the total attributes of relic. Pressing them will fix the attributes (the attributes will not be changed when the points are allocated).
The next ten buttons, corresponding to ten sub attributes, cooperate with the four sub attributes buttons to change the sub attributes.
There are four buttons for sub attributes. Click to show "[]" to indicate selection, and then press the ten sub attributes button to fill in the sub attributes.
There are six buttons under the expected total attribute. Press the ATK% and CRIT CRT buttons to synchronize the expected attribute with the total attribute of relics. Press the Ele Obr Rat and Ele Rtr Rat buttons to apply the total element charge and master of relics for scoring respectively (single relic scoring uses fixed element charging and mastery by default), The HP% and DEF% buttons are also used to fix the total attributes of relics (their input fields are shared).
Alter Exp/Alter Relics button. The drop-down box has 12 ratings for scoring. Changing this box will affect the expected/relics total attribute.
Select the box to the right of the expected total attribute and change the priority attribute.
Click the Relic Points button to assign points. Enter 0 or negative value in the right input box to calculate the relics addition attribute points (if the fixed attribute is converted to the percentage attribute, a certain number of points will be lost).
OnlyDMG box: select to calculate only ATK and CRIT.

HP:lef  DEF:def  ATK:atk  CRIT_Rate:ctr  CRIT_DMG:ctd
Ene_Charge:ene  Ele_Master:cx  DMG_Bonus:add  (plus p is the percentage, such as patk)

Author: Yang Yujun
Completed on April 19, 2021  Updated on May 21

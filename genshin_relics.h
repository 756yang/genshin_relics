#pragma once
//UINCODE环境
#ifndef UNICODE
#undef _UNICODE
#else
#ifndef _UNICODE
#define _UNICODE
#endif
#endif

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QDir>
#include <QClipboard>
#include <QMimeData>
#include <QFileDialog>
#include <QTranslator>
#include <QVector>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui_genshin_relics.h"
#include "charset_convert.h"

class genshin_relics : public QMainWindow
{
    Q_OBJECT

public:
    genshin_relics(QWidget *parent = Q_NULLPTR);

    enum addition_attr {
        lef,//生命值
        add=0,//附加伤害
        plef,//生命值%
        def,//防御力
        obr=2,//元素爆发率
        pdef,//防御力%
        atk,//攻击力
        rtr=4,//元素反应率
        patk,//攻击力%
        ctr,//暴击率
        ctd,//暴击伤害
        ene,//元素充能
        cx//元素精通
    };

    //每次强化副属性随机分配7、8、9、10点属性到一个副属性上
    /* this has a bug for assignment is no constexpr */
    constexpr static const double attrpoint[13] = {//属性分配的比值
        76.8,//生命值
        1.5,//生命值%
        6,//防御力
        1.875,//防御力%
        5,//攻击力
        1.5,//攻击力%
        1,//暴击率
        2,//暴击伤害
        5.0/3,//元素充能
        6,//元素精通
        1.5,//元素伤害
        1.875,//物理伤害
        1.155//治疗加成
        //1.875//优先属性的重分配比，适用于另一个圣遗物总评算法
    };

    constexpr static const double attr_ctr=0.3886;//一个属性点的暴击率，百分制，其他属性计算以暴击率为基准

    constexpr static const double mainattr[5] = {//主属性的属性点是固定的，以下是顶级五星圣遗物的主属性点数
        160,//生之花
        160,//死之羽
        80,//时之沙
        80,//空之杯
        80//理之冠
    };

    /* this has a bug for assignment is no constexpr */
    constexpr static const double main_scores[13] = {//主属性的得分
        100.0,//生命值
        74.0,//生命值%
        100.0,//防御力
        64.0,//防御力%
        100.0,//攻击力
        93.0,//攻击力%
        96.0,//暴击率
        90.0,//暴击伤害
        83.0,//元素充能
        80.0,//元素精通
        100.0,//元素伤害
        98.0,//物理伤害
        100.0//治疗加成
    };

    struct relics_degree {
        double relics_atk;
        double relics_ctr;
        double relics_ene;
        double relics_cx;
        double relics_add;
        double exp_atk;
        double exp_ctr;
        double exp_obr;
        double exp_rtr;
        double exp_lef;
        double exp_def;
    };

    struct relics_preset {
        int first;//优先属性
        double preset_obr;//以下是预设属性
        double preset_rtr;
        double preset_atk;
        double preset_ctr;
        double preset_ctd;
        double preset_ene;
        double preset_cx;
        double preset_add;
        double preset_lef;
        double preset_def;
        int index;//是通过预设属性计算出的
    };

    constexpr static const double ATK = 810.0;
    constexpr static const double LEF = 11537.0;
    constexpr static const double DEF = 719.0;

private slots:
    void on_pushButton_15_clicked();
    void on_pushButton_16_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_12_clicked();
    void on_pushButton_13_clicked();
    void on_pushButton_14_clicked();

    void on_lineEdit_6_editingFinished();
    void on_lineEdit_7_editingFinished();
    void on_lineEdit_8_editingFinished();
    void on_lineEdit_9_editingFinished();
    void on_lineEdit_10_editingFinished();
    void on_lineEdit_11_editingFinished();
    void on_lineEdit_12_editingFinished();
    void on_lineEdit_13_editingFinished();
    void on_lineEdit_14_editingFinished();
    void on_lineEdit_15_editingFinished();
    void on_lineEdit_16_editingFinished();
    void on_comboBox_5_currentIndexChanged(int index);
    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();
    void on_pushButton_19_clicked();
    void on_pushButton_20_clicked();
    void on_pushButton_21_clicked();
    void on_pushButton_24_clicked();
    void on_pushButton_25_clicked();
    void on_pushButton_26_clicked();
    void on_pushButton_27_clicked();
    void on_pushButton_28_clicked();
    void on_pushButton_22_clicked();
    void on_pushButton_23_clicked();

    void on_pushButton_29_clicked();
    void on_lineEdit_17_editingFinished();
    void on_checkBox_stateChanged(int state);

    void on_menuOpenSlt();
    void on_menuSaveSlt();
    void on_menuAboutSlt();
    void on_menuPasteSlt();
    void on_menuPaste2Slt();
    void on_menuLangSlt();
    void on_comboBox_character(int index);

private:

    static struct relics_coef {
        int index;//初始属性选择
        struct relics_degree relics_exp[12];//圣遗物期望级别数据
        double prior_attrpoint;//优先属性点与暴击率的比重
        double prior_coef[8];//各种优先属性下的小参
        double prior_gain_coef;//绝对优先下，优先属性收益的倍率
        double command_xe_coef;//精通系数，剧变反应是1，增幅反应是1/2.4，应介于两者之中
        double gain_plef_coef;//绝对优先下，默认生命%的收益倍率
        double gain_pdef_coef;//绝对优先下，默认防御%的收益倍率
        int cut_pictrue;//是否采用自定义的裁剪区域
        double cut_point_x;//裁剪粘贴图片，使用两个点坐标构成的矩形，屏幕分辨率取1920*1080
        double cut_point_y;
        double cut_point_2_x;//裁剪粘贴图片，第二个点坐标
        double cut_point_2_y;
        QVector<struct relics_preset> character_preset;//角色预设数据
    } relics_coef_st;
    double relics_exp_rat[4];//四个点数与标准点数的比值

    static inline QString d2qstring(double a, int n = 2) {
        QString str = QString::number(a,'f', n);
        if (n == 0)return str;
        str.remove(QRegExp("0+$")); // Remove any number of trailing 0's
        str.remove(QRegExp("\\.$")); // If the last character is just a '.' then remove it
        return str;
    }

    static inline QString d2qstring(int len, double a) {
        QString temp=d2qstring(a, 2);
        if (temp.length() < len)return QString("%1").arg(temp, -len);
        temp= d2qstring(a, 1);
        if (temp.length() < len)return QString("%1").arg(temp, -len);
        temp = d2qstring(a, 0);
        return QString("%1").arg(temp, -len);
    }

    static inline bool is_num(int a) {
        return a >= '0'&& a <= '9';
    }

    int language;
    QTranslator *translator;
    Ui::genshin_relicsClass ui;
    QAction *menuOpen;
    QAction *menuSave;
    QAction *menuAbout;
    QAction *menuPaste;
    QAction *menuPaste2;
    QAction *menuLang;
    QAction *action_com_zh;
    QAction *action_com_en;
    QComboBox *combox_zh;
    QComboBox *combox_en;
    int dmg_check_state;
    int relics_addindex;//副属性索引
    int addition_first;//第一条副属性
    int addition_second;//第二条副属性
    int addition_third;//第三条副属性
    int addition_fourth;//第四条副属性

    double relics_atk;//圣遗物攻击力%
    double relics_ctr;//圣遗物暴击率
    double relics_ctd;//圣遗物暴击伤害
    double relics_ene;//圣遗物元素充能
    double relics_cx;//圣遗物元素精通
    double relics_add;//圣遗物附加伤害

    double exp_ctr;//期望双暴暴击
    double exp_atk;//期望攻击力%
    double exp_obr;//期望元素爆发率
    double exp_rtr;//期望元素反应率
    double exp_lef;//期望生命%
    double exp_def;//期望防御%

    int relics_or_exp;//更改期望或更改圣遗物总属性

    double relics;//圣遗物总分，以输出伤害为基准
    double relics_2;//另一种评分机制（relics_accurate和relics_analysis）下的评分
    double total_scores;//单个圣遗物得分，以百分制为基准
    double total_scores_2;
    double addition_scores;//单个圣遗物副属性得分，以百分制为基准
    double addition_scores_2;

    //计算点数分配时固定分配点数到攻击力%(末位)、双暴(其次)、元素充能(其次)、元素精通(其次)、附加伤害(其次)、生命%(其次)、防御%(其次为1则固定为0则不固定)。
    int attr_fixed;
    //期望属性与圣遗物属性关联，攻击力%(末位)、双暴暴击(其次)、元素爆发率(其次)、元素反应率(其次为1则关联为0则不关联)。
    int exp_related;

    bool pic_ready;//是否准备了图片
    int scores_ready;//是否准备了评分数据
    //int attrpoints_float;//可分配的副属性点数

    /* 
     * 收益图
     * 表示了十项副属性的收益（取对数值）
     */
    double attr_gain[10];
    double attr_gain_2[10];

    static int compare_double(const void *a, const void *b);//排序比较

    static double relics_accurate(int prior, const double *attr);//计算准确的能用于优化点数的圣遗物评分
    static void relics_analysis(int prior,const double *attr,double *attr_gain);//计算点数分配的圣遗物增长收益
    static double relics_assign(double attrpoints, int attr_fixed, int prior, double *attr);//分配点数到圣遗物属性上

    static double relics_evaluate(int prior, const double *attr);//计算relics
    static void addition_attr_analysis(int prior, const double *attr, double *attr_gain);//计算收益图
    double addition_attr_evaluate(const double *attr_gain);//计算副属性分
    double main_attr_evaluate();//计算主属性分

    bool clipboard_to_picture(const QString & filePath, int flags = 0);
    void get_pic_attr();
    void draw_gain_attr(int index);

    void retranslateUi();
};

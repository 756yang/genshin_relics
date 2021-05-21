#include "genshin_relics.h"
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

struct genshin_relics::relics_coef genshin_relics::relics_coef_st;

genshin_relics::genshin_relics(QWidget *parent)
    : QMainWindow(parent)
{
    constexpr int FONT_SIZE = 10;
    ui.setupUi(this);
    ui.textBrowser->setText(tr("圣遗物评分为："));
    ui.lineEdit_17->setText("450");
    ui.lineEdit_5->setText("311");
    ui.lineEdit->setText("3.9");
    ui.lineEdit_2->setText("7.8");
    ui.lineEdit_3->setText("35");
    ui.lineEdit_4->setText("6.5");
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background, QBrush(QPixmap(QCoreApplication::applicationDirPath() + QString::fromStdWString(L"/background.png"))));
    setPalette(pal);
    ui.graphicsView->setScene(new QGraphicsScene(this));
    ui.graphicsView_2->setScene(new QGraphicsScene(this));
    //ui.graphicsView->show();
    //ui.graphicsView_2->show();
    menuOpen = new QAction(tr("打开图片"), this);//QT删除父控件时会自动删除子控件，故不需考虑析构
    menuSave = new QAction(tr("保存图片"), this);
    menuAbout = new QAction(tr("关于"), this);
    menuPaste = new QAction(tr("粘贴"), this);
    menuPaste2= new QAction(tr("剪贴\'"), this);
    menuLang = new QAction(tr("简体中文"), this);
    ui.mainToolBar->setStyleSheet("QToolBar {background: white}");
    ui.mainToolBar->addAction(menuOpen);
    connect(menuOpen, SIGNAL(triggered()), this, SLOT(on_menuOpenSlt()));
    ui.mainToolBar->addAction(menuSave);
    connect(menuSave, SIGNAL(triggered()), this, SLOT(on_menuSaveSlt()));
    ui.mainToolBar->addAction(menuAbout);
    connect(menuAbout, SIGNAL(triggered()), this, SLOT(on_menuAboutSlt()));
    ui.mainToolBar->addAction(menuPaste);
    connect(menuPaste, SIGNAL(triggered()), this, SLOT(on_menuPasteSlt()));
    ui.mainToolBar->addAction(menuPaste2);
    connect(menuPaste2, SIGNAL(triggered()), this, SLOT(on_menuPaste2Slt()));
    connect(menuLang, SIGNAL(triggered()), this, SLOT(on_menuLangSlt()));
    ui.mainToolBar->addAction(menuLang);
    menuAbout->setShortcut(QKeySequence::HelpContents);
    menuOpen->setShortcuts(QList<QKeySequence>() << QKeySequence::Open << QKeySequence::Find);
    menuSave->setShortcut(QKeySequence::Save);
    menuPaste->setShortcut(QKeySequence::Paste);
    menuPaste2->setShortcut(QKeySequence("Ctrl+G"));
    combox_en = new QComboBox(this);
    combox_en->setStyleSheet("QComboBox {background: transparent;color: black;border: 1px solid white;padding: 1px 1px 1px 1px;}"
        " QComboBox QAbstractScrollArea QScrollBar:vertical {width: 20px;}");
    combox_en->addItem("default");
    connect(combox_en, SIGNAL(activated(int)), this, SLOT(on_comboBox_character(int)));
    action_com_en = ui.mainToolBar->addWidget(combox_en);//combox_en是mainToolBar的子控件,其独立于com_en而关联之
    ui.mainToolBar->removeAction(action_com_en);
    combox_zh = new QComboBox(this);
    combox_zh->setStyleSheet("QComboBox {background: transparent;color: black;border: 1px solid white;padding: 1px 1px 1px 1px;}"
        " QComboBox QAbstractScrollArea QScrollBar:vertical {width: 20px;}");
    combox_zh->addItem("默认");
    connect(combox_zh, SIGNAL(activated(int)), this, SLOT(on_comboBox_character(int)));
    action_com_zh = ui.mainToolBar->addWidget(combox_zh);
    if (FONT_SIZE != 0){
        menuOpen->setFont(QFont("SimSun", FONT_SIZE, QFont::Normal));
        menuSave->setFont(QFont("SimSun", FONT_SIZE, QFont::Normal));
        menuAbout->setFont(QFont("SimSun", FONT_SIZE, QFont::Normal));
        menuPaste->setFont(QFont("SimSun", FONT_SIZE, QFont::Normal));
        menuPaste2->setFont(QFont("SimSun", FONT_SIZE, QFont::Normal));
        menuLang->setFont(QFont("SimSun", FONT_SIZE, QFont::Normal));
        combox_en->setFont(QFont("SimSun", FONT_SIZE, QFont::Normal));
        combox_zh->setFont(QFont("SimSun", FONT_SIZE, QFont::Normal));
    }
    language = 0;
    QString locale = QLocale::system().name();
    translator = new QTranslator(qApp);
    translator->load("genshin_relics_zh", ":/translations");
    qApp->installTranslator(translator);
    if (locale.left(locale.indexOf('_')).compare("zh", Qt::CaseInsensitive) != 0)on_menuLangSlt();
    struct relics_preset preset;
    relics_addindex = 0;
    QString fileName = QCoreApplication::applicationDirPath() + QString::fromStdWString(L"/genshin_relics.ini");
    int i, index = 0, ini = 0, ini_s = 0;
    qint64 fpos;
    QString str; QStringList strlist;
    QFile file(fileName);
    QString tips1 = tr("重复定义");
    QString tips2 = tr("文件genshin_relics.ini内容错误");
    QString tips3 = tr("数值错误");
    QString tips4 = tr("非正常结束");
    QString tips5 = tr("缺少信息");
    QString tips6 = tr("文件genshin_relics.ini缺少部分配置信息");
    QString tips7 = tr("打开文件错误");
    QString tips8 = tr("找不到文件genshin_relics.ini");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!file.atEnd())
        {
            switch (ini) {
            case 1://[relics_exp]
                if (ini_s & 1)QMessageBox::critical(0, tips1, tips2);
                do {
                    str = file.readLine();
                    str = str.remove(QRegExp("[\\f\\r\\n]"));//删除行尾的字符
                    if (str.startsWith('#'))continue;
                    if (str.contains(QRegExp("[0-9]"))) {
                        index = str.toInt();
                        if (index <= 0 || index > 12) {
                            QMessageBox::critical(0, tips3, tips2);
                        }
                        index--;
                        relics_coef_st.index = index;
                        break;
                    }
                } while (!file.atEnd());
                if (file.atEnd()) {
                    QMessageBox::critical(0, tips4, tips2);
                    break;
                }
                i = 0;
                do {
                    str = file.readLine();
                    str = str.remove(QRegExp("[\\f\\r\\n]"));
                    if (str.startsWith('#'))continue;
                    if (str.contains(QRegExp("[0-9]"))) {
                        strlist = str.split(" ",QString::SkipEmptyParts);
                        if (strlist.length() != 11) {
                            QMessageBox::critical(0, tips3, tips2);
                        }
                        relics_coef_st.relics_exp[i].relics_atk = strlist[0].toDouble();
                        relics_coef_st.relics_exp[i].relics_ctr = strlist[1].toDouble();
                        relics_coef_st.relics_exp[i].relics_ene = strlist[2].toDouble();
                        relics_coef_st.relics_exp[i].relics_cx = strlist[3].toDouble();
                        relics_coef_st.relics_exp[i].relics_add = strlist[4].toDouble();
                        relics_coef_st.relics_exp[i].exp_atk = strlist[5].toDouble();
                        relics_coef_st.relics_exp[i].exp_ctr = strlist[6].toDouble();
                        relics_coef_st.relics_exp[i].exp_obr = strlist[7].toDouble();
                        relics_coef_st.relics_exp[i].exp_rtr = strlist[8].toDouble();
                        relics_coef_st.relics_exp[i].exp_lef = strlist[9].toDouble();
                        relics_coef_st.relics_exp[i].exp_def = strlist[10].toDouble();
                        i++;
                    }
                } while (!file.atEnd() && i < 12);
                ini_s |= 1;
                ini = 0;
                break;
            case 2://[prior_attrpoint]
                if (ini_s & 2)QMessageBox::critical(0, tips1, tips2);
                do {
                    str = file.readLine();
                    str = str.remove(QRegExp("[\\f\\r\\n]"));
                    if (str.startsWith('#'))continue;
                    if (str.contains(QRegExp("[0-9]"))) {
                        relics_coef_st.prior_attrpoint = str.toDouble();
                        if (relics_coef_st.prior_attrpoint <= 0) {
                            QMessageBox::critical(0, tips3, tips2);
                        }
                        break;
                    }
                } while (!file.atEnd());
                ini_s |= 2;
                ini = 0;
                break;
            case 3://[prior_coef]
                if (ini_s & 4)QMessageBox::critical(0, tips1, tips2);
                do {
                    str = file.readLine();
                    str = str.remove(QRegExp("[\\f\\r\\n]"));
                    if (str.startsWith('#'))continue;
                    if (str.contains(QRegExp("[0-9]"))) {
                        strlist = str.split(" ", QString::SkipEmptyParts);
                        if (strlist.length() != 8) {
                            QMessageBox::critical(0, tips3, tips2);
                        }
                        relics_coef_st.prior_coef[0] = strlist[0].toDouble();
                        relics_coef_st.prior_coef[1] = strlist[1].toDouble();
                        relics_coef_st.prior_coef[2] = strlist[2].toDouble();
                        relics_coef_st.prior_coef[3] = strlist[3].toDouble();
                        relics_coef_st.prior_coef[4] = strlist[4].toDouble();
                        relics_coef_st.prior_coef[5] = strlist[5].toDouble();
                        relics_coef_st.prior_coef[6] = strlist[6].toDouble();
                        relics_coef_st.prior_coef[7] = strlist[7].toDouble();
                        break;
                    }
                } while (!file.atEnd());
                ini_s |= 4;
                ini = 0;
                break;
            case 4://[prior_gain_coef]
                if (ini_s & 8)QMessageBox::critical(0, tips1, tips2);
                do {
                    str = file.readLine();
                    str = str.remove(QRegExp("[\\f\\r\\n]"));
                    if (str.startsWith('#'))continue;
                    if (str.contains(QRegExp("[0-9]"))) {
                        relics_coef_st.prior_gain_coef = str.toDouble();
                        if (relics_coef_st.prior_gain_coef <= 0) {
                            QMessageBox::critical(0, tips3, tips2);
                        }
                        break;
                    }
                } while (!file.atEnd());
                ini_s |= 8;
                ini = 0;
                break;
            case 5://[command_xe_coef]
                if (ini_s & 16)QMessageBox::critical(0, tips1, tips2);
                do {
                    str = file.readLine();
                    str = str.remove(QRegExp("[\\f\\r\\n]"));
                    if (str.startsWith('#'))continue;
                    if (str.contains(QRegExp("[0-9]"))) {
                        relics_coef_st.command_xe_coef = str.toDouble();
                        if (relics_coef_st.command_xe_coef <= 0) {
                            QMessageBox::critical(0, tips3, tips2);
                        }
                        break;
                    }
                } while (!file.atEnd());
                ini_s |= 16;
                ini = 0;
                break;
            case 6://[gain_plef_coef]
                if (ini_s & 32)QMessageBox::critical(0, tips1, tips2);
                do {
                    str = file.readLine();
                    str = str.remove(QRegExp("[\\f\\r\\n]"));
                    if (str.startsWith('#'))continue;
                    if (str.contains(QRegExp("[0-9]"))) {
                        relics_coef_st.gain_plef_coef = str.toDouble();
                        if (relics_coef_st.gain_plef_coef <= 0.0 || relics_coef_st.gain_plef_coef > 1.0) {
                            QMessageBox::critical(0, tips3, tips2);
                        }
                        break;
                    }
                } while (!file.atEnd());
                ini_s |= 32;
                ini = 0;
                break;
            case 7://[gain_pdef_coef]
                if (ini_s & 64)QMessageBox::critical(0, tips1, tips2);
                do {
                    str = file.readLine();
                    str = str.remove(QRegExp("[\\f\\r\\n]"));
                    if (str.startsWith('#'))continue;
                    if (str.contains(QRegExp("[0-9]"))) {
                        relics_coef_st.gain_pdef_coef = str.toDouble();
                        if (relics_coef_st.gain_pdef_coef <= 0.0 || relics_coef_st.gain_pdef_coef > 1.0) {
                            QMessageBox::critical(0, tips3, tips2);
                        }
                        break;
                    }
                } while (!file.atEnd());
                ini_s |= 64;
                ini = 0;
                break;
            case 8://[pic_cut_rect]
                if (ini_s & 128)QMessageBox::critical(0, tips1, tips2);
                do {
                    str = file.readLine();
                    str = str.remove(QRegExp("[\\f\\r\\n]"));
                    if (str.startsWith('#'))continue;
                    if (str.contains(QRegExp("[0-9]"))) {
                        relics_coef_st.cut_pictrue = str.toInt();
                        break;
                    }
                } while (!file.atEnd());
                if (file.atEnd()) {
                    QMessageBox::critical(0, tips4, tips2);
                    break;
                }
                do {
                    str = file.readLine();
                    str = str.remove(QRegExp("[\\f\\r\\n]"));
                    if (str.startsWith('#'))continue;
                    if (str.contains(QRegExp("[0-9]"))) {
                        strlist = str.split(" ", QString::SkipEmptyParts);
                        if (strlist.length() != 4) {
                            QMessageBox::critical(0, tips3, tips2);
                        }
                        relics_coef_st.cut_point_x = strlist[0].toDouble();
                        relics_coef_st.cut_point_y = strlist[1].toDouble();
                        relics_coef_st.cut_point_2_x = strlist[2].toDouble();
                        relics_coef_st.cut_point_2_y = strlist[3].toDouble();
                        break;
                    }
                } while (!file.atEnd());
                ini_s |= 128;
                ini = 0;
                break;
            case 9://[preset_character]
                if (ini_s & 256)QMessageBox::critical(0, tips1, tips2);
                do {
                    fpos = file.pos();
                    str = file.readLine();
                    str = str.remove(QRegExp("[\\f\\r\\n]"));
                    if (str.startsWith('#'))continue;
                    if (str.startsWith('[')) {
                        file.seek(fpos);
                        break;
                    }
                    if (!str.contains(QRegExp("[\\S]")))continue;//跳过空白行
                    i = str.indexOf(' ');
                    combox_zh->addItem(str.left(i));
                    combox_en->addItem(str.mid(i + 1, str.length() - i - 1));
                    str = file.readLine();
                    str = str.remove(QRegExp("[\\f\\r\\n]"));
                    strlist = str.split(" ",QString::SkipEmptyParts);
                    memset(&preset, 0, sizeof(preset));
                    preset.first = strlist[0].toInt();
                    i = strlist.length();
                    preset.index = i << 4;//第一种形式
                    if (i > 1) {//第二种形式
                        preset.preset_obr = strlist[1].toDouble();
                        preset.preset_rtr = strlist[2].toDouble();
                    }
                    if (i > 3) {//第三种形式
                        preset.preset_atk = strlist[3].toDouble();
                        preset.preset_ctr = strlist[4].toDouble();
                        preset.preset_ctd = strlist[5].toDouble();
                        preset.preset_ene = strlist[6].toDouble();
                        preset.preset_cx = strlist[7].toDouble();
                        preset.preset_add = strlist[8].toDouble();
                        preset.preset_lef = strlist[9].toDouble();
                        preset.preset_def = strlist[10].toDouble();
                        i = 1;
                        if(preset.preset_add > 34.0) i++;
                        if(preset.preset_ctr + preset.preset_ctd * 0.5 > 99) {
                            i++;
                            if(!(preset.preset_add > 34.0)) i = 4;
                        }
                        preset.index |= i * 3;//i*3是其对应十二种级别的索引
                    }
                    relics_coef_st.character_preset.push_back(preset);
                } while (!file.atEnd());
                ini_s |= 256;
                ini = 0;
                break;
            default:
                str = file.readLine();
                if (str.startsWith('#'))continue;
                if (str.contains("[relics_exp]"))ini = 1;
                if (str.contains("[prior_attrpoint]"))ini = 2;
                if (str.contains("[prior_coef]"))ini = 3;
                if (str.contains("[prior_gain_coef]"))ini = 4;
                if (str.contains("[command_xe_coef]"))ini = 5;
                if (str.contains("[gain_plef_coef]"))ini = 6;
                if (str.contains("[gain_pdef_coef]"))ini = 7;
                if (str.contains("[pic_cut_rect]"))ini = 8;
                if (str.contains("[preset_character]"))ini = 9;
                break;
            }
        }
        file.close();
        if ((ini_s & 0xff) != 0xff)QMessageBox::critical(0, tips5, tips6);
    }
    else {
        QMessageBox::critical(0, tips7, tips8);
    }
    combox_zh->setMinimumWidth(combox_zh->sizeHint().width() + 20);//还是手动计算宽度
    combox_en->setMinimumWidth(combox_en->sizeHint().width() + 20);
    /* 这里计算四个点数与标准点数的比值 */
    relics_exp_rat[0] = (relics_coef_st.relics_exp[2].relics_atk - 38.4) / (attr_ctr*attrpoint[addition_attr::patk]) +
        (relics_coef_st.relics_exp[2].relics_ctr - 5.0) / (attr_ctr*attrpoint[addition_attr::ctr]) +
        (relics_coef_st.relics_exp[2].relics_ctr < 25.0 ? 0 : 2.0*relics_coef_st.relics_exp[2].relics_ctr - 50.0) / (attr_ctr*attrpoint[addition_attr::ctd]) +
        (relics_coef_st.relics_exp[2].relics_ene - 100.0) / (attr_ctr*attrpoint[addition_attr::ene]) +
        relics_coef_st.relics_exp[2].relics_cx / (attr_ctr*attrpoint[addition_attr::cx]) +
        (relics_coef_st.relics_exp[2].exp_lef - 41.4) / (attr_ctr*attrpoint[addition_attr::plef]) +
        relics_coef_st.relics_exp[2].exp_def / (attr_ctr*attrpoint[addition_attr::pdef]);
    relics_exp_rat[0] /= 690.0;
    relics_exp_rat[1] = (relics_coef_st.relics_exp[5].relics_atk - 38.4) / (attr_ctr*attrpoint[addition_attr::patk]) +
        (relics_coef_st.relics_exp[5].relics_ctr - 5.0) / (attr_ctr*attrpoint[addition_attr::ctr]) +
        (relics_coef_st.relics_exp[5].relics_ctr < 25.0 ? 0 : 2.0*relics_coef_st.relics_exp[5].relics_ctr - 50.0) / (attr_ctr*attrpoint[addition_attr::ctd]) +
        (relics_coef_st.relics_exp[5].relics_ene - 100.0) / (attr_ctr*attrpoint[addition_attr::ene]) +
        relics_coef_st.relics_exp[5].relics_cx / (attr_ctr*attrpoint[addition_attr::cx]) +
        (relics_coef_st.relics_exp[5].exp_lef - 41.4) / (attr_ctr*attrpoint[addition_attr::plef]) +
        relics_coef_st.relics_exp[5].exp_def / (attr_ctr*attrpoint[addition_attr::pdef]);
    relics_exp_rat[1] /= 610.0;
    relics_exp_rat[2] = (relics_coef_st.relics_exp[8].relics_atk - 38.4) / (attr_ctr*attrpoint[addition_attr::patk]) +
        (relics_coef_st.relics_exp[8].relics_ctr - 5.0) / (attr_ctr*attrpoint[addition_attr::ctr]) +
        (relics_coef_st.relics_exp[8].relics_ctr < 25.0 ? 0 : 2.0*relics_coef_st.relics_exp[8].relics_ctr - 50.0) / (attr_ctr*attrpoint[addition_attr::ctd]) +
        (relics_coef_st.relics_exp[8].relics_ene - 100.0) / (attr_ctr*attrpoint[addition_attr::ene]) +
        relics_coef_st.relics_exp[8].relics_cx / (attr_ctr*attrpoint[addition_attr::cx]) +
        (relics_coef_st.relics_exp[8].exp_lef - 41.4) / (attr_ctr*attrpoint[addition_attr::plef]) +
        relics_coef_st.relics_exp[8].exp_def / (attr_ctr*attrpoint[addition_attr::pdef]);
    relics_exp_rat[2] /= 610.0;
    relics_exp_rat[3] = (relics_coef_st.relics_exp[11].relics_atk - 38.4) / (attr_ctr*attrpoint[addition_attr::patk]) +
        (relics_coef_st.relics_exp[11].relics_ctr - 5.0) / (attr_ctr*attrpoint[addition_attr::ctr]) +
        (relics_coef_st.relics_exp[11].relics_ctr < 25.0 ? 0 : 2.0*relics_coef_st.relics_exp[11].relics_ctr - 50.0) / (attr_ctr*attrpoint[addition_attr::ctd]) +
        (relics_coef_st.relics_exp[11].relics_ene - 100.0) / (attr_ctr*attrpoint[addition_attr::ene]) +
        relics_coef_st.relics_exp[11].relics_cx / (attr_ctr*attrpoint[addition_attr::cx]) +
        (relics_coef_st.relics_exp[11].exp_lef - 41.4) / (attr_ctr*attrpoint[addition_attr::plef]) +
        relics_coef_st.relics_exp[11].exp_def / (attr_ctr*attrpoint[addition_attr::pdef]);
    relics_exp_rat[3] /= 690.0;
    relics_atk = relics_coef_st.relics_exp[index].relics_atk;
    relics_ctr = relics_coef_st.relics_exp[index].relics_ctr;
    relics_ctd = relics_ctr < 25.0 ? 50.0 : 2.0*relics_ctr;
    relics_ene = relics_coef_st.relics_exp[index].relics_ene;
    relics_cx = relics_coef_st.relics_exp[index].relics_cx;
    relics_add = relics_coef_st.relics_exp[index].relics_add;
    exp_ctr = relics_coef_st.relics_exp[index].exp_ctr;
    exp_atk = relics_coef_st.relics_exp[index].exp_atk;
    exp_obr = relics_coef_st.relics_exp[index].exp_obr;
    exp_rtr = relics_coef_st.relics_exp[index].exp_rtr;
    exp_lef = relics_coef_st.relics_exp[index].exp_lef;
    exp_def = relics_coef_st.relics_exp[index].exp_def;
    ui.lineEdit_12->setText(d2qstring(relics_atk, 2));
    ui.lineEdit_13->setText(d2qstring(relics_ctr, 2) + "/" + d2qstring(relics_ctd, 2));
    ui.lineEdit_14->setText(d2qstring(relics_ene, 2));
    ui.lineEdit_15->setText(d2qstring(relics_cx, 2));
    ui.lineEdit_16->setText(d2qstring(relics_add, 2));
    ui.lineEdit_6->setText(d2qstring(exp_atk, 2));
    ui.lineEdit_7->setText(d2qstring(exp_ctr, 2));
    ui.lineEdit_8->setText(d2qstring(exp_obr, 2));
    ui.lineEdit_9->setText(d2qstring(exp_rtr, 2));
    ui.lineEdit_10->setText(d2qstring(exp_lef, 2));
    ui.lineEdit_11->setText(d2qstring(exp_def, 2));
    relics_or_exp = 0;
    dmg_check_state = Qt::Unchecked;
    addition_first = addition_attr::ctr;
    addition_second = addition_attr::ctd;
    addition_third = addition_attr::patk;
    addition_fourth = addition_attr::ene;
    attr_fixed = 0;
    exp_related = 0;
    pic_ready = false;
    scores_ready = 0;
    total_scores_2 = total_scores = addition_scores_2 = addition_scores = relics_2 = relics = 0.0;
}

void genshin_relics::on_pushButton_15_clicked(void)
{//绝对评分按钮
    int prior = (dmg_check_state != Qt::Unchecked ? 0 : ui.comboBox_2->currentIndex());
    double attr_ten[10] = {
        relics_add,
        (attr_fixed & 32 ? 140 : exp_lef),//为防止评分问题，需固定
        (dmg_check_state != Qt::Unchecked ? 0 : exp_obr),
        (attr_fixed & 64 ? 145 : exp_def),//为防止评分问题，需固定
        (dmg_check_state != Qt::Unchecked ? 0 : exp_rtr),
        exp_atk,
        exp_ctr,
        (exp_ctr < 25.0 ? 50.0 : 2 * exp_ctr),
        (exp_related & 4) ? relics_ene : 152.0,//不关联元素充能则取固定152充能
        (exp_related & 8) ? relics_cx : 187.0//不关联元素精通则取固定187精通
    };
    double attr_relics[10] = {
        relics_add,
        exp_lef,
        (dmg_check_state != Qt::Unchecked ? 0 : exp_obr),
        exp_def,
        (dmg_check_state != Qt::Unchecked ? 0 : exp_rtr),
        relics_atk,
        relics_ctr,
        relics_ctd,
        relics_ene,
        relics_cx
    };
    relics = relics_evaluate(prior, attr_relics);
    addition_attr_analysis(prior, attr_ten, attr_gain);
    addition_scores = addition_attr_evaluate(attr_gain);
    double main_attr = main_attr_evaluate();
    total_scores = addition_scores * (9.0 / 17.0) + main_attr * (8.0 / 17.0);
    ui.textBrowser->setText(tr("[绝对评分]") +
        tr("\n圣遗物评分为：") + d2qstring(relics, 2) +
        tr("\n总属性评分为：") + d2qstring(total_scores, 2) +
        tr("\n副属性评分为：") + d2qstring(addition_scores, 2) +
        tr("\n每十属性点收益增长%：") +
        QString::fromStdWString(L"\nplef  pdef  patk  ctr   ctd\n") +
        QString::asprintf("%-6.3lf", attr_gain[addition_attr::plef] * 100.0) +
        QString::asprintf("%-6.3lf", attr_gain[addition_attr::pdef] * 100.0) +
        QString::asprintf("%-6.3lf", attr_gain[addition_attr::patk] * 100.0) +
        QString::asprintf("%-6.3lf", attr_gain[addition_attr::ctr] * 100.0) +
        QString::asprintf("%-6.3lf", attr_gain[addition_attr::ctd] * 100.0) +
        QString::fromStdWString(L"\nlef   def   atk   ene   cx\n") +
        QString::asprintf("%-6.3lf", attr_gain[addition_attr::lef] * 100.0) +
        QString::asprintf("%-6.3lf", attr_gain[addition_attr::def] * 100.0) +
        QString::asprintf("%-6.3lf", attr_gain[addition_attr::atk] * 100.0) +
        QString::asprintf("%-6.3lf", attr_gain[addition_attr::ene] * 100.0) +
        QString::asprintf("%-6.3lf", attr_gain[addition_attr::cx] * 100.0)
    );
    scores_ready |= 1;
    draw_gain_attr(0);
}


void genshin_relics::on_pushButton_16_clicked(void)
{//相对评分分按钮
    int prior = (dmg_check_state != Qt::Unchecked ? 0 : ui.comboBox_2->currentIndex());
    double attr_ten[10] = {
        relics_add,
        (attr_fixed & 32 ? 140 : exp_lef),//为防止评分问题，需固定
        (dmg_check_state != Qt::Unchecked ? 0 : exp_obr),
        (attr_fixed & 64 ? 145 : exp_def),//为防止评分问题，需固定
        (dmg_check_state != Qt::Unchecked ? 0 : exp_rtr),
        exp_atk,
        exp_ctr,
        (exp_ctr < 25.0 ? 50.0 : 2 * exp_ctr),
        (exp_related & 4) ? relics_ene : 152.0,//不关联元素充能则取固定152充能
        (exp_related & 8) ? relics_cx : 187.0//不关联元素精通则取固定187精通
    };
    double attr_relics[10] = {
        relics_add,
        exp_lef,
        (dmg_check_state != Qt::Unchecked ? 0 : exp_obr),
        exp_def,
        (dmg_check_state != Qt::Unchecked ? 0 : exp_rtr),
        relics_atk,
        relics_ctr,
        relics_ctd,
        relics_ene,
        relics_cx
    };
    relics_2 = relics_accurate(prior, attr_relics);
    relics_analysis(prior, attr_ten, attr_gain_2);
    for (int i = 0; i < 10; i++)attr_gain_2[i] = log(attr_gain_2[i]);//求对数是为了评分标准
    addition_scores_2 = addition_attr_evaluate(attr_gain_2);
    double main_attr = main_attr_evaluate();
    total_scores_2 = addition_scores_2 * (9.0 / 17.0) + main_attr * (8.0 / 17.0);
    ui.textBrowser->setText(tr("[相对评分]") +
        tr("\n圣遗物评分为：") + d2qstring(relics_2, 2) +
        tr("\n总属性评分为：") + d2qstring(total_scores_2, 2) +
        tr("\n副属性评分为：") + d2qstring(addition_scores_2, 2) +
        tr("\n每十属性点收益增长%：") +
        QString::fromStdWString(L"\nplef  pdef  patk  ctr   ctd\n") +
        QString::asprintf("%-6.3lf", attr_gain_2[addition_attr::plef] * 100.0) +
        QString::asprintf("%-6.3lf", attr_gain_2[addition_attr::pdef] * 100.0) +
        QString::asprintf("%-6.3lf", attr_gain_2[addition_attr::patk] * 100.0) +
        QString::asprintf("%-6.3lf", attr_gain_2[addition_attr::ctr] * 100.0) +
        QString::asprintf("%-6.3lf", attr_gain_2[addition_attr::ctd] * 100.0) +
        QString::fromStdWString(L"\nlef   def   atk   ene   cx\n") +
        QString::asprintf("%-6.3lf", attr_gain_2[addition_attr::lef] * 100.0) +
        QString::asprintf("%-6.3lf", attr_gain_2[addition_attr::def] * 100.0) +
        QString::asprintf("%-6.3lf", attr_gain_2[addition_attr::atk] * 100.0) +
        QString::asprintf("%-6.3lf", attr_gain_2[addition_attr::ene] * 100.0) +
        QString::asprintf("%-6.3lf", attr_gain_2[addition_attr::cx] * 100.0)
    );
    scores_ready |= 2;
    draw_gain_attr(1);
}

void genshin_relics::on_pushButton_clicked(void)
{//副属性按钮1
    if (relics_addindex != 1)ui.pushButton->setText(QString::fromStdWString(L"[") + ui.pushButton->text() + QString::fromStdWString(L"]"));
    switch (relics_addindex)
    {
    case 2:
        ui.pushButton_2->setText(ui.pushButton_2->text().mid(1, ui.pushButton_2->text().length() - 2));
        break;
    case 3:
        ui.pushButton_3->setText(ui.pushButton_3->text().mid(1, ui.pushButton_3->text().length() - 2));
        break;
    case 4:
        ui.pushButton_4->setText(ui.pushButton_4->text().mid(1, ui.pushButton_4->text().length() - 2));
        break;
    }
    relics_addindex = 1;
}

void genshin_relics::on_pushButton_2_clicked(void)
{//副属性按钮2
    if (relics_addindex != 2)ui.pushButton_2->setText(QString::fromStdWString(L"[") + ui.pushButton_2->text() + QString::fromStdWString(L"]"));
    switch (relics_addindex)
    {
    case 1:
        ui.pushButton->setText(ui.pushButton->text().mid(1, ui.pushButton->text().length() - 2));
        break;
    case 3:
        ui.pushButton_3->setText(ui.pushButton_3->text().mid(1, ui.pushButton_3->text().length() - 2));
        break;
    case 4:
        ui.pushButton_4->setText(ui.pushButton_4->text().mid(1, ui.pushButton_4->text().length() - 2));
        break;
    }
    relics_addindex = 2;
}

void genshin_relics::on_pushButton_3_clicked(void)
{//副属性按钮3
    if (relics_addindex != 3)ui.pushButton_3->setText(QString::fromStdWString(L"[") + ui.pushButton_3->text() + QString::fromStdWString(L"]"));
    switch (relics_addindex)
    {
    case 1:
        ui.pushButton->setText(ui.pushButton->text().mid(1, ui.pushButton->text().length() - 2));
        break;
    case 2:
        ui.pushButton_2->setText(ui.pushButton_2->text().mid(1, ui.pushButton_2->text().length() - 2));
        break;
    case 4:
        ui.pushButton_4->setText(ui.pushButton_4->text().mid(1, ui.pushButton_4->text().length() - 2));
        break;
    }
    relics_addindex = 3;
}

void genshin_relics::on_pushButton_4_clicked(void)
{//副属性按钮4
    if (relics_addindex != 4)ui.pushButton_4->setText(QString::fromStdWString(L"[") + ui.pushButton_4->text() + QString::fromStdWString(L"]"));
    switch (relics_addindex)
    {
    case 1:
        ui.pushButton->setText(ui.pushButton->text().mid(1, ui.pushButton->text().length() - 2));
        break;
    case 2:
        ui.pushButton_2->setText(ui.pushButton_2->text().mid(1, ui.pushButton_2->text().length() - 2));
        break;
    case 3:
        ui.pushButton_3->setText(ui.pushButton_3->text().mid(1, ui.pushButton_3->text().length() - 2));
        break;
    }
    relics_addindex = 4;
}

void genshin_relics::on_pushButton_5_clicked(void)
{//副属性选择按钮生命值
    QString tips = tr("生命值");
    switch (relics_addindex)
    {
    case 1:
        addition_first = addition_attr::lef;
        on_pushButton_2_clicked();
        ui.pushButton->setText(tips);
        break;
    case 2:
        addition_second = addition_attr::lef;
        on_pushButton_3_clicked();
        ui.pushButton_2->setText(tips);
        break;
    case 3:
        addition_third = addition_attr::lef;
        on_pushButton_4_clicked();
        ui.pushButton_3->setText(tips);
        break;
    case 4:
        addition_fourth = addition_attr::lef;
        ui.pushButton_4->setText(tips);
        relics_addindex = 0;
        break;
    default:
        break;
    }
}

void genshin_relics::on_pushButton_6_clicked(void)
{//副属性选择按钮生命值%
    QString tips = tr("生命值%");
    switch (relics_addindex)
    {
    case 1:
        addition_first = addition_attr::plef;
        on_pushButton_2_clicked();
        ui.pushButton->setText(tips);
        break;
    case 2:
        addition_second = addition_attr::plef;
        on_pushButton_3_clicked();
        ui.pushButton_2->setText(tips);
        break;
    case 3:
        addition_third = addition_attr::plef;
        on_pushButton_4_clicked();
        ui.pushButton_3->setText(tips);
        break;
    case 4:
        addition_fourth = addition_attr::plef;
        ui.pushButton_4->setText(tips);
        relics_addindex = 0;
        break;
    default:
        break;
    }
}

void genshin_relics::on_pushButton_7_clicked(void)
{//副属性选择按钮防御力
    QString tips = tr("防御力");
    switch (relics_addindex)
    {
    case 1:
        addition_first = addition_attr::def;
        on_pushButton_2_clicked();
        ui.pushButton->setText(tips);
        break;
    case 2:
        addition_second = addition_attr::def;
        on_pushButton_3_clicked();
        ui.pushButton_2->setText(tips);
        break;
    case 3:
        addition_third = addition_attr::def;
        on_pushButton_4_clicked();
        ui.pushButton_3->setText(tips);
        break;
    case 4:
        addition_fourth = addition_attr::def;
        ui.pushButton_4->setText(tips);
        relics_addindex = 0;
        break;
    default:
        break;
    }
}

void genshin_relics::on_pushButton_8_clicked(void)
{//副属性选择按钮防御力%
    QString tips = tr("防御力%");
    switch (relics_addindex)
    {
    case 1:
        addition_first = addition_attr::pdef;
        on_pushButton_2_clicked();
        ui.pushButton->setText(tips);
        break;
    case 2:
        addition_second = addition_attr::pdef;
        on_pushButton_3_clicked();
        ui.pushButton_2->setText(tips);
        break;
    case 3:
        addition_third = addition_attr::pdef;
        on_pushButton_4_clicked();
        ui.pushButton_3->setText(tips);
        break;
    case 4:
        addition_fourth = addition_attr::pdef;
        ui.pushButton_4->setText(tips);
        relics_addindex = 0;
        break;
    default:
        break;
    }
}

void genshin_relics::on_pushButton_9_clicked(void)
{//副属性选择按钮攻击力
    QString tips = tr("攻击力");
    switch (relics_addindex)
    {
    case 1:
        addition_first = addition_attr::atk;
        on_pushButton_2_clicked();
        ui.pushButton->setText(tips);
        break;
    case 2:
        addition_second = addition_attr::atk;
        on_pushButton_3_clicked();
        ui.pushButton_2->setText(tips);
        break;
    case 3:
        addition_third = addition_attr::atk;
        on_pushButton_4_clicked();
        ui.pushButton_3->setText(tips);
        break;
    case 4:
        addition_fourth = addition_attr::atk;
        ui.pushButton_4->setText(tips);
        relics_addindex = 0;
        break;
    default:
        break;
    }
}

void genshin_relics::on_pushButton_10_clicked(void)
{//副属性选择按钮攻击力%
    QString tips = tr("攻击力%");
    switch (relics_addindex)
    {
    case 1:
        addition_first = addition_attr::patk;
        on_pushButton_2_clicked();
        ui.pushButton->setText(tips);
        break;
    case 2:
        addition_second = addition_attr::patk;
        on_pushButton_3_clicked();
        ui.pushButton_2->setText(tips);
        break;
    case 3:
        addition_third = addition_attr::patk;
        on_pushButton_4_clicked();
        ui.pushButton_3->setText(tips);
        break;
    case 4:
        addition_fourth = addition_attr::patk;
        ui.pushButton_4->setText(tips);
        relics_addindex = 0;
        break;
    default:
        break;
    }
}

void genshin_relics::on_pushButton_11_clicked(void)
{//副属性选择按钮暴击率
    QString tips = tr("暴击率");
    switch (relics_addindex)
    {
    case 1:
        addition_first = addition_attr::ctr;
        on_pushButton_2_clicked();
        ui.pushButton->setText(tips);
        break;
    case 2:
        addition_second = addition_attr::ctr;
        on_pushButton_3_clicked();
        ui.pushButton_2->setText(tips);
        break;
    case 3:
        addition_third = addition_attr::ctr;
        on_pushButton_4_clicked();
        ui.pushButton_3->setText(tips);
        break;
    case 4:
        addition_fourth = addition_attr::ctr;
        ui.pushButton_4->setText(tips);
        relics_addindex = 0;
        break;
    default:
        break;
    }
}

void genshin_relics::on_pushButton_12_clicked(void)
{//副属性选择按钮暴击伤害
    QString tips = tr("暴击伤害");
    switch (relics_addindex)
    {
    case 1:
        addition_first = addition_attr::ctd;
        on_pushButton_2_clicked();
        ui.pushButton->setText(tips);
        break;
    case 2:
        addition_second = addition_attr::ctd;
        on_pushButton_3_clicked();
        ui.pushButton_2->setText(tips);
        break;
    case 3:
        addition_third = addition_attr::ctd;
        on_pushButton_4_clicked();
        ui.pushButton_3->setText(tips);
        break;
    case 4:
        addition_fourth = addition_attr::ctd;
        ui.pushButton_4->setText(tips);
        relics_addindex = 0;
        break;
    default:
        break;
    }
}

void genshin_relics::on_pushButton_13_clicked(void)
{//副属性选择按钮元素充能
    QString tips = tr("元素充能");
    switch (relics_addindex)
    {
    case 1:
        addition_first = addition_attr::ene;
        on_pushButton_2_clicked();
        ui.pushButton->setText(tips);
        break;
    case 2:
        addition_second = addition_attr::ene;
        on_pushButton_3_clicked();
        ui.pushButton_2->setText(tips);
        break;
    case 3:
        addition_third = addition_attr::ene;
        on_pushButton_4_clicked();
        ui.pushButton_3->setText(tips);
        break;
    case 4:
        addition_fourth = addition_attr::ene;
        ui.pushButton_4->setText(tips);
        relics_addindex = 0;
        break;
    default:
        break;
    }
}

void genshin_relics::on_pushButton_14_clicked(void)
{//副属性选择按钮元素精通
    QString tips = tr("元素精通");
    switch (relics_addindex)
    {
    case 1:
        addition_first = addition_attr::cx;
        on_pushButton_2_clicked();
        ui.pushButton->setText(tips);
        break;
    case 2:
        addition_second = addition_attr::cx;
        on_pushButton_3_clicked();
        ui.pushButton_2->setText(tips);
        break;
    case 3:
        addition_third = addition_attr::cx;
        on_pushButton_4_clicked();
        ui.pushButton_3->setText(tips);
        break;
    case 4:
        addition_fourth = addition_attr::cx;
        ui.pushButton_4->setText(tips);
        relics_addindex = 0;
        break;
    default:
        break;
    }
}




void genshin_relics::on_lineEdit_6_editingFinished()
{//期望攻击力%输入框
    bool ok;
    double a = ui.lineEdit_6->text().toDouble(&ok);
    if (ok) {
        if (fabs(a - exp_atk) > 0.005) {
            if (relics_or_exp == 0)ui.comboBox_5->setCurrentIndex(0);
            ui.lineEdit_6->setText(d2qstring(exp_atk = a, 2));
            if (exp_related & 1)on_pushButton_18_clicked();
        }
    }
    else {
        ui.lineEdit_6->setText(d2qstring(exp_atk, 2));
    }
}

void genshin_relics::on_lineEdit_7_editingFinished()
{//期望双暴暴击输入框
    bool ok;
    double a = ui.lineEdit_7->text().toDouble(&ok);
    if (ok) {
        if (fabs(a - exp_ctr) > 0.005) {
            if (relics_or_exp == 0)ui.comboBox_5->setCurrentIndex(0);
            ui.lineEdit_7->setText(d2qstring(exp_ctr=a, 2));
            if (exp_related & 2)on_pushButton_19_clicked();
        }
    }
    else {
        ui.lineEdit_7->setText(d2qstring(exp_ctr, 2));
    }
}

void genshin_relics::on_lineEdit_8_editingFinished(void)
{//期望元素爆发率输入框
    bool ok;
    double a = ui.lineEdit_8->text().toDouble(&ok);
    if (ok) {
        if (fabs(a - exp_obr) > 0.005) {
            if (relics_or_exp == 0)ui.comboBox_5->setCurrentIndex(0);
            ui.lineEdit_8->setText(d2qstring(exp_obr = a, 2));
        }
    }
    else {
        ui.lineEdit_8->setText(d2qstring(exp_obr, 2));
    }
}

void genshin_relics::on_lineEdit_9_editingFinished(void)
{//期望元素反应率输入框
    bool ok;
    double a = ui.lineEdit_9->text().toDouble(&ok);
    if (ok) {
        if (fabs(a - exp_rtr) > 0.005) {
            if (relics_or_exp == 0)ui.comboBox_5->setCurrentIndex(0);
            ui.lineEdit_9->setText(d2qstring(exp_rtr = a, 2));
        }
    }
    else {
        ui.lineEdit_9->setText(d2qstring(exp_rtr, 2));
    }
}

void genshin_relics::on_lineEdit_10_editingFinished(void)
{//期望生命%输入框
    bool oka = false, okb = true;
    double a, b;
    QString txt = ui.lineEdit_10->text();
    if (!txt.isEmpty()) {
        int j = txt.lastIndexOf(']');
        if (txt.at(0) == L'[' && j > 0) {
            int i = txt.indexOf('-');
            if (i >= 0) {//计算点数
                a = txt.mid(1, i - 1).toDouble(&oka);
                b = txt.mid(i + 1, j - i - 1).toDouble(&okb);
                if (b != 0)a = (a - mainattr[0] * attr_ctr*attrpoint[addition_attr::lef])*100.0 / b + 41.4;
                else okb = false;
            }
            else {
                i = txt.indexOf('+');
                if (i >= 0) {//计算输出
                    a = txt.mid(1, i - 1).toDouble(&oka);
                    b = txt.mid(i + 1, j - i - 1).toDouble(&okb);
                    a = 100.0*((a + b) / LEF - 1.0);
                }
                else {
                    i = txt.indexOf('/');
                    if (i >= 0) {//计算比值
                        a = txt.mid(1, i - 1).toDouble(&oka);
                        b = txt.mid(i + 1, j - i - 1).toDouble(&okb);
                        if (b != 0) a = a * 100.0 / b;
                        else okb = false;
                    }
                }
            }
        }
        else {
            a = txt.toDouble(&oka);
        }
    }
    if (oka && okb) {
        if (fabs(a - exp_lef) > 0.005) {
            ui.comboBox_5->setCurrentIndex(0);
            ui.lineEdit_10->setText(d2qstring(exp_lef = a, 2));
        }
    }
    else {
        ui.lineEdit_10->setText(d2qstring(exp_lef, 2));
    }
}

void genshin_relics::on_lineEdit_11_editingFinished(void)
{//期望防御%输入框
    bool oka = false, okb = true;
    double a, b;
    QString txt = ui.lineEdit_11->text();
    if (!txt.isEmpty()) {
        int j = txt.lastIndexOf(']');
        if (txt.at(0) == L'[' && j > 0) {
            int i = txt.indexOf('-');
            if (i >= 0) {//计算点数
                a = txt.mid(1, i - 1).toDouble(&oka);
                b = txt.mid(i + 1, j - i - 1).toDouble(&okb);
                if (b != 0)a = a * 100.0 / b;
                else okb = false;
            }
            else {
                i = txt.indexOf('+');
                if (i >= 0) {//计算输出
                    a = txt.mid(1, i - 1).toDouble(&oka);
                    b = txt.mid(i + 1, j - i - 1).toDouble(&okb);
                    a = 100.0*((a + b) / DEF - 1.0);
                }
                else {
                    i = txt.indexOf('/');
                    if (i >= 0) {//计算比值
                        a = txt.mid(1, i - 1).toDouble(&oka);
                        b = txt.mid(i + 1, j - i - 1).toDouble(&okb);
                        if (b != 0) a = a * 100.0 / b;
                        else okb = false;
                    }
                }
            }
        }
        else {
            a = txt.toDouble(&oka);
        }
    }
    if (oka && okb) {
        if (fabs(a - exp_def) > 0.005) {
            ui.comboBox_5->setCurrentIndex(0);
            ui.lineEdit_11->setText(d2qstring(exp_def = a, 2));
        }
    }
    else {
        ui.lineEdit_11->setText(d2qstring(exp_def, 2));
    }
}

void genshin_relics::on_lineEdit_12_editingFinished()
{//圣遗物总攻击力%输入框
    bool oka = false, okb = true;
    double a, b;
    QString txt = ui.lineEdit_12->text();
    if (!txt.isEmpty()) {
        int j = txt.lastIndexOf(']');
        if (txt.at(0) == L'[' && j > 0) {
            int i = txt.indexOf('-');
            if (i >= 0) {//计算点数
                a = txt.mid(1, i - 1).toDouble(&oka);
                b = txt.mid(i + 1, j - i - 1).toDouble(&okb);
                if (b != 0)a = (a - mainattr[1] * attr_ctr*attrpoint[addition_attr::atk])*100.0 / b + 38.4;
                else okb = false;
            }
            else {
                i = txt.indexOf('+');
                if (i >= 0) {//计算输出
                    a = txt.mid(1, i - 1).toDouble(&oka);
                    b = txt.mid(i + 1, j - i - 1).toDouble(&okb);
                    a = 100.0*((a + b) / ATK - 1.0);
                }
                else {
                    i = txt.indexOf('/');
                    if (i >= 0) {//计算比值
                        a = txt.mid(1, i - 1).toDouble(&oka);
                        b = txt.mid(i + 1, j - i - 1).toDouble(&okb);
                        if (b != 0) a = a * 100.0 / b;
                        else okb = false;
                    }
                }
            }
        }
        else {
            a = txt.toDouble(&oka);
        }
    }
    if (oka && okb) {
        if (fabs(a - relics_atk) > 0.005) {
            if (relics_or_exp == 1)ui.comboBox_5->setCurrentIndex(0);
            ui.lineEdit_12->setText(d2qstring(relics_atk = a, 2));
            if (exp_related & 1) {
                on_pushButton_18_clicked();
                on_pushButton_18_clicked();
            }
        }
    }
    else {
        ui.lineEdit_12->setText(d2qstring(relics_atk, 2));
    }
}

void genshin_relics::on_lineEdit_13_editingFinished()
{//圣遗物总双暴输入框
    bool oka = false, okb = false;
    double a, b;
    QString str = ui.lineEdit_13->text();
    if (!str.isEmpty()) {
        int j = str.lastIndexOf(']');
        if (str.at(0) == L'[' && j > 0) {
            int i = str.indexOf(' ');
            if (i < 0)i = str.indexOf('/');
            if (i >= 0) {//计算点数
                a = str.mid(1, i - 1).toDouble(&oka) + 5.0;
                b = str.mid(i + 1, j - i - 1).toDouble(&okb) + 50.0;
            }
        }
        else {
            a = str.left(str.indexOf('/')).toDouble(&oka);
            b = str.mid(str.indexOf('/') + 1).toDouble(&okb);
        }
    }
    if (oka && okb) {
        if (fabs(a - relics_ctr) > 0.005 || fabs(b - relics_ctd) > 0.005) {
            if (relics_or_exp == 1)ui.comboBox_5->setCurrentIndex(0);
            ui.lineEdit_13->setText(d2qstring(relics_ctr = a, 2) + "/" + d2qstring(relics_ctd = b, 2));
            if (exp_related & 2) {
                on_pushButton_19_clicked();
                on_pushButton_19_clicked();
            }
        }
    }
    else {
        ui.lineEdit_13->setText(d2qstring(relics_ctr, 2) + "/" + d2qstring(relics_ctd, 2));
    }
}

void genshin_relics::on_lineEdit_14_editingFinished(void)
{//圣遗物总元素充能输入框
    bool ok;
    double a = ui.lineEdit_14->text().toDouble(&ok);
    if (ok) {
        if (fabs(a - relics_ene) > 0.005) {
            if (relics_or_exp == 1 || (exp_related & 4))ui.comboBox_5->setCurrentIndex(0);
            ui.lineEdit_14->setText(d2qstring(relics_ene = a, 2));
        }
    }
    else {
        ui.lineEdit_14->setText(d2qstring(relics_ene, 2));
    }
}

void genshin_relics::on_lineEdit_15_editingFinished(void)
{//圣遗物总元素精通输入框
    bool ok;
    double a = ui.lineEdit_15->text().toDouble(&ok);
    if (ok) {
        if (fabs(a - relics_cx) > 0.005) {
            if (relics_or_exp == 1 || (exp_related & 8))ui.comboBox_5->setCurrentIndex(0);
            ui.lineEdit_15->setText(d2qstring(relics_cx = a, 2));
        }
    }
    else {
        ui.lineEdit_15->setText(d2qstring(relics_cx, 2));
    }
}

void genshin_relics::on_lineEdit_16_editingFinished(void)
{//圣遗物总附加伤害输入框
    bool ok;
    double a = ui.lineEdit_16->text().toDouble(&ok);
    if (ok) {
        if (fabs(a - relics_add) > 0.005) {
            if (relics_or_exp == 1)ui.comboBox_5->setCurrentIndex(0);
            ui.lineEdit_16->setText(d2qstring(relics_add = a, 2));
        }
    }
    else {
        ui.lineEdit_16->setText(d2qstring(relics_add, 2));
    }
}

void genshin_relics::on_comboBox_5_currentIndexChanged(int index)
{//期望级别组合框
    if (index == 0)return;
    index--;
    int preset = combox_zh->currentIndex();
    struct relics_preset character;
    if (preset == 0) {//默认,应恢复设置
        memset(&character, 0, sizeof(character));
        character.index = 1 << 4;
    }
    else character = relics_coef_st.character_preset.data()[preset - 1];
    double i_exp_atk, i_exp_ctr, i_exp_obr, i_exp_rtr, i_exp_lef, i_exp_def;
    double i_relics_atk, i_relics_ctr, i_relics_ctd, i_relics_ene, i_relics_cx, i_relics_add;
    int i_index = character.index;
    if ((i_index >> 4) > 1) {//第二种形式、第三种形式
        i_exp_obr = fabs(character.preset_obr);
        i_exp_rtr = fabs(character.preset_rtr);
    }
    else {//第一种形式
        i_exp_obr = relics_coef_st.relics_exp[index].exp_obr;
        i_exp_rtr = relics_coef_st.relics_exp[index].exp_rtr;
    }
    if ((i_index >> 4) > 3) {//第三种形式
        double EGG_MIN = 1.0e-10;//为了防止零除
        i_index = (i_index & 0xf) - 1;
        double dmg_points, dlef_points, org_points, dest_points, out_points, health_points;
        double atk_rat, ctr_rat, ctd_rat, ene_rat, cx_rat, lef_rat, def_rat;
        /* 计算角色预设中的输出点数 */
        dmg_points = (character.preset_atk >= 0 ? (character.preset_atk - 38.4) / (attr_ctr*attrpoint[addition_attr::patk]) : 0) +
            (character.preset_ctr >= 0 ? (character.preset_ctr - 5.0) / (attr_ctr*attrpoint[addition_attr::ctr]) +
            (character.preset_ctd - 50.0) / (attr_ctr*attrpoint[addition_attr::ctd]) : 0) +
            (character.preset_ene >= 0 ? (character.preset_ene - 100.0) / (attr_ctr*attrpoint[addition_attr::ene]) : 0) +
            (character.preset_cx >= 0 ? character.preset_cx / (attr_ctr*attrpoint[addition_attr::cx]) : 0) + EGG_MIN;
        /* 计算角色预设中的生命防御点数 */
        dlef_points = (character.preset_lef >= 0 ? (character.preset_lef - 41.4) / (attr_ctr*attrpoint[addition_attr::plef]) : 0) +
            (character.preset_def >= 0 ? character.preset_def / (attr_ctr*attrpoint[addition_attr::pdef]) : 0) + EGG_MIN;
        /* 计算角色对应的索引预设中的输出点数 */
        org_points = (character.preset_atk >= 0 ? (relics_coef_st.relics_exp[i_index].relics_atk - 38.4)
                / (attr_ctr*attrpoint[addition_attr::patk]) : 0) +
            (character.preset_ctr >= 0 ? (relics_coef_st.relics_exp[i_index].relics_ctr - 5.0)
                / (attr_ctr*attrpoint[addition_attr::ctr]) +
            (relics_coef_st.relics_exp[i_index].relics_ctr < 25.0 ? 0 : 2.0*relics_coef_st.relics_exp[i_index].relics_ctr - 50.0)
                / (attr_ctr*attrpoint[addition_attr::ctd]) : 0) +
            (character.preset_ene >= 0 ? (relics_coef_st.relics_exp[i_index].relics_ene - 100.0)
                / (attr_ctr*attrpoint[addition_attr::ene]) : 0) +
            (character.preset_cx >= 0 ? relics_coef_st.relics_exp[i_index].relics_cx
                / (attr_ctr*attrpoint[addition_attr::cx]) : 0) + EGG_MIN;
        /* 计算目前对应的索引预设中的输出点数 */
        dest_points = (character.preset_atk >= 0 ? (relics_coef_st.relics_exp[index].relics_atk - 38.4)
                / (attr_ctr*attrpoint[addition_attr::patk]) : 0) +
            (character.preset_ctr >= 0 ? (relics_coef_st.relics_exp[index].relics_ctr - 5.0)
                / (attr_ctr*attrpoint[addition_attr::ctr]) +
            (relics_coef_st.relics_exp[index].relics_ctr < 25.0 ? 0 : 2.0*relics_coef_st.relics_exp[index].relics_ctr - 50.0)
                / (attr_ctr*attrpoint[addition_attr::ctd]) : 0) +
            (character.preset_ene >= 0 ? (relics_coef_st.relics_exp[index].relics_ene - 100.0)
                / (attr_ctr*attrpoint[addition_attr::ene]) : 0) +
            (character.preset_cx >= 0 ? relics_coef_st.relics_exp[index].relics_cx
                / (attr_ctr*attrpoint[addition_attr::cx]) : 0) + EGG_MIN;
        /* 以下计算应设置的输出点数和生命防御点数 */
        out_points = (dmg_points + dlef_points) / org_points * dest_points * relics_exp_rat[i_index / 3] / relics_exp_rat[index / 3];//标准化总点数
        health_points = dlef_points / (dmg_points + dlef_points);
        constexpr double RAT_HEALTH = 5.0;//攻击路线和双暴路线的初始生命防御点数比重的比值
        constexpr double a_health = 1 / (RAT_HEALTH - 1);
        constexpr double b_health = RAT_HEALTH / (RAT_HEALTH - 1);
        constexpr double c_health = RAT_HEALTH / ((RAT_HEALTH - 1) * (RAT_HEALTH - 1));
        if (i_index >= 6) {
            if (index < 6) health_points = b_health - c_health / (health_points + a_health);//从双暴路线到攻击路线,生命防御点数比重增加
        }
        else {
            if (index >= 6) health_points = c_health / (b_health - health_points) - a_health;//从攻击路线到双暴路线,生命防御点数比重减少
        }
        i_relics_add = fabs(character.preset_add);
        if ((i_index >= 3 && i_index < 9) && !(index >= 3 && index < 9)) {
            health_points *= 1.024;
            if (character.preset_add >= 0) i_relics_add = character.preset_add - attr_ctr*attrpoint[10] * mainattr[3];//附加伤害设置
        }
        if (!(i_index >= 3 && i_index < 9) && (index >= 3 && index < 9)) {
            health_points *= 0.9765625;
            if (character.preset_add >= 0) i_relics_add = character.preset_add + attr_ctr*attrpoint[10] * mainattr[3];
        }
        if (i_relics_add < 0) i_relics_add = 0;
        health_points *= out_points;//计算出了应设置的生命防御点数
        out_points -= health_points;//计算出了应设置的输出点数
        if (character.preset_atk >= 0) {//不固定此属性,应计算
            atk_rat = (character.preset_atk - 38.4) / (attr_ctr*attrpoint[addition_attr::patk]) / dmg_points +
                (relics_coef_st.relics_exp[index].relics_atk - 38.4) / (attr_ctr*attrpoint[addition_attr::patk]) / dest_points -
                (relics_coef_st.relics_exp[i_index].relics_atk - 38.4) / (attr_ctr*attrpoint[addition_attr::patk]) / org_points;
            if (atk_rat < 0) i_exp_atk = i_relics_atk = 38.4;
            else i_exp_atk = i_relics_atk = atk_rat * out_points * attr_ctr*attrpoint[addition_attr::patk] + 38.4;
        }
        else {
            i_exp_atk = 132.0;//为防止评分问题，需固定
            i_relics_atk = -character.preset_atk;//固定此属性,直接赋值
        }
        if (character.preset_ctr >= 0) {//亦是，下亦同
            ctr_rat = (character.preset_ctr - 5.0) / (attr_ctr*attrpoint[addition_attr::ctr]) / dmg_points +
                (relics_coef_st.relics_exp[index].relics_ctr - 5.0) / (attr_ctr*attrpoint[addition_attr::ctr]) / dest_points -
                (relics_coef_st.relics_exp[i_index].relics_ctr - 5.0) / (attr_ctr*attrpoint[addition_attr::ctr]) / org_points;
            ctd_rat = (character.preset_ctd - 50.0) / (attr_ctr*attrpoint[addition_attr::ctd]) / dmg_points +
                (relics_coef_st.relics_exp[index].relics_ctr < 25.0 ? 0 : 2.0*relics_coef_st.relics_exp[index].relics_ctr - 50.0)
                    / (attr_ctr*attrpoint[addition_attr::ctd]) / dest_points -
                (relics_coef_st.relics_exp[i_index].relics_ctr < 25.0 ? 0 : 2.0*relics_coef_st.relics_exp[i_index].relics_ctr - 50.0)
                    / (attr_ctr*attrpoint[addition_attr::ctd]) / org_points;
            if (ctr_rat < 0) i_relics_ctr = 5.0;
            else {
                i_relics_ctr = ctr_rat * out_points * attr_ctr*attrpoint[addition_attr::ctr] + 5.0;
                if (i_relics_ctr > 100.0) i_relics_ctr = 100.0;
            }
            if (ctd_rat < 0) i_relics_ctd = 50.0;
            else i_relics_ctd = ctd_rat * out_points * attr_ctr*attrpoint[addition_attr::ctd] + 50.0;
            i_exp_ctr = 0.5 * i_relics_ctr + 0.25 * i_relics_ctd;
        }
        else {
            i_relics_ctr = -character.preset_ctr;
            i_relics_ctd = character.preset_ctd;
            i_exp_ctr = 74.8;//为防止评分问题，需固定
        }
        if (character.preset_ene >= 0) {
            ene_rat = (character.preset_ene - 100.0) / (attr_ctr*attrpoint[addition_attr::ene]) / dmg_points +
                (relics_coef_st.relics_exp[index].relics_ene - 100.0) / (attr_ctr*attrpoint[addition_attr::ene]) / dest_points -
                (relics_coef_st.relics_exp[i_index].relics_ene - 100.0) / (attr_ctr*attrpoint[addition_attr::ene]) / org_points;
            if (ene_rat < 0) i_relics_ene = 100.0;
            else i_relics_ene = ene_rat * out_points * attr_ctr*attrpoint[addition_attr::ene] + 100.0;
        }
        else i_relics_ene = -character.preset_ene;
        if (character.preset_cx >= 0) {
            cx_rat = character.preset_cx / (attr_ctr*attrpoint[addition_attr::cx]) / dmg_points +
                relics_coef_st.relics_exp[index].relics_cx / (attr_ctr*attrpoint[addition_attr::cx]) / dest_points -
                relics_coef_st.relics_exp[i_index].relics_cx / (attr_ctr*attrpoint[addition_attr::cx]) / org_points;
            if (cx_rat < 0) i_relics_cx = 0;
            else i_relics_cx = cx_rat * out_points * attr_ctr*attrpoint[addition_attr::cx];
        }
        else i_relics_cx = -character.preset_cx;
        if (character.preset_lef >= 0) {
            lef_rat = (character.preset_lef - 41.4) / (attr_ctr*attrpoint[addition_attr::plef]) / dlef_points;
            i_exp_lef = lef_rat * health_points * attr_ctr*attrpoint[addition_attr::plef] + 41.4;
        }
        else i_exp_lef = -character.preset_lef;
        if (character.preset_def >= 0) {
            def_rat = character.preset_def / (attr_ctr*attrpoint[addition_attr::pdef]) / dlef_points;
            i_exp_def = def_rat * health_points * attr_ctr*attrpoint[addition_attr::pdef];
        }
        else i_exp_def = -character.preset_def;
    }
    else {
        i_exp_atk = relics_coef_st.relics_exp[index].exp_atk;
        i_exp_ctr = relics_coef_st.relics_exp[index].exp_ctr;
        i_exp_lef = relics_coef_st.relics_exp[index].exp_lef;
        i_exp_def = relics_coef_st.relics_exp[index].exp_def;
        i_relics_atk = relics_coef_st.relics_exp[index].relics_atk;
        i_relics_ctr = relics_coef_st.relics_exp[index].relics_ctr;
        i_relics_ctd = i_relics_ctr < 25.0 ? 50.0 : 2.0*i_relics_ctr;
        i_relics_ene = relics_coef_st.relics_exp[index].relics_ene;
        i_relics_cx = relics_coef_st.relics_exp[index].relics_cx;
        i_relics_add = relics_coef_st.relics_exp[index].relics_add;
    }
    exp_lef = i_exp_lef;
    exp_def = i_exp_def;
    ui.lineEdit_10->setText(d2qstring(exp_lef, 2));
    ui.lineEdit_11->setText(d2qstring(exp_def, 2));
    if (relics_or_exp == 0) {//期望总属性
        exp_atk = i_exp_atk;
        exp_ctr = i_exp_ctr;
        exp_obr = i_exp_obr;
        exp_rtr = i_exp_rtr;
        ui.lineEdit_6->setText(d2qstring(exp_atk, 2));
        ui.lineEdit_7->setText(d2qstring(exp_ctr, 2));
        on_lineEdit_6_editingFinished();
        on_lineEdit_7_editingFinished();
        ui.lineEdit_8->setText(d2qstring(exp_obr, 2));
        ui.lineEdit_9->setText(d2qstring(exp_rtr, 2));
        if (exp_related & 1)on_pushButton_18_clicked();
        if (exp_related & 2)on_pushButton_19_clicked();
    }
    else {//圣遗物总属性
        relics_atk = i_relics_atk;
        relics_ctr = i_relics_ctr;
        relics_ctd = i_relics_ctd;
        relics_ene = i_relics_ene;
        relics_cx = i_relics_cx;
        relics_add = i_relics_add;
        ui.lineEdit_12->setText(d2qstring(relics_atk, 2));
        ui.lineEdit_13->setText(d2qstring(relics_ctr, 2)+"/"+ d2qstring(relics_ctd, 2));
        on_lineEdit_12_editingFinished();
        on_lineEdit_13_editingFinished();
        ui.lineEdit_14->setText(d2qstring(relics_ene, 2));
        ui.lineEdit_15->setText(d2qstring(relics_cx, 2));
        ui.lineEdit_16->setText(d2qstring(relics_add, 2));
        if (exp_related & 1) {
            on_pushButton_18_clicked();
            on_pushButton_18_clicked();
        }
        if (exp_related & 2) {
            on_pushButton_19_clicked();
            on_pushButton_19_clicked();
        }
    }
}

void genshin_relics::on_pushButton_17_clicked(void)
{//更改期望或更改圣遗物总属性
    ui.comboBox_5->setCurrentIndex(0);
    if (relics_or_exp == 0) {
        ui.pushButton_17->setText(tr("更改圣遗物"));
        relics_or_exp = 1;
    }
    else {
        ui.pushButton_17->setText(tr("更改期望"));
        relics_or_exp = 0;
    }
}

void genshin_relics::on_pushButton_18_clicked()
{//期望攻击力%按钮
    exp_related ^= 1;
    if (exp_related & 1) {
        ui.pushButton_18->setText(QString::fromStdWString(L"[") + ui.pushButton_18->text() + QString::fromStdWString(L"]"));
        ui.lineEdit_6->setText(d2qstring(exp_atk = relics_atk, 2));
        on_lineEdit_6_editingFinished();
        if (relics_or_exp == 0)ui.comboBox_5->setCurrentIndex(0);
    }
    else ui.pushButton_18->setText(ui.pushButton_18->text().mid(1, ui.pushButton_18->text().length() - 2));
}

void genshin_relics::on_pushButton_19_clicked()
{//期望双暴暴击按钮
    exp_related ^= 2;
    if (exp_related & 2) {
        ui.pushButton_19->setText(QString::fromStdWString(L"[") + ui.pushButton_19->text() + QString::fromStdWString(L"]"));
        ui.lineEdit_7->setText(d2qstring(exp_ctr = 0.5*(relics_ctr + 0.5*relics_ctd), 2));
        on_lineEdit_7_editingFinished();
        if (relics_or_exp == 0)ui.comboBox_5->setCurrentIndex(0);
    }
    else ui.pushButton_19->setText(ui.pushButton_19->text().mid(1, ui.pushButton_19->text().length() - 2));
}

void genshin_relics::on_pushButton_20_clicked()
{//期望元素爆发率按钮
    exp_related ^= 4;
    if (exp_related & 4)
        ui.pushButton_20->setText(QString::fromStdWString(L"[") + ui.pushButton_20->text() + QString::fromStdWString(L"]"));
    else ui.pushButton_20->setText(ui.pushButton_20->text().mid(1, ui.pushButton_20->text().length() - 2));
}

void genshin_relics::on_pushButton_21_clicked()
{//期望元素反应率按钮
    exp_related ^= 8;
    if (exp_related & 8)
        ui.pushButton_21->setText(QString::fromStdWString(L"[") + ui.pushButton_21->text() + QString::fromStdWString(L"]"));
    else ui.pushButton_21->setText(ui.pushButton_21->text().mid(1, ui.pushButton_21->text().length() - 2));
}

void genshin_relics::on_pushButton_24_clicked()
{//圣遗物总攻击力%按钮
    attr_fixed ^= 1;
    if (attr_fixed & 1)
        ui.pushButton_24->setText(QString::fromStdWString(L"[") + ui.pushButton_24->text() + QString::fromStdWString(L"]"));
    else ui.pushButton_24->setText(ui.pushButton_24->text().mid(1, ui.pushButton_24->text().length() - 2));
}

void genshin_relics::on_pushButton_25_clicked()
{//圣遗物总双暴按钮
    attr_fixed ^= 2;
    if (attr_fixed & 2)
        ui.pushButton_25->setText(QString::fromStdWString(L"[") + ui.pushButton_25->text() + QString::fromStdWString(L"]"));
    else ui.pushButton_25->setText(ui.pushButton_25->text().mid(1, ui.pushButton_25->text().length() - 2));
}

void genshin_relics::on_pushButton_26_clicked()
{//圣遗物总元素充能按钮
    attr_fixed ^= 4;
    if (attr_fixed & 4)
        ui.pushButton_26->setText(QString::fromStdWString(L"[") + ui.pushButton_26->text() + QString::fromStdWString(L"]"));
    else ui.pushButton_26->setText(ui.pushButton_26->text().mid(1, ui.pushButton_26->text().length() - 2));
}

void genshin_relics::on_pushButton_27_clicked()
{//圣遗物总元素精通按钮
    attr_fixed ^= 8;
    if (attr_fixed & 8)
        ui.pushButton_27->setText(QString::fromStdWString(L"[") + ui.pushButton_27->text() + QString::fromStdWString(L"]"));
    else ui.pushButton_27->setText(ui.pushButton_27->text().mid(1, ui.pushButton_27->text().length() - 2));
}

void genshin_relics::on_pushButton_28_clicked()
{//圣遗物总附加伤害按钮
    attr_fixed ^= 16;
    if (attr_fixed & 16)
        ui.pushButton_28->setText(QString::fromStdWString(L"[") + ui.pushButton_28->text() + QString::fromStdWString(L"]"));
    else ui.pushButton_28->setText(ui.pushButton_28->text().mid(1, ui.pushButton_28->text().length() - 2));
}

void genshin_relics::on_pushButton_22_clicked()
{//圣遗物总生命%按钮（和期望属性里面的按钮共用）
    attr_fixed ^= 32;
    if (attr_fixed & 32)
        ui.pushButton_22->setText(QString::fromStdWString(L"[") + ui.pushButton_22->text() + QString::fromStdWString(L"]"));
    else ui.pushButton_22->setText(ui.pushButton_22->text().mid(1, ui.pushButton_22->text().length() - 2));
}

void genshin_relics::on_pushButton_23_clicked()
{//圣遗物总防御%按钮（和期望属性里面的按钮共用）
    attr_fixed ^= 64;
    if (attr_fixed & 64)
        ui.pushButton_23->setText(QString::fromStdWString(L"[") + ui.pushButton_23->text() + QString::fromStdWString(L"]"));
    else ui.pushButton_23->setText(ui.pushButton_23->text().mid(1, ui.pushButton_23->text().length() - 2));
}


void genshin_relics::on_pushButton_29_clicked()
{//圣遗物点数按钮，进行点数分配优化
    constexpr static const double CTR = 50.0;
    constexpr static const double CTD = 100.0;
    constexpr static const double ADD = attr_ctr*attrpoint[10]*mainattr[3];
    double ctc_point = (CTR - 25.0) / (attr_ctr*attrpoint[addition_attr::ctr]) + (CTD - 50.0) / (attr_ctr*attrpoint[addition_attr::ctd]);
    double atk_point = (25.0 - 5.0) / (attr_ctr*attrpoint[addition_attr::ctr]);
    bool is_addition = relics_add > 34.0;
    int prior = (dmg_check_state != Qt::Unchecked ? 0 : ui.comboBox_2->currentIndex());
    double attrpoints = ui.lineEdit_17->text().toDouble() + mainattr[2] + mainattr[3] + mainattr[4] - atk_point;
    double attr_init[10] = {
        (is_addition ? (relics_add < ADD ? 0.0 : relics_add - ADD) : relics_add),
        41.4,//固定生命值4780，约为41.4%
        (dmg_check_state != Qt::Unchecked ? 0 : exp_obr),
        0.0,
        (dmg_check_state != Qt::Unchecked ? 0 : exp_rtr),
        38.4,//固定攻击力311，约为38.4%
        25.0,//攻击路线的暴击率
        50.0,//初始暴击伤害
        100.0,//初始元素充能
        0.0
    };
    if (attr_fixed & 1) {//固定攻击力%
        attrpoints -= (relics_atk - 38.4) / (attr_ctr*attrpoint[addition_attr::patk]);
        attr_init[addition_attr::patk] = relics_atk;
    }
    if (attr_fixed & 2) {//固定双暴
        attrpoints -= (relics_ctr - 25.0) / (attr_ctr*attrpoint[addition_attr::ctr]) + (relics_ctd - 50.0) / (attr_ctr*attrpoint[addition_attr::ctd]);
        attr_init[addition_attr::ctr] = relics_ctr;
        attr_init[addition_attr::ctd] = relics_ctd;
    }
    if (attr_fixed & 4) {//固定元素充能
        attrpoints -= (relics_ene - 100.0) / (attr_ctr*attrpoint[addition_attr::ene]);
        attr_init[addition_attr::ene] = relics_ene;
    }
    if (attr_fixed & 8) {//固定元素精通
        attrpoints -= relics_cx / (attr_ctr*attrpoint[addition_attr::cx]);
        attr_init[addition_attr::cx] = relics_cx;
    }
    if (attr_fixed & 16) {//固定附加伤害
        if (is_addition)attrpoints -= mainattr[3];
        attr_init[addition_attr::add] = relics_add;
    }
    if (attr_fixed & 32) {//固定生命%
        attrpoints -= (exp_lef - 41.4) / (attr_ctr*attrpoint[addition_attr::plef]);
        attr_init[addition_attr::plef] = exp_lef;
    }
    if (attr_fixed & 64) {//固定防御%
        attrpoints -= exp_def / (attr_ctr*attrpoint[addition_attr::pdef]);
        attr_init[addition_attr::pdef] = exp_def;
    }
    if (attrpoints <= 0.0) {
        QMessageBox::about(0, tr("点数分配"), tr("剩余点数过小，停止分配！点数：") + d2qstring(attrpoints, 2));
        return;
    }
    double attr[4][10];
    struct {
        double relics;
        int attr;
    } relics_on[4];
    memcpy(attr[0], attr_init, sizeof(attr_init));
    memcpy(attr[1], attr_init, sizeof(attr_init));
    memcpy(attr[2], attr_init, sizeof(attr_init));
    memcpy(attr[3], attr_init, sizeof(attr_init));
    relics_on[1].relics = relics_on[2].relics = relics_on[3].relics = -1.0;
    relics_on[0].attr = 0;//不带附加伤害杯子，走攻击力路线
    relics_on[1].attr = 1;//带附加伤害杯子，走攻击力路线
    relics_on[2].attr = 2;//不带附加伤害杯子，走双暴路线
    relics_on[3].attr = 3;//带附加伤害杯子，走双暴路线
    if ((attr_fixed & 2) == 0) {//不固定双暴
        attr[3][addition_attr::ctr] = attr[2][addition_attr::ctr] = CTR;
        attr[3][addition_attr::ctd] = attr[2][addition_attr::ctd] = CTD;
    }
    attr[3][addition_attr::add] = attr[1][addition_attr::add] = attr[1][addition_attr::add] + ADD;
    relics_on[0].relics = relics_assign(attrpoints, attr_fixed, prior, attr[0]);
    if ((attr_fixed & 16) == 0) {//不固定附加伤害
        if (attrpoints > mainattr[3])relics_on[1].relics = relics_assign(attrpoints - mainattr[3], attr_fixed, prior, attr[1]);
    }
    if ((attr_fixed & 2) == 0) {//不固定双暴
        if (attrpoints > ctc_point)relics_on[2].relics = relics_assign(attrpoints - ctc_point, attr_fixed, prior, attr[2]);
        if ((attr_fixed & 16) == 0) {//不固定附加伤害
            if (attrpoints > ctc_point + mainattr[3]) {
                relics_on[3].relics = relics_assign(attrpoints - (ctc_point + mainattr[3]), attr_fixed, prior, attr[3]);
            }
        }
    }
    qsort(relics_on, 4, sizeof(relics_on[0]), compare_double);
    QString text = tr("[点数分配]");
    for (int i = 0; i < 4; i++) {
        if (relics_on[i].relics < 0.0)continue;
        switch (relics_on[i].attr)
        {
        case 0:
            text.append(tr("\n攻击路线，评分：") + d2qstring(10, relics_on[i].relics) +
                "add=" + d2qstring(attr[0][addition_attr::add]));
            break;
        case 1:
            text.append(tr("\n攻击路线，评分：") + d2qstring(10, relics_on[i].relics) +
                "add=" + d2qstring(attr[1][addition_attr::add]));
            break;
        case 2:
            text.append(tr("\n双暴路线，评分：") + d2qstring(10, relics_on[i].relics) +
                "add=" + d2qstring(attr[2][addition_attr::add]));
            break;
        case 3:
            text.append(tr("\n双暴路线，评分：") + d2qstring(10, relics_on[i].relics) +
                "add=" + d2qstring(attr[3][addition_attr::add]));
            break;
        default:
            break;
        }
        text.append(QString::fromStdWString(L"\npatk plef pdef ctr  ctd  ene  cx\n"));
        text.append(d2qstring(5, attr[relics_on[i].attr][addition_attr::patk]));
        text.append(d2qstring(5, attr[relics_on[i].attr][addition_attr::plef]));
        text.append(d2qstring(5, attr[relics_on[i].attr][addition_attr::pdef]));
        text.append(d2qstring(5, attr[relics_on[i].attr][addition_attr::ctr]));
        text.append(d2qstring(5, attr[relics_on[i].attr][addition_attr::ctd]));
        text.append(d2qstring(5, attr[relics_on[i].attr][addition_attr::ene]));
        text.append(d2qstring(5, attr[relics_on[i].attr][addition_attr::cx]));
    }
    ui.textBrowser->setText(text);
}

void genshin_relics::on_lineEdit_17_editingFinished()
{//圣遗物点数输入框按钮，可以输入点数和计算点数
    double get_points = ui.lineEdit_17->text().toDouble();
    if (get_points <= 0.0) {//输入点数小于等于0，应该设置为计算得的点数
        get_points = (relics_atk - 38.4) / (attr_ctr*attrpoint[addition_attr::patk]) +
            (relics_ctr - 5.0) / (attr_ctr*attrpoint[addition_attr::ctr]) + (relics_ctd - 50.0) / (attr_ctr*attrpoint[addition_attr::ctd]) +
            (relics_ene - 100.0) / (attr_ctr*attrpoint[addition_attr::ene]) + relics_cx / (attr_ctr*attrpoint[addition_attr::cx]) +
            (exp_lef - 41.4) / (attr_ctr*attrpoint[addition_attr::plef]) + exp_def / (attr_ctr*attrpoint[addition_attr::pdef]) -
            mainattr[2] - (relics_add > 34.0 ? 0 : mainattr[3]) - mainattr[4];
        ui.lineEdit_17->setText(d2qstring(get_points));
    }
    else {
        ui.lineEdit_17->setText(d2qstring(get_points));
    }
}

void genshin_relics::on_menuOpenSlt()
{//打开图片动作
    QString dlgTitle = tr("打开图片"); //对话框标题
    QString filter = tr("所有图片(*.bmp *.jpg *.jpeg *.png);;PNG图片(*.png);;BMP图片(*.bmp);;JPEG图片(*.jpg *.jpeg);;所有文件(*.*)"); //文件过滤器
    QString file = QFileDialog::getOpenFileName(this, dlgTitle, nullptr, filter);
    QImage img;
    if (img.load(file)) {
        img.save(QCoreApplication::applicationDirPath() + QString::fromStdWString(L"/genshin_relics.png"));
        int w = img.width();
        int h = img.height();
        if (w * 238 > h * 318) {
            img = img.scaledToWidth(318, Qt::SmoothTransformation);
        }
        else {
            img = img.scaledToHeight(238, Qt::SmoothTransformation);
        }
        QGraphicsScene *scene = ui.graphicsView->scene();
        scene->clear();
        w = img.width();
        h = img.height();
        scene->setSceneRect(0, 0, w, h);
        scene->addPixmap(QPixmap::fromImage(img));
        ui.graphicsView->show();
        pic_ready = true;
        get_pic_attr();
        scores_ready = 0;
    }
    else {
        QMessageBox::critical(0, tr("打开图片"), tr("打开失败：") + file);
    }
}

void genshin_relics::on_menuSaveSlt()
{//保存图片动作
    if (pic_ready) {
        QString text;
        QString dlgTitle = tr("保存图片"); //对话框标题
        QString filter = tr("PNG图片(*.png);;BMP图片(*.bmp);;JPEG图片(*.jpg *.jpeg);;所有文件(*.*)"); //文件过滤器
        QString file = QFileDialog::getSaveFileName(this, dlgTitle, nullptr, filter);
        QGraphicsScene *scene = ui.graphicsView->scene();
        QImage img(scene->width(), scene->height(), QImage::Format_ARGB32);
        QPainter painter(&img);
        painter.setRenderHint(QPainter::Antialiasing);
        scene->render(&painter);
        QImage img2 = img.scaled(img.width() * 2, img.height() * 2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        QPainter painter2(&img2);
        painter2.setPen(Qt::red);
        painter2.setFont(QFont("Consolas", 16, QFont::Bold));
        if (scores_ready & 1) {
            text += tr("绝对总评:") + d2qstring(total_scores);
            text += tr("\n副属性分:") + d2qstring(addition_scores);
        }
        if (scores_ready & 2) {
            text += tr("\n相对总评:") + d2qstring(total_scores_2);
            text += tr("\n副属性分:") + d2qstring(addition_scores_2);
        }
        painter2.drawText(img2.rect(), Qt::AlignRight | Qt::AlignBottom, text);
        img2.save(file);
    }
    else {
        QMessageBox::critical(0, tr("保存图片"), tr("您还未打开或粘贴图片！"));
    }
}

void genshin_relics::on_menuAboutSlt()
{//关于动作
    QMessageBox box;
    box.setWindowTitle(tr("关于"));
    box.setText(tr("圣遗物特调是一款原神圣遗物工具，它集评分、优化、计算于一体。"
        "\n[评分]，可以对单个圣遗物进行两种类型的评分，绝对评分使优先属性具有绝对权，相对评分使优先属性相对优先，绝对优先给所有属性评分，而相对优先除了给优先属性评分外只给输出属性评分。"
        "\n绝对评分不收敛，相对评分收敛于最优属性，因此圣遗物点数分配采用相对评分体系，而绝对优先更适合评分体系。相对优先评分说明了各个属性对输出提升，这比绝对优先更准确。"
        "\n[优化]，即点数分配，一般地，一套圣遗物副属性点数最多450点，而毕业装输出点数才400 + ，加上武器一般450点左右，这是我们可以分配的点数。"
        "\n优化有两个方向，攻击路线适合前中期输出点数不到350，双暴路线（起始50 / 100双暴）适合后期点数达400 + ，主C推荐双暴路线，而辅C或辅助推荐攻击路线，后期辅C转型双暴路线。"
        "\n[计算]，能够计算一套圣遗物提供的副属性点数以及输出评分。武器和人物自带等属性也可以算到圣遗物里面，输出评分直接表明伤害，优先属性能够提升输出评分，一般输出评分高则圣遗物套装更强。"
        "\n不仅如此，还支持图片识别，从游戏界面直接截图（Alt + PrtSc）然后粘贴（Ctrl + V）到其中，可自动设置单个圣遗物的属性。"
        "\n"
        "\n打开图片：打开单个圣遗物的属性截图，并识别。"
        "\n保存图片：保存打开或粘贴的图片，如果有评分，则打印评分到保存的图片中。"
        "\n关于：帮助说明"
        "\n粘贴：打开剪切板中的图片数据（不是文件）并截取一部分作单个圣遗物的属性截图，并识别。"
        "\n剪贴\'：功能同粘贴，只是裁剪方式不一样。"
        "\n角色预设下拉选框：选择相应的预设，会影响定义的十二个分级。"
        "\n顶部五个按钮，对应圣遗物总属性，点按会固定属性（点数分配时不改变其属性）。"
        "\n其下十个按钮，对应十项副属性，与副属性四个按钮配合，以改变副属性项。"
        "\n副属性四个按钮，点按会出现\"[]\"以表示选中，然后按下十项副属性按钮，以填入副属性项。"
        "\n期望总属性下有六个按钮，攻击力%和双暴暴击按钮按下将其期望属性与圣遗物总属性同步，元素爆发率和元素反应率按钮按下分别将圣遗物总元素充能和元素精通用于评分（单个圣遗物评分默认使用固定的元素充能和精通），生命%和防御%按钮也是用作圣遗物总属性固定（其输入框是共用的）。"
        "\n更改期望 / 更改圣遗物按钮，下拉选框有十二个用于评分的分级，更改这个选框时影响期望总属性 / 圣遗物总属性。"
        "\n期望总属性右边选框，更改优先属性。"
        "\n圣遗物点数按钮，点按进行点数分配，其右输入框输入0或负值以计算圣遗物副属性点数（固定属性转换为百分比属性计算，会损失一定点数）。"
        "\n纯输出选框：选中以进行只考虑攻击力和双暴的计算。"
        "\n生命值lef  防御力def  攻击力atk  暴击率ctr  暴击伤害ctd"
        "\n元素充能ene  元素精通cx  附加伤害add  加p表示百分比，如patk"
        "\n"
        "\n作者：杨玉军"
        "\n于2021年4月19日完成  5月21日更新"));
    box.setStyleSheet("width : 900");
    box.exec();
}

void genshin_relics::on_menuPasteSlt()
{//粘贴动作
    QString fileName = QCoreApplication::applicationDirPath() + QString::fromStdWString(L"/genshin_relics.png");
    clipboard_to_picture(fileName, 0);
}

void genshin_relics::on_menuPaste2Slt()
{//剪贴'动作
    QString fileName = QCoreApplication::applicationDirPath() + QString::fromStdWString(L"/genshin_relics.png");
    clipboard_to_picture(fileName, 1);
}


int genshin_relics::compare_double(const void *a, const void *b)
{
    return *(double*)a < *(double*)b ? 1 : -1;
}

/*
 * 计算准确的能用于优化点数的圣遗物评分，以输出伤害为基准
 * 计算受优先属性影响
 */
double genshin_relics::relics_accurate(int prior, const double * attr)
{
    //可以通过以下系数调整优先度
    double PLEF = relics_coef_st.prior_coef[0] * 0.01;
    double PATK = relics_coef_st.prior_coef[1] * 0.01;
    double CTC = relics_coef_st.prior_coef[2] * 0.01;
    double PDEF = relics_coef_st.prior_coef[3] * 0.01;
    double ENE = relics_coef_st.prior_coef[4] * 0.01;
    double CX = relics_coef_st.prior_coef[5] * 0.01;
    double CTR = relics_coef_st.prior_coef[6] * 0.01;
    double CTD = relics_coef_st.prior_coef[7] * 0.01;
    double relics;
    double atk_point, ctr_point, ctd_point, ene_point, cx_point, lef_point, def_point;
    switch (prior)
    {
    case 1://生命值最优
        lef_point = attr[addition_attr::plef] / (attr_ctr * attrpoint[addition_attr::plef]);
        relics = ATK * (1.0 + attr[addition_attr::patk] * 0.01)*
            (1.0 + attr[addition_attr::ctr] * attr[addition_attr::ctd] * 0.0001)*
            (1.0 - attr[addition_attr::obr] + attr[addition_attr::ene] * 0.01 * attr[addition_attr::obr])*
            (1.0 - attr[addition_attr::rtr] + (1.0 + (20.0 / 3.0 - 9340.0 / (1401.0 + attr[addition_attr::cx]))*relics_coef_st.command_xe_coef)*1.75*attr[addition_attr::rtr])*
            (1.0 + attr[addition_attr::add] * 0.01)*
            (1.0 + lef_point * attr_ctr * relics_coef_st.prior_attrpoint * PLEF);
        break;
    case 2://攻击力最优
        atk_point = attr[addition_attr::patk] / (attr_ctr * attrpoint[addition_attr::patk]);
        relics = ATK * (1.0 + atk_point * attr_ctr * relics_coef_st.prior_attrpoint * PATK)*
            (1.0 + attr[addition_attr::ctr] * attr[addition_attr::ctd] * 0.0001)*
            (1.0 - attr[addition_attr::obr] + attr[addition_attr::ene] * 0.01 * attr[addition_attr::obr])*
            (1.0 - attr[addition_attr::rtr] + (1.0 + (20.0 / 3.0 - 9340.0 / (1401.0 + attr[addition_attr::cx]))*relics_coef_st.command_xe_coef)*1.75*attr[addition_attr::rtr])*
            (1.0 + attr[addition_attr::add] * 0.01);
        break;
    case 3://双暴最优
        ctr_point = attr[addition_attr::ctr] / (attr_ctr * attrpoint[addition_attr::ctr]);
        ctd_point = attr[addition_attr::ctd] / (attr_ctr * attrpoint[addition_attr::ctd]);
        relics = ATK * (1.0 + attr[addition_attr::patk] * 0.01)*
            (1.0 + attr[addition_attr::ctr] * attr[addition_attr::ctd] * 0.0001 +
                CTC*attr_ctr*(relics_coef_st.prior_attrpoint - attrpoint[addition_attr::patk])*(ctr_point + ctd_point))*
            (1.0 - attr[addition_attr::obr] + attr[addition_attr::ene] * 0.01 * attr[addition_attr::obr])*
            (1.0 - attr[addition_attr::rtr] + (1.0 + (20.0 / 3.0 - 9340.0 / (1401.0 + attr[addition_attr::cx]))*relics_coef_st.command_xe_coef)*1.75*attr[addition_attr::rtr])*
            (1.0 + attr[addition_attr::add] * 0.01);
        break;
    case 4://元素充能最优
        ene_point = attr[addition_attr::ene] / (attr_ctr * attrpoint[addition_attr::ene]);
        relics = ATK * (1.0 + attr[addition_attr::patk] * 0.01)*
            (1.0 + attr[addition_attr::ctr] * attr[addition_attr::ctd] * 0.0001)*
            (1.0 - attr[addition_attr::obr] + attr[addition_attr::ene] * 0.01 * attr[addition_attr::obr] +
                ene_point * attr_ctr * (relics_coef_st.prior_attrpoint - attrpoint[addition_attr::patk]) * ENE)*
            (1.0 - attr[addition_attr::rtr] + (1.0 + (20.0 / 3.0 - 9340.0 / (1401.0 + attr[addition_attr::cx]))*relics_coef_st.command_xe_coef)*1.75*attr[addition_attr::rtr])*
            (1.0 + attr[addition_attr::add] * 0.01);
        break;
    case 5://元素精通最优
        cx_point = attr[addition_attr::cx] / (attr_ctr * attrpoint[addition_attr::cx]);
        relics = ATK * (1.0 + attr[addition_attr::patk] * 0.01)*
            (1.0 + attr[addition_attr::ctr] * attr[addition_attr::ctd] * 0.0001)*
            (1.0 - attr[addition_attr::obr] + attr[addition_attr::ene] * 0.01 * attr[addition_attr::obr])*
            (1.0 - attr[addition_attr::rtr] + (1.0 + (20.0 / 3.0 - 9340.0 / (1401.0 + attr[addition_attr::cx]))*relics_coef_st.command_xe_coef)*1.75*attr[addition_attr::rtr] +
                cx_point * attr_ctr * (relics_coef_st.prior_attrpoint - attrpoint[addition_attr::patk]) * CX)*
            (1.0 + attr[addition_attr::add] * 0.01);
        break;
    case 6://防御力最优
        def_point = attr[addition_attr::pdef] / (attr_ctr * attrpoint[addition_attr::pdef]);
        relics = ATK * (1.0 + attr[addition_attr::patk] * 0.01)*
            (1.0 + attr[addition_attr::ctr] * attr[addition_attr::ctd] * 0.0001)*
            (1.0 - attr[addition_attr::obr] + attr[addition_attr::ene] * 0.01 * attr[addition_attr::obr])*
            (1.0 - attr[addition_attr::rtr] + (1.0 + (20.0 / 3.0 - 9340.0 / (1401.0 + attr[addition_attr::cx]))*relics_coef_st.command_xe_coef)*1.75*attr[addition_attr::rtr])*
            (1.0 + attr[addition_attr::add] * 0.01)*
            (1.0 + def_point * attr_ctr * relics_coef_st.prior_attrpoint * PDEF);
        break;
    case 7://暴击率最优
        ctr_point = attr[addition_attr::ctr] / (attr_ctr * attrpoint[addition_attr::ctr]);
        relics = ATK * (1.0 + attr[addition_attr::patk] * 0.01)*
            (1.0 + attr[addition_attr::ctr] * attr[addition_attr::ctd] * 0.0001 +
                CTR*attr_ctr*(relics_coef_st.prior_attrpoint - attrpoint[addition_attr::patk])*ctr_point)*
            (1.0 - attr[addition_attr::obr] + attr[addition_attr::ene] * 0.01 * attr[addition_attr::obr])*
            (1.0 - attr[addition_attr::rtr] + (1.0 + (20.0 / 3.0 - 9340.0 / (1401.0 + attr[addition_attr::cx]))*relics_coef_st.command_xe_coef)*1.75*attr[addition_attr::rtr])*
            (1.0 + attr[addition_attr::add] * 0.01);
        break;
    case 8://暴击伤害最优
        ctd_point = attr[addition_attr::ctd] / (attr_ctr * attrpoint[addition_attr::ctd]);
        relics = ATK * (1.0 + attr[addition_attr::patk] * 0.01)*
            (1.0 + attr[addition_attr::ctr] * attr[addition_attr::ctd] * 0.0001 +
                CTD*attr_ctr*(relics_coef_st.prior_attrpoint - attrpoint[addition_attr::patk])*ctd_point)*
            (1.0 - attr[addition_attr::obr] + attr[addition_attr::ene] * 0.01 * attr[addition_attr::obr])*
            (1.0 - attr[addition_attr::rtr] + (1.0 + (20.0 / 3.0 - 9340.0 / (1401.0 + attr[addition_attr::cx]))*relics_coef_st.command_xe_coef)*1.75*attr[addition_attr::rtr])*
            (1.0 + attr[addition_attr::add] * 0.01);
        break;
    default:
        relics = ATK * (1.0 + attr[addition_attr::patk] * 0.01)*
            (1.0 + attr[addition_attr::ctr] * attr[addition_attr::ctd] * 0.0001)*
            (1.0 - attr[addition_attr::obr] + attr[addition_attr::ene] * 0.01 * attr[addition_attr::obr])*
            (1.0 - attr[addition_attr::rtr] + (1.0 + (20.0 / 3.0 - 9340.0 / (1401.0 + attr[addition_attr::cx]))*relics_coef_st.command_xe_coef)*1.75*attr[addition_attr::rtr])*
            (1.0 + attr[addition_attr::add] * 0.01);
        break;
    }
    return relics;
}

/*
 * 计算收益图（不取对数值）
 * 计算受期望总属性和优先属性影响
 */
void genshin_relics::relics_analysis(int prior, const double * attr, double * attr_gain)
{
    double temp;
    //可以通过以下系数调整优先度
    double PLEF = relics_coef_st.prior_coef[0] * 0.01;
    double PATK = relics_coef_st.prior_coef[1] * 0.01;
    double CTC = relics_coef_st.prior_coef[2] * 0.01;
    double PDEF = relics_coef_st.prior_coef[3] * 0.01;
    double ENE = relics_coef_st.prior_coef[4] * 0.01;
    double CX = relics_coef_st.prior_coef[5] * 0.01;
    double CTR = relics_coef_st.prior_coef[6] * 0.01;
    double CTD = relics_coef_st.prior_coef[7] * 0.01;
    double atk_point, ctr_point, ctd_point, ene_point, cx_point, lef_point, def_point;
    /* 以下计算每十点属性增长的收益（不取对数值） */
    attr_gain[addition_attr::lef] = attr_gain[addition_attr::plef] = attr_gain[addition_attr::def] = attr_gain[addition_attr::pdef] = 1.0;
    temp = 1.0 + attr[addition_attr::patk] * 0.01;
    attr_gain[addition_attr::atk] = (1.0 + attr[addition_attr::patk] * 0.01 + 10.0 * attr_ctr*attrpoint[addition_attr::atk] / ATK) / temp;
    attr_gain[addition_attr::patk] = (1.0 + attr[addition_attr::patk] * 0.01 + 0.1 * attr_ctr*attrpoint[addition_attr::patk]) / temp;
    temp = 1.0 + attr[addition_attr::ctd] * 0.0001*attr[addition_attr::ctr];
    attr_gain[addition_attr::ctr] = (1.0 + attr[addition_attr::ctd] * 0.0001*(attr[addition_attr::ctr] + 10 * attr_ctr*attrpoint[addition_attr::ctr])) / temp;
    attr_gain[addition_attr::ctd] = (1.0 + attr[addition_attr::ctr] * 0.0001*(attr[addition_attr::ctd] + 10 * attr_ctr*attrpoint[addition_attr::ctd])) / temp;
    attr_gain[addition_attr::ene] = (1.0 - attr[addition_attr::obr] + (attr[addition_attr::ene] + 10 * attr_ctr*attrpoint[addition_attr::ene])*0.01*attr[addition_attr::obr]) /
        (1.0 - attr[addition_attr::obr] + attr[addition_attr::ene] * 0.01*attr[addition_attr::obr]);
    attr_gain[addition_attr::cx] = (1.0 - attr[addition_attr::rtr] + (1.0 +
        (20.0 / 3.0 - 9340.0 / (1401.0 + attr[addition_attr::cx] + 10 * attr_ctr*attrpoint[addition_attr::cx])) *relics_coef_st.command_xe_coef)*1.75*attr[addition_attr::rtr]) /
        (1.0 - attr[addition_attr::rtr] + (1.0 + (20.0 / 3.0 - 9340.0 / (1401.0 + attr[addition_attr::cx])) *relics_coef_st.command_xe_coef)*1.75*attr[addition_attr::rtr]);
    /* 优先属性影响计算 */
    switch (prior)
    {
    case 1://生命值最优
        lef_point = attr[addition_attr::plef] / (attr_ctr * attrpoint[addition_attr::plef]);
        temp = 1.0 + lef_point * attr_ctr * relics_coef_st.prior_attrpoint * PLEF;
        attr_gain[addition_attr::lef] = (1.0 + (lef_point + 1000.0 *attrpoint[addition_attr::lef] / (LEF* attrpoint[addition_attr::plef])) *
            attr_ctr * relics_coef_st.prior_attrpoint * PLEF) / temp;
        attr_gain[addition_attr::plef] = (1.0 + (lef_point + 10.0) * attr_ctr * relics_coef_st.prior_attrpoint * PLEF) / temp;
        break;
    case 2://攻击力最优
        atk_point = attr[addition_attr::patk] / (attr_ctr * attrpoint[addition_attr::patk]);
        temp = 1.0 + atk_point * attr_ctr * relics_coef_st.prior_attrpoint * PATK;
        attr_gain[addition_attr::atk] = (1.0 + (atk_point + 1000.0 *attrpoint[addition_attr::atk] / (ATK* attrpoint[addition_attr::patk])) *
            attr_ctr * relics_coef_st.prior_attrpoint * PATK) / temp;
        attr_gain[addition_attr::patk] = (1.0 + (atk_point + 10.0) * attr_ctr * relics_coef_st.prior_attrpoint * PATK) / temp;
        break;
    case 3://双暴最优
        ctr_point = attr[addition_attr::ctr] / (attr_ctr * attrpoint[addition_attr::ctr]);
        ctd_point = attr[addition_attr::ctd] / (attr_ctr * attrpoint[addition_attr::ctd]);
        temp = 1.0 + attr[addition_attr::ctr] * attr[addition_attr::ctd] * 0.0001 +
            CTC * attr_ctr*(relics_coef_st.prior_attrpoint - attrpoint[addition_attr::patk])*(ctr_point + ctd_point);
        attr_gain[addition_attr::ctr] = (1.0 + (attr[addition_attr::ctr] + 10 * attr_ctr*attrpoint[addition_attr::ctr]) * attr[addition_attr::ctd] * 0.0001 +
            CTC * attr_ctr*(relics_coef_st.prior_attrpoint - attrpoint[addition_attr::patk])*(ctr_point + ctd_point + 10.0)) / temp;
        attr_gain[addition_attr::ctd] = (1.0 + (attr[addition_attr::ctd] + 10 * attr_ctr*attrpoint[addition_attr::ctd]) * attr[addition_attr::ctr] * 0.0001 +
            CTC * attr_ctr*(relics_coef_st.prior_attrpoint - attrpoint[addition_attr::patk])*(ctr_point + ctd_point + 10.0)) / temp;
        break;
    case 4://元素充能最优
        ene_point = attr[addition_attr::ene] / (attr_ctr * attrpoint[addition_attr::ene]);
        temp = 1.0 - attr[addition_attr::obr] + attr[addition_attr::ene] * 0.01 * attr[addition_attr::obr] +
            ene_point * attr_ctr * (relics_coef_st.prior_attrpoint - attrpoint[addition_attr::patk]) * ENE;
        attr_gain[addition_attr::ene] = (1.0 - attr[addition_attr::obr] + (attr[addition_attr::ene] + 10 * attr_ctr*attrpoint[addition_attr::ene]) *
            0.01 * attr[addition_attr::obr] + (ene_point + 10.0) * attr_ctr * (relics_coef_st.prior_attrpoint - attrpoint[addition_attr::patk]) * ENE) / temp;
        break;
    case 5://元素精通最优
        cx_point = attr[addition_attr::cx] / (attr_ctr * attrpoint[addition_attr::cx]);
        temp = 1.0 - attr[addition_attr::rtr] + (1.0 + (20.0 / 3.0 - 9340.0 / (1401.0 + attr[addition_attr::cx]))*relics_coef_st.command_xe_coef)*1.75*attr[addition_attr::rtr] +
            cx_point * attr_ctr * (relics_coef_st.prior_attrpoint - attrpoint[addition_attr::patk]) * CX;
        attr_gain[addition_attr::cx] = (1.0 - attr[addition_attr::rtr] + (1.0 + (20.0 / 3.0 - 9340.0 / (1401.0 + attr[addition_attr::cx] +
            10 * attr_ctr*attrpoint[addition_attr::cx]))*relics_coef_st.command_xe_coef)*1.75*attr[addition_attr::rtr] +
            (cx_point + 10.0) * attr_ctr * (relics_coef_st.prior_attrpoint - attrpoint[addition_attr::patk]) * CX) / temp;
        break;
    case 6://防御力最优
        def_point = attr[addition_attr::pdef] / (attr_ctr * attrpoint[addition_attr::pdef]);
        attr_gain[addition_attr::def] = (1.0 + (def_point + 1000.0 *attrpoint[addition_attr::def] / (DEF* attrpoint[addition_attr::pdef])) *
            attr_ctr * relics_coef_st.prior_attrpoint * PDEF) / (1.0 + def_point * attr_ctr * relics_coef_st.prior_attrpoint * PDEF);
        attr_gain[addition_attr::pdef] = (1.0 + (def_point + 10.0) * attr_ctr * relics_coef_st.prior_attrpoint * PDEF) / (1.0 + def_point * attr_ctr * relics_coef_st.prior_attrpoint * PDEF);
        break;
    case 7://暴击率最优
        ctr_point = attr[addition_attr::ctr] / (attr_ctr * attrpoint[addition_attr::ctr]);
        temp = 1.0 + attr[addition_attr::ctr] * attr[addition_attr::ctd] * 0.0001 +
            CTR * attr_ctr*(relics_coef_st.prior_attrpoint - attrpoint[addition_attr::patk])*ctr_point;
        attr_gain[addition_attr::ctr] = (1.0 + (attr[addition_attr::ctr] + 10 * attr_ctr*attrpoint[addition_attr::ctr]) * attr[addition_attr::ctd] * 0.0001 +
            CTR * attr_ctr*(relics_coef_st.prior_attrpoint - attrpoint[addition_attr::patk])*(ctr_point + 10.0)) / temp;
        break;
    case 8://暴击伤害最优
        ctd_point = attr[addition_attr::ctd] / (attr_ctr * attrpoint[addition_attr::ctd]);
        temp = 1.0 + attr[addition_attr::ctr] * attr[addition_attr::ctd] * 0.0001 +
            CTD * attr_ctr*(relics_coef_st.prior_attrpoint - attrpoint[addition_attr::patk])*ctd_point;
        attr_gain[addition_attr::ctd] = (1.0 + attr[addition_attr::ctr] * (attr[addition_attr::ctd] + 10 * attr_ctr*attrpoint[addition_attr::ctd]) * 0.0001 +
            CTD * attr_ctr*(relics_coef_st.prior_attrpoint - attrpoint[addition_attr::patk])*(ctd_point + 10.0)) / temp;
        break;
    default:
        break;
    }
    if (attr[addition_attr::ctr] >= 97.4)attr_gain[addition_attr::ctr] = 1.0;//修正bug
}

double genshin_relics::relics_assign(double attrpoints, int attr_fixed, int prior, double * attr)
{
    double attr_points;
    double attr_gain[10];
    struct {
        double gain;
        int attr;
    } attr_st[10];
    relics_analysis(prior, attr, attr_gain);
    for (int i = 0; i < 10; i++) {
        attr_st[i].gain = attr_gain[i];
        attr_st[i].attr = i;
    }
    if (attr_fixed & 1) {//固定攻击力%
        attr_st[addition_attr::atk].gain = 1.0;
        attr_st[addition_attr::patk].gain = 1.0;
    }
    if (attr_fixed & 2) {//固定双暴
        attr_st[addition_attr::ctr].gain = 1.0;
        attr_st[addition_attr::ctd].gain = 1.0;
    }
    if (attr_fixed & 4) {//固定元素充能
        attr_st[addition_attr::ene].gain = 1.0;
    }
    if (attr_fixed & 8) {//固定元素精通
        attr_st[addition_attr::cx].gain = 1.0;
    }
    if (attr_fixed & 32) {//固定生命%
        attr_st[addition_attr::lef].gain = 1.0;
        attr_st[addition_attr::plef].gain = 1.0;
    }
    if (attr_fixed & 64) {//固定防御%
        attr_st[addition_attr::def].gain = 1.0;
        attr_st[addition_attr::pdef].gain = 1.0;
    }
    qsort(attr_st, 10, sizeof(attr_st[0]), compare_double);
    bool ret = attrpoints <= 10.01;
    attr_points = ret ? attrpoints : 10.0;
    switch (attr_st[0].attr) {
    case (addition_attr::lef):
        attr[addition_attr::plef] += attrpoint[addition_attr::lef] * 100.0 / LEF * attr_ctr* attr_points;
        break;
    case (addition_attr::plef):
        attr[addition_attr::plef] += attrpoint[addition_attr::plef] * attr_ctr* attr_points;
        break;
    case (addition_attr::def):
        attr[addition_attr::pdef] += attrpoint[addition_attr::def] * 100.0 / DEF * attr_ctr* attr_points;
        break;
    case (addition_attr::pdef):
        attr[addition_attr::pdef] += attrpoint[addition_attr::pdef] * attr_ctr* attr_points;
        break;
    case (addition_attr::atk):
        attr[addition_attr::patk] += attrpoint[addition_attr::atk] * 100.0 / ATK * attr_ctr* attr_points;
        break;
    case (addition_attr::patk):
        attr[addition_attr::patk] += attrpoint[addition_attr::patk] * attr_ctr* attr_points;
        break;
    case (addition_attr::ctr):
        attr[addition_attr::ctr] += attrpoint[addition_attr::ctr] * attr_ctr* attr_points;
        break;
    case (addition_attr::ctd):
        attr[addition_attr::ctd] += attrpoint[addition_attr::ctd] * attr_ctr* attr_points;
        break;
    case (addition_attr::ene):
        attr[addition_attr::ene] += attrpoint[addition_attr::ene] * attr_ctr* attr_points;
        break;
    case (addition_attr::cx):
        attr[addition_attr::cx] += attrpoint[addition_attr::cx] * attr_ctr* attr_points;
        break;
    default:
        break;
    }
    if (ret) return relics_accurate(prior, attr);
    return relics_assign(attrpoints - 10.0, attr_fixed, prior, attr);
}

/*
 * 全套圣遗物总属性评分函数，以输出伤害为基准
 * 计算受优先属性影响
 */
double genshin_relics::relics_evaluate(int prior, const double *attr)
{//计算公式：810*(1+atk/100)*(1+ctr*ctd/10000)*(1.0-obr+ene/100*obr)*(1-rtr+(1+(20/3-9340/(1401+cx))*cx_coef)*1.75*rtr)*(1+add/100)
    /* 这里为了计算优先属性影响，采用对数和指数运算，计算了每十点属性分配的收益（取对数值） */
    double eff_atk, eff_ctr, eff_ctd, eff_ene, eff_cx, eff_pro;
    eff_atk = log(1.0 + attr[addition_attr::patk] * 0.01) * 10 * attr_ctr*attrpoint[addition_attr::patk] / attr[addition_attr::patk];
    eff_cx = log(1.0 + 0.0001*attr[addition_attr::ctr] * attr[addition_attr::ctd]) *
        10 * attr_ctr*attrpoint[addition_attr::ctr] / (attr[addition_attr::ctr] + attr[addition_attr::ctd] * 0.5);
    eff_ctr = eff_cx * (attr[addition_attr::ctr] + attr[addition_attr::ctd] * 0.5) / (1.5*attr[addition_attr::ctr]);//暴击率和暴击伤害收益成比例的，但是同一个乘区，所以需特殊计算
    eff_ctd = eff_cx * (attr[addition_attr::ctr] + attr[addition_attr::ctd] * 0.5) / (1.5*attr[addition_attr::ctd]);//这个计算方法是解方程而得出的，保证双暴不优先时最後收益一致
    eff_ene = log(1.0 - attr[addition_attr::obr] + attr[addition_attr::ene] * 0.01 * attr[addition_attr::obr]) *
        10 * attr_ctr*attrpoint[addition_attr::ene] / (attr[addition_attr::ene] - 100.0);
    /* 元素精通收益按剧变反应和增幅反应比例为1:2 */
    eff_cx = log(1.0 - attr[addition_attr::rtr] + (1.0 + (20.0 / 3.0 - 9340.0 / (1401 + attr[addition_attr::cx])) *relics_coef_st.command_xe_coef)*1.75*attr[addition_attr::rtr]) *
        10 * attr_ctr*attrpoint[addition_attr::cx] / attr[addition_attr::cx];
    /* 取最大收益值 */
    eff_pro = eff_atk < eff_ctd ? eff_ctd : eff_atk;
    eff_pro = eff_pro < eff_ctr ? eff_ctr : eff_pro;
    eff_pro = eff_pro < eff_cx ? eff_cx : eff_pro;
    eff_pro = eff_pro < eff_ene ? eff_ene : eff_pro;
#if 0 /* 这是不受优先属性影响下的评分公式 */
    relics = ATK * (1.0 + attr[addition_attr::patk] / 100.0)*(1.0 + attr[addition_attr::ctr] * attr[addition_attr::ctd] / 10000.0)*
        (1.0 - attr[addition_attr::obr] + attr[addition_attr::ene] / 100 * attr[addition_attr::obr])*
        (1 - attr[addition_attr::rtr] + (1 + (20.0 / 3 - 9340.0 / (1401 + attr[addition_attr::cx]))*relics_coef_st.command_xe_coef)*1.75*attr[addition_attr::rtr])*
        (1.0 + attr[addition_attr::add] * 0.01);
#endif
    /* 优先属性影响计算 */
    switch (prior)
    {
    case 1://生命值最优
        eff_pro = relics_coef_st.prior_gain_coef*eff_pro*attr[addition_attr::lef] / (10 * attr_ctr*attrpoint[addition_attr::plef]);
        break;
    case 2://攻击力最优
        eff_atk = relics_coef_st.prior_gain_coef*eff_pro;
        eff_pro = 0;
        break;
    case 3://双暴最优
        if (eff_pro == eff_ctr || eff_pro == eff_ctd) {
            eff_ctr *= relics_coef_st.prior_gain_coef * 0.9;
            eff_ctd *= relics_coef_st.prior_gain_coef * 0.9;
        }
        else {
            eff_pro = relics_coef_st.prior_gain_coef * 1.8*eff_pro / (eff_ctr + eff_ctd);
            eff_ctr *= eff_pro;
            eff_ctd *= eff_pro;
        }
        eff_pro = 0;
        break;
    case 4://元素充能最优
        eff_ene = relics_coef_st.prior_gain_coef*eff_pro;
        eff_pro = 0;
        break;
    case 5://元素精通最优
        eff_cx = relics_coef_st.prior_gain_coef*eff_pro;
        eff_pro = 0;
        break;
    case 6://防御力最优
        eff_pro = relics_coef_st.prior_gain_coef*eff_pro*attr[addition_attr::def] / (10 * attr_ctr*attrpoint[addition_attr::pdef]);
        break;
    case 7://暴击率最优
        eff_ctr = relics_coef_st.prior_gain_coef*eff_pro;
        eff_pro = 0;
        break;
    case 8://暴击伤害最优
        eff_ctd = relics_coef_st.prior_gain_coef*eff_pro;
        eff_pro = 0;
        break;
    default:
        eff_pro = 0;
        break;
    }
    /* 这是受优先属性影响下的评分公式 */
    return exp(eff_pro + eff_atk * attr[addition_attr::patk] / (10 * attr_ctr*attrpoint[addition_attr::patk]) +
        eff_ctr * attr[addition_attr::ctr] / (10 * attr_ctr*attrpoint[addition_attr::ctr]) +
        eff_ctd * attr[addition_attr::ctd] / (10 * attr_ctr*attrpoint[addition_attr::ctd]) +
        eff_ene * (attr[addition_attr::ene] - 100.0) / (10 * attr_ctr*attrpoint[addition_attr::ene]) +
        eff_cx * attr[addition_attr::cx] / (10 * attr_ctr*attrpoint[addition_attr::cx])
    ) * ATK *(1.0 + attr[addition_attr::add] * 0.01);
}

/*
 * 计算收益图（取对数值）
 * 计算受期望总属性和优先属性影响
 */
void genshin_relics::addition_attr_analysis(int prior, const double *attr, double *attr_gain)
{
    /* 以下计算每十点属性增长的收益（取对数值） */
    double eff_pro;
    attr_gain[addition_attr::patk] = log(1.0 + attr[addition_attr::patk] * 0.01 + 0.05 * attr_ctr*attrpoint[addition_attr::patk]) -
        log(1.0 + attr[addition_attr::patk] * 0.01 - 0.05 * attr_ctr*attrpoint[addition_attr::patk]);
    attr_gain[addition_attr::atk] = attr_gain[addition_attr::patk] * (attrpoint[addition_attr::atk] * 100.0 / (ATK*attrpoint[addition_attr::patk]));
    attr_gain[addition_attr::ctr] = log(1.0 + attr[addition_attr::ctd] * 0.0001*(attr[addition_attr::ctr] + 5 * attr_ctr*attrpoint[addition_attr::ctr])) -
        log(1.0 + attr[addition_attr::ctd] * 0.0001*(attr[addition_attr::ctr] - 5 * attr_ctr*attrpoint[addition_attr::ctr]));
    attr_gain[addition_attr::ctd] = log(1.0 + attr[addition_attr::ctr] * 0.0001*(attr[addition_attr::ctd] + 5 * attr_ctr*attrpoint[addition_attr::ctd])) -
        log(1.0 + attr[addition_attr::ctr] * 0.0001*(attr[addition_attr::ctd] - 5 * attr_ctr*attrpoint[addition_attr::ctd]));
    attr_gain[addition_attr::cx] = log(1.0 - attr[addition_attr::rtr] + (1.0 + (20.0 / 3.0 - 9340.0 / (1401.0 + attr[addition_attr::cx] +
        5 * attr_ctr*attrpoint[addition_attr::cx])) *relics_coef_st.command_xe_coef)*1.75*attr[addition_attr::rtr]) -
        log(1.0 - attr[addition_attr::rtr] + (1.0 + (20.0 / 3.0 - 9340.0 / (1401.0 + attr[addition_attr::cx] -
        5 * attr_ctr*attrpoint[addition_attr::cx])) *relics_coef_st.command_xe_coef)*1.75*attr[addition_attr::rtr]);
    attr_gain[addition_attr::ene] = log(1.0 - attr[addition_attr::obr] + (attr[addition_attr::ene] +
        5 * attr_ctr*attrpoint[addition_attr::ene])*0.01*attr[addition_attr::obr]) -
        log(1.0 - attr[addition_attr::obr] + (attr[addition_attr::ene] -
        5 * attr_ctr*attrpoint[addition_attr::ene])*0.01*attr[addition_attr::obr]);
    /* 取最小收益值 */
    eff_pro = attr_gain[addition_attr::patk] > attr_gain[addition_attr::ctd] ? attr_gain[addition_attr::ctd] : attr_gain[addition_attr::patk];
    eff_pro = eff_pro > attr_gain[addition_attr::ctr] ? attr_gain[addition_attr::ctr] : eff_pro;
    eff_pro = eff_pro > attr_gain[addition_attr::cx] ? attr_gain[addition_attr::cx] : eff_pro;
    eff_pro = eff_pro > attr_gain[addition_attr::ene] ? attr_gain[addition_attr::ene] : eff_pro;
    attr_gain[addition_attr::plef] = eff_pro * relics_coef_st.gain_plef_coef;//生命%按最小收益值的某一倍率计算
    attr_gain[addition_attr::lef] = attr_gain[addition_attr::plef] * (attrpoint[addition_attr::lef] * 100.0 / (LEF*attrpoint[addition_attr::plef]));
    attr_gain[addition_attr::pdef] = eff_pro * relics_coef_st.gain_pdef_coef;//防御%按最小收益值的某一倍率计算
    attr_gain[addition_attr::def] = attr_gain[addition_attr::pdef] * (attrpoint[addition_attr::def] * 100.0 / (DEF*attrpoint[addition_attr::pdef]));
    /* 取最大收益值 */
    eff_pro = attr_gain[addition_attr::patk] < attr_gain[addition_attr::ctd] ? attr_gain[addition_attr::ctd] : attr_gain[addition_attr::patk];
    eff_pro = eff_pro < attr_gain[addition_attr::ctr] ? attr_gain[addition_attr::ctr] : eff_pro;
    eff_pro = eff_pro < attr_gain[addition_attr::cx] ? attr_gain[addition_attr::cx] : eff_pro;
    eff_pro = eff_pro < attr_gain[addition_attr::ene] ? attr_gain[addition_attr::ene] : eff_pro;
    /* 优先属性影响计算 */
    switch (prior)
    {
    case 1://生命值最优
        attr_gain[addition_attr::plef] = eff_pro * relics_coef_st.prior_gain_coef;
        attr_gain[addition_attr::lef] = attr_gain[addition_attr::plef] * (attrpoint[addition_attr::lef] * 100.0 / (LEF*attrpoint[addition_attr::plef]));
        break;
    case 2://攻击力最优
        attr_gain[addition_attr::patk] = eff_pro * relics_coef_st.prior_gain_coef;
        attr_gain[addition_attr::atk] = attr_gain[addition_attr::patk] * (attrpoint[addition_attr::atk] * 100.0 / (ATK*attrpoint[addition_attr::patk]));
        break;
    case 3://双暴最优
        if (eff_pro == attr_gain[addition_attr::ctr] || eff_pro == attr_gain[addition_attr::ctd]) {
            attr_gain[addition_attr::ctr] *= relics_coef_st.prior_gain_coef * 0.9;
            attr_gain[addition_attr::ctd] *= relics_coef_st.prior_gain_coef * 0.9;
        }
        else {
            eff_pro = relics_coef_st.prior_gain_coef * 1.8*eff_pro / (attr_gain[addition_attr::ctr] + attr_gain[addition_attr::ctd]);
            attr_gain[addition_attr::ctr] *= eff_pro;
            attr_gain[addition_attr::ctd] *= eff_pro;
        }
        break;
    case 4://元素充能最优
        attr_gain[addition_attr::ene] = eff_pro * relics_coef_st.prior_gain_coef;
        break;
    case 5://元素精通最优
        attr_gain[addition_attr::cx] = eff_pro * relics_coef_st.prior_gain_coef;
        break;
    case 6://防御力最优
        attr_gain[addition_attr::pdef] = eff_pro * relics_coef_st.prior_gain_coef;
        attr_gain[addition_attr::def] = attr_gain[addition_attr::pdef] * (attrpoint[addition_attr::def] * 100.0 / (DEF*attrpoint[addition_attr::pdef]));
        break;
    case 7://暴击率最优
        attr_gain[addition_attr::ctr] = relics_coef_st.prior_gain_coef*eff_pro;
        break;
    case 8://暴击伤害最优
        attr_gain[addition_attr::ctd] = relics_coef_st.prior_gain_coef*eff_pro;
        break;
    default:
        break;
    }
}

double genshin_relics::addition_attr_evaluate(const double *attr_gain)
{
    /* 取副属性值 */
    double patk, ctr, ctd, cx, ene;
    double plef, pdef;
    int addition[4] = { addition_first ,addition_second ,addition_third ,addition_fourth };
    double attr[4];
    attr[0] = ui.lineEdit->text().toDouble();
    attr[1] = ui.lineEdit_2->text().toDouble();
    attr[2] = ui.lineEdit_3->text().toDouble();
    attr[3] = ui.lineEdit_4->text().toDouble();
    patk = ctr = ctd = cx = ene = plef = pdef = 0.0;
    for (int i = 0; i < 4; i++) {
        switch (addition[i])
        {
        case (addition_attr::lef):
            plef += 100.0*attr[i] / LEF;
            break;
        case (addition_attr::plef):
            plef += attr[i];
            break;
        case (addition_attr::def):
            pdef += 100.0*attr[i] / DEF;
            break;
        case (addition_attr::pdef):
            pdef += attr[i];
            break;
        case (addition_attr::atk):
            patk += 100.0*attr[i] / ATK;
            break;
        case (addition_attr::patk):
            patk += attr[i];
            break;
        case (addition_attr::ctr):
            ctr += attr[i];
            break;
        case (addition_attr::ctd):
            ctd += attr[i];
            break;
        case (addition_attr::ene):
            ene += attr[i];
            break;
        case (addition_attr::cx):
            cx += attr[i];
            break;
        default:
            break;
        }
    }
    /* 计算副属性得分 */
    //addition_attr_analysis(ui.comboBox_2->currentIndex(),attr_ten,attr_gain);
    double scores = patk/(10 * attr_ctr*attrpoint[addition_attr::patk]) * attr_gain[addition_attr::patk] +
        ctr/(10 * attr_ctr*attrpoint[addition_attr::ctr]) * attr_gain[addition_attr::ctr] +
        ctd/(10 * attr_ctr*attrpoint[addition_attr::ctd]) * attr_gain[addition_attr::ctd] +
        cx/(10 * attr_ctr*attrpoint[addition_attr::cx]) * attr_gain[addition_attr::cx] +
        ene/(10 * attr_ctr*attrpoint[addition_attr::ene]) * attr_gain[addition_attr::ene] +
        plef/(10 * attr_ctr*attrpoint[addition_attr::plef]) * attr_gain[addition_attr::plef] +
        pdef/(10 * attr_ctr*attrpoint[addition_attr::pdef]) * attr_gain[addition_attr::pdef];
    /* 计算副属性理想得分 */
    double ideal_attr[10];
    memcpy(ideal_attr, attr_gain, sizeof(ideal_attr));
    switch (ui.comboBox->currentIndex())
    {
    case (addition_attr::lef):
        ideal_attr[addition_attr::lef] = 0;
        break;
    case (addition_attr::plef):
        ideal_attr[addition_attr::plef] = 0;
        break;
    case (addition_attr::def):
        ideal_attr[addition_attr::def] = 0;
        break;
    case (addition_attr::pdef):
        ideal_attr[addition_attr::pdef] = 0;
        break;
    case (addition_attr::atk):
        ideal_attr[addition_attr::atk] = 0;
        break;
    case (addition_attr::patk):
        ideal_attr[addition_attr::patk] = 0;
        break;
    case (addition_attr::ctr):
        ideal_attr[addition_attr::ctr] = 0;
        break;
    case (addition_attr::ctd):
        ideal_attr[addition_attr::ctd] = 0;
        break;
    case (addition_attr::ene):
        ideal_attr[addition_attr::ene] = 0;
        break;
    case (addition_attr::cx):
        ideal_attr[addition_attr::cx] = 0;
        break;
    default:
        break;
    }
    qsort(ideal_attr, 10, sizeof(double), compare_double);
    double max_scores = ideal_attr[0] * 6.0 + ideal_attr[1] + ideal_attr[2] + ideal_attr[3];
    /* 返回比值作为副属性评分 */
    return 100.0*scores / max_scores;
}

double genshin_relics::main_attr_evaluate()
{
    double main_attr = ui.lineEdit_5->text().toDouble();
    double scores, max_scores = 1.0;
    double main_prior[13];
    main_prior[0] = main_scores[0];
    main_prior[1] = main_scores[1];
    main_prior[2] = main_scores[2];
    main_prior[3] = main_scores[3];
    main_prior[4] = main_scores[4];
    main_prior[5] = main_scores[5];
    main_prior[6] = main_scores[6];
    main_prior[7] = main_scores[7];
    main_prior[8] = main_scores[8];
    main_prior[9] = main_scores[9];
    main_prior[10] = main_scores[10];
    main_prior[11] = main_scores[11];
    main_prior[12] = main_scores[12];
    //memcpy(main_prior, main_scores, sizeof(main_scores));//this has a bug for assignment is no constexpr
    /* 依据优先属性，修改主属性顶级时得分 */
    switch (ui.comboBox_2->currentIndex())
    {
    case 1://生命值最优
        main_prior[addition_attr::plef] = 120.0;
        max_scores = 1.2;
        break;
    case 2://攻击力最优
        main_prior[addition_attr::patk] = 120.0;
        max_scores = 1.2;
        break;
    case 3://双暴最优
        main_prior[addition_attr::ctr] = 240.0*main_scores[addition_attr::ctr] / (main_scores[addition_attr::ctr] + main_scores[addition_attr::ctd]);
        main_prior[addition_attr::ctd] = 240.0*main_scores[addition_attr::ctd] / (main_scores[addition_attr::ctr] + main_scores[addition_attr::ctd]);
        max_scores = fmax(main_prior[addition_attr::ctr], main_prior[addition_attr::ctd]) * 0.01;
        break;
    case 4://元素充能最优
        main_prior[addition_attr::ene] = 120.0;
        max_scores = 1.2;
        break;
    case 5://元素精通最优
        main_prior[addition_attr::cx] = 120.0;
        max_scores = 1.2;
        break;
    case 6://防御力最优
        main_prior[addition_attr::pdef] = 120.0;
        max_scores = 1.2;
        break;
    case 7://暴击率最优
        main_prior[addition_attr::ctr] = 120.0;
        max_scores = 1.2;
        break;
    case 8://暴击伤害最优
        main_prior[addition_attr::ctd] = 120.0;
        max_scores = 1.2;
        break;
    default:
        break;
    }
    /* 计算主属性得分 */
    int p = ui.comboBox->currentIndex();
    switch (p)
    {
    case (addition_attr::lef):
        scores = main_prior[addition_attr::lef] * main_attr / (mainattr[0] * attr_ctr*attrpoint[addition_attr::lef]);
        max_scores = 1.0;
        break;
    case (addition_attr::plef):
        scores = main_prior[addition_attr::plef] * main_attr / (mainattr[2] * attr_ctr*attrpoint[addition_attr::plef]);
        break;
    case (addition_attr::def):
        scores = main_prior[addition_attr::def] * main_attr / (mainattr[0] * attr_ctr*attrpoint[addition_attr::def]);
        max_scores = 1.0;
        break;
    case (addition_attr::pdef):
        scores = main_prior[addition_attr::pdef] * main_attr / (mainattr[2] * attr_ctr*attrpoint[addition_attr::pdef]);
        break;
    case (addition_attr::atk):
        scores = main_prior[addition_attr::atk] * main_attr / (mainattr[1] * attr_ctr*attrpoint[addition_attr::atk]);
        max_scores = 1.0;
        break;
    case (addition_attr::patk):
        scores = main_prior[addition_attr::patk] * main_attr / (mainattr[2] * attr_ctr*attrpoint[addition_attr::patk]);
        break;
    case (addition_attr::ctr):
        scores = main_prior[addition_attr::ctr] * main_attr / (mainattr[4] * attr_ctr*attrpoint[addition_attr::ctr]);
        break;
    case (addition_attr::ctd):
        scores = main_prior[addition_attr::ctd] * main_attr / (mainattr[4] * attr_ctr*attrpoint[addition_attr::ctd]);
        break;
    case (addition_attr::ene):
        scores = main_prior[addition_attr::ene] * main_attr / (mainattr[2] * attr_ctr*attrpoint[addition_attr::ene]);
        break;
    case (addition_attr::cx):
        scores = main_prior[addition_attr::cx] * main_attr / (mainattr[2] * attr_ctr*attrpoint[addition_attr::cx]);
        break;
    case 10://元素伤害
        scores = main_prior[10] * main_attr / (mainattr[3] * attr_ctr*attrpoint[10]);
        break;
    case 11://物理伤害
        scores = main_prior[11] * main_attr / (mainattr[3] * attr_ctr*attrpoint[11]);
        break;
    case 12://治疗加成
        scores = main_prior[12] * main_attr / (mainattr[4] * attr_ctr*attrpoint[12]);
        break;
    default:
        throw QString::fromStdWString(L"index out of limit!");
        /*if (p < 0 || p>12)throw QString::fromStdWString(L"index out of limit!");
        else scores = main_prior[p] * main_attr / (mainattr[2] * attr_ctr*attrpoint[p]);
        //this has a bug for assignment is no constexpr */
        break;
    }
    return scores / max_scores;
}

bool genshin_relics::clipboard_to_picture(const QString & filePath, int flags)
{
    int x, y, w, h;
    const QMimeData *mimeData = QApplication::clipboard()->mimeData();
    if (mimeData->hasImage())
    {
        //将图片数据转为QImage
        QImage img = qvariant_cast<QImage>(mimeData->imageData());
        w = img.width();
        h = img.height();
        if (relics_coef_st.cut_pictrue != 0) {//因为窗口模式带边框
            x = (int)((relics_coef_st.cut_point_x - 1) / 1920.0 * (w - 2)) + 1;
            y = (int)((relics_coef_st.cut_point_y - 32) / 1080.0 * (h - 33)) + 32;
            w = (int)((relics_coef_st.cut_point_2_x - 1) / 1920.0 * (w - 2)) + 1.5 - x;
            h = (int)((relics_coef_st.cut_point_2_y - 32) / 1080.0 * (h - 33)) + 32.5 - y;
        }
        else {
            w -= 2;
            h -= 33;
            double a = (double)w / (double)h;
            double m, n, t, u;
            if ((a < 1.801) ^ flags) {//电脑截图，系数已确定
                m = 0.00070421*a + 0.67349;
                n = 0.092216*a - 0.0042476;
                t = -0.00091932*a + 0.2691;
                u = 0.24789*a + 0.01261;
            }
            else {//手机截图，系数已确定
                m = 0.078987*a + 0.50042;
                n = 0.0030456*a + 0.19313;
                t = -0.14349*a + 0.58732;
                u = 0.00032361*a + 0.56641;
            }
            x = m * w + 1;
            y = n * h + 32;
            w = t * w + 0.5;
            h = u * h + 0.5;
        }
        img = img.copy(x, y, w, h);
        img.save(filePath);
        w = img.width();
        h = img.height();
        if (w * 238 > h * 318) {
            img = img.scaledToWidth(318, Qt::SmoothTransformation);
        }
        else {
            img = img.scaledToHeight(238, Qt::SmoothTransformation);
        }
        QGraphicsScene *scene = ui.graphicsView->scene();
        scene->clear();
        w = img.width();
        h = img.height();
        scene->setSceneRect(0, 0, w, h);
        scene->addPixmap(QPixmap::fromImage(img));
        ui.graphicsView->show();
        pic_ready = true;
        get_pic_attr();
        scores_ready = 0;
        return true;
    }
    return false;
}

DWORD ShellRun(const _TCHAR * csExe, const _TCHAR * csParam, DWORD nShow)
{//执行带参数的程序
    SHELLEXECUTEINFO ShExecInfo;
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = NULL;
    ShExecInfo.lpFile = csExe;
    ShExecInfo.lpParameters = csParam;
    ShExecInfo.lpDirectory = NULL;
    ShExecInfo.nShow = nShow;
    ShExecInfo.hInstApp = NULL;
    DWORD dwCode = 0;
    BOOL ret = ShellExecuteEx(&ShExecInfo);
    if(ret != FALSE){
        WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
        GetExitCodeProcess(ShExecInfo.hProcess, &dwCode);
        CloseHandle(ShExecInfo.hProcess);
    }
    else dwCode = GetLastError();
    return dwCode;
}

void genshin_relics::get_pic_attr()
{//获取图片中的属性数值
    QString fileName = QCoreApplication::applicationDirPath() + QString::fromStdWString(L"/genshin_relics.txt");
    fileName = QDir::toNativeSeparators(fileName);
    std::wstring wcs = fileName.toStdWString();//这里必须复制，以防临时变量的析构
    const wchar_t *file = wcs.data();
    //_tsystem(_T("ocr genshin_relics.png > genshin_relics.txt"));
    //需要不显示窗口地运行cmd命令
    ShellRun(_T("cmd.exe"), _T("/c ocr genshin_relics.png > genshin_relics.txt"), SW_HIDE);
    FILE *fp = _wfopen(file, L"rb");
    char txt[4096];
    wchar_t buf[4096];
    int len=fread(txt, 1, 4096, fp);
    txt[len] = '\0';
    GBCToUnicode(txt, buf, 4096);
    fclose(fp);
    std::wstring tips1 = tr("生命").toStdWString();
    std::wstring tips2 = tr("防御").toStdWString();
    std::wstring tips3 = tr("攻击").toStdWString();
    std::wstring tips4 = tr("暴击率").toStdWString();
    std::wstring tips5 = tr("暴击伤害").toStdWString();
    std::wstring tips6 = tr("元素充能").toStdWString();
    std::wstring tips7 = tr("元素精通").toStdWString();
    std::wstring tips8 = tr("元素伤害").toStdWString();
    std::wstring tips9 = tr("物理伤害").toStdWString();
    std::wstring tips10 = tr("治疗").toStdWString();
    const wchar_t *main_attr[10] = {
        tips1.data(),
        tips2.data(),
        tips3.data(),
        tips4.data(),
        tips5.data(),
        tips6.data(),
        tips7.data(),
        tips8.data(),
        tips9.data(),
        tips10.data()
    };
    wchar_t *pos_main[10];
    int index=0;
    /* 取第一个找到的属性项作主属性 */
    for (int i = 0; i < 10; i++) {
        pos_main[i] = wcsstr(buf, main_attr[i]);
        if (pos_main[i] == NULL)pos_main[i] = buf + 4096;
        if (pos_main[i] < pos_main[index])index = i;
    }
    double value;
    wchar_t *left, *right, *pos = pos_main[index] + wcslen(main_attr[index]);
    left = pos_main[index];
    right = pos;
    /* 从主属性项的文本向左、向右查找数字 */
    while (left > buf && !is_num(*left))left--;
    while (left > buf && !is_num(left[-1]) && (left[-1] != L'.' || !is_num(left[-2]))) {//跳过单独的数字
        left--;
        while (left > buf && !is_num(*left))left--;
    }
    while (*right != L'\0' && !is_num(*right))right++;
    while (*right != L'\0' && !is_num(right[1]) && (right[1] != L'.' || !is_num(right[2]))) {//应该找带两个以上数字的数值
        right++;
        while (*right != L'\0' && !is_num(*right))right++;
    }
    /* 选择离主属性项文本更近的数值并解析 */
    if (pos_main[index] - left > right - pos) {//选择right的数字
        value = wcstod(right, &pos);
    }
    else {//选择left的数字
        while (left > buf && (is_num(*left) || *left == L'.'))left--;
        while (!is_num(*left))left++;
        value = wcstod(left, NULL);
        right = left;
    }
    /* 解析主属性并设置 */
    switch (index)
    {//需注意百分号可能识别错误，固定属性不可能带点号
    case 0:
        if (right[-1] == L',' && is_num(right[-2]) && is_num(right[1]) && is_num(right[2])) {
            //固定生命值可能带逗号
            ui.comboBox->setCurrentIndex(addition_attr::lef);
            value += (right[-2] - L'0')*1000.0;
        }
        else {
            //固定生命值可能带逗号但被识别为点号
            while (*right != L'\0' && is_num(*right))right++;
            left = right;
            if (*left == L'.') {
                left++;
                while (*left != L'\0' && is_num(*left))left++;
            }
            if (*left != L'%'&& value < 4.781) {
                ui.comboBox->setCurrentIndex(addition_attr::lef);
                value *= 1000.0;
            }
            else {//固定生命值不可能带点号，但百分号可能被识别错误
                if (*right != L'.' && *right != L'%')ui.comboBox->setCurrentIndex(addition_attr::lef);
                else ui.comboBox->setCurrentIndex(addition_attr::plef);
            }
        }
        break;
    case 1:
        while (*right != L'\0' && is_num(*right))right++;
        if (*right != L'.' && *right != L'%') {
            ui.comboBox->setCurrentIndex(addition_attr::def);
        }
        else ui.comboBox->setCurrentIndex(addition_attr::pdef);
        break;
    case 2:
        while (*right != L'\0' && is_num(*right))right++;
        if (*right != L'.' && *right != L'%')ui.comboBox->setCurrentIndex(addition_attr::atk);
        else ui.comboBox->setCurrentIndex(addition_attr::patk);
        break;
    default:
        ui.comboBox->setCurrentIndex(index + addition_attr::ctr - 3);
        break;
    }
    ui.lineEdit_5->setText(d2qstring(value));
    struct {
        double pos_v;//用double存储指针的差值，是为了能用double进行排序
        int index;//代表副属性项
    } pos_attr[10];
    pos_attr[7].pos_v = pos_attr[8].pos_v = pos_attr[9].pos_v = -4096.0;//初始取最小值
    for (int i = 0; i < 3; i++) {
        right = wcsstr(pos, main_attr[i]);
        if (right != NULL) {//可能存在第二个副属性项，比如两个攻击:固定攻击力、攻击力%
            pos_attr[i].pos_v = (double)(pos - right);//取负数，距离近的数值大
            right = wcsstr(right + 1, main_attr[i]);
            if (right != NULL) {
                pos_attr[i + 7].pos_v = (double)(pos - right);
            }
            else pos_attr[i + 7].pos_v = -4096.0;
            pos_attr[i + 7].index = i;
        }
        else pos_attr[i].pos_v = -4096.0;
        pos_attr[i].index = i;
    }
    for (int i = 3; i < 7; i++) {//这里不存在第二个同样的副属性项文本
        right = wcsstr(pos, main_attr[i]);
        if (right != NULL)pos_attr[i].pos_v = (double)(pos - right);
        else pos_attr[i].pos_v = -4096.0;
        pos_attr[i].index = i;
    }
    qsort(pos_attr, 10, sizeof(pos_attr[0]), compare_double);//快速排序
    on_pushButton_clicked();//通过模拟按按钮来设置副属性
    for (int i = 0; i < 4; i++) {
        index = -pos_attr[i].pos_v;
        if (index == 4096)break;
        right = pos + index;//计算副属性文本的位置
        while (*right != L'\0' && !is_num(*right))right++;//找到文本後面的数值
        value = wcstod(right, NULL);
        switch (pos_attr[i].index)
        {//需注意百分号可能识别错误，固定属性不可能带点号
        case 0:
            if (right[-1] == L',' && is_num(right[-2]) && is_num(right[1]) && is_num(right[2])) {
                //固定生命值可能带逗号
                on_pushButton_5_clicked();
                value += (right[-2] - L'0')*1000.0;
            }
            else {
                //固定生命值可能带逗号但被识别为点号
                while (*right != L'\0' && is_num(*right))right++;
                left = right;
                if (*left == L'.') {
                    left++;
                    while (*left != L'\0' && is_num(*left))left++;
                }
                if (*left != L'%'&& value < 4.781) {
                    value *= 1000.0;
                    on_pushButton_5_clicked();
                }
                else {
                    if (*right != L'.' && *right != L'%')on_pushButton_5_clicked();
                    else on_pushButton_6_clicked();
                }
            }
            break;
        case 1:
            while (*right != L'\0' && is_num(*right))right++;
            if (*right != L'.' && *right != L'%')on_pushButton_7_clicked();
            else on_pushButton_8_clicked();
            break;
        case 2:
            while (*right != L'\0' && is_num(*right))right++;
            if (*right != L'.' && *right != L'%')on_pushButton_9_clicked();
            else on_pushButton_10_clicked();
            break;
        case 3:
            on_pushButton_11_clicked();
            break;
        case 4:
            on_pushButton_12_clicked();
            break;
        case 5:
            on_pushButton_13_clicked();
            break;
        case 6:
            on_pushButton_14_clicked();
            break;
        default:
            break;
        }
        switch (i)
        {//设置副属性输入框
        case 0:
            ui.lineEdit->setText(d2qstring(value));
            break;
        case 1:
            ui.lineEdit_2->setText(d2qstring(value));
            break;
        case 2:
            ui.lineEdit_3->setText(d2qstring(value));
            break;
        case 3:
            ui.lineEdit_4->setText(d2qstring(value));
            break;
        default:
            break;
        }
    }
}

void genshin_relics::draw_gain_attr(int index)
{//绘制收益图折线图
    QImage img(318, 238, QImage::Format_ARGB32);
    img.fill(Qt::white);//新建的QImage数据是脏的，所以需要初始化
    QPoint lef, plef, def, pdef, atk, patk, ctr, ctd, ene, cx;
    QPainter painter(&img);
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(10, 20, 10, 208);
    painter.drawLine(10, 208, 308, 208);
    painter.drawLine(10, 20, 6, 24);
    painter.drawLine(10, 20, 14, 24);
    painter.drawLine(308, 208, 304, 204);
    painter.drawLine(308, 208, 304, 212);
    double gain[10],*gain_p;
    if (index != 0)gain_p = attr_gain_2;
    else gain_p = attr_gain;
    memcpy(gain, gain_p, sizeof(gain));
    qsort(gain, 10, sizeof(double), compare_double);
    lef.setX(20);
    plef.setX(20);
    def.setX(66);
    pdef.setX(66);
    atk.setX(112);
    patk.setX(112);
    ctr.setX(158);
    ctd.setX(204);
    ene.setX(250);
    cx.setX(296);
    lef.setY((int)(208.499999 - gain_p[addition_attr::lef] / gain[0] * 180.0));
    plef.setY((int)(208.499999 - gain_p[addition_attr::plef] / gain[0] * 180.0));
    def.setY((int)(208.499999 - gain_p[addition_attr::def] / gain[0] * 180.0));
    pdef.setY((int)(208.499999 - gain_p[addition_attr::pdef] / gain[0] * 180.0));
    atk.setY((int)(208.499999 - gain_p[addition_attr::atk] / gain[0] * 180.0));
    patk.setY((int)(208.499999 - gain_p[addition_attr::patk] / gain[0] * 180.0));
    ctr.setY((int)(208.499999 - gain_p[addition_attr::ctr] / gain[0] * 180.0));
    ctd.setY((int)(208.499999 - gain_p[addition_attr::ctd] / gain[0] * 180.0));
    ene.setY((int)(208.499999 - gain_p[addition_attr::ene] / gain[0] * 180.0));
    cx.setY((int)(208.499999 - gain_p[addition_attr::cx] / gain[0] * 180.0));
    painter.drawLine(lef, pdef);
    painter.drawLine(def, patk);
    painter.drawLine(atk, ctr);
    painter.setPen(QPen(Qt::red, 2));
    painter.drawLine(plef, pdef);
    painter.drawLine(pdef, patk);
    painter.drawLine(patk, ctr);
    painter.drawLine(ctr, ctd);
    painter.drawLine(ctd, ene);
    painter.drawLine(ene, cx);
    QPen pen(Qt::gray, 1);
    pen.setStyle(Qt::DashLine);
    painter.setPen(pen);
    painter.drawLine(10, 73, 308, 73);
    painter.drawLine(10,118,308,118);
    painter.drawLine(10, 163, 308, 163);
    painter.drawLine(20, 20, 20, 208);
    painter.drawLine(66, 20, 66, 208);
    painter.drawLine(112, 20, 112, 208);
    painter.drawLine(158, 20, 158, 208);
    painter.drawLine(204, 20, 204, 208);
    painter.drawLine(250, 20, 250, 208);
    painter.drawLine(296, 20, 296, 208);
    painter.setPen(QPen(Qt::black, 1));
    painter.setFont(QFont("SimSun", 10, QFont::Bold));
    painter.drawText(0, 15, "gain");
    painter.drawText(10,228,"lef");
    painter.drawText(56, 228, "def");
    painter.drawText(102, 228, "atk");
    painter.drawText(148, 228, "ctr");
    painter.drawText(194, 228, "ctd");
    painter.drawText(240, 228, "ene");
    painter.drawText(286, 228, "cx");
    QGraphicsScene *scene = ui.graphicsView_2->scene();
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(img));
    ui.graphicsView_2->show();
}

void genshin_relics::on_menuLangSlt()
{//切换语言
    qApp->removeTranslator(translator);
    delete translator;
    translator = new QTranslator(qApp);
    if (language != 0) {
        translator->load("genshin_relics_zh", ":/translations");
        qApp->installTranslator(translator);
        ui.comboBox->setFixedWidth(120);
        ui.comboBox_2->setFixedWidth(100);
        ui.comboBox_5->setGeometry(720, 600, 110, 36);
        ui.mainToolBar->removeAction(action_com_en);
        ui.mainToolBar->addAction(action_com_zh);
    }
    else {
        translator->load("genshin_relics_en", ":/translations");
        qApp->installTranslator(translator);
        ui.comboBox->setFixedWidth(150);
        ui.comboBox_2->setFixedWidth(140);
        ui.comboBox_5->setGeometry(635, 600, 225, 36);
        ui.mainToolBar->removeAction(action_com_zh);
        ui.mainToolBar->addAction(action_com_en);
    }
    language = ~language;
    retranslateUi();
}

void genshin_relics::retranslateUi()
{//更新翻译
    ui.retranslateUi(this);
    int t_attr_fixed = attr_fixed, t_exp_related = exp_related;
    attr_fixed = exp_related = 0;
    if (t_attr_fixed & 1) on_pushButton_24_clicked();
    if (t_attr_fixed & 2) on_pushButton_25_clicked();
    if (t_attr_fixed & 4) on_pushButton_26_clicked();
    if (t_attr_fixed & 8) on_pushButton_27_clicked();
    if (t_attr_fixed & 16) on_pushButton_28_clicked();
    if (t_attr_fixed & 32) on_pushButton_22_clicked();
    if (t_attr_fixed & 64) on_pushButton_23_clicked();
    if (t_exp_related & 1) on_pushButton_18_clicked();
    if (t_exp_related & 2) on_pushButton_19_clicked();
    if (t_exp_related & 4) on_pushButton_20_clicked();
    if (t_exp_related & 8) on_pushButton_21_clicked();
    int t_relics_addindex = relics_addindex;
    relics_addindex = 0;
    switch (t_relics_addindex) {
    case 1:
        on_pushButton_clicked();
        break;
    case 2:
        on_pushButton_2_clicked();
        break;
    case 3:
        on_pushButton_3_clicked();
        break;
    case 4:
        on_pushButton_4_clicked();
        break;
    }
    ui.textBrowser->setText(tr("圣遗物评分为："));
    menuOpen->setText(tr("打开图片"));
    menuSave->setText(tr("保存图片"));
    menuAbout->setText(tr("关于"));
    menuPaste->setText(tr("粘贴"));
    menuPaste2->setText(tr("剪贴\'"));
    menuLang->setText(tr("简体中文"));
}

void genshin_relics::on_comboBox_character(int index)
{//更改角色预设
    combox_en->setCurrentIndex(index);
    combox_zh->setCurrentIndex(index);
    ui.comboBox_5->setCurrentIndex(0);
    int i = relics_coef_st.index;
    relics_atk = relics_coef_st.relics_exp[i].relics_atk;
    relics_ctr = relics_coef_st.relics_exp[i].relics_ctr;
    relics_ctd = relics_ctr < 25.0 ? 50.0 : 2.0*relics_ctr;
    relics_ene = relics_coef_st.relics_exp[i].relics_ene;
    relics_cx = relics_coef_st.relics_exp[i].relics_cx;
    relics_add = relics_coef_st.relics_exp[i].relics_add;
    exp_ctr = relics_coef_st.relics_exp[i].exp_ctr;
    exp_atk = relics_coef_st.relics_exp[i].exp_atk;
    exp_obr = relics_coef_st.relics_exp[i].exp_obr;
    exp_rtr = relics_coef_st.relics_exp[i].exp_rtr;
    exp_lef = relics_coef_st.relics_exp[i].exp_lef;
    exp_def = relics_coef_st.relics_exp[i].exp_def;
    if (attr_fixed & 1) on_pushButton_24_clicked();
    if (attr_fixed & 2) on_pushButton_25_clicked();
    if (attr_fixed & 4) on_pushButton_26_clicked();
    if (attr_fixed & 8) on_pushButton_27_clicked();
    if (attr_fixed & 16) on_pushButton_28_clicked();
    if (attr_fixed & 32) on_pushButton_22_clicked();
    if (attr_fixed & 64) on_pushButton_23_clicked();
    if (exp_related & 4) on_pushButton_20_clicked();
    if (exp_related & 8) on_pushButton_21_clicked();
    ui.comboBox_2->setCurrentIndex(0);
    if (index != 0) {//第一种形式
        struct relics_preset *it = relics_coef_st.character_preset.data();
        index--;
        i = it[index].index >> 4;
        if (i > 1) {//第二种形式
            exp_obr = fabs(it[index].preset_obr);
            if (it[index].preset_obr < 0) on_pushButton_20_clicked();//小于0则固定相应的属性
            exp_rtr = fabs(it[index].preset_rtr);
            if (it[index].preset_rtr < 0) on_pushButton_21_clicked();//亦是，下皆同
            if(i > 3) {//第三种形式
                relics_atk = exp_atk = fabs(it[index].preset_atk);
                if (it[index].preset_atk < 0) {
                    exp_atk = 132.0;//为防止评分问题，需固定
                    on_pushButton_24_clicked();
                }
                relics_ctr = fabs(it[index].preset_ctr);
                relics_ctd = fabs(it[index].preset_ctd);
                exp_ctr = relics_ctr * 0.5 + relics_ctd * 0.25;
                if (it[index].preset_ctr < 0) {
                    exp_ctr = 74.8;//为防止评分问题，需固定
                    on_pushButton_25_clicked();
                }
                relics_ene = fabs(it[index].preset_ene);
                if (it[index].preset_ene < 0) on_pushButton_26_clicked();
                relics_cx = fabs(it[index].preset_cx);
                if (it[index].preset_cx < 0) on_pushButton_27_clicked();
                relics_add = fabs(it[index].preset_add);
                if (it[index].preset_add < 0) on_pushButton_28_clicked();
                exp_lef = fabs(it[index].preset_lef);
                if (it[index].preset_lef < 0) on_pushButton_22_clicked();
                exp_def = fabs(it[index].preset_def);
                if (it[index].preset_def < 0) on_pushButton_23_clicked();
            }
        }
        ui.comboBox_2->setCurrentIndex(it[index].first);
    }
    ui.lineEdit_12->setText(d2qstring(relics_atk, 2));
    ui.lineEdit_13->setText(d2qstring(relics_ctr, 2) + "/" + d2qstring(relics_ctd, 2));
    ui.lineEdit_14->setText(d2qstring(relics_ene, 2));
    ui.lineEdit_15->setText(d2qstring(relics_cx, 2));
    ui.lineEdit_16->setText(d2qstring(relics_add, 2));
    ui.lineEdit_6->setText(d2qstring(exp_atk, 2));
    ui.lineEdit_7->setText(d2qstring(exp_ctr, 2));
    ui.lineEdit_8->setText(d2qstring(exp_obr, 2));
    ui.lineEdit_9->setText(d2qstring(exp_rtr, 2));
    ui.lineEdit_10->setText(d2qstring(exp_lef, 2));
    ui.lineEdit_11->setText(d2qstring(exp_def, 2));
    scores_ready = 0;
}

void genshin_relics::on_checkBox_stateChanged(int state)
{
    dmg_check_state = state;
}

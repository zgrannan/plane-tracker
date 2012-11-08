/********************************************************************************
** Form generated from reading UI file 'ui.ui'
**
** Created: Wed Nov 7 21:31:52 2012
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UI_H
#define UI_UI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UI
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QSlider *verticalSlider;
    QSlider *horizontalSlider;
    QSlider *verticalSlider_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *UI)
    {
        if (UI->objectName().isEmpty())
            UI->setObjectName(QString::fromUtf8("UI"));
        UI->resize(458, 450);
        centralWidget = new QWidget(UI);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(80, 90, 121, 111));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(230, 90, 121, 111));
        verticalSlider = new QSlider(centralWidget);
        verticalSlider->setObjectName(QString::fromUtf8("verticalSlider"));
        verticalSlider->setGeometry(QRect(10, 249, 22, 111));
        verticalSlider->setValue(50);
        verticalSlider->setOrientation(Qt::Vertical);
        horizontalSlider = new QSlider(centralWidget);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(20, 360, 121, 22));
        horizontalSlider->setValue(50);
        horizontalSlider->setOrientation(Qt::Horizontal);
        verticalSlider_2 = new QSlider(centralWidget);
        verticalSlider_2->setObjectName(QString::fromUtf8("verticalSlider_2"));
        verticalSlider_2->setGeometry(QRect(400, 220, 22, 160));
        verticalSlider_2->setOrientation(Qt::Vertical);
        UI->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(UI);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 458, 22));
        UI->setMenuBar(menuBar);
        mainToolBar = new QToolBar(UI);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        UI->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(UI);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        UI->setStatusBar(statusBar);

        retranslateUi(UI);

        QMetaObject::connectSlotsByName(UI);
    } // setupUi

    void retranslateUi(QMainWindow *UI)
    {
        UI->setWindowTitle(QApplication::translate("UI", "UI", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("UI", "Disable GPS", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("UI", "Disable Video", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UI: public Ui_UI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UI_H

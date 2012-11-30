/********************************************************************************
** Form generated from reading UI file 'imageview.ui'
**
** Created: Fri Nov 30 01:15:06 2012
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEVIEW_H
#define UI_IMAGEVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageView
{
public:
    QWidget *centralwidget;
    QLabel *imageLabel;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ImageView)
    {
        if (ImageView->objectName().isEmpty())
            ImageView->setObjectName(QString::fromUtf8("ImageView"));
        ImageView->resize(720, 405);
        centralwidget = new QWidget(ImageView);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        imageLabel = new QLabel(centralwidget);
        imageLabel->setObjectName(QString::fromUtf8("imageLabel"));
        imageLabel->setGeometry(QRect(0, 0, 720, 405));
        ImageView->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ImageView);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 720, 22));
        ImageView->setMenuBar(menubar);
        statusbar = new QStatusBar(ImageView);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ImageView->setStatusBar(statusbar);

        retranslateUi(ImageView);

        QMetaObject::connectSlotsByName(ImageView);
    } // setupUi

    void retranslateUi(QMainWindow *ImageView)
    {
        ImageView->setWindowTitle(QApplication::translate("ImageView", "MainWindow", 0, QApplication::UnicodeUTF8));
        imageLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ImageView: public Ui_ImageView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEVIEW_H

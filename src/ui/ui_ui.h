/********************************************************************************
** Form generated from reading UI file 'ui.ui'
**
** Created: Sun Apr 21 22:05:31 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UI_H
#define UI_UI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UI
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *mainLayout;
    QHBoxLayout *primaryControls;
    QVBoxLayout *panTiltLayout;
    QSlider *tiltSlider;
    QLabel *panTiltLabel;
    QSlider *panSlider;
    QSpacerItem *horizontalSpacer;
    QSlider *minBlobSlider;
    QLabel *minBlobSizeLabel;
    QSlider *maxBlobSlider;
    QLabel *maxBlobSizeLabel;
    QSlider *edgeThresholdingSlider;
    QLabel *thresholdingLabel;
    QSlider *amplificationSlider;
    QLabel *amplificationLabel;
    QHBoxLayout *scoreControls;
    QPushButton *planeColorButton;
    QSlider *colorThreshSlider;
    QLabel *colorThreshLabel;
    QSlider *sizeThreshSlider;
    QLabel *sizeThreshLabel;
    QSlider *positionThreshSlider;
    QLabel *positionThreshLabel;
    QHBoxLayout *horizontalLayout;
    QPushButton *toggleVideoButton;
    QPushButton *toggleGPSButton;
    QVBoxLayout *trackerPositionSettings;
    QPushButton *useRadioPosition;
    QPushButton *useTextPosition;
    QVBoxLayout *trackerLatitudeLayout;
    QLabel *trackerLatitudeLabel;
    QPlainTextEdit *trackerLatitudeTextEdit;
    QVBoxLayout *trackerLongitudeLayout;
    QLabel *trackerLongitudeLabel;
    QPlainTextEdit *trackerLongitudeTextEdit;
    QVBoxLayout *trackerAltitudeLayout;
    QLabel *trackerAltitudeLabel;
    QPlainTextEdit *trackerAltitudeTextEdit;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *UI)
    {
        if (UI->objectName().isEmpty())
            UI->setObjectName(QString::fromUtf8("UI"));
        UI->resize(831, 430);
        centralWidget = new QWidget(UI);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        mainLayout = new QVBoxLayout();
        mainLayout->setSpacing(6);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        primaryControls = new QHBoxLayout();
        primaryControls->setSpacing(6);
        primaryControls->setObjectName(QString::fromUtf8("primaryControls"));
        panTiltLayout = new QVBoxLayout();
        panTiltLayout->setSpacing(6);
        panTiltLayout->setObjectName(QString::fromUtf8("panTiltLayout"));
        tiltSlider = new QSlider(centralWidget);
        tiltSlider->setObjectName(QString::fromUtf8("tiltSlider"));
        tiltSlider->setMinimum(-45);
        tiltSlider->setMaximum(90);
        tiltSlider->setValue(0);
        tiltSlider->setOrientation(Qt::Vertical);

        panTiltLayout->addWidget(tiltSlider);

        panTiltLabel = new QLabel(centralWidget);
        panTiltLabel->setObjectName(QString::fromUtf8("panTiltLabel"));

        panTiltLayout->addWidget(panTiltLabel);

        panSlider = new QSlider(centralWidget);
        panSlider->setObjectName(QString::fromUtf8("panSlider"));
        panSlider->setMinimum(-90);
        panSlider->setMaximum(90);
        panSlider->setValue(0);
        panSlider->setOrientation(Qt::Horizontal);

        panTiltLayout->addWidget(panSlider);


        primaryControls->addLayout(panTiltLayout);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        primaryControls->addItem(horizontalSpacer);

        minBlobSlider = new QSlider(centralWidget);
        minBlobSlider->setObjectName(QString::fromUtf8("minBlobSlider"));
        minBlobSlider->setMaximum(100);
        minBlobSlider->setValue(0);
        minBlobSlider->setOrientation(Qt::Vertical);

        primaryControls->addWidget(minBlobSlider);

        minBlobSizeLabel = new QLabel(centralWidget);
        minBlobSizeLabel->setObjectName(QString::fromUtf8("minBlobSizeLabel"));

        primaryControls->addWidget(minBlobSizeLabel);

        maxBlobSlider = new QSlider(centralWidget);
        maxBlobSlider->setObjectName(QString::fromUtf8("maxBlobSlider"));
        maxBlobSlider->setMaximum(100);
        maxBlobSlider->setValue(100);
        maxBlobSlider->setOrientation(Qt::Vertical);

        primaryControls->addWidget(maxBlobSlider);

        maxBlobSizeLabel = new QLabel(centralWidget);
        maxBlobSizeLabel->setObjectName(QString::fromUtf8("maxBlobSizeLabel"));

        primaryControls->addWidget(maxBlobSizeLabel);

        edgeThresholdingSlider = new QSlider(centralWidget);
        edgeThresholdingSlider->setObjectName(QString::fromUtf8("edgeThresholdingSlider"));
        edgeThresholdingSlider->setMinimum(10);
        edgeThresholdingSlider->setMaximum(300);
        edgeThresholdingSlider->setValue(50);
        edgeThresholdingSlider->setOrientation(Qt::Vertical);

        primaryControls->addWidget(edgeThresholdingSlider);

        thresholdingLabel = new QLabel(centralWidget);
        thresholdingLabel->setObjectName(QString::fromUtf8("thresholdingLabel"));

        primaryControls->addWidget(thresholdingLabel);

        amplificationSlider = new QSlider(centralWidget);
        amplificationSlider->setObjectName(QString::fromUtf8("amplificationSlider"));
        amplificationSlider->setMinimum(-10);
        amplificationSlider->setMaximum(10);
        amplificationSlider->setSingleStep(2);
        amplificationSlider->setOrientation(Qt::Vertical);

        primaryControls->addWidget(amplificationSlider);

        amplificationLabel = new QLabel(centralWidget);
        amplificationLabel->setObjectName(QString::fromUtf8("amplificationLabel"));

        primaryControls->addWidget(amplificationLabel);


        mainLayout->addLayout(primaryControls);

        scoreControls = new QHBoxLayout();
        scoreControls->setSpacing(6);
        scoreControls->setObjectName(QString::fromUtf8("scoreControls"));
        scoreControls->setSizeConstraint(QLayout::SetMinimumSize);
        scoreControls->setContentsMargins(-1, 0, -1, -1);
        planeColorButton = new QPushButton(centralWidget);
        planeColorButton->setObjectName(QString::fromUtf8("planeColorButton"));
        planeColorButton->setCheckable(true);

        scoreControls->addWidget(planeColorButton);

        colorThreshSlider = new QSlider(centralWidget);
        colorThreshSlider->setObjectName(QString::fromUtf8("colorThreshSlider"));
        colorThreshSlider->setMaximum(100);
        colorThreshSlider->setValue(50);
        colorThreshSlider->setOrientation(Qt::Vertical);

        scoreControls->addWidget(colorThreshSlider);

        colorThreshLabel = new QLabel(centralWidget);
        colorThreshLabel->setObjectName(QString::fromUtf8("colorThreshLabel"));

        scoreControls->addWidget(colorThreshLabel);

        sizeThreshSlider = new QSlider(centralWidget);
        sizeThreshSlider->setObjectName(QString::fromUtf8("sizeThreshSlider"));
        sizeThreshSlider->setMaximum(100);
        sizeThreshSlider->setValue(50);
        sizeThreshSlider->setOrientation(Qt::Vertical);

        scoreControls->addWidget(sizeThreshSlider);

        sizeThreshLabel = new QLabel(centralWidget);
        sizeThreshLabel->setObjectName(QString::fromUtf8("sizeThreshLabel"));

        scoreControls->addWidget(sizeThreshLabel);

        positionThreshSlider = new QSlider(centralWidget);
        positionThreshSlider->setObjectName(QString::fromUtf8("positionThreshSlider"));
        positionThreshSlider->setValue(50);
        positionThreshSlider->setOrientation(Qt::Vertical);

        scoreControls->addWidget(positionThreshSlider);

        positionThreshLabel = new QLabel(centralWidget);
        positionThreshLabel->setObjectName(QString::fromUtf8("positionThreshLabel"));

        scoreControls->addWidget(positionThreshLabel);


        mainLayout->addLayout(scoreControls);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMinimumSize);
        toggleVideoButton = new QPushButton(centralWidget);
        toggleVideoButton->setObjectName(QString::fromUtf8("toggleVideoButton"));
        toggleVideoButton->setMinimumSize(QSize(0, 100));
        toggleVideoButton->setMaximumSize(QSize(115, 16777215));
        toggleVideoButton->setCheckable(true);
        toggleVideoButton->setChecked(true);

        horizontalLayout->addWidget(toggleVideoButton);

        toggleGPSButton = new QPushButton(centralWidget);
        toggleGPSButton->setObjectName(QString::fromUtf8("toggleGPSButton"));
        toggleGPSButton->setMinimumSize(QSize(0, 100));
        toggleGPSButton->setMaximumSize(QSize(100, 16777215));
        toggleGPSButton->setCheckable(true);
        toggleGPSButton->setChecked(true);

        horizontalLayout->addWidget(toggleGPSButton);

        trackerPositionSettings = new QVBoxLayout();
        trackerPositionSettings->setSpacing(6);
        trackerPositionSettings->setObjectName(QString::fromUtf8("trackerPositionSettings"));
        trackerPositionSettings->setSizeConstraint(QLayout::SetMaximumSize);
        trackerPositionSettings->setContentsMargins(0, -1, 0, -1);
        useRadioPosition = new QPushButton(centralWidget);
        useRadioPosition->setObjectName(QString::fromUtf8("useRadioPosition"));
        useRadioPosition->setMinimumSize(QSize(200, 50));

        trackerPositionSettings->addWidget(useRadioPosition);

        useTextPosition = new QPushButton(centralWidget);
        useTextPosition->setObjectName(QString::fromUtf8("useTextPosition"));
        useTextPosition->setMinimumSize(QSize(0, 50));

        trackerPositionSettings->addWidget(useTextPosition);

        trackerPositionSettings->setStretch(0, 5);
        trackerPositionSettings->setStretch(1, 5);

        horizontalLayout->addLayout(trackerPositionSettings);

        trackerLatitudeLayout = new QVBoxLayout();
        trackerLatitudeLayout->setSpacing(6);
        trackerLatitudeLayout->setObjectName(QString::fromUtf8("trackerLatitudeLayout"));
        trackerLatitudeLabel = new QLabel(centralWidget);
        trackerLatitudeLabel->setObjectName(QString::fromUtf8("trackerLatitudeLabel"));

        trackerLatitudeLayout->addWidget(trackerLatitudeLabel);

        trackerLatitudeTextEdit = new QPlainTextEdit(centralWidget);
        trackerLatitudeTextEdit->setObjectName(QString::fromUtf8("trackerLatitudeTextEdit"));
        trackerLatitudeTextEdit->setMaximumSize(QSize(16777215, 30));
        trackerLatitudeTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        trackerLatitudeTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        trackerLatitudeLayout->addWidget(trackerLatitudeTextEdit);


        horizontalLayout->addLayout(trackerLatitudeLayout);

        trackerLongitudeLayout = new QVBoxLayout();
        trackerLongitudeLayout->setSpacing(6);
        trackerLongitudeLayout->setObjectName(QString::fromUtf8("trackerLongitudeLayout"));
        trackerLongitudeLabel = new QLabel(centralWidget);
        trackerLongitudeLabel->setObjectName(QString::fromUtf8("trackerLongitudeLabel"));

        trackerLongitudeLayout->addWidget(trackerLongitudeLabel);

        trackerLongitudeTextEdit = new QPlainTextEdit(centralWidget);
        trackerLongitudeTextEdit->setObjectName(QString::fromUtf8("trackerLongitudeTextEdit"));
        trackerLongitudeTextEdit->setMaximumSize(QSize(16777215, 30));
        trackerLongitudeTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        trackerLongitudeLayout->addWidget(trackerLongitudeTextEdit);


        horizontalLayout->addLayout(trackerLongitudeLayout);

        trackerAltitudeLayout = new QVBoxLayout();
        trackerAltitudeLayout->setSpacing(6);
        trackerAltitudeLayout->setObjectName(QString::fromUtf8("trackerAltitudeLayout"));
        trackerAltitudeLabel = new QLabel(centralWidget);
        trackerAltitudeLabel->setObjectName(QString::fromUtf8("trackerAltitudeLabel"));

        trackerAltitudeLayout->addWidget(trackerAltitudeLabel);

        trackerAltitudeTextEdit = new QPlainTextEdit(centralWidget);
        trackerAltitudeTextEdit->setObjectName(QString::fromUtf8("trackerAltitudeTextEdit"));
        trackerAltitudeTextEdit->setMaximumSize(QSize(16777215, 30));
        trackerAltitudeTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        trackerAltitudeLayout->addWidget(trackerAltitudeTextEdit);


        horizontalLayout->addLayout(trackerAltitudeLayout);


        mainLayout->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(mainLayout);

        UI->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(UI);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 831, 22));
        UI->setMenuBar(menuBar);
        statusBar = new QStatusBar(UI);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        UI->setStatusBar(statusBar);

        retranslateUi(UI);

        QMetaObject::connectSlotsByName(UI);
    } // setupUi

    void retranslateUi(QMainWindow *UI)
    {
        UI->setWindowTitle(QApplication::translate("UI", "UI", 0, QApplication::UnicodeUTF8));
        panTiltLabel->setText(QApplication::translate("UI", "Pan / Tilt", 0, QApplication::UnicodeUTF8));
        minBlobSizeLabel->setText(QApplication::translate("UI", "Minimum Blob Size", 0, QApplication::UnicodeUTF8));
        maxBlobSizeLabel->setText(QApplication::translate("UI", "Maximum Blob Size", 0, QApplication::UnicodeUTF8));
        thresholdingLabel->setText(QApplication::translate("UI", "Edge Thresholding", 0, QApplication::UnicodeUTF8));
        amplificationLabel->setText(QApplication::translate("UI", "Tracker Amplification", 0, QApplication::UnicodeUTF8));
        planeColorButton->setText(QApplication::translate("UI", "Use Current Plane Color", 0, QApplication::UnicodeUTF8));
        colorThreshLabel->setText(QApplication::translate("UI", "Color Threshhold", 0, QApplication::UnicodeUTF8));
        sizeThreshLabel->setText(QApplication::translate("UI", "Size Threshhold", 0, QApplication::UnicodeUTF8));
        positionThreshLabel->setText(QApplication::translate("UI", "Position Threshhold", 0, QApplication::UnicodeUTF8));
        toggleVideoButton->setText(QApplication::translate("UI", "Disable Video", 0, QApplication::UnicodeUTF8));
        toggleGPSButton->setText(QApplication::translate("UI", "Disable GPS", 0, QApplication::UnicodeUTF8));
        useRadioPosition->setText(QApplication::translate("UI", "Use Radio to Set Position", 0, QApplication::UnicodeUTF8));
        useTextPosition->setText(QApplication::translate("UI", "Set Position from Text Fields", 0, QApplication::UnicodeUTF8));
        trackerLatitudeLabel->setText(QApplication::translate("UI", "Tracker Latitude", 0, QApplication::UnicodeUTF8));
        trackerLongitudeLabel->setText(QApplication::translate("UI", "Tracker Longitude", 0, QApplication::UnicodeUTF8));
        trackerAltitudeLabel->setText(QApplication::translate("UI", "Tracker Altitude", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UI: public Ui_UI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UI_H

/********************************************************************************
** Form generated from reading ui file 'kaleidoscope.ui'
**
** Created: Wed Sep 2 00:17:14 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_KALEIDOSCOPE_H
#define UI_KALEIDOSCOPE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Kaleidoscope
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Kaleidoscope)
    {
        if (Kaleidoscope->objectName().isEmpty())
            Kaleidoscope->setObjectName(QString::fromUtf8("Kaleidoscope"));
        Kaleidoscope->resize(600, 400);
        menuBar = new QMenuBar(Kaleidoscope);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        Kaleidoscope->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Kaleidoscope);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        Kaleidoscope->addToolBar(mainToolBar);
        centralWidget = new QWidget(Kaleidoscope);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        Kaleidoscope->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(Kaleidoscope);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Kaleidoscope->setStatusBar(statusBar);

        retranslateUi(Kaleidoscope);

        QMetaObject::connectSlotsByName(Kaleidoscope);
    } // setupUi

    void retranslateUi(QMainWindow *Kaleidoscope)
    {
        Kaleidoscope->setWindowTitle(QApplication::translate("Kaleidoscope", "Kaleidoscope", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Kaleidoscope);
    } // retranslateUi

};

namespace Ui {
    class Kaleidoscope: public Ui_Kaleidoscope {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KALEIDOSCOPE_H

/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QTextBrowser *textBrowser;
    QVBoxLayout *verticalLayout;
    QCheckBox *_isFile;
    QPushButton *_create;
    QPushButton *_delete;
    QPushButton *_clear;
    QPushButton *_shuffle;
    QPushButton *sort;
    QMenuBar *menubar;
    QMenu *menuTimSort;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(846, 289);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(10, 10, 821, 210));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        textBrowser = new QTextBrowser(horizontalLayoutWidget);
        textBrowser->setObjectName("textBrowser");

        horizontalLayout->addWidget(textBrowser);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        _isFile = new QCheckBox(horizontalLayoutWidget);
        _isFile->setObjectName("_isFile");

        verticalLayout->addWidget(_isFile);

        _create = new QPushButton(horizontalLayoutWidget);
        _create->setObjectName("_create");

        verticalLayout->addWidget(_create);

        _delete = new QPushButton(horizontalLayoutWidget);
        _delete->setObjectName("_delete");

        verticalLayout->addWidget(_delete);

        _clear = new QPushButton(horizontalLayoutWidget);
        _clear->setObjectName("_clear");

        verticalLayout->addWidget(_clear);

        _shuffle = new QPushButton(horizontalLayoutWidget);
        _shuffle->setObjectName("_shuffle");

        verticalLayout->addWidget(_shuffle);

        sort = new QPushButton(horizontalLayoutWidget);
        sort->setObjectName("sort");

        verticalLayout->addWidget(sort);


        horizontalLayout->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 846, 25));
        menuTimSort = new QMenu(menubar);
        menuTimSort->setObjectName("menuTimSort");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuTimSort->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        _isFile->setText(QCoreApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273", nullptr));
        _create->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214", nullptr));
        _delete->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
        _clear->setText(QCoreApplication::translate("MainWindow", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214", nullptr));
        _shuffle->setText(QCoreApplication::translate("MainWindow", "\320\237\320\265\321\200\320\265\320\274\320\265\321\210\320\260\321\202\321\214", nullptr));
        sort->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\200\321\202\320\270\321\200\320\276\320\262\320\260\321\202\321\214", nullptr));
        menuTimSort->setTitle(QCoreApplication::translate("MainWindow", "TimSort", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

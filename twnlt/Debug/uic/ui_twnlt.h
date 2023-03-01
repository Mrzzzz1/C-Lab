/********************************************************************************
** Form generated from reading UI file 'twnlt.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TWNLT_H
#define UI_TWNLT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_twnltClass
{
public:
    QAction *action_open;
    QAction *action_exit;
    QAction *action_zszc;
    QAction *action_zdjl;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QGraphicsView *graphicsView;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_Q;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *twnltClass)
    {
        if (twnltClass->objectName().isEmpty())
            twnltClass->setObjectName(QString::fromUtf8("twnltClass"));
        twnltClass->resize(1045, 778);
        twnltClass->setMaximumSize(QSize(1045, 790));
        action_open = new QAction(twnltClass);
        action_open->setObjectName(QString::fromUtf8("action_open"));
        action_open->setCheckable(true);
        action_exit = new QAction(twnltClass);
        action_exit->setObjectName(QString::fromUtf8("action_exit"));
        action_exit->setCheckable(true);
        action_zszc = new QAction(twnltClass);
        action_zszc->setObjectName(QString::fromUtf8("action_zszc"));
        action_zszc->setCheckable(true);
        action_zdjl = new QAction(twnltClass);
        action_zdjl->setObjectName(QString::fromUtf8("action_zdjl"));
        action_zdjl->setCheckable(true);
        centralWidget = new QWidget(twnltClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setMouseTracking(true);
        graphicsView->setAutoFillBackground(false);
        graphicsView->setStyleSheet(QString::fromUtf8("background-image: url(:/twnlt/\345\215\216\344\270\255\347\247\221\346\212\200\345\244\247\345\255\246\345\221\250\350\276\271\345\234\260\345\233\276.bmp);"));

        verticalLayout->addWidget(graphicsView);

        twnltClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(twnltClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1045, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_Q = new QMenu(menuBar);
        menu_Q->setObjectName(QString::fromUtf8("menu_Q"));
        twnltClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(twnltClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        twnltClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(twnltClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        twnltClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_Q->menuAction());
        menu->addAction(action_open);
        menu->addAction(action_exit);
        menu_Q->addAction(action_zszc);
        menu_Q->addAction(action_zdjl);

        retranslateUi(twnltClass);
        QObject::connect(action_open, SIGNAL(triggered()), twnltClass, SLOT(loadmap()));
        QObject::connect(action_exit, SIGNAL(triggered()), twnltClass, SLOT(closewnd()));
        QObject::connect(action_zszc, SIGNAL(triggered()), twnltClass, SLOT(zszc()));
        QObject::connect(action_zdjl, SIGNAL(triggered()), twnltClass, SLOT(zdjl()));

        QMetaObject::connectSlotsByName(twnltClass);
    } // setupUi

    void retranslateUi(QMainWindow *twnltClass)
    {
        twnltClass->setWindowTitle(QCoreApplication::translate("twnltClass", "\345\205\254\344\272\244\350\275\254\344\271\230", nullptr));
        action_open->setText(QCoreApplication::translate("twnltClass", "\350\257\273\345\205\245\345\234\260\345\233\276", nullptr));
        action_exit->setText(QCoreApplication::translate("twnltClass", "\351\200\200\345\207\272", nullptr));
        action_zszc->setText(QCoreApplication::translate("twnltClass", "\346\234\200\345\260\221\350\275\254\344\271\230", nullptr));
        action_zdjl->setText(QCoreApplication::translate("twnltClass", "\346\234\200\347\237\255\350\267\235\347\246\273", nullptr));
        menu->setTitle(QCoreApplication::translate("twnltClass", "\346\226\207\344\273\266(F)", nullptr));
        menu_Q->setTitle(QCoreApplication::translate("twnltClass", "\346\237\245\350\257\242(Q)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class twnltClass: public Ui_twnltClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TWNLT_H

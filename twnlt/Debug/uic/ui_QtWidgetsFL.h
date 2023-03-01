/********************************************************************************
** Form generated from reading UI file 'QtWidgetsFL.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTWIDGETSFL_H
#define UI_QTWIDGETSFL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtWidgetsFLClass
{
public:
    QLabel *labelWind;
    QLabel *labelStop;
    QLabel *labelLine;
    QLabel *labelHits;
    QTextEdit *textEditStop;
    QTextEdit *textEditLine;
    QPushButton *pushButtonStop;
    QPushButton *pushButtonLine;
    QPushButton *pushButtonDone;
    QPushButton *pushButtonQuit;

    void setupUi(QWidget *QtWidgetsFLClass)
    {
        if (QtWidgetsFLClass->objectName().isEmpty())
            QtWidgetsFLClass->setObjectName(QString::fromUtf8("QtWidgetsFLClass"));
        QtWidgetsFLClass->resize(635, 301);
        labelWind = new QLabel(QtWidgetsFLClass);
        labelWind->setObjectName(QString::fromUtf8("labelWind"));
        labelWind->setGeometry(QRect(170, 40, 331, 41));
        QFont font;
        font.setFamily(QString::fromUtf8("Agency FB"));
        font.setPointSize(18);
        labelWind->setFont(font);
        labelStop = new QLabel(QtWidgetsFLClass);
        labelStop->setObjectName(QString::fromUtf8("labelStop"));
        labelStop->setGeometry(QRect(40, 100, 72, 15));
        labelLine = new QLabel(QtWidgetsFLClass);
        labelLine->setObjectName(QString::fromUtf8("labelLine"));
        labelLine->setGeometry(QRect(40, 150, 72, 15));
        labelHits = new QLabel(QtWidgetsFLClass);
        labelHits->setObjectName(QString::fromUtf8("labelHits"));
        labelHits->setGeometry(QRect(10, 280, 635, 20));
        textEditStop = new QTextEdit(QtWidgetsFLClass);
        textEditStop->setObjectName(QString::fromUtf8("textEditStop"));
        textEditStop->setGeometry(QRect(120, 90, 341, 41));
        textEditLine = new QTextEdit(QtWidgetsFLClass);
        textEditLine->setObjectName(QString::fromUtf8("textEditLine"));
        textEditLine->setGeometry(QRect(120, 140, 341, 41));
        pushButtonStop = new QPushButton(QtWidgetsFLClass);
        pushButtonStop->setObjectName(QString::fromUtf8("pushButtonStop"));
        pushButtonStop->setGeometry(QRect(480, 100, 93, 28));
        pushButtonLine = new QPushButton(QtWidgetsFLClass);
        pushButtonLine->setObjectName(QString::fromUtf8("pushButtonLine"));
        pushButtonLine->setGeometry(QRect(480, 150, 93, 28));
        pushButtonDone = new QPushButton(QtWidgetsFLClass);
        pushButtonDone->setObjectName(QString::fromUtf8("pushButtonDone"));
        pushButtonDone->setGeometry(QRect(160, 200, 93, 28));
        pushButtonQuit = new QPushButton(QtWidgetsFLClass);
        pushButtonQuit->setObjectName(QString::fromUtf8("pushButtonQuit"));
        pushButtonQuit->setGeometry(QRect(330, 200, 93, 28));

        retranslateUi(QtWidgetsFLClass);
        QObject::connect(pushButtonLine, SIGNAL(clicked()), QtWidgetsFLClass, SLOT(inputLine()));
        QObject::connect(pushButtonDone, SIGNAL(clicked()), QtWidgetsFLClass, SLOT(checkFile()));
        QObject::connect(pushButtonQuit, SIGNAL(clicked()), QtWidgetsFLClass, SLOT(close()));
        QObject::connect(pushButtonStop, SIGNAL(clicked()), QtWidgetsFLClass, SLOT(inputStop()));

        QMetaObject::connectSlotsByName(QtWidgetsFLClass);
    } // setupUi

    void retranslateUi(QWidget *QtWidgetsFLClass)
    {
        QtWidgetsFLClass->setWindowTitle(QCoreApplication::translate("QtWidgetsFLClass", "QtWidgetsFL", nullptr));
        labelWind->setText(QCoreApplication::translate("QtWidgetsFLClass", "\347\253\231\347\202\271\344\277\241\346\201\257\345\217\212\346\226\207\344\273\266\345\212\240\350\275\275", nullptr));
        labelStop->setText(QCoreApplication::translate("QtWidgetsFLClass", "\347\253\231\347\202\271\346\226\207\344\273\266", nullptr));
        labelLine->setText(QCoreApplication::translate("QtWidgetsFLClass", "\347\272\277\350\267\257\346\226\207\344\273\266", nullptr));
        labelHits->setText(QString());
        pushButtonStop->setText(QCoreApplication::translate("QtWidgetsFLClass", "\346\265\217\350\247\210...", nullptr));
        pushButtonLine->setText(QCoreApplication::translate("QtWidgetsFLClass", "\346\265\217\350\247\210...", nullptr));
        pushButtonDone->setText(QCoreApplication::translate("QtWidgetsFLClass", "\347\241\256\345\256\232", nullptr));
        pushButtonQuit->setText(QCoreApplication::translate("QtWidgetsFLClass", "\351\200\200\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtWidgetsFLClass: public Ui_QtWidgetsFLClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTWIDGETSFL_H

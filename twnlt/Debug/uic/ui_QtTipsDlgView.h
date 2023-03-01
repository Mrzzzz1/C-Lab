/********************************************************************************
** Form generated from reading UI file 'QtTipsDlgView.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTTIPSDLGVIEW_H
#define UI_QTTIPSDLGVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_QtTipsDlgViewClass
{
public:
    QLabel *labelMsg;

    void setupUi(QDialog *QtTipsDlgViewClass)
    {
        if (QtTipsDlgViewClass->objectName().isEmpty())
            QtTipsDlgViewClass->setObjectName(QString::fromUtf8("QtTipsDlgViewClass"));
        QtTipsDlgViewClass->resize(312, 21);
        labelMsg = new QLabel(QtTipsDlgViewClass);
        labelMsg->setObjectName(QString::fromUtf8("labelMsg"));
        labelMsg->setGeometry(QRect(0, 0, 310, 20));

        retranslateUi(QtTipsDlgViewClass);

        QMetaObject::connectSlotsByName(QtTipsDlgViewClass);
    } // setupUi

    void retranslateUi(QDialog *QtTipsDlgViewClass)
    {
        QtTipsDlgViewClass->setWindowTitle(QCoreApplication::translate("QtTipsDlgViewClass", "QtTipsDlgView", nullptr));
        labelMsg->setText(QCoreApplication::translate("QtTipsDlgViewClass", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtTipsDlgViewClass: public Ui_QtTipsDlgViewClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTTIPSDLGVIEW_H

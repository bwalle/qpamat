/*
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation; You may only use
 * version 2 of the License, you have no option to use any other version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, write to the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * -------------------------------------------------------------------------------------------------
 */
#include "aboutdialog.h"

#include <QLabel>
#include <QDialogButtonBox>
#include <Q3HBox>
#include <Q3VBox>
#include <QFile>
#include <Q3TextEdit>
#include <QApplication>
#include <QDir>
#include <QPushButton>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextStream>

/*!
    \class AboutDialog

    \brief The "about" dialog of QPaMaT.

    \ingroup gui
    \author Bernhard Walle
*/

/*!
    Creates a new instance of a AboutDialog object.

    \param parent the parent widget
    \param name the name of the widget
*/
AboutDialog::AboutDialog(QWidget* parent, const char* name = 0)
    : QDialog(parent, name)
{
    m_mainLayout = new QVBoxLayout(this, 10, 6);
    setCaption(tr("About QPaMaT"));

    // the top of the dialog
    Q3HBox* titleBox = new Q3HBox(this);
    QLabel* titleIcon = new QLabel(titleBox);
    titleIcon->setPixmap(QPixmap(":/images/qpamat_48.png"));
    QLabel* titleText = new QLabel(tr("<p><b><big>QPaMaT %1</big></b></p>").arg(VERSION_STRING),
        titleBox);
    QWidget* filler = new QWidget(titleBox);
    titleText->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    titleBox->setSpacing(15);

    // the tab
    m_tabWidget = new QTabWidget(this);
    setupAboutTab();
    setupLicenseTab();

    // the ok Button
    QDialogButtonBox *dialogButtons = new QDialogButtonBox(QDialogButtonBox::Close,
            Qt::Horizontal, this);

    // main layout
    m_mainLayout->addWidget(titleBox);
    m_mainLayout->addWidget(m_tabWidget);
    m_mainLayout->addStretch(5);
    m_mainLayout->addWidget(dialogButtons);

    connect(dialogButtons, SIGNAL(clicked(QAbstractButton *)), SLOT(accept()));

}

/*!
    Sets up the about tab.
*/
void AboutDialog::setupAboutTab()
{
    Q3HBox* aboutTab = new Q3HBox(this);
    aboutTab->setMargin(15);

    (void)new QLabel(
        tr("<p><nobr>This is a password managing tool for Unix, Windows and "
            "MacOS X</nobr> written in C++<br> using the Qt library.</p>"
            "<p><b>Thanks to:</b>"
            "<ul><li>Trolltech for the Qt library</li>"
            "<li>OpenSSL team for the OpenSSL library</li>"
            "<li>Gtk+ artists for the nice stock icons</li>"
            "</ul></p>"
            "<p><b>Homepage:</b> <tt>http://qpamat.berlios.de</tt></p>"), aboutTab);

    m_tabWidget->addTab(aboutTab, tr("&About"));

}

/*!
    Sets up the license tab. Tries to search the \c LICENSE file.
*/
void AboutDialog::setupLicenseTab()
{
    Q3VBox* licenseTab = new Q3VBox(this);

    Q3TextEdit* textEdit = new Q3TextEdit(licenseTab);
    textEdit->setReadOnly(true);
    textEdit->setWordWrap(Q3TextEdit::FixedColumnWidth);
    textEdit->setWrapColumnOrWidth(100);

    QFile file(":/COPYING");
    if (file.open( QIODevice::ReadOnly ))
    {
        QTextStream stream(&file);
        textEdit->setText("<pre>" + stream.read() + "</pre>");
    }

    m_tabWidget->addTab(licenseTab, tr("&License"));
}

// :maxLineLen=100:

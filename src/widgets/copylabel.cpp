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
#include <QLayout>
#include <QPushButton>
#include <QPixmap>
#include <QAction>
#include <QDir>
#include <QApplication>
#include <QClipboard>
#include <QLabel>
#include <Q3Frame>
#include <QHBoxLayout>

#include "copylabel.h"


/**
 * @class CopyLabel
 *
 * @brief This class provides a QLineEdit combined with a copy button.
 *
 * The copy buttons lets the user copy the displayed text chooser dialog of Qt.
 *
 * @ingroup widgets
 * @author Bernhard Walle
 */

/**
 * @brief This constructor.
 *
 * Function is provided only for convenience. It behaves exactly like the above function,
 * \p hidden is set to false.
 */
CopyLabel::CopyLabel(QWidget* parent, const char* name)
    : QWidget(parent, name), m_hidden(false)
{
    init();
}


/**
 * @brief Creates a new instance of a FileLineEdit widget.
 *
 * @param hidden if the word should be hidden
 * @param parent the parent widget
 * @param name the name of the widget which can be NULL.
 */
CopyLabel::CopyLabel(bool hidden, QWidget* parent, const char* name)
    : QWidget(parent, name)
    , m_hidden(hidden)
{
    init();
}


/**
 * @brief Initializes the widget.
 *
 * Needed because of the two constructors.
 */
void CopyLabel::init()
{
    QHBoxLayout* boxLayout = new QHBoxLayout(this, 0, 2);
    m_label = new QLabel(this);
    m_label->setFrameStyle( Q3Frame::Panel | Q3Frame::Sunken );

    // we need a tool button because it has no border and it looks better in this size
    // in Platin style
    m_copyButton = new QToolButton(this);
    m_copyButton->setIconSet(QIcon(QPixmap(":/images/stock_copy_16.png")));

    boxLayout->addWidget(m_label);
    boxLayout->addWidget(m_copyButton);
    boxLayout->setStretchFactor(m_label, 5);
    boxLayout->setStretchFactor(m_copyButton, 0);

    connect(m_copyButton, SIGNAL(clicked()), SLOT(copyText()));

    QAction* copy = new QAction(tr("Copy"), this);
    copy->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_C));
    connect(copy, SIGNAL(activated()), SLOT(copyText()));
}


/**
 * @property CopyLabel::content
 *
 * The content of the label.
 */

/**
 * @brief Sets the content.
 *
 * @param content the content string
 */
void CopyLabel::setContent(const QString& content)
{
    if (m_hidden) {
        QString text;
        for (int i = 0; i < content.length(); ++i) {
            text += "*";
        }
        m_label->setText(text);
    } else
        m_label->setText(content);
}


/**
 * @brief This returns the current content.
 *
 * @return the content of the CopyLabel widget
 */
QString CopyLabel::getContent() const
{
    return m_label->text();
}


/**
 * @property CopyLabel::hidden
 *
 * Whether the text is displayed hidden.
 */

/**
 * @brief If the word is displayed hidden.
 *
 * @return the boolean value
 */
bool CopyLabel::isHidden() const
{
    return m_hidden;
}


/**
 * @brief Copys the text.
 */
void CopyLabel::copyText()
{
    QClipboard* cb = QApplication::clipboard();
    cb->setText(m_label->text(), QClipboard::Clipboard);
    if (cb->supportsSelection())
        cb->setText(m_label->text(), QClipboard::Selection);
}

// vim: set sw=4 ts=4 et:

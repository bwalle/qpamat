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
#ifndef COPYLABEL_H
#define COPYLABEL_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QEvent>

class CopyLabel : public QWidget
{
    Q_OBJECT
    Q_PROPERTY( QString content READ getContent WRITE setContent )
    Q_PROPERTY( bool hidden READ isHidden )

    public:
        CopyLabel(bool hidden, QWidget* parent, const char* name = 0);
        CopyLabel(QWidget* parent, const char* name);

        void setContent(const QString& content);
        QString getContent() const;

        bool isHidden() const;

    protected slots:
        void copyText();

    private:
        void init();

    private:
        QLabel*         m_label;
        QToolButton*    m_copyButton;
        const bool      m_hidden;
};

#endif // COPYLABEL_H

// vim: set sw=4 ts=4 et:

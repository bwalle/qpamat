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
#ifndef RANDOMPASSWORD_H
#define RANDOMPASSWORD_H

#include <QObject>
#include <QString>

class QpamatWindow;

class RandomPassword : public QObject
{
    Q_OBJECT

    public:
        RandomPassword(QpamatWindow* parent, const char* name = 0);

    public slots:
        void setInsertEnabled(bool enabled = true);
        void requestPassword();

    signals:
        void insertPassword(const QString& password);

    private:
        bool m_insertEnabled;
        QpamatWindow* m_parent;
};

#endif // RANDOMPASSWORD_H

// vim: set sw=4 ts=4 et: :tabSize=4:indentSize=4:maxLineLen=100:

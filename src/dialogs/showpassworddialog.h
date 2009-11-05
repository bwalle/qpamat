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
#ifndef SHOWPASSWORDDIALOG_H
#define SHOWPASSWORDDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QWidget>

#include "widgets/copylabel.h"

class ShowPasswordDialog : public QDialog
{
    Q_OBJECT

    public:
        enum DialogType {
            TRandomPasswordDlg,
            TRandomPasswordDlgInsert,
            TNormalPasswordDlg
        };

    public:
        ShowPasswordDialog(QWidget* parent, DialogType type,  const char* name = 0);

    public slots:
        QString getPassword() const;
        void setPassword(const QString& newPassword);

    signals:
        void insertPassword(const QString& password);

    private slots:
        void insertButtonHandler();

    private:
        CopyLabel* m_passwordEdit;
};

#endif // SHOWPASSWORDDIALOG_H

// vim: set sw=4 ts=4 et: :tabSize=4:indentSize=4:maxLineLen=100:

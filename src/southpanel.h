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
#ifndef SOUTHPANEL_H
#define SOUTHPANEL_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <Q3Frame>
#include <QEvent>
#include <QToolButton>
#include <QTimer>

#include "widgets/focuslineedit.h"
#include "property.h"

class SouthPanel : public Q3Frame
{
    Q_OBJECT

    public:
        SouthPanel(QWidget* parent);
        bool isFocusInside() const;

    public slots:
        void setItem (Property* property);
        void comboBoxChanged(int newChoice);
        void updateData();
        void clear();
        void setMovingEnabled(bool up, bool down);
        void insertPassword(const QString& password);

    signals:
        void moveUp();
        void moveDown();
        void passwordLineEditGotFocus(bool focus);
        void stateModified();
        void passwordStrengthUpdated();

    protected:
        void insertAutoText();

    private slots:
        void focusInValueHandler();
        void focusOutValueHandler();
        void updateIndicatorLabel(bool recompute = true);

    private:
        Property*                   m_currentProperty;
        QLineEdit*                  m_keyLineEdit;
        FocusLineEdit*              m_valueLineEdit;
        QComboBox*                  m_typeCombo;
        QToolButton*                m_upButton;
        QToolButton*                m_downButton;
        QLabel*                     m_indicatorLabel;
        Property::PasswordStrength  m_lastStrength;
        int                         m_oldComboValue;
        QTimer*                     m_updatePasswordQualityTimer;
};

#endif // SOUTHPANEL_H

// :maxLineLen=100:

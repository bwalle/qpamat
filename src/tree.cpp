/*
 * Id: $Id: tree.cpp,v 1.3 2003/10/12 15:11:52 bwalle Exp $
 * -------------------------------------------------------------------------------------------------
 * 
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
#include <qfile.h>
#include <qdom.h>
#include <qstring.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qheader.h>

#include "tree.h"
#include "treeentry.h"
#include "wrongpassword.h"
#include "cipher/passwordhash.h"
#include "cipher/encryptor.h"

// -------------------------------------------------------------------------------------------------
Tree::Tree(QWidget* parent)
// -------------------------------------------------------------------------------------------------
    : QListView(parent)
{
    addColumn("first");
    header()->setStretchEnabled(true);
    header()->hide();
    setRootIsDecorated(true);
    setShowSortIndicator(true);
}


// -------------------------------------------------------------------------------------------------
bool Tree::readFromXML(const QString& fileName, const QString& password) throw (WrongPassword)
// -------------------------------------------------------------------------------------------------
{
    // delete the old tree
    QListViewItem* item;
    while ((item = firstChild()))
    {
        delete item;
    }
    
    // load the XML structure
    QFile file(fileName);
    if (!file.open(IO_ReadOnly))
    {
        QMessageBox::critical(this, QPAMAT_USERVISIBLE, tr("The file %1 could not be opened:\n%2.").
            arg(fileName).arg(qApp->translate("QFile", file.errorString())), QMessageBox::Ok, 
            QMessageBox::NoButton);
        return false;
    }
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        showCurruptedMessage(fileName);
        return false;
    }
    file.close();
    QDomElement root = doc.documentElement();
    
    // check the password
    try
    {
        if (!PasswordHash::isCorrect(password, root.attribute("password-hash")))
        {
            throw WrongPassword(tr("The password is incorrect.").latin1());
        }
    }
    catch (const std::invalid_argument& ex)
    {
        qDebug(ex.what());
        showCurruptedMessage(fileName);
        return false;
    }
    
    Encryptor* enc;
    QString algorithm = root.attribute("crypt-algorithm");
    try
    {
        enc = new Encryptor(algorithm, password);
    }
    catch (const NoSuchAlgorithmException& ex)
    {
        QMessageBox::critical(this, QPAMAT_USERVISIBLE, tr("The algorithm '%1' is not avaible on "
            "your system.\nIt is impossible to read the file. Try to recompile or\n",
            "update your OpenSSL library.").arg(algorithm), QMessageBox::Ok, QMessageBox::NoButton);
        return false;
    }
    
    QDomNode n = root.firstChild();
    while( !n.isNull() ) 
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if( !e.isNull() ) 
        {
            TreeEntry::appendFromXML(this, e, *enc);
        }
        n = n.nextSibling();
    }
    
    delete enc;
    
    return true;
}


// -------------------------------------------------------------------------------------------------
void Tree::writeToXML(const QString& fileName, const QString& password, const QString& algorithm)
// -------------------------------------------------------------------------------------------------
{
    QFile file(fileName);
    if (!file.open(IO_WriteOnly))
    {
        QMessageBox::critical(this, QPAMAT_USERVISIBLE, tr("The data could not be saved. There "
            "was an\nerror while creating the file:\n%1").arg( qApp->translate("QFile",
            file.errorString())), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    QDomDocument doc;
    QDomElement root = doc.createElement("qpamat");
    root.setAttribute("version", VERSION);
    root.setAttribute("crypt-algorithm", algorithm);
    root.setAttribute("password-hash", PasswordHash::generateHash(password));
    doc.appendChild(root);
    
    Encryptor* enc;
    try
    {
        enc = new Encryptor(algorithm, password);
    }
    catch (const NoSuchAlgorithmException& e)
    {
        QMessageBox::critical(this, QPAMAT_USERVISIBLE, tr("The algorithm '%1' is not avaible on "
            "your system.\nChoose another crypto algorithm in the settings.\nThe data "
            "is not saved!").arg(algorithm), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    
    // we have one child that contains all children. The root child is not appended to
    // XML so we have to find the children of the child
    TreeEntry* currentItem = dynamic_cast<TreeEntry*>(firstChild());
    while (currentItem)
    {
        currentItem->appendXML(doc, root, *enc);
        currentItem = dynamic_cast<TreeEntry*>(currentItem->nextSibling());
    }
    
    QTextStream stream(&file);
    stream << doc.toString();
    file.close();
    delete enc;
}


// -------------------------------------------------------------------------------------------------
void Tree::showCurruptedMessage(const QString& fileName)
// -------------------------------------------------------------------------------------------------
{
    QMessageBox::critical(this, QPAMAT_USERVISIBLE, tr("The XML file (%1) may be corrupted "
            "and\ncould not be read. Check the file with a text editor.").arg(fileName), 
            QMessageBox::Ok, QMessageBox::NoButton);
}



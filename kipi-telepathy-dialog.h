/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.digikam.org
 *
 * Date        : 2012-01-13
 * Description : a kipi plugin to send files to a Telepathy contact
 *
 * Copyright (C) 2012 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
 *
 * Parts of this file are taken from debianscreenshots plugin that is:
 * Copyright (C) 2010 by Pau Garcia i Quiles <pgquiles at elpauer dot org>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */


#ifndef KIPI_TELEPATHY_DIALOG_H
#define KIPI_TELEPATHY_DIALOG_H

#include <KDialog>
#include <TelepathyQt/Types>

namespace KIPIPlugins {
    class KPAboutData;
}

namespace Tp {
    class PendingOperation;
}

namespace KIPITelepathy {
class Widget;

class Dialog : public KDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(Dialog)

public:
    Dialog(QWidget *parent);
    virtual ~Dialog();

    /**
     * Use this method to (re-)activate the dialog after it has been created
     * to display it. This also loads the currently selected images.
     */
    void reactivate();

private Q_SLOTS:
    void onDataChanged();
    void onAccountManagerReady(Tp::PendingOperation *op);
    void onHelpClicked();
    void onSendButtonClicked();
    void onStopAndCloseProgressBar();
    void onFileTransferRequestsFinished(Tp::PendingOperation *op);

protected:
    virtual void closeEvent(QCloseEvent *e);

private:
    KIPIPlugins::KPAboutData *m_about;
    KIPITelepathy::Widget *m_widget;
    Tp::AccountManagerPtr m_accountManager;
};

} // namespace KIPITelepathy

#endif // KIPI_TELEPATHY_DIALOG_H

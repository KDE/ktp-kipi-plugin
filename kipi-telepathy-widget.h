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


#ifndef KIPI_TELEPATHY_WIDGET_H
#define KIPI_TELEPATHY_WIDGET_H

#include <QWidget>
#include <KUrl>

#include <TelepathyQt/Types>

class AccountsModel;

namespace KIPIPlugins
{
    class KPImagesList;
    class KPProgressWidget;
}

namespace KTp
{
    class ContactGridWidget;
}

namespace KIPITelepathy
{

class Widget : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(Widget)
public:
    Widget(Tp::AccountManagerPtr accountManager, QWidget *parent = 0);
    virtual ~Widget();

    void reload();
    void clear();
    void clearProcessedStatus();
    bool isTransferAllowed() const;
    KUrl::List imageUrls(bool onlyUnprocessed = false) const;
    Tp::ContactPtr selectedContact() const;
    Tp::AccountPtr selectedAccount() const;
    void processed(const KUrl& url, bool success);
    KIPIPlugins::KPProgressWidget* progressBar() const;
    KIPIPlugins::KPImagesList* imagesList() const;

Q_SIGNALS:
    void dataChanged();

private Q_SLOTS:
    void onAccountManagerReady();

private:
    KIPIPlugins::KPImagesList *m_imgList;
    KTp::ContactGridWidget *m_contactGridWidget;
    KIPIPlugins::KPProgressWidget *m_progressBar;
    Tp::AccountManagerPtr m_accountManager;
    AccountsModel *m_accountsModel;
};

} // namespace KIPITelepathy

#endif // KIPI_TELEPATHY_WIDGET_H

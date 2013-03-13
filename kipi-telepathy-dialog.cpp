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


#include "kipi-telepathy-dialog.h"
#include "kipi-telepathy-widget.h"
#include "version.h"

#include <kpaboutdata.h>
#include <kpprogresswidget.h>
#include <kpimageslist.h>

#include <KAboutData>
#include <KHelpMenu>
#include <KMenu>
#include <KPushButton>
#include <KToolInvocation>
#include <KDebug>
#include <KMimeType>

#include <QCloseEvent>

#include <TelepathyQt/Types>
#include <TelepathyQt/AccountManager>
#include <TelepathyQt/Contact>
#include <TelepathyQt/Account>
#include <TelepathyQt/PendingChannelRequest>
#include <TelepathyQt/PendingOperation>
#include <TelepathyQt/PendingReady>
#include <TelepathyQt/PendingComposite>
#include <TelepathyQt/FileTransferChannelCreationProperties>

#include <KTp/actions.h>
#include <KTp/contact-factory.h>

KIPITelepathy::Dialog::Dialog(QWidget* parent)
    : KDialog (parent),
      m_about(0),
      m_widget(0)
{
    KIPIPlugins::KPAboutData *about = new KIPIPlugins::KPAboutData(ki18n("Send to Telepathy Contact"),
                                                                   KTP_KIPI_PLUGIN_VERSION,
                                                                   KAboutData::License_GPL,
                                                                   ki18n("A Kipi plugin to send an image collection "
                                                                         "to a KDE IM contact."),
                                                                   ki18n("(c) 2012, Daniele E. Domenichelli\n"));

    about->addAuthor(ki18n("Daniele E. Domenichelli"),
                     ki18n("Author and maintainer"),
                     "daniele.domenichelli@gmail.com",
                     "http://blogs.fsfe.org/drdanz");

    setWindowIcon(KIcon(QLatin1String("telepathy-kde")));
    setButtons(Help|User1|Close);
    setDefaultButton(Close);
    setModal(false);

    setWindowTitle(i18n("Send to KDE IM Contact"));
    setButtonGuiItem(User1, KGuiItem(i18n("Send"),
                                     QLatin1String("telepathy-kde"),
                                     i18n("Start file transfer to a KDE IM contact")));
    enableButton(User1, false); // Disable until package and version data have been fulfilled

    disconnect(this, SIGNAL(helpClicked()),
               this, SLOT(onHelpClicked()));

    KHelpMenu* helpMenu = new KHelpMenu(this, about, false);
    helpMenu->menu()->removeAction(helpMenu->menu()->actions().first());
    QAction *handbook   = new QAction(i18n("Handbook"), this);

    connect(handbook, SIGNAL(triggered(bool)),
            this,     SLOT(onHelpClicked()));

    helpMenu->menu()->insertAction(helpMenu->menu()->actions().first(), handbook);
    button(Help)->setMenu(helpMenu->menu());

    // Telepathy stuff
    Tp::registerTypes();

    Tp::AccountFactoryPtr  accountFactory = Tp::AccountFactory::create(QDBusConnection::sessionBus(),
                                                                       Tp::Features() << Tp::Account::FeatureCore
                                                                       << Tp::Account::FeatureAvatar
                                                                       << Tp::Account::FeatureProtocolInfo
                                                                       << Tp::Account::FeatureProfile
                                                                       << Tp::Account::FeatureCapabilities);

    Tp::ConnectionFactoryPtr connectionFactory = Tp::ConnectionFactory::create(QDBusConnection::sessionBus(),
                                                                               Tp::Features() << Tp::Connection::FeatureCore
                                                                               << Tp::Connection::FeatureRosterGroups
                                                                               << Tp::Connection::FeatureRoster
                                                                               << Tp::Connection::FeatureSelfContact);

    Tp::ContactFactoryPtr contactFactory = KTp::ContactFactory::create(Tp::Features()  << Tp::Contact::FeatureAlias
                                                                       << Tp::Contact::FeatureAvatarData
                                                                       << Tp::Contact::FeatureSimplePresence
                                                                       << Tp::Contact::FeatureCapabilities);

    Tp::ChannelFactoryPtr channelFactory = Tp::ChannelFactory::create(QDBusConnection::sessionBus());

    m_accountManager = Tp::AccountManager::create(QDBusConnection::sessionBus(),
                                                                      accountFactory,
                                                                      connectionFactory,
                                                                      channelFactory,
                                                                      contactFactory);

    connect(m_accountManager->becomeReady(),
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onAccountManagerReady(Tp::PendingOperation*)));
}

KIPITelepathy::Dialog::~Dialog()
{
}


void KIPITelepathy::Dialog::onAccountManagerReady(Tp::PendingOperation* op)
{
    if (op->isError()) {
        kWarning() << "Cannot get account manager ready";
        // TODO Warning dialog
        hide();
    }

    m_widget = new KIPITelepathy::Widget(m_accountManager, this);
    setMainWidget(m_widget);
    m_widget->setMinimumSize(750, 500);

    connect(m_widget, SIGNAL(dataChanged()),
            this,     SLOT(onDataChanged()));

    connect(this,
            SIGNAL(user1Clicked()),
            SLOT(onSendButtonClicked()));

    connect(m_widget->progressBar(),
            SIGNAL(signalProgressCanceled()),
            SLOT(onStopAndCloseProgressBar()));

    show();
}

void KIPITelepathy::Dialog::onDataChanged()
{
    enableButton(User1, m_widget->isTransferAllowed());
}

void KIPITelepathy::Dialog::onHelpClicked()
{
    KToolInvocation::invokeHelp(QLatin1String("telepathy"), QLatin1String("kipi-plugins"));
}

void KIPITelepathy::Dialog::onStopAndCloseProgressBar()
{
    m_widget->imagesList()->cancelProcess();
    m_widget->imagesList()->listView()->clear();
    m_widget->progressBar()->progressCompleted();
    done(Close);
}

void KIPITelepathy::Dialog::onSendButtonClicked()
{
    // don't do anytghing if no contact or no files have been selected
    if (!m_widget->isTransferAllowed()) {
        // show message box?
        return;
    }

    m_widget->clearProcessedStatus();
    m_widget->progressBar()->setMaximum(m_widget->imageUrls().count());
    m_widget->progressBar()->setValue(0);
    m_widget->progressBar()->show();
    m_widget->progressBar()->progressScheduled(i18n("KDE IM Send"), true, true);
    m_widget->progressBar()->progressThumbnailChanged(KIcon("telepathy-kde").pixmap(22, 22));

    Tp::ContactPtr contact = m_widget->selectedContact();
    Tp::AccountPtr sendingAccount = m_widget->selectedAccount();
    QDateTime now = QDateTime::currentDateTime();

    if (sendingAccount.isNull()) {
        kDebug() << "sending account: NULL";
        return;
    } else {
        kDebug() << "Account is: " << sendingAccount->displayName();
        kDebug() << "sending to: " << contact->alias();
    }

    QList<Tp::PendingOperation*> channelRequests;
    Q_FOREACH(const KUrl &url, m_widget->imageUrls()) {
        kDebug() << url;
        m_widget->progressBar()->setValue(channelRequests.count());

        channelRequests << KTp::Actions::startFileTransfer(sendingAccount,contact, url.toLocalFile());

        m_widget->processed(url, true);
    }

    Tp::PendingComposite *pendingOperations = new Tp::PendingComposite(channelRequests, sendingAccount);
    connect(pendingOperations,
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onFileTransferRequestsFinished(Tp::PendingOperation*)));
}

void KIPITelepathy::Dialog::onFileTransferRequestsFinished(Tp::PendingOperation* op)
{
    if (op->isError()) {
        //FIXME
    }

    m_widget->progressBar()->hide();
}

void KIPITelepathy::Dialog::closeEvent(QCloseEvent* e)
{
    if (!e) {
        return;
    }

    if (m_widget) {
        m_widget->clear();
    }

    e->accept();
}

void KIPITelepathy::Dialog::reactivate()
{
    if (m_widget) {
        m_widget->reload();
    }
}

#include "kipi-telepathy-dialog.moc"

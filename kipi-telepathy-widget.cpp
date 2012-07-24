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


#include "kipi-telepathy-widget.h"

#include <kpimageslist.h>
#include <kpprogresswidget.h>

#include <KDE/KDialog>
#include <KDE/KLocale>
#include <KDE/KLineEdit>
#include <KDE/KDebug>

#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QListView>

#include <TelepathyQt/Types>
#include <TelepathyQt/AccountManager>

#include <KTp/Models/accounts-model.h>
#include <KTp/Models/accounts-filter-model.h>
#include <KTp/Models/contact-model-item.h>
#include <KTp/Widgets/contact-grid-widget.h>


KIPITelepathy::Widget::Widget(Tp::AccountManagerPtr accountManager, QWidget* parent)
    : QWidget(parent)
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    m_imgList  = new KIPIPlugins::KPImagesList(this);
    m_imgList->setControlButtonsPlacement(KIPIPlugins::KPImagesList::ControlButtonsBelow);
    m_imgList->setAllowRAW(true);
    m_imgList->loadImagesFromCurrentSelection();
    m_imgList->listView()->setWhatsThis( i18n("This is the list of images to send to your contact.") );

    QWidget* settingsBox = new QWidget(this);
    QVBoxLayout* settingsBoxLayout = new QVBoxLayout(settingsBox);

    QGroupBox* contactGroupBox   = new QGroupBox(settingsBox);
    contactGroupBox->setTitle(i18n("Recipient"));
    contactGroupBox->setWhatsThis(i18n("This is the contact you will send the images."));

    QVBoxLayout* contactLayout = new QVBoxLayout(contactGroupBox);
    contactGroupBox->setLayout(contactLayout);


    m_accountsModel = new AccountsModel(this);
    m_accountsModel->setAccountManager(accountManager);

    m_contactGridWidget = new KTp::ContactGridWidget(m_accountsModel, contactGroupBox);
    m_contactGridWidget->filter()->setCapabilityFilterFlags(AccountsFilterModel::FilterByFileTransferCapability);
    m_contactGridWidget->filter()->setPresenceTypeFilterFlags(AccountsFilterModel::ShowOnlyConnected);

    contactLayout->addWidget(m_contactGridWidget);

    m_progressBar = new KIPIPlugins::KPProgressWidget(settingsBox);
    m_progressBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_progressBar->setFormat(i18n("%v / %m"));
    m_progressBar->setValue(0);
    m_progressBar->hide();

    settingsBoxLayout->addWidget(contactGroupBox);
    settingsBoxLayout->addWidget(m_progressBar);

    mainLayout->addWidget(m_imgList, 1);
    mainLayout->addWidget(settingsBox, 2);
    mainLayout->setSpacing(KDialog::spacingHint());
    mainLayout->setMargin(0);

    connect(m_imgList,
            SIGNAL(signalImageListChanged()),
            SIGNAL(dataChanged()));

    connect(m_contactGridWidget,
            SIGNAL(selectionChanged(Tp::AccountPtr,Tp::ContactPtr)),
            SIGNAL(dataChanged()));
}

KIPITelepathy::Widget::~Widget()
{
}

void KIPITelepathy::Widget::reload()
{
    m_imgList->loadImagesFromCurrentSelection();
}

void KIPITelepathy::Widget::clear()
{
    m_imgList->listView()->clear();
}

void KIPITelepathy::Widget::clearProcessedStatus()
{
    m_imgList->clearProcessedStatus();
}

KUrl::List KIPITelepathy::Widget::imageUrls(bool onlyUnprocessed) const
{
    return m_imgList->imageUrls(onlyUnprocessed);
}

Tp::ContactPtr KIPITelepathy::Widget::selectedContact() const
{
    return m_contactGridWidget->selectedContact();
}

Tp::AccountPtr KIPITelepathy::Widget::selectedAccount() const
{
    return m_contactGridWidget->selectedAccount();
}

void KIPITelepathy::Widget::processed(const KUrl& url, bool success)
{
    m_imgList->processed(url, success);
}

KIPIPlugins::KPProgressWidget* KIPITelepathy::Widget::progressBar() const
{
    return m_progressBar;
}

KIPIPlugins::KPImagesList* KIPITelepathy::Widget::imagesList() const
{
    return m_imgList;
}

void KIPITelepathy::Widget::onAccountManagerReady()
{
    m_accountsModel->setAccountManager(m_accountManager);
    dataChanged();
}

bool KIPITelepathy::Widget::isTransferAllowed() const
{
    kDebug() << !m_imgList->imageUrls().isEmpty() << m_contactGridWidget->hasSelection();
    return (!m_imgList->imageUrls().isEmpty() && m_contactGridWidget->hasSelection());
}

#include "kipi-telepathy-widget.moc"

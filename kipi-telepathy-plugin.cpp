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


#include "kipi-telepathy-plugin.h"
#include "kipi-telepathy-dialog.h"

#include <QtCore/QTimer>

#include <KDE/KDebug>
#include <KDE/KIcon>
#include <KDE/KAction>
#include <KDE/KActionCollection>
#include <KDE/KShortcut>
#include <KDE/KGenericFactory>
#include <KDE/KStandardDirs>
#include <KDE/KWindowSystem>
#include <KDE/KApplication>

#include <libkipi/interface.h>

#include <unistd.h>


K_PLUGIN_FACTORY( KTPFactory, registerPlugin<KIPITelepathy::Plugin>(); )
K_EXPORT_PLUGIN ( KTPFactory("kipiplugin_telepathy") )


KIPITelepathy::Plugin::Plugin(QObject* parent, const QVariantList& args)
    : KIPI::Plugin(KTPFactory::componentData(),
                   parent, "Send to IM contact")
{
    Q_UNUSED(args)
    kDebug(AREA_CODE_LOADING) << "Plugin_Telepathy plugin loaded";
}

KIPITelepathy::Plugin::~Plugin()
{
}


void KIPITelepathy::Plugin::setup(QWidget* widget)
{
    m_dialogSend = 0;
    KIPI::Plugin::setup(widget);

    m_action = actionCollection()->addAction(QLatin1String("send"));
    m_action->setText(i18n("Send to IM &Contact..."));
    m_action->setIcon(KIcon(QLatin1String("telepathy-kde")));
    m_action->setShortcut(KShortcut(Qt::ALT+Qt::SHIFT+Qt::Key_T));

    connect(m_action,
            SIGNAL(triggered(bool)),
            this,
            SLOT(onActionTriggered()));

    addAction(m_action);

    KIPI::Interface* interface = dynamic_cast<KIPI::Interface*>(parent());
    if (!interface) {
        kError() << "Kipi interface is null!";
        m_action->setEnabled(false);
        return;
    }

    m_action->setEnabled(true);
}

void KIPITelepathy::Plugin::onActionTriggered()
{
    KIPI::Interface* interface = dynamic_cast<KIPI::Interface*>(parent());
    if (!interface) {
        kError() << "Kipi interface is null!";
        return;
    }

    if (!m_dialogSend) {
        // We clean it up in the close button
        m_dialogSend = new KIPITelepathy::Dialog(interface, kapp->activeWindow());
    } else {
        if (m_dialogSend->isMinimized()) {
            KWindowSystem::unminimizeWindow(m_dialogSend->winId());
        }
        KWindowSystem::activateWindow(m_dialogSend->winId());
    }

    m_dialogSend->reactivate();
}

KIPI::Category KIPITelepathy::Plugin::category(KAction* action) const
{
    Q_UNUSED(action)
    return KIPI::ExportPlugin;
}

#include "kipi-telepathy-plugin.moc"

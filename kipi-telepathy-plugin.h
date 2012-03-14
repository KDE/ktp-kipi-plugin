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


#ifndef KIPI_TELEPATHY_PLUGIN_H
#define KIPI_TELEPATHY_PLUGIN_H

#include <libkipi/plugin.h>
#include <QtCore/QVariantList>

class KAction;
class QWidget;

namespace KIPITelepathy {
    class Dialog;

class Plugin : public KIPI::Plugin
{
    Q_OBJECT
    Q_DISABLE_COPY(Plugin)

public:
    Plugin(QObject* parent, const QVariantList& args);
    ~Plugin();

    KIPI::Category category(KAction* action) const;
    void setup(QWidget *widget);

public Q_SLOTS:
    void onActionTriggered();

private:
    KAction *m_action;
    KIPITelepathy::Dialog *m_dialogSend;
};

} // namespace KIPITelepathy

#endif // KIPI_PLUGINS_TELEPATHY_PLUGIN_TELEPATHY_H

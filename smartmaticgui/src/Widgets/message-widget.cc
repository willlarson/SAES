/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * smartmaticgui
 * Copyright (C) Juan Delgado 2011 <juan.delgado@smartmatic.com>
 * 
 * smartmaticgui is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * smartmaticgui is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "message-widget.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef ENABLE_NLS
#include <libintl.h>
#define _(String) dgettext (GETTEXT_PACKAGE, String) ///<macro for gettext
#else
#define _(String) (String) ///<macro without gettext
#endif

#define N_(String) (String) ///<macro for gettext not translate

using namespace std;
using namespace Gtk;
using namespace Smartmatic::GUI::Widgets;
using namespace Smartmatic::GUI::Configuration;

MessageWidget::MessageWidget(Gtk::Widget* widgetChild)
{
	add(*widgetChild);
}

MessageWidget::MessageWidget(Gtk::Widget* widgetChild, bool showButton, int timerToClose, string buttonText)
{
	vertBox = manage(new VBox());

	closeButton = manage(new SmartmaticButton());

	closeButton = manage(new SmartmaticButton());
	closeButton->set_size_request(100, 90);//TODO: configuration
	if(buttonText.empty()) closeButton->SetLabel(_("Smartmatic.GUI.Widgets.MessageWidget.OK"));
	else closeButton->SetLabel(_(buttonText.c_str()));
	closeButton->WidgetsPosition(5,0,0);
	closeButton->signal_clicked().connect(sigc::mem_fun(this,&MessageWidget::on_closeButton_clicked));

	if(!showButton) 
		closeButton->set_no_show_all();

	if(widgetChild)
		vertBox->pack_start(*widgetChild);

	buttonAlignment = manage(new Gtk::Alignment());
	buttonAlignment->set_padding(0, 0, 200, 200);
	buttonAlignment->add(*closeButton);
	vertBox->pack_start(*buttonAlignment, false, false);
	totalAlignment = manage(new Gtk::Alignment());
	totalAlignment->add(*vertBox);
	totalAlignment->set_padding(0, 100, 0, 0);

	add(*totalAlignment);
	show_all_children();

	connector_close = Glib::signal_timeout().connect(sigc::bind(sigc::mem_fun(*this, &MessageWidget::on_timeout), 0), timerToClose);
}

MessageWidget::~MessageWidget()
{
	connector_close.disconnect();
}

bool MessageWidget::on_timeout(int timer)
{
	m_close_clicked.emit();
	return false;
}

void MessageWidget::on_closeButton_clicked()
{
	connector_close.disconnect();
	m_close_clicked.emit();
}

MessageWidget::type_close_clicked MessageWidget::close_clicked()
{
	return m_close_clicked;
}

void MessageWidget::closeButtonSetName (std::string name)
{
	closeButton->set_name(name);
}

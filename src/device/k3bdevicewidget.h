/***************************************************************************
                          k3bdevicewidget.h  -  description
                             -------------------
    begin                : Wed Apr 17 2002
    copyright            : (C) 2002 by Sebastian Trueg
    email                : trueg@informatik.uni-freiburg.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef K3BDEVICEWIDGET_H
#define K3BDEVICEWIDGET_H

#include <qwidget.h>
#include <qptrlist.h>

class QComboBox;
class QLabel;
class QGroupBox;
class QPushButton;
class QCheckBox;
class K3bListView;
class QString;
class KIntNumInput;
class QFrame;
class QListViewItem;
class QString;
class K3bDevice;
class K3bDeviceManager;
class QLineEdit;


/**
  *@author Sebastian Trueg
  */
class K3bDeviceWidget : public QWidget
{
  Q_OBJECT

 public:
  K3bDeviceWidget( K3bDeviceManager*, QWidget *parent = 0, const char *name = 0 );
  ~K3bDeviceWidget();

 public slots:
  void init();
  void apply();

 signals:
  void refreshButtonClicked();

 private slots:
  void slotNewDevice();

 private:
  class PrivateTempDevice;
  class PrivateDeviceViewItem1;
  class PrivateDeviceViewItem2;

  /** list to save changes to the devices before applying */
  QList<PrivateTempDevice> m_tempDevices;

  void updateDeviceListViews();

  QListViewItem* m_writerParentViewItem;
  QListViewItem* m_readerParentViewItem;

  K3bDeviceManager* m_deviceManager;

  K3bListView*    m_viewDevices;
  QPushButton* m_buttonRefreshDevices;
  QPushButton* m_buttonAddDevice;
};

#endif

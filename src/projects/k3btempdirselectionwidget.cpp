/*
 *
 * $Id$
 * Copyright (C) 2003 Sebastian Trueg <trueg@k3b.org>
 *
 * This file is part of the K3b project.
 * Copyright (C) 1998-2003 Sebastian Trueg <trueg@k3b.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * See the file "COPYING" for the exact licensing terms.
 */


#include "k3btempdirselectionwidget.h"
#include <k3bglobals.h>
#include <k3bcore.h>

#include <qlabel.h>
#include <qgroupbox.h>
#include <qlayout.h>
#include <qtimer.h>
#include <qhbox.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qfileinfo.h>

#include <kconfig.h>
#include <klocale.h>
#include <kfiledialog.h>
#include <kdialog.h>
#include <kstandarddirs.h>
#include <kiconloader.h>
#include <kurlrequester.h>


K3bTempDirSelectionWidget::K3bTempDirSelectionWidget( QWidget *parent, const char *name )
  : QGroupBox( 4, Qt::Vertical, i18n( "Temp Directory" ), parent, name )
{
  layout()->setSpacing( KDialog::spacingHint() );
  layout()->setMargin( KDialog::marginHint() );

  QLabel* imageFileLabel = new QLabel( i18n( "Wri&te image file to:" ), this );
  m_editDirectory = new KURLRequester( this, "m_editDirectory" );

  imageFileLabel->setBuddy( m_editDirectory );

  QHBox* freeTempSpaceBox = new QHBox( this );
  freeTempSpaceBox->setSpacing( KDialog::spacingHint() );
  (void)new QLabel( i18n( "Free space in temporary directory" ), freeTempSpaceBox, "TextLabel2" );
  m_labelFreeSpace = new QLabel( "                       ",freeTempSpaceBox, "m_labelFreeSpace" );
  m_labelFreeSpace->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

  QHBox* cdSizeBox = new QHBox( this );
  cdSizeBox->setSpacing( KDialog::spacingHint() );
  (void)new QLabel( i18n( "Size of Project" ), cdSizeBox, "TextLabel4" );
  m_labelCdSize = new QLabel( "                        ", cdSizeBox, "m_labelCdSize" );
  m_labelCdSize->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

  connect( m_editDirectory, SIGNAL(openFileDialog(KURLRequester*)),
	   this, SLOT(slotTempDirButtonPressed(KURLRequester*)) );
  connect( m_editDirectory, SIGNAL(textChanged(const QString&)),
	   this, SLOT(slotUpdateFreeTempSpace()) );


  m_mode = DIR;

  m_editDirectory->setURL( K3b::defaultTempPath() );
  slotUpdateFreeTempSpace();

  // ToolTips
  // --------------------------------------------------------------------------------
  QToolTip::add( m_editDirectory, i18n("The directory in which to save the image files") );

  // What's This info
  // --------------------------------------------------------------------------------
  QWhatsThis::add( m_editDirectory, i18n("<p>This is the directory in which K3b will save the <em>image files</em>."
					 "<p>Please make sure that it resides on a partition that has enough free space.") );
}


K3bTempDirSelectionWidget::~K3bTempDirSelectionWidget()
{
}


void K3bTempDirSelectionWidget::slotFreeTempSpace(const QString&,
						  unsigned long,
						  unsigned long,
						  unsigned long kbAvail)
{
  m_labelFreeSpace->setText( KIO::convertSizeFromKB(kbAvail) );

  m_freeTempSpace = kbAvail;

  if( m_freeTempSpace < m_requestedSize/1024 )
    m_labelCdSize->setPaletteForegroundColor( red );
  else
    m_labelCdSize->setPaletteForegroundColor( m_labelFreeSpace->paletteForegroundColor() );
  QTimer::singleShot( 1000, this, SLOT(slotUpdateFreeTempSpace()) );
}


void K3bTempDirSelectionWidget::slotUpdateFreeTempSpace()
{
  QString path = m_editDirectory->url();

  if( !QFile::exists( path ) )
    path.truncate( path.findRev('/') );

  unsigned long size, avail;
  if( K3b::kbFreeOnFs( path, size, avail ) )
    slotFreeTempSpace( path, size, 0, avail );
  else
    m_labelFreeSpace->setText("-");
}


void K3bTempDirSelectionWidget::slotTempDirButtonPressed( KURLRequester* r )
{
  // set the correct mode for the filedialog
  if( m_mode == DIR ) {
    r->setCaption( i18n("Select Temporary Directory") );
    r->setMode( KFile::Directory | KFile::ExistingOnly | KFile::LocalOnly );
  }
  else {
    r->setCaption( i18n("Select Temporary File") );
    r->setMode( KFile::File | KFile::LocalOnly );
  }
}


void K3bTempDirSelectionWidget::setTempPath( const QString& dir )
{
  m_editDirectory->setURL( dir );
  slotUpdateFreeTempSpace();
}


QString K3bTempDirSelectionWidget::tempPath() const
{
  QFileInfo fi( m_editDirectory->url() );

  if( fi.exists() ) {
    if( m_mode == DIR ) {
      if( fi.isDir() )
	return fi.absFilePath();
      else
	return fi.dirPath( true );
    }
    else {
      if( fi.isFile() )
	return fi.absFilePath();
      else
	return fi.absFilePath() + "/k3b_image.img";
    }
  }
  else {
    return fi.absFilePath();
  }
}


QString K3bTempDirSelectionWidget::tempDirectory() const
{
  QString td( m_editDirectory->url() );

  // remove a trailing slash
  while( !td.isEmpty() && td[td.length()-1] == '/' )
    td.truncate( td.length()-1 );

  QFileInfo fi( td );
  if( fi.exists() && fi.isDir() )
    return td + "/";

  // now we treat the last section as a filename and return the path
  // in front of it
  td.truncate( td.findRev( '/' ) + 1 );
  return td;
}


void K3bTempDirSelectionWidget::setSelectionMode( int mode )
{
  m_mode = mode;

  if( m_mode == DIR )
    setTitle( i18n("Temporary Directory") );
  else
    setTitle( i18n("Temporary File") );
}


void K3bTempDirSelectionWidget::setNeededSize( unsigned long bytes )
{
  m_requestedSize = bytes;
  m_labelCdSize->setText( QString().sprintf( " %.2f MB", ((float)bytes)/1024.0/1024.0 ) );
}


void K3bTempDirSelectionWidget::saveConfig()
{
  QString oldGroup = k3bcore->config()->group();
  k3bcore->config()->setGroup( "General Options" );
  k3bcore->config()->writePathEntry( "Temp Dir", tempDirectory() );
  k3bcore->config()->setGroup( oldGroup );
}

#include "k3btempdirselectionwidget.moc"

/***************************************************************************
                          k3bdataview.cpp  -  description
                             -------------------
    begin                : Thu May 10 2001
    copyright            : (C) 2001 by Sebastian Trueg
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

#include "k3bdataview.h"
#include "k3bdatadoc.h"
#include "../k3bfillstatusdisplay.h"
#include "../k3b.h"
#include "k3bdataburndialog.h"
#include "k3bbootimageview.h"
#include <device/k3bdevice.h>
#include <tools/k3bdeviceselectiondialog.h>


#include <klocale.h>
#include <kurl.h>
#include <kapplication.h>
#include <kpopupmenu.h>
#include <kaction.h>
#include <kmessagebox.h>
#include <kio/global.h>
#include <kio/job.h>
#include <kdialogbase.h>

#include <qpixmap.h>
#include <qsplitter.h>
#include <qlayout.h>
#include <qdragobject.h>
#include <qheader.h>
#include <qptrlist.h>

#include <assert.h>
#include <kdebug.h>


K3bDataView::K3bDataView(K3bDataDoc* doc, QWidget *parent, const char *name )
  : K3bView(doc, parent,name)
{
  m_doc = doc;
	
  // --- setup GUI ---------------------------------------------------
  QSplitter* mainSplitter = new QSplitter( this );	
  m_dataDirTree = new K3bDataDirTreeView( this, doc, mainSplitter );
  m_dataFileView = new K3bDataFileView( this, m_dataDirTree, doc, mainSplitter );
  m_dataDirTree->setFileView( m_dataFileView );
  m_fillStatusDisplay = new K3bFillStatusDisplay( doc, this );
	
  QVBoxLayout* box = new QVBoxLayout( this );
  box->addWidget( mainSplitter );
  box->addWidget( m_fillStatusDisplay );
  box->setStretchFactor( mainSplitter, 1 );
  box->setSpacing( 5 );
  box->setMargin( 2 );


  connect( m_dataFileView, SIGNAL(dirSelected(K3bDirItem*)), m_dataDirTree, SLOT(setCurrentDir(K3bDirItem*)) );

  connect( m_doc, SIGNAL(itemRemoved(K3bDataItem*)), m_fillStatusDisplay, SLOT(update()) );
  connect( m_doc, SIGNAL(newFileItems()), m_fillStatusDisplay, SLOT(update()) );

  m_dataDirTree->updateContents();
  m_dataFileView->updateContents();
}


K3bDataView::~K3bDataView(){
}


void K3bDataView::burnDialog( bool withWriting )
{
  K3bDataBurnDialog d( m_doc, this, "databurndialog", true );
  d.exec( withWriting );
}


K3bDirItem* K3bDataView::currentDir() const
{
  return m_dataFileView->currentDir();
}





// void K3bDataView::slotItemRemoved( K3bDataItem* item )
// {
//   // we only need to search in the fileView if it currently displays the corresponding directory
//   if( item == m_dataFileView->currentDir() ) {
//     kdDebug() << "(K3bDataView) fileView currently displays a deleted directory. Setting to parent." << endl;
//     m_dataFileView->slotSetCurrentDir( item->parent() );
//   }
//   else if( item->parent() == m_dataFileView->currentDir() ) {
//     kdDebug() << "(K3bDataView) seaching in fileView for viewItems to delete" << endl;
//     QListViewItemIterator _it2(m_dataFileView);
//     for( ; _it2.current(); ++_it2 )
//       {
// 	if( K3bDataDirViewItem* _dirViewItem = dynamic_cast<K3bDataDirViewItem*>(_it2.current()) ) {
// 	  kdDebug() << "   found dirViewItem ... comparing ... " << endl;
// 	  if( _dirViewItem->dirItem() == item ) {
// 	    delete _it2.current();
// 	    kdDebug() << "(K3bDataView) found listViewItem to remove in fileView: " << item->k3bName() << endl;
// 	    break;
// 	  }
// 	}
// 	else if( K3bDataFileViewItem* _fileViewItem = dynamic_cast<K3bDataFileViewItem*>(_it2.current()) ) {
// 	  kdDebug() << "   found fileViewItem ... comparing ... " << endl;
// 	  if( _fileViewItem->fileItem() == item ) {
// 	    delete _it2.current();
// 	    kdDebug() << "(K3bDataView) found listViewItem to remove in fileView: " << item->k3bName() << endl;
// 	    break;
// 	  }
// 	}
			
//       } // for _it2
//   }

//   m_fillStatusDisplay->repaint();	
// }


void K3bDataView::importSession()
{
  // get the writer
  m_device = K3bDeviceSelectionDialog::selectWriter( this, i18n("Please select the appendable disk") );

  if( m_device ) {
    // TODO: check if it's a data cd and appendable

    k3bMain()->showBusyInfo( i18n("Mounting disk...") );

    // mount the cd
    connect( KIO::mount( true, 0L, m_device->mountDevice(), m_device->mountPoint(), false ), SIGNAL(result(KIO::Job*)),
	     this, SLOT(slotMountFinished(KIO::Job*)) );
  }
}


void K3bDataView::slotMountFinished( KIO::Job* job )
{
  if( job->error() ) {
    KMessageBox::error( this, KIO::buildErrorString( job->error(), m_device->vendor() + " " + m_device->description() ) );
  }
  else {
    k3bMain()->showBusyInfo( i18n("Importing old session...") );
    m_doc->setBurner( m_device );
    m_doc->importSession( m_device->mountPoint() );

    // unmount the cd
    KIO::unmount( m_device->mountPoint(), false );
  }

  k3bMain()->endBusy();
}


void K3bDataView::clearImportedSession()
{
  m_doc->clearImportedSession();
  m_doc->setMultiSessionMode( K3bDataDoc::NONE );
}


void K3bDataView::editBootImages()
{
  KDialogBase* d = new KDialogBase( this, "", true, i18n("Edit boot images"), KDialogBase::Ok, KDialogBase::Ok, true );
  d->setMainWidget( new K3bBootImageView( m_doc, d ) );
  d->exec();
}

#include "k3bdataview.moc"

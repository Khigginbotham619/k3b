/****************************************************************************
** Form implementation generated from reading ui file '/home/trueg/dev/k3b-setup.ui'
**
** Created: Sat Dec 1 14:39:25 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "k3bsetupwizard.h"

#include "k3bsetup.h"

#include "../device/k3bdevicemanager.h"
#include "../device/k3bdevice.h"

#include <pwd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <qvariant.h>   // first for gcc 2.7.2
#include <qcheckbox.h>
#include <qframe.h>
#include <qgroupbox.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qpixmap.h>
#include <qfile.h>
#include <qlist.h>
#include <qtextstream.h>

#include <klocale.h>
#include <kstddirs.h>
#include <ksimpleconfig.h>
#include <kmessagebox.h>
#include <klineeditdlg.h>
#include <kiconloader.h>
#include <klistview.h>
#include <kdialog.h>
#include <kfiledialog.h>



class K3bSetupWizard::PrivateDeviceViewItem : public KListViewItem
{
public:
  PrivateDeviceViewItem( K3bDevice*, KListView*, const QString& = QString::null );
  PrivateDeviceViewItem( K3bDevice*, KListViewItem*, const QString& = QString::null );
  PrivateDeviceViewItem( K3bDevice*, KListViewItem*, KListViewItem* prev, const QString& = QString::null );

  K3bDevice* device;
};


K3bSetupWizard::PrivateDeviceViewItem::PrivateDeviceViewItem( K3bDevice* dev, KListView* parent, const QString& str )
  : KListViewItem( parent, str )
{
  device = dev;
}


K3bSetupWizard::PrivateDeviceViewItem::PrivateDeviceViewItem( K3bDevice* dev, KListViewItem* parent, const QString& str )
  : KListViewItem( parent, str )
{
  device = dev;
}


K3bSetupWizard::PrivateDeviceViewItem::PrivateDeviceViewItem( K3bDevice* dev, KListViewItem* parent, KListViewItem* prev, const QString& str )
  : KListViewItem( parent, prev, str )
{
  device = dev;
}



K3bSetupWizard::K3bSetupWizard( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : KWizard( parent, name, modal, fl )
{

  // create the K3bSetup instance
  m_setup = new K3bSetup();

  m_configPath = locate( "data", "k3b/" ) + "k3bsetup";
  qDebug( m_configPath );
  m_config = new KSimpleConfig( m_configPath );

  // create a K3bDeviceManager
  m_deviceManager = new K3bDeviceManager( this );





  QPixmap image0( locate( "data", "k3b/pics/k3bsetup.png" ) );

  if ( !name )
    setName( "K3bSetupWizard" );

  setCaption( i18n( "K3b Setup" ) );

  cancelButton()->setText( i18n("Close") );
  cancelButton()->disconnect();
  connect( cancelButton(), SIGNAL(clicked()), this, SLOT(close()) );


  // setup page1
  // -----------------------------------------------------------------------------------------------------------
  m_page1 = new QWidget( this, "m_page1" );
  QGridLayout* pageLayout = new QGridLayout( m_page1 ); 
  pageLayout->setSpacing( KDialog::spacingHint() );
  pageLayout->setMargin( KDialog::marginHint() );

  QLabel* pixmapLabel = new QLabel( m_page1, "pixmapLabel" );
  pixmapLabel->setPixmap( image0 );
  pixmapLabel->setScaledContents( TRUE );

  pageLayout->addWidget( pixmapLabel, 0, 0 );

  m_labelWelcome = new QLabel( m_page1, "m_labelWelcome" );
  m_labelWelcome->setText( i18n( "Welcome to K3b Setup. This Wizard will help you to prepare your system for cd writing with K3b.\n"
				 "First of all: DO NOT fear linux permissions anymore. K3b Setup takes care of nearly everything."
				 " So if you are new to linux and know nothing about things like suid root don't panic!"
				 " Just click next.\n"
				 "If you know what is needed to write cds under linux and think you do not need a wizard like this"
				 " please do not exit since there are some things special to K3b and K3b Setup will not change your"
				 " system without your authentification.\n"
				 "\n"
				 "Thanx for using K3b." ) );
  m_labelWelcome->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );

  pageLayout->addWidget( m_labelWelcome, 0, 1 );

  pageLayout->setColStretch( 1, 1 );

  addPage( m_page1, i18n( "Welcome to K3b Setup" ) );
  // -----------------------------------------------------------------------------------------------------------



  // setup page 2
  // -----------------------------------------------------------------------------------------------------------
  m_page2 = new QWidget( this, "m_page2" );
  QGridLayout* pageLayout_2 = new QGridLayout( m_page2 );
  pageLayout_2->setSpacing( KDialog::spacingHint() );
  pageLayout_2->setMargin( KDialog::marginHint() );

  m_buttonAddDevice = new QPushButton( m_page2, "m_buttonAddDevice" );
  m_buttonAddDevice->setText( i18n( "Add device" ) );

  m_labelSetupDrives = new QLabel( m_page2, "m_labelSetupDrives" );
  m_labelSetupDrives->setText( i18n( "K3b Setup has detected the following CD drives. It tries hard to detect capabilities like the"
				     " maximum read and write speed but does sometimes fail.\n"
				     "Please check if all capabilities are detected correctly and change them manually if not."
				     " You can add additional devices (like /dev/cdrom) if your drive has not been detected." ) );
  m_labelSetupDrives->setAlignment( int( QLabel::WordBreak | QLabel::AlignTop ) );

  QLabel* pixmapLabel2 = new QLabel( m_page2, "pixmapLabel2" );
  pixmapLabel2->setPixmap( image0 );
  pixmapLabel2->setScaledContents( TRUE );



  QGroupBox* groupReader = new QGroupBox( i18n("Reading devices"), m_page2 );
  groupReader->setColumnLayout(0, Qt::Vertical );
  groupReader->layout()->setSpacing( 0 );
  groupReader->layout()->setMargin( 0 );
  QHBoxLayout* groupReaderLayout = new QHBoxLayout( groupReader->layout() );
  groupReaderLayout->setSpacing( KDialog::spacingHint() );
  groupReaderLayout->setMargin( KDialog::marginHint() );

  m_viewSetupReader = new KListView( groupReader, "m_viewSetupDrives" );
  groupReaderLayout->addWidget( m_viewSetupReader );
  m_viewSetupReader->addColumn( i18n( "system device" ) );
  m_viewSetupReader->addColumn( i18n( "value" ) );
  m_viewSetupReader->header()->hide();
  m_viewSetupReader->setSorting( -1 );
  m_viewSetupReader->setAllColumnsShowFocus( true );
  m_viewSetupReader->setItemsRenameable( true );
  m_viewSetupReader->setRenameable( 0, false );
  m_viewSetupReader->setRenameable( 1, true );



  QGroupBox* groupWriter = new QGroupBox( i18n("Writing devices"), m_page2 );
  groupWriter->setColumnLayout(0, Qt::Vertical );
  groupWriter->layout()->setSpacing( 0 );
  groupWriter->layout()->setMargin( 0 );
  QHBoxLayout* groupWriterLayout = new QHBoxLayout( groupWriter->layout() );
  groupWriterLayout->setSpacing( KDialog::spacingHint() );
  groupWriterLayout->setMargin( KDialog::marginHint() );

  m_viewSetupWriter = new KListView( groupWriter, "m_viewSetupDrives" );
  groupWriterLayout->addWidget( m_viewSetupWriter );
  m_viewSetupWriter->addColumn( i18n( "system device" ) );
  m_viewSetupWriter->addColumn( i18n( "value" ) );
  m_viewSetupWriter->header()->hide();
  m_viewSetupWriter->setSorting( -1 );
  m_viewSetupWriter->setAllColumnsShowFocus( true );
  m_viewSetupWriter->setItemsRenameable( true );
  m_viewSetupWriter->setRenameable( 0, false );
  m_viewSetupWriter->setRenameable( 1, true );


  QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
  QHBoxLayout* buttonLayout = new QHBoxLayout;
  buttonLayout->setMargin( 0 );
  buttonLayout->setSpacing( 0 );
  buttonLayout->addItem( spacer );
  buttonLayout->addWidget( m_buttonAddDevice, 2, 3 );

  pageLayout_2->addMultiCellWidget( pixmapLabel2, 0, 2, 0, 0 );
  pageLayout_2->addMultiCellWidget( m_labelSetupDrives, 0, 0, 1, 2 );
  pageLayout_2->addMultiCellLayout( buttonLayout, 2, 2, 1, 2 );
  pageLayout_2->addWidget( groupReader, 1, 1 );
  pageLayout_2->addWidget( groupWriter, 1, 2 );

  addPage( m_page2, i18n( "Setup cd drives" ) );
  // -----------------------------------------------------------------------------------------------------------




  // setup page 3
  // -----------------------------------------------------------------------------------------------------------
  m_page3 = new QWidget( this, "m_page3" );
  QGridLayout* pageLayout_3 = new QGridLayout( m_page3 ); 
  pageLayout_3->setSpacing( KDialog::spacingHint() );
  pageLayout_3->setMargin( KDialog::marginHint() );

  QLabel* pixmapLabel3 = new QLabel( m_page3, "pixmapLabel3" );
  pixmapLabel3->setPixmap( image0 );
  pixmapLabel3->setScaledContents( TRUE );

  pageLayout_3->addWidget( pixmapLabel3, 0, 0 );

  m_labelNoWriter = new QLabel( m_page3, "m_labelNoWriter" );
  m_labelNoWriter->setText( i18n( "K3b did not find a cd writer on your system. If you have no cd writer and want to use K3b only"
				  " for cd ripping everything is fine.\n"
				  "If you have an Atapi writer you should enable SCSI emulation.\n"
				  "\n"
				  "[Description how to do this]\n"
				  "\n"
				  "If you are sure you have either a SCSI cd writer or enabled SCSI emulation please go back and"
				  " add the device. If that does not work... well... please report!" ) );
  m_labelNoWriter->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );

  pageLayout_3->addWidget( m_labelNoWriter, 0, 1 );
  addPage( m_page3, i18n( "No cd writer found" ) );
  // -----------------------------------------------------------------------------------------------------------




  // setup page 4
  // -----------------------------------------------------------------------------------------------------------
  m_page4 = new QWidget( this, "m_page4" );
  QGridLayout* pageLayout_4 = new QGridLayout( m_page4 ); 
  pageLayout_4->setSpacing( KDialog::spacingHint() );
  pageLayout_4->setMargin( KDialog::marginHint() );

  QLabel* pixmapLabel4 = new QLabel( m_page4, "pixmapLabel4" );
  pixmapLabel4->setPixmap( image0 );
  pixmapLabel4->setScaledContents( TRUE );

  m_labelFstab = new QLabel( m_page4, "m_labelFstab" );
  m_labelFstab->setText( i18n( "K3b Setup will create entries in /etc/fstab for each of the detected cd drives.\n"
			       "You can change the mountpoint and create a new mountpoint or skip this step."
			       " (Caution: K3b will not be able to mount the device if no fstab entry is available!)" ) );
  m_labelFstab->setAlignment( int( QLabel::WordBreak | QLabel::AlignTop ) );

  m_viewFstab = new KListView( m_page4, "m_viewFstab" );
  m_viewFstab->addColumn( i18n( "CD drive" ) );
  m_viewFstab->addColumn( i18n( "System device" ) );
  m_viewFstab->addColumn( i18n( "Mount point" ) );
  m_viewFstab->setAllColumnsShowFocus( true );

  m_checkFstab = new QCheckBox( i18n("Let K3b Setup create fstab entries."), m_page4 );
  m_checkFstab->setChecked( true );

  m_buttonSelectMountPoint = new QPushButton( i18n("Select mount point"), m_page4 );


  pageLayout_4->addMultiCellWidget( pixmapLabel4, 0, 2, 0, 0 );
  pageLayout_4->addMultiCellWidget( m_labelFstab, 0, 0, 1, 2 );
  pageLayout_4->addMultiCellWidget( m_viewFstab, 1, 1, 1, 2 );
  pageLayout_4->addWidget( m_checkFstab, 2, 1 );
  pageLayout_4->addWidget( m_buttonSelectMountPoint, 2, 2 );

  pageLayout_4->setColStretch( 1, 1 );

  addPage( m_page4, i18n( "Create /etc/fstab entries" ) );
  // -----------------------------------------------------------------------------------------------------------




  // setup page 5
  // -----------------------------------------------------------------------------------------------------------
  m_page5 = new QWidget( this, "m_page5" );
  QGridLayout* pageLayout_5 = new QGridLayout( m_page5 ); 
  pageLayout_5->setSpacing( KDialog::spacingHint() );
  pageLayout_5->setMargin( KDialog::marginHint() );

  m_labelExternalPrograms = new QLabel( m_page5, "m_labelExternalPrograms" );
  m_labelExternalPrograms->setText( i18n( "K3b uses cdrdao, cdrecord and mkisofs to actually write the cds.\n"
					  "It is recommended to install these programs. K3b will run without them but major"
					  " functions will be disabled.\n"
					  "K3b Setup tries to find the executables. You can change the paths manually if you"
					  " want other versions to be used or K3b Setup did not find your installation." ) );
  m_labelExternalPrograms->setAlignment( int( QLabel::WordBreak | QLabel::AlignTop ) );

  pageLayout_5->addWidget( m_labelExternalPrograms, 0, 1 );

  QLabel* pixmapLabel5 = new QLabel( m_page5, "pixmapLabel5" );
  pixmapLabel5->setPixmap( image0 );
  pixmapLabel5->setScaledContents( TRUE );

  pageLayout_5->addMultiCellWidget( pixmapLabel5, 0, 1, 0, 0 );

  m_viewExternalPrograms = new KListView( m_page5, "m_viewExternalPrograms" );
  m_viewExternalPrograms->addColumn( i18n( "program" ) );
  m_viewExternalPrograms->addColumn( i18n( "found" ) );
  m_viewExternalPrograms->addColumn( i18n( "path" ) );
  m_viewExternalPrograms->setAllColumnsShowFocus( true );


  pageLayout_5->addWidget( m_viewExternalPrograms, 1, 1 );
  addPage( m_page5, i18n( "Setup external programs" ) );
  // -----------------------------------------------------------------------------------------------------------




  // setup page 6
  // -----------------------------------------------------------------------------------------------------------
  m_page6 = new QWidget( this, "m_page6" );
  QGridLayout* pageLayout_6 = new QGridLayout( m_page6 ); 
  pageLayout_6->setSpacing( KDialog::spacingHint() );
  pageLayout_6->setMargin( KDialog::marginHint() );

  m_labelPermissions1 = new QLabel( m_page6, "m_labelPermissions1" );
  m_labelPermissions1->setText( i18n( "The external programs need to be run as root since they need write access to the cd drives"
				      " and run with higher priority. K3b also needs write access to the cd drives (for functions"
				      " like detecting the capacity of a cd).\n"
				      "If you know what you are doing you can skip this and setup the permissions for yourself.\n"
				      "To learn more about what K3b Setup will do press \"Details\"." ) );
  m_labelPermissions1->setAlignment( int( QLabel::WordBreak | QLabel::AlignTop ) );

  pageLayout_6->addMultiCellWidget( m_labelPermissions1, 0, 0, 1, 3 );

  m_groupUsers = new QGroupBox( m_page6, "m_groupUsers" );
  m_groupUsers->setTitle( i18n( "K3b users" ) );
  m_groupUsers->setColumnLayout(0, Qt::Vertical );
  m_groupUsers->layout()->setSpacing( 6 );
  m_groupUsers->layout()->setMargin( 11 );
  QGridLayout* groupUsersLayout = new QGridLayout( m_groupUsers->layout() );
  groupUsersLayout->setAlignment( Qt::AlignTop );

  m_boxUsers = new QListBox( m_groupUsers, "m_boxUsers" );

  groupUsersLayout->addMultiCellWidget( m_boxUsers, 0, 2, 0, 0 );

  m_buttonRemoveUser = new QPushButton( m_groupUsers, "m_buttonRemoveUser" );
  m_buttonRemoveUser->setText( i18n( "Remove user" ) );

  groupUsersLayout->addWidget( m_buttonRemoveUser, 0, 1 );

  m_buttonAddUser = new QPushButton( m_groupUsers, "m_buttonAddUser" );
  m_buttonAddUser->setText( i18n( "Add user" ) );

  groupUsersLayout->addWidget( m_buttonAddUser, 1, 1 );
  QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
  groupUsersLayout->addItem( spacer_2, 2, 1 );

  pageLayout_6->addMultiCellWidget( m_groupUsers, 1, 3, 2, 3 );

  m_checkPermissionsDevices = new QCheckBox( m_page6, "m_checkPermissionsDevices" );
  m_checkPermissionsDevices->setText( i18n( "let K3b Setup do the needed changes for the devices" ) );
  m_checkPermissionsDevices->setChecked( TRUE );

  pageLayout_6->addMultiCellWidget( m_checkPermissionsDevices, 6, 6, 1, 2 );

  m_checkPermissionsExternalPrograms = new QCheckBox( m_page6, "m_checkPermissionsExternalPrograms" );
  m_checkPermissionsExternalPrograms->setText( i18n( "let K3b Setup do the needed changes for the external programs" ) );
  m_checkPermissionsExternalPrograms->setChecked( TRUE );

  pageLayout_6->addMultiCellWidget( m_checkPermissionsExternalPrograms, 5, 5, 1, 2 );

  QFrame* Line1 = new QFrame( m_page6, "Line1" );
  Line1->setProperty( "frameShape", (int)QFrame::HLine );
  Line1->setFrameShadow( QFrame::Sunken );
  Line1->setFrameShape( QFrame::HLine );

  pageLayout_6->addMultiCellWidget( Line1, 4, 4, 1, 3 );

  m_labelPermissions2 = new QLabel( m_page6, "m_labelPermissions2" );
  m_labelPermissions2->setText( i18n( "Please specify the users that will use K3b. You can also specify an alternative group name"
				      " if you want to. If you do not know what that means just leave the default. (See \"Details\" "
				      "for more information.)" ) );
  m_labelPermissions2->setAlignment( int( QLabel::WordBreak | QLabel::AlignTop ) );

  pageLayout_6->addWidget( m_labelPermissions2, 1, 1 );

  QVBoxLayout* Layout1 = new QVBoxLayout( 0, 0, 6, "Layout1"); 
  QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
  Layout1->addItem( spacer_3 );

  m_buttonPermissionsDetails = new QPushButton( m_page6, "m_buttonPermissionsDetails" );
  m_buttonPermissionsDetails->setText( i18n( "Details" ) );
  Layout1->addWidget( m_buttonPermissionsDetails );

  pageLayout_6->addMultiCellLayout( Layout1, 5, 6, 3, 3 );

  QLabel* pixmapLabel6 = new QLabel( m_page6, "pixmapLabel6" );
  pixmapLabel6->setPixmap( image0 );
  pixmapLabel6->setScaledContents( TRUE );

  pageLayout_6->addMultiCellWidget( pixmapLabel6, 0, 6, 0, 0 );

  m_groupWriterGroup = new QGroupBox( m_page6, "m_groupWriterGroup" );
  m_groupWriterGroup->setTitle( i18n( "cd writing group" ) );
  m_groupWriterGroup->setColumnLayout(0, Qt::Vertical );
  m_groupWriterGroup->layout()->setSpacing( 6 );
  m_groupWriterGroup->layout()->setMargin( 11 );
  QHBoxLayout* groupWriterGroupLayout = new QHBoxLayout( m_groupWriterGroup->layout() );
  groupWriterGroupLayout->setAlignment( Qt::AlignTop );

  m_editPermissionsGroup = new QLineEdit( m_groupWriterGroup, "LineEdit1" );
  m_editPermissionsGroup->setText( i18n( "cdrecording" ) );
  groupWriterGroupLayout->addWidget( m_editPermissionsGroup );

  pageLayout_6->addWidget( m_groupWriterGroup, 3, 1 );
  QSpacerItem* spacer_4 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
  pageLayout_6->addItem( spacer_4, 2, 1 );
  addPage( m_page6, i18n( "Setup permissions" ) );
  // -----------------------------------------------------------------------------------------------------------



  // setup page 7
  m_page7 = new QWidget( this, "m_page7" );
  QGridLayout* pageLayout_7 = new QGridLayout( m_page7 ); 
  pageLayout_7->setSpacing( KDialog::spacingHint() );
  pageLayout_7->setMargin( KDialog::marginHint() );

  QLabel* pixmapLabel7 = new QLabel( m_page7, "pixmapLabel6" );
  pixmapLabel7->setPixmap( image0 );
  pixmapLabel7->setScaledContents( TRUE );

  QLabel* finishedLabel = new QLabel( m_page7, "finishedLabel" );
  finishedLabel->setText( i18n("<b>Congratulations.</b><p>"
			       "You finished the K3b Setup. Just press the Finish button to save your changes and then enjoy "
			       "the best cd writing program of the universe! ;-)") );

  pageLayout_7->addWidget( pixmapLabel7, 0, 0 );
  pageLayout_7->addWidget( finishedLabel, 0, 1 );

  pageLayout_7->setColStretch( 1, 1 );

  addPage( m_page7, i18n( "Finished" ) );
  // -----------------------------------------------------------------------------------------------------------




  connect( m_buttonAddDevice, SIGNAL(clicked()), this, SLOT(slotAddDevice()) );
  connect( m_buttonPermissionsDetails, SIGNAL(clicked()), this, SLOT(slotPermissionsDetails()) );
  connect( m_buttonAddUser, SIGNAL(clicked()), this, SLOT(slotAddUser()) );
  connect( m_buttonRemoveUser, SIGNAL(clicked()), this, SLOT(slotRemoveUser()) );
  connect( m_buttonSelectMountPoint, SIGNAL(clicked()), this, SLOT(slotSelectMountPoint()) );

  connect( m_viewSetupWriter, SIGNAL(itemRenamed(QListViewItem*, const QString&, int)), 
	   this, SLOT(slotDeviceItemRenamed(QListViewItem*, const QString&, int)) );

  connect( m_viewSetupReader, SIGNAL(itemRenamed(QListViewItem*, const QString&, int)), 
	   this, SLOT(slotDeviceItemRenamed(QListViewItem*, const QString&, int)) );

  setFinishEnabled( m_page7, true );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
K3bSetupWizard::~K3bSetupWizard()
{
  delete m_config;
  delete m_setup;
}

							    
void K3bSetupWizard::closeEvent( QCloseEvent* e )
{
  if( KMessageBox::questionYesNo( this, "Do you really want to discard all changes?", "Close" ) == KMessageBox::Yes )
    e->accept();
  else
    e->ignore();
}


void K3bSetupWizard::keyPressEvent( QKeyEvent* e )
{
  if( e->key() == Key_Escape )
    close();   // QDialog calls reject by default which will close our wizard without asking
  else
    KWizard::keyPressEvent( e );
}


bool K3bSetupWizard::appropriate( QWidget* page ) const
{
  if( page == m_page3 )
    return m_deviceManager->burningDevices().isEmpty();

  return true;
}



void K3bSetupWizard::slotAddDevice()
{
  bool ok;
  QString newDevicename = KLineEditDlg::getText( "Please enter the devicename where\n K3b shall search for a new drive\n(example: /dev/mebecdrom)", "/dev/", &ok, this );

  if( ok ) {
    if( K3bDevice* dev = m_deviceManager->addDevice( newDevicename ) ) {
      updateDevices();
    }
    else
      KMessageBox::error( this, "Sorry, could not find an additional device at\n" + newDevicename, i18n("Error"), false );
  }
}


void K3bSetupWizard::slotAddUser()
{
  QString user;
  QString text = i18n("Please enter a user name");
  bool ok = true;
  bool validUser = false;
  while( ok && !validUser ) {
    user = KLineEditDlg::getText( text, QString::null, &ok, this );
    if( ok && !user.isEmpty() )
      validUser = ( getpwnam( user.latin1() ) != 0 );
    else
      validUser = false;
    text = i18n("No valid user name. Please enter a user name");
  }

  if( ok )
    m_boxUsers->insertItem( user );
}


void K3bSetupWizard::slotRemoveUser()
{
  int item = m_boxUsers->currentItem();
  if( item != -1 )
    m_boxUsers->removeItem( item );
}


void K3bSetupWizard::slotPermissionsDetails()
{
  QString info = i18n("more details to follow");
  KMessageBox::information( this, info, i18n("K3b Setup Permission Details") );
}


void K3bSetupWizard::init()
{
  m_deviceManager->scanbus();

  if( m_config->hasGroup("Devices") ) {
    m_config->setGroup( "Devices" );
    m_deviceManager->readConfig( m_config );
  }

  // create listViewItems
  updateDevices();

  updateFstabEntries();


  if( m_config->hasGroup( "Permissions" ) ) {
    m_config->setGroup( "Permissions" );
    m_setup->loadConfig( m_config );
  }

  m_editPermissionsGroup->setText( m_setup->cdWritingGroup() );
  for ( QStringList::ConstIterator it = m_setup->users().begin(); it != m_setup->users().end(); ++it ) {
    m_boxUsers->insertItem( *it );
  }
}


void K3bSetupWizard::updateDevices()
{
  m_viewSetupWriter->clear();
  m_viewSetupReader->clear();

  K3bDevice* dev = m_deviceManager->readingDevices().first();
  while( dev != 0 ) {
    // add device to device list
    PrivateDeviceViewItem* item_2 = new PrivateDeviceViewItem( dev, m_viewSetupReader, dev->devicename() );
    item_2->setPixmap( 0, KGlobal::instance()->iconLoader()->loadIcon( "cdrom_unmount", KIcon::NoGroup, KIcon::SizeSmall ) );

    PrivateDeviceViewItem* item = new PrivateDeviceViewItem( dev, item_2 );
    item->setText( 0, i18n( "Vendor" ) );
    item->setText( 1, dev->vendor() );

    item = new PrivateDeviceViewItem( dev, item_2, item );
    item->setText( 0, i18n( "Model" ) );
    item->setText( 1, dev->description() );

    item = new PrivateDeviceViewItem( dev, item_2, item );
    item->setText( 0, i18n( "Firmware" ) );
    item->setText( 1, dev->version() );

    item = new PrivateDeviceViewItem( dev, item_2, item );
    item->setText( 0, i18n( "Max read speed" ) );
    item->setText( 1, QString::number( dev->maxReadSpeed() ) );

    item_2->setOpen( TRUE );

    dev = m_deviceManager->readingDevices().next();
  }


  dev = m_deviceManager->burningDevices().first();
  while( dev != 0 ) {
    // add device to device list
    PrivateDeviceViewItem* item_2 = new PrivateDeviceViewItem( dev, m_viewSetupWriter, dev->devicename() );
    item_2->setPixmap( 0, KGlobal::instance()->iconLoader()->loadIcon( "cdwriter_unmount", KIcon::NoGroup, KIcon::SizeSmall ) );

    PrivateDeviceViewItem* item = new PrivateDeviceViewItem( dev, item_2 );
    item->setText( 0, i18n( "Vendor" ) );
    item->setText( 1, dev->vendor() );

    item = new PrivateDeviceViewItem( dev, item_2, item );
    item->setText( 0, i18n( "Model" ) );
    item->setText( 1, dev->description() );

    item = new PrivateDeviceViewItem( dev, item_2, item );
    item->setText( 0, i18n( "Firmware" ) );
    item->setText( 1, dev->version() );

    item = new PrivateDeviceViewItem( dev, item_2, item );
    item->setText( 0, i18n( "Max read speed" ) );
    item->setText( 1, QString::number( dev->maxReadSpeed() ) );

    item = new PrivateDeviceViewItem( dev, item_2, item );
    item->setText( 0, i18n( "Max write speed" ) );
    item->setText( 1, QString::number( dev->maxWriteSpeed() ) );
    
    item = new PrivateDeviceViewItem( dev, item_2, item );
    item->setText( 0, i18n( "Burnproof" ) );
    item->setText( 1, dev->burnproof() ? i18n("yes") : i18n("no") );
    
    item_2->setOpen( TRUE );

    dev = m_deviceManager->burningDevices().next();
  }
}


void K3bSetupWizard::slotDeviceItemRenamed(QListViewItem* item, const QString& newText, int col )
{
  if( col != 1 )
    return;

  PrivateDeviceViewItem* deviceItem = dynamic_cast<PrivateDeviceViewItem*>( item );
  if( deviceItem != 0 ) {
    if( item->text(0) == i18n("Max read speed") ) {
      bool ok;
      int newSpeed = newText.toInt( &ok );
      if( ok )
	deviceItem->device->setMaxReadSpeed( newSpeed );
      else
	item->setText( 1, QString::number( deviceItem->device->maxReadSpeed() ) );
    }
    else if( item->text(0) == i18n("Max write speed") ) {
      bool ok;
      int newSpeed = newText.toInt( &ok );
      if( ok )
	deviceItem->device->setMaxWriteSpeed( newSpeed );
      else
	item->setText( 1, QString::number( deviceItem->device->maxWriteSpeed() ) );
    }
    else if( item->text(0) == i18n("Burnproof") ) {
      if( newText == i18n("yes") )
	deviceItem->device->setBurnproof( true );
      else if( newText == i18n("no") )
	deviceItem->device->setBurnproof( false );
      else
	item->setText( 1, deviceItem->device->burnproof() ? i18n("yes") : i18n("no") );
    }
    else
      qDebug("(K3bSetupWizard) invalid item renamed");
  }
}


void K3bSetupWizard::slotSelectMountPoint()
{
  if( m_viewFstab->selectedItem() == 0 )
    return;

  PrivateDeviceViewItem* deviceItem = dynamic_cast<PrivateDeviceViewItem*>( m_viewFstab->selectedItem() );
  if( deviceItem != 0 ) {
    QString newMp = KFileDialog::getExistingDirectory( deviceItem->device->mountPoint(), this,
						       i18n("Select new mount point for %1").arg(deviceItem->device->ioctlDevice()) );
    if( !newMp.isEmpty() ) {
      deviceItem->setText( 2, newMp );
      deviceItem->device->setMountPoint( newMp );
    }
  }
}


void K3bSetupWizard::updateFstabEntries()
{
  m_viewFstab->clear();

  K3bDevice* dev = m_deviceManager->readingDevices().first();
  int i = 0;
  while( dev != 0 ) {
    PrivateDeviceViewItem* item = new PrivateDeviceViewItem( dev, m_viewFstab );
    item->setPixmap( 0, KGlobal::instance()->iconLoader()->loadIcon( "cdrom_unmount", KIcon::NoGroup, KIcon::SizeSmall ) );
    item->setText( 0, dev->vendor() + " " + dev->description() );
    item->setText( 1, dev->ioctlDevice() );
    if( !dev->mountPoint().isEmpty() )
      item->setText( 2, dev->mountPoint() );
    else {
      if( i == 0 )
	item->setText( 2, "/cdrom" );
      else
	item->setText( 2, QString("/cdrom%1").arg(i) );

      dev->setMountPoint( item->text(2) );
    }
      
    i++;
    dev = m_deviceManager->readingDevices().next();
  }

  dev = m_deviceManager->burningDevices().first();
  i = 0;
  while( dev != 0 ) {
    PrivateDeviceViewItem* item = new PrivateDeviceViewItem( dev, m_viewFstab );
    item->setPixmap( 0, KGlobal::instance()->iconLoader()->loadIcon( "cdwriter_unmount", KIcon::NoGroup, KIcon::SizeSmall ) );
    item->setText( 0, dev->vendor() + " " + dev->description() );
    item->setText( 1, dev->ioctlDevice() );
    if( !dev->mountPoint().isEmpty() )
      item->setText( 2, dev->mountPoint() );
    else {
      if( i == 0 )
	item->setText( 2, "/cdrecorder" );
      else
	item->setText( 2, QString("/cdrecorder%1").arg(i) );

      dev->setMountPoint( item->text(2) );
    }
      
    i++;
    dev = m_deviceManager->burningDevices().next();
  }
}


void K3bSetupWizard::apply()
{
  // save devices
  // -----------------------------------------------------------------------
  if( m_config->hasGroup( "Devices" ) )
    m_config->deleteGroup( "Devices" );
  m_config->setGroup( "Devices" );
  m_deviceManager->saveConfig( m_config );
  // -----------------------------------------------------------------------


  // save external programs
  // -----------------------------------------------------------------------

  // -----------------------------------------------------------------------


  // update permissions
  // -----------------------------------------------------------------------
  m_setup->setCdWritingGroup( m_editPermissionsGroup->text() );
  m_setup->clearUsers();
  for( uint i = 0; i < m_boxUsers->count(); ++i ) {
    m_setup->addUser( m_boxUsers->item(i)->text() );
  }

  if( m_config->hasGroup( "Permissions" ) )
    m_config->deleteGroup( "Permissions" );
  m_config->setGroup( "Permissions" );
  m_setup->saveConfig( m_config );

  if( m_checkPermissionsDevices->isChecked() )
    m_setup->applyDevicePermissions( m_deviceManager );
//   if( m_checkPermissionsExternalPrograms->isChecked() )
//     m_setup->applyExternalProgramPermissions( m_externalBinManager );
  // -----------------------------------------------------------------------


  // create fstab entries
  // -----------------------------------------------------------------------
  if( m_checkFstab->isChecked() ) {
    createNewFstab();
  }
  // -----------------------------------------------------------------------


  m_config->sync();
  // let everybody read the global K3b config file
  if( QFile::exists( m_configPath ) )
    chmod( m_configPath.latin1(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );
}


void K3bSetupWizard::createNewFstab()
{
  qDebug("(K3bSetupWizard) creating new /etc/fstab");
  qDebug("(K3bSetupWizard) saving backup to /etc/fstab.k3bsetup");
  
  // move /etc/fstab to /etc/fstab.k3bsetup
  rename( "/etc/fstab", "/etc/fstab.k3bsetup" );


  // create fstab entries or update fstab entries
  QFile oldFstabFile( "/etc/fstab.k3bsetup" );
  oldFstabFile.open( IO_ReadOnly );
  QTextStream fstabStream( &oldFstabFile );
  
  QFile newFstabFile( "/etc/fstab" );
  newFstabFile.open( IO_WriteOnly );
  QTextStream newFstabStream( &newFstabFile );
  
  QString line = fstabStream.readLine();
  while( !line.isNull() ) {
    bool write = true;

    K3bDevice* dev = m_deviceManager->allDevices().first();
    while( dev != 0 ) {
      if( line.startsWith( dev->ioctlDevice() ) )
	write = false;
      dev = m_deviceManager->allDevices().next();
    }

    if( write ) {
      newFstabStream << line << "\n";
    }

    line = fstabStream.readLine();
  }

  // create entries for the devices
  K3bDevice* dev = m_deviceManager->allDevices().first();
  while( dev != 0 ) {

    bool createMoutPoint = true;

    // create mountpoint if it does not exist
    if( !QFile::exists( dev->mountPoint() ) ) {
      if( mkdir( dev->mountPoint().latin1(), S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH ) != 0 ) {
	KMessageBox::error( this, i18n("Could not create mount point '%1'\nNo fstab entry will be created for device %2").arg(dev->mountPoint()).arg(dev->ioctlDevice()) );
	createMoutPoint = false;
      }
    }

    if( createMoutPoint ) {
      // set the correct permissions for the mountpoint
      chmod( dev->mountPoint().latin1(), S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH );
	
      newFstabStream << dev->ioctlDevice() << "\t" 
		     << dev->mountPoint() << "\t"
		     << "auto" << "\t"
		     << "ro,noauto,user,exec" << "\t"
		     << "0 0" << "\n";
    }

    dev = m_deviceManager->allDevices().next();
  }
    
    
  newFstabFile.close();
  newFstabFile.close();

  // set the correct permissions (although they seem to be correct. Just to be sure!)
  chmod( "/etc/fstab", S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH );
}


void K3bSetupWizard::accept()
{
  apply();

  QString finishMessage = i18n("All settings have been saved.\n");
  if( m_checkFstab->isChecked() )
    finishMessage.append( i18n("Your old /etc/fstab file has been saved to /etc/fstab.k3bsetup.\n") );
  if( m_checkPermissionsDevices->isChecked() || m_checkPermissionsExternalPrograms->isChecked() )
    finishMessage.append( i18n("Your old /etc/group file has been saved to /etc/group.k3bsetup.\n") );

  finishMessage.append( i18n("If the configuration of your system changes "
			     "just run K3b Setup again. Your settings will be remembered.\n"
			     "Thanx for using K3b. Have a lot of fun!") );

  KMessageBox::information( this, finishMessage, i18n("K3b Setup finished") );

  KWizard::accept();
}

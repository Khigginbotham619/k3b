/***************************************************************************
                          k3bmixedjob.h  -  Job that creates a mixed mode cd
                             -------------------
    begin                : Fri Aug 23 2002
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

#ifndef K3BMIXEDJOB_H
#define K3BMIXEDJOB_H

#include "../k3bjob.h"


class K3bMixedDoc;
class K3bIsoImager;


/**
  *@author Sebastian Trueg
  */
class K3bMixedJob : public K3bBurnJob
{
  Q_OBJECT
	
 public:
  K3bMixedJob( K3bMixedDoc*, QObject* parent = 0 );
  ~K3bMixedJob();
	
  K3bDoc* doc() const;
/*   K3bDevice* writer() const; */
		
 public slots:
  void cancel();
  void start();

 protected slots:
  void slotSizeCalculationFinished( int, int );

 private:
  K3bMixedDoc* m_doc;
  K3bIsoImager* m_isoImager;
};

#endif

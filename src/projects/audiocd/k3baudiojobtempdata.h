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


#ifndef _K3B_AUDIO_JOB_TEMPDATA_H_
#define _K3B_AUDIO_JOB_TEMPDATA_H_

#include <qobject.h>
#include <k3bmsf.h>

class K3bAudioTrack;
class K3bAudioDoc;
class QTextStream;


class K3bAudioJobTempData : public QObject
{
  Q_OBJECT

 public:
  K3bAudioJobTempData( K3bAudioDoc* doc, QObject* parent = 0, const char* name = 0 );
  ~K3bAudioJobTempData();

  const QString& bufferFileName( int track );
  const QString& bufferFileName( K3bAudioTrack* track );
  
  const QString& infFileName( int track );
  const QString& infFileName( K3bAudioTrack* track );
  
  const QString& tocFileName();

  bool writeInfFiles();
  bool writeTocFile();

  /**
   * This is called internally by writeTocFile but may be used
   * to create the audio part of a mixed toc file
   * @param t the stream to write the data to
   * @param start used for writing on-the-fly: where does the audio data start in stdin
   */
  bool writeAudioTocFilePart( QTextStream& t, const K3b::Msf& start = K3b::Msf(0) );

  void writeAudioTocCdTextHeader( QTextStream& );

  K3bAudioDoc* doc() const;

  /**
   * use this if you want
   * a specific directory
   * it defaults to the default K3b temp dir
   */
  void prepareTempFileNames( const QString& path = QString::null );

  /**
   * remove all temp files (this does not include the audio buffer files
   * since these are not created and thus not handled by the K3bAudioJobTempData)
   */
  void cleanup();

  static QString encodeForTocFile( const QString& str );

 private:
  void writeCdTextEntries( K3bAudioTrack* track, QTextStream& t );

  class Private;
  Private* d;
};

#endif

/***********************************************************************
 **
 **   vlapi2.cpp
 **
 **   This file is part of libkfrgcs.
 **
 ************************************************************************
 **
 **   Copyright (c):  2002 by Garrecht Ingenieurgesellschaft
 **
 **   This file is distributed under the terms of the General Public
 **   Licence. See the file COPYING for more information.
 **
 **   $Id$
 **
 ***********************************************************************/

#include "vlapi2.h"
#include "dbbconv.h"
#include "grecord.h"
#include "utils.h"
#include "Database.hpp"
#include "Device/Port/Port.hpp"
#include "Util.hpp"
#include "Protocol.hpp"
#include "Operation/Operation.hpp"

#include <memory.h>
#include <string.h>
#include <stdlib.h>

// sizes of VL memory regions
const int VLAPI_DBB_MEMSIZE = 16384;
const int32 VLAPI_LOG_MEMSIZE = 81920L;

// ------------------------------------------------------------
//                        VLA_XFR
// ------------------------------------------------------------

// Blockweises Schreiben in den Logger (z.B. Datenbank)
VLA_ERROR
VLA_XFR::dbbput(const void *dbbbuffer, int32 dbbsize)
{
  return Volkslogger::SendCommandWriteBulk(*port, env, Volkslogger::cmd_PDB,
                                           dbbbuffer, dbbsize)
    ? VLA_ERR_NOERR
    : VLA_ERR_MISC;
}

VLA_ERROR
VLA_XFR::dbbget(void *dbbbuffer, int32 dbbsize)
{
  int groesse = Volkslogger::SendCommandReadBulk(*port, env,
                                                 Volkslogger::cmd_RDB,
                                                 dbbbuffer, dbbsize, databaud);
  env.Sleep(300);
  if (groesse <= 0)
    return VLA_ERR_NODATA;
  return VLA_ERR_NOERR;
}

VLA_ERROR
VLA_XFR::readdir(void *buffer, int32 size) {
  if(buffer==0)
    return VLA_ERR_MISC;

  int r = Volkslogger::SendCommandReadBulk(*port, env,
                                           Volkslogger::cmd_DIR,
                                           buffer, size);
  if (r <= 0)
    return VLA_ERR_NOFLIGHTS;
  else
    return VLA_ERR_NOERR;
}

VLA_ERROR
VLA_XFR::all_logsget(void *dbbbuffer, int32 dbbsize)
{
  int groesse = Volkslogger::SendCommandReadBulk(*port, env,
                                                 Volkslogger::cmd_ERO,
                                                 dbbbuffer, dbbsize, databaud);
  env.Sleep(300);
  if (groesse <= 0)
    return VLA_ERR_NODATA;
  return VLA_ERR_NOERR;
}


/*
  Auslesen des Fluges flightnumber im Sicherheitslevel secmode,
  Abspeichern als GCS-Datei im Speicher
*/
long
VLA_XFR::flightget(void *buffer, int32 buffersize,
                   int16 flightnr, int16 secmode)
{
  // read binary flightlog
  const Volkslogger::Command cmd = secmode
    ? Volkslogger::cmd_GFS
    : Volkslogger::cmd_GFL;
  int groesse = Volkslogger::SendCommandReadBulk(*port, env, cmd, flightnr,
                                                 buffer, buffersize, databaud);
  if (groesse <= 0)
    return 0;

  // read signature
  env.Sleep(300);

  int sgr = Volkslogger::SendCommandReadBulk(*port, env, Volkslogger::cmd_SIG,
                                             (uint8_t *)buffer + groesse,
                                             buffersize - groesse,
                                             databaud);
  if (sgr <= 0)
    return 0;

  return groesse + sgr;
}


//
// wait a specified amount of seconds (waittime) for connection of the
// VL to the serial port
//
VLA_ERROR VLA_XFR::connect(int32 waittime) {
  const unsigned timeout_ms = waittime * 1000;
  return Volkslogger::ConnectAndFlush(*port, env, timeout_ms)
    ? VLA_ERR_NOERR
    : VLA_ERR_MISC;
}

VLA_XFR::VLA_XFR(Port &_port, unsigned _databaud, OperationEnvironment &_env)
  :VLA_SYS(_port), env(_env), databaud(_databaud) {
}


VLA_ERROR VLA_XFR::readinfo(void *buffer, int32 buffersize) {
  int nbytes = Volkslogger::SendCommandReadBulk(*port, env,
                                                Volkslogger::cmd_INF,
                                                buffer, buffersize);
  if (nbytes <= 0)
    return VLA_ERR_NODATA;

  return VLA_ERR_NOERR;
}


// ------------------------------------------------------------
//                          VLAPI
// ------------------------------------------------------------

// constructor
VLAPI::VLAPI(Port &_port, unsigned _databaud, OperationEnvironment &_env)
  :VLA_XFR(_port, _databaud, _env) {
}

VLA_ERROR VLAPI::read_info() {
  uint8_t buffer[8];
  VLA_ERROR err;

  if ((err = readinfo(buffer, sizeof(buffer))) == VLA_ERR_NOERR) {
    // Aufbau der Versions- und sonstigen Nummern
    vlinfo.sessionid = 256*buffer[0] + buffer[1];
    vlinfo.vlserno = 256*buffer[2] + buffer[3];
    vlinfo.fwmajor = buffer[4] / 16;
    vlinfo.fwminor = buffer[4] % 16;
    vlinfo.fwbuild = buffer[7];
  }

  return err;
}

// tries a connect without user interaction first.
// if this fails, ask user to connect the VOLKSLOGGER
VLA_ERROR VLAPI::stillconnect() {
  VLA_ERROR err;
  err = connect(4);
  if(err != VLA_ERR_NOERR)
    err = connect(10);
  return err;
}

VLA_ERROR VLAPI::read_db_and_declaration() {
  uint8_t dbbbuffer[VLAPI_DBB_MEMSIZE];
  VLA_ERROR err = stillconnect();
  if(err != VLA_ERR_NOERR)
    return err;

  err = dbbget(dbbbuffer,sizeof(dbbbuffer));
  if(err != VLA_ERR_NOERR)
    return err;

  DBB dbb1;
  memcpy(dbb1.block,dbbbuffer,sizeof(dbb1.block));
  memcpy(dbb1.fdf,dbbbuffer+DBB::FrmBeg,sizeof(dbb1.fdf));

  dbb1.open_dbb();

  database.CopyFrom(dbb1);
  declaration.get(&dbb1);

  return VLA_ERR_NOERR;
}


VLA_ERROR VLAPI::write_db_and_declaration() {

  DBB dbb1;
  database.CopyTo(dbb1);
  declaration.put(&dbb1);

  // copy dbb1 blocks into buffer
  uint8_t dbbbuffer[VLAPI_DBB_MEMSIZE];
  memcpy(dbbbuffer,dbb1.block,sizeof(dbb1.block));
  memcpy(dbbbuffer+DBB::FrmBeg,dbb1.fdf,sizeof(dbb1.fdf));
  // and write buffer back into VOLKSLOGGER
  VLA_ERROR err = stillconnect();
  if(err != VLA_ERR_NOERR)
    return err;
  err = dbbput(dbbbuffer,sizeof(dbbbuffer));
  return err;
}

VLA_ERROR VLAPI::read_directory() {
  VLA_ERROR err = stillconnect();
  if(err != VLA_ERR_NOERR)
    return err;

  uint8_t dirbuffer[VLAPI_LOG_MEMSIZE];
  err = readdir(dirbuffer, sizeof(dirbuffer));

  if(err == VLA_ERR_NOERR) {
    int fcount = conv_dir(0,dirbuffer,1);
    delete[] directory.flights;
    directory.flights = NULL;
    if(fcount>0) {
      directory.nflights = fcount;
      directory.flights = new DIRENTRY[fcount];
      conv_dir(directory.flights,dirbuffer,0);
      err = VLA_ERR_NOERR;
    }
    else {
      directory.nflights = 0;
      err = VLA_ERR_NOFLIGHTS;
    }
  }

  return err;

}

VLA_ERROR
VLAPI::read_igcfile(const char *filename, int index, int secmode)
{
  FILE *outfile = fopen(filename,"wt");
  if(!outfile)
    return VLA_ERR_FILE;

  VLA_ERROR err = stillconnect();
  if(err != VLA_ERR_NOERR) {
    fclose(outfile);
    return err;
  }

  uint8_t logbuffer[VLAPI_LOG_MEMSIZE];
  if (flightget(logbuffer, sizeof(logbuffer), index, secmode)>0)
    err = VLA_ERR_NOERR;

  word serno; long sp;
  long r;
  if(err == VLA_ERR_NOERR) {
    r = convert_gcs(0,outfile,logbuffer,1,&serno,&sp);
    if(r>0) {
      err = VLA_ERR_NOERR;
      print_g_record(
                     outfile,   // output to stdout
                     logbuffer, // binary file is in buffer
                     r          // length of binary file to include
                     );
    }
    else
      err = VLA_ERR_MISC;
  }
  fclose(outfile);
  return err;
}


// getting a waypoint object out of the database memory
//
void VLAPI_DATA::WPT::get(const void *p) {
  const Volkslogger::Waypoint *src = (const Volkslogger::Waypoint *)p;

  memcpy(name, src->name, sizeof(src->name));
  name[sizeof(src->name)] = 0;
  strupr(name);

  typ = (WPTTYP)(src->type_and_longitude_sign & 0x7f);

  uint32_t ll = ((src->latitude[0] & 0x7f) << 16) |
    (src->latitude[1] << 8) | src->latitude[2];
  double lat = ll / 60000.0;
  if (src->latitude[0] & 0x80)
    lat = -lat;
  location.latitude = Angle::Degrees(fixed(lat));

  ll = (src->longitude[0] << 16) |
    (src->longitude[1] << 8) | src->longitude[2];
  double lon = ll / 60000.0;

  if (src->type_and_longitude_sign & 0x80)
    lon = -lon;
  location.longitude = Angle::Degrees(fixed(lon));
}



// putting a waypoint object into the database memory
//
void VLAPI_DATA::WPT::put(void *p) const {
  Volkslogger::Waypoint *dest = (Volkslogger::Waypoint *)p;
  // String, evtl. mit Blanks aufgef�llt, zur�ckschreiben
  CopyPaddedUpper(dest->name, sizeof(dest->name), name);

  // Koordinaten zur�ckschreiben
  uint32_t llat = labs((long)(location.latitude.Degrees() * 60000));
  uint32_t llon = labs((long)(location.longitude.Degrees() * 60000));

  dest->type_and_longitude_sign = (typ & 0x7f);
  if (negative(location.longitude.Native()))
    dest->type_and_longitude_sign |= 0x80;

  dest->latitude[0] = llat >> 16;
  if (negative(location.latitude.Native()))
    dest->latitude[0] |= 0x80;
  dest->latitude[1] = llat >> 8;
  dest->latitude[2] = llat;

  dest->longitude[0] = llon >> 16;
  dest->longitude[1] = llon >> 8;
  dest->longitude[2] = llon;
}

// getting a declaration waypoint object out of the database
//
void VLAPI_DATA::DCLWPT::get(const void *p) {
  const Volkslogger::DeclarationWaypoint *src =
    (const Volkslogger::DeclarationWaypoint *)p;

  WPT::get(src);

  oztyp = (OZTYP)src->oz_shape;
  ws = src->direction * 2;
  if(oztyp == OZTYP_LINE) {
    lw = (src->oz_parameter & 0x0f) * ((src->oz_parameter & 0xf0) >> 4);
  }
  else {
    rz = (src->oz_parameter & 0x0f) * 100;
    rs = ((src->oz_parameter & 0xf0) >> 4) * 1000;
  }
}

// putting a declaration waypoint object into the database memory
//
void VLAPI_DATA::DCLWPT::put(void *p) const {
  Volkslogger::DeclarationWaypoint *dest =
    (Volkslogger::DeclarationWaypoint *)p;

  WPT::put(dest);

  dest->oz_shape = oztyp;
  dest->direction = ws / 2;

  if(oztyp == OZTYP_LINE) {
    // find two integer numbers between 1 and 15 the product of which
    // is just lower or equal the linewidth
    int w1 = 0;
    int w2 = 0;
    for(int i=1;i<=15;i++) {
      if(lw%i==0 && lw/i <= 15) {
        w1 = i;
        w2 = lw/i;
        break;
      }
    }

    dest->oz_parameter = (w1 << 4) + w2;
  }
  else {
    dest->oz_parameter = (rz / 100) + ((rs / 1000 ) << 4);
  }
}

void VLAPI_DATA::ROUTE::get(const void *p) {
  const Volkslogger::Route *src = (const Volkslogger::Route *)p;

  memcpy(name, src->name, sizeof(src->name));
  name[sizeof(src->name)] = 0;
  strupr(name);

  for(int i=0; i<10; i++)
    wpt[i].get(&src->waypoints[i]);
}

void VLAPI_DATA::ROUTE::put(void *p) const {
  Volkslogger::Route *dest = (Volkslogger::Route *)p;

	int i;
  CopyPaddedUpper(dest->name, sizeof(dest->name), name);
  // In the following line, we insertes "int"
  // (Florian Ehinger)
  for(i=0; i<10; i++)
    wpt[i].put(&dest->waypoints[i]);
}

void
VLAPI_DATA::PILOT::get(const void *p)
{
  const Volkslogger::Pilot *src = (const Volkslogger::Pilot *)p;

  memcpy(name, src->name, sizeof(src->name));
  name[sizeof(src->name)] = 0;
  strupr(name);
}

void
VLAPI_DATA::PILOT::put(void * p) const
{
  Volkslogger::Pilot *dest = (Volkslogger::Pilot *)p;

  CopyPaddedUpper(dest->name, sizeof(dest->name), name);
}

void
VLAPI_DATA::DATABASE::CopyFrom(const DBB &dbb)
{
  // convert and write list of waypoints
  if (dbb.header[0].dsfirst != 0xffff) {
    nwpts = 1 + (dbb.header[0].dslast - dbb.header[0].dsfirst)
      / dbb.header[0].dslaenge;
    delete[] wpts;
    wpts = new WPT[nwpts];
    for (int i=0; i<nwpts; i++) {
      wpts[i].get(dbb.block + dbb.header[0].dsfirst +
                           i*dbb.header[0].dslaenge);
    }
  }

  // convert and write list of routes
  if (dbb.header[3].dsfirst != 0xffff) {
    nroutes = 1 + (dbb.header[3].dslast - dbb.header[3].dsfirst)
      / dbb.header[3].dslaenge ;
    delete[] routes;
    routes = new ROUTE[nroutes];
    for (int i=0; i<nroutes; i++) {
      routes[i].get(dbb.block + dbb.header[3].dsfirst +
                             i*dbb.header[3].dslaenge);
    }
  }

  // convert and write list of pilots
  if (dbb.header[1].dsfirst != 0xffff) {
    npilots = 1 + (dbb.header[1].dslast - dbb.header[1].dsfirst)
      / dbb.header[1].dslaenge;
    delete[] pilots;
    pilots = new PILOT[npilots];
    for (int i=0; i<npilots; i++) {
      pilots[i].get(dbb.block + dbb.header[1].dsfirst +
                             i*dbb.header[1].dslaenge);
    }
  }
}

void
VLAPI_DATA::DATABASE::CopyTo(DBB &dbb) const
{
  dbb.open_dbb();

  for (int i = 0; i < nwpts; ++i) {
    uint8_t bwpt[13];
    wpts[i].put(bwpt);
    dbb.add_ds(0, bwpt);
  }
  dbb.close_db(0);

  for (int i = 0; i < npilots; ++i) {
    uint8_t bpilot[17];
    pilots[i].put(bpilot);
    dbb.add_ds(1, bpilot);
  }
  dbb.close_db(1);

  for (int i = 0; i < nroutes; ++i) {
    uint8_t broute[144];
    routes[i].put(broute);
    dbb.add_ds(3, broute);
  }
  dbb.close_db(3);
}

// read flight-declaration fields database into structure
//
void VLAPI_DATA::DECLARATION::get(DBB *dbb) {
  int16 i,p;
  char plt1[17];
  char plt2[17];
  char plt3[17];
  char plt4[17];
  plt1[0] = 0;
  plt2[0] = 0;
  plt3[0] = 0;
  plt4[0] = 0;
  if ((p = dbb->fdf_findfield(FLDPLT1))>=0)
    strncpy(plt1,(char*)(dbb->fdf+p+2),sizeof(plt1));
  if ((p = dbb->fdf_findfield(FLDPLT2))>=0)
    strncpy(plt2,(char*)(dbb->fdf+p+2),sizeof(plt2));
  if ((p = dbb->fdf_findfield(FLDPLT3))>=0)
    strncpy(plt3,(char*)(dbb->fdf+p+2),sizeof(plt3));
  if ((p = dbb->fdf_findfield(FLDPLT4))>=0)
    strncpy(plt4,(char*)(dbb->fdf+p+2),sizeof(plt4));
  flightinfo.pilot[0] = 0;
  strcat(flightinfo.pilot,plt1);
  strcat(flightinfo.pilot,plt2);
  strcat(flightinfo.pilot,plt3);
  strcat(flightinfo.pilot,plt4);
  if ((p = dbb->fdf_findfield(FLDGTY))>=0)
    strncpy(flightinfo.glidertype,(char*)(dbb->fdf+p+2),sizeof(flightinfo.glidertype));
  if ((p = dbb->fdf_findfield(FLDGID))>=0)
    strncpy(flightinfo.gliderid,(char*)(dbb->fdf+p+2),sizeof(flightinfo.gliderid));
  if ((p = dbb->fdf_findfield(FLDCCL))>=0)
    strncpy(flightinfo.competitionclass,(char*)(dbb->fdf+p+2),sizeof(flightinfo.competitionclass));
  if ((p = dbb->fdf_findfield(FLDCID))>=0)
    strncpy(flightinfo.competitionid,(char*)(dbb->fdf+p+2),sizeof(flightinfo.competitionid));
  if ((p = dbb->fdf_findfield(FLDTKF))>=0)
    flightinfo.homepoint.get(dbb->fdf + p + 2);

  if ((p = dbb->fdf_findfield(FLDSTA))>=0)
    task.startpoint.get(dbb->fdf + p + 2);
  if ((p = dbb->fdf_findfield(FLDFIN))>=0)
    task.finishpoint.get(dbb->fdf + p + 2);
  if ((p = dbb->fdf_findfield(FLDNTP))>=0)
    task.nturnpoints = dbb->fdf[p+2];
  for (i=0; i<task.nturnpoints; i++) {
    if ((p = dbb->fdf_findfield(FLDTP1+i))>=0)
      task.turnpoints[i].get(dbb->fdf + p + 2);
  }
}


void VLAPI_DATA::DECLARATION::put(DBB *dbb) {
  strupr(flightinfo.pilot);
  strupr(flightinfo.glidertype);
  strupr(flightinfo.gliderid);
  strupr(flightinfo.competitionclass);
  strupr(flightinfo.competitionid);

  char name[65];
  char name2[17];
  strncpy(name,flightinfo.pilot,sizeof(name));
  int i;
  for(i=0; i<4; i++) {
    strncpy(name2,name+16*i,16);
    name2[16] = 0;
    dbb->add_fdf(i+1,17,name2);
  }

  dbb->add_fdf(FLDGTY, strlen(flightinfo.glidertype)+1, flightinfo.glidertype);
  dbb->add_fdf(FLDGID, strlen(flightinfo.gliderid)+1, flightinfo.gliderid);
  dbb->add_fdf(FLDCCL, strlen(flightinfo.competitionclass)+1,
               flightinfo.competitionclass);
  dbb->add_fdf(FLDCID, strlen(flightinfo.competitionid)+1,
               flightinfo.competitionid);

  uint8_t fdfwpt[16]; // temporary space for data conversions
  flightinfo.homepoint.put(fdfwpt);
  dbb->add_fdf(FLDTKF,sizeof(fdfwpt),&fdfwpt);

  uint8_t ntp = task.nturnpoints;
  dbb->add_fdf(FLDNTP,1,&ntp);

  task.startpoint.put(fdfwpt);
  dbb->add_fdf(FLDSTA,sizeof(fdfwpt),&fdfwpt);

  task.finishpoint.put(fdfwpt);
  dbb->add_fdf(FLDFIN,sizeof(fdfwpt),&fdfwpt);

  for (i=0; i<task.nturnpoints; i++) {
    task.turnpoints[i].put(fdfwpt);
    dbb->add_fdf(FLDTP1+i,sizeof(fdfwpt),&fdfwpt);
  }
}

//============================================================================
//
//   SSSS    tt          lll  lll
//  SS  SS   tt           ll   ll
//  SS     tttttt  eeee   ll   ll   aaaa
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2023 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//============================================================================

#include <map>

#include "bspf.hxx"
#include "FSNode.hxx"
#include "Logger.hxx"
#include "DefProps.hxx"
#include "Props.hxx"
#include "PropsSet.hxx"
#include "repository/CompositeKeyValueRepositoryNoop.hxx"
#include "repository/KeyValueRepositoryPropertyFile.hxx"

#include "StellaLIBRETRO.hxx"

extern StellaLIBRETRO stella;
extern const char * rom_data;
extern bool force_pal;
extern bool force_ntsc;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
PropertiesSet::PropertiesSet()
  : myRepository{make_shared<CompositeKeyValueRepositoryNoop>()}
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void PropertiesSet::setRepository(shared_ptr<CompositeKeyValueRepository> repository)
{
  myRepository = std::move(repository);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool PropertiesSet::getMD5(string_view md5, Properties& properties,
                           bool useDefaults) const
{

  properties.setDefaults();
  bool found = false;

  // There are three lists to search when looking for a properties entry,
  // which must be done in the following order
  // If 'useDefaults' is specified, only use the built-in list
  //
  //  'save': entries previously inserted that are saved on program exit
  //  'temp': entries previously inserted that are discarded
  //  'builtin': the defaults compiled into the program

  // First check properties from external file
  if(!useDefaults)
  {
    if (myRepository->has(md5)) {
      properties.load(*myRepository->get(md5));

      found = true;
    }
    else  // Search temp list
    {
      const auto tmp = myTempProps.find(md5);
      if(tmp != myTempProps.end())
      {
        properties = tmp->second;
        found = true;
      }
    }
  }

  // Otherwise, search the internal database using binary search
  if(!found)
  {
    int low = 0, high = DEF_PROPS_SIZE - 1;
    while(low <= high)
    {
      const int i = (low + high) / 2;
      const int cmp = BSPF::compareIgnoreCase(md5,
          DefProps[i][static_cast<uInt8>(PropType::Cart_MD5)]);

      if(cmp == 0)  // found it
      {
        for(uInt8 p = 0; p < static_cast<uInt8>(PropType::NumTypes); ++p)
          if(DefProps[i][p][0] != 0)
            properties.set(PropType{p}, DefProps[i][p]);

        found = true;
        break;
      }
      else if(cmp < 0)
        high = i - 1; // look at lower range
      else
        low = i + 1;  // look at upper range
    }
  }

  properties.printHeader();

  // Turbo (Arcade)
  if (md5 == "f6d30309badadcd56799bcad239fb200") {
    properties.set(PropType::Cart_Type, "cdf");
  // Pick 'n Pile (PAL)
  } else if (md5 == "da79aad11572c80a96e261e4ac6392d0") {
    force_pal = true;
  // Moonsweeper (PAL)
  } else if (md5 == "dbfb9e03cd578dd4fd7bda2bb4f9fb16") {
    force_pal = true;
  // Time Pilot (b1)
  } else if (md5 == "4e99ebd65a967cabf350db54405d577c") {
    uInt8* rom = (uInt8*)stella.getROM();
    uInt8* newRom = (uInt8*)malloc(8 * 1024);
    int offset = 4 * 1024;
    for (int i = 0; i < offset; i++) {
      newRom[i] = rom[offset + i];
      newRom[offset + i] = rom[i];
    }
    memcpy((unsigned char*)rom_data, newRom, 8 * 1024);
  // Tac-Scan
  } else if (md5 == "d45ebf130ed9070ea8ebd56176e48a38") {
    properties.set(PropType::Console_SwapPorts, "YES");
    properties.set(PropType::Controller_SwapPaddles, "YES");
    properties.set(PropType::Controller_Right, "PADDLES");
  // Spike's Peak (PAL)
  } else if (md5 == "9bb136b62521c67ac893213e01dd338f") {
    properties.set(PropType::Controller_Left, "JOYSTICK");
    properties.set(PropType::Controller_Right, "JOYSTICK");
  // Spike's Peak (NTSC)
  } else if (md5 == "a4e885726af9d97b12bb5a36792eab63") {
    properties.set(PropType::Controller_Left, "JOYSTICK");
    properties.set(PropType::Controller_Right, "JOYSTICK");
  // Brik180
  } else if (md5 == "576ec3587aafd9b068a1d955a8aa00cf") {
    properties.set(PropType::Controller_Left, "JOYSTICK");
    properties.set(PropType::Controller_Right, "PADDLES");
  } else if (md5 == "0ac295096eb96229a5a757bcb8d5a66b") {
    properties.set(PropType::Controller_Left, "JOYSTICK");
    properties.set(PropType::Controller_Right, "PADDLES");
  // Maze Craze
  } else if (md5 == "f825c538481f9a7a46d1e9bc06200aaf") {
    force_ntsc = true;
  // Polaris (PAL)
  } else if (md5 == "203049f4d8290bb4521cc4402415e737") {
    uInt8* rom = (uInt8*)stella.getROM();
    memcpy((unsigned char*)rom_data, rom, 8 * 1024);
    unsigned char * p = (unsigned char*)&rom_data[0x140];
    (*p++) = 0xd8;
    (*p++) = 0xe8;
    (*p++) = 0x86;
    (*p++) = 0x5b;
    (*p++) = 0x86;
    (*p++) = 0x5c;
    (*p++) = 0x86;
  }

  properties.print();

  return found;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void PropertiesSet::insert(const Properties& properties, bool save)
{
  // TODO
  // Note that the following code is optimized for insertion when an item
  // doesn't already exist, and when the external properties file is
  // relatively small (which is the case with current versions of Stella,
  // as the properties are built-in)
  // If an item does exist, it will be removed and insertion done again
  // This shouldn't be a speed issue, as insertions will only fail with
  // duplicates when you're changing the current ROM properties, which
  // most people tend not to do

  // Since the PropSet is keyed by md5, we can't insert without a valid one
  const string& md5 = properties.get(PropType::Cart_MD5);
  if(md5.empty())
    return;

  // Make sure the exact entry isn't already in any list
  Properties defaultProps;
  if(getMD5(md5, defaultProps, false) && defaultProps == properties)
    return;
  else if(getMD5(md5, defaultProps, true) && defaultProps == properties)
  {
    cerr << "DELETE" << endl << std::flush;
    myRepository->remove(md5);
    return;
  }

  if (save) {
    properties.save(*myRepository->get(md5));
  } else {
    const auto ret = myTempProps.emplace(md5, properties);
    if(!ret.second)
    {
      // Remove old item and insert again
      myTempProps.erase(ret.first);
      myTempProps.emplace(md5, properties);
    }
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void PropertiesSet::loadPerROM(const FSNode& rom, string_view md5)
{
  Properties props;

  // Handle ROM properties, do some error checking
  // Only add to the database when necessary
  bool toInsert = false;

#if 0 // raz
  // First, does this ROM have a per-ROM properties entry?
  // If so, load it into the database
  const FSNode propsNode(rom.getPathWithExt(".pro"));
  if (propsNode.exists()) {
    KeyValueRepositoryPropertyFile repo(propsNode);
    props.load(repo);

    insert(props, false);
  }
#endif

  // Next, make sure we have a valid md5 and name
  if(!getMD5(md5, props))
  {
    props.set(PropType::Cart_MD5, md5);
    toInsert = true;
  }

  if(toInsert || props.get(PropType::Cart_Name) == EmptyString)
  {
    props.set(PropType::Cart_Name, rom.getNameWithExt(""));
    toInsert = true;
  }

  // Finally, insert properties if any info was missing
  if(toInsert)
    insert(props, false);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void PropertiesSet::print() const
{
  // We only look at the external properties and the built-in ones;
  // the temp properties are ignored
  // Also, any properties entries in the external file override the built-in
  // ones
  // The easiest way to merge the lists is to create another temporary one
  // This isn't fast, but I suspect this method isn't used too often (or at all)

  // First insert all external props
  PropsList list = myExternalProps;

  // Now insert all the built-in ones
  // Note that if we try to insert a duplicate, the insertion will fail
  // This is fine, since a duplicate in the built-in list means it should
  // be overrided anyway (and insertion shouldn't be done)
  Properties properties;
  for(uInt32 i = 0; i < DEF_PROPS_SIZE; ++i)
  {
    properties.setDefaults();
    for(uInt8 p = 0; p < static_cast<uInt8>(PropType::NumTypes); ++p)
      if(DefProps[i][p][0] != 0)
        properties.set(PropType{p}, DefProps[i][p]);

    list.emplace(DefProps[i][static_cast<uInt8>(PropType::Cart_MD5)], properties);
  }

  // Now, print the resulting list
  Properties::printHeader();
  for(const auto& i: list)
    i.second.print();
}

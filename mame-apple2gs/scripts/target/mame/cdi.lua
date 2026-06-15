-- license:BSD-3-Clause
-- copyright-holders:MAMEdev Team

---------------------------------------------------------------------------
--
--   cdi.lua
--
--   Minimal driver-specific makefile for Philips CD-i
--
---------------------------------------------------------------------------


--------------------------------------------------
-- Specify all the CPU cores necessary for the
-- drivers referenced in cdi.
--------------------------------------------------

CPUS["M680X0"] = true
CPUS["M6805"]  = true
CPUS["MCS51"]  = true


--------------------------------------------------
-- Specify all the sound cores necessary for the
-- drivers referenced in cdi.
--------------------------------------------------

SOUNDS["CDDA"]   = true
SOUNDS["DMADAC"] = true


--------------------------------------------------
-- specify available video cores
--------------------------------------------------


--------------------------------------------------
-- specify available machine cores
--------------------------------------------------

MACHINES["SCC68070"] = true
MACHINES["TIMEKPR"]  = true


--------------------------------------------------
-- specify available bus cores
--------------------------------------------------


--------------------------------------------------
-- This is the list of files that are necessary
-- for building all of the drivers referenced
-- in cdi.
--------------------------------------------------

function createProjects_mame_cdi(_target, _subtarget)
	project ("mame_cdi")
	targetsubdir(_target .."_" .. _subtarget)
	kind (LIBTYPE)
	uuid (os.uuid("drv-mame-cdi"))
	addprojectflags()
	precompiledheaders_novs()

	includedirs {
		MAME_DIR .. "src/osd",
		MAME_DIR .. "src/emu",
		MAME_DIR .. "src/devices",
		MAME_DIR .. "src/mame/shared",
		MAME_DIR .. "src/lib",
		MAME_DIR .. "src/lib/util",
		MAME_DIR .. "3rdparty",
		GEN_DIR  .. "mame/layout",
	}

files{
	MAME_DIR .. "src/mame/philips/cdi.cpp",
	MAME_DIR .. "src/mame/philips/cdi.h",
	MAME_DIR .. "src/mame/philips/cdicdic.cpp",
	MAME_DIR .. "src/mame/philips/cdicdic.h",
	MAME_DIR .. "src/mame/philips/cdislavehle.cpp",
	MAME_DIR .. "src/mame/philips/cdislavehle.h",
	MAME_DIR .. "src/mame/philips/mcd212.cpp",
	MAME_DIR .. "src/mame/philips/mcd212.h",
}
end

function linkProjects_mame_cdi(_target, _subtarget)
	links {
		"mame_cdi",
	}
end

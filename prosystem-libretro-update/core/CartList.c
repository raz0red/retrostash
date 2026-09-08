#include <string.h>
#include "ProSystem.h"
#include "CartList.h"

const struct cart cart_list[] =
{
    // 10 Print Maze Demo (20170826) (222089B9).a78
    {
        "b57c10dc366511916afe6af45350ea5c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // 1E78 Demo (PAL) (20190420) (DA69EACE).a78
    {
        "bf42861845fcef7392970012470fbd3f",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // 320BI Test (20221005) (2D97E9EF).a78
    {
        "362c9452bf696806761d4183e151a7aa",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // 320C Sprite Editor - Viewer Demo (PAL) (20230612) (F9EE245E).a78
    {
        "4395d150d0d5b778b644af692f6d885a",
        0, // cart type	0
        4, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // 32k RAM Test Demo (20230123) (7FADF2D1).a78
    {
        "f1a12906b75f1eaa20a1e16c9f2bd0c2",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // 3D Test Demo 1 (PAL) (20180102) (443932F9).a78
    {
        "e547492ebd342e57c28bb235546da299",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // 3D Worldrunner Theme Melody (4000) (20190903) (5297CA50).a78
    {
        "0d61360b0f75e91b11451f4f1dea3a2f",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // 3D Worldrunner Theme Melody (450) (20190903) (929654B0).a78
    {
        "19a9b4f27dcbfb94fee5d751b83d9acb",
        0, // cart type	0
        64, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // 633 Squadron Movie Theme (YM) (20221001) (7C1E630B).a78
    {
        "71d128b626b92e345e196d59a0be3e0f",
        8, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // 7800 Non-Interactive Title Screen (20210320) (B733E025).a78
    {
        "452a7de378858aa165bc578c0d2f804c",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // 9 Colour Demo (320C) (PAL) (20230618) (DEDB38AB).a78
    {
        "dae6f894727864002ffd316b2f513c32",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // A Warm Welcome (POKEY Music Demo) (20130612) (3B6B4235).a78
    {
        "5ec758a4d37a0a4cfe5f83def30d041f",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Acidjazzed (Dual POKEY 440 450 Demo) (20210117) (D728DD85).a78
    {
        "49769fc164e4c9a22f8e6688f9150361",
        4, // cart type	0
        64, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Adventure Map Demo (20141230) (DC531FB6).a78
    {
        "05b04e9822a75ceeeaa2eb106ffe768e",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Adventureland (Mini Demo) (20230424) (9B8FF967).a78
    {
        "fec2b82d1dd4d543638d9057431c5e4a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Adventurer (Sample) (20140301) (60E657D0).a78
    {
        "31a0b26699b7ee195fa0b47473c7d29d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Adventurer Scroll Demo (20150307) (A38817D0).a78
    {
        "502775a8f47ebbcca8916663a569c029",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Adventurer Scroll Demo (v2) (20150312) (50C2839A).a78
    {
        "0dca2185f8f16f56353cef6141265026",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Aim (Demo) (20150731) (D364D19C).a78
    {
        "69167cda674aa1d7660d0c89d5fd0fdb",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Al Capone (YM2151 Demo) (20211201) (7E7D44AB).a78
    {
        "a7abac1ca9ec493663e61f154ff6335a",
        8, // cart type	0
        8, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Allura (Picture Demo) (20170126) (CFFE9790).a78
    {
        "ab6535eee446045a70a6ab42aad32861",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Altered Beast - Title and Sprite Demo (20220222) (CABFE9EA).a78
    {
        "9c2dcd4d0a85fadf2d094c8f6b853e19",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Animated Moving Demo 24 (20171127) (782A7A8B).a78
    {
        "cf64383c818aefbfe9eecfa71fef0d87",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Animated Moving Demo 32 (20171127) (ACD1295C).a78
    {
        "40b806ce570ac062f6077991a0f371c8",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Arkanoid (Level Start Music Demo) (4000) (20190628) (3F237759).a78
    {
        "48575576f79e3a307dfbbdb44dae652a",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Arkanoid (Title Music Demo) (4000) (20190628) (DC60B513).a78
    {
        "e3a43f25b23c98322b75ce13acb8fb6a",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Arkanoid 2 DOH Theme Music (4000) (20200831) (EAB0A4EF).a78
    {
        "79bf3a831562a2fdaa50ef4c9f62fb8e",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Arkanoid 2 DOH Theme Music (450) (20200831) (C41D18DD).a78
    {
        "deb4744d99e3ef57af0179f2e5ad2415",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Arkanoid 2 Intro Music (4000) (20200830) (D0C49C3E).a78
    {
        "45fb9f73d10d7ef389dd18cdceea6c7e",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Arkanoid 2 Intro Music (450) (20200830) (7C91B34E).a78
    {
        "80f509d2d3c595a300b7ac8a0e51e12a",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Arkanoid 2 Level Start Sound Demo (4000) (20200831) (4D1310AF).a78
    {
        "0bd4e64b58d950d54f8e377c1e2a2c37",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Arkanoid 2 Level Start Sound Demo (450) (20200831) (4FA6E337).a78
    {
        "99fa586e07021e9b3348ed2880dc1646",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Arkanoid Background 3 Colors Demo (20190714) (1DFB4BA6).a78
    {
        "55a97e99e19e11e54296ffcca2994b30",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Art Of Scrolling Without Scrolling, The (Demo) (20200603) (4065BA2D).a78
    {
        "cb9cc36255a3cf80da0396a94e714d94",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Asuka Sky (Picture Demo) (20170126) (C9E2EB42).a78
    {
        "9a5451e0e67526902f76c32cd5764c33",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Asuka Surf (Picture Demo) (20170126) (3123488D).a78
    {
        "0b180873142fd08b08c2c50e4ea308e2",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Atari 7800 ProSystem Memo Pad (20210401) (28C437C4).a78
    {
        "bb265aa233b97568f0f1e878dd817695",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Atari Fuji Logo Demo (20220222) (025DEBBA).a78
    {
        "9d49c878e8d7fd4542db8d2b8f539bac",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // AtariAge Jingle (4000) (20190708) (528387A4).a78
    {
        "0f77b6e4e18253ae5036ecd09a444320",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // AtariAge Jingle (v2) (4000) (20190708) (AF25F578).a78
    {
        "36ac21405311b75ca9f2a15fcafed78d",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Bad Apple Demo (PAL) (20221213) (D477F043).a78
    {
        "38abb96aef01b45eb3ce74a91e2908ac",
        8, // cart type	0
        6, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        1  // xm
    },
    // Bankset Test - 2x128K (20220321) (B055CB30).a78
    {
        "2a7bb2b113c828344632fb27c522bacd",
        32, // cart type	0
        18, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Bankset Test - 2x128K RAM (20220321) (2D33D09F).a78
    {
        "94d57e3f44b9288b53b586c0f8b664fa",
        96, // cart type	0
        2, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Bankset Test - 2x128K RAM Pokey800 (20220321) (141B631A).a78
    {
        "85f986155bfe6677a85a2bb1329f193f",
        224, // cart type	0
        2, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Bankset Test - 2x32K (20220321) (34485781).a78
    {
        "76172b03a17af0dc88498042877656fd",
        32, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Bankset Test - 2x32K Pokey4000 (20220321) (8D07CF6A).a78
    {
        "d231c51e8915287f34c59385f150b84f",
        32, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Bankset Test - 2x32K RAM Pokey800 (20220321) (AE3CACAE).a78
    {
        "5dadf198c8eac8131d4b256e50ff5bc4",
        224, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Bankset Test - 2x48K (20220321) (1FD91BB3).a78
    {
        "8ebd74e4fab61e9256e2208a558b3895",
        32, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Bankset Test - 2x48K Pokey4000 (20220321) (C7603522).a78
    {
        "ff3bc72ca4086065e92408f18986a169",
        32, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Bankset Test - 2x52K (20220321) (B5DC83E0).a78
    {
        "6c36bfdb98759e14b5b62675a746c3bb",
        32, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Bankset Test - 2x52K Pokey4000 (20220321) (98509ECD).a78
    {
        "f569abef972929404893c2461f811c15",
        32, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Batari Basic Demo Conversion (20210409) (32867785).a78
    {
        "5eda75d54e9aa92992513d9e691d5a5c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Batman (Picture Demo) (20170126) (FAECE01B).a78
    {
        "70fd7820c63d98d84e5ae18917c6d568",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Battlezone (NTSC) (20160208) (440680AD).a78
    {
        "a8458c510fdd71a1f9cc4c0b243b177a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Battlezone (PAL) (20160208) (D63F3EE7).a78
    {
        "42fa4bd854a2813b19099da524461a64",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Beach Adventure (20220802) (0FBDC8C9).a78
    {
        "3c15686e7634aba1da67a1af7d4d14dd",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Bear Arms (Alpha) (20140523) (5409B9DA).a78
    {
        "c359037e2a32b6bed329adeb0159cff7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Bee Moved (Covox Music Demo) (31440Hz) (PAL) (20210410) (C15E4AB7).a78
    {
        "65fd3ebcbc04c90f0ef8fc67673d29fc",
        0, // cart type	0
        2, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Bee Moved (Covox Music Demo) (94320Hz) (PAL) (20210411) (7E2D9505).a78
    {
        "2a0ff2cb23f08e73a2fe043331dc308f",
        0, // cart type	0
        2, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Bionic Commando - The Forest (YM2151 Demo) (PAL) (20211031) (240CB017).a78
    {
        "3dd687f1e9cba5769972c229effddf34",
        8, // cart type	0
        8, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Bitmap Template Demo (V2) (20220123) (D7FA6A8B).a78
    {
        "b4c453de16bdd56d3022c0e2dee71aa2",
        0, // cart type	0
        4, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Black Lamp Music Demo (4000) (20210318) (5B0532F3).a78
    {
        "04763ab985aac23bc35e0d4e94543db9",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Black Lamp Music Demo (800) (20220821) (042A7AC5).a78
    {
        "2832642b6795d7236d6fc5b94daaed06",
        128, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Blocky Clouds Scroll Demo (20030507) (CA7D2C33).a78
    {
        "ff97231fc5a9d33ab37bee1f8b7e6ad2",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Bloodfighter (Covox Music Demo) (PAL) (20210404) (BEC80401).a78
    {
        "0c818254d9bf6eb0cd16c9377e2c911b",
        0, // cart type	0
        72, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Bomb Jack Sound Demo (POKEY 450) (20210201) (9D7CA714).a78
    {
        "b8eca598d6ce771fa00c27ca48c4c96e",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Bouncing Balls (Demo) (20070330) (813DB2BE).a78
    {
        "56cd3b40ceb8f82e45c8fb60ccb40256",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Bubble Bobble Theme (Bass Only Demo) (4000) (20190903) (041AE140).a78
    {
        "0606dc99a9c39eec12b434b150129afd",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Bubble Bobble Theme Sound Demo (4000) (20190724) (6F12EB92).a78
    {
        "5ee92463a67e4f3e88d88f3ba7f20831",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Bubble Bobble Theme Sound Demo (450) (20190819) (28BA847D).a78
    {
        "1e942d29909025ee9324aa5944afe709",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Buttons (Picture Demo) (20170126) (D23E364C).a78
    {
        "18fa4322c30a5246a6da921ee0c02734",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Byzex (Dual POKEY 440 450 Demo) (20210120) (362C673A).a78
    {
        "80883ab80e676219c32145a903e56040",
        4, // cart type	0
        64, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Call Of Duty Black Ops - Cold War (AtariVox Demo) (20210112) (763858DB).a78
    {
        "86fa4d2d6b31d33cb2b2928ec77f03b3",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        2, // save device
        0  // xm
    },
    // Camouflage - The Great Commandment (Music Demo) (4000) (NTSC) (20210314) (FF5216BF).a78
    {
        "40032865050c7f67e0415acea6fa869e",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Camouflage - The Great Commandment (Music Demo) (4000) (PAL) (20210314) (C2F90D02).a78
    {
        "5c0b688c574df5a0823264ae89ab5f0f",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Cannon Fodder GFX Demo (320C) (PAL) (20230617) (572D9695).a78
    {
        "a457d2264aa0fa5094ed439127cb0e19",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Capcom Vs SNK (Picture Demo) (20170121) (8FDC2DC8).a78
    {
        "e1faa1765c795dda776aaec395e8fc9d",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Cheetahmen, The (YM2151 Demo) (20211201) (37FCFEF9).a78
    {
        "516bbcf7e40f5e0d534036ce82b3305c",
        8, // cart type	0
        8, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Cloak and Dagger - Title Screen Demo (Original Logo) (20220509) (5164CCF5).a78
    {
        "b081027e39a0e8da172e74308bc9854a",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Cloak and Dagger - Title Screen Demo (Smooth Logo) (20220509) (5C63EAAB).a78
    {
        "f7829750bf9c3585fabe248889372874",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Color Demo 1 (20181222) (7AC33FE3).a78
    {
        "c87346075580c2ccf0520a5fa564dcd4",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Color Gradients WSYNC (Demo) (20220221) (709AD71F).a78
    {
        "6cfbfebb732434b48b8afa2e345912d3",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Color Gradients WSYNC Fullscreen (Demo) (20220221) (ED39E069).a78
    {
        "9f10927d57083c5eb4e5fae9d98faf02",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Commando Scroll - Copy Screen (20220716) (DF2B5275).a78
    {
        "95c433ec0010ae3af17ee0eab1708133",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Commando Scroll - Wrapped (20220717) (B37A2569).a78
    {
        "5da2a0d465b9292cbead7d1d763b843a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Congo Bongo (Graphics Demo) (20210408) (5E831621).a78
    {
        "c033417023b373a0647fec451c32761d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Cybernoid II (Music Demo) (POKEY 4000) (20210116) (AB5C4613).a78
    {
        "2b3d47ab264f7d45adbbdac629625da6",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Cybernoid II (Music Demo) (POKEY 450) (20210116) (758B9D67).a78
    {
        "a944729fd19d2dfb640a6f1018923191",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Dark Tower (Demo) (20130506) (2CA8BBD5).a78
    {
        "8afb6509645ffdf2da9a14a78bd6ded7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Deep Purple - Smoke On The Water (Music Demo) (4000) (20200317) (95FEBC89).a78
    {
        "d75a6ba5ed712bae61350df6793b351b",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Deep Purple - Smoke On The Water (Music Demo) (450) (20200317) (3957160C).a78
    {
        "590f1d5dc2a36cc6a7a55d789d8c534e",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Demo X (v1_01F) (20100423) (FF567110).a78
    {
        "696326787655b262d27ec534d23370b3",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Dig Dug II (20211219) (62377D74).a78
    {
        "7f4309081bd456f2fbd520b0f9b2330e",
        0, // cart type	0
        64, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Direct Mode Scroll (20220807) (A3114BE7).a78
    {
        "360496bc946bb5f4c9255b61f062b54d",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Direct Mode Scroll with Sprites (20220808) (92697EDB).a78
    {
        "1b8081c7b527267325f74eea23bcb942",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Direct Mode Vertical Scroll (20220810) (F213B788).a78
    {
        "e2b924a480c8f67e7171f6c2f28cb8c6",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Direct Mode Vertical Scroll (Joystick and Collisions) (20220811) (6EDADCF8).a78
    {
        "70ead07c9e65a54f656b28e0d49dd6e3",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Doodle Chaos Engine (20200622) (BAB8ECE2).a78
    {
        "55d6c883e4a7c6ec0df83344542957ae",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Double Buffer Demo (20170415) (77F0272F).a78
    {
        "fbfd82392bc459cf62c2e3b8770779e2",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Double Dragon (Graphics Demo) (20170121) (C32A4147).a78
    {
        "57ed3d6aa8cfdde5ece77d6211ad1d06",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Double Dragon (Title Picture Demo) (20170121) (A0F0417E).a78
    {
        "2c260c9bb6dc243d2a07559f78cc493f",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Double Dragon (Walking Demo) (20170130) (D3FDBC2D).a78
    {
        "b89cef619b13c24a9aa4fc94443371b3",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Dragon's Verse (Scroll Prototype) (20230201) (E5799329).a78
    {
        "fa694a9fc85f10d2e6ffe1b813f93892",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Dragonfly (Graphics Demo) (Test 2) (20210331) (8B317DB1).a78
    {
        "847c14de1c00b5f577536021b0d322bf",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // E X O Band - Area 1 Prison (450) (20210303) (1A6F998A).a78
    {
        "a789faefea6ab48475577abda10abb9d",
        0, // cart type	0
        70, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Elysium (Covox Music Demo) (7720Hz) (PAL) (20210411) (A64FB41E).a78
    {
        "268c0573d13575b89db49b8d0e7b5747",
        0, // cart type	0
        2, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Endless Runner (Demo 1) (20210814) (333E1865).a78
    {
        "aaa6e977fec6b537f6071a62b69c996b",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Endless Runner (Demo 2) (20210814) (AAF31717).a78
    {
        "1030a41e0ab2cc0f908e472fe3b57126",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Enterprise (Picture Demo) (20170126) (794E5319).a78
    {
        "2a6b2424b7d18dd208ede8bff6bcd8cf",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Fader (20220829) (2EDE4E4E).a78
    {
        "7ffe1161dd18ee2614a545493cc28a66",
        0, // cart type	0
        128, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Fader (PAL) (20220830) (050A9489).a78
    {
        "42207b65376c80c0bbe578c842bc01da",
        0, // cart type	0
        128, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Falling (Demo) (20091107) (C383CBBE).a78
    {
        "0d34b6a8ad428bf95997cd03b95a7d0a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Fantasy Zone - Dreaming Tomorrow (YM2151 Demo) (PAL) (20211031) (85929B46).a78
    {
        "d25363e6ff76f7e9e09ebd1c130ea9dd",
        8, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Flimbo's Quest Sound Demo (POKEY 450) (20210201) (FDFA4EC4).a78
    {
        "75cd5ea3b9a12fcab7e4031da8c536a0",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Frankie Goes To Hollywood - Relax (Music Demo) (20150311) (602334F5).a78
    {
        "a1502d6ff0e0a770eb6397102eadb161",
        0, // cart type	0
        2, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Frogger (Demo) (20091107) (B857AE70).a78
    {
        "dc90623e85fa481724c05f32c92d056f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // G2F Slideshow (Demo) (v1_00F) (20100206) (A02E18AC).a78
    {
        "bcb0c55f47549c2b13796fb7eed06959",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Galaxian Alien Death 1 Sound (POKEY 4000) (20210118) (887F8019).a78
    {
        "8edba95b36c905522f037d960a01d50f",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Alien Death 1 Sound (POKEY 450) (20210118) (FA8B8D29).a78
    {
        "462bf457dbd2d5a3f6464b01fc32448f",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Alien Death 1 Sound 16bit (POKEY 4000) (20210118) (D2E98A81).a78
    {
        "662401b1fc25f97f7f38ccf5bf2e5cfb",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Alien Death 1 Sound 16bit (POKEY 450) (20210118) (803BDD59).a78
    {
        "bbb5baf928438851863ea6541cebd08e",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Alien Death 2 Sound (POKEY 4000) (20210118) (F94A9ADA).a78
    {
        "1199427ed852f463c82ac30ae98d07e8",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Alien Death 2 Sound (POKEY 450) (20210118) (C8ABE86D).a78
    {
        "721eed5abda061bd4708f42ef89f1fc3",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Alien Death 2 Sound 16bit (POKEY 4000) (20210118) (EB70B1C7).a78
    {
        "8665bd80f27b85ab6d6e861c8a37a858",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Galaxian Alien Death 2 Sound 16bit (POKEY 450) (20210118) (39E3FB37).a78
    {
        "365edbafdeb5ad2be4d5f540264eba17",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Alien Death 2a Sound (POKEY 4000) (20210118) (C4B9D3E8).a78
    {
        "9750a8bf627337b0e6d2c5c34eb54fe7",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Alien Death 2a Sound (POKEY 450) (20210118) (8DDF74E8).a78
    {
        "c21e6a429e39d932b6e1b72656ec3127",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Coin Sound (POKEY 4000) (20210118) (9BE5110D).a78
    {
        "d6cd69cec6b84cfdb76e316117601688",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Coin Sound (POKEY 450) (20210118) (5B001AD9).a78
    {
        "319c7ec16ba033eb5000778455dba52b",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Endswoop Sound (POKEY 4000) (20210118) (B20B2DD9).a78
    {
        "9eda80b9c3b28128a3980621f8eac75e",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Endswoop Sound (POKEY 450) (20210118) (9D544BDB).a78
    {
        "7fcf91f03e4fc9e5b776897eb2aae37c",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Explosion Sound (POKEY 4000) (20210118) (F47A04D5).a78
    {
        "69b8c60a9deebf6c739190a1dd53e05b",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Explosion Sound (POKEY 450) (20210118) (5074FD09).a78
    {
        "2a218d8d8aca5ae0fba6b3f24cea83eb",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Extra Life Sound (POKEY 4000) (20210118) (E98C61A2).a78
    {
        "599f8c57ec7404e1f18f97eca23602b6",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Extra Life Sound (POKEY 450) (20210118) (74248366).a78
    {
        "f85f3c847532e4d8f792b668b48cf3f6",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Hum 1a Sound (POKEY 4000) (20210118) (3BB3E8BD).a78
    {
        "ea74f9d71015364075b303331986819e",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Hum 1a Sound (POKEY 450) (20210118) (B4C80069).a78
    {
        "98c92a4082431e96d08305c53c830eef",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Hum 2a Sound (POKEY 4000) (20210118) (2A0241E6).a78
    {
        "c6896c25582af2cfc249101aa29bc3ee",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Hum 2a Sound (POKEY 450) (20210118) (72AF6EF3).a78
    {
        "349be109f3deb07ddf01a28fe4b96da8",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Hum 3a Sound (POKEY 4000) (20210118) (9396AD44).a78
    {
        "ebd37b2178150d2d180c2eea84342631",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Hum 3a Sound (POKEY 450) (20210118) (C307F4B1).a78
    {
        "20511a8cfd998305c75eb5810b9df186",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Hum 4a Sound (POKEY 4000) (20210118) (99DB4025).a78
    {
        "043b92bbf78af139d067d51b9cb5d8d9",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Hum 4a Sound (POKEY 450) (20210118) (B7546DFF).a78
    {
        "94e3b549261cc474b88943019248af9d",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Hum 5a Sound (POKEY 4000) (20210118) (F31068FD).a78
    {
        "d577280d41e5769c83c96ceea5231e83",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Hum 5a Sound (POKEY 450) (20210118) (1A423698).a78
    {
        "9534d9d21700ecc12cf592b38f94d75a",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Hum 6a Sound (POKEY 4000) (20210118) (3A539FBB).a78
    {
        "1506c4c76f4c9a522f6cb3202ba60cf1",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Hum 6a Sound (POKEY 450) (20210118) (411585D9).a78
    {
        "703b96b6ca548f9e9937fd4deeebdefa",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Shot Sound (POKEY 4000) (20210118) (15F3E2F2).a78
    {
        "dee887d22f3555e62758d07b371a229d",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Shot Sound (POKEY 450) (20210118) (73583CF8).a78
    {
        "f8382b15ec3c709bc5b2d71275410807",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Start Sound (POKEY 4000) (20210118) (AA81915E).a78
    {
        "da0ce6102c43c26eb7f6d76a1d34c0f9",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Start Sound (POKEY 450) (20210118) (DD0B018A).a78
    {
        "154970232542e6bf4f6047c42e4401eb",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Swoop Sound (POKEY 4000) (20210119) (78B00AB2).a78
    {
        "4363e0bedea1e8b0126650fd353f12eb",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Galaxian Swoop Sound (POKEY 450) (20210119) (46198EC6).a78
    {
        "671157863510679d833bd79adbb08a96",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Gary Denise Music Demo (POKEY 450) (20210123) (B73997A7).a78
    {
        "137024440fdfad37d2cedbb3c3e50435",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ghosts'n Goblins (320B PlotMapFile Title Demo) (20170821) (1AB65445).a78
    {
        "28b26c7f42c79e35f428992cda1180d5",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // GODS Title Demo (20200809) (6DC37AA9).a78
    {
        "c8f96af9446825083728b1cde51ada15",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Good Dino (Multicolor 160B Plus Mode Demo) (20210221) (03C30945).a78
    {
        "1ab299dc6480ed1922709d10b8e357fc",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Graphic Demo (Ecernosoft) (20220905) (C39D21FE).a78
    {
        "2d595c5e3aee4d58a35422ae5972f836",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Graphics Demo 1 (20181222) (3BCFB93E).a78
    {
        "db2eb4ac596bc8389bc210c38d6e5907",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Gravity Demo (v1.78b) (20210811) (3841ABB9).a78
    {
        "fbbe7afae5daba3df5f208e3951f7bcf",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Gridrunner Engine Demo (20210625) (087A5D9C).a78
    {
        "1fbaa5aff58fc972d049a09bc0420262",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Hallo (POKEY Music Demo) (4000) (20130612) (3BF9C84F).a78
    {
        "637aee76d45d7588af73a19fe646de38",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Happy Halloween (Demo) (20071031) (28500F8C).a78
    {
        "d263c862b8c9d90a8de426133b699930",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Happy Halloween (No Year) (Demo) (20120812) (E507DB26).a78
    {
        "019a4e714837756ddbe5a868bfc0267f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Hello World (Demo) (20091107) (E432341C).a78
    {
        "bbed9dc62d26e8b1faea98969f90f773",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Hello World (MADS Version) (20210416) (817B117C).a78
    {
        "646e64a009e6df01f24462b123198c02",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Heofonfir (Vertical Shooter Demo) (20230307) (60C75E71).a78
    {
        "74a2fa2ae2fc512ec0f7fe12ac39fe84",
        0, // cart type	0
        2, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Horizontal Scroll - Mix Modes (20220904) (971D70EB).a78
    {
        "3a9cce996a66f831d70efc3f3b97652c",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Horizontal Scroll - Mix Modes 320 (20220909) (E22F6D10).a78
    {
        "efbf762fecab00b81beb40655640da0f",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Horizontal Shooting Demo (20210630) (77354BF7).a78
    {
        "432bc86e48d0aa146df9c7079900bb5a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Huffmunch - Collision Chaos (Covox Music Demo) (20230115) (83C3B327).a78
    {
        "b4ae2347200829a0bcc3e3d3fed49aad",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // HunterS (Picture Demo) (20170121) (3B8BD950).a78
    {
        "ff433328d6c5c43385962264277ddedb",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ice Climber (320B PMF Demo Screen) (20170823) (743DC770).a78
    {
        "dcfc48c3879c21b2a2b1f4df9b3f1653",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ikari Warriors Title Demo (20200809) (67EFEE39).a78
    {
        "063ac04a072eb8395d6f54171800f493",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // INXS (320B PlotMapFile Title Demo) (20170821) (D7EB8839).a78
    {
        "b996e6bd302484125f21cb2be49654e9",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // It's Conner Time (v1_42) (20220812) (41260958).a78
    {
        "900eedbb1255624fc4bf085578d51b8b",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // It's Conner Time 2 - Conner's Dream Land (v1_45) (20220831) (517BC1E7).a78
    {
        "f23384853e59b1563072ed3706401dfc",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // It's Conner Time 3 (v23_75) (20221214) (A5595B72).a78
    {
        "35d34ecd7ba67314a848b7dd709de882",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Jinks Scroll Demo (20220812) (66E67754).a78
    {
        "ea5a3c7c34fd4c6eb0c37f40f64b3476",
        0, // cart type	0
        128, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Kid Daedalus (160A) (POKEY 450) (20230213) (3305A8FB).a78
    {
        "d89c7b6daeab0331be0a61049b1b02b7",
        0, // cart type	0
        64, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Kid Daedalus (320C) (POKEY 450) (20230213) (2B33AD39).a78
    {
        "e3ab78ad99dfee47de305fa6279c2927",
        0, // cart type	0
        64, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Kid Icarus (Screen Image) (20190220) (F408F116).a78
    {
        "570cf7a7b6fa01d4b324671ec27aa038",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Kirby's Dream Land (Direct Scroll Demo) (20220810) (A03AFF9C).a78
    {
        "6d55d960988f165ba89eab7b7fbe826c",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Krull 320B Title Screen Demo (20230430) (03CFF69C).a78
    {
        "c201bd62e76a0bbd74136a629b7ac5c2",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Kung-Fu Master - End Stage Sound Demo (4000) (20200729) (462E574E).a78
    {
        "d8c2f887507ffe1104244df81be32dc8",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Kung-Fu Master - End Stage Sound Demo (450) (20200729) (BBB9EABA).a78
    {
        "d4e77eece98bd378717464343c9e219a",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Kung-Fu Master - Game Complete Sound Demo (4000) (20200729) (6C7804E5).a78
    {
        "1324e90bd0ae0f888f43fe3d557b8489",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Kung-Fu Master - Game Complete Sound Demo (450) (20200729) (FB962099).a78
    {
        "9c83ccc2c049f64a45860a9aaa8fdb8b",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Kung-Fu Master - Game Over Sound Demo (4000) (20200702) (29B7E500).a78
    {
        "5ca4746952864f723064c9aeb482208f",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Kung-Fu Master - Game Over Sound Demo (450) (20200702) (965341B8).a78
    {
        "29f74f2e23ad3f5853fb9d57314e4c5c",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Kung-Fu Master - Intro Sound Demo (4000) (20200702) (6FF93CD9).a78
    {
        "60ab82348f1781e4241d12000c7fd03d",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Kung-Fu Master - Intro Sound Demo (450) (20200702) (06617D12).a78
    {
        "1a8b9eaf243a40e8dee6b805a6acd8bb",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Kung-Fu Master - Main Theme Sound Demo (4000) (20200731) (96B26C43).a78
    {
        "4399be4ff6dd5e8081bb62ee36fb8d26",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Kung-Fu Master - Main Theme Sound Demo (450) (20200731) (04D98094).a78
    {
        "de1bf2dd0ba70ba5c220742ef96cdf6f",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Lattice Blaster (20210923) (3F4B6842).a78
    {
        "0ccaba793e2c0f7bc61e0405fda6a282",
        0, // cart type	0
        0, // cart type	1
        3, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Lava Demo (20201004) (AD45AC5D).a78
    {
        "71c911d2d9c0e7568a05397f5c95d959",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Lenny Kravitz - Fly Away (NTSC) (YM2151) (20211109) (F6286678).a78
    {
        "7010d19064e08d53c1bbef280d88d030",
        8, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Lenny Kravitz - Fly Away (PAL) (YM2151) (20211109) (59C0CCA7).a78
    {
        "d70d606661e333fb1955835f6e62b314",
        8, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Line Target Demo (20230209) (F35BB24D).a78
    {
        "35691f1a5c635b418c22b4fb8743d424",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // LSD Lizard (Multicolor 160B Plus Mode Demo) (20210215) (DC2E71BF).a78
    {
        "f1605829c9eeb62ea8c3e20fa0c6bc31",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Lucelia Movement Demo (20210820) (CA384CDB).a78
    {
        "bd5eead1ad994be35f5808bf481e33d9",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // LZSS Player (v0_1) (20230226) (167E1987).a78
    {
        "1b52ac47279d221d0301ecceaea9f7af",
        0, // cart type	0
        68, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // LZSS Player - Przeszlem Freda w lewo (POKEY 440 450) (PAL) (20230303) (1BADDF4F).a78
    {
        "38495bfefcec5def6ebbb25cc45135aa",
        4, // cart type	0
        70, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Mappy - Death (Sound Demo) (4000) (20140416) (B5B0FFA6).a78
    {
        "14a227673341bfe63f490f7ee3d42bce",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mappy - Game Over (Sound Demo) (4000) (20140416) (1EF37E1F).a78
    {
        "f48ea150a43419c8abb9e73b7e460fc9",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mappy - Melody (Sound Demo) (4000) (20160412) (47695552).a78
    {
        "475db227ac46b2cb3523df912352e272",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Maria Bug (20210401) (82C1936F).a78
    {
        "5f08f31dfce85f9db0535490f7d79ca5",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Mario Bros (Ice Stress Test) (20210315) (DB235070).a78
    {
        "3869cdcd3208f7bd270708cee9babc0f",
        0, // cart type	0
        98, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Mario Bros - Coin Bonus (4000) (20210326) (ECBA9C68).a78
    {
        "ba31169a5341166b590f7677770838a2",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Coin Count (450) (20210325) (7850DFE9).a78
    {
        "e037a1c434431f59f9ff357ecf1dc6fa",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Coin Drop (4000) (20210312) (8A91AE97).a78
    {
        "e672906df7c312568ef7ce5426a95c50",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Coin Grab (450) (20210325) (0684D644).a78
    {
        "d3f3d63f6c714f83bc2bc682958f912c",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Coin Timer (450) (20210325) (9854D73F).a78
    {
        "45a629cb11e9244855f413b9f3de3acb",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Crab Drop (4000) (20210325) (F72B1E39).a78
    {
        "5261e825d3f244a85f28c82649416863",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Death (4000) (20210325) (A148BA74).a78
    {
        "17c7c50de60cdc19d214c26ceefdd4fa",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Extra Life (4000) (20210312) (B4E53CD1).a78
    {
        "28ebd237b91c9cc5a7630d105932df1b",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Fireball (4000) (20210325) (C7CB5A86).a78
    {
        "616cdfe48e788013a7d101654ada8aaf",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Fireball Vanish (4000) (20210325) (CDCB7897).a78
    {
        "3357bd3894f55cb78d147a91ee6ee76d",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Flip Enemy (4000) (20210325) (7DEE712A).a78
    {
        "9a10408a259f1423f1cfc7f5c0449a3e",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Fly Drop (4000) (20210325) (9435B0E6).a78
    {
        "96d0f248178224e37d9a26b3e575b7e5",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Game Over (4000) (20210322) (D6F96FF2).a78
    {
        "47dc966545bc77e158e981705fa6070a",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Hit Enemy (4000) (20210325) (7594C116).a78
    {
        "708d42ea01219858c130825c7a51b9fe",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Ice Freeze (4000) (20210325) (C2DE8483).a78
    {
        "1cc09b4ff0e416250c241c189cc4e6e4",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Icicle Drop (4000) (20210325) (1FC8E532).a78
    {
        "6a5b4527deef735b9adfd1c1c92f5283",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Jump (4000) (20210312) (8AADD5B6).a78
    {
        "abbe65a63804110a19718385b19cc425",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Kick (4000) (20210325) (C0ABD704).a78
    {
        "b18a7a59320e2849ce5aa3816f5441b9",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Kick Final (4000) (20210325) (A557A4BB).a78
    {
        "7819a0ca2e40ea5628578bb51085ecc7",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Level Start (4000) (20210322) (01676591).a78
    {
        "644fbbc878286e65af8c9d7e164f2f0a",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Life Restart (4000) (20210325) (CA799A81).a78
    {
        "4c697f0b1b9610e06da02bd93af41476",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - POW Block (4000) (20210325) (3E5F37A7).a78
    {
        "2533285afa34a036bfc8045271de3ce8",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Second Intermission (4000) (20210322) (167CF865).a78
    {
        "e8ed2f586ad489949dba2e417805b61b",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Shell Creeper Intro (4000) (20210322) (FB3E5B79).a78
    {
        "9bc4dc551b3b6c6798f9566060096cab",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Slipice Emerge (4000) (20210325) (3048BB3A).a78
    {
        "13d9f55108b8b4adfc65f603fc65212c",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Splash (4000) (20210325) (3627B74F).a78
    {
        "1d05db4cc64017e738ab2bf2f5f39336",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Stop (4000) (20210312) (C9B0DD57).a78
    {
        "aef1152c1f644bf03b326f2d5e9e30f7",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Title (4000) (20210322) (2D63DF1E).a78
    {
        "5a983fa7d52d26087952725e22b4e33e",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Turtle Drop (4000) (20210325) (5354D29F).a78
    {
        "d01d37cc4b70884a446907756f7cbbf4",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Walk 1 (4000) (20210312) (B6250548).a78
    {
        "95dccfd4ae4838a149baa077aef4d64b",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mario Bros - Walk 2 (4000) (20210312) (D27C5D56).a78
    {
        "c5edfcc07a99ba6470572d107c604bad",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mayhem in Monsterland (Scroll Demo) (20210117) (9AE6E663).a78
    {
        "2fe1b661021096a8251d56845147b3c4",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Maze Generator (20170831) (7A7750AE).a78
    {
        "292fad25ef64b84202a31f812c1199f9",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Mega Man 2 (Start Level Music Demo) (4000) (20190730) (E91FE8A4).a78
    {
        "ce670e292b4cb5bcb31f389044efd274",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Mega Man 2 (Start Level Music Demo) (450) (20190819) (646EF5DF).a78
    {
        "52e7bcc8e796123bcbe94f9b8097a25b",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Merry Christmas 2012 e-card (v1_10F) (20121227) (B276B1F9).a78
    {
        "114e215b8cfc8698bc0286a79e1cb9b2",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Metroid Brinstar Theme (Bass and Percussion) (4000) (20201123) (484DC2ED).a78
    {
        "dfb4bc159810414f03f8ef0bf746fd58",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Metroid Brinstar Theme (Lead and Harmony) (4000) (20201123) (A24393C5).a78
    {
        "2ec8ced60496693476e6fe401e09efa5",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Metroid Krayt Lair (Bass Lead) (4000) (20201129) (45A255BC).a78
    {
        "4cd8d6fe1670fc86287efed3cb3c1a42",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Metroid Krayt Lair (Bass Lead) (450) (20201129) (11267B6B).a78
    {
        "ba038b9b214a3db70f6c12f50269263e",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Metroid Krayt Lair (Filtered Lead) (4000) (20201129) (61A0CF1C).a78
    {
        "5336d65ccc24f788c8d7c217bcb4c6ca",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Metroid Krayt Lair (Filtered Lead) (450) (20201129) (93AF458F).a78
    {
        "e9b340e820a7d9742dcfd3fac561336d",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Millipede (Snapshot) (20171101) (B8939BD1).a78
    {
        "c9b5830c7335c23f800128043bf8b11a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Mister Purple Pants (Demo) (20091107) (9F600B69).a78
    {
        "dbc197d1038c6cadae5b0d57ceb4512b",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Moon Patrol Background (Scroll Demo) (20080920) (C8D0B881).a78
    {
        "35ab3e0a4c483e736e3a15a841240234",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Mr Do (20210824) (14E4D851).a78
    {
        "813d0006376fbf517d213b2a00264fc8",
        0, // cart type	0
        64, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Multi-Lock On (20140311) (5CDB3F72).a78
    {
        "79202cb7d2bc150ffca0c96a4d8b42cf",
        0, // cart type	0
        3, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Multi-Sprite (Sample) (20140318) (D4F212F4).a78
    {
        "b7446eb688d93fb6c925616e3ebb1e3b",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Multicolor RPG (Test) (20210130) (6DC05908).a78
    {
        "7adb7e4ef9dba71ab2960c899f86723f",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Multisprite Generator (20220522) (B3B3120A).a78
    {
        "b34e61ce0d295a48eec71e100c452a91",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Neverending Story, The (Full) (YM2151 Demo) (20211130) (DA47F343).a78
    {
        "badcb8fa671cfeff2122b7f8974979e7",
        8, // cart type	0
        8, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Neverending Story, The (Short Music Demo) (YM2151) (20211129) (0D565046).a78
    {
        "dcabeb5a3d00f986a7a23e24b8d8d6d3",
        8, // cart type	0
        8, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Nyan Cat! (20111230) (576E61C0).a78
    {
        "875744a88898e83ea54aa2d257d0c63a",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Occlude Sprite Demo (20200219) (F55181C6).a78
    {
        "a0811cb0d5a6628bb28c13da422bc328",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Omega Race Ship (Demo) (20150927) (31D16416).a78
    {
        "cc1e5781b1b560a30909aabb9932d85a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // One Hundred And One (Sprite Demo) (20200408) (2FD0D8FE).a78
    {
        "f3df85946635f81a0c38b4e541966968",
        0, // cart type	0
        6, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // One Zak And His Kracken (Multicolor 160B Plus Mode Demo) (20210214) (DDAB5E1A).a78
    {
        "d127e083146608aabb819af652c5a303",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Opmeridian (YM2151 Demo) (20211201) (32A8C60E).a78
    {
        "223977cd85430f4d32ad7dd569ad6963",
        8, // cart type	0
        8, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Outer Rim (320B Demo) (20180510) (663DBF20).a78
    {
        "aa9a3a8dd612a3624e055fa6ab0d3cbb",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // OutRun (Dual POKEY 440 450 Demo) (20221017) (2AEF290B).a78
    {
        "73b834afea606c1d3a156f2722c0b6b4",
        4, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // OutRun - Last Wave (No Waves) (YM2151 Demo) (PAL) (20211106) (412EAA9B).a78
    {
        "199fbb47181bb2efd929e96658394ba9",
        8, // cart type	0
        8, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // OutRun 2 (Dual POKEY 440 450 Demo) (20210117) (587223FC).a78
    {
        "ca3d5ad6ce2df1024d5319964bad471b",
        4, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // OutRun 3 (Dual POKEY 440 450 Demo) (20210117) (948CE618).a78
    {
        "fa51f88ec4a01dbed7ba2d20fbd5a97c",
        4, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Land Break Time Tune (4000) (20191011) (5C1FE9B4).a78
    {
        "49a00b14420b9fc183558eb15eb1cde7",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Pac-Land Break Time Tune (450) (20191011) (7EB5D2FE).a78
    {
        "f2f428ef3933d3955c2a111a2898aa3b",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Pac-Land Castle Theme (4000) (20191023) (BF396C21).a78
    {
        "6c010d5aceab137e37d4a0854ab91c4b",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Pac-Land Castle Theme (450) (20191023) (A36955A6).a78
    {
        "76b74f23b5e1df497131bd296dbe0e50",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Pac-Land Death Tune (4000) (20191005) (C4998815).a78
    {
        "1dd3f92f1cd4ae3da8c15e8274165ffc",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Pac-Land Death Tune (450) (20191005) (E6CAF48B).a78
    {
        "25b228cdbcfd9f22823f84591d864388",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Pac-Land Fairyland Melody (4000) (20191005) (E2098F22).a78
    {
        "ccb19da49a64f89c3b82accdd91cf3ac",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Pac-Land Fairyland Melody (450) (20191005) (104AB80E).a78
    {
        "0678603263bed024a2fe9a7648037f12",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Land Intro Melody (4000) (20191005) (92A1873C).a78
    {
        "cd0840fe67d37c5842b13fc3e2fd404a",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Pac-Land Intro Melody (450) (20191005) (5D45AECC).a78
    {
        "d53803ae9fa194b05a98aa3cbeb240cb",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Land Main Theme Music (4000) (20191004) (A4E9F5BC).a78
    {
        "7932eb2ae2252ddc80b57317c8d0d5ec",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Pac-Land Main Theme Music (450) (20191004) (77B95A67).a78
    {
        "02ee33242b5b60a3e3a3d83aad289b02",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Pac-Man (YM Sound Test) (v14) (20140825) (09927591).a78
    {
        "fb0b763aaa9b05c9b427089bbfc06506",
        8, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Palette Test - Pseudo 3D Flying Effect (20220219) (47CE20F1).a78
    {
        "b56779477a7f4c488f662a45eec891c0",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Parallax Scroll Demo 160B Mode (20210131) (9C2B111A).a78
    {
        "cccf165399e6777a749ba2db014fc285",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Plasma Demo (20200818) (B99A3941).a78
    {
        "5bd691552838ddaa1133acb1744fa6cd",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Plasma Demo (X-Only) (20200819) (1EEEF234).a78
    {
        "7a7b4f19a4219e1198939aeb62811ff6",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Plasma Demo (Y-Only) (20200819) (7958FBDB).a78
    {
        "adada18eaa134def6964bac4958429a4",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Platformer Demo (v01_78b) (20210318) (B91AD466).a78
    {
        "076cda4e8a67b3ea263b269eb81b7f8a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Plumber Buddies (Rough Demo) (20210314) (B7475E4D).a78
    {
        "7ca12ee0d2be999a90799d27a6efb1d0",
        0, // cart type	0
        34, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Popcorn (Dual POKEY 440 450 Demo) (20210117) (178D0804).a78
    {
        "0e00cc5d1d08dffa26ba77a4caa75d3e",
        4, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Price Randomization Test (20170811) (D5D953B0).a78
    {
        "f9ec873a59695e3522599df5976c9793",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Prince Of Persia Title Music (4000) (20200407) (898B35D7).a78
    {
        "318e4c045b8c39df3dcaa79621f4e757",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Prince Of Persia Title Music (450) (20200407) (091688D9).a78
    {
        "b60ed89155524b136ee188c4c518df53",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Punch-Out!! (Demo) (20141218) (5979E592).a78
    {
        "f35dcdea20e5b379164682bc7ae16c57",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Qix Sound Demo (v2) (TIATracker Player) (20200410) (DFE82FF3).a78
    {
        "fbe0c194d75766a9f1c64a2f2741750e",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Raindrops (QuadTari Demo) (v2) (20210304) (2400519C).a78
    {
        "634ecd3cd65ca6735aa316ff78b40e7c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Random GFX From NES (320C Demo) (PAL) (20230614) (7FBA4309).a78
    {
        "0760bb82a7463c83c1c167cfc807cf92",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Ray Future 2203 (YM2151 Demo) (20211130) (431E87AF).a78
    {
        "f7b67568a0cb357bb820ed95e6ae028a",
        8, // cart type	0
        8, // cart type	1
        2, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Ray Future 2203 (YM2151 Demo) (PAL) (20211202) (46180315).a78
    {
        "0e263486bd57fa47ab4e048db7b1cbd5",
        8, // cart type	0
        8, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        1  // xm
    },
    // Red Bandana, The (320C Demo) (PAL) (20230613) (CBC6EF55).a78
    {
        "b70967edd357f42ee02d772a26df2c55",
        0, // cart type	0
        4, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Reksio - Silly Venture 2023 SE Atari 7800 Invitro (Demo) (PAL) (20230409) (A2F17470).a78
    {
        "194e77d5f792e93b1fbca2358ca64caa",
        8, // cart type	0
        6, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Rick Dangerous Monochromatic Demo (20221103) (4F80FA59).a78
    {
        "96d862c169dbd64aa80ea943be95dfb5",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Rick Dangerous Spirte Demo (20221217) (2B871F0B).a78
    {
        "718f864c8e6780a1a0681b3b23ec0c46",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // RMT POKEY Player Test (Demo) (4000) (20030326) (354A80F2).a78
    {
        "03daa19b7aae2d27e61f2a4dbe3b9b79",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Rolling Thunder (Intro Melody) (450) (20190914) (07463A19).a78
    {
        "277e23d3b9ae27086b3768356cb0eb2c",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // RPG Player and Map Tiles (Demo) (20190518) (49647DCF).a78
    {
        "01c7bc3cd8375e353c8aa837fe1262ec",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // RPG Tile Demo (20170930) (32135BEB).a78
    {
        "5b01b454b1f8ac58994fc1096f0e2d0e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Scrolling Demo (Uridium Map) (20220704) (15AAB8FB).a78
    {
        "1a5914cc3c9bfd65f8e28fed9babfed1",
        0, // cart type	0
        6, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // SD Tunnel (Demo) (20211019) (FBB9D731).a78
    {
        "4de545ec8e270508c3e1cf2924455c64",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Senso 7800 DX (20011118) (09EF22DB).a78
    {
        "59b16698dc515039e765d23130ce9510",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Sexy Six (Music Demo) (NTSC) (YM2151) (20211109) (C653943A).a78
    {
        "1ec9a0357c166d9b4e3ce6c8ac155acd",
        8, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Sexy Six (Music Demo) (PAL) (YM2151) (20211109) (1ACC9B18).a78
    {
        "e45d96058c05b87cd62ace2788356d80",
        8, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Sexy Six (YM2151 & Covox Demo) (PAL) (20211128) (995A3F8C).a78
    {
        "4e0bfce5b34006dddea34f34cf7b8fd5",
        8, // cart type	0
        8, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // SFX Examples (PAL) (20230619) (979A0614).a78
    {
        "7148223052df6cef29a8a047bab6bc6c",
        0, // cart type	0
        4, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Shadow Of The Beast (Tech Demo) (20220523) (243146F5).a78
    {
        "42c4bfb0ce39e43423d00b47aa7184e8",
        0, // cart type	0
        6, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Shadow Of The Beast - Title Sceen Demo (20201210) (4D0D7FCA).a78
    {
        "88dcaeaa315caff8fe7f04f73bcbfd50",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Shantae Tiles (PlotMap) (20160626) (E5B6D113).a78
    {
        "af8ad990b4f6e721148c0c7bf70a6a74",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Silent Colony (YM2151 Demo) (20211130) (94CCCD80).a78
    {
        "6b5d057cb8f9561da3f172ef789e63c3",
        8, // cart type	0
        8, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Silent Colony (YM2151 Demo) (PAL) (20211130) (C18FA90F).a78
    {
        "e6b3cc12b4829f30a02e4997caf5ebb7",
        8, // cart type	0
        8, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        1  // xm
    },
    // Simple (Sample) (20140301) (058C496B).a78
    {
        "ffd0fecc48b60934b05f986dd1f0cee8",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Simple Starfield (Demo) (20230220) (552E08B3).a78
    {
        "985032c03b9164477ff2c78dddfd0044",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Slots (20141221) (97DA3370).a78
    {
        "b3fae0085acda69415544f49c881e40c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // SMB 2 Screen (PlotMap) (20171003) (F1DED04D).a78
    {
        "116fbaf1c3f04f10380372da90fd07e1",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // SMB Screen (PlotMap) (20160626) (A3E043A5).a78
    {
        "d59ae1ea4b3e3d7e8a30bb5ed0fa37d5",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // SN Cart Demo (PAL) (20221211) (253F3C42).a78
    {
        "fde65d2abde9072e29987102c3d67270",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Snooze Ewes Demo (20230128) (D704A902).a78
    {
        "16a7494aa2917d817f37679f8df321da",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Soft Cell - Tainted Love (Music Demo) (20140311) (1C0C62AE).a78
    {
        "ca1c27b53fcfb9fed83bc9e92920707c",
        0, // cart type	0
        18, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Space Chess (20230106) (CE1AE773).a78
    {
        "ea2b4d8e1bd7de938609a358cd35d400",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Space Manbow (Music Demo) (YM2151) (20211129) (F1C12DDE).a78
    {
        "70f02585f2066bb0b10bd793fa7f82b8",
        8, // cart type	0
        8, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Space Physics Sample (20200215) (88DB6151).a78
    {
        "4b7c02555dd12d073d2a5a157cd7432a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Split Mode (Demo) (20150107) (C89D766D).a78
    {
        "ad7a7ba7e368f10cb2984d5dfc560ba1",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Sprite Decompression (20220610) (E831617C).a78
    {
        "252c205a400442ff42237e3cae9bc656",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Sprite Demo (20040117) (4DEF0B0B).a78
    {
        "cf0d140195ead4e2850d753e9afed672",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Sprite Demo 2 (20040117) (98D4A416).a78
    {
        "625e611aeaf162ce8fd77980c64e5e30",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Sprite Engine Test (20210319) (253EC99A).a78
    {
        "601a0344a3597b535aa55bad0c26dd6e",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Sprite Engine Test 2 (20210320) (76578979).a78
    {
        "1a6786837b9c77b12b8b9c3ce5781fd5",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Spy Hunter Scroll Demo (20220830) (C4D9E419).a78
    {
        "15a9dbfed92eee6207bb7f77e844d084",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Stanley - The Search For Dr Livingston (320C Demo) (PAL) (20230614) (748F6E4C).a78
    {
        "060b9f741df66b98f458e684e4c7e06e",
        0, // cart type	0
        4, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Stardust (Forward Scroll Demo) (PAL) (20220911) (113C9322).a78
    {
        "2335d78f9c06b9a9622eda75958a0a5d",
        0, // cart type	0
        128, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Steam Ponny (Multicolor 160B Plus Mode Demo) (20210221) (7E3D5393).a78
    {
        "986b26abb8898a6e856cde909b92b413",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Stickerbrush Symphony Music Demo (4000) (20200326) (CBCA55A0).a78
    {
        "5352eba5f1dd0e5887a4e42e429d4d42",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Stickerbrush Symphony Music Demo (450) (20200326) (59FED6C5).a78
    {
        "9abd770ce284aed14cc5abde5981f96e",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Stickerbush Huffmunch (Music Demo) (YM2151) (PAL) (20211129) (87D14DDF).a78
    {
        "1c50c360b3505c8acb2287d770c4136c",
        8, // cart type	0
        8, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        1  // xm
    },
    // Stickerbush Symphony (Music Demo) (YM2151) (PAL) (20211129) (EB3B942D).a78
    {
        "bd9fa9897f1907454910cbcfa6b4f94e",
        8, // cart type	0
        8, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        1  // xm
    },
    // Stranger Things Main Theme Music (4000) (20191023) (BBF50000).a78
    {
        "9d1826f1ba1155b03bab2af674979d73",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Stranger Things Main Theme Music (450) (20191023) (C0DC2F14).a78
    {
        "06f8ffef827569343ae2f9bdd5e07ca0",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Stranger Things Upside Down Theme Music (4000) (20191023) (D948B8E0).a78
    {
        "6e94a8c7a39a37f180968eb955b4d4c6",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Stranger Things Upside Down Theme Music (450) (20191023) (E6EDD265).a78
    {
        "c3351539daa5936eab808af3d39e7fda",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Styverson (Picture Demo) (20170126) (0025C6CF).a78
    {
        "bbc3921c505ebbcc317e8c2d98b5bfd6",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Super IRG Flipping Images Demo (20171120) (4C012EC7).a78
    {
        "aea1718d7226ad0194a140eb8f14fa23",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Super Mario World Opening Melody (4000) (20190815) (175FE360).a78
    {
        "b5912c6e17c33cd82e3cf774d8b0933e",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Super Mario World Opening Melody (450) (20190815) (B7CF4358).a78
    {
        "61af38a3292c41436c0d77752ea808b2",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Super Mario World Overworld Theme (4000) (20190917) (B89E92DE).a78
    {
        "a540559e4fb9ea2d166926eaf6d417c5",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Super Mario World Overworld Theme (450) (20190917) (4747B643).a78
    {
        "a97a0b327783adf492e2f2f1908c4b7e",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Suspiciously Shifty Demo (20190830) (93D809BB).a78
    {
        "fb5ebf60dc7c753a0a64b04f02162d05",
        0, // cart type	0
        38, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Text Incrementer Demo (20210310) (A83595B0).a78
    {
        "0cec381dc65d25cbfe3e7e91fa1f964c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Thermostat Music Demo (POKEY 450) (20210123) (61997089).a78
    {
        "eab0a5b480e9e2897aecae9a522a1cd7",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // TIA Music Demo dbAS (20190106) (FE7ED047).a78
    {
        "90e09de2af44ac65b2ae09eee74175df",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Tiled Test (20230612) (F3BC8377).a78
    {
        "b3d2fbb71d2f81f1cfd99569efad5f25",
        0, // cart type	0
        4, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Toki (Single Screen Background Demo) (20190304) (2BEEB41C).a78
    {
        "4e1b749bbd0f4642835bec6c2ab29286",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Top Gun - Danger Zone Demo (4000) (20200612) (51F998B8).a78
    {
        "3bb5f34a74436c0c43db11e543b31509",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Top Gun - Danger Zone Demo (450) (20200612) (45C7A4A0).a78
    {
        "21621e3358533adcb8191cee238f929b",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Track & Field (Demo) (20210814) (4851E3B0).a78
    {
        "f01789d62cd76584dc22fc3f199d0592",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Truchet Tiling (Demo) (20170827) (E0B743E4).a78
    {
        "8e5d0262ffcd9b2573728bbcff15618f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Turrican II Circular Scroll Test (20211213) (51B09E0D).a78
    {
        "9558c2f32d0ed730569a4a449da05e7d",
        0, // cart type	0
        130, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Two Modes (Demo) (20220221) (50D6F240).a78
    {
        "091f398ae75b85441610bbb376344bfb",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // UN Squadron - Cave (YM2151 Demo) (NTSC) (20211107) (D801B2A4).a78
    {
        "b6878a8c1b919d5216ce510197ccca4f",
        8, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // UN Squadron - Cave (YM2151 Demo) (PAL) (20211031u1) (08ADA7A5).a78
    {
        "c5c8dd4dc382a78a03e50c7e888dcbf1",
        8, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Under The Hood (Demo) (20200530) (15810446).a78
    {
        "24080666bcbf6ee78136d2f4b4a71ca8",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Untum Intro (PAL) (20181228) (194F7118).a78
    {
        "26409d39e3aadbf870dd3f918bf944e4",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Uridium Scroll Demo (20220708) (BEA7F011).a78
    {
        "e9eb6ea912c0af98e166559f5afd2694",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // VGM Player - Flooded Cellar (YM) (PAL) (20230429) (D071256D).a78
    {
        "69028de97c24f877ce4d3a771276682a",
        8, // cart type	0
        6, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        1  // xm
    },
    // VGM Player - Highway (YM) (PAL) (20230429) (059956D6).a78
    {
        "98e1135e748dd678e50c763ba27c8f5f",
        8, // cart type	0
        6, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        1  // xm
    },
    // VGM Player - Oil (YM) (PAL) (20230429) (CCBE932D).a78
    {
        "8ef65dad29500592a94788c061bc7969",
        8, // cart type	0
        6, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        1  // xm
    },
    // VGM Player - R-Type Title (YM) (PAL) (20230429) (60C1D179).a78
    {
        "8e7042ebc05a25846a188698b5fd8068",
        8, // cart type	0
        6, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        1  // xm
    },
    // VGM Player - Sky Chase (YM) (PAL) (20230429) (84B7E1E6).a78
    {
        "57a6ba1d67f87ace4776250fb2ad84ce",
        8, // cart type	0
        6, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        1  // xm
    },
    // VGM Player - Studiopolis (YM) (NTSC) (20230429) (434405A5).a78
    {
        "54bb050f203adbceb47344f571be0d2a",
        8, // cart type	0
        6, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // VGM Player - Studiopolis (YM) (PAL) (20230429) (0E1FF449).a78
    {
        "6e4377948980156c1b4d304605da120f",
        8, // cart type	0
        6, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        1  // xm
    },
    // VGM Player - The Only (YM) (PAL) (20230429) (F08A47D1).a78
    {
        "912cfd0eab8a674d330a11110a130939",
        8, // cart type	0
        6, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        1  // xm
    },
    // Visage - Mind Of A Toy (20140104) (BE66A692).a78
    {
        "3257d492ed7a3f674047248d280ec470",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Walk Cycle (Demo) (20221110u1) (0F33719A).a78
    {
        "8ff33b39d1405a78d1fbd4da05167154",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Walker - Tech Demo (20210321) (DD61A051).a78
    {
        "846032d029458ac26e7a03bc6f751708",
        0, // cart type	0
        6, // cart type	1
        8, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Waterfall (Demo) (20211019) (46F5656D).a78
    {
        "0f0c5d6cb273c24d7a4ef16a1c0464e2",
        0, // cart type	0
        10, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // We Robots (Demo) (20210225) (8FCC3CD7).a78
    {
        "8ae80dbffad933dde9ca1af8e1eaf507",
        0, // cart type	0
        64, // cart type	1
        5, // controller 1
        5, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // When The World Explodes (320C Demo) (PAL) (20230613) (C698B1CC).a78
    {
        "05a0bb2b765c3a224f7cd12afb39d194",
        0, // cart type	0
        4, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // White Lamp Music Demo (Dual POKEY 440 450) (20210120) (7EA838D4).a78
    {
        "24a0b16d3798ab9169b0a7cc2056b383",
        4, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // White Lamp Music Demo (Dual POKEY 800 810) (20220824) (AF91C850).a78
    {
        "ff3a7b143fd2bf8f5edc1470aad4a0e2",
        128, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Wizzy (20220317) (B0B0D56D).a78
    {
        "175297a0de778a0f3f403c21abf8f955",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Wond'ring Aloud (Covox Demo) (Main Loop) (20210327) (A7DCE833).a78
    {
        "63942aa35b98c3d1f780579ada012649",
        0, // cart type	0
        2, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Wond'ring Aloud (Covox Demo) (Via IRQ) (20210329) (C8AFCDD0).a78
    {
        "c8bda599112a99f8b214ac94f51696bf",
        0, // cart type	0
        66, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Wonder Boy III - Side-Crawler's Dance (YM2151 Demo) (PAL) (20211031) (DE416725).a78
    {
        "69d6874cd817d6a91f3960dcf218e3f6",
        8, // cart type	0
        8, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Wonderland (Multicolor 160B Plus Mode Demo) (20210220) (4D077096).a78
    {
        "fb1c9af5fb0c77fe5939f5f8e20d8f67",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Wormy Demo (320B Mode) (20070415) (FFB64127).a78
    {
        "e228a26971e649dafec2822d21f98cd3",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Xenoform (320C Demo) (PAL) (20230613) (D7336733).a78
    {
        "28b7a8c99b2a89feb7e0dab1d9fa2547",
        0, // cart type	0
        4, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // XMYM Tracker - Ghosts'n Goblins Stages 1 and 2 (20220224) (DD1CB69A).a78
    {
        "f5cc933a16f8d59c72881622396d3165",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // XMYM Tracker - Side-Crawler's Dance (20200223) (30A8535E).a78
    {
        "237219056cc0b84a76fff22680492048",
        8, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // XMYM Tracker - Zanac (20200221) (9255F49A).a78
    {
        "a466a699324d11e1a2594cc89a9f18e1",
        8, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ye Olde Inn (Music Demo) (YM2151) (20211129) (2A8B3648).a78
    {
        "61c05fd0ea1d4f711c347715176a7c6b",
        8, // cart type	0
        8, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Yoomp (Demo) (NTSC) (20211218) (3C75C447).a78
    {
        "58bb6324638f1d83d047a697006ab0f3",
        0, // cart type	0
        6, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Yoomp (Demo) (PAL) (20211218) (B028469C).a78
    {
        "42d367fcb3e19307dab89ad7f8980310",
        0, // cart type	0
        6, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Yoomp Multiplayer (Demo) (PAL) (20211219) (C1146D88).a78
    {
        "3d2b49b0c0b472fcea8c4ca388b77b61",
        0, // cart type	0
        4, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Zanac Area 1 and 2 Demo Music (v1) (4000) (20190915) (D28E4623).a78
    {
        "f29e1583210a491554c39a79e924ae89",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Zanac Area 1 and 2 Demo Music (v1) (450) (20190915) (BFB1DE18).a78
    {
        "8c807ca164a4b2fe315c41c97f7e82de",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Zanac Area 1 and 2 Demo Music (v2) (450) (20190915) (28A27FB8).a78
    {
        "5b767589b18fb73572903d695b8455fa",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Zanac Theme Music Demo (4000) (20200805) (D721C7AA).a78
    {
        "8cc40870e3021ea0f894509e2af5412e",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Zanac Theme Music Demo (450) (20200805) (A37A9E7B).a78
    {
        "f7a9d75661330098309e9a27f3e8dff5",
        0, // cart type	0
        64, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Zanaz Area 1 and 2 Demo Music (v2) (4000) (20190915) (357A36D9).a78
    {
        "64a11fe53d036ce34cc14dc98444d20c",
        0, // cart type	0
        1, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Zelda GB DX Demo (20230101) (ABE872AB).a78
    {
        "be487ad4715b499356dc3c89c71cb979",
        0, // cart type	0
        70, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Zelda-like (Demo) (20091107) (DFCACBE0).a78
    {
        "f8adc537ac7e219c9a939621f79aeac4",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // 2600 Maze Pac-Man (Hack) (20171231) (7955FD37).a78
    {
        "fc283888e7c58298c1744447ed06b506",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Asterix (Hack) (20110401) (1D19EC23).a78
    {
        "0b73444d901773638b73bf035e16e81d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Asterix Quest (Hack) (20161224) (5EFAD5EE).a78
    {
        "608fa599f06f935e05d445ff236f6d7a",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Asteroids (Vector Edition Hack) (20050724) (783F7667).a78
    {
        "ff8d8283553af5d5dbdaddb5781b4896",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Asteroids Deluxe (Blue Hack) (20200405) (523F67D5).a78
    {
        "4766575679a2ccb49f37d790952d6725",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Asteroids Deluxe (Blue Hack) (P2 Pink) (20200405) (62E60AF7).a78
    {
        "7b0491b519d5339d600e014abfb5ca3d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Asteroids, 3D (NTSC) (Hack) (20131104) (0C70D101).a78
    {
        "3de805e09da58b856125da3c8eec732c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Baby Pac-Man (Ferrell's Hack) (20180610) (67048F7F).a78
    {
        "40cd9bcce4482d3e5f2088b1c72c2cfc",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Baby Pac-Man (Jayzn's Hack) (20130210) (1E7765D2).a78
    {
        "0d7394c706f0b7d03ef95f4be27103e2",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Baby Pac-Man (Jayzn's Hack) (Fast) (Invincible) (20130403) (9558E862).a78
    {
        "cc496a6d5ab258c1e13400316265edf8",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Baby Pac-Man (Jayzn's Hack) (Invincible) (20130403) (A0C9440B).a78
    {
        "0ce0f02f95a9663d68a43f62605fd0b2",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Baby Pac-Man (Justin's Hack) (20130510) (82789365).a78
    {
        "13478a0b3733697c062b90236e9c2342",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Baby Pac-Man (Uki) (Hack) (20130422) (C06965B7).a78
    {
        "120817a77f563d16408445e588c62198",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Beef Drop (New Levels) (Hack) (20061225) (E104798F).a78
    {
        "6010a398070dfacb4c0173d75d73c50a",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Bentley Bear's Crystal Quest (Wonder Hack) (20170820) (D496FF6E).a78
    {
        "e1f0a708fbc107001fc49ce48151fefa",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Bleach Pac-Man (Hack) (20160917) (4BD0C840).a78
    {
        "d24a8b49ae091193cc0364a570e4d6ae",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Blinky (Hack) (20200305) (D24BC02B).a78
    {
        "12f7aca1463ec191f6eacf49b61baf74",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // BonQ (Final AtariAge) (No POKEY Init Hack) (20210403) (5C6F91AF).a78
    {
        "46fcbd65098d3fdb0e99a172e2699d41",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        2, // save device
        0  // xm
    },
    // Caterpiller (Hack) (20090828) (2AF89143).a78
    {
        "580db13b820b7a46c4c4b39685469982",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Centipede (Tball) (Hack) (20050630) (8F091429).a78
    {
        "e11ff2269327673f2d2aa42d01385c09",
        0, // cart type	0
        0, // cart type	1
        4, // controller 1
        4, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Centipede Arcade Bezel (Hack) (20200831) (F0DFA8C8).a78
    {
        "b481b0d3f4659537346599cb04a58833",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Centipede Arcade Bezel (Trak-Ball) (Hack) (v3) (20200831) (46CD8E75).a78
    {
        "76512942fcd706268c1ae2f6d438ae2a",
        0, // cart type	0
        0, // cart type	1
        4, // controller 1
        4, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Centipede Dark Bezel (Trak-Ball) (Hack) (v3) (20200831) (724E3CFF).a78
    {
        "44a0941d58dcfaa75b127648d3e4527a",
        0, // cart type	0
        0, // cart type	1
        4, // controller 1
        4, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Centipede Frameless (Hack) (20150604) (49BC6EF2).a78
    {
        "20660b667df538ec32a8e1b998438604",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Centipede Frameless (Trak-Ball) (Hack) (v3) (20200831) (7229A7DE).a78
    {
        "db8f39e5a03a3741ffe42dfc01bbff8a",
        0, // cart type	0
        0, // cart type	1
        4, // controller 1
        4, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Centipede PMI (NTSC) (Hack) (20150927) (9B3EF1DB).a78
    {
        "be86cc76289b3c05311c5e9992e259a5",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Centipede PMI (PAL) (Hack) (20150927) (E7CE0ED7).a78
    {
        "af1c8f89f0aef0d9e2e15901d6e0539a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Centipede PMI (TBall) (20150927) (885AE253).a78
    {
        "1e3618f11da5ee15924feac90dd1d0ac",
        0, // cart type	0
        0, // cart type	1
        4, // controller 1
        4, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Centipede REV (NTSC) (Hack) (20151003) (336A67CD).a78
    {
        "8b4721b786f2575b6dbc9be87cb7ff14",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Centipede REV (PAL) (Hack) (20151003) (64CEF556).a78
    {
        "3d5ee8174ed79a82059331e0e430c196",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Centipede REV (TBall) (Hack) (20151003) (DE782A8A).a78
    {
        "edf6d9e8ac8d8ad60449dfbf9fd45c72",
        0, // cart type	0
        0, // cart type	1
        4, // controller 1
        4, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Centipede+ (NTSC) (Hack) (20130108) (2C7A508A).a78
    {
        "40b12bbf1500c5a865f29e830852655e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Centipede+ (TBall) (NTSC) (Hack) (20130108) (1086FDE8).a78
    {
        "5057ff01cdcd70f42961f8cab63b63a3",
        0, // cart type	0
        0, // cart type	1
        4, // controller 1
        4, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // ChampPac SJB (Hack) (20130420) (775A17B8).a78
    {
        "e5582caf6b9088fff1107396b52b9123",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Chomper (Hack) (20160307) (0EF9F2B5).a78
    {
        "26897ab47b8c5d3b57d3cc235d7635d8",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Choplifter (HUD) (Hack) (20130921) (FAF7F159).a78
    {
        "f23d805b7747ff7c0bfc6df173a2f0c7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Choplifter (Wings) (Hack) (20130921) (4F7FB9B6).a78
    {
        "1f5743dcdd1092d3089fc30dda4910a5",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Christmas Jr Pac-Man (C64 Mazes) (Hack) (20091224) (83AE15A7).a78
    {
        "7d0621dc193600e408a952dda62554ef",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Christmas Jr Pac-Man (New Mazes) (Hack) (20091223) (E6849320).a78
    {
        "766f06d4425fba629a475d3a9532a807",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Christmas Jr Pac-Man (Old Mazes) (Hack) (20091223) (295B1462).a78
    {
        "efdde895f504b0f9ba7e9ca9ac2556a7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Christmas Pac-Man (Hack) (20171225) (457C3EE1).a78
    {
        "2b1f78aaa2b8de5dae3ee4b93ab678f4",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Christmas Pac-Man 2 (Hack) (20181225) (2736B163).a78
    {
        "3bda710f399c3b2d96cefe449cd76fa2",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Christmas Pac-Man Plus (Hack) (20191225) (82BD2089).a78
    {
        "d228f7514cf2794baaf7f3c22664bd01",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Christmas Salvo (Hack) (20201201) (9E9B680A).a78
    {
        "0ab90a8fce009abf36a9a09d5cf40275",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Clean Sweep (De Lucia's Hack) (20130502) (B43B9AB3).a78
    {
        "8acfefbbd5d4779d796ef35b6b85b64c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Clean Sweep (Vectrex Edition) (Hack) (20151204) (DA1F2D6C).a78
    {
        "bd3d32af937111020975494d064484b1",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Clobberman (Hack) (20181210) (E84927D7).a78
    {
        "86de3b52de881d7bb81223edebc47dae",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Clobberman (Invincible) (Hack) (20181210) (29B7D343).a78
    {
        "c77501cfc24ca1a531dfeaf1a7faa4b0",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Coronavirus Pac-Man (Hack) (20200710) (A0A093E4).a78
    {
        "fdc5c94c996ad2138224125f5f5b3d6c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Cosmic Cruncher (Hack) (20141220) (2027BEC5).a78
    {
        "bb8bab7ff1817e9f5247b8634f93d697",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Damaged Pac (Hack) (20130422) (EA38513F).a78
    {
        "84c0787708ba24bdc3d274cefb9e50d1",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Desert Falcon (HSC Support) (Hack) (20040616) (53E277F6).a78
    {
        "9ea73fd07e43f61209876d33e6f6dc04",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Desert Falcon X (NTSC) (20130805) (9DE60AE1).a78
    {
        "eb2343078325258a4d1043df90911635",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Desert Falcon X (PAL) (20130805) (FD9D63FF).a78
    {
        "28ea2232475b2cb8b3463d03d1264c36",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Dig Dog (NTSC) (Harp) (Hack) (20130103) (8B75F1E7).a78
    {
        "238bab817ac761314935b2ef870e8c35",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Dig Dog (NTSC) (No Harp) (Hack) (20130103) (8B75F1E7).a78
    {
        "238bab817ac761314935b2ef870e8c35",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Dig Dog (PAL) (Harp) (Hack) (20130103) (FCB569B2).a78
    {
        "54829fb744d4cd7a794ccd2580df7c3d",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Dig Dog (PAL) (No Harp) (Hack) (20130103) (A2FED464).a78
    {
        "dbb493bdc4e98436dbbfd4f2e4413397",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Donkey Kong (NES Title Music) (13B4ED2E).a78
    {
        "3e7e976b7dfa41638be8d3e0aba59d19",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Donkey Kong - Pauline Edition (Hack) (20230513) (107078DF).a78
    {
        "8b8b0dd994cfe744ab9aa3bc6a321b28",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Donkey Kong Jr (Graphics Hack) (20140927) (CE541F67).a78
    {
        "79f9692678818179554672fe4cf9b8a2",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Donkey Kong Jr (TIA Sound Hack) (20221217) (8D24E4EF).a78
    {
        "a620463707fcb47be866e3f71a86318b",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Donkey Kong PK (Pre-Alpha Build) (Kill Screen Level 22 Start) (20121209) (A3C2AF49).a78
    {
        "2d22ee7ae9c524b6762bb3fa2d7c430a",
        0, // cart type	0
        11, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Double Dragon (AM) (Graphics Hack) (RC7b) (20170616) (05F3CEC2).a78
    {
        "098b209aac126f2c2edbc982df09cd1b",
        1, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Double Dragon (Sprite Test 2) (Hack) (20140922) (759FDBF9).a78
    {
        "4565867aa6e5cc710a7edaf6d434b3af",
        1, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Dual Pac-Man (Hack) (20050902) (C58595BC).a78
    {
        "fa4aec407b90e9360b9cfeb41839b09a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // FASTeroids (NTSC) (Hack) (20131202) (D825D6B8).a78
    {
        "b0479c32eff527ad93b1a9558f5acff9",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Fatal Run (Graphics Hack) (RC1) (20140927) (1A297C55).a78
    {
        "2fb85cab6e0f0582e3057bf1ac33c74a",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Field Hockey (Hack) (20140122) (F1B243F0).a78
    {
        "c720cd8d8a7ca495a9125aee6b74a13d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Food Fight X (NTSC) (Hack) (20160724) (23A4CF11).a78
    {
        "806e8b3ec1fd143c510207b2bffffd6e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Food Fight X (PAL) (Hack) (20130318) (AE2429D2).a78
    {
        "f5127063b61e2deb28f264c2c8b52087",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        1  // xm
    },
    // Fuzzipede (NTSC) (Hack) (20150919) (6472B496).a78
    {
        "3cfdbad1276dfe0328a418f037101a44",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Galaga (Graphics Hack) (20130320) (E55A2CCF).a78
    {
        "f0c62a1155956a0db0e41be2b97e4d75",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Galaga Dual Hero (Type 1 Hack) (7C34B86D).a78
    {
        "4704682ec325f5755e68182f71b6c85c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Galaga Dual Hero (Type 2 Hack) (4BB0D3FC).a78
    {
        "2a04e69ed1edf943a389b317b38daf6f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Galaga-14 (NTSC) (Hack) (20130121) (9996CC26).a78
    {
        "b8eb008a602776fce429bcfb89b27759",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Galaga-14 (PAL) (Hack) (20130121) (C1768CA0).a78
    {
        "046c395d616fce103181502e4025eefe",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        1  // xm
    },
    // Galaxians (Hack) (20181207) (29E0CAE0).a78
    {
        "ee09789d61a693e387ccdc9a2f025b43",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Game Maker Pac-Man (Hack) (20150809) (AA9CDB05).a78
    {
        "86d5dfe26642bbeefe738d32eefaa000",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ghost Muncher, The (Hack) (19EE549D).a78
    {
        "e094dbd52b15cc4d35f78c0d60827bb8",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ghosts Revenge (Hack) (20130501) (C6BB1F60).a78
    {
        "72ebf517fe93efa24645e0a09aa2d23f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Gobbler (Hack) (20130501) (3BFA90CE).a78
    {
        "33205860dcb1530026f4291b03e0d6a7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Griffey The Dog (Fast) (Hack) (20100924) (78041DF6).a78
    {
        "b03a723571a7ed3368417583163bd8a6",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Griffey The Dog (Hack) (20100924) (6498AE41).a78
    {
        "a2a134957ccde7ba2cf08283201bcab5",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Griffey The Dog (Halloween Edition) (Hack) (20101104) (B9376894).a78
    {
        "a4fe6cb10ab147ae9b8dadc80e36252a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Handyman (Hack) (20181115) (A0184D40).a78
    {
        "a73620118f568a0ab9bf6de408f5874f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Handyman (Invincible) (Hack) (20181115) (5304A8E7).a78
    {
        "3593f8a590a102e2a6f299e37e01f3ff",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Hangly-Man (Hack) (20050531) (8560A801).a78
    {
        "46dbc5108151e963b120cdaedd7d6d4c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Hockey (Hack) (20140122) (AE2FA8E7).a78
    {
        "00d8f419c55ebe4069f65a17d7fd47e0",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Humantron 2084 (NTSC) (Hack) (20130425) (4E1BE017).a78
    {
        "61809684eefd6cbb2963574ffb0a3fab",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Impossible Mission (Blue Graphics Hack) (20160905) (E1B7ACA7).a78
    {
        "bf070f04c8fc7ec721b9506b63b48470",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Impossible Mission (C64 Graphics Hack) (20160905) (6DB2C25E).a78
    {
        "7d852889f398c3389acb6441e5b7ce3b",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Impossible Mission (Fix 1) (Hack) (NTSC) (7A545A56).a78
    {
        "5cfd6fa7e2d691c3a4a879219d2d53b1",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Impossible Mission (Fix 2) (Hack) (R_Cade) (NTSC) (2456E0C3).a78
    {
        "a39e00fd187db968f7b747f4e072c4e6",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Impossible Mission (Green Graphics Hack) (20160905) (3A7C61FB).a78
    {
        "4a811d87d1730a334a21e7bda9fe535a",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Impossible Mission (New Graphics - C64 Hack) (20190529) (157CD59C).a78
    {
        "740a0478e0592a9e0635a75ebe90eb61",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Impossible Mission (Red Graphics Hack) (20160905) (804DAFB4).a78
    {
        "05c21a88fd736d59d28b1d95e79840b6",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Inky (Hack) (20150530) (1D42BDA7).a78
    {
        "49e2af0732e4aacac9e63d552bd8a62f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Invader of Space (Hack) (20190128) (2A72593E).a78
    {
        "f4d0eb90fb95a73eb863ffdb5dece0ba",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Invincivaders (NTSC) (Hack) (20121231) (735632F9).a78
    {
        "9e9abfc36ed35117ffb6e40db80d71e6",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Invincivaders (PAL) (Hack) (20121231) (78523DC0).a78
    {
        "5543230e5a0092646d42c4676470663e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Invisible Maze Pac-Man (Hack) (20170407) (EBEB895E).a78
    {
        "a3a4dbd27c80eff9bef51f73cd26f1d5",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Invisible Pac-Man (Graphics Hack) (20110509) (2CBBC6F1).a78
    {
        "c8454693777f05b255528ae829b89925",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // i_Cat (NTSC) (Harp) (Hack) (20130124) (6D467EF5).a78
    {
        "346678062fb9f41d814c9ea46c87d743",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // i_Cat (NTSC) (No Harp) (Hack) (20130124) (AF18AEDA).a78
    {
        "aa655a1c4a03a4a6e26015fcc4aa0c55",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // i_Cat (PAL) (Harp) (Hack) (20130124) (0E6B1BF9).a78
    {
        "fa99949e42f6f41f9489c3860f3d23d4",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // i_Cat (PAL) (No Harp) (Hack) (20130124) (8C085BC8).a78
    {
        "b1d5f3f10193cf9fcc961acafa5a3a96",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Java Ms Pac-Man (Hack) (20130420) (2E675F0A).a78
    {
        "227b1075906386eac795926fca013061",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Jawbreaker (Hack) (20120426) (8F272CDC).a78
    {
        "43a0059ff1b5bf76e0c7023fde7c33a5",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Joust (Broke Hack 2) (20180920) (59C899CD).a78
    {
        "79f91e65f523f66178ef457a8a5a7d08",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Joust (Broken Hack) (20180811) (48D2ED35).a78
    {
        "f545bfe365e50c78a2cd904b5903aebe",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Joust (Deviled Egg Hack - Joustween) (20181031) (91D6E3B4).a78
    {
        "0579561766ae98b046c93ddf4f4e6409",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Joust (Eggs Easy Over Hack) (20181029) (B8780EF5).a78
    {
        "d10d324d4ec34c9129f7d7b5c41b4780",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Joust (GT-R Hack) (20190924) (689AFA66).a78
    {
        "bc036c1c6edbd6254ff1606212ae4b62",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Joust (Lava Hack) (20181219) (25CCF7B6).a78
    {
        "5f768fe49d005c6d1bb6c8862557a38f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Joust X (NTSC) (Hack) (20130320) (8E86834C).a78
    {
        "6b9cb9702d95cc4435c24e66772d82ec",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Joust X (PAL) (Hack) (20130320) (D2BEAF63).a78
    {
        "0c0cc1374b6dc2d24d71c77cfeacd6ec",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        1  // xm
    },
    // Joyman (Hack) (20150807) (C7D55BD2).a78
    {
        "48fcdb7337b2b539ca242d4cb7e7356d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Jr Ms Pac (Hack) (20090705) (535A693E).a78
    {
        "9e16efced49d5df52d78ce8c17dd15a8",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Jr Ms Pac-Man (Inv_Fast) (NTSC) (Hack) (20130128) (7CEEC00A).a78
    {
        "d7056ab905015b7d8307375cea17edff",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Jr Pac 2 (Hack) (20120325) (4188DC5E).a78
    {
        "0592bc0e6bbd51890f80d7b98f323927",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Jr Pac-Man (St Pats Hack) (20100317) (9170FC38).a78
    {
        "17fcadf657c143648d93afc7c9975bb3",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Jr Pac-Man C64 (Hack) (20170407) (CF41B950).a78
    {
        "465b1795f627335c15cbec724c9aa39b",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Kessel Attack (Hack) (20080904) (30841755).a78
    {
        "09f5623b61788bbc7430151fbcb48018",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Kung-Fu Master (Graphics Hack) (RC2) (20160905) (D69464F1).a78
    {
        "4fc0b16e3a5c463790a04fb42a1e4c40",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Le Siti (NTSC) (Hack) (20150919) (5756CD9C).a78
    {
        "ee4d284c2976bf74f3669f120cd2dfb4",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Le Siti (PAL) (Hack) (20150919) (FD85D5BE).a78
    {
        "ce146e325aba4ee1ef3024bb8f378c6a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Mario Bros (Mario Right Hack) (v11) (B23B8093).a78
    {
        "ba1882fa6699c989f513f6f4a3ae02eb",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Mario Bros (Sprite Hack v4) (20160306) (092ECAB7).a78
    {
        "54c073ba8f39ef23f70dc07e76029de8",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Mario Bros 256 (NTSC) (Hack) (20131217) (9C693F0B).a78
    {
        "ed65eee7c1a3733b623d4af0f2ac649b",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Mario Bros 98 (NTSC) (Hack) (20131217) (A7665B4D).a78
    {
        "9eb484247eac3417de607f7be2de0b26",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Mastersoft Munch (Hack) (20130420) (37F82F6C).a78
    {
        "e3e3a50ef6322a1bd98468ad96f31c41",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Meteor Bath (RC2) (NTSC) (Hack) (20131213) (2CE419CC).a78
    {
        "eb3c1443f4a25806de4657e106d504e8",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Midnight Mutants (Colorburst Signal Fix) (Hack) (NTSC) (20210607) (B57D8802).a78
    {
        "8b312716d2049fc9f199945fa60796f0",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Millipede (NTSC) (Hack) (20150405) (F403F2A8).a78
    {
        "c641921c43259f266367e3eeef61d2b7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Millipede (TBall) (NTSC) (Hack) (20150405) (F84711F4).a78
    {
        "03a6219a2e19f82f8e87d5541f30a4fb",
        0, // cart type	0
        0, // cart type	1
        4, // controller 1
        4, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Minipede (NTSC) (Hack) (20150808) (37FABA42).a78
    {
        "5b9b22e43a081fd10222053b1d1901ed",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Minipede (PAL) (Hack) (20150808) (4253387F).a78
    {
        "4431b2674706272d957e644103a556bf",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Minipede (TBall) (Hack) (20150808) (08067930).a78
    {
        "211a8ad9fec38e806194633e30890ec8",
        0, // cart type	0
        0, // cart type	1
        4, // controller 1
        4, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Moon Crest B (NTSC) (Hack) (20131125) (B04FFED2).a78
    {
        "59e8b4d3c840e86aacc6ccacfb71d8cd",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Moon Crest B (PAL) (Hack) (20131125) (22DF546D).a78
    {
        "629e8b89a9c2061900cc61fba65f19f3",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // More Beef Drop (Hack) (20060901) (ACCABC73).a78
    {
        "89f90b661d1b79e956b10bb6a9771f78",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Mr Pac NES (Hack) (20130427) (0E32B124).a78
    {
        "b7841f9a72d8b0766aaef4958049f806",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ms Fruit-Man (Hack) (20130422) (9C8207FA).a78
    {
        "0b2e02a065f2051361b0d505be070eed",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Attack (Hack) (20050531) (5139BC15).a78
    {
        "d8dbb5c4d7d02e6b0627df8f657a13b4",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Ms Pac-Man (Fast Hack) (20060304) (1D6B466A).a78
    {
        "27c133965dfd80b3acb1ed598817aea0",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Ms Pac-Man (Ferrell's Hack) (20170407) (17E0E69B).a78
    {
        "00dfa9f911cd5429847b682bdea22120",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man (Graphics Hack) (20150220) (60C52996).a78
    {
        "de136dfa2f7c95bcb50e4fe8d14e32b4",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man (Intellivision Hack) (20210110) (098B27B6).a78
    {
        "91d59ee00469fbc76274d94f3d7dec73",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man (Inv) (NTSC) (Hack) (20130128) (FE2918AD).a78
    {
        "e55b69bd16383539a5694c17fe6488d7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man (Inv) (PAL) (Hack) (20130128) (A4D927FA).a78
    {
        "e610d5f514a914444394239640d828dc",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man (Inv_Fast) (NTSC) (Hack) (20130128) (D2DF851C).a78
    {
        "e0c77f5fdd8c2d1cfe00ccf8df87e62e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man (Inv_Fast) (PAL) (Hack) (20130128) (E9CF4B9A).a78
    {
        "210c82f45aa52a87cd45d40e56315e2f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man (Pac-Man Maze) (Hack) (20170407) (B4EB18D5).a78
    {
        "6e48a0cda9d845b6dea9bd19b4f8c776",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man (PacManPlus' Hack) (20090501) (F06E8107).a78
    {
        "cf007563fe94cacf5ea5295dc93ce9ef",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Ms Pac-Man (Purple Ghost Hack) (20131226) (DED38E44).a78
    {
        "b5b01e34376ce38d7de3d367b7146c8e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man (St Pats Hack) (20100317) (60B36A41).a78
    {
        "30ba6868df4458206c73852f630be0b3",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man 320 (Invincible) (NTSC) (Hack) (20130128) (3386FEFF).a78
    {
        "db768297985178cec034c12a41d6f1a7",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man 320 (Inv_Fast) (NTSC) (Hack) (20130128) (2CBA6FAD).a78
    {
        "cad1e733986bce1ee4c9da73de1dcff1",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man After Dark (v4) (Hack) (20150904) (DD7B95DC).a78
    {
        "c7492edd3d5eb800b7ff0f448076a1ea",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man After Dark Xtreme (Hack) (20150906) (EB784EF3).a78
    {
        "830b29497f99e80c7f591e61207c26b5",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man Mini Mazes (Hack) (20120325) (173B528D).a78
    {
        "df2a1398bc425162358be5762420200d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man VCS (Hack) (20150901) (8B1FDAB5).a78
    {
        "94bcaaf2337a1e20f4bf7ffe40ec96bd",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man Vector (Small dots) (Hack) (20150905) (81CFE18B).a78
    {
        "dcaad68d5d4eadfce64c09ad2722442b",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man Vector (Standard dots) (Hack) (20150905) (AE4B6AF9).a78
    {
        "f3443f41cfee790eb4ad3cb7b1580f23",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Munch Man 64 (Hack) (20160206) (B2AF0977).a78
    {
        "d285a1265d112fde059e71c729ef98cf",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Munchkin (Hack) (20130629) (8CB0A0BB).a78
    {
        "a8e752d108efceff504ae4edc4766b75",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Munchman Texas (Hack) (20130507) (5D537474).a78
    {
        "04c985ebbd0bea4c557207e69b8cbd8f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Munchman Texas (Multi-level) (Hack) (20130507) (0DDE065C).a78
    {
        "c6e93c17f87b60442924b76d40ee69e7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Num-Munch (Hack) (20181213) (CD577025).a78
    {
        "e539c9484eb3e0c0badf6ae506035391",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Num-Munch (Invincible) (Hack) (20181213) (DD0FE807).a78
    {
        "27a3dacb106078ea8fd067f3f933fe40",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // One-on-One Basketball - Title Fix Hack (NTSC) (20201229) (6E185015).a78
    {
        "ab46638a33635b26876ffb0a21b2786e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac Jason (Hack) (20121101) (885A696E).a78
    {
        "07fccc6025b98915f64fb42b2aa250a7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac Jr (De Lucia's Hack) (20130422) (54C36E8A).a78
    {
        "c5b3c24e08ebbcd9eed41bce74129d6d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac Nestor (Hack) (20060920) (E7790C0C).a78
    {
        "90145d56c6c39d6f33aa151785f2fdd5",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac Pollux (Hack) (20060305) (1B3DD167).a78
    {
        "04edf4f3c6b186147c1117359c8f5076",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Pac-Boy (Hack) (20130625) (BFED8D97).a78
    {
        "0659e61b133e0109d9ea5d290ad731ae",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Jr (Ferrell's Hack) (20170407) (459428D5).a78
    {
        "a5dc75fd2be28a7998415d62c524ddbf",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Maine (Hack) (20130420) (40350FF7).a78
    {
        "6881d31ad3ae6f63534ccafd73478b42",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man (Atari 2600 Homage Hack) (20100812) (3A5E16E2).a78
    {
        "ec2b8804dff40d2edfc5f15cca3c61f6",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man (Fast Hack) (20060526) (96159F91).a78
    {
        "8338eca612eedf6ddec57d54942863e7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man (Ferrell's Hack) (20170407) (A1BF6D94).a78
    {
        "575c18f77a4215332bf56d0080a234b8",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man (Intellivision Maze) (De Lucia's Hack) (20130422) (D712AC90).a78
    {
        "957e719ea72efd25a3aa5cb3389e2dfd",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man (Intellivision Maze) (Ferrell's Hack) (20120406) (E73FAE49).a78
    {
        "2026eeb5af18c456c812e58773fb67fc",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man (PacManPlus' Hack) (20050530) (D5D08D7E).a78
    {
        "72ec68627bb7d879ae35a71d7679f71e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Pac-Man (St Pats Hack) (20100317) (67AB1A4E).a78
    {
        "9b6bbd8410b29501828f5d40762fab03",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man (Vic-20 Maze) (Hack) (20130422) (7C9B977A).a78
    {
        "328feca37561627aeec7b6a4b4f9e68c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man 26 (Hack) (20180217) (E3078B6A).a78
    {
        "8aa9ed0a7bd428320c3ee46fdf10bdf2",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man Arrangement (Hack) (20170326) (4182724C).a78
    {
        "4615390a32101fac129bacb5c7afe7ea",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man Christmas 2018 (Hack) (858ACABB).a78
    {
        "4fb119f6db26380abba03e4ce3ca04c8",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man Christmas 2018 (Invincible) (Hack) (7762553D).a78
    {
        "0b7635d0f39ff97d1e841888e1b23b7b",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man Collection (Encore Hack) (20200716) (AB7E983C).a78
    {
        "1cfa9a3cdf632941b83b647dee812dab",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man Collection (Encore Hack) (Alt) (20200716) (512B662E).a78
    {
        "1b29f0edc76c50b74e6635f91a699318",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man Collection 2 (Hack) (20120411) (25B59C18).a78
    {
        "b1685dcbaf1b578cb1b6643666d813e4",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man Collection Remake (Hack) (20091223) (429F0B42).a78
    {
        "aaae9f6665aa0e1f79afc3b7103a920f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man Collector's Edition (Hack) (20200603) (EBC62B16).a78
    {
        "80351614f85a572e7d99386760281696",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man G (Hack) (20130427) (77B1389B).a78
    {
        "5209e339408f777983f0207697422319",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man G2 (Hack) (20130427) (0BA32839).a78
    {
        "5c727dd455fbe7b7a6ba48a4a62c8ae6",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man Handheld (Hack) (20130420) (CBCF97A3).a78
    {
        "4748a62d5c628fefd28df5de5567edec",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man III (Hack) (20170404) (3A02A4FA).a78
    {
        "1e74f2f107ede21b11c56fa829478f5f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man Piranha (Inv) (NTSC) (Hack) (20130128) (A9970C22).a78
    {
        "fc6f3e6140dcd113c1a27e19002fd3b3",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man Plus (Ferrell's Hack) (20170405) (299EC285).a78
    {
        "1d7d65997d7cd0858e9bee71ded272aa",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man Plus (PacManPlus' Hack) (20050602) (E2DC056E).a78
    {
        "791e55db03903988280388573a2fcdc1",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Pac-Man Re-Arrangement (Hack) (20181204) (09B24A43).a78
    {
        "ca9149b280e840a394b6d66d30e751ee",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man Re-Arrangement (Invincible) (Hack) (20181204) (A3901064).a78
    {
        "ea5d43741202222f16a0571fc34f29da",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man Remix (Hack) (20120419) (FC7BF9FC).a78
    {
        "9f219741c4f878e8004fa7fee22783ba",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man Vs (De Lucia Hack) (20130430) (6B422FD3).a78
    {
        "e8e388ace8368baacdbae77ed7f4be1e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man VS (Ferrell's Hack) (20170730) (B15B5084).a78
    {
        "36a15978009960d091316260c2cfbb54",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man Wide (Hack) (20130625) (B7EC5501).a78
    {
        "61dbb85ef906f77bf92f932400764e6e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Munch Jr (Blue Azure's Hack) (20181210) (82470225).a78
    {
        "86b50e8a9595299150d23acaebe5c822",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Munch Jr (Ferrell's Hack) (20180704) (AAD7A009).a78
    {
        "c60ca07efc65e67ae30201186c6452b3",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Munch Jr (Inv) (Blue Azure's Hack) (20181210) (F82A83E7).a78
    {
        "bce59b1c034008f2fdab58e1cc14f98a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Prototype (Hack) (20130408) (023FE93A).a78
    {
        "4ee23e6f832d22968c3ce21e9320ab24",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Who (Hack) (20130501) (8D02ABCC).a78
    {
        "5da2cb028dea639ea4df5e14a8b3ae4d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pacaroids (NTSC) (Hack) (20140208) (56CBC707).a78
    {
        "87b92ff46da5ce7c84577bc9e1180030",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pacaroids (PAL) (Hack) (20140208) (F54368F3).a78
    {
        "a7810ac9077dab854c4a1faa7bbc5c1a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // PacIIINES (Hack) (20130502) (D7B566EE).a78
    {
        "c2ad95ed5578097d82b35371b56422e4",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pacland (Gambler172) (Hack) (20200416) (BDFEBC32).a78
    {
        "422dc74d0b5ea42673a527713342c8ed",
        0, // cart type	0
        74, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Pacland (JRH) (Hack) (20200331) (483C150E).a78
    {
        "a55fcc397ff18b4a622a67d8749b9a73",
        0, // cart type	0
        74, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pactron 1984 (Hack) (20150124) (85F61CED).a78
    {
        "8cb66c6ed5b379181b1420d8e4758834",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pactron 1984 Extreme (Hack) (20150124) (F248D386).a78
    {
        "c1ac9987a9483e200c338ccbd2ee94b5",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Papa Smurf in Pacland (Hack) (20090830) (A2F48B43).a78
    {
        "de76212b8e1df59eb77d50886df41da2",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Papa Smurf in Pacland II (Hack) (20090915) (87423DA3).a78
    {
        "177409b649e13f15453e0d5fb086f7ca",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // PC-Man (Hack) (20130420) (FD6B4EDF).a78
    {
        "b55e4d255173e5b2c2e620f3186a1ecc",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pedes2Go (Inv) (NTSC) (Hack) (20151101) (D1F1FC52).a78
    {
        "cd870d8b6b8debd55de102ef0622255b",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pedes2Go (Inv) (PAL) (Hack) (20151101) (5A762EAC).a78
    {
        "49819a407eda1fd11a9c48a022a947c7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Pedes2Go (Inv) (TBall) (Hack) (20151101) (445D9FAB).a78
    {
        "9bd66040cbdc20d440298315fe89163b",
        0, // cart type	0
        0, // cart type	1
        4, // controller 1
        4, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pedes2Go (NTSC) (Hack) (20151101) (1AA98EB6).a78
    {
        "4bd5a5e0685fb457743020341b569e2e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // PentaGo (Tall) (Level 15 Start) (Demo) (20230421) (C6F6B973).a78
    {
        "5877bf424151ec07a1845acfb40db209",
        0, // cart type	0
        74, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Pesco (3 Ghosts) (Hack) (20130510) (637B342F).a78
    {
        "5d757ff04e60db712ecc67c85fbb1d0b",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pesco 7800 (Hack) (20130422) (1E71B96E).a78
    {
        "c261fc865fc40845256f640b00296030",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Piranha (Hack) (20090828) (8D3BB14A).a78
    {
        "64ef92ea845c9ea3f07bb2025bb28d88",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Piranha 2 (Hack) (20180211) (581F69E8).a78
    {
        "2178894250239f5fdf585bd0b049ebfb",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Piranha 2 (Invincible) (Hack) (20180211) (2746F83E).a78
    {
        "c87bd82bc3439950908fee629b7eb192",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Plutos (XM Required) (20200627) (77E3B09B).a78
    {
        "74f0283c566bdee8543e4fdc5cb8b201",
        0, // cart type	0
        2, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Pokey Kong (Early Version) (Hack) (20101029) (3F0FEDB2).a78
    {
        "2370f7ce1b91fc775bce3e72454f908a",
        0, // cart type	0
        64, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pole Position 3 (Inv) (NTSC) (Hack) (20151106) (54EB8DF2).a78
    {
        "66bda11d8e70103c5e074316d950a04d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pole Position 3 (Inv) (PAL) (20151106) (F2CE81D4).a78
    {
        "abfc8f63fea2c9d8d55762620ee48c29",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Pole Position 3 (NTSC) (Hack) (20151106) (54147732).a78
    {
        "a91d6d57b4eda96a415fde7ea9c31c89",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pole Position 3 (PAL) (Hack) (20151106) (9631308F).a78
    {
        "06efc52e44230b217690a03dc4705f93",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Pole Position II (Graphics Hack) (20140412) (1474C59B).a78
    {
        "4a0e6fdba3b47fe8a7e618859511084f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Portable Ms Pac (Hack) (20130501) (3E5788F3).a78
    {
        "46d419bd4994136b57467ab35eddd7e4",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Possible Mission (Propane13) (PAL to NTSC Hack) (20050130) (C4B61D17).a78
    {
        "1745feadabb24e7cefc375904c73fa4c",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Q-bert (Unlimited Lives) (Hack) (20060531) (A674B92D).a78
    {
        "2b6947466a9b94142eb62e25902ec9bc",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // RealSports Baseball (NTSC) (Overdump 78SG Hack) (20210124) (61146B68).a78
    {
        "bfad016d6e77eaccec74c0340aded8b9",
        0, // cart type	0
        2, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Robotron 2084 (PAL Hack) (20210428) (73BF0B76).a78
    {
        "45500fe5d433f873d71736baf1959e16",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Robotron X (NTSC) (Hack) (20130114) (8DACD429).a78
    {
        "03935b9a1f2561bada58fcd5d9fd27de",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Rowdytron (NTSC) (Hack) (20130207) (740904D0).a78
    {
        "d27ed8f883af9b4ee3b5570f30e9ff71",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Scrapyard Dog (Graphics and Infinite Lives Hack) (20200612) (87A71A3E).a78
    {
        "5b0c67d4b709050ac56d7a75daad42a6",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Scrapyard Dog (Graphics and Infinite Lives Hack) (Green Title) (20200612) (913C80BA).a78
    {
        "3d430a34fefdd9cbca02c0b961116d02",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Scrapyard Dog (Graphics Hack) (20200612) (EA90B594).a78
    {
        "8b5712fa22f93c5a2f0b7675dd4b6ca4",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Scrapyard Dog (Graphics Hack) (Green Title) (20200612) (FC0B2F10).a78
    {
        "6cb9d737643d262918aa15f0c854eea5",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Scrapyard Dog (Scraps Hack and Infinite Lives Hack) (RC5) (20200612) (E94242D8).a78
    {
        "9fe015d52dc3eec20b1e173ffba1dcaa",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Scrapyard Dog (Scraps Hack) (RC5) (20200612) (8475ED72).a78
    {
        "ec66056d80519f2bc400fe57b433e3b5",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Scrapyard Dog (Unlimited Lives Hack) (20160826) (31FF6CE5).a78
    {
        "0070751edb8bbf4dd4a685f58b5b72c5",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Shark Attack (Hack) (20130420) (D35ED64A).a78
    {
        "95c8a795e30640c9ec82609872c80517",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Shatneroids (Hack) (20160114) (52E8F299).a78
    {
        "c5b7343d6a13478240d8131e836a4e97",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Sirius (XM Required) (20200627) (B0F30A69).a78
    {
        "c044ac97a06a537b68ec116363147086",
        0, // cart type	0
        2, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Slick Pickles (Hack) (20180929) (0E429576).a78
    {
        "f0eb34515aabe7182c51657f8fd2e6e0",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Slime Time (Hack) (20170221) (5AA2BFC9).a78
    {
        "46caeccacc8440d6f28070dc8279e3d6",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Slime Time 2 (Hack) (20180217) (5FB5F0DC).a78
    {
        "48dc427e570a9c33e25635b79c368996",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Slime Time 2 (Invincible) (Hack) (20181116) (5BF5D772).a78
    {
        "a08a7e7c5e3c9bb3ea5bca8b62da0bba",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Slinkipede (NTSC) (Hack) (20181210) (7FCF63F0).a78
    {
        "8dd01b209dd69764bafc2778ff4d9df5",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Slinkipede (PAL) (Hack) (20151206) (E9E0BAD6).a78
    {
        "d521c36500f0b8bbd3e0926561c02618",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Slinkipede (TBall) (Hack) (20181210) (A0680811).a78
    {
        "9ffc20714037e63f6cdf9c646e0660cf",
        0, // cart type	0
        0, // cart type	1
        4, // controller 1
        4, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Snapper (Hack) (20130420) (C2482DC5).a78
    {
        "0a6809b11d292a52e4cfd3de7719e801",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Snow Day Jr Pac-Man (Hack) (20090725) (F54B456A).a78
    {
        "f4a0f803723d8387569e0537e452a40a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Space Junk (NTSC) (Hack) (20131209) (8E69F9AC).a78
    {
        "824ebf690ca6e2dc94d8501c23258b4a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Space Pong (NTSC) (Hack) (20131130) (C835AF15).a78
    {
        "2839e2479126de0dc6ea428ec20da99a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Specman (Hack) (20130420) (B22255EB).a78
    {
        "22d8124b29a59dc073cc099bf622e05d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Star Wars 7800 (Hack) (20070927) (67C8AF57).a78
    {
        "f5ea8fec9efe14307e2b09956a8ad1c2",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Star Wars X (NTSC) (Hack) (20130304) (6EF206E0).a78
    {
        "2d69c452d090d238812c7bcc3941b856",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Super Cobra (Hack) (20120503) (A9F61352).a78
    {
        "f41f651417c234104d37296477fa29eb",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Tank Command (Color+Sprite+Title) (128K) (Hack) (20141225) (EB52D83E).a78
    {
        "3632fcc732a33591b91f0eea2c01e599",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Tank Command (NTSC) (Overdump 78SG Hack) (20210124) (9F9E1927).a78
    {
        "44f862bca77d68b56b32534eda5c198d",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Tank Command - Midnight Run (Hack) (20141225) (6CE845CA).a78
    {
        "d0b87d349d6d5e40920cc4ff95253339",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Titanman (Hack) (20130420) (BEB827E2).a78
    {
        "e144d2ef872ceb008c538a36c80e664d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (49ers Edition) (v2_19) (20221121) (4F877C42).a78
    {
        "c3c484864c05eefeb51524f57924fcd7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Bears Edition) (v2_19) (20221116) (2FE09256).a78
    {
        "e116ac20244043a630ce62564bcc8eb1",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Bengals Edition) (v2_19) (20221118) (EACE01A9).a78
    {
        "be074ea38501df9e6cf2a9f76cdfc5ea",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Bills Edition) (v2_19) (20221116) (E5958266).a78
    {
        "753f530048cfce06678cd0b324ef8a7e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Broncos Edition) (v2_19) (20221119) (D3ACA25C).a78
    {
        "db4684b8bf87db7d5f6d5229ddfd3381",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Browns Edition) (v2_19) (20221117) (957DAE6E).a78
    {
        "6e8c1b8996c004d5d2980df73761d73b",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Browns-Alt Edition) (v2_19) (20221121) (123DD0BA).a78
    {
        "79983669f8c8e5c573fc14b8a2001743",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Buccaneers Edition) (v2_19) (20221122) (669ADA25).a78
    {
        "326afb177b96a39f59e2993dbdc86ad5",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Cardinals Edition) (v2_19) (20221118) (E14C21AF).a78
    {
        "ad3e123e1aa897bdfdcc8c1a6cf54feb",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Chargers Edition) (v2_19) (20221120) (29B39A66).a78
    {
        "13e13bcf7aa709d683f5b56073069e89",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Chiefs Edition) (v2_19) (20221116) (11A22B46).a78
    {
        "c3ce065f9a3467f6383c2ec169a1de3e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Colts Edition) (v2_19) (20221117) (FEA9B31E).a78
    {
        "b12fb715a2b9a11a058132d1f3bea400",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Commanders Edition) (v2_19) (20221120) (D87C37B8).a78
    {
        "df72cf8f27ade7a6cc5f6fb7250fa04a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Cowboys Edition) (v2_19) (20221119) (DB46A143).a78
    {
        "442d338356a2e71a4f7fb2f0591122b8",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Dolphins Edition) (v2_19) (20221122) (773721F5).a78
    {
        "a46c4f483ad306c67f1d674b19c69eb3",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Eagles Edition) (v2_19) (20221118) (813DC469).a78
    {
        "50134f19e799228a5fe0c4c3d7210594",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Falcons Edition) (v2_19) (20221117) (FECECD9F).a78
    {
        "e13570473e03a68aea754757d5ec5555",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Giants Edition) (v2_19) (20221120) (B08FBA04).a78
    {
        "fc6c4a7c7c137d7ad4c9c11c6ca3c8ef",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Jaguars Edition) (v2_19) (20221120) (E1DB58DE).a78
    {
        "d325635a04781501693fc1bc289e9eb9",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Jets Edition) (v2_19) (20221120) (360005FA).a78
    {
        "62eb20802525f9982ad69a10704aa2f3",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Lions Edition) (v2_19) (20221117) (870E19C7).a78
    {
        "97317bafc5991bac7c536051237d4d79",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Packers Edition) (v2_19) (20221117) (83B2D0B0).a78
    {
        "1ecf1f7c13e454c0e2af71082ea8ba27",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Panthers Edition) (v2_19) (20221118) (748E5430).a78
    {
        "49d71169e987c86d1c273bdbe71fdc7c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Patriots Edition) (v2_19) (20221122) (95646CB8).a78
    {
        "8f2f7d937e637d277848c23c58ff9e11",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Raiders Edition) (v2_19) (20221121) (DF00A7E4).a78
    {
        "5306aa4c4334ea69152610286c17329d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Rams Edition) (v2_19) (20221122) (BA8765BF).a78
    {
        "edf2a61deca579e911c92d98c80db4cd",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Ravens Edition) (v2_19) (20221119) (F8364F31).a78
    {
        "cc7206cd218df18c1792e7b93408e027",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Saints Edition) (v2_19) (20221118) (137C73DC).a78
    {
        "1572e4e4a32ced8d6a70d28c9fbf5ea9",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Seahawks Edition) (v2_19) (20221121) (2548EC79).a78
    {
        "8d37a0be8fa60b6ee2dbe1f01c818964",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Steelers Edition) (v2_19) (20221122) (7E8D6F2C).a78
    {
        "33e3c1c0d629d5e244a5d9cd648d24ef",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Texans Edition) (v2_19) (20221118) (A1FBE8CF).a78
    {
        "c7ccbae6aac326bcd345aff0c87d001a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Titans Edition) (v2_19) (20221121) (8804BE94).a78
    {
        "a2bd4acaf7cbe1f288d59cc79a301e63",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Challenge (Vikings Edition) (v2_19) (20221122) (582247C9).a78
    {
        "32555bb94b6071a6b28ab1044070312a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Tower Toppler (NTSC) (Overdump 78SG Hack) (20210124) (F2D480CB).a78
    {
        "d12e665347f354048b9d13092f7868c9",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Trix Brix (No Level 13) (NTSC) (Joystick) (Hack) (20131118) (3766A277).a78
    {
        "32e937e7796db3a01e9bcf5fe93929b0",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Trix Brix (No Level 13) (NTSC) (Paddle) (Hack) (20131118) (3766A277).a78
    {
        "32e937e7796db3a01e9bcf5fe93929b0",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        3, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Trix Brix (NTSC) (Joystick) (Hack) (20131118) (5B50FA00).a78
    {
        "3209039148e0b7a2b1927bd05bae4685",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Trix Brix (NTSC) (Paddle) (Hack) (20131118) (5B50FA00).a78
    {
        "3209039148e0b7a2b1927bd05bae4685",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        3, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Turbo Maze Man (Hack) (20160617) (8F0984F8).a78
    {
        "c90569d588bfafcc4300d7f02a7ab26c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // UFO! Genesys (Hack) (20181118) (05371317).a78
    {
        "43f8e9cec3d9991017709f48a7aa22f6",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ultra Pac-Man (Fast Hack) (20060526) (C5E82BBD).a78
    {
        "040a0260e1e0beb697edf8b6731507b4",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ultra Pac-Man (Fast Unlimited Lives Hack) (20060526) (BE2F2D21).a78
    {
        "04ec2031be7e877d28be8c90c54d6cc7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ultra Pac-Man (Ferrell's Hack) (20170405) (833D2AF3).a78
    {
        "9e75c6fb771b43f1de62cd42f8112a4a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ultra Pac-Man (PacManPlus' Hack) (20050619) (4BAEAC35).a78
    {
        "f85d506f5933427c8de664be0c5510a3",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Upside Down Ms Pac-Man (Hack) (20090718) (A9AF82D7).a78
    {
        "cb6b01ad69369d5f3824b5a2e7a63260",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Upside Down Pac-Man (Hack) (20090718) (1498DAAF).a78
    {
        "f83849cf3f5ac95856e8f93ee90d5a8d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Vector Ms Pac-Man (Hack) (20120325) (F4BCCB73).a78
    {
        "d2379d9ec4307b6da74c47fbd02aee65",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Waka Waka (Hack) (20130503) (7867D912).a78
    {
        "26259622f3d367b45e8dc1ff51ea005b",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Water Ski (NTSC) (Overdump 78SG Hack) (20210124) (9F491FA0).a78
    {
        "acf63758ecf3f3dd03e9d654ae6b69b7",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Water Ski (Title+Color+Realign) (128k) (Hack) (20130224) (F427A5FC).a78
    {
        "40913dcf24a623c1dc2495a1c4931b48",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Water Ski (Title+Color+Realign) (Trained) (128k) (Hack) (20130224) (786D8CB5).a78
    {
        "8b49549763f4f0e42a23942b8df6b248",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // World Cup (Hack) (20140614) (13A00C23).a78
    {
        "a7e882ae4754b90650939af33724b3a0",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // WWE Title Match (Alt Colors) (Hack) (20121010) (3B97A4F4).a78
    {
        "cdca32a7d30df5cf301723bcd3625dc0",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // WWE Title Match (Hack) (20121010) (1298A809).a78
    {
        "c1b6a707005f86c2c074bc11a7d9004e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Xevious (Title and Color Hack) (20130319) (20E8A78A).a78
    {
        "4212c009c6d8cec963929e61456cb8eb",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Xevious X (NTSC) (Hack) (20130304) (741EB079).a78
    {
        "88bccaca6de1117d03b39c613537c2ab",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Xevious X (PAL) (Hack) (20130304) (D94DE9B9).a78
    {
        "7db031f1c4dc957719812fe68ee42531",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Xmas Time (Hack) (20061225) (394ADC12).a78
    {
        "8e0c5fc77b27422a39d86ac2e57dc73d",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // YPS Quest (Hack) (20140712) (D32D2C60).a78
    {
        "bc1f56d7cc14f15ddfcba5e21e19937b",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // 1942 (20221005) (F9E37618).a78
    {
        "fd9353d42cca5f81fe7af866592b94c3",
        8, // cart type	0
        6, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // 2048 (RC1a) (20211113) (7432A34A).a78
    {
        "6f157f421c7ed5d952b393122d37915e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // 7800 XMAS - Santa vs The Nightmares (20201219u1) (1A114E52).a78
    {
        "63f9217bbd3ba91c1fc09fda18de9275",
        0, // cart type	0
        70, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // 7iX (20220305) (A8F12175).a78
    {
        "a837a34f540fd1371bfcfb8e8af4c375",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // A R T I (v102) (Public Demo 2) (20221228) (3A69C0D6).a78
    {
        "6ce16ea13adb3f880d3749d60c8b707a",
        0, // cart type	0
        70, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // A Roach In Space - Part II - Electric Bugaloo (20201119) (D8005F18).a78
    {
        "ff056f2858f14fc4725fcb0015d78d3b",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ah Zombies (v03) (20221118) (D0DD190B).a78
    {
        "909f8879f9b8c47f3b487e402c70227c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Alpha Race (20150920) (DC81DDD0).a78
    {
        "10ff87ebfeec99858cc80293b0c1686e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Apple Snaffle (v1_30F) (20100803) (3DD0C2B7).a78
    {
        "510ea66b6375a848a21db019b36078dd",
        0, // cart type	0
        3, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Arkanoid (ChunkyPixel Games) (Demo) (20210103) (48E43794).a78
    {
        "e1da4c3ea0d26ae0a893741b49be2274",
        0, // cart type	0
        70, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Arkanoid (Propane13) (20110911) (3F544EDB).a78
    {
        "0a9e58ef5eb9ff93246e0fff684dc7f1",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Armor Attack II (20230627) (6CF67401).a78
    {
        "eea04359df6770d66b0d97c2cea1932f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Asteroids Deluxe (NTSC) (20071014) (56239D62).a78
    {
        "a65f79ad4a0bbdecd59d5f7eb3623fd7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Asteroids Deluxe (PAL) (20071014) (AB72EA89).a78
    {
        "1baf41de200f26ec643625021290bec2",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Astro Blaster (20230627) (FB86A5A0).a78
    {
        "55ffe535897c368be7a80d582f6a68cb",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Astro Fighter (20230627) (C7133EA4).a78
    {
        "b4be9c25ea078608310f0ddc409e7cc1",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Atlas (20111023) (045FE06D).a78
    {
        "a6c02ae92d6937cb885f6909c0a8a2e1",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Attack of the Petscii Robots (Demo) (POKEY 800) (20230216) (C4209106).a78
    {
        "a662862f20362fc5eb5c651065cbd51c",
        224, // cart type	0
        2, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Baby Pac-Man (20230627) (4E4C4D2D).a78
    {
        "2b31dfab41dce110dd136fd06606e1ca",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Beef Drop (Final AtariAge) (055CD1E8).a78
    {
        "78b1061d651ef806becac1dd3fda29a0",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Bentley Bear's Crystal Quest (NTSC) (20151220) (D44622B3).a78
    {
        "34483432b92f565f4ced82a141119164",
        0, // cart type	0
        74, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Bentley Bear's Crystal Quest (PAL) (20151220) (CFA247F5).a78
    {
        "1926b9b322ac0f8f36e119b524aa48bd",
        0, // cart type	0
        74, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Bernie and the Cubic Conundrum (Alpha 10) (20211225) (EEDB9115).a78
    {
        "b11b1a2bae8a1d0cd1c180798c6e6169",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Bernie and the Tower Of Doom (Public Demo 1) (20221229) (2D95E4FF).a78
    {
        "625d64a744455836c3d9962803afcb9c",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Binary Land (20210208) (AD20DEAA).a78
    {
        "a541b16e95619afa4960487c23a9a9ff",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // BlocDrop (20130310) (9882A876).a78
    {
        "fb805af5b8ee40203df122e21dd94018",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Bomber Hero (20230111) (03F460FE).a78
    {
        "e7ae5372a741defb8346ca750ad9e94b",
        0, // cart type	0
        2, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // BonQ (Final AtariAge) (E330FA66).a78
    {
        "9fa7743a016c9b7015ee1d386326f88e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        2, // save device
        0  // xm
    },
    // Boom (v1_5) (20150526) (DBD45794).a78
    {
        "4a8a22cff154f479f1ddaa386f21fc39",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Breakout (v08) (20230315) (55D20479).a78
    {
        "c0b354044148bf9c5b3427e9c3c2702b",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Bullet Festival (20150831) (ED33EDA6).a78
    {
        "9f5bbb4b42b4042f6f465953710788b6",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Cannon in D - D for Defense (Demo 03) (20220130) (3C0C960C).a78
    {
        "000b5888d2489f7e256d80a0848ecd14",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Captain Comic (20200926) (F26C9CDD).a78
    {
        "51d2a23152ad23d734d7ef1e36fa367d",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Cartesian Chaos (v11) (20221218) (56CC4AA6).a78
    {
        "825c03c049306c16bd654d9d0e344cf3",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Chase (20201231) (C2C34433).a78
    {
        "a4b5d742860beb25c29def4530194c1e",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Chicago Basement (20160914) (709852DB).a78
    {
        "993d034677b88612835bc8e566c578be",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Clean Sweep (20160208) (4BD3D55D).a78
    {
        "3f87a884858870efa98731834ba42c0e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Conversion - batari Basic Demo (20210409) (32867785).a78
    {
        "5eda75d54e9aa92992513d9e691d5a5c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Cosmic Cabbie (20201229) (BAA151F9).a78
    {
        "60144e642ed0ace13e50e9b19e6ae723",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Court Pooper (Demo) (20220617) (FD9FEDB3).a78
    {
        "26b55d71ba1d62fb2c71efd86c83f030",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Crazy Brix (Joystick) (20160427) (32A87B61).a78
    {
        "299d31c8e181fdd011df2014451bdf0f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Crazy Brix (Paddle) (20160427) (32A87B61).a78
    {
        "299d31c8e181fdd011df2014451bdf0f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        3, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Crazy Otto (NTSC) (20230627) (D543ABB0).a78
    {
        "100551363027dc5f093d049a5fd00933",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Crazy Otto (PAL) (20230627) (CAB285AD).a78
    {
        "8966ba51ceacf7c0769dc1003b3b9fc0",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        1, // save device
        0  // xm
    },
    // Crazy Tank (20180310) (54089FF2).a78
    {
        "156be6e5854b2681a773ffb52e42b079",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Cubicle Chaos (20141205) (9D0B4951).a78
    {
        "bfb5ac5f3de019e7aff8146bcc1a3142",
        0, // cart type	0
        3, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Cyb Ur (RC1) (20190526) (88846A7E).a78
    {
        "40bd21c9698c6b8e71b703f860c11359",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Danger Zone (RC-4C) (NTSC Demo) (20201231) (77E52797).a78
    {
        "0c2f248a1ae9bfd14b1bcc1bd9f3a41e",
        0, // cart type	0
        64, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Death Merchant (v1_30) (20220709) (DFE8E67A).a78
    {
        "fab7b59dd580dce0b28be3e74a7b8433",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Defender (20220128) (03522A22).a78
    {
        "c19454c55a15d5676a960a0615b4aa6b",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Donkey Kong PK-XM (NTSC) (Demo) (v1.2) (20230619) (12E63725).a78
    {
        "dd1cfc933d2bfacc613ef745c1366438",
        0, // cart type	0
        74, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Donkey Kong PK-XM (PAL) (Demo) (v1.2) (20130905) (9F6C0600).a78
    {
        "df700753d8ba9353a7045868778eef6d",
        0, // cart type	0
        74, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Donkey Kong Remix (Demo) (POKEY 450) (20230216) (63C13C43).a78
    {
        "312363c7691fa51ef3a1e53134ff2e2c",
        0, // cart type	0
        70, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Dragon's Cache (20210207) (F163C3E6).a78
    {
        "7b7825ca2c79148f1c4ade6baacc1a76",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Dragon's Descent (20210731) (9478EB46).a78
    {
        "77164df89ae49b4dd72906a21e787233",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        2, // save device
        0  // xm
    },
    // Dragon's Havoc (Demo Dec 2022) (20221231) (25E45E5E).a78
    {
        "8c2798f929a43317f300d3ccbe25918e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Draker Quest (Beta 4) (20150727) (6DAC084D).a78
    {
        "fab1290f9a4c4f2b4d831c8a57f969f5",
        0, // cart type	0
        3, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        2, // save device
        0  // xm
    },
    // Draker Quest II (20170619) (2ECDA8A0).a78
    {
        "a9f29004412621f20ad9f5c51cc11486",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Drone Patrol (v0_51) (20230513) (02C5A505).a78
    {
        "e1dc742074ff6568760472808711c3d1",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // DrunkWitch (20221231) (F745683C).a78
    {
        "c73d04c367d2bed7f1e57bddd0662f67",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Dungeon Stalker (20151022) (BA519F24).a78
    {
        "b3143adbbb7d7d189e918e5b29d55a72",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // E X O (RC Demo A) (20211225) (5047384F).a78
    {
        "a44e8b7b7881beb0fe3c71a1a04441c8",
        0, // cart type	0
        70, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        2, // save device
        0  // xm
    },
    // ET Book Cart (Demo) (20150524) (32CE5325).a78
    {
        "253cd7325a1f454b68eede5136805f30",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // FailSafe (NTSC) (20100227) (67EBF1ED).a78
    {
        "6287727ab36391a62f728bbdee88675c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // FailSafe (PAL) (20100227) (56AC5864).a78
    {
        "84c4b4ed75f41417ac7cbceac71e3856",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Fat Axl (20160613) (9D14DC33).a78
    {
        "0338fffc3391f74d30e6ab391d1b4c25",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Flappy Bird (Beta) (20141221) (E6E1C828).a78
    {
        "bac78b53f4f6b3f68c0f8073f0073179",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Freeway (v0_88) (20221010) (7D03A3B2).a78
    {
        "c8efc272ca334fa8196ab0542882d78e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Frenzy (w-Berzerk) (20211025) (3A554710).a78
    {
        "26031dea7251fb861cb55f86742c9d6e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Froggie (NTSC) (Final Release) (20180920) (224B93D4).a78
    {
        "6053233cb59c0b4ca633623fd76c4576",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Froggie (found by DEANJIMMY)
    {
        "c41854b0001935372f26803129337e5d",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Froggie (PAL) (Final Release) (20180920) (224B93D4).a78
    {
        "6053233cb59c0b4ca633623fd76c4576",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        1, // save device
        0  // xm
    },
    // Frogus (20221020) (0AFE5AFE).a78
    {
        "9daaac9b25783a7e3c8858f3987ed18d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Galaxian (20211118u1) (01347055).a78
    {
        "2f4ae1015a345652b36004a8c62a4ac6",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Game of the Bear - Polar Opposites (RC1) (20230211) (EAFE6161).a78
    {
        "c2e131a091ceed2e04e71f19219b7804",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Get Lost! (20210407) (06F069D1).a78
    {
        "5483748fcf63464250277cae99348221",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        3, // save device
        1  // xm
    },
    // Ghosts'n Goblins (20211205u1) (EBAA96BB).a78
    {
        "ffc704d30566a2c24d6e9d845e4c9f11",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Gorf (20091213) (AD5ACC79).a78
    {
        "3d12489c553cb1a90c8ebd6534383fa1",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // GoSub (20200506) (EC8A9736).a78
    {
        "e443f7fb5be3283dd44c0f5d80c3a7b3",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Graze Suit Alpha (20170910) (2AC2E398).a78
    {
        "1e21bf1d9d7b3c0cebaac576964c9eb2",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Halloween Last Stand (20201102) (95C868D4).a78
    {
        "9fd44e8c7ead334cd0308f4dbbaf7b56",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Harpy's Curse (20221222) (C7E2F31D).a78
    {
        "7a3eefc7907a5ebc7835d9b9f8ac78c4",
        0, // cart type	0
        70, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Harry's Hen House (1_07F) (20100117) (983BE828).a78
    {
        "3f14e9b07f9809867facd592ddcda41f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Heartlight (v0_98c) (20221108) (BC6A8F89).a78
    {
        "8362cfc1a62d8172c7adb9d867094b5d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Hearty Manslapper (20161129) (349136F9).a78
    {
        "25ce1f5dfc909bcb46086e414d6a0f30",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // High Card Draw (20220303) (A498EB69).a78
    {
        "fbf0154e8d8be2a618e61676ba7d8add",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Hollywood Brawler (20160811) (7ACF469F).a78
    {
        "26578cedccc6c61984d5a55048a42d7d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // I C B M (20230507) (C3521748).a78
    {
        "e5bde14cbfa45d791587cbbef309c98b",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // I Ran (Demo 01) (20230429) (66A752DF).a78
    {
        "d85564ef05d607df034ba0231989f5c1",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Iron Grip - Destroyer Of Worlds (20210518) (BCA56BC5).a78
    {
        "571875af24243cfea7b13796096884c1",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Jacks or Better (v1_37) (20221210) (3495BADA).a78
    {
        "64717953882a4aebe463f453ca00b9be",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Jr Pac-Man (NTSC) (20230627) (1B79AA55).a78
    {
        "bde3abe40d302d8c4c65c9690c05dbc4",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Jr Pac-Man (PAL) (20230627) (B039B160).a78
    {
        "e274e7285bb8f97d4d9acddc8497ed9e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        1, // save device
        0  // xm
    },
    // KC Munchkin (20230627) (6BD7388C).a78
    {
        "aa0b9560d6610378bda58f09f265d6ad",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // KC Munchkin (Alt Movement) (20170409) (83CF579D).a78
    {
        "927edf157f88b8f5863254d9a65f05a8",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Keystone Koppers (Demo Dec 22) (20221222) (FCB02B92).a78
    {
        "1c9deabc48f07d1bf2c68731fccd27b5",
        0, // cart type	0
        70, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Knight Guy - Quest For Something (20210423) (88980038).a78
    {
        "3ec728e116017be89c552a85a8f86d90",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Knight Guy In Low Res World - Castle Days (RC 01-1) (20201001) (9B0230C6).a78
    {
        "33dbb58f9ee73e9f476b4ebbc8190c88",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Knight Guy On Board - 30 Squares Of Fate (20210116) (14BA6C45).a78
    {
        "1d47c3802135d864dc1d922ec27aa708",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Krull (v06) (20220319) (5168B651).a78
    {
        "cf521d4eb0d74e87f24e65a32ba15038",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // LadyLady (20200810) (DA306095).a78
    {
        "02da71dc8beac00dd556d7b33b1edfb6",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Legend of Silverpeak (v1_01) (20220714) (55238725).a78
    {
        "271864e0978278a3e2fb04273db69d57",
        0, // cart type	0
        2, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Lemmings Squidginator (20200330) (954062A5).a78
    {
        "8dda6b319c28cf4e005c9d33de1b04be",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Lunar Patrol (20221231) (9D4C7C5A).a78
    {
        "79cd8bc8e4d4327d81808e7cf5a89f68",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Lyra the Tenrec (20230418) (9F806A0E).a78
    {
        "762809470c3fecd63c3b27be95b46375",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Merlain 7800 (20191102) (EE27F0F6).a78
    {
        "181a9978d9da7a7e21f770808cc681f2",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Meteor Shower (NTSC) (20120218) (1A6B1E88).a78
    {
        "c3f6201d6a9388e860328c963a3301cc",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Meteor Shower (PAL) (20120218) (EA240BF3).a78
    {
        "dc0bf52475030c05671dd187e9a99f08",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Millie And Molly (Demo) (POKEY 450) (20230305) (8CFED8AC).a78
    {
        "3f80432f156088bf328cff15842766ee",
        0, // cart type	0
        70, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Monster Maze (20220306) (4AFE78EA).a78
    {
        "1c860298a8966cc8e176ab8453b172c3",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Moon Cresta (NTSC) (20230627) (7E4AE0C3).a78
    {
        "3a15fe7bead3d9b90f3fb13d63e1ee42",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Moon Cresta (PAL) (20230627) (A6C810B2).a78
    {
        "3c05784f55167a10028225d20b38fef5",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        1, // save device
        0  // xm
    },
    // Morf (20220314) (BC2124C6).a78
    {
        "d1b56eae7227c12d0122bb84925c89c0",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man Twin (20160208) (768817BE).a78
    {
        "a69347c8a681b8e94f79d8d848998007",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Ninjish Guy - Perilous Island (20211107) (8EF0F551).a78
    {
        "ac5c99ac01c96ad92832c0544889a702",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Number Crusher (20210523u1) (33090508).a78
    {
        "da4b11103175dbf72dd631d6faf78946",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Oozy The Goo - Gaiden (Demo 4) (20220717) (ECF5F8EA).a78
    {
        "daaf3b784e4f0f2949dfcc2c9602f491",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Orion Assault (vA02) (20220522) (2517A21D).a78
    {
        "45e1aa23828c32f7de88a5ccd1fec8b2",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man - Energy Drink Edition (20230322) (60BE7493).a78
    {
        "2b51ebf2f371d0790a5629e27727ebba",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man Collection (POKEY 4000) (NTSC) (20230627) (BCAB7DE6).a78
    {
        "8b132c1c1e629c2b12d76237c8a11e01",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Pac-Man Collection (POKEY 4000) (PAL) (20230627) (C2E40BE5).a78
    {
        "9bb2e38d30e8ec1806e8042efd4c0744",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        1, // save device
        0  // xm
    },
    // Pac-Man Collection (TIA) (NTSC) (20230627) (0542BFF8).a78
    {
        "290dbb0fa08f4750a5934bbdc27dc5bc",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man Collection (TIA) (PAL) (20230627) (20A0873C).a78
    {
        "d622c9de8a823ba1c079510cef6ed817",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Pac-Man Collection - 40th Anniversary Edition (20230627) (8FE553A1).a78
    {
        "39dc7f6f39f9b3e341a5ffea76e71fb1",
        8, // cart type	0
        2, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Pac-Man Collection - 40th Anniversary Edition (Short Mazes) (20230627) (B9FAD466).a78
    {
        "2686f20449c339f7d31671f1cdec7249",
        8, // cart type	0
        2, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Panda Racer (20151224) (7D702BC6).a78
    {
        "b5c9f0bf5b5763a923b7f370376b1849",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // PentaGo (Demo) (20230422) (1C2CF2C5).a78
    {
        "6ac5a7f8b6a3198ed08abb9866753763",
        0, // cart type	0
        74, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // PentaGo (Tall) (Demo) (20230422) (AAA1069E).a78
    {
        "badb455b8087b39d66890fe4dd11fe9b",
        0, // cart type	0
        74, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Pineapple (20220619) (70991C78).a78
    {
        "5e92f7926a0d4a8603b1a966cb79e8b6",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pirate Cove (20220415) (6A6678BA).a78
    {
        "a6b91a759b34f6c61462195fc783b6db",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Plink (20201213) (F945CBD9).a78
    {
        "851f901cbc78824f6754f362c00ae648",
        0, // cart type	0
        6, // cart type	1
        3, // controller 1
        3, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Plumb Luck (20170702) (5191005B).a78
    {
        "9e9e4b53f5dcb42f61579fdf0d438921",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Plumb Luck DX (RC2) (20230410) (A5D8E4FC).a78
    {
        "0254afa887fcfc8c4b1a63b41b9ba613",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Poetiru (20150901) (65804DD9).a78
    {
        "f0abd58c1de2ba3e577f90c3d52dd7fc",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pong (v0_7) (20221210) (8FD41239).a78
    {
        "514b0e645b4771ba43cd00f679315224",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Popeye (RC 3_5 Demo) (20210303) (E7FC3488).a78
    {
        "bf38e9f6bfccba51f46bf9443bd1892e",
        0, // cart type	0
        74, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Portal (PAL) (20210331) (9E5AAC54).a78
    {
        "106c42d1cb70f3c05a14382f2a126f85",
        4, // cart type	0
        70, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // RatTrap (Demo) (20230307) (4492F203).a78
    {
        "d9dce76429009ac8facf1af9f3ef33a5",
        0, // cart type	0
        74, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Realm Of No, The (Book Cart) (20161224) (4C327C8C).a78
    {
        "4d00e5bbcf1e39e4fb94e6e5cf127f93",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // ReZolve (20141207) (F7D8F766).a78
    {
        "f2047b149e72be8f97e9671314748ec4",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Rider of the Night (20190711) (5A2A2DCE).a78
    {
        "e1b01dd7e842d2b682ef48f689d5a4eb",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Rip-Off (20230627) (AC056F59).a78
    {
        "803743fe18600f292456539906464421",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Robbo (20160513) (CFE2179A).a78
    {
        "b6561537290e6e25e1249394366c3c63",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Robot Finds Kitten (20050619) (FCB76ABE).a78
    {
        "9646d482a8ad187ae52cd21c0de365fb",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Robot Finds Kitten (224 Lines) (20220331) (A7D9EFB4).a78
    {
        "8994a0560ab6d452cfe59e33de145a3d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Robots Rumble (20220217) (3CE780A0).a78
    {
        "fc525819ec2bdc4a30bb2e55524f8d81",
        0, // cart type	0
        70, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Roof Pooper (20161006) (2ADAB9A3).a78
    {
        "45c33c3799539545e8bf37719eb6f88c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Santa Simon (Valid Signature) (20170511) (5D8CFDB7).a78
    {
        "2582f89d53a899db0b70160ad5762cb7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Scramble (NTSC) (20230627) (753DCA0A).a78
    {
        "31b20a4710e691300bb4aa62cf02284c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Scramble (PAL) (20230627) (298897C8).a78
    {
        "d79a44f51f21fc5bfe4448453d402d30",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        1, // save device
        0  // xm
    },
    // Serpentine (20161029) (D9660CDC).a78
    {
        "9bd70c06d3386f76f8162881699a777a",
        0, // cart type	0
        70, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Shoot the UFO 2015 (20150328) (15EEAD4B).a78
    {
        "19586366f625db75b145f62c1d668f8e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Sick Pickles (20171202) (632696DA).a78
    {
        "96f69b85e0b43bbebbbd59bb8276a372",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Sky Scraper 2115 (6f) (20171019) (66574A8B).a78
    {
        "1c8139c584e1cf5c6afdd2f3455a2446",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Slide Boy in Maze Land (RC1) (20210515) (39AE3804).a78
    {
        "40567f50c569a60cc461cdf0e2853ff4",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Smasteroids (20140712) (8ABCA713).a78
    {
        "4cabc9042ec5097a3136cb680710ec52",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Soccer 2 (20140614) (ECF6AA7E).a78
    {
        "5fb5d1452de1c3fb2d74f8982cf8955d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Space Chess (Demo) (20230127) (224F3D84).a78
    {
        "42ff0f1c55ba940910143c2d50e1406d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Space Duel (NTSC) (20071014) (EF2D2560).a78
    {
        "771cb4609347657f63e6f0eb26036e35",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Space Duel (PAL) (20071014) (0E6A8443).a78
    {
        "a84c1b2300fbfbf21b1c02387f613dad",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Space Invaders (NTSC) (20230627) (BE900754).a78
    {
        "783d09ad9542b0bb28aa4cc6ffcf8aa6",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Space Invaders (PAL) (20230627) (BE900754).a78
    {
        "783d09ad9542b0bb28aa4cc6ffcf8aa6",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Space Junk (1b) (20170927) (9F4B7138).a78
    {
        "cd358a8502bd65702da040f78575fad0",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Space Peril (NTSC) (v8) (20210907) (FA11DA80).a78
    {
        "91f4cb1f642ff1de936a74672dce7198",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Space Peril (PAL) (v8) (20210907) (51054662).a78
    {
        "42e05315e8566ba471a1d8112c0ca296",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Space Race (v0_96) (20220925) (E59C249D).a78
    {
        "9a11430d4eaeb3f7375bc2b92353c5db",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Spacewar! (20070321) (38DB0756).a78
    {
        "f7f53ef9cfb32318e8bc11a9f33390c4",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Spire Of The Ancients (NTSC) (20201223) (13C1F46B).a78
    {
        "19844117863cd38d4e1e4cbc867ae599",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Spire Of The Ancients (PAL) (20201223) (A93D6306).a78
    {
        "43eaa0447144c4c6769bca1a1bd22a78",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        3, // save device
        0  // xm
    },
    // Super Circus Atari Age (NTSC) (Joystick) (POKEY 0450) (20170611) (18896B1D).a78
    {
        "02508e6df5e173b4063a7e6e63295817",
        0, // cart type	0
        64, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Super Circus Atari Age (NTSC) (Joystick) (POKEY 4000) (20170611) (EC839981).a78
    {
        "81cee326b99d6831de10a566e338bd25",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Super Circus Atari Age (NTSC) (Paddle) (POKEY 0450) (20170611) (18896B1D).a78
    {
        "02508e6df5e173b4063a7e6e63295817",
        0, // cart type	0
        64, // cart type	1
        1, // controller 1
        3, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Super Circus Atari Age (NTSC) (Paddle) (POKEY 4000) (20170611) (EC839981).a78
    {
        "81cee326b99d6831de10a566e338bd25",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        3, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Super Circus Atari Age (PAL) (Joystick) (POKEY 0450) (20170611) (64ECD787).a78
    {
        "1c9b0bb028e63f83a2d1c1def675acc9",
        0, // cart type	0
        64, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Super Circus Atari Age (PAL) (Joystick) (POKEY 4000) (20170611) (07090B5A).a78
    {
        "f4ad1a1d732c2c8cdbd21dabaf38a46c",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Super Circus Atari Age (PAL) (Paddle) (POKEY 0450) (20170611) (64ECD787).a78
    {
        "1c9b0bb028e63f83a2d1c1def675acc9",
        0, // cart type	0
        64, // cart type	1
        1, // controller 1
        3, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Super Circus Atari Age (PAL) (Paddle) (POKEY 4000) (20170611) (07090B5A).a78
    {
        "f4ad1a1d732c2c8cdbd21dabaf38a46c",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        3, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Super Pac-Man (NTSC) (20230627) (415A28AD).a78
    {
        "88b9de0eba37ba516590fa8b860155f0",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Super Pac-Man (PAL) (20230627) (24C93682).a78
    {
        "ca9a7b264b334a15e3b1c7411758ade7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Tempest (BBC X-Port 1) (v1_00F) (20100324) (00719763).a78
    {
        "9cb3848416e39ebf642357dbee3e5970",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // The Big Burrito (20170823) (DA6980F4).a78
    {
        "a8b71da3375f73c801a5315c6c883dca",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Time Machine, The (Book Cart) (V1_02) (20230401) (B0FE53BE).a78
    {
        "43d738e5989ffff9a40e2e62f121b2fc",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // TiME Salvo (20160910) (B4CF597F).a78
    {
        "a60e4b608505d1fb201703b266f754a7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Touchdown Challenge (v2_21) (20230225) (65952659).a78
    {
        "ff825fcbed9bf6993edd422fcc592673",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Tubes (20050809) (D2A154B0).a78
    {
        "c5208b29102215b872c6c5ee0313fca6",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Tunnels of Hyperion (RC1) (20221029) (ACA9AF3E).a78
    {
        "0d7e2674d802b41286e667bbae3bcc94",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // UFO! (20091213) (314E2891).a78
    {
        "b1ec7bd809ab3deb746c5a5eb2efaecb",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // UniWarS (20230627) (F2CC89F8).a78
    {
        "f982c7dbf74c9c049cf7f875a46ed818",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // uSokoban (20220818) (A6127658).a78
    {
        "d23c8b7b7339fd62dd6aba8723549abe",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // V Blank - Chapter 1 - Into The Void (20210326u1) (9EC1B708).a78
    {
        "af96ce75837c72148d8e4413313ed1d3",
        0, // cart type	0
        70, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // WarBirds (20091213) (5C527238).a78
    {
        "c62632545c91823f72f6f14b19766804",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Wasp! (Standard Edition) (20090923) (CCDC2DBF).a78
    {
        "412cc5bfa08bd03244b9c4e8d46cd0a0",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Wizard's Dungeon (20211111) (B5010737).a78
    {
        "846751861993b907c512cc9c10c67035",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // WORDLE (20220308) (02C9EA66).a78
    {
        "71ce8910b0efd5d0014a9695cce3b7ad",
        0, // cart type	0
        2, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Worm! (v1_16F) (20100307) (39296DA2).a78
    {
        "6813ffff510f930c867b3f0aba78ac85",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Asteroids (3-D Asteroids Rev A) (Prototype) (xx-xx-84) (E820A057).a78
    {
        "db0b71fdd48ce1ea7151aea36cbad830",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Asteroids (3-D Asteroids) (Prototype) (1987) (8C610E0F).a78
    {
        "a0285769753b18c407cb6e818b7dcad2",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Asteroids (3-D Asteroids) (Prototype) (Alt 1) (6DDFB54C).a78
    {
        "4332c24e4f3bc72e7fe1b77adf66c2b7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // AtariLab - Light Module (Prototype) (24BD70BC).a78
    {
        "9f76d0276812f2bd4ad41537903808fd",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // AtariLab - Temperature Module (Prototype) (8E374C75).a78
    {
        "15245ecb7d14726242d1274c93784300",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Centipede (Prototype) (NTSC) (5-2-84) (0B85A1F2).a78
    {
        "bf7a6103337086840093af257048c834",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Commando (Prototype) (4-29-88) (9EF973A9).a78
    {
        "2a4f20c216e77048449d26205b62c563",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Desert Falcon (Prototype) (NTSC) (3-25-87) (63887912).a78
    {
        "f4ed29662536267db967182b6ba8d40e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Desert Falcon (Prototype) (NTSC) (3-9-87) (70451FC5).a78
    {
        "fc81d3a35cecae454629d3fe93b88133",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Desert Falcon (Prototype) (NTSC) (53E277F6).a78
    {
        "9ea73fd07e43f61209876d33e6f6dc04",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Dig Dug (Prototype) (xx-xx-84) (890C7C74).a78
    {
        "e9cddb6fb0b20207eda58e89b54f9bc8",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Food Fight (Prototype) (8-16-84) (47E94868).a78
    {
        "e05041ad193e354f9fa37c18f3550b5a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Galaga (Prototype) (NTSC) (4-18-84) (2906B10A).a78
    {
        "2a55cf0d92a9637ba3e42a85f4ce00b0",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Galaga (Prototype) (NTSC) (C4AE0B52).a78
    {
        "e0b4157894f7a9908d701c9206e9448f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Galaga (Prototype) (NTSC) (Rev 1) (1984) (EB97C702).a78
    {
        "7d05aa2eda32fee10f21c4e68d6bf311",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // GATO (Prototype) (1-xx-88) (715E1A2F).a78
    {
        "e7b22ca9cca1383330ef24f79e807152",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // GATO (Prototype) (12-xx-87) (0F2F66F8).a78
    {
        "1b3bfa0535e3fe74a06852e2a201d28c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // GATO (Prototype) (5-23-87) (A0529DB5).a78
    {
        "06204dadc975be5e5e37e7cc66f984cf",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // GATO (Prototype) (6-11-87) (40B629A1).a78
    {
        "acc0047293878b1365440472e6e8f6f5",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // GATO (Prototype) (6-23-87) (9FA0FBDA).a78
    {
        "ceb57af193b0ac9d10225474d5f92891",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // GATO (Prototype) (7-27-87) (15DE2327).a78
    {
        "159cc827b7fdfca99fcf3b3f65050b90",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // GATO (Prototype) (Alt 1) (198x) (B05CF8D2).a78
    {
        "d6beb56ed0e75e70a89ff2557e562d19",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // GATO (Prototype) (Alt 2) (198x) (6E9FB96B).a78
    {
        "97db562fb485eb82e85322e8557b98e8",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // GATO (Prototype) (v1) (11-xx-87) (3796705F).a78
    {
        "e3d589aa2ee5fa9c591b5ed26f58608e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // GATO (Prototype) (v2) (11-xx-87) (E51EB3BC).a78
    {
        "05a5cde44b650ed6721210b9b8ea0c74",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Hat Trick (Prototype) (5-1-87) (70C5D996).a78
    {
        "69766137ade953cbcaad23e6c8cfa197",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Impossible Mission (Prototype) (NTSC) (8-11-89) (97A0EA98).a78
    {
        "55c959a86b7bbb78b85a3fb3dbafeb6d",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Joust (Prototype) (4-23-84) (012EEA04).a78
    {
        "52ef3c474f86e5c90b71bf826989327b",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Karateka (Prototype) (12-23-86) (E9D30841).a78
    {
        "e2a6c2d5ac1fa58b1bd34d9eea49a8fd",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Klax (Prototype) (NTSC) (Impossible Wave 1) (B0776037).a78
    {
        "5f9899099f2374fc62077138d9ee1e57",
        0, // cart type	0
        2, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Klax (Prototype) (NTSC) (Impossible Wave 2) (853E59FA).a78
    {
        "6c9d0b4adc87bb9e86954d6c96e50a8a",
        0, // cart type	0
        2, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Klax (Prototype) (NTSC) (Impossible Wave 3) (0FB0A4D8).a78
    {
        "003e4ba650ec7c677b3492222ba22332",
        0, // cart type	0
        2, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Klax (Prototype) (NTSC) (xx-xx-92) (F26621E3).a78
    {
        "17b3b764d33eae9b5260f01df7bb9d2f",
        0, // cart type	0
        2, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Klax (Prototype) (NTSC) (xx-xx-92) (Fixed) (Alt 1) (C56318E4).a78
    {
        "14256da558bfa8184a2aa9c2c53840f5",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Klax (Prototype) (NTSC) (xx-xx-92) (Fixed) (Alt 2) (6C289518).a78
    {
        "9de4ecd1cad6bedfc580db48bc6441f4",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Klax (Prototype) (NTSC) (xx-xx-92) (Fixed) (D92A79B0).a78
    {
        "5fb805f2b69820a9b196f5fed2a23c99",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Missing In Action (TNT Games) (Prototype) (xx-xx-89) (FF7A4C60).a78
    {
        "017066f522908081ec3ee624f5e4a8aa",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man (Encrypt) (Color Hack) (Prototype) (xx-xx-84) (05937770).a78
    {
        "8c235eb4fd011930d30652fe9bdadedc",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man (Prototype) (NTSC) (Rev 1) (1984) (DB4C3A1D).a78
    {
        "b689244e2c1e9abfdf2d38176c06248d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Ms Pac-Man (Prototype) (xx-xx-84) (E4C93003).a78
    {
        "dcbd141c5c43e0df29a8d59af6d36209",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Pit Fighter (Atari) (Prototype) (1992) (22A88779).a78
    {
        "ec206c8db4316eb1ebce9fc960da7d8f",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pit Fighter (Atari) (Prototype) (Alt 1) (1992) (4AE0E7F6).a78
    {
        "8ffd465270a4261eecc43007805af5a6",
        40, // cart type	0
        19, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pit Fighter (Atari) (Prototype) (Alt 2) (1992) (A5E75537).a78
    {
        "05f43244465943ce819780a71a5b572a",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Plutos (Prototype) (2F211F7F).a78
    {
        "86546808dc60961cdb1b20e761c50ab1",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Plutos (Prototype) (Alt 1) (E446F530).a78
    {
        "58d713dd9940242f5df80f7b4765a927",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Plutos (Prototype) (Infinite Lives) (B6D8FD9B).a78
    {
        "eb3a1741e19429dc7eeb5337dde5567d",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pole Position II (Prototype) (4-11-84) (94EBFE99).a78
    {
        "9ba7e8d8dee1da4d3995b1ff8096ebc9",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pole Position II (Prototype) (NTSC) (Rev 1) (1984) (9530DC7E).a78
    {
        "1cd1309a7cf4d2ecbfeb08510d751a95",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Rampart (Prototype) (xx-xx-91) (E8C2A662).a78
    {
        "442761655bb25ddfe5f7ab16bf591c6f",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Rescue On Fractalus (Prototype) (Alt 1) (xx-xx-84) (ABE19E33).a78
    {
        "e1f1da48e1cb10087a745a10a8048ce9",
        0, // cart type	0
        128, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Rescue On Fractalus (Prototype) (xx-xx-84) (9A74FBF2).a78
    {
        "8f7eb10ad0bd75474abf0c6c36c08486",
        0, // cart type	0
        128, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Robotron 2084 (Prototype) (NTSC) (Rev 1) (1984) (BC119646).a78
    {
        "28da355a0db3ac5822e95c0505f35037",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Sentinel (NTSC) (Finished Prototype) (11-21-90) (2FDDAD78).a78
    {
        "b697d9c2d1b9f6cb21041286d1bbfa7f",
        0, // cart type	0
        18, // cart type	1
        2, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Sirius (Prototype) (65AE616E).a78
    {
        "2d643ac548c40e58c99d0fe433ba4ba0",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Sirius (Prototype) (Alt 1) (C56AD054).a78
    {
        "062c3f87b50d118d77dac6562f3b3233",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Sirius (Prototype) (Infinite Lives) (E0535CBA).a78
    {
        "222f2d35de811f9e1475d03a166bb96b",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Star Typer (Prototype) (Bad Dump) (6-2-84) (6ADF22B3).a78
    {
        "0043582a0c73df79be9bf6ae48899954",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Star Typer (Prototype) (Bad Dump) (6-28-84) (6D0C6A0B).a78
    {
        "7cbfbc682d2d07d25c18595475571eaf",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Super Stunt Cycle (Prototype) (11-3-88) (60D253E2).a78
    {
        "2ba7ffb58607d52a5bda7b259d4c6a9c",
        0, // cart type	0
        2, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Super Stunt Cycle (Prototype) (4-14-89) (CB3DFF09).a78
    {
        "914a4598882d232af7ac992d594c6896",
        0, // cart type	0
        2, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Super Stunt Cycle (Prototype) (5-26-89) (3F4DA5C3).a78
    {
        "2e0b41aa4a8989f4e599ebe8d5c661d7",
        0, // cart type	0
        2, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Super Stunt Cycle (Prototype) (Fixed Short) (4-14-89) (0ED56BFA).a78
    {
        "d6e0ae44b45088568d424e23ed07c38c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Super Stunt Cycle (Prototype) (Fixed Short) (5-26-89) (FAA53130).a78
    {
        "d179161ccc3ddc81b3b61d0d812b586e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Test Cartridge (NTSC) (Prototype) (EADF6BF5).a78
    {
        "c5af500fdc2289193c7134e44273cde6",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Test Cartridge (PAL) (Prototype) (729DD769).a78
    {
        "6bfc9970b1a78e53653b650d3a833145",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Xevious (Prototype) (8-10-84) (6FD15498).a78
    {
        "dbbe99aeeb8cbc0f5f17cd0082d260a0",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // 24bit Value Comparison (20170811) (34C839CD).a78
    {
        "115b2dfbe97eedb80946f6225b262a19",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // 320A Mode Test (20230321) (186DCEF6).a78
    {
        "878b3f27a73dbc8d4c3b68f21f6b4ab0",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // 320D Font Test (20190705) (9F1C29C4).a78
    {
        "45dbab74cbc24f8e06b45bd10d1f36da",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // 320D Font Test 2 (20190705) (5C4D7CF8).a78
    {
        "b4fae89fd07625679815911da4c9cb79",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // 320D Interlace Test (20190704) (860E79F2).a78
    {
        "51e8f30fb61b80ea4289baaa034ce489",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // 7800 Test (NTSC) (20140406) (EF65C77A).a78
    {
        "89e7b20e7e0c629d00c6ca68949a216f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // 7800 Test (PAL) (20140406) (6F1BE96A).a78
    {
        "b4498bc576ae50e5215e90da92716fdd",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // 7800 Utility Cart (20161210) (DF65D7AA).a78
    {
        "f5150c0fc1948832211e57852abb0c6e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // 7800 Utility Cart (POKEY 4000) (20221220) (DF65D7AA).a78
    {
        "f5150c0fc1948832211e57852abb0c6e",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // AtariVox Speech Test (20151021) (EF199D2B).a78
    {
        "143cf76725a1c7e0dfce25f96ad74c1c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        2, // save device
        0  // xm
    },
    // AtariVox Test (20180915) (D49170D7).a78
    {
        "6cc2e2841872edc93877621ed2501888",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        2, // save device
        0  // xm
    },
    // Banked RAM Test (20221103) (4D3BE84B).a78
    {
        "1f27694f1ccd82022302148cd8916d05",
        0, // cart type	0
        34, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Bubble Sort Test (20220803) (5D38978D).a78
    {
        "a26778bc2bbc249b389f03e31621c277",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Color (Demo) (20091107) (CD5DE302).a78
    {
        "8752380146246883535387d2c7745b62",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Color Bars Selector Demo (1984) (D98BC2F8).a78
    {
        "441ac404cdc7bcbd4d787f911df7bf0d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Color Grid (20030527) (22CA4444).a78
    {
        "7b58f7be78ed22e1090ab24b269b746e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Colour Picker (v1_00F) (20100113) (5E8612CD).a78
    {
        "5dab213acc3c0a3ed47c36e65bd80bda",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // CPS 7800 Diagnostic Test Cartridge (v1_00) (19840511) (A1EAA7C1).a78
    {
        "91041aadd1700a7a4076f4005f2c362f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Detect Emulator (NTSC) (20220626) (6EE11650).a78
    {
        "076bb45b93b5cd3ab6175b8f4a0da86a",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Detect Emulator (PAL) (20220627) (IE8E88FD0).a78
    {
        "5d06609407cdd4d9a72e0ad6a50a94da",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Font Check (20201202) (217AB19A).a78
    {
        "ee80cc6f3bb0b331872a719ba30e8614",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Graphics Modes Test (20100914) (E8D0E19D).a78
    {
        "31fbcf03946e804fc2d77cca5d61c928",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // High Score Cartridge (9BE408D3).a78
    {
        "c8a73288ab97226c52602204ab894286",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Keypad Controller Test (20180623) (87BA7357).a78
    {
        "85eabafb511e177594ccff1d29b709a6",
        0, // cart type	0
        0, // cart type	1
        7, // controller 1
        7, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Keypad Delay Test (20180914) (CB797880).a78
    {
        "9b244b2e64380a84e0cc5ce2fd364af3",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        7, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Legend of Silverpeak - SaveKey Reader (20220714) (403D2EB8).a78
    {
        "74c8bffffb369ca1693a7d7ba11ca26c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // Mouse Control Demo (20200829) (6D0E0EB8).a78
    {
        "96b6c1beb867c08e3a3991d1df8ce00f",
        0, // cart type	0
        0, // cart type	1
        8, // controller 1
        8, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Paddle Test (DLI Sampling) (20050510) (C19CE22C).a78
    {
        "ed8d080ad9cdb63abd7eda731b351338",
        0, // cart type	0
        0, // cart type	1
        3, // controller 1
        3, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Palette Tool Test 160B (20190714) (A87CA5DA).a78
    {
        "6bbe116d1d121e6045fef4f65440d723",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // POKEY (Sample) (20140301) (C560D72F).a78
    {
        "f9fa5107ed213e709858d8876359309e",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // POKEY Player (450) (20201214) (B7CC5B55).a78
    {
        "be91e772a9631283e8ac8520937329fb",
        0, // cart type	0
        70, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // POKEY Test (450) (20200506) (C5F46712).a78
    {
        "a0b0413b4c3ef1844bdf35fd975ec4b6",
        0, // cart type	0
        64, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // POKEY Tester (450) (20220605) (8BF8ECD7).a78
    {
        "04447d069732b6885ac93ad3521aa269",
        0, // cart type	0
        64, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // POKEY Tester (800) (20220821) (F54AA99D).a78
    {
        "305fa23fe56046d6dfd11a156ac7d02f",
        128, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // POKEY Tester (810) (20220821) (5E584DE3).a78
    {
        "23dae560e02ad6b4bd1966be7b20660a",
        128, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // POKEY Tester (Reset Version) (450) (20220606) (4C57EEF2).a78
    {
        "598adb2072833fea9c4c128bf75f510b",
        0, // cart type	0
        64, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Prickle (POKEY Utility) (v09) (20150811) (476A1ACB).a78
    {
        "b4f137e85588ce42d302102ba7215437",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Proline Charge Check (20210227) (5EF90F20).a78
    {
        "d9579c0b6e4e7e686c26b61de8c2ab90",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // RIOT DMA Test (20200816) (D37881D7).a78
    {
        "67a7d1f73559a6f47838994f092a5355",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Rotational Controls Demo (20200829) (59A5457E).a78
    {
        "a2b443c9d7788a18b09a41cdd075a5b8",
        0, // cart type	0
        0, // cart type	1
        6, // controller 1
        6, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Save Dev Test (20201206) (D67E698C).a78
    {
        "57ca832fa1a2d27f82e2db68759cd3d5",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        3, // save device
        0  // xm
    },
    // SaveKey Test (20190214) (CAD56598).a78
    {
        "0694524c3714b87f2b0b5fb69461d8db",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        2, // save device
        0  // xm
    },
    // Screen Safe (20210312) (0638011F).a78
    {
        "ebbe655d0eb09853f3bd4bfbd11c7564",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Sound Test - TIA SFX Library (20220414) (27AE5A98).a78
    {
        "979e77d7f5a022ad51dcb8b52997f388",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Sprite DMA Test (20171208) (822B977C).a78
    {
        "0c35c91979394aea745fe41aa02ebfe7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Sprite Hole DMA (20171208) (E791B1F4).a78
    {
        "596a92a1f1baa8af97c6abdfb0cfda5c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Sprite Test - 16 Pixels Zone (160A Mode) (9376748B).a78
    {
        "c05ab9c8dc911f81451052babbabfe1e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Sprite Test - 16 Pixels Zone (320A Mode) (AD21310D).a78
    {
        "6e47492d11bda1ecbedff290b3df78f3",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Sprite Test - 8 Pixels Zone (160A Mode) (C6962202).a78
    {
        "93122375e1e7ecdebd89f075d9c6b313",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Sprite Test - 8 Pixels Zone (320A Mode) (6EE1545C).a78
    {
        "2a536b1eb437fd230c489392c84ca458",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Tinymon 7800 (Memory Tool) (20151218) (2F5323B0).a78
    {
        "d49624a530cf9981e4e20dac0fc2947a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Trakball Test (DLI Sampling) (20050510) (C8AB8BCB).a78
    {
        "64a13a9bfcff4e1814242c06fafa7e9b",
        0, // cart type	0
        0, // cart type	1
        4, // controller 1
        4, // controller 2
        0, // tv type
        0, // save device
        1  // xm
    },
    // Voxolotl - AtariVox Voice Tracker (20200514) (EEA11D07).a78
    {
        "9e6b190adb92f3ddfe1ac8c1b5844e08",
        0, // cart type	0
        0, // cart type	1
        7, // controller 1
        0, // controller 2
        0, // tv type
        2, // save device
        0  // xm
    },
    // WSYNC Test (PAL) (20220823) (B062B1F1).a78
    {
        "bfd1af94653c4d1fcd61a091ae11bdc2",
        0, // cart type	0
        0, // cart type	1
        0, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Ace of Aces (1988) (Atari) (05A2B94C).a78
    {
        "0be996d25144966d5541c9eb4919b289",
        0, // cart type	0
        2, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Alien Brigade (NTSC) (Joystick) (Atari) (1990) (C8849D36).a78
    {
        "877dcc97a775ed55081864b2dbf5f1e2",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Alien Brigade (NTSC) (Lightgun) (Atari) (1990) (C8849D36).a78
    {
        "877dcc97a775ed55081864b2dbf5f1e2",
        0, // cart type	0
        10, // cart type	1
        2, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Double Dragon (OM) (NTSC) (Activision) (1989) (F20773D5).a78
    {
        "426298bcc76405aa7b58a9858d828db5",
        1, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Rampage (OM) (NTSC) (Activision) (1989) (D2876EE2).a78
    {
        "825126724d520ecbdcb096201420dc39",
        1, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Winter Games (Alternate) (NTSC) (Atari) (1987) (CF648E88).a78
    {
        "90fa275f9f2a65b341796e11b2f551af",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Asteroids (NTSC) (Atari) (1987) (DFB93F40).a78
    {
        "07342c78619ba6ffcc61c10e907e3b50",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Ballblazer (NTSC) (Atari-Lucasfilm) (1987) (A4C4808B).a78
    {
        "8fc3a695eaea3984912d98ed4a543376",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Barnyard Blaster (NTSC) (Atari) (1988) (ED0A587D).a78
    {
        "42682415906c21c6af80e4198403ffda",
        0, // cart type	0
        18, // cart type	1
        2, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Basketbrawl (NTSC) (Atari) (1990) (C8B9D7B5).a78
    {
        "f5f6b69c5eb4b55fc163158d1a6b423e",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Centipede (NTSC) (Atari) (1987) (020EFE25).a78
    {
        "5a09946e57dbe30408a8f253a28d07db",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Choplifter (NTSC) (Atari) (1987) (419BFF61).a78
    {
        "93e4387864b014c155d7c17877990d1e",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Commando (NTSC) (Atari) (1989) (CD1A98C5).a78
    {
        "2e8e28f6ad8b9b9267d518d880c73ebb",
        0, // cart type	0
        3, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Crack'ed (NTSC) (Atari) (1988) (E645FD1F).a78
    {
        "db691469128d9a4217ec7e315930b646",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Crossbow (NTSC) (Joystick) (Atari) (1988) (D2EA5686).a78
    {
        "a94e4560b6ad053a1c24e096f1262ebf",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Crossbow (NTSC) (Lightgun) (Atari) (1988) (D2EA5686).a78
    {
        "a94e4560b6ad053a1c24e096f1262ebf",
        0, // cart type	0
        10, // cart type	1
        2, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Dark Chambers (NTSC) (Atari) (1988) (366777F1).a78
    {
        "179b76ff729d4849b8f66a502398acae",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Desert Falcon (NTSC) (Atari) (1987) (A1DC0899).a78
    {
        "95ac811c7d27af0032ba090f28c107bd",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Dig Dug (NTSC) (Atari) (1987) (50CB13F3).a78
    {
        "731879ea82fc0ca245e39e036fe293e6",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Donkey Kong (NTSC) (Atari) (1988) (065E063F).a78
    {
        "19f1ee292a23636bd57d408b62de79c7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Donkey Kong Jr (NTSC) (Atari) (1988) (1C5A1082).a78
    {
        "5e332fbfc1e0fc74223d2e73271ce650",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Double Dragon (AM) (NTSC) (Activision) (1989) (AA265865).a78
    {
        "543484c00ba233736bcaba2da20eeea9",
        1, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // F-18 Hornet (NTSC) (Absolute) (1988) (63D1C7C1).a78
    {
        "2251a6a0f3aec84cc0aff66fc9fa91e8",
        2, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Fatal Run (NTSC) (Atari) (1990) (A44DF354).a78
    {
        "d25d5d19188e9f149977c49eb0367cd1",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Fight Night (NTSC) (Atari) (1988) (720EBC74).a78
    {
        "07dbbfe612a0a28e283c01545e59f25e",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Food Fight (NTSC) (Atari) (1987) (E64A5CDE).a78
    {
        "cf76b00244105b8e03cdc37677ec1073",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Galaga (NTSC) (Atari) (1987) (1A0A3EB3).a78
    {
        "fb8d803b328b2e442548f7799cfa9a4a",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Hat Trick (NTSC) (Atari) (1987) (CF6F4A6C).a78
    {
        "fd9e78e201b6baafddfd3e1fbfe6ba31",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ikari Warriors (NTSC) (Atari) (1989) (8A6C1F15).a78
    {
        "c3672482ca93f70eafd9134b936c3feb",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Impossible Mission (NTSC) (Atari) (1987) (3B1F2F47).a78
    {
        "baebc9246c087e893dfa489632157180",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Jinks (NTSC) (Atari) (1989) (0818D8CD).a78
    {
        "045fd12050b7f2b842d5970f2414e912",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        2, // tv type
        0, // save device
        0  // xm
    },
    // Joust (NTSC) (Atari) (1987) (2C430BCE).a78
    {
        "f18b3b897a25ab3885b43b4bd141b396",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Karateka (NTSC) (Atari) (1987) (FEC21472).a78
    {
        "c3a5a8692a423d43d9d28dd5b7d109d9",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Kung-Fu Master (NTSC) (Absolute) (1989) (297899BB).a78
    {
        "f57d0af323d4e173fb49ed447f0563d7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Mario Bros (NTSC) (Atari) (1988) (8021A13B).a78
    {
        "431ca060201ee1f9eb49d44962874049",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Mat Mania Challenge (NTSC) (Atari) (1989) (ABA91829).a78
    {
        "37b5692e33a98115e574185fa8398c22",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Mean 18 Ultimate Golf (NTSC) (Atari) (1988) (CF21D9AC).a78
    {
        "f2f5e5841e4dda89a2faf8933dc33ea6",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Meltdown (NTSC) (Atari) (1990) (4A8F2171).a78
    {
        "bedc30ec43587e0c98fc38c39c1ef9d0",
        0, // cart type	0
        18, // cart type	1
        2, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Midnight Mutants (NTSC) (Atari) (1990) (187EC84E).a78
    {
        "bc1e905db1008493a9632aa83ab4682b",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Motor Psycho (NTSC) (Atari) (1990) (1E219482).a78
    {
        "3bc8f554cf86f8132a623cc2201a564b",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man (NTSC) (Atari) (1987) (E42FC700).a78
    {
        "fc0ea52a9fac557251b65ee680d951e5",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Ninja Golf (NTSC) (Atari) (1990) (CB48E8DC).a78
    {
        "220121f771fc4b98cef97dc040e8d378",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // One-on-One Basketball (NTSC) (Atari) (1987) (7D9370FB).a78
    {
        "74569571a208f8b0b1ccfb22d7c914e1",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pete Rose Baseball (NTSC) (Absolute) (1989) (F33D4DFA).a78
    {
        "1a5207870dec6fae9111cb747e20d8e3",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Planet Smashers (NTSC) (Atari) (1990) (4E3B9E64).a78
    {
        "33aea1e2b6634a1dec8c7006d9afda22",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Pole Position II (NTSC) (Atari) (1987) (A85FB962).a78
    {
        "584582bb09ee8122e7fc09dc7d1ed813",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Rampage (AM) (NTSC) (Activision) (1989) (39A316AA).a78
    {
        "ac03806cef2558fc795a7d5d8dba7bc0",
        1, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // RealSports Baseball (NTSC) (Atari) (1988) (B1508030).a78
    {
        "383ed9bd1efb9b6cb3388a777678c928",
        0, // cart type	0
        2, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Robotron 2084 (NTSC) (Atari) (1987) (CB22305D).a78
    {
        "66ecaafe1b82ae68ffc96267aaf7a4d7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Scrapyard Dog (NTSC) (Atari) (1990) (5CC8C34F).a78
    {
        "980c35ae9625773a450aa7ef51751c04",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Summer Games (NTSC) (Atari) (1987) (65C6DF3F).a78
    {
        "cbb0746192540a13b4c7775c7ce2021f",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Super Huey UH-IX (NTSC) (Atari) (1989) (71846500).a78
    {
        "cc18e3b37a507c4217eb6cb1de8c8538",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Super Skateboardin' (NTSC) (Absolute) (1989) (13B68650).a78
    {
        "59b5793bece1c80f77b55d60fb39cb94",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Tank Command (NTSC) (Froggo) (1988) (DB91B181).a78
    {
        "5c4f752371a523f15e9980fea73b874d",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Title Match Pro Wrestling (NTSC) (Absolute) (1989) (BC23C57B).a78
    {
        "1af475ff6429a160752b592f0f92b287",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Tomcat - The F-14 Fighter Simulator (NTSC) (Absolute) (1989) (513CB9EE).a78
    {
        "c3903ab01a51222a52197dbfe6538ecf",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Touchdown Football (NTSC) (Atari) (1988) (AAE12695).a78
    {
        "208ef955fa90a29815eb097bce89bace",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Tower Toppler (NTSC) (Atari) (1988) (4407BA04).a78
    {
        "8d64763db3100aadc552db5e6868506a",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        1, // controller 2
        2, // tv type
        0, // save device
        0  // xm
    },
    // Water Ski (NTSC) (Froggo) (1988) (930B30DF).a78
    {
        "427cb05d0a1abb068998e2760d77f4fb",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Winter Games (NTSC) (Atari) (1987) (8981B531).a78
    {
        "3799d72f78dda2ee87b0ef8bf7b91186",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Xenophobe (NTSC) (Atari) (1989) (5FD9A141).a78
    {
        "05fb699db9eef564e2fe45c568746dbc",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        0, // save device
        0  // xm
    },
    // Xevious (NTSC) (Atari) (1987) (75FC124F).a78
    {
        "d7dc17379aa25e5ae3c14b9e780c6f6d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
    // Ace of Aces (PAL) (Atari) (1988) (20C5DB80).a78
    {
        "aadde920b3aaba03bc10b40bd0619c94",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Alien Brigade (PAL) (Joystick) (Atari) (1990) (6A19F0FE).a78
    {
        "de3e9496cb7341f865f27e5a72c7f2f5",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Alien Brigade (PAL) (Lightgun) (Atari) (1990) (6A19F0FE).a78
    {
        "de3e9496cb7341f865f27e5a72c7f2f5",
        0, // cart type	0
        10, // cart type	1
        2, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Double Dragon (OM) (PAL) (Activision) (1989) (4D634BF5).a78
    {
        "d3477fc0f7a42a8a5e69aa3de2c38062",
        1, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Ballblazer (PAL) (Atari-Lucasfilm) (1987) (AFF85565).a78
    {
        "b558814d54904ce0582e2f6a801d03af",
        0, // cart type	0
        1, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Barnyard Blaster (PAL) (Atari) (1988) (02764A86).a78
    {
        "babe2bc2976688bafb8b23c192658126",
        0, // cart type	0
        18, // cart type	1
        2, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Baseball (PAL) (Absolute) (1989) (68D48FDC).a78
    {
        "386bded4a944bae455fedf56206dd1dd",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Basketbrawl (PAL) (Atari) (1990) (A4265D4B).a78
    {
        "fba002089fcfa176454ab507e0eb76cb",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Centipede (PAL) (Atari) (1987) (33F8A8F6).a78
    {
        "38c056a48472d9a9e16ebda5ed91dae7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        1, // save device
        0  // xm
    },
    // Choplifter (PAL) (Atari) (1987) (12EFEB07).a78
    {
        "59d4edb0230b5acc918b94f6bc94779f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Commando (PAL) (Atari) (1989) (7C211612).a78
    {
        "55da6c6c3974d013f517e725aa60f48e",
        0, // cart type	0
        3, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Crack'ed (PAL) (Atari) (1988) (50FD19CB).a78
    {
        "7cbe78fa06f47ba6516a67a4b003c9ee",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Crossbow (PAL) (Joystick) (Atari) (1988) (E93D8894).a78
    {
        "63db371d67a98daec547b2abd5e7aa95",
        0, // cart type	0
        10, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Crossbow (PAL) (Lightgun) (Atari) (1988) (E93D8894).a78
    {
        "63db371d67a98daec547b2abd5e7aa95",
        0, // cart type	0
        10, // cart type	1
        2, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Dark Chambers (PAL) (Atari) (1988) (C93A563E).a78
    {
        "a2b8e2f159642c4b91de82e9a2928494",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Desert Falcon (PAL) (Atari) (1987) (33991EE8).a78
    {
        "2d5d99b993a885b063f9f22ce5e6523d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Dig Dug (PAL) (Atari) (1987) (EFAB7077).a78
    {
        "408dca9fc40e2b5d805f403fa0509436",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        1, // save device
        0  // xm
    },
    // Donkey Kong (PAL) (Atari) (1988) (0A43D1C5).a78
    {
        "8e96ef14ce9b5d84bcbc996b66d6d4c7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Donkey Kong Jr (PAL) (Atari) (1988) (C784DBAC).a78
    {
        "4dc5f88243250461bd61053b13777060",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Double Dragon (AM) (PAL) (Activision) (1989) (F29ABDB2).a78
    {
        "de2ebafcf0e37aaa9d0e9525a7f4dd62",
        1, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // F-18 Hornet (PAL) (Absolute) (1988) (E5D714B0).a78
    {
        "e7709da8e49d3767301947a0a0b9d2e6",
        2, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Fatal Run (PAL) (Atari) (1990) (2A0A93BE).a78
    {
        "23505651ac2e47f3637152066c3aa62f",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Fight Night (PAL) (Atari) (1988) (64D9C4BE).a78
    {
        "e80f24e953563e6b61556737d67d3836",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Food Fight (PAL) (Atari) (1987) (C64549EC).a78
    {
        "de0d4f5a9bf1c1bddee3ed2f7ec51209",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        1, // save device
        0  // xm
    },
    // Galaga (PAL) (Atari) (1987) (5990A9F8).a78
    {
        "f5dc7dc8e38072d3d65bd90a660148ce",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        1, // save device
        0  // xm
    },
    // Hat Trick (PAL) (Atari) (1987) (057D9C3C).a78
    {
        "0baec96787ce17f390e204de1a136e59",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Ikari Warriors (PAL) (Atari) (1989) (CB621CDC).a78
    {
        "8c2c2a1ea6e9a928a44c3151ba5c1ce3",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Impossible Mission (PAL) (Atari) (1987) (994AF6E0).a78
    {
        "80dead01ea2db5045f6f4443faa6fce8",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Jinks (PAL) (Atari) (1989) (9207ED3B).a78
    {
        "dfb86f4d06f05ad00cf418f0a59a24f7",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        0, // controller 2
        3, // tv type
        0, // save device
        0  // xm
    },
    // Joust (PAL) (Atari) (1987) (9B3BE616).a78
    {
        "f2dae0264a4b4a73762b9d7177e989f6",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        1, // save device
        0  // xm
    },
    // Karateka (PAL) (Atari) (1987) (6C8D9E68).a78
    {
        "5e0a1e832bbcea6facb832fde23a440a",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Kung-Fu Master (PAL) (Absolute) (1989) (C1531FC4).a78
    {
        "2931b75811ad03f3ac9330838f3d231b",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Mario Bros (PAL) (Atari) (1988) (EFAD3D9D).a78
    {
        "d2e861306be78e44248bb71d7475d8a3",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Mat Mania Challenge (PAL) (Atari) (1989) (065F3873).a78
    {
        "6819c37b96063b024898a19dbae2df54",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Mean 18 Ultimate Golf (PAL) (Atari) (1988) (3CEC4BE8).a78
    {
        "2e9dbad6c0fa381a6cd1bb9abf98a104",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Meltdown (PAL) (Atari) (1990) (177FC850).a78
    {
        "c80155d7eec9e3dcb79aa6b83c9ccd1e",
        0, // cart type	0
        18, // cart type	1
        2, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Midnight Mutants (PAL) (Atari) (1990) (7CA6D521).a78
    {
        "6794ea31570eba0b88a0bf1ead3f3f1b",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Motor Psycho (PAL) (Atari) (1990) (38BB45BA).a78
    {
        "5330bfe428a6b601b7e76c2cfc4cd049",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Ms Pac-Man (PAL) (Atari) (1987) (8421033A).a78
    {
        "56469e8c5ff8983c6cb8dadc64eb0363",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        1, // save device
        0  // xm
    },
    // Ninja Golf (PAL) (Atari) (1990) (2B5382B7).a78
    {
        "ea0c859aa54fe5eaf4c1f327fab06221",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // One-on-One Basketball (PAL) (Atari) (1987) (7A628295).a78
    {
        "8dba0425f0262e5704581d8757a1a6e3",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Planet Smashers (PAL) (Atari) (1990) (95DC7E96).a78
    {
        "2837a8fd49b7fc7ccd70fd45b69c5099",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Pole Position II (PAL) (Atari) (1987) (D751C7A6).a78
    {
        "865457e0e0f48253b08f77b9e18f93b2",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Scrapyard Dog (PAL) (Atari) (1990) (46DD9662).a78
    {
        "53db322c201323fe2ca8f074c0a2bf86",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Sentinel (PAL) (Atari) (1991) (47340DF9).a78
    {
        "5469b4de0608f23a5c4f98f331c9e75f",
        0, // cart type	0
        18, // cart type	1
        2, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Super Huey UH-IX (PAL) (Atari) (1989) (6B27EA2C).a78
    {
        "162f9c953f0657689cc74ab20b40280f",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Super Skateboardin' (PAL) (Absolute) (1989) (8F167BA9).a78
    {
        "95d7c321dce8f57623a9c5b4947bb375",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Title Match Pro Wrestling (PAL) (Absolute) (1989) (16B54D5B).a78
    {
        "3bb9c8d9adc912dd7f8471c97445cd8d",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Tomcat - The F-14 Fighter Simulator (PAL) (Absolute) (1989) (B01205BF).a78
    {
        "682338364243b023ecc9d24f0abfc9a7",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        0, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Tower Toppler (PAL) (Atari) (1988) (0F87FD7B).a78
    {
        "32a37244a9c6cc928dcdf02b45365aa8",
        0, // cart type	0
        6, // cart type	1
        1, // controller 1
        1, // controller 2
        3, // tv type
        0, // save device
        0  // xm
    },
    // Xenophobe (PAL) (Atari) (1989) (CF0E44F1).a78
    {
        "70937c3184f0be33d06f7f4382ca54de",
        0, // cart type	0
        18, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        0, // save device
        0  // xm
    },
    // Xevious (PAL) (Atari) (1987) (D5AC738B).a78
    {
        "b1a9f196ce5f47ca8caf8fa7bc4ca46c",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        1, // save device
        0  // xm
    },

    // Pac-man 7800+ (PAL)
    {
        "f47dace752419f4b1622b02f3cbca490",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        1, // tv type
        1, // save device
        0  // xm
    },
    // Pac-man 7800+ (NTSC)
    {
        "a9792abc71547a8793268e00690de0a8",
        0, // cart type	0
        0, // cart type	1
        1, // controller 1
        1, // controller 2
        0, // tv type
        1, // save device
        0  // xm
    },
};

uint32_t get_cart_list_length() {
  return sizeof(cart_list) / sizeof(cart_list[0]);
}


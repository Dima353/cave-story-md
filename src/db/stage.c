#include "common.h"
#include "res/stage.h"
#include "res/pal.h"
#include "tables.h"

const stage_info_def stage_info[STAGE_COUNT] = {
	{ NULL,			PXE_0,		PAL_Regu,	0,	0,	"ni{ego" },
	{ PXM_Pens1,	PXE_Pens1,	PAL_LabB,	16,	14,	"dom artura" },
	{ PXM_Eggs,		PXE_Eggs,	PAL_Regu,	6,	13,	"hranili|e qic" },
	{ PXM_EggX,		PXE_EggX,	PAL_Regu,	7,	0,	"qjco N.00" },
	{ PXM_Egg6,		PXE_Egg6,	PAL_Regu,	5,	0,	"qjco N.06" },
	{ PXM_EggR,		PXE_EggR,	PAL_Regu,	19,	0,	"komnata kontrolq" },
	{ PXM_Weed,		PXE_Weed,	PAL_Regu,	20,	2,	"grasstaun" },
	{ PXM_Santa,	PXE_Santa,	PAL_Regu,	2,	0,	"dom santy" },
	{ PXM_Chako,	PXE_Chako,	PAL_Chaco,	2,	0,	"dom {ako" },
	{ PXM_MazeI,	PXE_MazeI,	PAL_XB,	13,	9,	"labirint I" },
	{ PXM_Sand,		PXE_Sand,	PAL_Regu,	18,	16,	"ruiny" },
	{ PXM_Mimi,		PXE_Mimi,	PAL_Regu,	14,	2,	"derewnq mimiga" },
	{ PXM_Cave,		PXE_Cave,	PAL_Regu,	3,	0,	"perwaq pe|era" },
	{ PXM_Start,	PXE_Start,	PAL_Regu,	3,	0,	"to{ka ots{eta" },
	{ PXM_Barr,		PXE_Barr,	PAL_Regu,	14,	0,	"hivina" },
	{ PXM_Pool,		PXE_Pool,	PAL_Regu,	14,	2,	"ozero" },
	/* 0x10 (16) */
	{ PXM_Cemet,	PXE_Cemet,	PAL_Regu,	14,	0,	"kladbi|e" },
	{ PXM_Plant,	PXE_Plant,	PAL_Plant,	14,	2,	"ferma qmasita" },
	{ PXM_Shelt,	PXE_Shelt,	PAL_Regu,	19,	0,	"pristani|e" },
	{ PXM_Comu,		PXE_Comu,	PAL_Regu,	16,	0,	"zal sobranij" },
	{ PXM_MiBox,	PXE_MiBox,	PAL_Regu,	14,	0,	"bezopasnoe mesto" },
	{ PXM_EgEnd1,	PXE_EgEnd1, PAL_Regu,	19,	0,	"bokowaq komnata" },
	{ PXM_Cthu,		PXE_Cthu,	PAL_Regu,	19,	0,	"obitelx ktulhu" },
	{ PXM_Egg1,		PXE_Egg1,	PAL_Regu,	5,	0,	"qjco N.01" },
	{ PXM_Pens2,	PXE_Pens2,	PAL_LabB,	16,	14,	"dom artura" },
	{ PXM_Malco,	PXE_Malco,	PAL_Regu,	2,	0,	"~nergostanciq" },
	{ PXM_WeedS,	PXE_WeedS,	PAL_Regu,	2,	0,	"bezopasnoe mesto" },
	{ PXM_WeedD,	PXE_WeedD,	PAL_Regu,	19,	0,	"kamera pytok" },
	{ PXM_Frog,		PXE_Frog,	PAL_Frog,	20,	2,	"sklad plastida" },
	{ PXM_Curly,	PXE_Curly,	PAL_Regu,	18,	0,	"vili|e" },
	{ PXM_WeedB,	PXE_WeedB,	PAL_Regu,	16,	0,	"wisq|ij dom" },
	{ PXM_Stream,	PXE_Stream,	PAL_Regu,	17,	3,	"te{enie" },
	/* 0x20 (32) */
	{ PXM_CurlyS,	PXE_CurlyS,	PAL_Regu,	16,	0,	"malenxkaq komnata" },
	{ PXM_Jenka1,	PXE_Jenka1,	PAL_Jenka,	2,	0,	"dom dvenki" },
	{ PXM_Dark,		PXE_Dark,	PAL_Regu,	18,	0,	"zabro}ennoe vili|e" },
	{ PXM_Gard,		PXE_Gard,	PAL_Regu,	9,	5,	"sklad semqn" },
	{ PXM_Jenka2,	PXE_Jenka2,	PAL_Jenka,	2,	0,	"dom dvenki" },
	{ PXM_SandE,	PXE_SandE,	PAL_Regu,	18,	16,	"ruiny" },
	{ PXM_MazeH,	PXE_MazeH,	PAL_XB,	13,	9,	"labirint H" },
	{ PXM_MazeW,	PXE_MazeW,	PAL_X,		13,	8,	"labirint W" },
	{ PXM_MazeO,	PXE_MazeO,	PAL_Regu,	13,	0,	"palata" },
	{ PXM_MazeD,	PXE_MazeD,	PAL_Dark,	13,	0,	"staraq klinika" },
	{ PXM_MazeA,	PXE_MazeA,	PAL_Maze,	19,	0,	"magazin labirinta" },
	{ PXM_MazeB,	PXE_MazeB,	PAL_LabB,	13,	14,	"labirint B" },
	{ PXM_MazeS,	PXE_MazeS,	PAL_Boulder,13,6,	"komnata kamnq" },
	{ PXM_MazeM,	PXE_MazeM,	PAL_MazeM,	24,	10,	"labirint M" },
	{ PXM_Drain,	PXE_Drain,	PAL_Regu,	3,	11,	"temnyj hod" },
	{ PXM_Almond,	PXE_Almond,	PAL_Regu,	1,	11,	"qdro" },
	/* 0x30 (48) */
	{ PXM_River,	PXE_River,	PAL_Regu,	17,	2,	"pe|ernye reki" },
	{ PXM_Eggs2,	PXE_Eggs2,	PAL_Eggs2,	23,	13,	"hranili|e qic?" },
	{ PXM_Cthu2,	PXE_Cthu2,	PAL_Regu,	19,	0,	"obitelx ktulhu?" },
	{ PXM_EggR2,	PXE_EggR2,	PAL_Sisters,19,0,	"komnata kontrolq?" },
	{ PXM_EggX,		PXE_EggX2,	PAL_Regu,	22,	0,	"qjco N.00" },
	{ PXM_Oside,	PXE_Oside,	PAL_Regu,	15,	7,	"wne}nqq stena" },
	{ PXM_EgEnd2,	PXE_EgEnd2,	PAL_Regu,	19,	0,	"bokowaq komnata" },
	{ PXM_Itoh,		PXE_Itoh,	PAL_LabB,	19,	14,	"hranili|e" },
	{ PXM_Cent,		PXE_Cent,	PAL_Regu,	4,	2,	"plantaciq" },
	{ PXM_Jail1,	PXE_Jail1,	PAL_Regu,	11,	0,	"t`rxma N.1" },
	{ PXM_Momo,		PXE_Momo,	PAL_Regu,	11,	0,	"ukrytie" },
	{ PXM_Lounge,	PXE_Lounge,	PAL_Regu,	11,	0,	"komnata otdyha" },
	{ PXM_CentW,	PXE_CentW,	PAL_Regu,	19,	0,	"teleport" },
	{ PXM_Jail2,	PXE_Jail2,	PAL_Regu,	19,	0,	"t`rxma N.2" },
	{ PXM_Blcny1,	PXE_Blcny1,	PAL_Regu,	21,	4,	"balkon" },
	{ PXM_Priso1,	PXE_Priso1,	PAL_Red,	11,	0,	"pe|era tajn" },
	/* 0x40 (64) */
	{ PXM_Ring1,	PXE_Ring1,	PAL_Miza,	25,	4,	"tronnyj zal" },
	{ PXM_Ring2,	PXE_Ring2,	PAL_Regu,	27,	4,	"korolewskie pokoi" },
	{ PXM_Prefa1,	PXE_Prefa1,	PAL_Regu,	16,	0,	"samodelxnyj domik" },
	{ PXM_Priso2,	PXE_Priso2,	PAL_Red,	11,	0,	"pe|era tajn (sekretnaq)" },
	{ PXM_Ring3,	PXE_Ring3,	PAL_Miza,	28,	0,	"temnyj zal" },
	{ PXM_Little,	PXE_Little,	PAL_Regu,	16,	0,	"dom liliputow" },
	{ PXM_Blcny2,	PXE_Blcny2,	PAL_Regu,	21,	4,	"balkon" },
	{ PXM_Fall,		PXE_Fall,	PAL_Regu,	8,	15,	"Ending" },
	{ PXM_Kings,	PXE_Kings,	PAL_Regu,	25,	0,	"Intro" },
	{ PXM_Pixel,	PXE_Pixel,	PAL_Regu,	16,	0,	"podsobka" },
	{ PXM_e_Maze,	PXE_e_Maze,	PAL_Regu,	13,	0,	"Credits" },
	{ PXM_e_Jenk,	PXE_e_Jenk,	PAL_Regu,	2,	0,	"Credits" },
	{ PXM_e_Malc,	PXE_e_Malc,	PAL_Regu,	2,	0,	"Credits" },
	{ PXM_e_Ceme,	PXE_e_Ceme,	PAL_Regu,	14,	0,	"Credits" },
	{ PXM_e_Sky,	PXE_e_Sky,	PAL_Regu,	8,	0,	"Credits" },
	{ PXM_Prefa2,	PXE_Prefa2,	PAL_Regu,	16,	0,	"samodelxnyj domik" },
	/* 0x50 (80) */
	{ PXM_Hell1,	PXE_Hell1,	PAL_Regu,	10,	12,	"zapretnaq pe|era, ur. 1" },
	{ PXM_Hell2,	PXE_Hell2,	PAL_Regu,	10,	12,	"zapretnaq pe|era, ur. 2" },
	{ PXM_Hell3,	PXE_Hell3,	PAL_Regu,	10,	12,	"zapretnaq pe|era, ur. 3" },
	{ PXM_Mapi,		PXE_Mapi,	PAL_Mapi,	3,	0,	"vili|e griba" },
	{ PXM_Hell4,	PXE_Hell4,	PAL_Regu,	10,	0,	"prohod?" },
	{ PXM_Hell42,	PXE_Hell42,	PAL_Regu,	10,	0,	"prohod?" },
	{ PXM_Statue,	PXE_Statue,	PAL_LabB,	26,	14,	"zal statuj" },
	{ PXM_Ballo1,	PXE_Ballo1,	PAL_LabB,	10,	14,	"zape{atannaq komnata" },
	{ PXM_Ostep,	PXE_Ostep,	PAL_Regu,	25,	4,	"koridor" },
	{ PXM_e_Labo,	PXE_e_Labo,	PAL_Regu,	12,	0,	"Credits" },
	{ PXM_Pole,		PXE_Pole,	PAL_Gunsmith,3,0,	"masterskaq ot}elxnika" },
	{ PXM_Island,	PXE_Island,	PAL_XX,	21,	0,	"" },
	{ PXM_Ballo2,	PXE_Ballo2,	PAL_LabB,	10,	14,	"Seal Chamber" },
	{ PXM_e_Blcn,	PXE_e_Blcn,	PAL_Regu,	21,	0,	"Credits" },
	{ PXM_Clock,	PXE_Clock,	PAL_Regu,	15,	7,	"komnata {asow" },
};

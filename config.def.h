/* See LICENSE file for copyright and license details. */

#define SESSION_FILE "/tmp/dwm-session"
#include "gaplessgrid.c"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel       = 0;        /* 0 allows the user to scroll window without changing focus */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-n", "spfm", "-g", "144x41", "-e", "ranger", NULL };
const char *spcmd3[] = {"keepassxc", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spranger",    spcmd2},
	{"keepassxc",   spcmd3},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
    /* swallow-2020 patch */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  resizehints  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,			1,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,			1,        -1 },
	{ "St",      NULL,     NULL,           0,         0,          1,           0,			1,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,			1,        -1 }, /* xev */
    /* scratchpads-2020 patch */
 	/* class      instance    title       tags mask     isfloating   monitor */
	/* { NULL,		  "spterm",		NULL,		SPTAG(0),		1,			 -1 }, */
	/* { NULL,		  "spfm",		NULL,		SPTAG(1),		1,			 -1 }, */
	/* { NULL,		  "keepassxc",	NULL,		SPTAG(2),		0,			 -1 }, */
	{ NULL,		 "spterm",		NULL,		SPTAG(0),	1,			1,			-1,			1,			 -1 },
	{ NULL,		 "spfm",		NULL,		SPTAG(1),	1,			1,			-1,			1,			 -1 },
	{ NULL,		 "keepassxc",	NULL,		SPTAG(2),	0,			0,			-1,			1,			 -1 },
    /* clientresizehints-6.5 patch */
	/* class      instance    title       tags mask     isfloating   monitor    resizehints */
	/* { "Gimp",     NULL,       NULL,       0,            1,           -1,        1}, */
	/* { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1,        1}, */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "HHH",      gaplessgrid },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };
#include "shift-tools.c"
#include "exitdwm.c"
#include "movestack.c"

/* keysequence-2025 patch, template for keysequences */
/* static Key keyseq_a[] = { */
/*   { 0,	        XK_t,	setlayout,	    {.v = &layouts[0]}}, */
/*   { ShiftMask,	XK_t,	setlayout,	    {.v = &layouts[1]}}, */
/*   { MODKEY,		XK_y,	setlayout,	    {.v = &layouts[2]}}, */
/*   {0} */
/* } */

/* static Key keys[] = { */
/*   { MODKEY,       XK_a,   keypress_other, {.v = keyseq_a}}, */
/*   {0} */
/* } */

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_o, shiftviewclients,    { .i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,	shiftview,         { .i = +1 } },
	{ MODKEY|ShiftMask,             XK_i,	shiftview,         { .i = -1 } },
	{ MODKEY,	                XK_i, shiftviewclients,    { .i = -1 } },
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	/* { MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} }, /\* cfact patch, clashes with shift-tools patch *\/ */
	/* { MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} }, */
	/* { MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} }, */
	{ MODKEY|ControlMask|ShiftMask, XK_h,     setcfact, {.f = +0.25} },
	{ MODKEY|ControlMask|ShiftMask, XK_l,     setcfact, {.f = -0.25} },
	{ MODKEY|ControlMask|ShiftMask, XK_o,     setcfact, {.f = 0.00} },
	{ MODKEY|ShiftMask,		XK_h,      shiftboth,      { .i = -1 }	},
	{ MODKEY|ControlMask,		XK_h,      shiftswaptags,  { .i = -1 }	},
	{ MODKEY|ControlMask,		XK_l,      shiftswaptags,  { .i = +1 }	},
	{ MODKEY|ShiftMask,             XK_l,      shiftboth,      { .i = +1 }	},
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
    { MODKEY|ControlMask,           XK_c,      killclient,     {.ui = 1} },  // kill unselect
    { MODKEY|ShiftMask|ControlMask, XK_c,      killclient,     {.ui = 2} },  // killall
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_Down,   moveresize,     {.v = "0x 25y 0w 0h" } },
	{ MODKEY,                       XK_Up,     moveresize,     {.v = "0x -25y 0w 0h" } },
	{ MODKEY,                       XK_Right,  moveresize,     {.v = "25x 0y 0w 0h" } },
	{ MODKEY,                       XK_Left,   moveresize,     {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = "0x 0y 0w 25h" } },
	{ MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = "0x 0y 0w -25h" } },
	{ MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = "0x 0y 25w 0h" } },
	{ MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = "0x 0y -25w 0h" } },
	{ MODKEY|ControlMask,           XK_Up,     moveresizeedge, {.v = "t"} },
	{ MODKEY|ControlMask,           XK_Down,   moveresizeedge, {.v = "b"} },
	{ MODKEY|ControlMask,           XK_Left,   moveresizeedge, {.v = "l"} },
	{ MODKEY|ControlMask,           XK_Right,  moveresizeedge, {.v = "r"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Up,     moveresizeedge, {.v = "T"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Down,   moveresizeedge, {.v = "B"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Left,   moveresizeedge, {.v = "L"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Right,  moveresizeedge, {.v = "R"} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_F5,     xrdb,           {.v = NULL } },
	{ MODKEY,            			XK_y,  	   togglescratch,  {.ui = 0 } },
	{ MODKEY,            			XK_u,	   togglescratch,  {.ui = 1 } },
	{ MODKEY,            			XK_x,	   togglescratch,  {.ui = 2 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	/* { MODKEY|ShiftMask,             XK_j,      focusbynum,     {.i = 0} }, /\* focusbynum patch, clashes with shift-tools, movestack and cfact patches *\/ */
	/* { MODKEY|ShiftMask,             XK_k,      focusbynum,     {.i = 1} }, */
	/* { MODKEY|ShiftMask,             XK_l,      focusbynum,     {.i = 2} }, */
	/* { MODKEY|ShiftMask,             XK_colon,  focusbynum,     {.i = 3} }, */
	/* { MODKEY|ShiftMask,             XK_f,      focusbynum,     {.i = 4} }, */
	/* { MODKEY|ShiftMask,             XK_d,      focusbynum,     {.i = 5} }, */
	/* { MODKEY|ShiftMask,             XK_s,      focusbynum,     {.i = 6} }, */
	/* { MODKEY|ShiftMask,             XK_a,      focusbynum,     {.i = 7} }, */
	{ MODKEY|ControlMask,             XK_j,      focusbynum,     {.i = 0} },
	{ MODKEY|ControlMask,             XK_k,      focusbynum,     {.i = 1} },
	{ MODKEY|ControlMask,             XK_f,      focusbynum,     {.i = 2} },
	{ MODKEY|ControlMask,             XK_d,      focusbynum,     {.i = 3} },
	{ MODKEY|ControlMask,             XK_s,      focusbynum,     {.i = 4} },
	{ MODKEY|ControlMask,             XK_a,      focusbynum,     {.i = 5} },
	{ MODKEY|ShiftMask,             XK_q,      exitdwm,        {0} }, /* exitmenu patch, modified */
	{ MODKEY,                       XK_o,      winview,        {0} },
	{ MODKEY,                       XK_g,      gridall,        {} },
	{ MODKEY,                       XK_r,      winviewmono,    {} },
	{0}
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigstatusbar,   {.i = 5} },
	{ ClkStatusText,        0,              6,              sigstatusbar,   {.i = 6} },
	{ ClkStatusText,        0,              7,              sigstatusbar,   {.i = 7} },
	{ ClkStatusText,        0,              8,              sigstatusbar,   {.i = 8} },
	{ ClkStatusText,        0,              9,              sigstatusbar,   {.i = 9} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	/* { ClkClientWin,         MODKEY,         Button1,        resizemouse,    {0} }, /\* scratchpads-2020 patch, rejecting *\/ */
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signum>"` */
static Signal signals[] = {
	/* signum       function        argument  */
	{ 1,            setlayout,      {.v = 0} },
};

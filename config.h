/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
#define ICONSIZE 16                             /* icon size */
#define ICONSPACING 5                           /* space between icon and title */
static const char *fonts[]          = { "DejaVuSans:size=10", "NotoColorEmoji:pixelsize=10:antialias=true:autohint=true"  };
static const char dmenufont[]       = "DejaVuSans:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-n", "spfm", "-g", "120x34", "-e", "ranger", NULL };
const char *spcmd3[] = {"st", "-n", "spgopass", "-g", "120x34", "-e", "gopass", NULL };
const char *spcmd4[] = {"st", "-n", "spnotes", "-g", "120x34", "-e", "emacs", "-nw", "/home/rbartl/ownCloud/org/notes.org", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",       spcmd1},
	{"spfm",         spcmd2},
	{"spgopass",     spcmd3},
	{"spnotes",      spcmd4},
};


/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *  WM_CLASS(STRING) = instance, class
	 *  WM_NAME(STRING) = title
	 */
	/* class          instance       title       tags mask     isfloating   monitor */
	{ "Gimp",         NULL,          NULL,       1 << 7,       1,           -1 },
	{ "Firefox",      NULL,          NULL,       1 << 8,       0,           -1 },
	{ "Thunderbird",  NULL,          NULL,       1 << 8,       0,           -1 },
	{ "Vlc",          "vlc",         "webcam",   1,            1,           -1 },
	{ NULL,           "spterm",      NULL,       SPTAG(0),     1,           -1 },
	{ NULL,           "spfm",        NULL,       SPTAG(1),     1,           -1 },
	{ NULL,           "spgopass",    NULL,       SPTAG(2),     1,           -1 },
	{ NULL,           "spnotes",     NULL,       SPTAG(3),     1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
#include "movestack.c"
#include <X11/XF86keysym.h>

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "[H]",      deck },
	{ "|M|",      centeredmaster },

	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ ">M>",      centeredfloatingmaster },
	{ NULL,       NULL },
};

/* key definitions */
//#define MODKEY Mod1Mask
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd ,NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
/* static const char *termcmd[]  = { "st", NULL }; */
static const char *termcmd[]        = { "st", "-e", "tmux", NULL };
static const char *emacscmd[]       = { "emacs", NULL };
static const char *browsercmd[]     = { "chrome", NULL };
static const char *mailcmd[]        = { "thunderbird", NULL };
static const char *brightupcmd[]    = { "intel_backlight", "incr", NULL };
static const char *brightdowncmd[]  = { "intel_backlight", "decr", NULL };
static const char *volumemutecmd[]  = { "mixer", "vol",   "0", NULL };
static const char *volumeupcmd[]    = { "mixer", "vol", "+10", NULL };
static const char *volumedowncmd[]  = { "mixer", "vol", "-10", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	/* { MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } }, */
	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_e,      spawn,          {.v = emacscmd } },
	{ MODKEY|ShiftMask,             XK_b,      spawn,          {.v = browsercmd } },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          {.v = mailcmd } },
	{ MODKEY,                       XK_F1,     spawn,          SHCMD("mixer vol   0; kill -39 $(pidof dwmblocks)") },
	{ MODKEY,                       XK_F2,     spawn,          SHCMD("mixer vol -10; kill -39 $(pidof dwmblocks)") },
	{ MODKEY,                       XK_F3,     spawn,          SHCMD("mixer vol +10; kill -39 $(pidof dwmblocks)") },
	{ MODKEY,                       XK_F5,     spawn,          SHCMD("intel_backlight decr; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,                       XK_F6,     spawn,          SHCMD("intel_backlight incr; kill -44 $(pidof dwmblocks)") },

	/* FN Keys */
	{ 0, XF86XK_AudioMute,	                   spawn,	        SHCMD("mixer vol 0; kill -39 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioRaiseVolume,              spawn,           SHCMD("mixer vol +10; kill -39 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioLowerVolume,              spawn,           SHCMD("mixer vol -10; kill -39 $(pidof dwmblocks)") },
	{ 0, XF86XK_MonBrightnessUp,               spawn,           SHCMD("intel_backlight incr; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_MonBrightnessDown,             spawn,           SHCMD("intel_backlight decr; kill -44 $(pidof dwmblocks)") },

	// Not working
	{ 0, XF86XK_Keyboard,                      spawn,           SHCMD("notify-send 'FN KEY'") },
	{ 0, XF86XK_Display,                       spawn,           SHCMD("notify-send 'FN KEY'") },
	{ 0, XF86XK_WLAN,                          spawn,           SHCMD("notify-send 'FN KEY'") },
	{ 0, XF86XK_Bluetooth,                     spawn,           SHCMD("notify-send 'FN KEY'") },
	{ 0, XF86XK_AudioMicMute,                  spawn,           SHCMD("notify-send 'FN KEY'") },
	{ 0, XF86XK_Favorites,                     spawn,           SHCMD("notify-send 'FN KEY'") },
	{ 0, XF86XK_Launch1,                       spawn,           SHCMD("notify-send 'FN KEY'") },

	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	/* { MODKEY,                       XK_Tab,    zoom2,          {.i = +1} }, */
	/* { MODKEY|ShiftMask,             XK_Tab,    zoom2,          {.i = -1} }, */
	{ MODKEY,                       XK_Tab,    rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Tab,    rotatestack,    {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },

	{ MODKEY|ShiftMask,             XK_j,      aspectresize,   {.i = +24} },
	{ MODKEY|ShiftMask,             XK_k,      aspectresize,   {.i = -24} },

	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },

	/* Modify Gaps */
	{ MODKEY|Mod1Mask,              XK_h,      incrgaps,       {.i = +5 } },
	{ MODKEY|Mod1Mask,              XK_l,      incrgaps,       {.i = -5 } },
	{ MODKEY|Mod1Mask,              XK_0,      defaultgaps,    {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      togglegaps,     {0} },


	{ MODKEY,                       XK_Return, zoom,           {0} },
	/* { MODKEY,                       XK_Tab,    view,           {0} }, */
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },

	/* Layouts */
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ControlMask,           XK_r,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ControlMask,           XK_t,      setlayout,      {.v = &layouts[5]} },
	/* gapless grid */
	{ MODKEY|ControlMask,           XK_m,      setlayout,      {.v = &layouts[12]} },
	{ MODKEY|ControlMask,		    XK_comma,  cyclelayout,    {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },

	/* { MODKEY,                       XK_space,  setlayout,      {0} }, */
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_s,      togglesticky,   {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_x,      togglescratch,  {.ui = 0 } },  // term (xterm)
	{ MODKEY|ShiftMask,             XK_o,      togglescratch,  {.ui = 1 } },  // lf (open)
	{ MODKEY|ShiftMask,             XK_p,      togglescratch,  {.ui = 2 } },  // gopass (pass)
	{ MODKEY|ShiftMask,             XK_n,      togglescratch,  {.ui = 3 } },  // notes
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY,                       XK_q,      killclient,     {0} },
	/* { MODKEY|ShiftMask,             XK_q,      quit,           {0} }, */
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	/* { ClkLtSymbol,          0,              Button1,        setlayout,      {0} }, */
	/* { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} }, */
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1 } },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2 } },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3 } },
	{ ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigstatusbar,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigstatusbar,   {.i = 6} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};


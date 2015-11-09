/* See LICENSE file for copyright and license details. */

/* appearance */
static const char *fonts[] = {
	"terminus:size=10"
};
static const char dmenufont[]       = "terminus:size=10";
static const char normbordercolor[] = "#282828";
static const char normbgcolor[]     = "#282828";
static const char normfgcolor[]     = "#585858";
static const char selbordercolor[]  = "#7cafc2";
static const char selbgcolor[]      = "#7cafc2";
static const char selfgcolor[]      = "#181818";
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const Bool showbar           = 1;        /* False means no bar */
static const Bool topbar            = 1;        /* False means bottom bar */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class         instance    title       tags mask     isfloating   monitor */
	{ "Firefox",     NULL,       NULL,       0,            0,           -1 },
	{ "Vimperator",  NULL,       NULL,       0,            0,           -1 },
};

/* layout(s) */
static const float mfact      = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = 0;    /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",         tile },    /* first entry is default */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "urxvtc", NULL };

/* volume control */
static const char *raisevol[] = { "amixer", "-q", "set", "Master", "5%+", NULL };
static const char *lowervol[] = { "amixer", "-q", "set", "Master", "5%-", NULL };

/* backlight control */
static const char *raiseblight[] = { "xbacklight", "-inc", "5", NULL };
static const char *lowerblight[] = { "xbacklight", "-dec", "5", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_e,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_r,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_t,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_r,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_t,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_n,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_u,      togglebar,      {0} },
	{ MODKEY,                       XK_f,      togglefullscrn, {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_minus,  view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_minus,  tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	TAGKEYS(                        XK_0,                      9)

	/* volume level control hotkeys */

	{ 0, XF86XK_AudioRaiseVolume,  spawn, {.v = raisevol } },
	{ 0, XF86XK_AudioLowerVolume,  spawn, {.v = lowervol } },

	/* backlight brightness control hotkeys */

	{ 0, XF86XK_MonBrightnessUp,   spawn, {.v = raiseblight } },
	{ 0, XF86XK_MonBrightnessDown, spawn, {.v = lowerblight } },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
};


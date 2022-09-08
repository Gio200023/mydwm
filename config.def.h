/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx = 0; /* border pixel of windows */
static const int CORNER_RADIUS = 10;    /* broder rounder */
static const unsigned int gappx = 10;   /* gap pixel between windows */
static const unsigned int snap = 32;    /* snap pixel */
static const int showbar = 1;           /* 0 means no bar */
static const int topbar = 1;            /* 0 means bottom bar */
static const double defaultopacity = 1; /* change opacity */
static const int vertpad = 10;          /* vertical padding of bar */
static const int sidepad = 10;          /* horizontal padding of bar */
static const int user_bh = 30;          /* 0 means that dwm will calculate bar height, >=
                                           1 means dwm will user_bh as bar height */
static const char *fonts[] = {
    "monospace:size=20",
    "Symbols Nerd Font:pixelsize=21:antialias=true;autohint=true"};
static const char col_gray1[] = "#222222";
static const char col_gray2[] = "#ebeeff";
static const char col_gray3[] = "#bbbbbb";
static const char col_gray4[] = "#eeeeee";
static const char col_bluDistroTube[] = "#292d3e";
static const char *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeNorm] = {col_gray3, col_gray1, col_gray2},
    [SchemeSel] = {col_gray4, col_bluDistroTube, col_gray2},
    [SchemeStatus] = {col_gray2, col_bluDistroTube,
                      "#000000"}, // Statusbar right {text,background,not used
                                  // but cannot be empty}
    [SchemeTagsSel] = {col_gray2, col_bluDistroTube,
                       "#000000"}, // Tagbar left selected {text,background,not
                                   // used but cannot be empty}
    [SchemeTagsNorm] =
        {col_gray3, col_gray1,
         "#000000"}, // Tagbar left unselected {text,background,not used but
                     // cannot be empty}
    [SchemeInfoSel] =
        {col_gray4, col_bluDistroTube,
         "#000000"}, // infobar middle  selected {text,background,not used but
                     // cannot be empty}
    [SchemeInfoNorm] =
        {col_gray3, col_gray1,
         "#000000"}, // infobar middle  unselected {text,background,not used but
                     // cannot be empty}

};

/* tagging */
static const char *tags[] = {"", "", "", "", "", "", ""};
static const char *tagsalt[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
static const int momentaryalttags = 0; // 1 means alltags show when key pressed

/* launcher commands (They must be NULL terminated) */
static const char *surf[] = {"surf", "duckduckgo.com", NULL};

static const Launcher launchers[] = {
    /* command       name to display */
    {surf, ""},
};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     switchtotag isfloating
       monitor */
    {"Gimp", NULL, NULL, 0, 0, 1, -1},
    {"firefox", NULL, NULL, 1 << 0, 1, 0, -1},
    {"Telegram", NULL, NULL, 1 << 6, 1, 0, -1},
    {"Spotify", NULL, NULL, 1 << 5, 1, 0, -1},
};

/* layout(s) */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
    1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"﵁", tile}, /* first entry is default */
    {"恵", NULL},  /* no layout function means floating behavior */
    {"[M]", monocle},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                          \
    {MODKEY, KEY, view, {.ui = 1 << TAG}},                         \
        {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}}, \
        {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},          \
        {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                           \
    {                                                        \
        .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL } \
    }

/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *rofi[] = {"rofi", "-show", "run", NULL};
static const char *termcmd[] = {"alacritty", NULL};
static const char *browser[] = {"firefox", NULL};
static const char *lock[] = {"i3lock-fancy", "-t \"stronzo non provarci\" ",
                             NULL};
static const char *nemo[] = {"nemo", NULL};
static const char *flameshot[] = {"flameshot", "gui", NULL};

static Key keys[] = {
    /* modifier                     key        function        argument */
    {MODKEY, XK_p, spawn, {.v = rofi}},
    {MODKEY, XK_t, spawn, {.v = termcmd}},
    {MODKEY, XK_s, spawn, {.v = browser}},
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_d, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    {MODKEY, XK_Return, zoom, {0}},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY, XK_w, killclient, {0}},
    {MODKEY, XK_Return, setlayout, {.v = &layouts[0]}},
    {MODKEY | ShiftMask, XK_f, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
    {MODKEY, XK_space, setlayout, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
    {MODKEY, XK_n, togglealttag, {0}},
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8){MODKEY | ShiftMask, XK_q, quit, {0}},
    {MODKEY | ShiftMask, XK_l, spawn, {.v = lock}},
    {MODKEY, XK_e, spawn, {.v = nemo}},
    {MODKEY | ShiftMask, XK_s, spawn, SHCMD("transset-df -a --dec .1")},
    {MODKEY | ShiftMask, XK_d, spawn, SHCMD("transset-df -a --inc .1")},
    {MODKEY, XK_f, spawn, {.v = flameshot}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function argument
     */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};

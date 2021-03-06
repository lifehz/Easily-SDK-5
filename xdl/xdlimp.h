﻿
#ifndef _XDLIMP_H
#define _XDLIMP_H

#include "impjmp.h"
#include "impmem.h"
#include "impassert.h"
#include "imperr.h"


#ifdef XDK_SUPPORT_MBCS
#include "impmbcs.h"
#endif

#ifdef XDK_SUPPORT_DATE
#include "impdate.h"
#endif

#ifdef XDK_SUPPORT_ASYNC
#include "impasync.h"
#endif

#ifdef XDK_SUPPORT_THREAD
#include "impthr.h"
#endif

#ifdef XDK_SUPPORT_TIMER
#include "imptimer.h"
#endif

#ifdef XDK_SUPPORT_FILE
#include "impuncf.h"
#endif

#ifdef XDK_SUPPORT_SOCK
#include "impsock.h"
#endif

#ifdef XDK_SUPPORT_SHARE
#include "impshare.h"
#endif

#ifdef XDK_SUPPORT_PROCESS
#include "impproc.h"
#endif

#ifdef XDK_SUPPORT_PIPE
#include "imppipe.h"
#endif

#ifdef XDK_SUPPORT_COMM
#include "impcomm.h"
#endif

#ifdef XDK_SUPPORT_CONS
#include "impcons.h"
#endif

#ifdef XDK_SUPPORT_SHELL
#include "impshell.h"
#endif

#ifdef XDK_SUPPORT_CLIPBOARD
#include "impclip.h"
#endif

#ifdef XDK_SUPPORT_CONTEXT
#include "impcontext.h"
#endif

#ifdef XDK_SUPPORT_CONTEXT_BITMAP
#include "impbitmap.h"
#endif

#ifdef XDK_SUPPORT_CONTEXT_GRAPHIC
#include "impgdi.h"
#endif

#ifdef XDK_SUPPORT_WIDGET
#include "impwin.h"
#endif

#include "impicon.h"

#endif //_XDLIMP_H

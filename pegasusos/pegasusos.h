#ifndef PEGASUSOS_H
#define PEGASUSOS_H

/* C Lib includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include <inttypes.h>

#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>

/* Board specifiques defs */
#include <boardsDefs.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "List.h"

#include <pegasushal.h>

#include "Trace.h"
#include "Thread.h"
#include "Queue.h"
#include "Module.h"
#include "ModuleManager.h"

#endif

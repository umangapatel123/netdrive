#ifndef CLIENT_H
#define CLIENT_H

#include "naming_server.h"
#include "errorcode.h"
#include "command/help.h"
#include "command/makedir.h"
#include "command/makefile.h"
#include "command/deletefile.h"
#include "command/deletedir.h"
#include "command/list.h"
#include "command/read.h"
#include "command/write.h"
#include "command/info.h"
#include "command/copyfile.h"
#include "command/copydir.h"
#include <wait.h>

#define NM_PORT 51234
#define ip "127.0.0.1"

#endif
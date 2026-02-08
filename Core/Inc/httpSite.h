#ifndef  _HTTP_SITE_H_
#define  _HTTP_SITE_H_

#include "main.h"

#include "lwip/apps/httpd.h"
#include "lwip/apps/fs.h"

#include <string.h>
#include <stdio.h>

typedef struct
{
    char name[32];
    char tunerMode[8];
    char polarization[2];
    uint32_t tunerSymbolRate;
    uint32_t tunerFrequency;
    float satelliteLon;

}preset_t;


void http_server_init(void);
#endif

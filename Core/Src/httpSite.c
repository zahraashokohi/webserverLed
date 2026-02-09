#include "httpSite.h"

static uint8_t fileBuffer[1024] = {0};
preset_t 			gpresets;
realTimeData  realData;

int fs_open_custom(struct fs_file *file, const char *name) {
  if (strcmp(name, "/timestamp.html") == 0) {
    file->len = snprintf((char*) fileBuffer, sizeof(fileBuffer) - 1, "%u", HAL_GetTick());
    file->data = (const char*) fileBuffer;
  #if LWIP_HTTPD_DYNAMIC_FILE_READ
    file->index = 0;
  #else
    file->index = file->len;
  #endif
    file->flags = 0;
    return 1;
  }
  else if (strcmp(name, "/version.html") == 0) {
    file->len = snprintf((char*) fileBuffer, sizeof(fileBuffer) - 1, "V%u.%u.%u",
      (HAL_GetHalVersion() >> 24) & 0xFF,
      (HAL_GetHalVersion() >> 16) & 0xFF,
      (HAL_GetHalVersion() >> 8)  & 0xFF
    );
    file->data = (const char*) fileBuffer;
  #if LWIP_HTTPD_DYNAMIC_FILE_READ
    file->index = 0;
  #else
    file->index = file->len;
  #endif
    file->flags = 0;
    return 1;
  }





  return 0;
}
void fs_close_custom(struct fs_file *file) {
  file->data = NULL;
  file->len = 0;
  file->index = 0;
  file->flags = 0;
//  file->is_custom_file = 0;
}
int fs_read_custom(struct fs_file *file, char *buffer, int count) {
  int32_t bytesLeft = file->len - file->index;

  if (bytesLeft > 0) {
    if (bytesLeft > count) {
      bytesLeft = count;
    }

    memcpy(buffer, &file->data[file->index], bytesLeft);

    file->index += bytesLeft;
  }

  return (file->len - file->index) > 0 ? bytesLeft : FS_READ_EOF;
}


const char* LedCGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    for (int i = 0; i < iNumParams; i++)
    {
        if (strcmp(pcParam[i], "led") == 0)
        {
            if (strcmp(pcValue[i], "on") == 0)
            {
                HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_RESET);
            }
            else if (strcmp(pcValue[i], "off") == 0)
            {
                HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_SET);
            }
        }
    }

    return "/index.html";
}

const char* PresetsCGI_Handler (int iIndex, int iNumParams,char *pcParam[],char *pcValue[])
{
    for (int i = 0; i < iNumParams; i++)
    {
        if (!strcmp(pcParam[i], "name"))
            strncpy(gpresets.name, pcValue[i], sizeof(gpresets.name));

        else if (!strcmp(pcParam[i], "tunerMode"))
            strncpy(gpresets.tunerMode, pcValue[i], sizeof(gpresets.tunerMode));

        else if (!strcmp(pcParam[i], "polarization"))
            strncpy(gpresets.polarization, pcValue[i], sizeof(gpresets.polarization));

        else if (!strcmp(pcParam[i], "tunerSymbolRate"))
        	gpresets.tunerSymbolRate = atoi(pcValue[i]);

        else if (!strcmp(pcParam[i], "tunerFrequency"))
        	gpresets.tunerFrequency = atoi(pcValue[i]);

        else if (!strcmp(pcParam[i], "satelliteLon"))
        	gpresets.satelliteLon = atof(pcValue[i]);
    }
// save to flash

    return "/presets.html";
}

static const char *ssi_tags[] = {
    "beam", "heading", "lat", "lon",
    "az", "el", "pol"
};
u16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen)
{
    int len = 0;

    switch (iIndex)
    {
    case 0: // beam
        len = snprintf(pcInsert, iInsertLen, "%.2f", realData.beamValue);
        break;

    case 1: // heading
        len = snprintf(pcInsert, iInsertLen, "%.2f", realData.heading);
        break;

    case 2: // lat
        len = snprintf(pcInsert, iInsertLen, "%.4f", realData.locationlat);
        break;

    case 3: // lon
        len = snprintf(pcInsert, iInsertLen, "%.4f", realData.locationLong);
        break;

    case 4: // az
        len = snprintf(pcInsert, iInsertLen, "%s", realData.Azimuth);
        break;

    case 5: // el
        len = snprintf(pcInsert, iInsertLen, "%s", realData.elevation);
        break;

    case 6: // pol
        len = snprintf(pcInsert, iInsertLen, "%s", realData.polar);
        break;
    }

    return len;
}

const tCGI CgiTable[] =
{
    {"/",                LedCGI_Handler},
    {"/savePresets.cgi", PresetsCGI_Handler},
		//{"/status.json",     StatusJSON_Handler},    // این خط را اضافه کنید
  //  {"/status.cgi",        status_cgi_handler},
   // {"/move.cgi",         MoveCGI_Handler}
};
void http_server_init(void)
{
	 realData.beamValue = 1.0;
	    realData.heading = 0.0;
	    realData.locationlat = 0.0;
	    realData.locationLong = 0.0;
	    strcpy(realData.Azimuth, "STOP");
	    strcpy(realData.elevation, "STOP");
	    strcpy(realData.polar, "STOP");
    httpd_init();

    http_set_cgi_handlers(CgiTable,sizeof(CgiTable)/sizeof(tCGI));
    http_set_ssi_handler(
        ssi_handler,
        ssi_tags,
        sizeof(ssi_tags) / sizeof(ssi_tags[0])
    );


}



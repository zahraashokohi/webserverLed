#include "httpSite.h"

static uint8_t fileBuffer[1024] = {0};

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
const tCGI CgiTable[] =
{
    {"/",                LedCGI_Handler},
   // {"/saveSettings.cgi", SaveSettingsCGI},
   // {"/motor.cgi",        MotorCGI_Handler},
   // {"/move.cgi",         MoveCGI_Handler}
};
void http_server_init(void)
{
    httpd_init();
    http_set_cgi_handlers(CgiTable,sizeof(CgiTable)/sizeof(tCGI));
}



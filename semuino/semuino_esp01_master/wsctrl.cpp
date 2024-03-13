#include "wsctrl.h"

#include <ESP8266WebServer.h>
#include <uri/UriBraces.h>

ESP8266WebServer _server(80);

void _handleNotFound(void);
void _handleGetInfo(void);

/**
 * @brief Controleur WS de recuperation des infos
 *
 * URL: /semuino/info
 *
 * Exemple: "/petuino/2/status" pour récupérer le statut du module esclave 2
*/
void _handleGetInfo(void)
{
  _server.send(400, "text/plain", "A CODER!");
}

/**
 * @brief Controleur WS en cas d'erreur
*/
void _handleNotFound(void)
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += _server.uri();
  message += "\nMethod: ";
  message += (_server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += _server.args();
  message += "\n";
  for (uint8_t i = 0; i < _server.args(); i++)
  {
    message += " " + _server.argName(i) + ": " + _server.arg(i) + "\n";
  }
  _server.send(404, "text/plain", message);
}

void wsctrl_init(void)
{
  _server.on(UriBraces("/info"), _handleGetInfo);
  _server.onNotFound(_handleNotFound);

  _server.begin();
  _server.enableCORS(true);  
}

void wsctrl_loop(void)
{
  _server.handleClient();
}

# led_display
Pantalla Led dinamica, proyecto para IEM4060 UASD 

Participantes: 
Franklyn Terrero 100575264  
Kelvin Diaz 100630111
Oliver Rodriguez 100309119
Daniel Angeles 100438521 

Pantalla LED controlada por ESP32 


Este proyecto permite controlar un display LED (MAX7219) mediante un ESP32 a través de una interfaz web. Se pueden enviar mensajes, elegir entre modo fijo o desplazamiento (scroll), y ajustar la velocidad del desplazamiento.

Requisitos de hardware
ESP32 Dev Module (compatible con WiFi 2.4 GHz)

Display LED con controladores MAX7219 (4 módulos conectados en serie)

Conexiones mediante cables Dupont (según pines definidos en el código)

Fuente de alimentación estable (vía USB o fuente externa de 5V si el consumo del display lo requiere)

Librerías requeridas (Arduino IDE)
WiFi.h (incluida en las placas ESP32)

MD_Parola (desarrollada por MajicDesigns, disponible en el Gestor de Librerías)

MD_MAX72xx (desarrollada por MajicDesigns, disponible en el Gestor de Librerías)

SPI.h (incluida por defecto en el entorno Arduino)

Requisitos de red WiFi
Red WiFi operando en 2.4 GHz (las placas ESP32 no son compatibles con redes exclusivas de 5 GHz)

Nombre de red (SSID) y contraseña configurados correctamente en el código fuente (variables ssid y password)

El router debe permitir conexiones entre clientes locales (desactivar aislamiento si está activo)

Evitar filtrado MAC en el router si se presenta algún problema de conexión

Configuración del entorno de desarrollo
Instalar drivers USB correspondientes al modelo de ESP32:

Si la placa usa chip CP2102, descargar desde: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers

Si la placa usa chip CH340/CH341, descargar desde: https://sparks.gogo.co.nz/ch340.html

En el Arduino IDE:

Seleccionar Placa → ESP32 Dev Module

Seleccionar el puerto correcto en Herramientas → Puerto

Configurar velocidad de carga a 115200 baudios (ajustable si se presentan errores)

Cargar el sketch al ESP32

Abrir el Monitor Serie para obtener la dirección IP asignada por el router

Acceder a la interfaz web ingresando la dirección IP en un navegador conectado a la misma red

# absurdo timbre
sonidos diferentes usando un timbre

Demo: https://youtu.be/Yj5uMbEvICs

## Componentes:
* Esp32-wroom-32
* PCM5102 (convertidor digital a analógico)
* Pam8403 (amplificador)
* Pulsadores (o timbre)
* Altavoz

## Conexiones:
### Entre el Esp32 y el PCM5102

3.3V → 3.3V

GND → GND

FLT → GND

DMP → GND

SCL → GND

FMT → GND

XMT → 3.3V

BCK → GPIO27

DIN → GPIO26

LCK → GPIO25


### Entre el PCM5102 y el PAM8403 (un solo altavoz, un solo canal)

ROUT → INPUT R

GND → INPUT GND

### Entre el PAM8403 y el ALTAVOZ (un solo altavoz, un solo canal)

OUT+ y OUT– de ese canal → los dos terminales del altavoz

### Entre el ESP32 y el PAM8403 (para la alimentación)

VIN → POWER +

GND → POWER -



### Entre el ESP32 y los pulsadores
Están disponibles estas entradas del esp32 para usarlas como pulsadores: GPIO 4, GPIO 5, GPIO 13, GPIO 16, GPIO 17, GPIO 18, GPIO 19, GPIO 21, GPIO 22 y GPIO 23.

Los pulsadores deben conectar esos pines con el GND del ESP32.

## Instalación del código:

Usar Visual Code, instalando platformIO. 

Modificar el código para decidir cuántos sonidos se quieren y en qué pines (ver los comentarios del código).

Primero subir los mp3 (ojo, hay un límite de 3 megas aprox entre todos). Los mp3 no pueden tener TAGS. Se ponen en la carpeta DATA y se suben con la opción: "Upload Filesystem Image".

Luego subir el código con la opción "Upload and monitor".




## Resolución de problemas:
* El driver del esp32 no funciona en windows -> estoy usando los drivers "Silicon Labs CP210x USB to UART Bridge" version 11.4.0.393. Si falla, recomiendan usar la v6
* Al subir el código o los mp3, falla en "Connecting..." -> en ese momento hay que mantener pulsado el botón "boot" del esp32 hasta que empiece a subir (y en ese momento, se despulsa)


## Fotos de los componentes:

<img width="402" height="348" alt="imagen" src="https://github.com/user-attachments/assets/858298e7-0450-4217-8b09-e72b26d2da06" />
<img width="255" height="196" alt="imagen" src="https://github.com/user-attachments/assets/a419e3ff-3599-485b-9a01-78fe06adadfa" />
<img width="379" height="338" alt="imagen" src="https://github.com/user-attachments/assets/ec509433-064a-4a35-aafd-abec799f4354" />

## Esquema del ESP32:


<img width="885" height="462" alt="imagen" src="https://github.com/user-attachments/assets/199372f5-b4ba-4d61-8b83-be1f81413259" />



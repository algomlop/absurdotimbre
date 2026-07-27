#include <Arduino.h>
#include <LittleFS.h>
#include <driver/i2s.h>
#include <AudioFileSourceLittleFS.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>

// ============================================================
// CONFIGURACIÓN: añade/quita pines aquí para cambiar el número
// de canciones. Los mp3 deben llamarse 1.mp3, 2.mp3, 3.mp3...
// en el mismo orden que los pines de esta lista.
// GPIOs disponibles en tu ESP32 (candidatos): 4, 5, 13, 16, 17, 18, 19, 21, 22, 23
// ============================================================
const int pinesBotones[] = {32, 33, 14};
// Para ampliar, por ejemplo: const int pinesBotones[] = {32, 33, 14, 4, 5, 13, 16, 17, 18, 19};

// Número de canciones/botones, calculado automáticamente
const int NUM_CANCIONES = sizeof(pinesBotones) / sizeof(pinesBotones[0]);

// Nombres de los mp3 generados automáticamente: "/1.mp3", "/2.mp3", ...
char canciones[NUM_CANCIONES][8];

// Pines I2S hacia el PCM5102
#define PIN_BCK 27
#define PIN_LCK 25
#define PIN_DIN 26

unsigned long ultimoPulso[NUM_CANCIONES] = {0};
const unsigned long DEBOUNCE_MS = 200;

AudioGeneratorMP3 *mp3;
AudioFileSourceLittleFS *file;
AudioOutputI2S *out;

int indiceActual = 0;

void rampaGain(float desde, float hasta, int pasos, int msPorPaso)
{
    for (int i = 0; i <= pasos; i++)
    {
        float g = desde + (hasta - desde) * i / pasos;
        out->SetGain(g);
        delay(msPorPaso);
    }
}

void reproducirCancion(int indice)
{
    if (mp3->isRunning())
    {
        rampaGain(0.6, 0.0, 15, 2); // baja en ~30ms, en pasitos
        mp3->stop();
        delay(30);
    }
    if (file)
    {
        delete file;
        file = nullptr;
    }
    Serial.printf("Reproduciendo: %s\n", canciones[indice]);
    file = new AudioFileSourceLittleFS(canciones[indice]);
    out->SetGain(0.0);
    mp3->begin(file, out);
    rampaGain(0.0, 0.6, 15, 2); // sube en ~30ms
}

void setup()
{
    Serial.begin(115200);

    // Genera los nombres "/1.mp3", "/2.mp3", ... y configura los pines
    for (int i = 0; i < NUM_CANCIONES; i++)
    {
        snprintf(canciones[i], sizeof(canciones[i]), "/%d.mp3", i + 1);
        pinMode(pinesBotones[i], INPUT_PULLUP);
    }

    if (!LittleFS.begin(true))
    {
        Serial.println("Error montando LittleFS");
        while (true)
            delay(1000);
    }

    out = new AudioOutputI2S(); // I2S externo (por defecto, sin DAC interno)
    out->SetPinout(PIN_BCK, PIN_LCK, PIN_DIN);
    out->SetGain(0.6); // volumen, 0.0 - 1.0
    out->begin();

    mp3 = new AudioGeneratorMP3();
}

void loop()
{
    for (int i = 0; i < NUM_CANCIONES; i++)
    {
        if (digitalRead(pinesBotones[i]) == LOW)
        {
            if (millis() - ultimoPulso[i] > DEBOUNCE_MS)
            {
                ultimoPulso[i] = millis();
                reproducirCancion(i);
            }
        }
    }

    if (mp3->isRunning())
    {
        if (!mp3->loop())
        {
            mp3->stop();
        }
    }
}
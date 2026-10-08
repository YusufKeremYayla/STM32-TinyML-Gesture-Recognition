/*
 * ai_controller.cpp
 *
 *  Created on: 8 Eki 2026
 *      Author: yusuf
 */

#include "ai_controller.h"
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"
#include <stdio.h>
#include <string.h>

// Artık features_buffer sadece bu dosyanın içinde yaşayacak.

float features_buffer[525];

void AI_Process_Buffer(Circular_Buffer_t *kuyruk)
{
    printf("Veri Doldu! Yapay Zeka Hesaplaniyor...\r\n");
    Sensor_Data_t temp_veri;

    // 1. Kuyruktaki verileri AI formatına (float) çevir!
    for (int i = 0; i < 175; i++)
    {
        Circular_Buffer_Dequeue(kuyruk, &temp_veri);
        features_buffer[(i * 3) + 0] = ((float)temp_veri.x) * 0.0006f;
        features_buffer[(i * 3) + 1] = ((float)temp_veri.y) * 0.0006f;
        features_buffer[(i * 3) + 2] = ((float)temp_veri.z) * 0.0006f;
    }

    signal_t signal;
    numpy::signal_from_buffer(features_buffer, 525, &signal);

    // 2. Modeli Çalıştır
    ei_impulse_result_t result = { 0 };
    EI_IMPULSE_ERROR res = run_classifier(&signal, &result, false);

    if (res == EI_IMPULSE_OK)
    {
        printf("Tahmin Sonuclari:\r\n");

        float max_deger = 0.0f;
        int en_iyi_indis = 0;

        for (int i = 0; i < 4; i++)
        {
            printf(" - %s: %% %.2f\r\n", result.classification[i].label, result.classification[i].value * 100);

            if (result.classification[i].value > max_deger)
            {
                max_deger = result.classification[i].value;
                en_iyi_indis = i;
            }
        }

        float anomali_degeri = 0.0f;
        #if EI_CLASSIFIER_HAS_ANOMALY
        anomali_degeri = result.anomaly;
        printf(" - Anomali Skoru: %.3f\r\n", anomali_degeri);
        #endif

        // 3. Karar ve LED İşlemleri
        if (max_deger > 0.70f && anomali_degeri < 0.30f)
        {
            const char *kazanan = result.classification[en_iyi_indis].label;
            printf(">> KAZANAN HAREKET: %s (%% %.2f) <<\r\n", kazanan, max_deger * 100);

            if (strcmp(kazanan, "circle") == 0)
                HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
            else if (strcmp(kazanan, "left-right") == 0)
                HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
            else if (strcmp(kazanan, "up-down") == 0)
                HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
            else if (strcmp(kazanan, "idle") == 0)
                HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
        }
        else
        {
            if (anomali_degeri >= 0.30f)
                printf(">> HAREKET REDDEDILDI! Anomali Algilandi. Skoru: %.3f <<\r\n", anomali_degeri);
            else
                printf(">> Hareketi tam anlayamadim. (%% %.2f) <<\r\n", max_deger * 100);
        }
    }
}

#include "color_sensor.h"
#include "../led/led.h"
#include "../inc/inc.h"
#include "../color_sensor/i2c_simple_master.h"
#include "../color_sensor/i2c_master.h"

#include "xc.h"
#include <stdio.h>
#include <math.h>
#include <stdint.h>

uint16_t sensor_value_clear;
uint16_t sensor_value_red;
uint16_t sensor_value_green;
uint16_t sensor_value_blue;

typedef struct {
    const char* name;
    uint16_t r, g, b;
} Color;

const Color colors[] = {
    {"ZELENA",     6500, 29000, 28000},
    {"PLAVA",      13800, 45100, 65500},
    {"CRVENA",    40500, 21800, 24400},
    {"ZUTA",      45000, 65535, 40500},
    {"NARANDZASTA",40700, 18800, 14500},
    {"ROZA",      34100, 20200, 29800},
    {"CRNA",       6000, 12300, 16500},
    {"BIJELA",    48900, 65535, 65535},
    {"NO",         15700,  19000,  21000},
    {"OCRNA",      7800,11800,16000}, // zeton okrenut naopacke
    {"OBIJELA",      30800,45900,63900} // zeton okrenut naopacke
};

char* detect_color(){
    
    uint16_t r = sensor_value_red;
    uint16_t g = sensor_value_green;
    uint16_t b = sensor_value_blue;

    float min_distance = 1e9; 
    const char* detected_color = "NEPOZNATA BOJA";

    for (int i = 0; i < NUM_COLORS; i++) {
        float distance = color_distance(r, g, b, colors[i].r, colors[i].g, colors[i].b);
        if (distance < min_distance) {
            min_distance = distance;
            detected_color = colors[i].name;
        }
    }

    return detected_color;
}

void WS2812_Send_Byte(uint8_t byte){
    for (int i = 7; i >= 0; i--)  
    {
        uint8_t spi_data = (byte & (1 << i)) ? 0b11110000 : 0b11000000; 
        SPI1_Exchange8bit(spi_data);
    }
}

void WS2812_SetColor(uint8_t red, uint8_t green, uint8_t blue){
  
    SPI1CON1Lbits.SPIEN = 0;
    __delay_us(50);
    SPI1CON1Lbits.SPIEN = 1;
    
    WS2812_Send_Byte(green);
    WS2812_Send_Byte(red);
    WS2812_Send_Byte(blue);
     
    SPI1CON1Lbits.SPIEN = 0;
    __delay_us(50);
    SPI1CON1Lbits.SPIEN = 1;
      
}

uint16_t VEML3328_readConfig(uint16_t r){    
    uint16_t reg = i2c_read2ByteRegister(VEML3328_SLAVE_ADD, r); 
    return ((reg & 0xFF) << 8) | (reg >> 8);
}

char* read_colors(){
    
    char buff[100];
    uint16_t values_red[NUM_SAMPLES];
    uint16_t values_green[NUM_SAMPLES];
    uint16_t values_blue[NUM_SAMPLES];

    uint32_t sum_red = 0, sum_green = 0, sum_blue = 0;
    uint16_t avg_red, avg_green, avg_blue;

    __delay_ms(1000);

    
    for (int i = 0; i < NUM_SAMPLES; i++) {
        
        sum_red += VEML3328_readConfig(R_DATA);
        sum_green += VEML3328_readConfig(G_DATA);
        sum_blue += VEML3328_readConfig(B_DATA);

        __delay_ms(100);
     
    }

    avg_red = sum_red / NUM_SAMPLES;
    avg_green = sum_green / NUM_SAMPLES;
    avg_blue = sum_blue / NUM_SAMPLES;

    sensor_value_red = avg_red;
    sensor_value_blue = avg_blue;
    sensor_value_green = avg_green;

    return detect_color();
} 

float color_distance(uint16_t r1, uint16_t g1, uint16_t b1, uint16_t r2, uint16_t g2, uint16_t b2){
    
    int32_t dr = (int32_t)r1 - (int32_t)r2;
    int32_t dg = (int32_t)g1 - (int32_t)g2;
    int32_t db = (int32_t)b1 - (int32_t)b2;
    return sqrtf(dr*dr + dg*dg + db*db);
}

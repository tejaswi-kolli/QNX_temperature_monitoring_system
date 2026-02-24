#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/neutrino.h>
#include <sys/mman.h>
#include <sys/dispatch.h>

#include "ipc_protocol.h"

/* ================= GPIO (BCM2711) ================= */

#define GPIO_BASE   0xFE200000
#define GPIO_SIZE   0x1000

#define DHT_PIN     4
#define LED_PIN     16

#define GPFSEL0     0
#define GPSET0      7
#define GPCLR0      10
#define GPLEV0      13

static volatile uint32_t *gpio;

/* ================= TIMING ================= */

static inline void delay_us(int us)
{
    nanospin_ns(us * 1000);
}

/* ================= GPIO HELPERS ================= */

static void gpio_set_output(int pin)
{
    int reg = pin / 10;
    int shift = (pin % 10) * 3;
    gpio[reg] &= ~(7 << shift);
    gpio[reg] |=  (1 << shift);
}

static void gpio_set_input(int pin)
{
    int reg = pin / 10;
    int shift = (pin % 10) * 3;
    gpio[reg] &= ~(7 << shift);
}

static void gpio_write(int pin, int val)
{
    if (val)
        gpio[GPSET0] = (1 << pin);
    else
        gpio[GPCLR0] = (1 << pin);
}

static int gpio_read(int pin)
{
    return (gpio[GPLEV0] & (1 << pin)) ? 1 : 0;
}

/* ================= DHT11 READ ================= */

static int dht11_read(int *temperature)
{
    uint8_t data[5] = {0};
    int i, timeout, bit;

    gpio_set_output(DHT_PIN);
    gpio_write(DHT_PIN, 0);
    usleep(25000);

    gpio_write(DHT_PIN, 1);
    delay_us(40);
    gpio_set_input(DHT_PIN);

    timeout = 20000;
    while (gpio_read(DHT_PIN) && timeout--) delay_us(1);
    timeout = 20000;
    while (!gpio_read(DHT_PIN) && timeout--) delay_us(1);
    timeout = 20000;
    while (gpio_read(DHT_PIN) && timeout--) delay_us(1);

    for (i = 0; i < 40; i++) {
        timeout = 20000;
        while (!gpio_read(DHT_PIN) && timeout--) delay_us(1);

        delay_us(35);
        bit = gpio_read(DHT_PIN);

        data[i / 8] <<= 1;
        if (bit) data[i / 8] |= 1;

        timeout = 20000;
        while (gpio_read(DHT_PIN) && timeout--) delay_us(1);
    }

    if ((uint8_t)(data[0] + data[1] + data[2] + data[3]) != data[4])
        return -1;

    *temperature = data[2];
    return 0;
}

/* ================= MAIN ================= */

int main(void)
{
    int coid, temperature;
    sensor_msg_t msg;

    ThreadCtl(_NTO_TCTL_IO, NULL);

    gpio = mmap_device_memory(
        NULL,
        GPIO_SIZE,
        PROT_READ | PROT_WRITE | PROT_NOCACHE,
        MAP_SHARED,
        GPIO_BASE
    );

    gpio_set_output(LED_PIN);
    gpio_write(LED_PIN, 0);

    while ((coid = name_open(LOGGER_ATTACH_POINT, 0)) == -1) {
        printf("[Sensor] Waiting for logger...\n");
        sleep(1);
    }

    printf("[Sensor] Started\n");

    while (1) {
        if (dht11_read(&temperature) == 0) {
            msg.type = MSG_SENSOR_DATA;
            msg.temperature = temperature;

            printf("[Sensor] Temp = %d C\n", temperature);
            MsgSend(coid, &msg, sizeof(msg), NULL, 0);
        }
        sleep(2);
    }
}


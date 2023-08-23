#include "ringbuffer.h"
#include <stdio.h>

int main()
{
    RingBuffer *buffer;
    buffer = RingBuffer_create(10);
    printf("buffer:");
    for (int i = 0; i < 10; i++)
        printf("%d ", buffer->buffer[i]);
    printf("%d\n", buffer->buffer[10]);
    printf("start:%d end:%d data:%d space:%d\n", buffer->start, buffer->end, RingBuffer_available_data(buffer), RingBuffer_available_space(buffer));
    printf("\n");

    char data[5] = {1, 2, 3, 4, 5};
    char target[5] = {'\0', '\0', '\0', '\0', '\0'};

    RingBuffer_write(buffer, data, 5);
    printf("buffer:");
    for (int i = 0; i < 10; i++)
        printf("%d ", buffer->buffer[i]);
    printf("%d\n", buffer->buffer[10]);
    printf("start:%d end:%d data:%d space:%d\n", buffer->start, buffer->end, RingBuffer_available_data(buffer), RingBuffer_available_space(buffer));
    printf("\n");

    // RingBuffer_read(buffer, target, 2);
    // printf("target:");
    // for (int i = 0; i < 4; i++)
    //     printf("%d ", target[i]);
    // printf("%d\n", target[4]);
    // printf("buffer:");
    // for (int i = 0; i < 10; i++)
    //     printf("%d ", buffer->buffer[i]);
    // printf("%d\n", buffer->buffer[10]);
    // printf("start:%d end:%d data:%d space:%d\n", buffer->start, buffer->end, RingBuffer_available_data(buffer), RingBuffer_available_space(buffer));
    // printf("\n");

    RingBuffer_write(buffer, data, 5);
    printf("buffer:");
    for (int i = 0; i < 10; i++)
        printf("%d ", buffer->buffer[i]);
    printf("%d\n", buffer->buffer[10]);
    printf("start:%d end:%d data:%d space:%d\n", buffer->start, buffer->end, RingBuffer_available_data(buffer), RingBuffer_available_space(buffer));
    printf("\n");

    RingBuffer_read(buffer, target, 5);
    printf("target:");
    for (int i = 0; i < 4; i++)
        printf("%d ", target[i]);
    printf("%d\n", target[4]);
    printf("buffer:");
    for (int i = 0; i < 10; i++)
        printf("%d ", buffer->buffer[i]);
    printf("%d\n", buffer->buffer[10]);
    printf("start:%d end:%d data:%d space:%d\n", buffer->start, buffer->end, RingBuffer_available_data(buffer), RingBuffer_available_space(buffer));
    printf("\n");

    // RingBuffer_write(buffer, data, 2);
    // printf("buffer:");
    // for (int i = 0; i < 10; i++)
    //     printf("%d ", buffer->buffer[i]);
    // printf("%d\n", buffer->buffer[10]);
    // printf("start:%d end:%d data:%d space:%d\n", buffer->start, buffer->end, RingBuffer_available_data(buffer), RingBuffer_available_space(buffer));
    // printf("\n");

    RingBuffer_read(buffer, target, 5);
    printf("target:");
    for (int i = 0; i < 4; i++)
        printf("%d ", target[i]);
    printf("%d\n", target[4]);
    printf("buffer:");
    for (int i = 0; i < 10; i++)
        printf("%d ", buffer->buffer[i]);
    printf("%d\n", buffer->buffer[10]);
    printf("start:%d end:%d data:%d space:%d\n", buffer->start, buffer->end, RingBuffer_available_data(buffer), RingBuffer_available_space(buffer));
    printf("\n");

    RingBuffer_destroy(buffer);

    return 0;
}
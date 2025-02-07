#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

typedef struct log {
    char name[15];
    char time[9];
    char date[9];
    char t[3];
    char status[4];
} log;

typedef struct data {
    char id[13];
    char name[15];
} data;

int sc(char *p, char *q) {
    while (*p) {
        if (*p != *q)
            return 0;
        p++, q++;
    }
    return 1;
}

int main() {
    while (1) {
        int serial_port = open("/dev/ttyUSB0", O_RDWR);
        struct termios tty;

        if (tcgetattr(serial_port, &tty) != 0) {
            printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
            return 1;
        }

        tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
        tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
        tty.c_cflag &= ~CSIZE;  // Clear all bits that set the data size
        tty.c_cflag |= CS8;     // 8 bits per byte (most common)
        tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
        tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
        tty.c_lflag &= ~ICANON;
        tty.c_lflag &= ~ECHO;    // Disable echo
        tty.c_lflag &= ~ECHOE;   // Disable erasure
        tty.c_lflag &= ~ECHONL;  // Disable new-line echo
        tty.c_lflag &= ~ISIG;    // Disable interpretation of INTR, QUIT and SUSP
        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
        tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes
        tty.c_oflag &= ~OPOST;   // Prevent special interpretation of output bytes (e.g. newline chars)
        tty.c_oflag &= ~ONLCR;   // Prevent conversion of newline to carriage return/line feed
        tty.c_cc[VTIME] = 10;
        tty.c_cc[VMIN] = 120;
        cfsetispeed(&tty, B9600);
        cfsetospeed(&tty, B9600);

        if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
            printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
            return 1;
        }

        char read_buf[256];
        memset(&read_buf, '\0', sizeof(read_buf));
        int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

        if (num_bytes < 0) {
            printf("Error reading: %s", strerror(errno));
            return 1;
        }

        /////////////////////////////////////
        int i, f, c = 0;
        data *new;
        new = malloc(sizeof(data));
        FILE *fp = fopen("data", "r");

        while (1) {
            if ((fscanf(fp, "%s %s", new->id, new->name)) == -1)
                break;

            for (i = 0, f = 1; i < 12; i++) {
                if (read_buf[i] != new->id[i]) {
                    f = 0;
                    break;
                }
            }

            if (f) {
                log *dp, *temp;
                dp = malloc(sizeof(log));
                temp = malloc(sizeof(log));
                FILE *FP = fopen("log", "a+");

                strcpy(dp->name, new->name);

                dp->time[0] = read_buf[13];
                dp->time[1] = read_buf[14];
                dp->time[2] = read_buf[15];
                dp->time[3] = read_buf[16];
                dp->time[4] = read_buf[17];
                dp->time[5] = read_buf[18];
                dp->time[6] = '\0';

                dp->t[0] = read_buf[20];
                dp->t[1] = read_buf[21];
                dp->t[2] = '\0';

                dp->date[0] = read_buf[23];
                dp->date[1] = read_buf[24];
                dp->date[2] = read_buf[25];
                dp->date[3] = read_buf[26];
                dp->date[4] = read_buf[27];
                dp->date[5] = read_buf[28];
                dp->date[6] = read_buf[29];
                dp->date[7] = read_buf[30];

                c = 0;

                while (1) {
                    if ((fscanf(FP, "%s %s %s %s %s", temp->name, temp->time, temp->t, dp->date, temp->status)) == -1)
                        break;

                    if (sc(temp->name, dp->name))
                        c++;
                }

                if (c % 2 == 0) {
                    c = 0;
                    dp->status[0] = 'I';
                    dp->status[1] = 'N';
                    dp->status[2] = '\0';
                    fprintf(FP, "%s %s %s %s %s\n", dp->name, dp->time, dp->t, dp->date, dp->status);
                    printf("%s %s %s %s %s\n", dp->name, dp->time, dp->t, dp->date, dp->status);
                } else {
                    c = 0;
                    dp->status[0] = 'O';
                    dp->status[1] = 'U';
                    dp->status[2] = 'T';
                    dp->status[3] = '\0';
                    fprintf(FP, "%s %s %s %s %s\n", dp->name, dp->time, dp->t, dp->date, dp->status);
                    printf("%s %s %s %s %s\n", dp->name, dp->time, dp->t, dp->date, dp->status);
                }

                fclose(FP);
            }
        }

        /////////////////////////////////////

        fclose(fp);
        close(serial_port);
    }
}


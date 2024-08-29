#ifndef SET_FD_H
#define SET_FD_H

int setnonblocking(int fd);
void addfd(int epollfd, int fd, bool one_shot, int TRIGMode);
void removefd(int epollfd, int fd);
void modfd(int epollfd, int fd, int ev, int TRIGMode);

#endif
/*
Copyright (c) 2009-2010 by Pejman Attar, Yoann Canal, Juliusz Chroboczek and
                      Gabriel Kerneis

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <sys/socket.h>

extern int debug_level;
extern size_t pagesize;

#define LARGE_CHUNK (1024*1024)

#if defined(i386) || defined(__mc68020__) || defined(__x86_64__)
#define DO_NTOHS(_d, _s) do { _d = ntohs(*(unsigned short*)(_s)); } while(0)
#define DO_NTOHL(_d, _s) do { _d = ntohl(*(unsigned*)(_s)); } while(0)
#define DO_HTONS(_d, _s) do { *(unsigned short*)(_d) = htons(_s); } while(0)
#define DO_HTONL(_d, _s) do { *(unsigned*)(_d) = htonl(_s); } while(0)
#else
#define DO_NTOHS(_d, _s) \
    do { unsigned short _dd; \
         memcpy(&(_dd), (_s), sizeof(unsigned short)); \
         _d = ntohs(_dd); } while(0)
#define DO_NTOHL(_d, _s) \
    do { unsigned _dd; \
         memcpy(&(_dd), (_s), sizeof(unsigned)); \
         _d = ntohl(_dd); } while(0)
#define DO_HTONS(_d, _s) \
    do { unsigned short _dd; \
         _dd = htons(_s); \
         memcpy((_d), &(_dd), sizeof(unsigned short)); } while(0);
#define DO_HTONL(_d, _s) \
    do { unsigned _dd; \
         _dd = htonl(_s); \
         memcpy((_d), &(_dd), sizeof(unsigned)); } while(0);
#endif

void debugf(int level, const char *format, ...) CPC_NO_RETAIN_ATTRIBUTE;
int prefetch(void *begin, size_t length);
int incore(void *begin, size_t length);
int get_source_address(const struct sockaddr *dst, socklen_t dst_len,
                       struct sockaddr *src, socklen_t *src_len);
int get_name_source_address(int af, const char *name,
                            struct sockaddr *src, socklen_t *src_len);
int global_unicast_address(struct sockaddr *sa);
int find_global_address(int af, void *addr, int *addr_len) CPC_NO_RETAIN_ATTRIBUTE;


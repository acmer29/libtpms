/********************************************************************************/
/*										*/
/*		   LibTPM internal interface functions				*/
/*                        Written by Stefan Berger				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*	      $Id: tpm_library_intern.h 4432 2011-02-11 15:30:31Z kgoldman $		*/
/*										*/
/* (c) Copyright IBM Corporation 2011.						*/
/*										*/
/* All rights reserved.								*/
/* 										*/
/* Redistribution and use in source and binary forms, with or without		*/
/* modification, are permitted provided that the following conditions are	*/
/* met:										*/
/* 										*/
/* Redistributions of source code must retain the above copyright notice,	*/
/* this list of conditions and the following disclaimer.			*/
/* 										*/
/* Redistributions in binary form must reproduce the above copyright		*/
/* notice, this list of conditions and the following disclaimer in the		*/
/* documentation and/or other materials provided with the distribution.		*/
/* 										*/
/* Neither the names of the IBM Corporation nor the names of its		*/
/* contributors may be used to endorse or promote products derived from		*/
/* this software without specific prior written permission.			*/
/* 										*/
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS		*/
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT		*/
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR	*/
/* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT		*/
/* HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,	*/
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT		*/
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,	*/
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY	*/
/* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT		*/
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE	*/
/* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.		*/
/********************************************************************************/
#ifndef TPM_LIBRARY_INTERN_H
#define TPM_LIBRARY_INTERN_H

#include <stdbool.h>
#include "tpm_library.h"

#define ROUNDUP(VAL, SIZE) \
  ( ( (VAL) + (SIZE) - 1 ) / (SIZE) ) * (SIZE)

struct libtpms_callbacks *TPMLIB_GetCallbacks(void);

/*
 * TPM functionality must all be accessible with this interface
 */
struct tpm_interface {
    TPM_RESULT (*MainInit)(void);
    void (*Terminate)(void);
    uint32_t (*SetBufferSize)(uint32_t wanted_size, uint32_t *min_size,
                              uint32_t *max_size);
    TPM_RESULT (*Process)(unsigned char **respbuffer, uint32_t *resp_size,
                          uint32_t *respbufsize,
		          unsigned char *command, uint32_t command_size);
    TPM_RESULT (*VolatileAllStore)(unsigned char **buffer, uint32_t *buflen);
    TPM_RESULT (*GetTPMProperty)(enum TPMLIB_TPMProperty prop,
                                 int *result);
    char *(*GetInfo)(enum TPMLIB_InfoFlags flags);
    TPM_RESULT (*TpmEstablishedGet)(TPM_BOOL *tpmEstablished);
    TPM_RESULT (*HashStart)(void);
    TPM_RESULT (*HashData)(const unsigned char *data,
                           uint32_t data_length);
    TPM_RESULT (*HashEnd)(void);
    TPM_RESULT (*ValidateState)(enum TPMLIB_StateType st,
                                unsigned int flags);
    TPM_RESULT (*SetState)(enum TPMLIB_StateType st,
                           const unsigned char *buffer, uint32_t buflen);
    TPM_RESULT (*GetState)(enum TPMLIB_StateType st,
                           unsigned char **buffer, uint32_t *buflen);
};

extern const struct tpm_interface TPM12Interface;

/* prototypes for TPM 1.2 */
TPM_RESULT TPM12_IO_Hash_Start(void);
TPM_RESULT TPM12_IO_Hash_Data(const unsigned char *data,
			      uint32_t data_length);
TPM_RESULT TPM12_IO_Hash_End(void);
TPM_RESULT TPM12_IO_TpmEstablished_Get(TPM_BOOL *tpmEstablished);

uint32_t TPM12_GetBufferSize(void);

/* internal logging function */
int TPMLIB_LogPrintf(const char *format, ...);
void TPMLIB_LogPrintfA(unsigned int indent, const char *format, ...);

#define TPMLIB_LogError(format, ...) \
     TPMLIB_LogPrintfA(~0, "libtpms: "format, __VA_ARGS__)
#define TPMLIB_LogTPM12Error(format, ...) \
     TPMLIB_LogPrintfA(~0, "libtpms/tpm12: "format, __VA_ARGS__)

struct sized_buffer {
    unsigned char *buffer;
    uint32_t buflen;
#define BUFLEN_EMPTY_BUFFER 0xFFFFFFFF
};

void ClearCachedState(enum TPMLIB_StateType st);
void ClearAllCachedState(void);
void SetCachedState(enum TPMLIB_StateType st,
                    unsigned char *buffer, uint32_t buflen);
void GetCachedState(enum TPMLIB_StateType st,
                    unsigned char **buffer, uint32_t *buflen,
                    bool *is_empty_buffer);
bool HasCachedState(enum TPMLIB_StateType st);
TPM_RESULT CopyCachedState(enum TPMLIB_StateType st,
                           unsigned char **buffer, uint32_t *buflen,
                           bool *is_empty_buffer);

const char *TPMLIB_StateTypeToName(enum TPMLIB_StateType st);
enum TPMLIB_StateType TPMLIB_NameToStateType(const char *name);

#endif /* TPM_LIBRARY_INTERN_H */

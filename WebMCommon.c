// Copyright (c) 2010 The WebM project authors. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS.  All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.


#include "WebMCommon.h"

void initBuffer(WebMBuffer *buf)
{
    buf->data = NULL;
    buf->size = 0;
    buf->offset = 0;
}

int freeBuffer(WebMBuffer *buf)
{
    if (buf->data != NULL)
        free(buf->data);

    initBuffer(buf);
    return 0;
}

int allocBuffer(WebMBuffer *buf, size_t size)
{
    int iFreed = freeBuffer(buf);

    if (iFreed)
        return iFreed;

    buf->data = malloc(size);

    if (buf->data == NULL)
        return 1;

    buf->size = size;
    buf->offset = 0;
}

void initMovieGetParams(StreamSource *get)
{
    get->params.recordSize = sizeof(MovieExportGetDataParams);
    get->params.trackID = get->trackID;
    get->params.requestedTime = get->time;
    get->params.sourceTimeScale = get->timeScale;
    get->params.actualTime = 0;
    get->params.dataPtr = NULL;
    get->params.dataSize = 0;
    get->params.desc = NULL;
    get->params.descType = 0;
    get->params.descSeed = 0;
    get->params.requestedSampleCount = 0;
    get->params.actualSampleCount = 0;
    get->params.durationPerSample = 1;
    get->params.sampleFlags = 0;
}

void dbg_printDataParams(StreamSource *get)
{
    MovieExportGetDataParams *p = &get->params;
    dbg_printf("[webM]  Data Params  %ld [%ld]"
               " %ld [%ld] [%ld @ %ld] %ld '%4.4s'\n",
               p->requestedSampleCount,
               p->actualSampleCount, p->requestedTime,
               p->actualTime, p->durationPerSample,
               p->sourceTimeScale, p->dataSize,
               (char *) &p->descType);
}

ComponentResult initStreamSource(StreamSource *source, TimeScale scale,
                                 long trackID, MovieExportGetPropertyUPP propertyProc,
                                 MovieExportGetDataUPP getDataProc, void *refCon)
{
    ComponentResult err = noErr;

    dbg_printf("[WebM] InitStreamSource %d timescale = %d\n", trackID, scale);

    source->trackID = trackID;
    source->propertyProc = propertyProc;
    source->dataProc = getDataProc;
    source->refCon = refCon;
    source->timeScale = scale;

    memset(&source->params, 0, sizeof(MovieExportGetDataParams));
    source->params.recordSize = sizeof(MovieExportGetDataParams);
    source->params.trackID = source->trackID;
    source->params.sourceTimeScale = scale;
    source->eos = false;
    source->bQdFrame = false;

    return err;
}

double getTimeAsSeconds(StreamSource *source)
{
    double val = (source->time * 1.0) / (source->timeScale * 1.0);
    return val;
}